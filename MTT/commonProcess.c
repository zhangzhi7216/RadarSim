/*
***************************************
����    ��commonProcess.c
����    ������������
����ʱ�䣺2012.04.14
�汾    ��1.0
��Ȩ    ��
*
***************************************
*/

/*
***************************************
*            ͷ�ļ�����               *
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
*          ȫ�ֱ�������               *
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
/* sMdlPara���ڴ��溽��ģ�Ͳ�������ǰ����CV��CA1��CA2����ģ��  */
MODELPARA pMdl[MODEL_NUM];
/*
***************************************
*          �ⲿ��������               *
***************************************
*/

/*
***************************************
*         ���ļ��ڵĺ�������          *
***************************************
*/

/*
***************************************
������  ��Initialize_Models
�����������˲�ģ�Ͳ�����ʼ����CV+CA1+CA2��
һΪ����ģ��CV
��Ϊ�͹����������ȼ���ģ��CA1
��ά�߹����������ȼ���ģ��CA2
����    ��pMdl��ָ���˲�ģ�Ͳ�����ָ��
����ֵ  ����
***************************************
*/
void Initialize_Models(MODELPARA *pMdl)
{
	/* TΪ������� */
	double T = SAMPLE_PERIOD;

	/* fCV3FkΪ��ά�˲�ʱCVģ�͵�ϵͳת�ƾ���
	* fCV3QkΪ��ά�˲�ʱCVģ�͵Ĺ�����������
	* fCV3PkΪ��ά�˲�ʱCVģ�͵ĳ�ʼЭ������ */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4),0.5*pow(T,3),0,0.5*pow(T,3),pow(T,2),0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,90000,0,0,0,0};

	/* fCA3FkΪ��ά�˲�ʱCAģ�͵�ϵͳת�ƾ���
	* fCA3QkΪ��ά�˲�ʱCAģ�͵Ĺ�����������
	* fCA3PkΪ��ά�˲�ʱCAģ�͵ĳ�ʼЭ������ */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	double fCA3Qk[9] = {pow(T,5)/20,pow(T,4)/8,pow(T,3)/6,pow(T,4)/8,pow(T,3)/6,pow(T,2)/2,pow(T,3)/6,pow(T,2)/2,T};
	double fCA3Pk[9] = {9000000,0,0,0,90000,0,0,0,900};

	/* ������� */
	double fHk[3][9];

	/* ��ʱ���� */
	double fTmp[9];

	/* ��ʼ��Hk���� */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* ��ʼ��ģ�����ͣ������CVģ�ͣ�����1������Ϊ0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* ��ʼ��CVģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* ��ʼ��CA1ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* ��ʼ��CA2ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);
}

/*
***************************************
������   ��Select_Measure
�������� ����֡����������ѡȡ���ڴ�����iSnsIdx��iTgtIdx��Ŀ���������Ϣ
����     ��pMeas��ָ�����������ļ���ָ��
iTgtIdx�������ţ���ΧΪ[0,TARGET_NUM)
iSnsIdx����������,��ΧΪ[0,SENSOR_NUM)������-1ʱ��ʾ��Ҫѡȡ���д�������������Ϣ
����ֵ   ��pTgtZk,ָ��ǰ֡ĳһĿ�������������Ϣ
***************************************
*/
MEASINFO* Select_Measure(MEASINFO *pMeas,int iTgtIdx,int iSnsIdx,int *iSnrNum)
{
	/* pTgtZk,ָ�򺽼���������Ϣ;pTmpZk��pNxZk,��ʱ���� */
	MEASINFO *pTgtZk,*pTmpZk,*pNxZk;

	/* ������ʼ�� */
	*iSnrNum = 0;
	pTgtZk = NULL;
	pTmpZk = NULL;
	pNxZk = pMeas;

	while (pNxZk != NULL){

		if (iSnsIdx == -1){
			/* iSnsIdx����-1����ʾѡȡĿ������д�������õ�������Ϣ */
			if(pNxZk->tidx == iTgtIdx){
				(*iSnrNum)++;

				pTmpZk = (MEASINFO *)malloc(sizeof(MEASINFO));
				Assign_MeasInfo(pTmpZk,pNxZk);

				pTmpZk->next = pTgtZk;
				pTgtZk = pTmpZk;
			}
		}else{
			/* �����������ѡ���Ӧ�Ĵ�����������Ϣ */
			if(pNxZk->tidx == iTgtIdx && pNxZk->sidx == iSnsIdx){
				(*iSnrNum)++;

				pTmpZk = (MEASINFO *)malloc(sizeof(MEASINFO));
				Assign_MeasInfo(pTmpZk,pNxZk);

				pTmpZk->next = pTgtZk;
				pTgtZk = pTmpZk;
			}
		}

		/* ָ����һ��������Ϣ */
		pNxZk = pNxZk->next;
	}

	return pTgtZk;
}
/*
***************************************
������   ��Assign_MeasInfo
�������� ��������Ϣ��ֵ
����     ��pLeft��ָ��������Ϣ��ֵ
pRight��ָ��������Ϣ��ֵ
����ֵ   ����
***************************************
*/
void Assign_MeasInfo(MEASINFO *pLeft,MEASINFO *pRight)
{
	/* ��ʱ�������� */
	int i;

	/* �������źͺ����ø�ֵ */
	pLeft->sidx = pRight->sidx;
	pLeft->tidx = pRight->tidx;

	/* �����������Ϣ��ֵ */
	for (i=0; i<MEAS_DIM; i++){
		pLeft->meas[i] = pRight->meas[i];
		pLeft->nstd[i] = pRight->nstd[i];
	}

	/* ��һ����ָ�븳ֵ */
	pLeft->next = pRight->next;
}

