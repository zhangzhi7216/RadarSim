/*
***************************************
件名    ：fusionFilter.c
描述    ：融合估计滤波器
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
#include "commonProcess.h"
#include "matrixOperation.h"
#include "fusionFilter.h"

/*
***************************************
*          全局变量声明               *
***************************************
*/

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
函数名  ：IMM_Filtering
功能描述：交互式多模型滤波主函数
参数    ：pTrk，航迹信息
          pMeas，量测信息
		  pMdl，模型参数
		  iMethod，选择的融合方法
		  iSpt，帧号
返回值  ：无
***************************************
*/
void IMM_Filtering(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSnr,int iMethod,int iSpt)
{
	/* 临时变量 */
	int i;
	int Calcu_Dis = 0;
	/* fValue储存各个模型的似然值；fChat储存混合概率之和 */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* 模型条件重初始化后的混合滤波信息 */
	FLTINFO sMix[MODEL_NUM];

	/* 模型条件重初始化 */
	Reinitialize_Condition(pTrk->fMdlProb,fChat,pTrk->imm,sMix);

	/* 模型条件滤波 */
	/* 似然函数计算及滤波更新 */
//	printf("输入的融合滤波算法为 %d\n",iMethod);
	switch (iMethod){
	case 1:
		/* 并行滤波 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Parallel_Filtering(&pTrk->imm[i],sMix[i],pMeas,pMdl[i],iSnr);
		break;
	case 2:
		/* 数据压缩滤波 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Data_Compression_Filtering(&pTrk->imm[i],sMix[i],pMeas,pMdl[i],iSnr);
		break;
	case 3:
		/* Bar-Shalom Campo 融合算法 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Bar_Shalom_Campo_Fusion(pTrk,sMix[i],pMeas,pMdl[i],iSnr,i);
		break;
	case 4:
		/* Bar-Shalom Campo 融合算法 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Optimal_Distributed_Fusion_Without_Ack(pTrk,sMix[i],pMeas,pMdl[i],iSnr,i);
		break;
	case 5:
		/* 单个量测交互式多模型滤波算法 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = IMM_Kalman_Filtering(&pTrk->imm[i],&sMix[i],pMeas,pMdl[i],iSnr,&Dis[i],&Calcu_Dis);
		break;
	default:
		/* 提示输入的信息错误 */
		printf("输入的融合滤波算法有误！\n");
		break;
	}

	/* 更新融合中心的状态信息 */
	Renew_Fusion(pTrk,fValue,fChat);
}


/*
***************************************
函数名  ：Parallel_Filtering
功能描述：并行滤波
参数    ：pTrk，更新的滤波信息
          pMix，混合估计信息
		  pMeas，量测信息
		  sMdl,模型参数
返回值  ：无
***************************************
*/
double Parallel_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr)
{
	/* iMeasNum量测个数；Dk似然值 */
	int iMeasNum;
	double Dk;

	/* fRn为量测噪声矩阵 */
	RKNS *fRn;

	/* sPred保存一步预测信息 */
	FLTINFO sPred;

	/* 初始化Rk矩阵 */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* 进行一步预测 */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* 量测预测残差、协方差阵以及似然函数的计算 */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* 滤波方程和误差协方差矩阵的计算 */
	Update_StateByParallelFiltering(pFlt,sPred,sMdl,fRn,pMeas,iMeasNum);

	/* 释放内存 */
	free(fRn);

	return Dk;
}

/*
***************************************
函数名  ：Data_Compression_Filtering
功能描述：数据压缩滤波
参数    ：pTrk，更新的滤波信息
          pMix，混合估计信息
		  pMeas，量测信息
		  sMdl,模型参数
返回值  ：无
***************************************
*/
double Data_Compression_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr)
{
	/* iMeasNum量测个数；Dk似然值 */
	int iMeasNum;
	double Dk;

	/* fRn为量测噪声矩阵 */
	RKNS *fRn;

	/* sPred保存一步预测信息 */
	FLTINFO sPred;

	/* 压缩后的伪量测方程参数信息 */
	COMPINFO sFgd;

	/* 初始化Rk矩阵 */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* 进行一步预测 */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* 生成伪量测及其伪量测矩阵 */
	Generate_ForgedMeas(&sFgd,pMeas,fRn,sMdl,iMeasNum);

	/* 计算似然函数 */
	Dk = Calculate_Likelihood(sPred,sFgd.Hk[0],sFgd.Rk[0],sFgd.Zk,1);

	/* 滤波方程和误差协方差矩阵的计算 */
	Update_StateByCompressedFiltering(pFlt,sPred,sFgd,iMeasNum);

	return Dk;
}

/*
***************************************
函数名  ：Bar_Shalom_Campo_Fusion
功能描述：Bar-Shalom-Campo融合算法
参数    ：pTrk，航迹信息
          pMix，混合估计信息
		  pMeas，量测信息
		  sMdl,模型参数
		  iMdl,模型序号
返回值  ：无
***************************************
*/
double Bar_Shalom_Campo_Fusion(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl)
{
	/* i,j为计数变量；iTgt表示航迹号；iMeasNum表示量测个数 */
	int i,j,iTgt,iMeasNum,iSnrNum;
	int Calcu_Dis = 0;
	/* Dk似然函数值 */
	double Dk;

	/* P互协方差矩阵；pXk扩维状态信息；pIk为扩维单位矩阵 */
	double *P,*pXk,*pIk;
	double Dis;
	/* fRn为量测噪声矩阵 */
	RKNS *fRn;

	/* sFltTmp滤波信息临时变量；
	   sPred储存融合中心一步预测滤波信息；
	   sSnsPr储存各个传感器的一步预测信息；
	   sFlt储存储存各传感器滤波信息的临时变量 */
	FLTINFO sFltTmp,sPred;
	FLTINFO *sSnsPr,*sFlt,*sSnsFlt;

	/* sCvFlt 储存降维后的滤波信息；sCvNew储存降维的融合中西滤波信息 */
	CVFLTINFO *sCvFlt,sCvNew;

	/* 量测信息临时变量 */
	MEASINFO *pMs;

	/* 分配内存空间 */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);

	sSnsPr = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sSnsFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);

	sCvFlt = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);

	/* 初始化Rk矩阵 */

	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* 进行一步预测 */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* 量测预测残差、协方差阵以及似然函数的计算 */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* 各个传感器的局部航迹状态估计 */
	pMs = pMeas;
	if (pMeas != NULL)
		iTgt = pMs->tidx;

	for (i=0; i<iSnr; i++){
		/* 选取属于该航迹该传感器的量测 */
		pMs = Select_Measure(pMeas,iTgt,i,&iSnrNum);

		/* 将模型iMdl的滤波信息赋给临时变量 */
		Assign_FltInfo(&sFltTmp,&pTrk->imm[iMdl]);

		/* 卡尔曼滤波，获得传感器级的滤波信息和一步预测信息 */
		Kalman_Filtering(&sFltTmp,&sSnsPr[i],sMdl,fRn[i].Rk[0],pMs->meas,&Dis,&Calcu_Dis);

		/* 将临时变量的值赋给传感器i模型iMdl的滤波信息 */
		Assign_FltInfo(&sSnsFlt[i],&sFltTmp);

		pMs = pMs->next;
	}

	/* 对于CV模型的滤波状态信息进行降维操作，以便求逆 */
	if (sMdl.isCV == CVMODEL){
		/* 为pXk，pIk分配内存 */
		pXk = (double *)malloc(sizeof(double) * CV_STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * CV_STATE_DIM * CV_STATE_DIM * iSnr);

		/* 传感器滤波信息降维 */
		for(j=0; j<iSnr; j++)
			Decrease_Dimension(&sSnsFlt[j],&sCvFlt[j],1);

		/* 根据方差上界技术获得对角线型的互协方差阵 */
		P = ErrVar_UpperBoundByCV(sCvFlt,CV_STATE_DIM,iSnr);

		/* 获得扩维的Xk和IK阵 */
		Generate_CombStateNEyeByCV(pXk,pIk,sCvFlt,CV_STATE_DIM,iSnr);

		/* 更新滤波信息 */
		Update_StateByML(sCvNew.xk,sCvNew.pk[0],P,pXk,pIk,CV_STATE_DIM,iSnr);

		/* 还原维数 */
		Increase_Dimension(&pTrk->imm[iMdl],&sCvNew,1);
	}else{
		/* 为pXk，pIk分配内存 */
		pXk = (double *)malloc(sizeof(double) * STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * STATE_DIM * STATE_DIM * iSnr);

		/* 传感器滤波信息降维 */
		for(j=0; j<iSnr; j++)
			Assign_FltInfo(&sFlt[j],&sSnsFlt[j]);

		/* 根据方差上界技术获得对角线型的互协方差阵 */
		P = ErrVar_UpperBound(sFlt,STATE_DIM,iSnr);

		/* 获得扩维的Xk和IK阵 */
		Generate_CombStateNEye(pXk,pIk,sFlt,STATE_DIM,iSnr);

		/* 更新滤波信息 */
		Update_StateByML(pTrk->imm[iMdl].xk,pTrk->imm[iMdl].pk[0],P,pXk,pIk,STATE_DIM,iSnr);
	}

	/* 释放空间 */
	free(pXk);
	free(pIk);

	free(fRn);
	free(sSnsPr);
	free(sFlt);
	free(sSnsFlt);
	free(sCvFlt);

	return Dk;
}

