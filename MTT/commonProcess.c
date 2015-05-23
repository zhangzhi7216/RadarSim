/*
***************************************
件名    ：commonProcess.c
描述    ：公共处理函数
创建时间：2012.04.14
版本    ：1.0
版权    ：
*
***************************************
*/

/*
***************************************
*            头文件引用               *
***************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "globalParameter.h"
#include "structDefinition.h"
#include "fusionFilter.h"
#include "commonProcess.h"
#include "matrixOperation.h"

/*
***************************************
*          全局变量声明               *
***************************************
*/
ECHO sEcho_RadarAIS[MAX_ECHO];
ECHO sEcho_TongLei[MAX_ECHO];
ECHO sEcho_MTT[2*MAX_ECHO];        

ECHO_PARAMETER sECHO_Param;
ECHO sEcho_SelfPosition[MAX_ECHO];

TRACKINFO sTracks[MAX_TRACK];
TRACKINFO sTrkTmp[MAX_TRACK];

MEASINFO sEcho_DataFusion;

int g_iTrkNum = 0;
int iEchoNum_RadarAIS = 0;
int iEchoNum_TongLei = 0;
int iEchoNum_Total = 0;
double ShareA[MAX_TRACK][Share_N] = {0};
double ShareB[MAX_TRACK][Share_N] = {0};
double ShareC[MAX_TRACK][Share_N] = {0};
int Spt = 1;
/* sMdlPara用于储存航迹模型参数，当前包括CV，CA1，CA2三种模型  */
MODELPARA pMdl[MODEL_NUM];
/*
***************************************
*          外部变量声明               *
***************************************
*/

/*
***************************************
*         本文件内的函数定义          *
***************************************
*/

/*
***************************************
函数名  ：Initialize_Models
功能描述：滤波模型参数初始化（CV+CA1+CA2）
一为匀速模型CV
二为低过程噪声的匀加速模型CA1
三维高过程噪声的匀加速模型CA2
参数    ：pMdl，指向滤波模型参数的指针
返回值  ：无
***************************************
*/
void Initialize_Models(MODELPARA *pMdl)
{
	/* T为采样间隔 */
	double T = SAMPLE_PERIOD;

	/* fCV3Fk为单维滤波时CV模型的系统转移矩阵
	* fCV3Qk为单维滤波时CV模型的过程噪声矩阵
	* fCV3Pk为单维滤波时CV模型的初始协方差阵 */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4),0.5*pow(T,3),0,0.5*pow(T,3),pow(T,2),0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,90000,0,0,0,0};

	/* fCA3Fk为单维滤波时CA模型的系统转移矩阵
	* fCA3Qk为单维滤波时CA模型的过程噪声矩阵
	* fCA3Pk为单维滤波时CA模型的初始协方差阵 */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	double fCA3Qk[9] = {pow(T,5)/20,pow(T,4)/8,pow(T,3)/6,pow(T,4)/8,pow(T,3)/6,pow(T,2)/2,pow(T,3)/6,pow(T,2)/2,T};
	double fCA3Pk[9] = {9000000,0,0,0,90000,0,0,0,900};

	/* 量测矩阵 */
	double fHk[3][9];

	/* 临时变量 */
	double fTmp[9];

	/* 初始化Hk矩阵 */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* 初始化模型类型，如果是CV模型，则置1；否则为0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* 初始化CV模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* 初始化CA1模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* 初始化CA2模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);
}

/*
***************************************
函数名   ：Select_Measure
功能描述 ：从帧量测数据中选取属于传感器iSnsIdx第iTgtIdx个目标的量测信息
参数     ：pMeas，指向量测数据文件的指针
iTgtIdx，航迹号，范围为[0,TARGET_NUM)
iSnsIdx，传感器号,范围为[0,SENSOR_NUM)；等于-1时表示需要选取所有传感器的量测信息
返回值   ：pTgtZk,指向当前帧某一目标的所有量测信息
***************************************
*/
MEASINFO* Select_Measure(MEASINFO *pMeas,int iTgtIdx,int iSnsIdx,int *iSnrNum)
{
	/* pTgtZk,指向航迹的量测信息;pTmpZk和pNxZk,临时变量 */
	MEASINFO *pTgtZk,*pTmpZk,*pNxZk;

	/* 变量初始化 */
	*iSnrNum = 0;
	pTgtZk = NULL;
	pTmpZk = NULL;
	pNxZk = pMeas;

	while (pNxZk != NULL){

		if (iSnsIdx == -1){
			/* iSnsIdx等于-1，表示选取目标的所有传感器获得的量测信息 */
			if(pNxZk->tidx == iTgtIdx){
				(*iSnrNum)++;

				pTmpZk = (MEASINFO *)malloc(sizeof(MEASINFO));
				Assign_MeasInfo(pTmpZk,pNxZk);

				pTmpZk->next = pTgtZk;
				pTgtZk = pTmpZk;
			}
		}else{
			/* 其它情况，则选择对应的传感器量测信息 */
			if(pNxZk->tidx == iTgtIdx && pNxZk->sidx == iSnsIdx){
				(*iSnrNum)++;

				pTmpZk = (MEASINFO *)malloc(sizeof(MEASINFO));
				Assign_MeasInfo(pTmpZk,pNxZk);

				pTmpZk->next = pTgtZk;
				pTgtZk = pTmpZk;
			}
		}

		/* 指向下一个量测信息 */
		pNxZk = pNxZk->next;
	}

	return pTgtZk;
}
/*
***************************************
函数名   ：Assign_MeasInfo
功能描述 ：量测信息赋值
参数     ：pLeft，指向量测信息左值
pRight，指向量测信息右值
返回值   ：无
***************************************
*/
void Assign_MeasInfo(MEASINFO *pLeft,MEASINFO *pRight)
{
	/* 临时计数变量 */
	int i;

	/* 传感器号和航迹好赋值 */
	pLeft->sidx = pRight->sidx;
	pLeft->tidx = pRight->tidx;

	/* 量测和噪声信息赋值 */
	for (i=0; i<MEAS_DIM; i++){
		pLeft->meas[i] = pRight->meas[i];
		pLeft->nstd[i] = pRight->nstd[i];
	}

	/* 下一量测指针赋值 */
	pLeft->next = pRight->next;
}

/*
***************************************
函数名   ：Release_MeasInfo
功能描述 ：释放量测信息
参数     ：pMeas，指向需要释放的量测信息
返回值   ：无
***************************************
*/
void Release_MeasInfo(MEASINFO *pMeas)
{
	MEASINFO *pMs;
	pMs = pMeas;

	/* 循环释放量测信息所占的内存空间 */
	while(pMs != NULL){
		pMs = pMs->next;
		free(pMeas);
		pMeas = pMs;
	}
}