/*
***************************************
������   ��Release_MeasInfo
�������� ���ͷ�������Ϣ
����     ��pMeas��ָ����Ҫ�ͷŵ�������Ϣ
����ֵ   ����
***************************************
*/
void Release_MeasInfo(MEASINFO *pMeas)
{
	MEASINFO *pMs;
	pMs = pMeas;

	/* ѭ���ͷ�������Ϣ��ռ���ڴ�ռ� */
	while(pMs != NULL){
		pMs = pMs->next;
		free(pMeas);
		pMeas = pMs;
	}
}

/*
***************************************
������   ��Initialize_State
�������� ����ʼ���˲�״̬
����     ��pTrk��������Ϣ
pMeas��������Ϣ
pMdl��ģ����Ϣ
iSpt��֡��
����ֵ   ����
***************************************
*/
void Initialize_State(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSpt)
{
	/* ��ʱ�������� */
	int i,iCnt;

	/* fXk��ʱ����������״̬��Ϣ��fAvg��ʱ����������״̬��Ϣ��ֵ */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* ������Ϣ */
	MEASINFO *pZk;

	/* ��ʼ����ʱ���� */
	iCnt = 0;
	pZk = pMeas;

	/* ȡƽ��ֵ */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* ����������״̬��Ϣ��� */
	while(pZk != NULL){
		iCnt++;

		for (i=0; i<MEAS_DIM; i++){
			fAvg[i] += pZk->meas[i];
		}
		pZk = pZk->next;
	}

	/* ���״̬��Ϣ��ֵ */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] /= iCnt;
	}

	/* ��ʼ����ʱ���� */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}

	/* ��ʼ��״̬��Ϣ */
	if (iSpt == 0){
		/* ��һ֡����ʼ��λ����Ϣ */
		fXk[0] = fAvg[0];
		fXk[3] = fAvg[1];
		fXk[6] = fAvg[2];
	}else{
		/* �ڶ�֡����ʼ��λ����Ϣ���ٶ���Ϣ */
		fXk[0] = fAvg[0];
		fXk[3] = fAvg[1];
		fXk[6] = fAvg[2];

		fXk[1] = (fAvg[0] - pTrk->fsn.xk[0])/SAMPLE_PERIOD;
		fXk[4] = (fAvg[1] - pTrk->fsn.xk[3])/SAMPLE_PERIOD;
		fXk[7] = (fAvg[2] - pTrk->fsn.xk[6])/SAMPLE_PERIOD;
	}

	/* ��ʼ�������� */
	pTrk->idx = pMeas->tidx;

	/* ��ʼ��ģ�͸��� */
	pTrk->fMdlProb[0] = 0.4;
	pTrk->fMdlProb[1] = 0.3;
	pTrk->fMdlProb[2] = 0.3;

	/* Xk��Pk��ĳ�ʼ�� */
	for (i=0; i<MODEL_NUM; i++){
		/* ����ģ��Xk��Pk�ĳ�ʼ�� */
		MtrAssign(1,STATE_DIM,fXk,pTrk->imm[i].xk);
		MtrAssign(STATE_DIM,STATE_DIM,pMdl[i].pk[0],pTrk->imm[i].pk[0]);
	}

	/* �ں�����״̬��ʼ�� */
	MtrAssign(1,STATE_DIM,fXk,pTrk->fsn.xk);

}