/*
***************************************
函数名  ：Optimal_Distributed_Fusion_Without_Ack
功能描述：不带反馈的最优分布式估计融合
参数    ：pTrk，航迹信息
          pMix，混合估计信息
		  pMeas，量测信息
		  sMdl,模型参数
		  iMdl,模型序号
返回值  ：无
***************************************
*/
double Optimal_Distributed_Fusion_Without_Ack(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl)
{
	/* i,j为计数变量；iTgt表示航迹号；iMeasNum表示量测个数 */
	int i,j,iTgt,iMeasNum,iSnrNum;
	int Calcu_Dis = 0;
	/* Dk似然函数值 */
	double Dk;

	/* pXk扩维状态信息；pIk为扩维单位矩阵 */
	double *pXk,*pIk;
	double Dis;
	/* fRn为量测噪声矩阵 */
	RKNS *fRn;

	/* sFltTmp滤波信息临时变量；
	   sPred储存融合中心一步预测滤波信息；
	   sSnsPr储存各个传感器的一步预测信息；
	   sFlt储存储存各传感器滤波信息的临时变量 */
	FLTINFO sFltTmp,sPred;
	FLTINFO *sSnsPr,*sFlt,*sSnsFlt;

	/* sCvFlt储存降维后的滤波信息；
	   sCvPr各个传感器CV模型的一步预测信息；
       sCvNew储存降维的融合中西滤波信息；
	   sCvPred储存融合中心一步预测信息*/
	CVFLTINFO sCvNew,sCvPred;
	CVFLTINFO *sCvFlt,*sCvPr;

	/* 量测信息临时变量 */
	MEASINFO *pMs;

	/* 分配内存空间 */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);

	sSnsPr = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sSnsFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);

	sCvFlt = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);
	sCvPr = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);

	/* 初始化Rk矩阵 */
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* 进行一步预测 */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* 量测预测残差、协方差阵以及似然函数的计算 */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* 各个传感器的局部航迹状态估计 */
	pMs = pMeas;
	if (pMeas != NULL)
		iTgt = pMs->tidx;

	for (i=0; i<iSnr; i++){
		/* 选取属于该航迹该传感器的量测 */
		pMs = Select_Measure(pMeas,iTgt,i,&iSnrNum);

		/* 将模型iMdl的滤波信息赋给临时变量 */
		Assign_FltInfo(&sFltTmp,&pTrk->imm[iMdl]);

		/* 卡尔曼滤波，获得传感器级的滤波信息和一步预测信息 */
		Kalman_Filtering(&sFltTmp,&sSnsPr[i],sMdl,fRn[i].Rk[0],pMs->meas,&Dis,&Calcu_Dis);

		/* 将临时变量的值赋给传感器i模型iMdl的滤波信息 */
		Assign_FltInfo(&sSnsFlt[i],&sFltTmp);

		pMs = pMs->next;
	}

	/* 对于CV模型的滤波状态信息进行降维操作，以便求逆 */
	if (sMdl.isCV == CVMODEL){
		/* 为pXk，pIk分配内存 */
		pXk = (double *)malloc(sizeof(double) * CV_STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * CV_STATE_DIM * CV_STATE_DIM * iSnr);

		/* 降维 */
		for(j=0; j<iSnr; j++)
			Decrease_Dimension(&sSnsFlt[j],&sCvFlt[j],1);
		Decrease_Dimension(sSnsPr,sCvPr,iSnr);
		Decrease_Dimension(&sPred,&sCvPred,1);

		/* 更新融合中心滤波信息 */
		Update_StateByOptimalDistributionByCV(&sCvNew,sCvFlt,sCvPr,&sCvPred,CV_STATE_DIM,iSnr);

		/* 升维 */
		Increase_Dimension(&pTrk->imm[iMdl],&sCvNew,1);
	}else{
		/* 为pXk，pIk分配内存 */
		pXk = (double *)malloc(sizeof(double) * STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * STATE_DIM * STATE_DIM * iSnr);

		/* 转储各个传感器的滤波信息 */
		for(j=0; j<iSnr; j++)
			Assign_FltInfo(&sFlt[j],&sSnsFlt[j]);

		/* 更新融合中心滤波信息 */
		Update_StateByOptimalDistribution(&pTrk->imm[iMdl],sFlt,sSnsPr,&sPred,STATE_DIM,iSnr);
	}

	/* 释放内存 */
	free(pXk);
	free(pIk);

	free(fRn);
	free(sSnsPr);
	free(sFlt);
	free(sSnsFlt);
	free(sCvFlt);
	free(sCvPr);

	return Dk;
}

/*
***************************************
函数名  ：IMM_Kalman_Filtering
功能描述：用于交互式多模型的卡尔曼滤波,
          该函数默认输入量测只有一个
参数    ：pTrk，滤波后k+1时刻的状态信息
          pFltPred，k时刻的一步预测值
		  sMdl,模型参数
		  iSnr传感器个数
返回值  ：Dk，似然函数值
***************************************
*/
double IMM_Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,double *Dis,int *Calcu_Dis)
{
	int i;
	double fValue;
	FLTINFO sFltTmp,sPred;
	double Rk[MEAS_DIM][MEAS_DIM];
	/* fVar 储存量测噪声方差 */
	double fVar[MEAS_DIM];

	if (iSnr != 1){
		printf("注意：该算法默认只有一个传感器一个量测的情况，请确认是否符合要求！\n");

	}

	/* 将标准差化为方差 */
	for (i=0; i<MEAS_DIM; i++){
		fVar[i] = pMeas->nstd[i] * pMeas->nstd[i];
	}
	VectDiag(MEAS_DIM,fVar,Rk[0]);
	// VectDiag(MEAS_DIM,pMeas->nstd,Rk[0]);
	/* 将重初始化后的模型滤波信息赋给临时变量 */
	Assign_FltInfo(&sFltTmp,pMix);

	/* 卡尔曼滤波，获得传感器级的滤波信息和一步预测信息 */
	fValue = Kalman_Filtering(&sFltTmp,&sPred,sMdl,Rk[0],pMeas->meas,Dis,Calcu_Dis);

	/* 将临时变量的值赋给传感器i模型iMdl的滤波信息 */
	Assign_FltInfo(pFlt,&sFltTmp);

	return fValue;

}