/*
***************************************
函数名   ：Initialize_State
功能描述 ：初始化滤波状态
参数     ：pTrk，航迹信息
pMeas，量测信息
pMdl，模型信息
iSpt，帧号
返回值   ：无
***************************************
*/
void Initialize_State(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSpt)
{
	/* 临时计数变量 */
	int i,iCnt;

	/* fXk临时变量，储存状态信息；fAvg临时变量，储存状态信息均值 */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* 量测信息 */
	MEASINFO *pZk;

	/* 初始化临时变量 */
	iCnt = 0;
	pZk = pMeas;

	/* 取平均值 */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* 各个传感器状态信息求和 */
	while(pZk != NULL){
		iCnt++;

		for (i=0; i<MEAS_DIM; i++){
			fAvg[i] += pZk->meas[i];
		}
		pZk = pZk->next;
	}

	/* 获得状态信息均值 */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] /= iCnt;
	}

	/* 初始化临时变量 */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}

	/* 初始化状态信息 */
	if (iSpt == 0){
		/* 第一帧，初始化位置信息 */
		fXk[0] = fAvg[0];
		fXk[3] = fAvg[1];
		fXk[6] = fAvg[2];
	}else{
		/* 第二帧，初始化位置信息和速度信息 */
		fXk[0] = fAvg[0];
		fXk[3] = fAvg[1];
		fXk[6] = fAvg[2];

		fXk[1] = (fAvg[0] - pTrk->fsn.xk[0])/SAMPLE_PERIOD;
		fXk[4] = (fAvg[1] - pTrk->fsn.xk[3])/SAMPLE_PERIOD;
		fXk[7] = (fAvg[2] - pTrk->fsn.xk[6])/SAMPLE_PERIOD;
	}

	/* 初始化航迹号 */
	pTrk->idx = pMeas->tidx;

	/* 初始化模型概率 */
	pTrk->fMdlProb[0] = 0.4;
	pTrk->fMdlProb[1] = 0.3;
	pTrk->fMdlProb[2] = 0.3;

	/* Xk与Pk阵的初始化 */
	for (i=0; i<MODEL_NUM; i++){
		/* 各个模型Xk和Pk的初始化 */
		MtrAssign(1,STATE_DIM,fXk,pTrk->imm[i].xk);
		MtrAssign(STATE_DIM,STATE_DIM,pMdl[i].pk[0],pTrk->imm[i].pk[0]);
	}

	/* 融合中心状态初始化 */
	MtrAssign(1,STATE_DIM,fXk,pTrk->fsn.xk);

}

/*
***************************************
函数名   ：Assign_Matrix2Block
功能描述 ：将整个矩阵pRight赋给pLeft的特定块上，该块的维数与pRight矩阵相同
参数     ：iRowBeg,初始行号
iRowFin，结束行号+1
iColBeg，初始列号
iColFin，结束列号+1
iLCol，左矩阵的列维数
iRCol，右矩阵的列维数
pLeft，左值矩阵，即待被赋值的矩阵
pRight，右值矩阵，即要赋给左值的矩阵
返回值   ：无
***************************************
*/
void Assign_Matrix2Block(int iRowBeg,int iRowFin,int iColBeg,int iColFin,int iLCol,int iRCol,double *pLeft,double *pRight)
{
	int i,j;
	int iLeft,iRight;

	for (i=iRowBeg; i<iRowFin; i++){
		for (j=iColBeg; j<iColFin; j++){
			/* 计算左右矩阵的对应下标号 */
			iLeft = i*iLCol + j;
			iRight = (i-iRowBeg)*iRCol + (j-iColBeg);

			pLeft[iLeft] = pRight[iRight];
		}
	}
}

/*
***************************************
函数名   ：Assign_Block2Block
功能描述 ：将矩阵pRight的特定块赋给pLeft的特定块上,考虑矩阵与矩阵不同分块的赋值
参数     ：sLeft,左矩阵的块信息
sRight，右矩阵块信息
pLeft，左值矩阵，即待被赋值的矩阵
pRight，右值矩阵，即要赋给左值的矩阵
返回值   ：无
***************************************
*/
void Assign_Block2Block(BMINFO sLeft,BMINFO sRight,double *pLeft,double *pRight)
{
	int i,j;
	int iLeft,iRight;

	for (i=sLeft.iRowBeg; i<sLeft.iRowFin; i++){
		for (j=sLeft.iColBeg; j<sLeft.iColFin; j++){
			/* 计算左右矩阵的对应下标号 */
			iLeft = i*sLeft.iColDim + j;
			iRight = (sRight.iRowBeg + i -sLeft.iRowBeg) * sRight.iColDim + (sRight.iColBeg + j -sLeft.iColBeg) ;

			pLeft[iLeft] = pRight[iRight];
		}
	}
}


/*
***************************************
函数名   ：Assign_FltInfo
功能描述 ：滤波信息赋值
参数     ：pLeft，指向需要被赋值滤波信息
pRight，指向赋值滤波信息
返回值   ：无
***************************************
*/
void Assign_FltInfo(FLTINFO *pLeft,FLTINFO *pRight)
{
	MtrAssign(STATE_DIM,1,pRight->xk,pLeft->xk);
	MtrAssign(STATE_DIM,STATE_DIM,pRight->pk[0],pLeft->pk[0]);
}

/*
***************************************
函数名   ：Display_TrackInfo
功能描述 ：显示航迹信息
参数     ：pTrk，航迹信息指针
返回值   ：无
***************************************
*/
void Display_TrackInfo(TRACKINFO *pTrk)
{
	int i,j;

	for (i=0; i<TARGET_NUM; i++){
		printf("---------航迹号：%d-----------\n",pTrk[i].idx);

		printf("各个模型的概率分别是：");
		for (j=0; j<MODEL_NUM; j++){
			printf("%f\t",pTrk[i].fMdlProb[j]);
		}
		printf("\n");

		printf("当前融合估计的状态：");
		for (j=0; j<STATE_DIM; j++){
			printf("%f\t",pTrk[i].fsn.xk[j]);
		}
		printf("\n");
	}
}

/*
***************************************
函数名   ：Display_MeasInfo
功能描述 ：显示量测信息
参数     ：pTrk，量测信息指针
返回值   ：无
***************************************
*/
void Display_MeasInfo(MEASINFO *pMeas)
{
	MEASINFO *pTmp;
	pTmp = pMeas;

	while(pTmp != NULL){
		printf("传感器号%d，航迹号%d\n",pTmp->sidx,pTmp->tidx);
		printf("量测：%f\t%f\t%f\n",pTmp->meas[0],pTmp->meas[1],pTmp->meas[2]);
		printf("量测：%f\t%f\t%f\n",pTmp->nstd[0],pTmp->nstd[1],pTmp->nstd[2]);

		pTmp = pTmp->next;
	}

}