/*
***************************************
������   ��Assign_Matrix2Block
�������� ������������pRight����pLeft���ض����ϣ��ÿ��ά����pRight������ͬ
����     ��iRowBeg,��ʼ�к�
iRowFin�������к�+1
iColBeg����ʼ�к�
iColFin�������к�+1
iLCol����������ά��
iRCol���Ҿ������ά��
pLeft����ֵ���󣬼�������ֵ�ľ���
pRight����ֵ���󣬼�Ҫ������ֵ�ľ���
����ֵ   ����
***************************************
*/
void Assign_Matrix2Block(int iRowBeg,int iRowFin,int iColBeg,int iColFin,int iLCol,int iRCol,double *pLeft,double *pRight)
{
	int i,j;
	int iLeft,iRight;

	for (i=iRowBeg; i<iRowFin; i++){
		for (j=iColBeg; j<iColFin; j++){
			/* �������Ҿ���Ķ�Ӧ�±�� */
			iLeft = i*iLCol + j;
			iRight = (i-iRowBeg)*iRCol + (j-iColBeg);

			pLeft[iLeft] = pRight[iRight];
		}
	}
}

/*
***************************************
������   ��Assign_Block2Block
�������� ��������pRight���ض��鸳��pLeft���ض�����,���Ǿ��������ͬ�ֿ�ĸ�ֵ
����     ��sLeft,�����Ŀ���Ϣ
sRight���Ҿ������Ϣ
pLeft����ֵ���󣬼�������ֵ�ľ���
pRight����ֵ���󣬼�Ҫ������ֵ�ľ���
����ֵ   ����
***************************************
*/
void Assign_Block2Block(BMINFO sLeft,BMINFO sRight,double *pLeft,double *pRight)
{
	int i,j;
	int iLeft,iRight;

	for (i=sLeft.iRowBeg; i<sLeft.iRowFin; i++){
		for (j=sLeft.iColBeg; j<sLeft.iColFin; j++){
			/* �������Ҿ���Ķ�Ӧ�±�� */
			iLeft = i*sLeft.iColDim + j;
			iRight = (sRight.iRowBeg + i -sLeft.iRowBeg) * sRight.iColDim + (sRight.iColBeg + j -sLeft.iColBeg) ;

			pLeft[iLeft] = pRight[iRight];
		}
	}
}


/*
***************************************
������   ��Assign_FltInfo
�������� ���˲���Ϣ��ֵ
����     ��pLeft��ָ����Ҫ����ֵ�˲���Ϣ
pRight��ָ��ֵ�˲���Ϣ
����ֵ   ����
***************************************
*/
void Assign_FltInfo(FLTINFO *pLeft,FLTINFO *pRight)
{
	MtrAssign(STATE_DIM,1,pRight->xk,pLeft->xk);
	MtrAssign(STATE_DIM,STATE_DIM,pRight->pk[0],pLeft->pk[0]);
}

/*
***************************************
������   ��Display_TrackInfo
�������� ����ʾ������Ϣ
����     ��pTrk��������Ϣָ��
����ֵ   ����
***************************************
*/
void Display_TrackInfo(TRACKINFO *pTrk)
{
	int i,j;

	for (i=0; i<TARGET_NUM; i++){
		printf("---------�����ţ�%d-----------\n",pTrk[i].idx);

		printf("����ģ�͵ĸ��ʷֱ��ǣ�");
		for (j=0; j<MODEL_NUM; j++){
			printf("%f\t",pTrk[i].fMdlProb[j]);
		}
		printf("\n");

		printf("��ǰ�ںϹ��Ƶ�״̬��");
		for (j=0; j<STATE_DIM; j++){
			printf("%f\t",pTrk[i].fsn.xk[j]);
		}
		printf("\n");
	}
}