/*
***************************************
函数名  ：Kalman_Filtering
功能描述：卡尔曼滤波
参数    ：pTrk，滤波后k+1时刻的状态信息
          pFltPred，k时刻的一步预测值
		  sMdl,模型参数
		  Rk，量测噪声
		  pMs，量测
返回值  ：无
***************************************
*/
double Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pFltPred,MODELPARA sMdl,double *Rk,double *pMs,double *Dis,int *Calcu_Dis)
{
	/* 滤波运算中用到的中间变量 */
	int i;
	double HkPkp[3][9],tHk[9][3],HPH[3][3],Sk[3][3];
	double PktHk[9][3],iSk[3][3],Kk[9][3];
	double HkXkp[3],Vk[3];
	double KkVk[9];
	double KkRk[9][3],KRK[9][9],tKk[3][9],Arr[9],Eye[9][9];
	double KkHk[9][9],IKH[9][9],tIKH[9][9],IKHP[9][9],IPI[9][9];
	double VkiSk[3],VSV,detS,Dk;

	/* 进行一步预测 */
	Predict_OneStep(pFltPred,*pFlt,sMdl);

	/* S=Hk*Pkp*Hk'+Rk */
	MtrMtrMulti(3,9,9,sMdl.hk[0],pFltPred->pk[0],HkPkp[0]);
	MtrTrans(3,9,sMdl.hk[0],tHk[0]);
	MtrMtrMulti(3,9,3,HkPkp[0],tHk[0],HPH[0]);
	MtrMtrPlus(3,3,HPH[0],Rk,Sk[0]);

	/* Kk=Pkp*Hk'*inv(Sk) */
	MtrMtrMulti(9,9,3,pFltPred->pk[0],tHk[0],PktHk[0]);
	MtrInvers(3,Sk[0],iSk[0]);
	MtrMtrMulti(9,3,3,PktHk[0],iSk[0],Kk[0]);

	/* V=Zk-Hk*Xkp */
	MtrMtrMulti(3,9,1,sMdl.hk[0],pFltPred->xk,HkXkp);
	MtrMtrMinus(3,1,pMs,HkXkp,Vk);

	/* 计算Gauss假设下的似然函数值 */
	MtrMtrMulti(1,3,3,Vk,iSk[0],VkiSk);
	MtrMtrMulti(1,3,1,VkiSk,Vk,&VSV);

	if ( *Calcu_Dis == 0 ){

	/* Xkk=Xkp+Kk*Vk */
	MtrMtrMulti(9,3,1,Kk[0],Vk,KkVk);
	MtrMtrPlus(9,1,pFltPred->xk,KkVk,pFlt->xk);

	/* Pkk=Kk*Rk*Kk'+(fEye(9)-Kk*Hk)*Pkp*(fEye(9)-Kk*Hk)' */
	MtrMtrMulti(9,3,3,Kk[0],Rk,KkRk[0]);
	MtrTrans(9,3,Kk[0],tKk[0]);
	MtrMtrMulti(9,3,9,KkRk[0],tKk[0],KRK[0]);

	/* 生成单位阵 */
	for (i=0; i<STATE_DIM; i++)
		Arr[i] = 1.0;
	VectDiag(STATE_DIM,Arr,Eye[0]);

	MtrMtrMulti(9,3,9,Kk[0],sMdl.hk[0],KkHk[0]);
	MtrMtrMinus(9,9,Eye[0],KkHk[0],IKH[0]);
	MtrMtrMulti(9,9,9,IKH[0],pFltPred->pk[0],IKHP[0]);
	MtrTrans(9,9,IKH[0],tIKH[0]);
	MtrMtrMulti(9,9,9,IKHP[0],tIKH[0],IPI[0]);
	MtrMtrPlus(9,9,KRK[0],IPI[0],pFlt->pk[0]);
	detS = MtrDet(3,Sk[0]);
	Dk = exp(-0.5*VSV) / sqrt(2*PI*detS);

	}
	*Dis = VSV;
	return Dk;
}

/*
***************************************
函数名  ：Reinitialize_Condition
功能描述：模型条件重初始化
参数    ：fMdlProb,模型概率；
          fChat，混合概率中间变量；
		  pOldFlt，混合前滤波信息；
		  pNewFlt，混合后滤波信息；
返回值  ：无
***************************************
*/
void Reinitialize_Condition(double *fMdlProb,double *fChat,FLTINFO *pOldFlt,FLTINFO *pNewFlt)
{
	/* 计算中用到的中间变量 */
	int i,j;
	double tPt[MODEL_NUM*MODEL_NUM],fMixProb[MODEL_NUM][MODEL_NUM];
	double fXkTmp[STATE_DIM];
	double fXeXe[STATE_DIM][STATE_DIM],fPkTmp[STATE_DIM][STATE_DIM],fPkPrb[STATE_DIM][STATE_DIM];

	/* 状态误差信息 */
	struct xkerr{
		double Xe[STATE_DIM];
	}sXkErr[MODEL_NUM][MODEL_NUM];

	/* 计算混合概率 */
	/* 马尔科夫概率转移矩阵 */
	double Pt[MODEL_NUM*MODEL_NUM] = {0.9,0.05,0.05,0.05,0.9,0.05,0.2,0.2,0.6};

	/* 计算模型概率 */
	MtrTrans(MODEL_NUM,MODEL_NUM,Pt,tPt);
	MtrMtrMulti(MODEL_NUM,MODEL_NUM,1,tPt,fMdlProb,fChat);
	for(i=0;i<MODEL_NUM;i++)
		for(j=0;j<MODEL_NUM;j++)
			fMixProb[i][j] = Pt[i*MODEL_NUM+j] *fMdlProb[i]/fChat[j];

	/* 混合估计 */
	for (i=0; i<MODEL_NUM; i++){
		MtrInitialize(1,STATE_DIM,pNewFlt[i].xk);

		for (j=0; j<MODEL_NUM; j++){
			MtrCnstMulti(1,STATE_DIM,fMixProb[j][i],pOldFlt[j].xk,fXkTmp);
			MtrMtrPlus(1,STATE_DIM,fXkTmp,pNewFlt[i].xk,pNewFlt[i].xk);
		}
	}

	/* 计算误差 */
	for (i=0; i<MODEL_NUM; i++){
		for(j=0; j<MODEL_NUM; j++){
			MtrMtrMinus(1,STATE_DIM,pOldFlt[i].xk,pNewFlt[j].xk,sXkErr[i][j].Xe);
		}	
	}

	/* 计算混合Pk阵 */
	for (i=0; i<MODEL_NUM; i++){
		MtrInitialize(STATE_DIM,STATE_DIM,pNewFlt[i].pk[0]);

		for (j=0; j<MODEL_NUM; j++){
			MtrMtrMulti(STATE_DIM,1,STATE_DIM,sXkErr[j][i].Xe,sXkErr[j][i].Xe,fXeXe[0]);
			MtrMtrPlus(STATE_DIM,STATE_DIM,pOldFlt[j].pk[0],fXeXe[0],fPkTmp[0]);
			MtrCnstMulti(STATE_DIM,STATE_DIM,fMixProb[j][i],fPkTmp[0],fPkPrb[0]);
			MtrMtrPlus(STATE_DIM,STATE_DIM,fPkPrb[0],pNewFlt[i].pk[0],pNewFlt[i].pk[0]);
		}
	}
}

/*
***************************************
函数名  ：Initialize_Rk
功能描述：初始化Rk矩阵
参数    ：pMeas,量测信息
          pRn，量测噪声矩阵
返回值  ：iCnt，量测个数
***************************************
*/
int Initialize_Rk(MEASINFO *pMeas,RKNS *pRn)
{
	/* i计数变量；iCnt计算量测个数 */
	int i,iCnt;

	/* fVar 储存量测噪声方差 */
	double fVar[MEAS_DIM];

	/* 临时变量 */
	MEASINFO *pMsTmp;

	/* 初始化 */
	iCnt = 0;
	pMsTmp = pMeas;
	while (pMsTmp != NULL){
		/* 将标准差化为方差 */
		for (i=0; i<MEAS_DIM; i++){
			fVar[i] = pMsTmp->nstd[i] * pMsTmp->nstd[i];
		}
		
		/* 对角化 */
		VectDiag(MEAS_DIM,fVar,pRn[iCnt].Rk[0]);

		pMsTmp = pMsTmp->next;
		iCnt++;
	}

	return iCnt;
}