/*
***************************************
函数名   ：Display_DblMatrix
功能描述 ：显示浮点型矩阵
参数     ：pMtr，指向矩阵
iRow，矩阵行数
iCol，矩阵列数
返回值   ：无
***************************************
*/
void Display_DblMatrix(double *pMtr,int iRow,int iCol)
{
	int i,j;

	for (i=0; i<iRow; i++){
		for (j=0; j<iCol; j++){
			printf("%f\t",pMtr[i*iCol + j]);
		}
		printf("\n");
	}
}

/*
***************************************
函数名   ：Display_IntMatrix
功能描述 ：显示整型矩阵
参数     ：pMtr，指向矩阵
iRow，矩阵行数
iCol，矩阵列数
返回值   ：无
***************************************
*/
void Display_IntMatrix(int *pMtr,int iRow,int iCol)
{
	int i,j;

	for (i=0; i<iRow; i++){
		for (j=0; j<iCol; j++){
			printf("%d\t",pMtr[i*iCol + j]);
		}
		printf("\n");
	}
}

/*
***************************************
函数名  ：MTTFusion_Decision
功能描述：航迹融合与否判定
参数    ：
pRight，
返回值  ：无
***************************************
*/
void MTTORFusion_Decision()
{
	int i,j;
	int iIdx = 0;
	ECHO *pEcho = NULL;
	double Relative_XYZ[3] = {0};
	double *Tmp1 = NULL;
	double *Tmp2 = NULL;
	double Tmp3, Tmp4;
	double Tmp5 = iEchoNum_TongLei;
	int iflag = 0;

	for (i=0;i<Tmp5;i++){
		pEcho = &sEcho_TongLei[i];		
		// 数据链内的绝对球坐标系下位置
/*		Tmp1 = PolToDesc(pEcho->msr);						   // 数据链内的绝对笛卡尔坐标系下位置
		Relative_XYZ[0] = Tmp1[0] - sEcho_SelfPosition[0].msr[0]; // 数据链内的相对于我机的笛卡尔坐标系下位置
		Relative_XYZ[1] = Tmp1[1] - sEcho_SelfPosition[0].msr[1];
		Relative_XYZ[2] = Tmp1[2] - sEcho_SelfPosition[0].msr[2];

		Tmp2 = DescToPol(Relative_XYZ);							// 数据链内的相对于我机的球面坐标系下位置
		if ( ( Tmp2[0] <= sECHO_Param.Radar_RgeScope ) && ( Tmp2[1] <= sECHO_Param.Radar_Azi_MaxScope) &&( Tmp2[1] >= -sECHO_Param.Radar_Azi_MaxScope) && ( Tmp2[2] <= sECHO_Param.Radar_Ele_MaxScope) && ( Tmp2[2] >= -sECHO_Param.Radar_Ele_MaxScope) ){
			sEcho_TongLei[i].Fusion_OrNot = 1;
		} 
		free(Tmp1);
		free(Tmp2); */
		iflag = 0;
		for (j=0;j<iEchoNum_RadarAIS;j++){
			Tmp3 = fabs(pEcho->msr[1] - sEcho_RadarAIS[j].msr[1]);
			Tmp4 = fabs(pEcho->msr[2] - sEcho_RadarAIS[j].msr[2]);
			if ( (Tmp3 <= 0.00001) && (Tmp4 <= 0.08) ){
				pEcho->msr[0] = sEcho_RadarAIS[j].msr[0];
				sEcho_TongLei[i].Fusion_OrNot = 1;
				iflag = 1;
				break;
			}
			//		printf("ESM和TongLei数据%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],i);
		}

		if ( iflag == 0){
			for ( j=i; j<Tmp5;j++ ){
				sEcho_TongLei[j].msr[0] = sEcho_TongLei[j+1].msr[0];
				sEcho_TongLei[j].msr[1] = sEcho_TongLei[j+1].msr[1];
				sEcho_TongLei[j].msr[2] = sEcho_TongLei[j+1].msr[2];
				sEcho_TongLei[j].Fusion_OrNot = sEcho_TongLei[j+1].Fusion_OrNot;
			}
			iEchoNum_TongLei--;
		}
	}


	/*          加完标识后， 将ESM数据和TongLei数据合并在一起 方便后面处理 */
	for (i=0;i<iEchoNum_RadarAIS;i++){
		sEcho_MTT[i].msr[0] = sEcho_RadarAIS[i].msr[0];
		sEcho_MTT[i].msr[1] = sEcho_RadarAIS[i].msr[1];
		sEcho_MTT[i].msr[2] = sEcho_RadarAIS[i].msr[2];
		sEcho_MTT[i].Type = sEcho_RadarAIS[i].Type;

		sEcho_MTT[i].Fusion_OrNot = sEcho_RadarAIS[i].Fusion_OrNot;

		//		printf("ESM和TongLei数据%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],i);
	}

	for (j=0;j<iEchoNum_TongLei;j++){
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[0] = sEcho_TongLei[j].msr[0];
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[1] = sEcho_TongLei[j].msr[1];
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[2] = sEcho_TongLei[j].msr[2];
		sEcho_MTT[j+iEchoNum_RadarAIS].Type = sEcho_TongLei[j].Type;

		sEcho_MTT[j+iEchoNum_RadarAIS].Fusion_OrNot = sEcho_TongLei[j].Fusion_OrNot;

		//		printf("ESM和TongLei数据%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],j+iEchoNum_RadarAIS);
	}

	iEchoNum_Total = iEchoNum_RadarAIS + iEchoNum_TongLei;
}