/*
***************************************
������   ��Display_MeasInfo
�������� ����ʾ������Ϣ
����     ��pTrk��������Ϣָ��
����ֵ   ����
***************************************
*/
void Display_MeasInfo(MEASINFO *pMeas)
{
	MEASINFO *pTmp;
	pTmp = pMeas;

	while(pTmp != NULL){
		printf("��������%d��������%d\n",pTmp->sidx,pTmp->tidx);
		printf("���⣺%f\t%f\t%f\n",pTmp->meas[0],pTmp->meas[1],pTmp->meas[2]);
		printf("���⣺%f\t%f\t%f\n",pTmp->nstd[0],pTmp->nstd[1],pTmp->nstd[2]);

		pTmp = pTmp->next;
	}

}

/*
***************************************
������   ��Display_DblMatrix
�������� ����ʾ�����;���
����     ��pMtr��ָ�����
iRow����������
iCol����������
����ֵ   ����
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
������   ��Display_IntMatrix
�������� ����ʾ���;���
����     ��pMtr��ָ�����
iRow����������
iCol����������
����ֵ   ����
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
������  ��MTTFusion_Decision
���������������ں�����ж�
����    ��
pRight��
����ֵ  ����
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
		// �������ڵľ���������ϵ��λ��
/*		Tmp1 = PolToDesc(pEcho->msr);						   // �������ڵľ��Եѿ�������ϵ��λ��
		Relative_XYZ[0] = Tmp1[0] - sEcho_SelfPosition[0].msr[0]; // �������ڵ�������һ��ĵѿ�������ϵ��λ��
		Relative_XYZ[1] = Tmp1[1] - sEcho_SelfPosition[0].msr[1];
		Relative_XYZ[2] = Tmp1[2] - sEcho_SelfPosition[0].msr[2];

		Tmp2 = DescToPol(Relative_XYZ);							// �������ڵ�������һ�����������ϵ��λ��
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
			//		printf("ESM��TongLei����%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],i);
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


	/*          �����ʶ�� ��ESM���ݺ�TongLei���ݺϲ���һ�� ������洦�� */
	for (i=0;i<iEchoNum_RadarAIS;i++){
		sEcho_MTT[i].msr[0] = sEcho_RadarAIS[i].msr[0];
		sEcho_MTT[i].msr[1] = sEcho_RadarAIS[i].msr[1];
		sEcho_MTT[i].msr[2] = sEcho_RadarAIS[i].msr[2];
		sEcho_MTT[i].Type = sEcho_RadarAIS[i].Type;

		sEcho_MTT[i].Fusion_OrNot = sEcho_RadarAIS[i].Fusion_OrNot;

		//		printf("ESM��TongLei����%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],i);
	}

	for (j=0;j<iEchoNum_TongLei;j++){
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[0] = sEcho_TongLei[j].msr[0];
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[1] = sEcho_TongLei[j].msr[1];
		sEcho_MTT[j+iEchoNum_RadarAIS].msr[2] = sEcho_TongLei[j].msr[2];
		sEcho_MTT[j+iEchoNum_RadarAIS].Type = sEcho_TongLei[j].Type;

		sEcho_MTT[j+iEchoNum_RadarAIS].Fusion_OrNot = sEcho_TongLei[j].Fusion_OrNot;

		//		printf("ESM��TongLei����%lf, %lf,%lf,%d\n",sEcho_MTT[i].msr[0],sEcho_MTT[i].msr[1],sEcho_MTT[i].msr[2],j+iEchoNum_RadarAIS);
	}

	iEchoNum_Total = iEchoNum_RadarAIS + iEchoNum_TongLei;
}