/*
***************************************
函数名  ：Predict_OneStep
功能描述：进行一步预测
参数    ：pPred,一步预测滤波信息
          pFlt，滤波信息
		  pMdl，模型参数
返回值  ：无
***************************************
*/
void Predict_OneStep(FLTINFO *pPred,FLTINFO pFlt,MODELPARA pMdl)
{
	/* 计算中间变量 */
	double tFk[STATE_DIM][STATE_DIM],fFP[STATE_DIM][STATE_DIM],fFPF[STATE_DIM][STATE_DIM];

	/* Xkp = Fk*Xk; */
	MtrMtrMulti(STATE_DIM,STATE_DIM,1,pMdl.fk[0],pFlt.xk,pPred->xk);

	/* Pkp = Fk*Pk*Fk' + Qk; */
	MtrTrans(STATE_DIM,STATE_DIM,pMdl.fk[0],tFk[0]);
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,pMdl.fk[0],pFlt.pk[0],fFP[0]);
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,fFP[0],tFk[0],fFPF[0]);
	MtrMtrPlus(STATE_DIM,STATE_DIM,fFPF[0],pMdl.qk[0],pPred->pk[0]);
}

/*
***************************************
函数名  ：Calculate_LikelihoodOld
功能描述：量测预测残差、协方差阵以及似然函数的计算。当前版本不使用该函数
参数    ：pPred,一步预测滤波信息
          pMeas,量测信息
		  sMdl，模型参数
		  pRk,量测噪声
		  iMeasNum，量测个数
返回值  ：Dk，似然函数值
***************************************
*/
double Calculate_LikelihoodOld(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum)
{
	int i;
	int iRB,iRF;

	double *Zp,*Hp,*Rp;
	double *HpXkp,*Zkd,*HpPkp,*tHp,*HPH,*Sk;
	double *tZkd,*iSk,*tZkdSk;

	double ZSZ,Dk;

	MEASINFO *pMs;

	/* 量测扩围 */
	pMs = pMeas;

	Zp = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	Hp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	Rp = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);

	MtrInitialize(MEAS_DIM * iMeasNum,1,Zp);
	MtrInitialize(MEAS_DIM * iMeasNum,STATE_DIM,Hp);
	MtrInitialize(MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,Rp);

	for (i=0; i<iMeasNum; i++){
		iRB = i*MEAS_DIM;
		iRF = (i+1)*MEAS_DIM;

		Assign_Matrix2Block(iRB,iRF,0,1,1,1,Zp,pMs->meas);
		Assign_Matrix2Block(iRB,iRF,0,STATE_DIM,STATE_DIM,STATE_DIM,Hp,sMdl.hk[0]);
		Assign_Matrix2Block(iRB,iRF,iRB,iRF,MEAS_DIM * iMeasNum,MEAS_DIM,Rp,pRk[i].Rk[0]);

		pMs = pMs->next;
	}

	/* 计算似然函数 */
	//Zkd = Zp - Hp*Xkp;
	HpXkp = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	Zkd = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);

	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,1,Hp,sPred.xk,HpXkp);
	MtrMtrMinus(MEAS_DIM * iMeasNum,1,Zp,HpXkp,Zkd);

	//Sk = Hp*Pkp*Hp' + Rp;
	tHp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	HpPkp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	HPH = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum* MEAS_DIM * iMeasNum);
	Sk = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);

	MtrTrans(MEAS_DIM * iMeasNum,STATE_DIM,Hp,tHp);
	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,STATE_DIM,Hp,sPred.pk[0],HpPkp);
	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,MEAS_DIM * iMeasNum,HpPkp,tHp,HPH);
	MtrMtrPlus(MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,HPH,Rp,Sk);

	// Dk = exp(-0.5*Zkd'*inv(Sk)*Zkd);
	tZkd = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	iSk = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);
	tZkdSk = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	
	MtrTrans(MEAS_DIM * iMeasNum,1,Zkd,tZkd);
	MtrInvers(MEAS_DIM  * iMeasNum,Sk,iSk);
	MtrMtrMulti(1,MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,tZkd,iSk,tZkdSk);
	MtrMtrMulti(1,MEAS_DIM * iMeasNum,1,tZkdSk,Zkd,&ZSZ);
	Dk = exp(-0.5 * ZSZ);

	/* 释放内存 */
	free(Zp);
	free(Hp);
	free(Rp);
	free(HpXkp);
	free(Zkd);
	free(tHp);
	free(HpPkp);
	free(HPH);
	free(Sk);
	free(tZkd);
	free(iSk);
	free(tZkdSk);

	return Dk;
}

/*
***************************************
函数名  ：Calculate_LikelihoodByAugmentedMeas
功能描述：量测预测残差、协方差阵以及似然函数的计算
参数    ：sPred,一步预测滤波信息
          pMeas,量测信息
		  sMdl，模型参数
		  pRk,量测噪声
		  iMeasNum，量测个数
返回值  ：Dk，似然函数值
***************************************
*/
double Calculate_LikelihoodByAugmentedMeas(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum)
{
	/* 似然函数值 */
	double Dk;

	/* 扩维方程参数 */
	double *Zp,*Hp,*Rp;

	/* 分配空间 */
	Zp = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	Hp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	Rp = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);

	/* 量测扩维 */
	Augment_Dimension(pMeas,sMdl,pRk,Hp,Rp,Zp,iMeasNum);

	/* 计算似然函数 */
	Dk = Calculate_Likelihood(sPred,Hp,Rp,Zp,iMeasNum);

	/* 释放内存 */
	free(Zp);
	free(Hp);
	free(Rp);

	return Dk;
}

/*
***************************************
函数名  ：Calculate_LikelihoodByAugmentedMeas
功能描述：量测扩维
参数    ：pMeas,量测信息
		  sMdl，模型参数
		  pRk,量测噪声
		  Hp,扩维后的量测矩阵
		  Rp，扩维后的量测噪声矩阵
		  Zp，扩维后的量测
		  iMeasNum，量测个数
返回值  ：无
***************************************
*/
void Augment_Dimension(MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,double *Hp,double *Rp,double *Zp,int iMeasNum)
{
	/* 临时变量 */
	int i;
	int iRB,iRF;

	/* 临时指针变量 */
	MEASINFO *pMs;

	/* 量测扩围 */
	pMs = pMeas;

	/* 初始化矩阵 */
	MtrInitialize(MEAS_DIM * iMeasNum,1,Zp);
	MtrInitialize(MEAS_DIM * iMeasNum,STATE_DIM,Hp);
	MtrInitialize(MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,Rp);

	/* 扩维 */
	for (i=0; i<iMeasNum; i++){
		iRB = i*MEAS_DIM;
		iRF = (i+1)*MEAS_DIM;

		Assign_Matrix2Block(iRB,iRF,0,1,1,1,Zp,pMs->meas);
		Assign_Matrix2Block(iRB,iRF,0,STATE_DIM,STATE_DIM,STATE_DIM,Hp,sMdl.hk[0]);
		Assign_Matrix2Block(iRB,iRF,iRB,iRF,MEAS_DIM * iMeasNum,MEAS_DIM,Rp,pRk[i].Rk[0]);

		pMs = pMs->next;
	}
}

