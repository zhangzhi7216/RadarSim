/*
***************************************
����    ��fusionFilter.c
����    ���ںϹ����˲���
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
#include "commonProcess.h"
#include "matrixOperation.h"
#include "fusionFilter.h"

/*
***************************************
*          ȫ�ֱ�������               *
***************************************
*/

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
������  ��IMM_Filtering
��������������ʽ��ģ���˲�������
����    ��pTrk��������Ϣ
          pMeas��������Ϣ
		  pMdl��ģ�Ͳ���
		  iMethod��ѡ����ںϷ���
		  iSpt��֡��
����ֵ  ����
***************************************
*/
void IMM_Filtering(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSnr,int iMethod,int iSpt)
{
	/* ��ʱ���� */
	int i;
	int Calcu_Dis = 0;
	/* fValue�������ģ�͵���Ȼֵ��fChat�����ϸ���֮�� */
	double fValue[MODEL_NUM],fChat[MODEL_NUM];
	double Dis[MODEL_NUM];
	/* ģ�������س�ʼ����Ļ���˲���Ϣ */
	FLTINFO sMix[MODEL_NUM];

	/* ģ�������س�ʼ�� */
	Reinitialize_Condition(pTrk->fMdlProb,fChat,pTrk->imm,sMix);

	/* ģ�������˲� */
	/* ��Ȼ�������㼰�˲����� */
//	printf("������ں��˲��㷨Ϊ %d\n",iMethod);
	switch (iMethod){
	case 1:
		/* �����˲� */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Parallel_Filtering(&pTrk->imm[i],sMix[i],pMeas,pMdl[i],iSnr);
		break;
	case 2:
		/* ����ѹ���˲� */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Data_Compression_Filtering(&pTrk->imm[i],sMix[i],pMeas,pMdl[i],iSnr);
		break;
	case 3:
		/* Bar-Shalom Campo �ں��㷨 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Bar_Shalom_Campo_Fusion(pTrk,sMix[i],pMeas,pMdl[i],iSnr,i);
		break;
	case 4:
		/* Bar-Shalom Campo �ں��㷨 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = Optimal_Distributed_Fusion_Without_Ack(pTrk,sMix[i],pMeas,pMdl[i],iSnr,i);
		break;
	case 5:
		/* �������⽻��ʽ��ģ���˲��㷨 */
		for (i=0; i<MODEL_NUM; i++)
			fValue[i] = IMM_Kalman_Filtering(&pTrk->imm[i],&sMix[i],pMeas,pMdl[i],iSnr,&Dis[i],&Calcu_Dis);
		break;
	default:
		/* ��ʾ�������Ϣ���� */
		printf("������ں��˲��㷨����\n");
		break;
	}

	/* �����ں����ĵ�״̬��Ϣ */
	Renew_Fusion(pTrk,fValue,fChat);
}


/*
***************************************
������  ��Parallel_Filtering
���������������˲�
����    ��pTrk�����µ��˲���Ϣ
          pMix����Ϲ�����Ϣ
		  pMeas��������Ϣ
		  sMdl,ģ�Ͳ���
����ֵ  ����
***************************************
*/
double Parallel_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr)
{
	/* iMeasNum���������Dk��Ȼֵ */
	int iMeasNum;
	double Dk;

	/* fRnΪ������������ */
	RKNS *fRn;

	/* sPred����һ��Ԥ����Ϣ */
	FLTINFO sPred;

	/* ��ʼ��Rk���� */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* ����һ��Ԥ�� */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* ����Ԥ��вЭ�������Լ���Ȼ�����ļ��� */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* �˲����̺����Э�������ļ��� */
	Update_StateByParallelFiltering(pFlt,sPred,sMdl,fRn,pMeas,iMeasNum);

	/* �ͷ��ڴ� */
	free(fRn);

	return Dk;
}

/*
***************************************
������  ��Data_Compression_Filtering
��������������ѹ���˲�
����    ��pTrk�����µ��˲���Ϣ
          pMix����Ϲ�����Ϣ
		  pMeas��������Ϣ
		  sMdl,ģ�Ͳ���
����ֵ  ����
***************************************
*/
double Data_Compression_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr)
{
	/* iMeasNum���������Dk��Ȼֵ */
	int iMeasNum;
	double Dk;

	/* fRnΪ������������ */
	RKNS *fRn;

	/* sPred����һ��Ԥ����Ϣ */
	FLTINFO sPred;

	/* ѹ�����α���ⷽ�̲�����Ϣ */
	COMPINFO sFgd;

	/* ��ʼ��Rk���� */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* ����һ��Ԥ�� */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* ����α���⼰��α������� */
	Generate_ForgedMeas(&sFgd,pMeas,fRn,sMdl,iMeasNum);

	/* ������Ȼ���� */
	Dk = Calculate_Likelihood(sPred,sFgd.Hk[0],sFgd.Rk[0],sFgd.Zk,1);

	/* �˲����̺����Э�������ļ��� */
	Update_StateByCompressedFiltering(pFlt,sPred,sFgd,iMeasNum);

	return Dk;
}