/*
***************************************
������  ��InitiateTrack
����������ģ�Ͳ�����ʼ��
����    ����
����ֵ  ����
***************************************
*/
void InitiateTrack()
{
	int i = 0;
	int j = 0;
	double *Tmp = NULL;
	/* TΪ������� */
	double T = SAMPLE_PERIOD;

	/* ��ʱ�������� */
	int iCnt;

	/* fXk��ʱ����������״̬��Ϣ��fAvg��ʱ����������״̬��Ϣ��ֵ */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* fCV3FkΪ��ά�˲�ʱCVģ�͵�ϵͳת�ƾ���
	* fCV3QkΪ��ά�˲�ʱCVģ�͵Ĺ�����������
	* fCV3PkΪ��ά�˲�ʱCVģ�͵ĳ�ʼЭ������ */
	double Sigma_X = pow((double)PNOISE_X,2);
	double Sigma_Y = pow((double)PNOISE_Y,2);
	double Sigma_Z = pow((double)PNOISE_Z,2);

	/* fCV3FkΪ��ά�˲�ʱCVģ�͵�ϵͳת�ƾ���
	* fCV3QkΪ��ά�˲�ʱCVģ�͵Ĺ�����������
	* fCV3PkΪ��ά�˲�ʱCVģ�͵ĳ�ʼЭ������ */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4)*Sigma_X,0.5*pow(T,3)*Sigma_X,0,0.5*pow(T,3)*Sigma_X,pow(T,2)*Sigma_X,0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,900000,0,0,0,0};

	/* fCA3FkΪ��ά�˲�ʱCAģ�͵�ϵͳת�ƾ���
	* fCA3QkΪ��ά�˲�ʱCAģ�͵Ĺ�����������
	* fCA3PkΪ��ά�˲�ʱCAģ�͵ĳ�ʼЭ������ */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	// double fCA3Qk[9] = {pow(T,5)*Sigma_X/20,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,T*Sigma_X};
	double fCA2Qk[9] = {0.25*pow(T,4)*Sigma_Y, 0.5*pow(T,3)*Sigma_Y, 0.5*pow(T,2)*Sigma_Y, 0.5*pow(T,3)*Sigma_Y, pow(T,2)*Sigma_Y, T*Sigma_Y, 0.5*pow(T,2)*Sigma_Y, T*Sigma_Y, Sigma_Y};

	double fCA3Qk[9] = {0.25*pow(T,4)*Sigma_Z, 0.5*pow(T,3)*Sigma_Z, 0.5*pow(T,2)*Sigma_Z, 0.5*pow(T,3)*Sigma_Z, pow(T,2)*Sigma_Z, T*Sigma_Z, 0.5*pow(T,2)*Sigma_Z, T*Sigma_Z, Sigma_Z};
	double fCA3Pk[9] = {9000000,0,0,0,9000000,0,0,0,9000};

	/* ������� */
	double fHk[3][9];

	/* ��ʱ���� */
	double fTmp[9];
	MEASINFO pEcho_DataFusion;

	/* ��ʼ����ʱ���� */
	iCnt = 0;

	/* ȡƽ��ֵ */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* ��ʼ����ʱ���� */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}
	/* ��ʼ��Hk���� */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* ��ʼ��ģ�����ͣ������CVģ�ͣ�����1������Ϊ0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* ��ʼ��CVģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* ��ʼ��CA1ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA2Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* ��ʼ��CA2ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);

	/* ��ʼ��״̬��Ϣ */

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

			/* ��ʼ��ģ�͸��� */
			sTracks[i].fMdlProb[0] = 0.4;
			sTracks[i].fMdlProb[1] = 0.3;
			sTracks[i].fMdlProb[2] = 0.3;

			Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[i]);
			/* ��һ֡����ʼ��λ����Ϣ */
			fXk[0] = pEcho_DataFusion.meas[0];
			fXk[3] = pEcho_DataFusion.meas[1];
			fXk[6] = pEcho_DataFusion.meas[2];
			/* Xk��Pk��ĳ�ʼ�� */
			for (j=0; j<MODEL_NUM; j++){
				/* ����ģ��Xk��Pk�ĳ�ʼ�� */
				MtrAssign(1,STATE_DIM,fXk,sTracks[i].imm[j].xk);
				MtrAssign(STATE_DIM,STATE_DIM,pMdl[j].pk[0],sTracks[i].imm[j].pk[0]);
			}

			/* �ں�����״̬��ʼ�� */
			MtrAssign(1,STATE_DIM,fXk,sTracks[i].fsn.xk);

		}
		else{
			sTracks[i].idx = 0;
		}
	}

}