/*
***************************************
函数名  ：Calculate_Likelihood
功能描述：计算似然函数
参数    ：sPred,一步预测滤波信息
		  Hp,扩维后的量测矩阵
		  Rp，扩维后的量测噪声矩阵
		  Zp，扩维后的量测
		  iMeasNum，量测个数
返回值  ：Dk，似然函数值
***************************************
*/
double Calculate_Likelihood(FLTINFO sPred,double *Hp,double *Rp,double *Zp,int iMeasNum)
{
	/* 临时变量 */
	double *HpXkp,*Zkd,*HpPkp,*tHp,*HPH,*Sk;
	double *tZkd,*iSk,*tZkdSk;
	double ZSZ,Dk;

	/* 计算似然函数 */
	/* Zkd = Zp - Hp*Xkp; */
	HpXkp = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	Zkd = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);

	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,1,Hp,sPred.xk,HpXkp);
	MtrMtrMinus(MEAS_DIM * iMeasNum,1,Zp,HpXkp,Zkd);

	/* Sk = Hp*Pkp*Hp' + Rp; */
	tHp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	HpPkp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	HPH = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum* MEAS_DIM * iMeasNum);
	Sk = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);

	MtrTrans(MEAS_DIM * iMeasNum,STATE_DIM,Hp,tHp);
	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,STATE_DIM,Hp,sPred.pk[0],HpPkp);
	MtrMtrMulti(MEAS_DIM * iMeasNum,STATE_DIM,MEAS_DIM * iMeasNum,HpPkp,tHp,HPH);
	MtrMtrPlus(MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,HPH,Rp,Sk);

	/* Dk = exp(-0.5*Zkd'*inv(Sk)*Zkd); */
	tZkd = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	iSk = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);
	tZkdSk = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	
	MtrTrans(MEAS_DIM * iMeasNum,1,Zkd,tZkd);
	MtrInvers(MEAS_DIM  * iMeasNum,Sk,iSk);
	MtrMtrMulti(1,MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,tZkd,iSk,tZkdSk);
	MtrMtrMulti(1,MEAS_DIM * iMeasNum,1,tZkdSk,Zkd,&ZSZ);
	Dk = exp(-0.5 * ZSZ);

	/* 释放内存 */
	free(HpXkp);
	free(Zkd);
	free(tHp);
	free(HpPkp);
	free(HPH);
	free(Sk);
	free(tZkd);
	free(iSk);
	free(tZkdSk);

	return Dk;
}

/*
***************************************
函数名  ：Update_StateByParallelFiltering
功能描述：并行滤波的滤波状态和协方差矩阵计算
参数    ：pFlt，融合中西滤波信息
          sPred,一步预测滤波信息
          pMeas,量测信息
		  sMdl，模型参数
		  pRk,量测噪声
		  iMeasNum，量测个数
返回值  ：无
***************************************
*/
void Update_StateByParallelFiltering(FLTINFO *pFlt,FLTINFO sPred,MODELPARA sMdl,RKNS *pRk,MEASINFO *pMeas,int iMeasNum)
{
	/* 计算中间变量 */
	int i;
	double tHk[STATE_DIM][MEAS_DIM],iRk[MEAS_DIM][MEAS_DIM],tHR[STATE_DIM][MEAS_DIM],HkXkp[MEAS_DIM],Err[MEAS_DIM];
	double HRH[STATE_DIM][STATE_DIM],HRZH[STATE_DIM],tHRH[STATE_DIM][STATE_DIM],tHRZH[STATE_DIM];
	double arr[STATE_DIM],eye[STATE_DIM][STATE_DIM];
	double PH[STATE_DIM][STATE_DIM],EPH[STATE_DIM][STATE_DIM],iEPH[STATE_DIM][STATE_DIM],PHZ[STATE_DIM];

	/* 量测噪声 */
	RKNS *pRn;

	/* 量测信息 */
	MEASINFO *pMs;

	/* 初始化 */
	pRn = pRk;
	pMs = pMeas;

	/* 初始化矩阵为0 */
	MtrInitialize(STATE_DIM,STATE_DIM,HRH[0]);
	MtrInitialize(STATE_DIM,1,HRZH);

	for (i=0; i<iMeasNum; i++){
		/* HRH = HRH + Hk'*inv(Rk(:,:,i))*Hk; */
		MtrTrans(MEAS_DIM,STATE_DIM,sMdl.hk[0],tHk[0]);
		MtrInvers(MEAS_DIM,pRn->Rk[0],iRk[0]);
		MtrMtrMulti(STATE_DIM,MEAS_DIM,MEAS_DIM,tHk[0],iRk[0],tHR[0]);
		MtrMtrMulti(STATE_DIM,MEAS_DIM,STATE_DIM,tHR[0],sMdl.hk[0],tHRH[0]);
		MtrMtrPlus(STATE_DIM,STATE_DIM,tHRH[0],HRH[0],HRH[0]);

		/* HRZH = HRZH + Hk'*inv(Rk(:,:,i))*(Zk(:,i)-Hk*Xkp); */
		MtrMtrMulti(MEAS_DIM,STATE_DIM,1,sMdl.hk[0],sPred.xk,HkXkp);
		MtrMtrMinus(MEAS_DIM,1,pMs->meas,HkXkp,Err);
		MtrMtrMulti(STATE_DIM,MEAS_DIM,1,tHR[0],Err,tHRZH);
		MtrMtrPlus(STATE_DIM,1,tHRZH,HRZH,HRZH);

		pRn++;
		pMs = pMs->next;
	}

	/* Pkk = inv(eye(9) + Pkp*HRH) * Pkp; */
	for (i=0; i<STATE_DIM; i++)
		arr[i] = 1;
	VectDiag(STATE_DIM,arr,eye[0]);
	
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,sPred.pk[0],HRH[0],PH[0]);
	MtrMtrPlus(STATE_DIM,STATE_DIM,eye[0],PH[0],EPH[0]);
	MtrInvers(STATE_DIM,EPH[0],iEPH[0]);
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,iEPH[0],sPred.pk[0],pFlt->pk[0]);

	/* Xkk = Xkp + Pkk*HRZH; */
	MtrMtrMulti(STATE_DIM,STATE_DIM,1,pFlt->pk[0],HRZH,PHZ);
	MtrMtrPlus(STATE_DIM,1,sPred.xk,PHZ,pFlt->xk);

}

/*
***************************************
函数名  ：Renew_Fusion
功能描述：更新模型概率和IMM滤波状态
参数    ：pTrk,航迹信息
          pDk，似然函数值
		  pChat，混合概率中间变量
返回值  ：无
***************************************
*/
void Renew_Fusion(TRACKINFO *pTrk,double *pDk,double *pChat)
{
	/* 临时变量 */
	int i;
	double fUkTmp,fXkTmp[STATE_DIM];
	double fXkErr[STATE_DIM],fPkErr[STATE_DIM][STATE_DIM],fPkNew[STATE_DIM][STATE_DIM],fPkTmp[STATE_DIM][STATE_DIM];

	/* 根据滤波结果更新模型概率 */
	MtrMtrMulti(1,MODEL_NUM,1,pDk,pChat,&fUkTmp);
	for (i=0; i<MODEL_NUM; i++)
		pTrk->fMdlProb[i] = pDk[i] * pChat[i] / fUkTmp;

	/* 估计融合,更新滤波值 */
	MtrInitialize(STATE_DIM,1,pTrk->fsn.xk);
	for (i=0; i<MODEL_NUM; i++){
		MtrCnstMulti(STATE_DIM,1,pTrk->fMdlProb[i],pTrk->imm[i].xk,fXkTmp);
		MtrMtrPlus(STATE_DIM,1,fXkTmp,pTrk->fsn.xk,pTrk->fsn.xk);
	}

	/* 估计融合，更新误差协方差矩阵 */
	/// 2012.6.24新增
	MtrInitialize(STATE_DIM,STATE_DIM,pTrk->fsn.pk[0]);
	for (i=0; i<MODEL_NUM; i++){
		MtrMtrMinus(STATE_DIM,1,pTrk->fsn.xk,pTrk->imm[i].xk,fXkErr);
		MtrMtrMulti(STATE_DIM,1,STATE_DIM,fXkErr,fXkErr,fPkErr[0]);
		MtrMtrPlus(STATE_DIM,STATE_DIM,pTrk->imm[i].pk[0],fPkErr[0],fPkNew[0]);

		MtrCnstMulti(STATE_DIM,STATE_DIM,pTrk->fMdlProb[i],fPkNew[0],fPkTmp[0]);
		MtrMtrPlus(STATE_DIM,STATE_DIM,fPkTmp[0],pTrk->fsn.pk[0],pTrk->fsn.pk[0]);
	}
}