/*
***************************************
函数名  ：InitiateTrack
功能描述：模型参数初始化
参数    ：无
返回值  ：无
***************************************
*/
void InitiateTrack()
{
	int i = 0;
	int j = 0;
	double *Tmp = NULL;
	/* T为采样间隔 */
	double T = SAMPLE_PERIOD;

	/* 临时计数变量 */
	int iCnt;

	/* fXk临时变量，储存状态信息；fAvg临时变量，储存状态信息均值 */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* fCV3Fk为单维滤波时CV模型的系统转移矩阵
	* fCV3Qk为单维滤波时CV模型的过程噪声矩阵
	* fCV3Pk为单维滤波时CV模型的初始协方差阵 */
	double Sigma_X = pow((double)PNOISE_X,2);
	double Sigma_Y = pow((double)PNOISE_Y,2);
	double Sigma_Z = pow((double)PNOISE_Z,2);

	/* fCV3Fk为单维滤波时CV模型的系统转移矩阵
	* fCV3Qk为单维滤波时CV模型的过程噪声矩阵
	* fCV3Pk为单维滤波时CV模型的初始协方差阵 */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4)*Sigma_X,0.5*pow(T,3)*Sigma_X,0,0.5*pow(T,3)*Sigma_X,pow(T,2)*Sigma_X,0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,900000,0,0,0,0};

	/* fCA3Fk为单维滤波时CA模型的系统转移矩阵
	* fCA3Qk为单维滤波时CA模型的过程噪声矩阵
	* fCA3Pk为单维滤波时CA模型的初始协方差阵 */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	// double fCA3Qk[9] = {pow(T,5)*Sigma_X/20,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,T*Sigma_X};
	double fCA2Qk[9] = {0.25*pow(T,4)*Sigma_Y, 0.5*pow(T,3)*Sigma_Y, 0.5*pow(T,2)*Sigma_Y, 0.5*pow(T,3)*Sigma_Y, pow(T,2)*Sigma_Y, T*Sigma_Y, 0.5*pow(T,2)*Sigma_Y, T*Sigma_Y, Sigma_Y};

	double fCA3Qk[9] = {0.25*pow(T,4)*Sigma_Z, 0.5*pow(T,3)*Sigma_Z, 0.5*pow(T,2)*Sigma_Z, 0.5*pow(T,3)*Sigma_Z, pow(T,2)*Sigma_Z, T*Sigma_Z, 0.5*pow(T,2)*Sigma_Z, T*Sigma_Z, Sigma_Z};
	double fCA3Pk[9] = {9000000,0,0,0,9000000,0,0,0,9000};

	/* 量测矩阵 */
	double fHk[3][9];

	/* 临时变量 */
	double fTmp[9];
	MEASINFO pEcho_DataFusion;

	/* 初始化临时变量 */
	iCnt = 0;

	/* 取平均值 */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* 初始化临时变量 */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}
	/* 初始化Hk矩阵 */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* 初始化模型类型，如果是CV模型，则置1；否则为0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* 初始化CV模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* 初始化CA1模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA2Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* 初始化CA2模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);

	/* 初始化状态信息 */

	for (i=0;i<MAX_TRACK;i++){
		if (i<g_iTrkNum){
			sTracks[i].idx = i+1;
			sTracks[i].Conf_flag = 1;
			sTracks[i].ConfNum = 0;
			sTracks[i].ConfStart = 0;

			sTracks[i].Del_flag = 0;
			sTracks[i].Del_Init = 0;
			sTracks[i].DelNum = 0;
			sTracks[i].DelStart = 0;
			sTracks[i].Points = 0;

			sTracks[i].Theat = 0;

			sTracks[i].Type = sEcho_RadarAIS[i].Type;
			sTracks[i].Civi_Num = 0;

			/* 初始化模型概率 */
			sTracks[i].fMdlProb[0] = 0.4;
			sTracks[i].fMdlProb[1] = 0.3;
			sTracks[i].fMdlProb[2] = 0.3;

			Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[i]);
			/* 第一帧，初始化位置信息 */
			fXk[0] = pEcho_DataFusion.meas[0];
			fXk[3] = pEcho_DataFusion.meas[1];
			fXk[6] = pEcho_DataFusion.meas[2];
			/* Xk与Pk阵的初始化 */
			for (j=0; j<MODEL_NUM; j++){
				/* 各个模型Xk和Pk的初始化 */
				MtrAssign(1,STATE_DIM,fXk,sTracks[i].imm[j].xk);
				MtrAssign(STATE_DIM,STATE_DIM,pMdl[j].pk[0],sTracks[i].imm[j].pk[0]);
			}

			/* 融合中心状态初始化 */
			MtrAssign(1,STATE_DIM,fXk,sTracks[i].fsn.xk);

		}
		else{
			sTracks[i].idx = 0;
		}
	}

}


/*
***************************************
函数名  ：Track_Trim
功能描述：航迹后期整理函数
参数    ：

返回值  ：无
***************************************
*/
void Track_Trim()
{
	int i,j;
	for (i=0;i<MAX_TRACK;i++){
		if ( sTracks[i].idx != 0){
			for (j=0;j<Share_N;j++){
				ShareA[i][j] = ShareA[i][j+1];
				ShareB[i][j] = ShareB[i][j+1];
				ShareC[i][j] = ShareC[i][j+1];
				if ( j == (Share_N-1) ){
					ShareA[i][j] = sTracks[i].fsn.xk[0];
					ShareB[i][j] = sTracks[i].fsn.xk[1];
					ShareC[i][j] = sTracks[i].fsn.xk[2];
				}
			}
		}
	}
	TrkDel();
}

/*
***************************************
函数名  ：PolToDesc
功能描述：极坐标系转换到直角坐标系
参数    ：pPol，极坐标指针
返回值  ：pDesc，直角坐标指针
***************************************
*/
double *PolToDesc(double *pPol)
{
	double *pDesc = (double *)malloc(3*sizeof(double));

	pDesc[0] = pPol[0] * cos(pPol[2]) * cos(pPol[1]);
	pDesc[1] = pPol[0] * cos(pPol[2]) * sin(pPol[1]);
	pDesc[2] = pPol[0] * sin(pPol[2]);

	return pDesc;
}
/*
***************************************
函数名  ：DescToPol
功能描述：直角坐标系转换到极坐标系
参数    ：pDesc，直角坐标指针
返回值  ：pPol，极坐标指针
***************************************
*/
double *DescToPol(double *pDesc)
{
	double fRng;
	double *pPol = (double *)malloc(3*sizeof(double));

	fRng = sqrt(pow(pDesc[0],2) + pow(pDesc[1],2) + pow(pDesc[2],2));
	pPol[0] = fRng;
	pPol[1] = atan2(pDesc[1],pDesc[0]);
	pPol[2] = asin(pDesc[2]/fRng);

	return pPol;
}
/*
***************************************
函数名  ：Unbias_Conv
功能描述：量测由球坐标到直角坐标无偏转换 
参数    ：pTrk，航迹指针
返回值  ：pEcho，量测指针
***************************************
*/
void Unbias_Conv(MEASINFO *pEcho_Fusion,ECHO *pEcho)
{

	double *Tmp1 = NULL;
	double *Tmp2 = NULL;


	Tmp1 = PolToDesc(pEcho->msr);
	pEcho_Fusion->meas[0] = Tmp1[0];
	pEcho_Fusion->meas[1] = Tmp1[1];
	pEcho_Fusion->meas[2] = Tmp1[2];

	Tmp2 = Calculate_R(pEcho);

	pEcho_Fusion->nstd[0] = Tmp2[0];
	pEcho_Fusion->nstd[1] = Tmp2[1];
	pEcho_Fusion->nstd[2] = Tmp2[2];

	free(Tmp1);
	free(Tmp2);
}