/*
***************************************
������  ��Bar_Shalom_Campo_Fusion
����������Bar-Shalom-Campo�ں��㷨
����    ��pTrk��������Ϣ
          pMix����Ϲ�����Ϣ
		  pMeas��������Ϣ
		  sMdl,ģ�Ͳ���
		  iMdl,ģ�����
����ֵ  ����
***************************************
*/
double Bar_Shalom_Campo_Fusion(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl)
{
	/* i,jΪ����������iTgt��ʾ�����ţ�iMeasNum��ʾ������� */
	int i,j,iTgt,iMeasNum,iSnrNum;
	int Calcu_Dis = 0;
	/* Dk��Ȼ����ֵ */
	double Dk;

	/* P��Э�������pXk��ά״̬��Ϣ��pIkΪ��ά��λ���� */
	double *P,*pXk,*pIk;
	double Dis;
	/* fRnΪ������������ */
	RKNS *fRn;

	/* sFltTmp�˲���Ϣ��ʱ������
	   sPred�����ں�����һ��Ԥ���˲���Ϣ��
	   sSnsPr���������������һ��Ԥ����Ϣ��
	   sFlt���洢����������˲���Ϣ����ʱ���� */
	FLTINFO sFltTmp,sPred;
	FLTINFO *sSnsPr,*sFlt,*sSnsFlt;

	/* sCvFlt ���潵ά����˲���Ϣ��sCvNew���潵ά���ں������˲���Ϣ */
	CVFLTINFO *sCvFlt,sCvNew;

	/* ������Ϣ��ʱ���� */
	MEASINFO *pMs;

	/* �����ڴ�ռ� */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);

	sSnsPr = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sSnsFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);

	sCvFlt = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);

	/* ��ʼ��Rk���� */

	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* ����һ��Ԥ�� */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* ����Ԥ��вЭ�������Լ���Ȼ�����ļ��� */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* �����������ľֲ�����״̬���� */
	pMs = pMeas;
	if (pMeas != NULL)
		iTgt = pMs->tidx;

	for (i=0; i<iSnr; i++){
		/* ѡȡ���ڸú����ô����������� */
		pMs = Select_Measure(pMeas,iTgt,i,&iSnrNum);

		/* ��ģ��iMdl���˲���Ϣ������ʱ���� */
		Assign_FltInfo(&sFltTmp,&pTrk->imm[iMdl]);

		/* �������˲�����ô����������˲���Ϣ��һ��Ԥ����Ϣ */
		Kalman_Filtering(&sFltTmp,&sSnsPr[i],sMdl,fRn[i].Rk[0],pMs->meas,&Dis,&Calcu_Dis);

		/* ����ʱ������ֵ����������iģ��iMdl���˲���Ϣ */
		Assign_FltInfo(&sSnsFlt[i],&sFltTmp);

		pMs = pMs->next;
	}

	/* ����CVģ�͵��˲�״̬��Ϣ���н�ά�������Ա����� */
	if (sMdl.isCV == CVMODEL){
		/* ΪpXk��pIk�����ڴ� */
		pXk = (double *)malloc(sizeof(double) * CV_STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * CV_STATE_DIM * CV_STATE_DIM * iSnr);

		/* �������˲���Ϣ��ά */
		for(j=0; j<iSnr; j++)
			Decrease_Dimension(&sSnsFlt[j],&sCvFlt[j],1);

		/* ���ݷ����Ͻ缼����öԽ����͵Ļ�Э������ */
		P = ErrVar_UpperBoundByCV(sCvFlt,CV_STATE_DIM,iSnr);

		/* �����ά��Xk��IK�� */
		Generate_CombStateNEyeByCV(pXk,pIk,sCvFlt,CV_STATE_DIM,iSnr);

		/* �����˲���Ϣ */
		Update_StateByML(sCvNew.xk,sCvNew.pk[0],P,pXk,pIk,CV_STATE_DIM,iSnr);

		/* ��ԭά�� */
		Increase_Dimension(&pTrk->imm[iMdl],&sCvNew,1);
	}else{
		/* ΪpXk��pIk�����ڴ� */
		pXk = (double *)malloc(sizeof(double) * STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * STATE_DIM * STATE_DIM * iSnr);

		/* �������˲���Ϣ��ά */
		for(j=0; j<iSnr; j++)
			Assign_FltInfo(&sFlt[j],&sSnsFlt[j]);

		/* ���ݷ����Ͻ缼����öԽ����͵Ļ�Э������ */
		P = ErrVar_UpperBound(sFlt,STATE_DIM,iSnr);

		/* �����ά��Xk��IK�� */
		Generate_CombStateNEye(pXk,pIk,sFlt,STATE_DIM,iSnr);

		/* �����˲���Ϣ */
		Update_StateByML(pTrk->imm[iMdl].xk,pTrk->imm[iMdl].pk[0],P,pXk,pIk,STATE_DIM,iSnr);
	}

	/* �ͷſռ� */
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
������  ��Optimal_Distributed_Fusion_Without_Ack
�����������������������ŷֲ�ʽ�����ں�
����    ��pTrk��������Ϣ
          pMix����Ϲ�����Ϣ
		  pMeas��������Ϣ
		  sMdl,ģ�Ͳ���
		  iMdl,ģ�����
����ֵ  ����
***************************************
*/
double Optimal_Distributed_Fusion_Without_Ack(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl)
{
	/* i,jΪ����������iTgt��ʾ�����ţ�iMeasNum��ʾ������� */
	int i,j,iTgt,iMeasNum,iSnrNum;
	int Calcu_Dis = 0;
	/* Dk��Ȼ����ֵ */
	double Dk;

	/* pXk��ά״̬��Ϣ��pIkΪ��ά��λ���� */
	double *pXk,*pIk;
	double Dis;
	/* fRnΪ������������ */
	RKNS *fRn;

	/* sFltTmp�˲���Ϣ��ʱ������
	   sPred�����ں�����һ��Ԥ���˲���Ϣ��
	   sSnsPr���������������һ��Ԥ����Ϣ��
	   sFlt���洢����������˲���Ϣ����ʱ���� */
	FLTINFO sFltTmp,sPred;
	FLTINFO *sSnsPr,*sFlt,*sSnsFlt;

	/* sCvFlt���潵ά����˲���Ϣ��
	   sCvPr����������CVģ�͵�һ��Ԥ����Ϣ��
       sCvNew���潵ά���ں������˲���Ϣ��
	   sCvPred�����ں�����һ��Ԥ����Ϣ*/
	CVFLTINFO sCvNew,sCvPred;
	CVFLTINFO *sCvFlt,*sCvPr;

	/* ������Ϣ��ʱ���� */
	MEASINFO *pMs;

	/* �����ڴ�ռ� */
	fRn = (RKNS *)malloc(sizeof(RKNS) * iSnr);

	sSnsPr = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);
	sSnsFlt = (FLTINFO *)malloc(sizeof(FLTINFO) * iSnr);

	sCvFlt = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);
	sCvPr = (CVFLTINFO *)malloc(sizeof(CVFLTINFO) * iSnr);

	/* ��ʼ��Rk���� */
	iMeasNum = Initialize_Rk(pMeas,fRn);

	/* ����һ��Ԥ�� */
	Predict_OneStep(&sPred,pMix,sMdl);

	/* ����Ԥ��вЭ�������Լ���Ȼ�����ļ��� */
	Dk = Calculate_LikelihoodByAugmentedMeas(sPred,pMeas,sMdl,fRn,iMeasNum);

	/* �����������ľֲ�����״̬���� */
	pMs = pMeas;
	if (pMeas != NULL)
		iTgt = pMs->tidx;

	for (i=0; i<iSnr; i++){
		/* ѡȡ���ڸú����ô����������� */
		pMs = Select_Measure(pMeas,iTgt,i,&iSnrNum);

		/* ��ģ��iMdl���˲���Ϣ������ʱ���� */
		Assign_FltInfo(&sFltTmp,&pTrk->imm[iMdl]);

		/* �������˲�����ô����������˲���Ϣ��һ��Ԥ����Ϣ */
		Kalman_Filtering(&sFltTmp,&sSnsPr[i],sMdl,fRn[i].Rk[0],pMs->meas,&Dis,&Calcu_Dis);

		/* ����ʱ������ֵ����������iģ��iMdl���˲���Ϣ */
		Assign_FltInfo(&sSnsFlt[i],&sFltTmp);

		pMs = pMs->next;
	}

	/* ����CVģ�͵��˲�״̬��Ϣ���н�ά�������Ա����� */
	if (sMdl.isCV == CVMODEL){
		/* ΪpXk��pIk�����ڴ� */
		pXk = (double *)malloc(sizeof(double) * CV_STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * CV_STATE_DIM * CV_STATE_DIM * iSnr);

		/* ��ά */
		for(j=0; j<iSnr; j++)
			Decrease_Dimension(&sSnsFlt[j],&sCvFlt[j],1);
		Decrease_Dimension(sSnsPr,sCvPr,iSnr);
		Decrease_Dimension(&sPred,&sCvPred,1);

		/* �����ں������˲���Ϣ */
		Update_StateByOptimalDistributionByCV(&sCvNew,sCvFlt,sCvPr,&sCvPred,CV_STATE_DIM,iSnr);

		/* ��ά */
		Increase_Dimension(&pTrk->imm[iMdl],&sCvNew,1);
	}else{
		/* ΪpXk��pIk�����ڴ� */
		pXk = (double *)malloc(sizeof(double) * STATE_DIM * iSnr);
		pIk = (double *)malloc(sizeof(double) * STATE_DIM * STATE_DIM * iSnr);

		/* ת���������������˲���Ϣ */
		for(j=0; j<iSnr; j++)
			Assign_FltInfo(&sFlt[j],&sSnsFlt[j]);

		/* �����ں������˲���Ϣ */
		Update_StateByOptimalDistribution(&pTrk->imm[iMdl],sFlt,sSnsPr,&sPred,STATE_DIM,iSnr);
	}

	/* �ͷ��ڴ� */
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
������  ��IMM_Kalman_Filtering
�������������ڽ���ʽ��ģ�͵Ŀ������˲�,
          �ú���Ĭ����������ֻ��һ��
����    ��pTrk���˲���k+1ʱ�̵�״̬��Ϣ
          pFltPred��kʱ�̵�һ��Ԥ��ֵ
		  sMdl,ģ�Ͳ���
		  iSnr����������
����ֵ  ��Dk����Ȼ����ֵ
***************************************
*/
double IMM_Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,double *Dis,int *Calcu_Dis)
{
	int i;
	double fValue;
	FLTINFO sFltTmp,sPred;
	double Rk[MEAS_DIM][MEAS_DIM];
	/* fVar ���������������� */
	double fVar[MEAS_DIM];

	if (iSnr != 1){
		printf("ע�⣺���㷨Ĭ��ֻ��һ��������һ��������������ȷ���Ƿ����Ҫ��\n");

	}

	/* ����׼�Ϊ���� */
	for (i=0; i<MEAS_DIM; i++){
		fVar[i] = pMeas->nstd[i] * pMeas->nstd[i];
	}
	VectDiag(MEAS_DIM,fVar,Rk[0]);
	// VectDiag(MEAS_DIM,pMeas->nstd,Rk[0]);
	/* ���س�ʼ�����ģ���˲���Ϣ������ʱ���� */
	Assign_FltInfo(&sFltTmp,pMix);

	/* �������˲�����ô����������˲���Ϣ��һ��Ԥ����Ϣ */
	fValue = Kalman_Filtering(&sFltTmp,&sPred,sMdl,Rk[0],pMeas->meas,Dis,Calcu_Dis);

	/* ����ʱ������ֵ����������iģ��iMdl���˲���Ϣ */
	Assign_FltInfo(pFlt,&sFltTmp);

	return fValue;

}