/*
***************************************
函数名  ：Generate_ForgedMeas
功能描述：数据压缩,生成伪量测方程信息
参数    ：pCi，伪方程参数信息
          pMeas,量测信息
		  sMdl，模型参数
		  pRk,量测噪声
		  iMeasNum，量测个数
返回值  ：无
***************************************
*/
void Generate_ForgedMeas(COMPINFO *pCi,MEASINFO *pMeas,RKNS *pRk,MODELPARA sMdl,int iMeasNum)
{
	/* 临时变量 */
	int i;

	double tmpRz[MEAS_DIM],tmpRk[MEAS_DIM][MEAS_DIM],tmpRh[MEAS_DIM][STATE_DIM];
	double iRk[MEAS_DIM][MEAS_DIM],iRz[MEAS_DIM],iRh[MEAS_DIM][STATE_DIM];

	/* 量测信息  */
	MEASINFO *pMs;

	/* 量测噪声 */
	RKNS *pRn;

	/* 初始化为0 */
	MtrInitialize(MEAS_DIM,1,tmpRz);
	MtrInitialize(MEAS_DIM,MEAS_DIM,tmpRk[0]);
	MtrInitialize(MEAS_DIM,STATE_DIM,tmpRh[0]);

	/* 数据压缩 */
	pMs = pMeas;
	pRn = pRk;
	for(i=0; i<iMeasNum; i++){
		/* tmpRk = tmpRk + inv(Rk(:,:,i)); */
		MtrInvers(MEAS_DIM,pRn->Rk[0],iRk[0]);
		MtrMtrPlus(MEAS_DIM,MEAS_DIM,iRk[0],tmpRk[0],tmpRk[0]);

		/* tmpRz = tmpRz + inv(Rk(:,:,i)) * Zk(:,i); */
		MtrMtrMulti(MEAS_DIM,MEAS_DIM,1,iRk[0],pMs->meas,iRz);
		MtrMtrPlus(MEAS_DIM,1,iRz,tmpRz,tmpRz);

		/* tmpRh = tmpRh + inv(Rk(:,:,i)) * Hk; */
		MtrMtrMulti(MEAS_DIM,MEAS_DIM,STATE_DIM,iRk[0],sMdl.hk[0],iRh[0]);
		MtrMtrPlus(MEAS_DIM,STATE_DIM,iRh[0],tmpRh[0],tmpRh[0]);

		pMs = pMs->next;
		pRn ++;
	}

	/* hypoRk = inv(tmpRk); */
	MtrInvers(MEAS_DIM,tmpRk[0],pCi->Rk[0]);

	/* hypoZk = hypoRk * tmpRz; */
	MtrMtrMulti(MEAS_DIM,MEAS_DIM,1,pCi->Rk[0],tmpRz,pCi->Zk);

	/* hypoHk = hypoRk * tmpRh; */
	MtrMtrMulti(MEAS_DIM,MEAS_DIM,STATE_DIM,pCi->Rk[0],tmpRh[0],pCi->Hk[0]);
}

/*
***************************************
函数名  ：Update_StateByCompressedFiltering
功能描述：数据压缩滤波的滤波状态和协方差矩阵计算
参数    ：pFlt,融合中心滤波状态信息
          sPred,一步预测信息
          sCi，伪方程参数信息
		  iMeasNum，量测个数
返回值  ：无
***************************************
*/
void Update_StateByCompressedFiltering(FLTINFO *pFlt,FLTINFO sPred,COMPINFO sCi,int iMeasNum)
{
	/* 中间变量 */
	int i;
	double tHk[STATE_DIM][MEAS_DIM],iRk[MEAS_DIM][MEAS_DIM],tHR[STATE_DIM][MEAS_DIM],HkXkp[MEAS_DIM],Err[MEAS_DIM];
	double HRH[STATE_DIM][STATE_DIM],HRZH[STATE_DIM];

	double arr[STATE_DIM],eye[STATE_DIM][STATE_DIM];
	double PH[STATE_DIM][STATE_DIM],EPH[STATE_DIM][STATE_DIM],iEPH[STATE_DIM][STATE_DIM],PHZ[STATE_DIM];

	/* 初始化为0 */
	MtrInitialize(STATE_DIM,STATE_DIM,HRH[0]);
	MtrInitialize(STATE_DIM,1,HRZH);

	/* HRH = hypoHk'*inv(hypoRk)*hypoHk; */
	MtrTrans(MEAS_DIM,STATE_DIM,sCi.Hk[0],tHk[0]);
	MtrInvers(MEAS_DIM,sCi.Rk[0],iRk[0]);
	MtrMtrMulti(STATE_DIM,MEAS_DIM,MEAS_DIM,tHk[0],iRk[0],tHR[0]);
	MtrMtrMulti(STATE_DIM,MEAS_DIM,STATE_DIM,tHR[0],sCi.Hk[0],HRH[0]);

	/* HRZH = hypoHk'*inv(hypoRk)*(hypoZk-hypoHk*Xkp); */
	MtrMtrMulti(MEAS_DIM,STATE_DIM,1,sCi.Hk[0],sPred.xk,HkXkp);
	MtrMtrMinus(MEAS_DIM,1,sCi.Zk,HkXkp,Err);
	MtrMtrMulti(STATE_DIM,MEAS_DIM,1,tHR[0],Err,HRZH);


	/* Pkk = inv(eye(9) + Pkp*HRH) * Pkp; */
	for (i=0; i<STATE_DIM; i++)
		arr[i] = 1;
	VectDiag(STATE_DIM,arr,eye[0]);
	
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,sPred.pk[0],HRH[0],PH[0]);
	MtrMtrPlus(STATE_DIM,STATE_DIM,eye[0],PH[0],EPH[0]);
	MtrInvers(STATE_DIM,EPH[0],iEPH[0]);
	MtrMtrMulti(STATE_DIM,STATE_DIM,STATE_DIM,iEPH[0],sPred.pk[0],pFlt->pk[0]);

	/* Xkk = Xkp + Pkk*HRZH; */
	MtrMtrMulti(STATE_DIM,STATE_DIM,1,pFlt->pk[0],HRZH,PHZ);
	MtrMtrPlus(STATE_DIM,1,sPred.xk,PHZ,pFlt->xk);

}

/*
***************************************
函数名  ：Decrease_Dimension
功能描述：为避免矩阵不可求逆，将CV状态信息降维
参数    ：pFlt,滤波状态信息
          pCvFlt,CV模型的滤波状态信息
		  iNum，需要降维的滤波状态信息的个数
返回值  ：无
***************************************
*/
void Decrease_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum)
{
	int i,j;
	BMINFO pl,pr;

	for (i=0; i<iNum; i++){
		/* 滤波状态降维 */
		pCvFlt[i].xk[0] = pFlt[i].xk[0];
		pCvFlt[i].xk[1] = pFlt[i].xk[1];
		pCvFlt[i].xk[2] = pFlt[i].xk[3];
		pCvFlt[i].xk[3] = pFlt[i].xk[4];
		pCvFlt[i].xk[4] = pFlt[i].xk[6];
		pCvFlt[i].xk[5] = pFlt[i].xk[7];

		/* 误差协方差矩阵降维 */
		MtrInitialize(CV_STATE_DIM,CV_STATE_DIM,pCvFlt[i].pk[0]);

		for (j=0; j<SGL_DIM; j++){
			pl.iRowBeg = j*CV_SGL_DIM;
			pl.iRowFin = j*CV_SGL_DIM + CV_SGL_DIM;
			pl.iColBeg = j*CV_SGL_DIM;
			pl.iColFin = j*CV_SGL_DIM + CV_SGL_DIM;
			pl.iColDim = CV_STATE_DIM;

			pr.iRowBeg = j*SGL_DIM;
			pr.iRowFin = j*SGL_DIM + CV_SGL_DIM;
			pr.iColBeg = j*SGL_DIM;
			pr.iColFin = j*SGL_DIM + CV_SGL_DIM;
			pr.iColDim = STATE_DIM;

			Assign_Block2Block(pl,pr,pCvFlt[i].pk[0],pFlt[i].pk[0]);
		}
	}

}