/*
***************************************
函数名  ：Unbias_Conv
功能描述：量测由球坐标到直角坐标无偏转换 
参数    ：pTrk，航迹指针
返回值  ：pEcho，量测指针
***************************************
*/
double *Calculate_R(ECHO *pEcho)
{
	double LamdaB,LamdaBT,LamdaE,LamdaET;
	double Rxx,Ryy,Rzz,Rxy,Ryz,Rxz;
	double Rk,Bk,Ek;
	double c,d,K;
	double pxz,pxy,pyz;
	double Tmp1,Tmp2,Tmp3,Tmp4,Tmp5,Tmp6;
	double *Rmatrix = (double *)malloc(3*sizeof(double));

	LamdaB = exp(-1*sECHO_Param.Radar_aziMN*sECHO_Param.Radar_aziMN/2);
	LamdaBT = pow(LamdaB,4);

	LamdaE = exp(-1*sECHO_Param.Radar_eleMN*sECHO_Param.Radar_eleMN/2);
	LamdaET = pow(LamdaE,4);

	Rk = pEcho->msr[0];
	Bk = pEcho->msr[1];
	Ek = pEcho->msr[2];

	Rxx = -pow( (LamdaB*LamdaE*Rk*cos(Bk)*cos(Ek)),2) + 0.25*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*( 1 + LamdaBT*cos(2*Bk) )*(1+LamdaET*cos(2*Ek));
	Rxy = -pow( (LamdaB*LamdaE*Rk),2)*sin(Bk)*cos(Bk)*cos(Ek)*cos(Ek) + 0.25*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*LamdaBT*sin(2*Bk)*(1+LamdaET*cos(2*Ek));
	Rxz = -LamdaB*LamdaE*LamdaE*Rk*Rk*cos(Bk)*sin(Ek)*cos(Ek) + 0.5*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*LamdaB*LamdaET*cos(Bk)*sin(2*Ek);

	Ryy = -pow( (LamdaB*LamdaE*Rk*sin(Bk)*cos(Ek)),2) + 0.25*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*( 1 - LamdaBT*cos(2*Bk) )*(1+LamdaET*cos(2*Ek));
	Ryz = -LamdaB*LamdaE*LamdaE*Rk*Rk*sin(Bk)*sin(Ek)*cos(Ek) + 0.5*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*LamdaB*LamdaET*sin(Bk)*sin(2*Ek);
	Rzz = -pow( LamdaE*Rk*sin(Ek),2) + 0.5*(Rk*Rk + sECHO_Param.Radar_rgeMN*sECHO_Param.Radar_rgeMN )*( 1 - LamdaET*cos(2*Ek) );

	/*            计算协方差阵系数          */

	pxy = Rxy/sqrt(Rxx*Ryy);
	pyz = Ryz/sqrt(Ryy*Rzz);
	pxz = Rxz/sqrt(Rxx*Rzz);

	c = -( pow(pxy,2) + pow(pyz,2) + pow(pxz,2) );
	d = -2*pxy*pxz*pyz;
	Tmp1 = 27*d;
	Tmp2 = 3*c;
	Tmp5 = fabs( pow(Tmp1,2) + 4*pow(Tmp2,3) );
	Tmp6 = sqrt( Tmp5);
	Tmp3 = 0.5*(-Tmp1 + sqrt( Tmp5));
	Tmp4 = 0.5*(-Tmp1 - sqrt( Tmp5));
	K = 1 + pow( Tmp3,1/3)/3 + pow(Tmp4 ,1/3)/3;
	K = 5;

	/*	Rmatrix[0] = K * Rxx;
	Rmatrix[1] = 2*K * Ryy;
	Rmatrix[2] = 2*K * Rzz; */

	Rmatrix[0] = 4 * sqrt(Rxx);
	Rmatrix[1] = 3 * sqrt(Ryy);
	Rmatrix[2] = 2 * sqrt(Rzz); 
	return Rmatrix;
}

/*
***************************************
函数名  FilterPrediction
功能描述：滤波函数
参数    ：无
返回值  ：无
***************************************
*/
void FilterPrediction()
{
	int i;
	int Calcu_Dis = 0;
	/* 临时变量 */
	int j;
	int iSnr = 1;
	/* fValue储存各个模型的似然值；fChat储存混合概率之和 */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* 模型条件重初始化后的混合滤波信息 */
	FLTINFO sMix[MODEL_NUM];
	MEASINFO pEcho_DataFusion;

	for (i=0;i<MAX_TRACK;i++){
		if ( sTracks[i].idx != 0){

			Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[i]);
			/* 模型条件重初始化 */
			Reinitialize_Condition(&sTracks[i].fMdlProb[0],fChat,&sTracks[i].imm[0],sMix);
			for (j=0; j<MODEL_NUM; j++)
				fValue[j] = IMM_Kalman_Filtering(&sTracks[i].imm[j],&sMix[j],&pEcho_DataFusion,pMdl[j],iSnr,&Dis[j],&Calcu_Dis);
			/* 更新融合中心的状态信息 */
			Renew_Fusion(&sTracks[i],fValue,fChat);
		}

		sTracks[i].Points = sTracks[i].Points + 1;
	}

}