/*
***************************************
������  ��Kalman_Filtering
�����������������˲�
����    ��pTrk���˲���k+1ʱ�̵�״̬��Ϣ
          pFltPred��kʱ�̵�һ��Ԥ��ֵ
		  sMdl,ģ�Ͳ���
		  Rk����������
		  pMs������
����ֵ  ����
***************************************
*/
double Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pFltPred,MODELPARA sMdl,double *Rk,double *pMs,double *Dis,int *Calcu_Dis)
{
	/* �˲��������õ����м���� */
	int i;
	double HkPkp[3][9],tHk[9][3],HPH[3][3],Sk[3][3];
	double PktHk[9][3],iSk[3][3],Kk[9][3];
	double HkXkp[3],Vk[3];
	double KkVk[9];
	double KkRk[9][3],KRK[9][9],tKk[3][9],Arr[9],Eye[9][9];
	double KkHk[9][9],IKH[9][9],tIKH[9][9],IKHP[9][9],IPI[9][9];
	double VkiSk[3],VSV,detS,Dk;

	/* ����һ��Ԥ�� */
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

	/* ����Gauss�����µ���Ȼ����ֵ */
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

	/* ���ɵ�λ�� */
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
������  ��Reinitialize_Condition
����������ģ�������س�ʼ��
����    ��fMdlProb,ģ�͸��ʣ�
          fChat����ϸ����м������
		  pOldFlt�����ǰ�˲���Ϣ��
		  pNewFlt����Ϻ��˲���Ϣ��
����ֵ  ����
***************************************
*/
void Reinitialize_Condition(double *fMdlProb,double *fChat,FLTINFO *pOldFlt,FLTINFO *pNewFlt)
{
	/* �������õ����м���� */
	int i,j;
	double tPt[MODEL_NUM*MODEL_NUM],fMixProb[MODEL_NUM][MODEL_NUM];
	double fXkTmp[STATE_DIM];
	double fXeXe[STATE_DIM][STATE_DIM],fPkTmp[STATE_DIM][STATE_DIM],fPkPrb[STATE_DIM][STATE_DIM];

	/* ״̬�����Ϣ */
	struct xkerr{
		double Xe[STATE_DIM];
	}sXkErr[MODEL_NUM][MODEL_NUM];

	/* �����ϸ��� */
	/* ����Ʒ����ת�ƾ��� */
	double Pt[MODEL_NUM*MODEL_NUM] = {0.9,0.05,0.05,0.05,0.9,0.05,0.2,0.2,0.6};

	/* ����ģ�͸��� */
	MtrTrans(MODEL_NUM,MODEL_NUM,Pt,tPt);
	MtrMtrMulti(MODEL_NUM,MODEL_NUM,1,tPt,fMdlProb,fChat);
	for(i=0;i<MODEL_NUM;i++)
		for(j=0;j<MODEL_NUM;j++)
			fMixProb[i][j] = Pt[i*MODEL_NUM+j] *fMdlProb[i]/fChat[j];

	/* ��Ϲ��� */
	for (i=0; i<MODEL_NUM; i++){
		MtrInitialize(1,STATE_DIM,pNewFlt[i].xk);

		for (j=0; j<MODEL_NUM; j++){
			MtrCnstMulti(1,STATE_DIM,fMixProb[j][i],pOldFlt[j].xk,fXkTmp);
			MtrMtrPlus(1,STATE_DIM,fXkTmp,pNewFlt[i].xk,pNewFlt[i].xk);
		}
	}

	/* ������� */
	for (i=0; i<MODEL_NUM; i++){
		for(j=0; j<MODEL_NUM; j++){
			MtrMtrMinus(1,STATE_DIM,pOldFlt[i].xk,pNewFlt[j].xk,sXkErr[i][j].Xe);
		}	
	}

	/* ������Pk�� */
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
������  ��Initialize_Rk
������������ʼ��Rk����
����    ��pMeas,������Ϣ
          pRn��������������
����ֵ  ��iCnt���������
***************************************
*/
int Initialize_Rk(MEASINFO *pMeas,RKNS *pRn)
{
	/* i����������iCnt����������� */
	int i,iCnt;

	/* fVar ���������������� */
	double fVar[MEAS_DIM];

	/* ��ʱ���� */
	MEASINFO *pMsTmp;

	/* ��ʼ�� */
	iCnt = 0;
	pMsTmp = pMeas;
	while (pMsTmp != NULL){
		/* ����׼�Ϊ���� */
		for (i=0; i<MEAS_DIM; i++){
			fVar[i] = pMsTmp->nstd[i] * pMsTmp->nstd[i];
		}
		
		/* �Խǻ� */
		VectDiag(MEAS_DIM,fVar,pRn[iCnt].Rk[0]);

		pMsTmp = pMsTmp->next;
		iCnt++;
	}

	return iCnt;
}

/*
***************************************
������  ��Predict_OneStep
��������������һ��Ԥ��
����    ��pPred,һ��Ԥ���˲���Ϣ
          pFlt���˲���Ϣ
		  pMdl��ģ�Ͳ���
����ֵ  ����
***************************************
*/
void Predict_OneStep(FLTINFO *pPred,FLTINFO pFlt,MODELPARA pMdl)
{
	/* �����м���� */
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
������  ��Calculate_LikelihoodOld
��������������Ԥ��вЭ�������Լ���Ȼ�����ļ��㡣��ǰ�汾��ʹ�øú���
����    ��pPred,һ��Ԥ���˲���Ϣ
          pMeas,������Ϣ
		  sMdl��ģ�Ͳ���
		  pRk,��������
		  iMeasNum���������
����ֵ  ��Dk����Ȼ����ֵ
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

	/* ������Χ */
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

	/* ������Ȼ���� */
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

	/* �ͷ��ڴ� */
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
������  ��Calculate_LikelihoodByAugmentedMeas
��������������Ԥ��вЭ�������Լ���Ȼ�����ļ���
����    ��sPred,һ��Ԥ���˲���Ϣ
          pMeas,������Ϣ
		  sMdl��ģ�Ͳ���
		  pRk,��������
		  iMeasNum���������
����ֵ  ��Dk����Ȼ����ֵ
***************************************
*/
double Calculate_LikelihoodByAugmentedMeas(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum)
{
	/* ��Ȼ����ֵ */
	double Dk;

	/* ��ά���̲��� */
	double *Zp,*Hp,*Rp;

	/* ����ռ� */
	Zp = (double *)malloc(sizeof(double) * MEAS_DIM * iMeasNum);
	Hp = (double *)malloc(sizeof(double) * MEAS_DIM * STATE_DIM * iMeasNum);
	Rp = (double *)malloc(sizeof(double) * MEAS_DIM * MEAS_DIM  * iMeasNum * iMeasNum);

	/* ������ά */
	Augment_Dimension(pMeas,sMdl,pRk,Hp,Rp,Zp,iMeasNum);

	/* ������Ȼ���� */
	Dk = Calculate_Likelihood(sPred,Hp,Rp,Zp,iMeasNum);

	/* �ͷ��ڴ� */
	free(Zp);
	free(Hp);
	free(Rp);

	return Dk;
}

/*
***************************************
������  ��Calculate_LikelihoodByAugmentedMeas
����������������ά
����    ��pMeas,������Ϣ
		  sMdl��ģ�Ͳ���
		  pRk,��������
		  Hp,��ά����������
		  Rp����ά���������������
		  Zp����ά�������
		  iMeasNum���������
����ֵ  ����
***************************************
*/
void Augment_Dimension(MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,double *Hp,double *Rp,double *Zp,int iMeasNum)
{
	/* ��ʱ���� */
	int i;
	int iRB,iRF;

	/* ��ʱָ����� */
	MEASINFO *pMs;

	/* ������Χ */
	pMs = pMeas;

	/* ��ʼ������ */
	MtrInitialize(MEAS_DIM * iMeasNum,1,Zp);
	MtrInitialize(MEAS_DIM * iMeasNum,STATE_DIM,Hp);
	MtrInitialize(MEAS_DIM * iMeasNum,MEAS_DIM * iMeasNum,Rp);

	/* ��ά */
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
������  ��Calculate_Likelihood
����������������Ȼ����
����    ��sPred,һ��Ԥ���˲���Ϣ
		  Hp,��ά����������
		  Rp����ά���������������
		  Zp����ά�������
		  iMeasNum���������
����ֵ  ��Dk����Ȼ����ֵ
***************************************
*/
double Calculate_Likelihood(FLTINFO sPred,double *Hp,double *Rp,double *Zp,int iMeasNum)
{
	/* ��ʱ���� */
	double *HpXkp,*Zkd,*HpPkp,*tHp,*HPH,*Sk;
	double *tZkd,*iSk,*tZkdSk;
	double ZSZ,Dk;

	/* ������Ȼ���� */
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

	/* �ͷ��ڴ� */
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
������  ��Update_StateByParallelFiltering
���������������˲����˲�״̬��Э����������
����    ��pFlt���ں������˲���Ϣ
          sPred,һ��Ԥ���˲���Ϣ
          pMeas,������Ϣ
		  sMdl��ģ�Ͳ���
		  pRk,��������
		  iMeasNum���������
����ֵ  ����
***************************************
*/
void Update_StateByParallelFiltering(FLTINFO *pFlt,FLTINFO sPred,MODELPARA sMdl,RKNS *pRk,MEASINFO *pMeas,int iMeasNum)
{
	/* �����м���� */
	int i;
	double tHk[STATE_DIM][MEAS_DIM],iRk[MEAS_DIM][MEAS_DIM],tHR[STATE_DIM][MEAS_DIM],HkXkp[MEAS_DIM],Err[MEAS_DIM];
	double HRH[STATE_DIM][STATE_DIM],HRZH[STATE_DIM],tHRH[STATE_DIM][STATE_DIM],tHRZH[STATE_DIM];
	double arr[STATE_DIM],eye[STATE_DIM][STATE_DIM];
	double PH[STATE_DIM][STATE_DIM],EPH[STATE_DIM][STATE_DIM],iEPH[STATE_DIM][STATE_DIM],PHZ[STATE_DIM];

	/* �������� */
	RKNS *pRn;

	/* ������Ϣ */
	MEASINFO *pMs;

	/* ��ʼ�� */
	pRn = pRk;
	pMs = pMeas;

	/* ��ʼ������Ϊ0 */
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
������  ��Renew_Fusion
��������������ģ�͸��ʺ�IMM�˲�״̬
����    ��pTrk,������Ϣ
          pDk����Ȼ����ֵ
		  pChat����ϸ����м����
����ֵ  ����
***************************************
*/
void Renew_Fusion(TRACKINFO *pTrk,double *pDk,double *pChat)
{
	/* ��ʱ���� */
	int i;
	double fUkTmp,fXkTmp[STATE_DIM];
	double fXkErr[STATE_DIM],fPkErr[STATE_DIM][STATE_DIM],fPkNew[STATE_DIM][STATE_DIM],fPkTmp[STATE_DIM][STATE_DIM];

	/* �����˲��������ģ�͸��� */
	MtrMtrMulti(1,MODEL_NUM,1,pDk,pChat,&fUkTmp);
	for (i=0; i<MODEL_NUM; i++)
		pTrk->fMdlProb[i] = pDk[i] * pChat[i] / fUkTmp;

	/* �����ں�,�����˲�ֵ */
	MtrInitialize(STATE_DIM,1,pTrk->fsn.xk);
	for (i=0; i<MODEL_NUM; i++){
		MtrCnstMulti(STATE_DIM,1,pTrk->fMdlProb[i],pTrk->imm[i].xk,fXkTmp);
		MtrMtrPlus(STATE_DIM,1,fXkTmp,pTrk->fsn.xk,pTrk->fsn.xk);
	}

	/* �����ںϣ��������Э������� */
	/// 2012.6.24����
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
������  ��Generate_ForgedMeas
��������������ѹ��,����α���ⷽ����Ϣ
����    ��pCi��α���̲�����Ϣ
          pMeas,������Ϣ
		  sMdl��ģ�Ͳ���
		  pRk,��������
		  iMeasNum���������
����ֵ  ����
***************************************
*/
void Generate_ForgedMeas(COMPINFO *pCi,MEASINFO *pMeas,RKNS *pRk,MODELPARA sMdl,int iMeasNum)
{
	/* ��ʱ���� */
	int i;

	double tmpRz[MEAS_DIM],tmpRk[MEAS_DIM][MEAS_DIM],tmpRh[MEAS_DIM][STATE_DIM];
	double iRk[MEAS_DIM][MEAS_DIM],iRz[MEAS_DIM],iRh[MEAS_DIM][STATE_DIM];

	/* ������Ϣ  */
	MEASINFO *pMs;

	/* �������� */
	RKNS *pRn;

	/* ��ʼ��Ϊ0 */
	MtrInitialize(MEAS_DIM,1,tmpRz);
	MtrInitialize(MEAS_DIM,MEAS_DIM,tmpRk[0]);
	MtrInitialize(MEAS_DIM,STATE_DIM,tmpRh[0]);

	/* ����ѹ�� */
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
������  ��Update_StateByCompressedFiltering
��������������ѹ���˲����˲�״̬��Э����������
����    ��pFlt,�ں������˲�״̬��Ϣ
          sPred,һ��Ԥ����Ϣ
          sCi��α���̲�����Ϣ
		  iMeasNum���������
����ֵ  ����
***************************************
*/
void Update_StateByCompressedFiltering(FLTINFO *pFlt,FLTINFO sPred,COMPINFO sCi,int iMeasNum)
{
	/* �м���� */
	int i;
	double tHk[STATE_DIM][MEAS_DIM],iRk[MEAS_DIM][MEAS_DIM],tHR[STATE_DIM][MEAS_DIM],HkXkp[MEAS_DIM],Err[MEAS_DIM];
	double HRH[STATE_DIM][STATE_DIM],HRZH[STATE_DIM];

	double arr[STATE_DIM],eye[STATE_DIM][STATE_DIM];
	double PH[STATE_DIM][STATE_DIM],EPH[STATE_DIM][STATE_DIM],iEPH[STATE_DIM][STATE_DIM],PHZ[STATE_DIM];

	/* ��ʼ��Ϊ0 */
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
������  ��Decrease_Dimension
����������Ϊ������󲻿����棬��CV״̬��Ϣ��ά
����    ��pFlt,�˲�״̬��Ϣ
          pCvFlt,CVģ�͵��˲�״̬��Ϣ
		  iNum����Ҫ��ά���˲�״̬��Ϣ�ĸ���
����ֵ  ����
***************************************
*/
void Decrease_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum)
{
	int i,j;
	BMINFO pl,pr;

	for (i=0; i<iNum; i++){
		/* �˲�״̬��ά */
		pCvFlt[i].xk[0] = pFlt[i].xk[0];
		pCvFlt[i].xk[1] = pFlt[i].xk[1];
		pCvFlt[i].xk[2] = pFlt[i].xk[3];
		pCvFlt[i].xk[3] = pFlt[i].xk[4];
		pCvFlt[i].xk[4] = pFlt[i].xk[6];
		pCvFlt[i].xk[5] = pFlt[i].xk[7];

		/* ���Э�������ά */
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
������  ��Increase_Dimension
�����������ָ�CV״̬��Ϣά��
����    ��pFlt,�˲�״̬��Ϣ
          pCvFlt,CVģ�͵��˲�״̬��Ϣ
		  iNum����Ҫ��ά���˲�״̬��Ϣ�ĸ���
����ֵ  ����
***************************************
*/
void Increase_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum)
{
	int i,j;
	BMINFO pl,pr;

	for (i=0; i<iNum; i++){
		/* �˲�״̬��ά */
		MtrInitialize(STATE_DIM,1,pFlt[i].xk);

		pFlt[i].xk[0] = pCvFlt[i].xk[0];
		pFlt[i].xk[1] = pCvFlt[i].xk[1];
		pFlt[i].xk[3] = pCvFlt[i].xk[2];
		pFlt[i].xk[4] = pCvFlt[i].xk[3];
		pFlt[i].xk[6] = pCvFlt[i].xk[4];
		pFlt[i].xk[7] = pCvFlt[i].xk[5];

		/* ���Э�������ά */
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
������  ��ErrVar_UpperBound
�������������ݷ����Ͻ�����������Э������
����    ��pFlt,�˲�״̬��Ϣ
          pDim�������ά��
		  iNum����������
����ֵ  ��P����Э�������
***************************************
*/
double* ErrVar_UpperBound(FLTINFO *pFlt,int iDim,int iNum)
{
	/* �м���� */
	int i,iBeg,iFin,iPdim;

	/* ��Э������� */
	double *P;

	/* ��ʼ�� */
	iPdim = iDim * iNum;
	P = (double *)malloc(sizeof(double) * iPdim * iPdim);
	MtrInitialize(iPdim,iPdim,P);

	/* �Ȼ�öԽ����͵ľ��� */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,iBeg,iFin,iPdim,iDim,P,pFlt[i].pk[0]);
	}

	/* ����䷽���Ͻ� */
	MtrCnstMulti(iPdim,iPdim,iNum,P,P);

	return P;
}

/*
***************************************
������  ��ErrVar_UpperBoundByCV
�������������ݷ����Ͻ�����������Э������,CVģ��ר��
����    ��pFlt,�˲�״̬��Ϣ
          pDim�������ά��
		  iNum����������
����ֵ  ��P����Э�������
***************************************
*/
double* ErrVar_UpperBoundByCV(CVFLTINFO *pCvFlt,int iDim,int iNum)
{
	/* �м���� */
	int i,iBeg,iFin,iPdim;

	/* ��Э������� */
	double *P;

	/* ��ʼ�� */
	iPdim = iDim * iNum;
	P = (double *)malloc(sizeof(double) * iPdim * iPdim);
	MtrInitialize(iPdim,iPdim,P);

	/* �Ȼ�öԽ����͵ľ��� */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,iBeg,iFin,iPdim,iDim,P,pCvFlt[i].pk[0]);
	}

	/* ����䷽���Ͻ� */
	MtrCnstMulti(iPdim,iPdim,iNum,P,P);

	return P;
}

/*
***************************************
������  ��Generate_CombStateNEye
�����������ϳɹ�����ϳɵ�λ���������
����    ��pXk,��ά״̬��Ϣ
          pIk����ά��λ����
		  pFlt���˲���Ϣ
          pDim�������ά��
		  iNum����������
����ֵ  ����
***************************************
*/
void Generate_CombStateNEye(double *pXk,double *pIk,FLTINFO *pFlt,int iDim,int iNum)
{
	/* ��ʱ���� */
	int i,iBeg,iFin;
	double *pArr,*pEye;

	/* ��ʼ������ */
	MtrInitialize(iDim * iNum,1,pXk);
	MtrInitialize(iDim * iNum,iDim,pIk);

	/* ����ռ� */
	pArr = (double *)malloc(sizeof(double) * iDim);
	pEye = (double *)malloc(sizeof(double) * iDim * iDim);

	/* ��õ�λ�� */
	for (i=0; i<iDim; i++)
		pArr[i] = 1.0;
	VectDiag(iDim,pArr,pEye);

	/* ������ά */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,0,1,1,1,pXk,pFlt[i].xk);
		Assign_Matrix2Block(iBeg,iFin,0,iDim,iDim,iDim,pIk,pEye);
	}

	/* �ͷſռ� */
	free(pArr);
	free(pEye);
}

/*
***************************************
������  ��Generate_CombStateNEye
���������� CVģ�ͺϳɹ�����ϳɵ�λ���������
����    ��pXk,��ά״̬��Ϣ
          pIk����ά��λ����
		  pFlt���˲���Ϣ
          pDim�������ά��
		  iNum����������
����ֵ  ����
***************************************
*/
void Generate_CombStateNEyeByCV(double *pXk,double *pIk,CVFLTINFO *pFlt,int iDim,int iNum)
{
	/* ��ʱ���� */
	int i,iBeg,iFin;
	double *pArr,*pEye;

	/* ��ʼ������ */
	MtrInitialize(iDim * iNum,1,pXk);
	MtrInitialize(iDim * iNum,iDim,pIk);

	/* ����ռ� */
	pArr = (double *)malloc(sizeof(double) * iDim);
	pEye = (double *)malloc(sizeof(double) * iDim * iDim);

	/* ��õ�λ�� */
	for (i=0; i<iDim; i++)
		pArr[i] = 1.0;
	VectDiag(iDim,pArr,pEye);

	/* ������ά */
	for (i=0; i<iNum; i++){
		iBeg = i * iDim;
		iFin = (i+1) * iDim;

		Assign_Matrix2Block(iBeg,iFin,0,1,1,1,pXk,pFlt[i].xk);
		Assign_Matrix2Block(iBeg,iFin,0,iDim,iDim,iDim,pIk,pEye);
	}

	/* �ͷſռ� */
	free(pArr);
	free(pEye);
}

/*
***************************************
������  ��Update_StateByML
���������������Ȼ�����µ��ںϹ��ƺ͹���Э�������ļ���
����    ��Xkk,�ں�״̬��Ϣ
          Pkk���ں�Э������Ϣ
		  P����Э�������
		  Xk����ά״̬��Ϣ
          Ik����ά��λ����
          pDim�������ά��
		  iNum����������
����ֵ  ����
***************************************
*/
void Update_StateByML(double *Xkk,double *Pkk,double *P,double *Xk,double *Ik,int iDim,int iNum)
{
	/* �м���� */
	double *tIk,*iP,*tIkiP,*IPI,*PIP;

	/* ����ռ� */
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

	/* �ͷ��ڴ� */
	free(tIk);
	free(iP);
	free(tIkiP);
	free(IPI);
	free(PIP);
}

/*
***************************************
������  ��Update_StateByOptimalDistribution
�����������������������ŷֲ�ʽ�ں��㷨���������Ĺ�������״̬��Э�������
����    ��pNew,�ں������˲���Ϣ
          pKf����������Kalman�˲���Ϣ
		  pPr����������һ��Ԥ��
		  pCPr���ں�����һ��Ԥ��
          pDim�������ά��
		  iNum����������
����ֵ  ����
***************************************
*/
void Update_StateByOptimalDistribution(FLTINFO *pNew,FLTINFO *pKf,FLTINFO *pPr,FLTINFO *pCPr,int iDim,int iNum)
{
	/* �м���� */
	int i;
	double *tmpPk,*tmpPX;
	double *dP,*dPX,*iPk,*iPkp,*iPkXk,*iPkpXkp;
	double *iCPk,*PP,*iCPkXk,*PXPX;

	/* �����ڴ� */
	tmpPk = (double *)malloc(sizeof(double) * iDim * iDim);
	tmpPX = (double *)malloc(sizeof(double) * iDim); 
	dP       = (double *)malloc(sizeof(double) * iDim * iDim);
	dPX     = (double *)malloc(sizeof(double) * iDim);
	iPk      =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkp	   =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkXk   =  (double *)malloc(sizeof(double) * iDim); 
	iPkpXkp=  (double *)malloc(sizeof(double) * iDim); 

	/* �����ʼ�� */
	MtrInitialize(iDim,iDim,tmpPk);
	MtrInitialize(iDim,1,tmpPX);

	/* �������Ĺ�������״̬��Э������� */
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
	
	/* �����ڴ� */
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

	/* �ͷ��ڴ� */
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
������  ��Update_StateByOptimalDistributionByCV
����������CVģ�͵Ĳ������������ŷֲ�ʽ�ں��㷨���������Ĺ�������״̬��Э�������
����    ��pNew,�ں������˲���Ϣ
          pKf����������Kalman�˲���Ϣ
		  pPr����������һ��Ԥ��
		  pCPr���ں�����һ��Ԥ��
          pDim�������ά��
		  iNum����������
����ֵ  ����
***************************************
*/
void Update_StateByOptimalDistributionByCV(CVFLTINFO *pNew,CVFLTINFO *pKf,CVFLTINFO *pPr,CVFLTINFO *pCPr,int iDim,int iNum)
{
	/* �м���� */
	int i;
	double *tmpPk,*tmpPX;
	double *dP,*dPX,*iPk,*iPkp,*iPkXk,*iPkpXkp;
	double *iCPk,*PP,*iCPkXk,*PXPX;

	/* �����ڴ� */
	tmpPk = (double *)malloc(sizeof(double) * iDim * iDim);
	tmpPX = (double *)malloc(sizeof(double) * iDim); 
	dP       = (double *)malloc(sizeof(double) * iDim * iDim);
	dPX     = (double *)malloc(sizeof(double) * iDim);
	iPk      =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkp	   =  (double *)malloc(sizeof(double) * iDim * iDim);
	iPkXk   =  (double *)malloc(sizeof(double) * iDim); 
	iPkpXkp=  (double *)malloc(sizeof(double) * iDim); 

	/* �����ʼ�� */
	MtrInitialize(iDim,iDim,tmpPk);
	MtrInitialize(iDim,1,tmpPX);

	/* �������Ĺ�������״̬��Э������� */
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
	
	/* �����ڴ� */
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

	/* �ͷ��ڴ� */
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