/*
***************************************
函数名  ：Increase_Dimension
功能描述：恢复CV状态信息维数
参数    ：pFlt,滤波状态信息
          pCvFlt,CV模型的滤波状态信息
		  iNum，需要降维的滤波状态信息的个数
返回值  ：无
***************************************
*/
void Increase_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum)
{
	int i,j;
	BMINFO pl,pr;

	for (i=0; i<iNum; i++){
		/* 滤波状态升维 */
		MtrInitialize(STATE_DIM,1,pFlt[i].xk);

		pFlt[i].xk[0] = pCvFlt[i].xk[0];
		pFlt[i].xk[1] = pCvFlt[i].xk[1];
		pFlt[i].xk[3] = pCvFlt[i].xk[2];
		pFlt[i].xk[4] = pCvFlt[i].xk[3];
		pFlt[i].xk[6] = pCvFlt[i].xk[4];
		pFlt[i].xk[7] = pCvFlt[i].xk[5];

		/* 误差协方差矩阵降维 */
		MtrInitialize(STATE_DIM,STATE_DIM,pFlt[i].pk[0]);

		for (j=0; j<SGL_DIM; j++){
			pr.iRowBeg = j*CV_SGL_DIM;
			pr.iRowFin = j*CV_SGL_DIM + CV_SGL_DIM;
			pr.iColBeg = j*CV_SGL_DIM;
			pr.iColFin = j*CV_SGL_DIM + CV_SGL_DIM;
			pr.iColDim = CV_STATE_DIM;

			pl.iRowBeg = j*SGL_DIM;
			pl.iRowFin = j*SGL_DIM + CV_SGL_DIM;
			pl.iColBeg = j*SGL_DIM;
			pl.iColFin = j*SGL_DIM + CV_SGL_DIM;
			pl.iColDim = STATE_DIM;

			Assign_Block2Block(pl,pr,pFlt[i].pk[0],pCvFlt[i].pk[0]);
		}
	}

}

/*
***************************************
函数名  ：ErrVar_UpperBound
功能描述：根据方差上界计算联合误差协方差阵
参数    ：pFlt,滤波状态信息
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：P，互协方差矩阵
***************************************
*/
double* ErrVar_UpperBound(FLTINFO *pFlt,int iDim,int iNum)
{
	/* 中间变量 */
	int i,iBeg,iFin,iPdim;

	/* 互协方差矩阵 */
	double *P;

	/* 初始化 */
	iPdim = iDim * iNum;
	P = (double *)malloc(sizeof(double) * iPdim * iPdim);
	MtrInitialize(iPdim,iPdim,P);

	/* 先获得对角线型的矩阵 */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,iBeg,iFin,iPdim,iDim,P,pFlt[i].pk[0]);
	}

	/* 获得其方差上界 */
	MtrCnstMulti(iPdim,iPdim,iNum,P,P);

	return P;
}

/*
***************************************
函数名  ：ErrVar_UpperBoundByCV
功能描述：根据方差上界计算联合误差协方差阵,CV模型专用
参数    ：pFlt,滤波状态信息
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：P，互协方差矩阵
***************************************
*/
double* ErrVar_UpperBoundByCV(CVFLTINFO *pCvFlt,int iDim,int iNum)
{
	/* 中间变量 */
	int i,iBeg,iFin,iPdim;

	/* 互协方差矩阵 */
	double *P;

	/* 初始化 */
	iPdim = iDim * iNum;
	P = (double *)malloc(sizeof(double) * iPdim * iPdim);
	MtrInitialize(iPdim,iPdim,P);

	/* 先获得对角线型的矩阵 */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,iBeg,iFin,iPdim,iDim,P,pCvFlt[i].pk[0]);
	}

	/* 获得其方差上界 */
	MtrCnstMulti(iPdim,iPdim,iNum,P,P);

	return P;
}

/*
***************************************
函数名  ：Generate_CombStateNEye
功能描述：合成估计与合成单位矩阵的生成
参数    ：pXk,扩维状态信息
          pIk，扩维单位矩阵
		  pFlt，滤波信息
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：无
***************************************
*/
void Generate_CombStateNEye(double *pXk,double *pIk,FLTINFO *pFlt,int iDim,int iNum)
{
	/* 临时变量 */
	int i,iBeg,iFin;
	double *pArr,*pEye;

	/* 初始化矩阵 */
	MtrInitialize(iDim * iNum,1,pXk);
	MtrInitialize(iDim * iNum,iDim,pIk);

	/* 分配空间 */
	pArr = (double *)malloc(sizeof(double) * iDim);
	pEye = (double *)malloc(sizeof(double) * iDim * iDim);

	/* 获得单位阵 */
	for (i=0; i<iDim; i++)
		pArr[i] = 1.0;
	VectDiag(iDim,pArr,pEye);

	/* 矩阵扩维 */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,0,1,1,1,pXk,pFlt[i].xk);
		Assign_Matrix2Block(iBeg,iFin,0,iDim,iDim,iDim,pIk,pEye);
	}

	/* 释放空间 */
	free(pArr);
	free(pEye);
}

/*
***************************************
函数名  ：Generate_CombStateNEye
功能描述： CV模型合成估计与合成单位矩阵的生成
参数    ：pXk,扩维状态信息
          pIk，扩维单位矩阵
		  pFlt，滤波信息
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：无
***************************************
*/
void Generate_CombStateNEyeByCV(double *pXk,double *pIk,CVFLTINFO *pFlt,int iDim,int iNum)
{
	/* 临时变量 */
	int i,iBeg,iFin;
	double *pArr,*pEye;

	/* 初始化矩阵 */
	MtrInitialize(iDim * iNum,1,pXk);
	MtrInitialize(iDim * iNum,iDim,pIk);

	/* 分配空间 */
	pArr = (double *)malloc(sizeof(double) * iDim);
	pEye = (double *)malloc(sizeof(double) * iDim * iDim);

	/* 获得单位阵 */
	for (i=0; i<iDim; i++)
		pArr[i] = 1.0;
	VectDiag(iDim,pArr,pEye);

	/* 矩阵扩维 */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,0,1,1,1,pXk,pFlt[i].xk);
		Assign_Matrix2Block(iBeg,iFin,0,iDim,iDim,iDim,pIk,pEye);
	}

	/* 释放空间 */
	free(pArr);
	free(pEye);
}

/*
***************************************
函数名  ：Update_StateByML
功能描述：最大似然意义下的融合估计和估计协方差矩阵的计算
参数    ：Xkk,融合状态信息
          Pkk，融合协方差信息
		  P，互协方差矩阵
		  Xk，扩维状态信息
          Ik，扩维单位矩阵
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：无
***************************************
*/
void Update_StateByML(double *Xkk,double *Pkk,double *P,double *Xk,double *Ik,int iDim,int iNum)
{
	/* 中间变量 */
	double *tIk,*iP,*tIkiP,*IPI,*PIP;

	/* 分配空间 */
	tIk = (double *) malloc (sizeof(double) * iDim * iDim * iNum);
	iP = (double *) malloc (sizeof(double) * iDim * iDim * iNum * iNum);
	tIkiP = (double *) malloc (sizeof(double) * iDim * iDim * iNum);
	IPI = (double *) malloc (sizeof(double) * iDim * iDim);
	PIP = (double *) malloc (sizeof(double) * iDim * iDim * iNum);

	/* Pkk = inv(Ik' * inv(P) * Ik); */
	MtrTrans(iDim * iNum,iDim,Ik,tIk);
	MtrInvers(iDim * iNum,P,iP);
	MtrMtrMulti(iDim,iDim * iNum,iDim * iNum,tIk,iP,tIkiP);
	MtrMtrMulti(iDim,iDim * iNum,iDim,tIkiP,Ik,IPI);
	MtrInvers(iDim,IPI,Pkk);

	/* Xkk = Pkk * Ik' * inv(P) * Xk; */
	MtrMtrMulti(iDim,iDim,iDim * iNum,Pkk,tIkiP,PIP);
	MtrMtrMulti(iDim,iDim * iNum,1,PIP,Xk,Xkk);

	/* 释放内存 */
	free(tIk);
	free(iP);
	free(tIkiP);
	free(IPI);
	free(PIP);
}