/*
***************************************
函数名    GNN_Asso
功能描述：最近邻关联
参数    ：无
返回值  ：无
***************************************
*/
void GNN_Asso(int *iMethod)
{
	TRACKINFO *pTrkTmp = NULL;
	TRACKINFO *pTrkNew = NULL;
	ECHO *pEcho = NULL;
	int i,j,k,t;
	int Tused;                         //航迹有更新标志位
	int tracknum = g_iTrkNum;
	int measnum = iEchoNum_Total;
	int Mused[2*MAX_ECHO] = {0};         //量测有更新标志位
	double Gate1,Gate2;
	double *Tmp = NULL;
	double Tmp2,Tmp3,Tmp4,Tmp5;
	int Tmp6;
	int Tmp7 = 5;
	double Distance[2*MAX_ECHO] = {0};   // 保留统计距离
	int MNUM;							 // 量测落入波门计数器
	int FusionNum;                       // 量测在本机探测范围的计数器
	int Singe_IMMFilter = 5;
	int iTmp = 0;
	double Dis = 0;
	double L = 0;
	double Gate = 0;
	double Share_Gate1 = 10;
	double Share_Gate2 = 10;
	double Share_Gate3 = 10;
	double ShareNum = 0;
	MEASINFO pEcho_DataFusion;
	MEASINFO pEcho_DataFusionTmp;
	MEASINFO *pTmpZk = NULL;
	MEASINFO *pTmpZk_Tmp = NULL;
	Gate1 = 5;
	Gate2 = 50;
	/*         波门设定      */
	if ( Spt < 2){
		Gate = Gate1;
	}
	else{
		Gate = Gate2;
	}
	/*         数据关联      */
	for (i=0;i<MAX_TRACK;i++){
		Tused = 0;

		if (sTracks[i].idx != 0){

			pTrkTmp = &sTracks[i];
			MNUM = 0;
			FusionNum = 0;
			for (j=0;j<iEchoNum_Total;j++){
				pEcho = &sEcho_MTT[j];
				//	for (j=0;j<iEchoNum_RadarAIS;j++){
				//		pEcho = &sEcho_RadarAIS[j];
				Statistic_Distance(pTrkTmp,pEcho,&Dis,&L);
				if (Dis < Gate ){
					Distance[j] = Dis;  //记录每个量测对于航迹的统计距离
					Tused = 1;			//指示该航迹已有量测落入波门
					Mused[j] = 1;		//判断分别是当前量测序列中的那些量测落入波门
					MNUM++;				//统计共有几个量测落入波门
					if (pEcho->Fusion_OrNot == 1)   //若落入本机的探测范围内 则计数 方便后面判断是否所有落入波门的量测都落入本机探测范围内
						FusionNum++;   

					if ( (pEcho->Type == 4) && (pTrkTmp->Type == 3)){
						pTrkTmp->Civi_Num++;
					}
				}
			}
			/*         航迹起始 滑窗法 3/5起始     */
			if (pTrkTmp->Conf_flag == 0){
				if (Tused == 1){
					pTrkTmp->ConfNum = pTrkTmp->ConfNum + 1;
				}
				if ( (Spt - pTrkTmp->ConfStart) > 4){
					if (pTrkTmp->ConfNum > 2){
						pTrkTmp->Conf_flag = 1;
					} 
					else
					{
						pTrkTmp->Del_flag = 1;
					}
				}
			}
			/*       航迹删除 判断是否删除航迹 若连续10帧均未有量测更新 则删除      */
			if (Tused == 0){
				if (pTrkTmp->Del_Init == 0){
					pTrkTmp->DelStart = Spt;
					pTrkTmp->Del_Init = 1;
					pTrkTmp->DelNum = 1;
				} 
				else
				{
					pTrkTmp->DelStart = pTrkTmp->DelStart + 1;
					if (Spt == pTrkTmp->DelStart){
						pTrkTmp->DelNum = pTrkTmp->DelNum + 1;
					} 
					else
					{
						pTrkTmp->Del_Init = 0;
					}
					if (pTrkTmp->DelNum == 5){
						pTrkTmp->Del_flag = 1;
					}
				}
			}
			/*           对进入融合算法还是多目标跟踪算法 进行判断 */
			iTmp = 0;
			Tmp5 = 0;
			Tmp6 = 0;
			if ( MNUM > 0){
				if ( ( FusionNum == MNUM) && ( MNUM > 1) &&( *iMethod < 5 )){  // 落入本机探测范围内 且 有多个量测落入当前航迹波门内
					pTmpZk = (MEASINFO *)malloc(sizeof(MEASINFO));
					pTmpZk_Tmp = pTmpZk;

					for (k=0;k<iEchoNum_Total;k++){
						//	if ( (Distance[k] != 0) && (iTmp<2) ){
						if  (Distance[k] != 0){
							Unbias_Conv(&pEcho_DataFusionTmp,&sEcho_MTT[k]);

							if (iTmp > 0){
								pTmpZk_Tmp->next = (MEASINFO *)malloc(sizeof(MEASINFO));
								pTmpZk_Tmp = pTmpZk_Tmp->next;
							}
							Assign_MeasInfo(pTmpZk_Tmp,&pEcho_DataFusionTmp);
							pTmpZk_Tmp->tidx =  pTrkTmp->idx;
							pTmpZk_Tmp->sidx = iTmp;
							pTmpZk_Tmp->next = NULL;
							iTmp++;
						}
					}

					IMM_Filtering(pTrkTmp,pTmpZk,pMdl,MNUM,*iMethod,Spt);
					for (t=0;t<iTmp;t++){
						pTmpZk_Tmp = pTmpZk->next;
						free(pTmpZk);
						pTmpZk = pTmpZk_Tmp;
					}
					pTmpZk_Tmp = NULL;
					pTmpZk = NULL;
				} 
				else{
					/*       对于没有进入融合算法的量测找到统计距离最小的量测      */
					for (k = 0;k < iEchoNum_Total;k++){            //        找到第一个非0的统计距离
						if (Distance[k] != 0){
							Tmp5 = Distance[k];
							break;
						}
					}

					for (k = 0;k < iEchoNum_Total;k++){            //        找到最小的非0的统计距离
						if ( ( Distance[k] != 0 )&& ( Tmp5 >= Distance[k] ) ){
							Tmp5 = Distance[k];
							Tmp6 = k;
						}
						Distance[k] = 0;
					}
					if (Tmp6 >= 0){
						Unbias_Conv(&pEcho_DataFusion,&sEcho_MTT[Tmp6]);
					}
					IMM_Filtering(pTrkTmp,&pEcho_DataFusion,pMdl,1,5,Spt);
				}
			}
			for (k = 0;k < iEchoNum_Total;k++){            //找到最小的非0的统计距离
				Distance[k] = 0;
			}

		}
	}
	/*       航迹合并 判断是否该合并航迹 若连续Share_N满足合并条件 则合并航迹      */
	if ( Spt>Share_N){

		for (i=0;i<MAX_TRACK;i++){
			for (j=0;j<MAX_TRACK;j++){
				ShareNum = 0;
				if ( (i != j) && (sTracks[i].idx != 0) &&  (sTracks[j].idx != 0) ){
					for (k=0;k<Share_N;k++){
						Tmp2 = fabs(ShareA[i][k] - ShareA[j][k]);
						Tmp3 = fabs(ShareB[i][k] - ShareB[j][k]);
						Tmp4 = fabs(ShareC[i][k] - ShareC[j][k]);
						if (Tmp2 < Share_Gate1 && Tmp3 < Share_Gate2 && Tmp4 < Share_Gate3)
							ShareNum++;
					}
					if (ShareNum > ShareConf_Num){
						if (sTracks[i].Points > sTracks[j].Points){
							sTracks[i].Del_flag = 1;
						} 
						else{
							sTracks[j].Del_flag = 1;
						}

					}
				}
			}
		}
	}
	/*       起始新航迹      */
	for (j=0;j<iEchoNum_Total;j++){
		if ( Mused[j] == 0 ){
			InitNewTrack(Spt,j);
		}
	}

	Spt++;
}