/*
***************************************
������  ��Track_Trim
������������������������
����    ��

����ֵ  ����
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
������  ��PolToDesc
����������������ϵת����ֱ������ϵ
����    ��pPol��������ָ��
����ֵ  ��pDesc��ֱ������ָ��
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
������  ��DescToPol
����������ֱ������ϵת����������ϵ
����    ��pDesc��ֱ������ָ��
����ֵ  ��pPol��������ָ��
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
������  ��Unbias_Conv
���������������������굽ֱ��������ƫת�� 
����    ��pTrk������ָ��
����ֵ  ��pEcho������ָ��
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
������  ��Unbias_Conv
���������������������굽ֱ��������ƫת�� 
����    ��pTrk������ָ��
����ֵ  ��pEcho������ָ��
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

	/*            ����Э������ϵ��          */

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
������  FilterPrediction
�����������˲�����
����    ����
����ֵ  ����
***************************************
*/
void FilterPrediction()
{
	int i;
	int Calcu_Dis = 0;
	/* ��ʱ���� */
	int j;
	int iSnr = 1;
	/* fValue�������ģ�͵���Ȼֵ��fChat�����ϸ���֮�� */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* ģ�������س�ʼ����Ļ���˲���Ϣ */
	FLTINFO sMix[MODEL_NUM];
	MEASINFO pEcho_DataFusion;

	for (i=0;i<MAX_TRACK;i++){
		if ( sTracks[i].idx != 0){

			Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[i]);
			/* ģ�������س�ʼ�� */
			Reinitialize_Condition(&sTracks[i].fMdlProb[0],fChat,&sTracks[i].imm[0],sMix);
			for (j=0; j<MODEL_NUM; j++)
				fValue[j] = IMM_Kalman_Filtering(&sTracks[i].imm[j],&sMix[j],&pEcho_DataFusion,pMdl[j],iSnr,&Dis[j],&Calcu_Dis);
			/* �����ں����ĵ�״̬��Ϣ */
			Renew_Fusion(&sTracks[i],fValue,fChat);
		}

		sTracks[i].Points = sTracks[i].Points + 1;
	}

}


/*
***************************************
������    GNN_Asso
��������������ڹ���
����    ����
����ֵ  ����
***************************************
*/
void GNN_Asso(int *iMethod)
{
	TRACKINFO *pTrkTmp = NULL;
	TRACKINFO *pTrkNew = NULL;
	ECHO *pEcho = NULL;
	int i,j,k,t;
	int Tused;                         //�����и��±�־λ
	int tracknum = g_iTrkNum;
	int measnum = iEchoNum_Total;
	int Mused[2*MAX_ECHO] = {0};         //�����и��±�־λ
	double Gate1,Gate2;
	double *Tmp = NULL;
	double Tmp2,Tmp3,Tmp4,Tmp5;
	int Tmp6;
	int Tmp7 = 5;
	double Distance[2*MAX_ECHO] = {0};   // ����ͳ�ƾ���
	int MNUM;							 // �������벨�ż�����
	int FusionNum;                       // �����ڱ���̽�ⷶΧ�ļ�����
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
	/*         �����趨      */
	if ( Spt < 2){
		Gate = Gate1;
	}
	else{
		Gate = Gate2;
	}
	/*         ���ݹ���      */
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
					Distance[j] = Dis;  //��¼ÿ��������ں�����ͳ�ƾ���
					Tused = 1;			//ָʾ�ú��������������벨��
					Mused[j] = 1;		//�жϷֱ��ǵ�ǰ���������е���Щ�������벨��
					MNUM++;				//ͳ�ƹ��м����������벨��
					if (pEcho->Fusion_OrNot == 1)   //�����뱾����̽�ⷶΧ�� ����� ��������ж��Ƿ��������벨�ŵ����ⶼ���뱾��̽�ⷶΧ��
						FusionNum++;   

					if ( (pEcho->Type == 4) && (pTrkTmp->Type == 3)){
						pTrkTmp->Civi_Num++;
					}
				}
			}
			/*         ������ʼ ������ 3/5��ʼ     */
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
			/*       ����ɾ�� �ж��Ƿ�ɾ������ ������10֡��δ��������� ��ɾ��      */
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
			/*           �Խ����ں��㷨���Ƕ�Ŀ������㷨 �����ж� */
			iTmp = 0;
			Tmp5 = 0;
			Tmp6 = 0;
			if ( MNUM > 0){
				if ( ( FusionNum == MNUM) && ( MNUM > 1) &&( *iMethod < 5 )){  // ���뱾��̽�ⷶΧ�� �� �ж���������뵱ǰ����������
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
					/*       ����û�н����ں��㷨�������ҵ�ͳ�ƾ�����С������      */
					for (k = 0;k < iEchoNum_Total;k++){            //        �ҵ���һ����0��ͳ�ƾ���
						if (Distance[k] != 0){
							Tmp5 = Distance[k];
							break;
						}
					}

					for (k = 0;k < iEchoNum_Total;k++){            //        �ҵ���С�ķ�0��ͳ�ƾ���
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
			for (k = 0;k < iEchoNum_Total;k++){            //�ҵ���С�ķ�0��ͳ�ƾ���
				Distance[k] = 0;
			}

		}
	}
	/*       �����ϲ� �ж��Ƿ�úϲ����� ������Share_N����ϲ����� ��ϲ�����      */
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
	/*       ��ʼ�º���      */
	for (j=0;j<iEchoNum_Total;j++){
		if ( Mused[j] == 0 ){
			InitNewTrack(Spt,j);
		}
	}

	Spt++;
}