/*
***************************************
函数名  ：Update_StateByOptimalDistribution
功能描述：不带反馈的最优分布式融合算法，更新中心估计器的状态和协方差矩阵
参数    ：pNew,融合中心滤波信息
          pKf，各传感器Kalman滤波信息
		  pPr，各传感器一步预测
		  pCPr，融合中心一步预测
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：无
***************************************
*/
void Update_StateByOptimalDistribution(FLTINFO *pNew,FLTINFO *pKf,FLTINFO *pPr,FLTINFO *pCPr,int iDim,int iNum)
{
	/* 中间变量 */
	int i;
	double *tmpPk,*tmpPX;
	double *dP,*dPX,*iPk,*iPkp,*iPkXk,*iPkpXkp;
	double *iCPk,*PP,*iCPkXk,*PXPX;

	/* 分配内存 */
	tmpPk = (double *)malloc(sizeof(double) * iDim * iDim);
	tmpPX = (double *)malloc(sizeof(double) * iDim); 
	dP       = (double *)malloc(sizeof(double) * iDim * iDim);
	dPX     = (double *)malloc(sizeof(double) * iDim);
	iPk      =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkp	   =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkXk   =  (double *)malloc(sizeof(double) * iDim); 
	iPkpXkp=  (double *)malloc(sizeof(double) * iDim); 

	/* 矩阵初始化 */
	MtrInitialize(iDim,iDim,tmpPk);
	MtrInitialize(iDim,1,tmpPX);

	/* 更新中心估计器的状态和协方差矩阵 */
	for(i=0; i<iNum; i++){
		/* tmpPk = tmpPk + inv(LclPk(:,:,i)) - inv(LclPkp(:,:,i)); */
		MtrInvers(iDim,pKf[i].pk[0],iPk);
		MtrInvers(iDim,pPr[i].pk[0],iPkp);
		MtrMtrMinus(iDim,iDim,iPk,iPkp,dP);
		MtrMtrPlus(iDim,iDim,dP,tmpPk,tmpPk);

		/* tmpPkXk = tmpPkXk + inv(LclPk(:,:,i))*LclXk(:,i) - inv(LclPkp(:,:,i))*LclXkp(:,i); */
		MtrMtrMulti(iDim,iDim,1,iPk,pKf[i].xk,iPkXk);
		MtrMtrMulti(iDim,iDim,1,iPkp,pPr[i].xk,iPkpXkp);
		MtrMtrMinus(iDim,1,iPkXk,iPkpXkp,dPX);
		MtrMtrPlus(iDim,1,dPX,tmpPX,tmpPX);
	}
	
	/* 分配内存 */
	iCPk =  (double *)malloc(sizeof(double) * iDim * iDim);
	PP  =   (double *)malloc(sizeof(double) * iDim * iDim);
	iCPkXk  = (double *)malloc(sizeof(double) * iDim);
	PXPX = (double *)malloc(sizeof(double) * iDim);

	/* tPkk = inv(inv(tPkp) + tmpPk); */
	MtrInvers(iDim,pCPr->pk[0],iCPk);
	MtrMtrPlus(iDim,iDim,iCPk,tmpPk,PP);
	MtrInvers(iDim,PP,pNew->pk[0]);

	/* tXkk = tPkk * (inv(tPkp)*tXkp + tmpPkXk); */
	MtrMtrMulti(iDim,iDim,1,iCPk,pCPr->xk,iCPkXk);
	MtrMtrPlus(iDim,1,iCPkXk,tmpPX,PXPX);
	MtrMtrMulti(iDim,iDim,1,pNew->pk[0],PXPX,pNew->xk);

	/* 释放内存 */
	free(tmpPk);
	free(tmpPX);
	free(dP);
	free(dPX);
	free(iPk);
	free(iPkp);
	free(iPkXk);
	free(iPkpXkp);
	free(iCPk);
	free(PP);
	free(iCPkXk);
	free(PXPX);
}

/*
***************************************
函数名  ：Update_StateByOptimalDistributionByCV
功能描述：CV模型的不带反馈的最优分布式融合算法，更新中心估计器的状态和协方差矩阵
参数    ：pNew,融合中心滤波信息
          pKf，各传感器Kalman滤波信息
		  pPr，各传感器一步预测
		  pCPr，融合中心一步预测
          pDim，块矩阵维数
		  iNum，块矩阵个数
返回值  ：无
***************************************
*/
void Update_StateByOptimalDistributionByCV(CVFLTINFO *pNew,CVFLTINFO *pKf,CVFLTINFO *pPr,CVFLTINFO *pCPr,int iDim,int iNum)
{
	/* 中间变量 */
	int i;
	double *tmpPk,*tmpPX;
	double *dP,*dPX,*iPk,*iPkp,*iPkXk,*iPkpXkp;
	double *iCPk,*PP,*iCPkXk,*PXPX;

	/* 分配内存 */
	tmpPk = (double *)malloc(sizeof(double) * iDim * iDim);
	tmpPX = (double *)malloc(sizeof(double) * iDim); 
	dP       = (double *)malloc(sizeof(double) * iDim * iDim);
	dPX     = (double *)malloc(sizeof(double) * iDim);
	iPk      =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkp	   =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkXk   =  (double *)malloc(sizeof(double) * iDim); 
	iPkpXkp=  (double *)malloc(sizeof(double) * iDim); 

	/* 矩阵初始化 */
	MtrInitialize(iDim,iDim,tmpPk);
	MtrInitialize(iDim,1,tmpPX);

	/* 更新中心估计器的状态和协方差矩阵 */
	for(i=0; i<iNum; i++){
		/* tmpPk = tmpPk + inv(LclPk(:,:,i)) - inv(LclPkp(:,:,i)); */
		MtrInvers(iDim,pKf[i].pk[0],iPk);
		MtrInvers(iDim,pPr[i].pk[0],iPkp);
		MtrMtrMinus(iDim,iDim,iPk,iPkp,dP);
		MtrMtrPlus(iDim,iDim,dP,tmpPk,tmpPk);

		/* tmpPkXk = tmpPkXk + inv(LclPk(:,:,i))*LclXk(:,i) - inv(LclPkp(:,:,i))*LclXkp(:,i); */
		MtrMtrMulti(iDim,iDim,1,iPk,pKf[i].xk,iPkXk);
		MtrMtrMulti(iDim,iDim,1,iPkp,pPr[i].xk,iPkpXkp);
		MtrMtrMinus(iDim,1,iPkXk,iPkpXkp,dPX);
		MtrMtrPlus(iDim,1,dPX,tmpPX,tmpPX);
	}
	
	/* 分配内存 */
	iCPk =  (double *)malloc(sizeof(double) * iDim * iDim);
	PP  =   (double *)malloc(sizeof(double) * iDim * iDim);
	iCPkXk  = (double *)malloc(sizeof(double) * iDim);
	PXPX = (double *)malloc(sizeof(double) * iDim);

	/* tPkk = inv(inv(tPkp) + tmpPk); */
	MtrInvers(iDim,pCPr->pk[0],iCPk);
	MtrMtrPlus(iDim,iDim,iCPk,tmpPk,PP);
	MtrInvers(iDim,PP,pNew->pk[0]);

	/* tXkk = tPkk * (inv(tPkp)*tXkp + tmpPkXk); */
	MtrMtrMulti(iDim,iDim,1,iCPk,pCPr->xk,iCPkXk);
	MtrMtrPlus(iDim,1,iCPkXk,tmpPX,PXPX);
	MtrMtrMulti(iDim,iDim,1,pNew->pk[0],PXPX,pNew->xk);

	/* 释放内存 */
	free(tmpPk);
	free(tmpPX);
	free(dP);
	free(dPX);
	free(iPk);
	free(iPkp);
	free(iPkXk);
	free(iPkpXkp);
	free(iCPk);
	free(PP);
	free(iCPkXk);
	free(PXPX);
}