/*
***************************************
函数名  ：Statistic_Distance
功能描述：计算不同情形下的统计距离和
新息协方差阵
参数    ：pTrkTmp，航迹指针
pEcho，量测指针
Dis，统计距离
Lis，信息协方差行列式值
返回值  ：无
***************************************
*/
void Statistic_Distance(TRACKINFO *pTrkTmp,ECHO *pEcho,double *Distance,double *Lis)
{

	int j;
	int iSnr = 1;
	int Cacl_Distance = 1;
	/* fValue储存各个模型的似然值；fChat储存混合概率之和 */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* 模型条件重初始化后的混合滤波信息 */
	FLTINFO sMix[MODEL_NUM];
	MEASINFO pEcho_DataFusion;
	TRACKINFO *pTrk = NULL;
	pTrk = (TRACKINFO*)malloc( sizeof(TRACKINFO) );

	TrksAssign(pTrk,pTrkTmp);
	Unbias_Conv(&pEcho_DataFusion,pEcho);

	/* 模型条件重初始化 */
	Reinitialize_Condition(pTrk->fMdlProb,fChat,pTrk->imm,sMix);
	for (j=0; j<MODEL_NUM; j++)
		fValue[j] = IMM_Kalman_Filtering(&pTrk->imm[j],&sMix[j],&pEcho_DataFusion,pMdl[j],iSnr,&Dis[j],&Cacl_Distance);
	/* 更新融合中心的状态信息 */
	Renew_Fusion(pTrk,fValue,fChat);
	*Distance = fChat[0]*Dis[0] + fChat[1]*Dis[1] + fChat[2]*Dis[2];

	free(pTrk);
}


/*
***************************************
函数名  ：InitNewTrack
功能描述：模型参数初始化
参数    ：无
返回值  ：无
***************************************
*/
void InitNewTrack(int iSpt,int MeasN)
{
	int i = 0;
	int j = 0;
	int Tmp3;
	double *Tmp = NULL;
	/* T为采样间隔 */
	double T = SAMPLE_PERIOD;

	/* 临时计数变量 */
	int iCnt;

	/* fXk临时变量，储存状态信息；fAvg临时变量，储存状态信息均值 */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* fCV3Fk为单维滤波时CV模型的系统转移矩阵
	* fCV3Qk为单维滤波时CV模型的过程噪声矩阵
	* fCV3Pk为单维滤波时CV模型的初始协方差阵 */
	double Sigma_X = pow((double)PNOISE_X,2);
	double Sigma_Y = pow((double)PNOISE_Y,2);
	double Sigma_Z = pow((double)PNOISE_Z,2);

	/* fCV3Fk为单维滤波时CV模型的系统转移矩阵
	* fCV3Qk为单维滤波时CV模型的过程噪声矩阵
	* fCV3Pk为单维滤波时CV模型的初始协方差阵 */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4)*Sigma_X,0.5*pow(T,3)*Sigma_X,0,0.5*pow(T,3)*Sigma_X,pow(T,2)*Sigma_X,0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,90000,0,0,0,0};

	/* fCA3Fk为单维滤波时CA模型的系统转移矩阵
	* fCA3Qk为单维滤波时CA模型的过程噪声矩阵
	* fCA3Pk为单维滤波时CA模型的初始协方差阵 */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	double fCA3Qk[9] = {pow(T,5)*Sigma_X/20,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,T*Sigma_X};
	double fCA3Pk[9] = {9000000,0,0,0,90000,0,0,0,900};

	/* 量测矩阵 */
	double fHk[3][9];

	/* 临时变量 */
	double fTmp[9];
	MEASINFO pEcho_DataFusion;

	/* 初始化临时变量 */
	iCnt = 0;

	/* 取平均值 */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* 初始化临时变量 */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}
	/* 初始化Hk矩阵 */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* 初始化模型类型，如果是CV模型，则置1；否则为0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* 初始化CV模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* 初始化CA1模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* 初始化CA2模型参数，变为三维模型参数 */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);

	/* 初始化状态信息 */

	for (i=0;i<MAX_TRACK;i++){
		if (sTracks[i].idx == 0 ){
			Tmp3 = i;
			break;
		}
	}
	g_iTrkNum = g_iTrkNum + 1;

	sTracks[Tmp3].idx = Tmp3 + 1;
	sTracks[Tmp3].ConfStart = iSpt;
	sTracks[Tmp3].Conf_flag = 0;
	sTracks[Tmp3].ConfNum = 0;

	sTracks[Tmp3].Del_flag = 0;
	sTracks[Tmp3].Del_Init = 0;
	sTracks[Tmp3].DelNum = 0;
	sTracks[Tmp3].DelStart = 0;

	sTracks[Tmp3].Points = 0;

	/* 初始化模型概率 */
	sTracks[Tmp3].fMdlProb[0] = 0.4;
	sTracks[Tmp3].fMdlProb[1] = 0.3;
	sTracks[Tmp3].fMdlProb[2] = 0.3;

	Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[MeasN]);
	/* 第一帧，初始化位置信息 */
	fXk[0] = pEcho_DataFusion.meas[0];
	fXk[3] = pEcho_DataFusion.meas[1];
	fXk[6] = pEcho_DataFusion.meas[2];
	/* Xk与Pk阵的初始化 */
	for (j=0; j<MODEL_NUM; j++){
		/* 各个模型Xk和Pk的初始化 */
		MtrAssign(1,STATE_DIM,fXk,sTracks[Tmp3].imm[j].xk);
		MtrAssign(STATE_DIM,STATE_DIM,pMdl[j].pk[0],sTracks[Tmp3].imm[j].pk[0]);
	}

	/* 融合中心状态初始化 */
	MtrAssign(1,STATE_DIM,fXk,sTracks[Tmp3].fsn.xk);
	sTracks[Tmp3].Points = sTracks[Tmp3].Points + 1;
}

/*
***************************************
函数名  ：TrkDel
功能描述：根据航迹删除标记删除航迹
参数    ：无
返回值  ：无
***************************************
*/
void TrkDel(void)
{
	int i,j,k,l,t;
	for (i=0;i<MAX_TRACK;i++){
		if ( sTracks[i].Del_flag == 1){

			g_iTrkNum = g_iTrkNum - 1;

			sTracks[i].idx = 0;
			sTracks[i].Conf_flag = 0;
			sTracks[i].ConfNum = 0;
			sTracks[i].ConfStart = 0;

			sTracks[i].Del_flag = 0;
			sTracks[i].Del_Init = 0;
			sTracks[i].DelNum = 0;
			sTracks[i].DelStart = 0;

			sTracks[i].Points = 0;


			for (j=0;j<MODEL_NUM;j++){
				sTracks[i].fMdlProb[j] = 0;
				for (k=0;k<STATE_DIM;k++){
					sTracks[i].fsn.xk[k] = 0;
					sTracks[i].imm[j].xk[k] = 0;
					for (l=0;l<STATE_DIM;l++){
						sTracks[i].fsn.pk[k][l] = 0;
						sTracks[i].imm[j].pk[k][l] = 0;
					}
				}
			}

			for (t=0;t<Share_N;t++){
				ShareA[i][t] = 0;
				ShareB[i][t] = 0;
				ShareC[i][t] = 0;
			}
		}
	}
}