/*
***************************************
������  ��Statistic_Distance
�������������㲻ͬ�����µ�ͳ�ƾ����
��ϢЭ������
����    ��pTrkTmp������ָ��
pEcho������ָ��
Dis��ͳ�ƾ���
Lis����ϢЭ��������ʽֵ
����ֵ  ����
***************************************
*/
void Statistic_Distance(TRACKINFO *pTrkTmp,ECHO *pEcho,double *Distance,double *Lis)
{

	int j;
	int iSnr = 1;
	int Cacl_Distance = 1;
	/* fValue�������ģ�͵���Ȼֵ��fChat�����ϸ���֮�� */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* ģ�������س�ʼ����Ļ���˲���Ϣ */
	FLTINFO sMix[MODEL_NUM];
	MEASINFO pEcho_DataFusion;
	TRACKINFO *pTrk = NULL;
	pTrk = (TRACKINFO*)malloc( sizeof(TRACKINFO) );

	TrksAssign(pTrk,pTrkTmp);
	Unbias_Conv(&pEcho_DataFusion,pEcho);

	/* ģ�������س�ʼ�� */
	Reinitialize_Condition(pTrk->fMdlProb,fChat,pTrk->imm,sMix);
	for (j=0; j<MODEL_NUM; j++)
		fValue[j] = IMM_Kalman_Filtering(&pTrk->imm[j],&sMix[j],&pEcho_DataFusion,pMdl[j],iSnr,&Dis[j],&Cacl_Distance);
	/* �����ں����ĵ�״̬��Ϣ */
	Renew_Fusion(pTrk,fValue,fChat);
	*Distance = fChat[0]*Dis[0] + fChat[1]*Dis[1] + fChat[2]*Dis[2];

	free(pTrk);
}