void TrksAssign(TRACKINFO *pTLeft, TRACKINFO *pTRight)
{
	pTLeft->idx = pTRight->idx;
	pTLeft->ConfNum = pTRight->ConfNum;
	pTLeft->Conf_flag = pTRight->Conf_flag;
	pTLeft->ConfStart = pTRight->ConfStart;

	pTLeft->Del_flag = pTRight->Del_flag;
	pTLeft->Del_Init = pTRight->Del_Init;
	pTLeft->DelNum = pTRight->DelNum;
	pTLeft->DelStart = pTRight->DelStart;

	pTLeft->Points = pTRight->Points;

	pTLeft->fMdlProb[0] = pTRight->fMdlProb[0];
	pTLeft->fMdlProb[1] = pTRight->fMdlProb[1];
	pTLeft->fMdlProb[2] = pTRight->fMdlProb[2];

	/* 将重初始化后的模型滤波信息赋给临时变量 */
	Assign_FltInfo(&pTLeft->fsn,&pTRight->fsn);
	Assign_FltInfo(&pTLeft->imm[0],&pTRight->imm[0]);
	Assign_FltInfo(&pTLeft->imm[1],&pTRight->imm[1]);
	Assign_FltInfo(&pTLeft->imm[2],&pTRight->imm[2]);

}

/*
***************************************
函数名  ：InitiateTrack
功能描述：模型参数初始化
参数    ：无
返回值  ：无
***************************************
*/
void Receive_Parameter(ECHO *sEcho_TL,ECHO *sEcho_RA,int *Echo_RA,int *Echo_TL,TRACKINFO *pTmp,ECHO_PARAMETER *sEcho_Param,int *TrkNum)
{
	TRACKINFO *pTrk = NULL;

	ECHO *pTmp_TL = NULL;
	ECHO *pTmp_RA = NULL;
	ECHO *pTmp_Self = NULL;
	int i = 0;
	int j = 0;

	pTmp_RA = sEcho_RA;
	pTmp_TL = sEcho_TL;

	for (i=0;i<MAX_ECHO;i++){

		sEcho_RadarAIS[i].Fusion_OrNot = pTmp_RA->Fusion_OrNot;
		sEcho_RadarAIS[i].msr[0] = pTmp_RA->msr[0];
		sEcho_RadarAIS[i].msr[1] = pTmp_RA->msr[1];
		sEcho_RadarAIS[i].msr[2] = pTmp_RA->msr[2];
		sEcho_RadarAIS[i].Type = pTmp_RA->Type;

		sEcho_TongLei[i].Fusion_OrNot = pTmp_TL->Fusion_OrNot;
		sEcho_TongLei[i].msr[0] = pTmp_TL->msr[0];
		sEcho_TongLei[i].msr[1] = pTmp_TL->msr[1];
		sEcho_TongLei[i].msr[2] = pTmp_TL->msr[2];
		sEcho_TongLei[i].Type = pTmp_TL->Type;

		pTmp_RA++;
		pTmp_TL++;

	}
	iEchoNum_RadarAIS = *Echo_RA;
	iEchoNum_TongLei = *Echo_TL;

	pTrk = pTmp;

	for (j=0;j<MAX_ECHO;j++){
		TrksAssign(&sTracks[j],pTrk);
		pTrk++;
	}
	



	sECHO_Param.Radar_RgeScope = sEcho_Param->Radar_RgeScope;
	sECHO_Param.Radar_Azi_MaxScope = sEcho_Param->Radar_Azi_MaxScope;
	sECHO_Param.Radar_Azi_MinScope = sEcho_Param->Radar_Azi_MinScope;
	sECHO_Param.Radar_Ele_MaxScope = sEcho_Param->Radar_Ele_MaxScope;
	sECHO_Param.Radar_Ele_MinScope = sEcho_Param->Radar_Ele_MinScope;

	sECHO_Param.AIS_RgeScope = sEcho_Param->AIS_RgeScope;
	sECHO_Param.AIS_Azi_MaxScope = sEcho_Param->AIS_Azi_MaxScope;
	sECHO_Param.AIS_Azi_MinScope = sEcho_Param->AIS_Azi_MinScope;
	sECHO_Param.AIS_Ele_MaxScope = sEcho_Param->AIS_Ele_MaxScope;
	sECHO_Param.AIS_Ele_MinScope = sEcho_Param->AIS_Ele_MinScope;

	sECHO_Param.Tong_RgeScope = sEcho_Param->Tong_RgeScope;
	sECHO_Param.Tong_Azi_MaxScope = sEcho_Param->Tong_Azi_MaxScope;
	sECHO_Param.Tong_Azi_MinScope = sEcho_Param->Tong_Azi_MinScope;
	sECHO_Param.Tong_Ele_MaxScope = sEcho_Param->Tong_Ele_MaxScope;
	sECHO_Param.Tong_Ele_MinScope = sEcho_Param->Tong_Ele_MinScope;

	sECHO_Param.Lei_RgeScope = sEcho_Param->Lei_RgeScope;
	sECHO_Param.Lei_Azi_MaxScope = sEcho_Param->Lei_Azi_MaxScope;
	sECHO_Param.Lei_Azi_MinScope = sEcho_Param->Lei_Azi_MinScope;
	sECHO_Param.Lei_Ele_MaxScope = sEcho_Param->Lei_Ele_MaxScope;
	sECHO_Param.Lei_Ele_MinScope = sEcho_Param->Lei_Ele_MinScope;



	sECHO_Param.Radar_aziMN = sEcho_Param->Radar_aziMN;
	sECHO_Param.Radar_eleMN = sEcho_Param->Radar_eleMN;
	sECHO_Param.Radar_rgeMN = sEcho_Param->Radar_rgeMN;

	sECHO_Param.AIS_rgeMN = sEcho_Param->AIS_rgeMN;
	sECHO_Param.AIS_aziMN = sEcho_Param->AIS_aziMN;
	sECHO_Param.AIS_eleMN = sEcho_Param->AIS_eleMN;

	
	sECHO_Param.Tong_aziMN = sEcho_Param->Tong_aziMN;
	sECHO_Param.Tong_eleMN = sEcho_Param->Tong_eleMN;

	sECHO_Param.Lei_aziMN = sEcho_Param->Lei_aziMN;
	sECHO_Param.Lei_eleMN = sEcho_Param->Lei_eleMN;
	sECHO_Param.NOISEandOUTLIER_TYPE = sEcho_Param->NOISEandOUTLIER_TYPE;

	g_iTrkNum = *TrkNum;

}

/*
***************************************
函数名  ：Transmit_Trk
功能描述：航迹从DLL中输出
参数    ：无
返回值  ：无
***************************************
*/
void Transmit_Trk(TRACKINFO *pTrk)
{
	int j;

	for (j=0;j<MAX_ECHO;j++){
		TrksAssign(pTrk,&sTracks[j]);
		pTrk++;
	}

}