/*
***************************************
������  ��InitNewTrack
����������ģ�Ͳ�����ʼ��
����    ����
����ֵ  ����
***************************************
*/
void InitNewTrack(int iSpt,int MeasN)
{
	int i = 0;
	int j = 0;
	int Tmp3;
	double *Tmp = NULL;
	/* TΪ������� */
	double T = SAMPLE_PERIOD;

	/* ��ʱ�������� */
	int iCnt;

	/* fXk��ʱ����������״̬��Ϣ��fAvg��ʱ����������״̬��Ϣ��ֵ */
	double fXk[STATE_DIM],fAvg[MEAS_DIM];

	/* fCV3FkΪ��ά�˲�ʱCVģ�͵�ϵͳת�ƾ���
	* fCV3QkΪ��ά�˲�ʱCVģ�͵Ĺ�����������
	* fCV3PkΪ��ά�˲�ʱCVģ�͵ĳ�ʼЭ������ */
	double Sigma_X = pow((double)PNOISE_X,2);
	double Sigma_Y = pow((double)PNOISE_Y,2);
	double Sigma_Z = pow((double)PNOISE_Z,2);

	/* fCV3FkΪ��ά�˲�ʱCVģ�͵�ϵͳת�ƾ���
	* fCV3QkΪ��ά�˲�ʱCVģ�͵Ĺ�����������
	* fCV3PkΪ��ά�˲�ʱCVģ�͵ĳ�ʼЭ������ */
	double fCV3Fk[9] = {1,T,0,0,1,0,0,0,0};
	double fCV3Qk[9] = {0.25*pow(T,4)*Sigma_X,0.5*pow(T,3)*Sigma_X,0,0.5*pow(T,3)*Sigma_X,pow(T,2)*Sigma_X,0,0,0,0};
	double fCV3Pk[9] = {9000000,0,0,0,90000,0,0,0,0};

	/* fCA3FkΪ��ά�˲�ʱCAģ�͵�ϵͳת�ƾ���
	* fCA3QkΪ��ά�˲�ʱCAģ�͵Ĺ�����������
	* fCA3PkΪ��ά�˲�ʱCAģ�͵ĳ�ʼЭ������ */
	double fCA3Fk[9] = {1,T,0.5*pow(T,2),0,1,T,0,0,1};
	double fCA3Qk[9] = {pow(T,5)*Sigma_X/20,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,4)*Sigma_X/8,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,pow(T,3)*Sigma_X/6,pow(T,2)*Sigma_X/2,T*Sigma_X};
	double fCA3Pk[9] = {9000000,0,0,0,90000,0,0,0,900};

	/* ������� */
	double fHk[3][9];

	/* ��ʱ���� */
	double fTmp[9];
	MEASINFO pEcho_DataFusion;

	/* ��ʼ����ʱ���� */
	iCnt = 0;

	/* ȡƽ��ֵ */
	for (i=0; i<MEAS_DIM; i++){
		fAvg[i] = 0;
	}

	/* ��ʼ����ʱ���� */
	for (i=0; i<STATE_DIM; i++){
		fXk[i] = 0;
	}
	/* ��ʼ��Hk���� */
	MtrInitialize(3,9,fHk[0]);
	fHk[0][0] = 1;
	fHk[1][3] = 1;
	fHk[2][6] = 1;

	/* ��ʼ��ģ�����ͣ������CVģ�ͣ�����1������Ϊ0 */
	pMdl[0].isCV = CVMODEL;
	pMdl[1].isCV = NOCVMODEL;
	pMdl[1].isCV = NOCVMODEL;

	/* ��ʼ��CVģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCV3Fk,pMdl[0].fk[0]);
	MtrDiag(3,9,fCV3Pk,pMdl[0].pk[0]);
	MtrDiag(3,9,fCV3Qk,pMdl[0].qk[0]);
	MtrAssign(3,9,fHk[0],pMdl[0].hk[0]);

	/* ��ʼ��CA1ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[1].fk[0]);
	MtrCnstMulti(3,3,9,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[1].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[1].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[1].hk[0]);

	/* ��ʼ��CA2ģ�Ͳ�������Ϊ��άģ�Ͳ��� */
	MtrDiag(3,9,fCA3Fk,pMdl[2].fk[0]);
	MtrCnstMulti(3,3,49,fCA3Qk,fTmp);
	MtrDiag(3,9,fTmp,pMdl[2].qk[0]);
	MtrDiag(3,9,fCA3Pk,pMdl[2].pk[0]);
	MtrAssign(3,9,fHk[0],pMdl[2].hk[0]);

	/* ��ʼ��״̬��Ϣ */

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

	/* ��ʼ��ģ�͸��� */
	sTracks[Tmp3].fMdlProb[0] = 0.4;
	sTracks[Tmp3].fMdlProb[1] = 0.3;
	sTracks[Tmp3].fMdlProb[2] = 0.3;

	Unbias_Conv(&pEcho_DataFusion,&sEcho_RadarAIS[MeasN]);
	/* ��һ֡����ʼ��λ����Ϣ */
	fXk[0] = pEcho_DataFusion.meas[0];
	fXk[3] = pEcho_DataFusion.meas[1];
	fXk[6] = pEcho_DataFusion.meas[2];
	/* Xk��Pk��ĳ�ʼ�� */
	for (j=0; j<MODEL_NUM; j++){
		/* ����ģ��Xk��Pk�ĳ�ʼ�� */
		MtrAssign(1,STATE_DIM,fXk,sTracks[Tmp3].imm[j].xk);
		MtrAssign(STATE_DIM,STATE_DIM,pMdl[j].pk[0],sTracks[Tmp3].imm[j].pk[0]);
	}

	/* �ں�����״̬��ʼ�� */
	MtrAssign(1,STATE_DIM,fXk,sTracks[Tmp3].fsn.xk);
	sTracks[Tmp3].Points = sTracks[Tmp3].Points + 1;
}

/*
***************************************
������  ��TrkDel
�������������ݺ���ɾ�����ɾ������
����    ����
����ֵ  ����
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

	/* ���س�ʼ�����ģ���˲���Ϣ������ʱ���� */
	Assign_FltInfo(&pTLeft->fsn,&pTRight->fsn);
	Assign_FltInfo(&pTLeft->imm[0],&pTRight->imm[0]);
	Assign_FltInfo(&pTLeft->imm[1],&pTRight->imm[1]);
	Assign_FltInfo(&pTLeft->imm[2],&pTRight->imm[2]);

}

/*
***************************************
������  ��InitiateTrack
����������ģ�Ͳ�����ʼ��
����    ����
����ֵ  ����
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
������  ��Transmit_Trk
����������������DLL�����
����    ����
����ֵ  ����
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