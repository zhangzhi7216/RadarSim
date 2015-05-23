#ifndef _FUSIONFILTER_H
#define _FUSIONFILTER_H

/*
***************************************
*            ͷ�ļ�����               *
***************************************
*/
#include "structDefinition.h"

/*
***************************************
*         ���ļ��ڵĺ�������          *
***************************************
*/

/* ����ʽ��ģ���˲� */
void IMM_Filtering(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSnr,int iMethod,int iSpt);

/* ģ�������س�ʼ�� */
void Reinitialize_Condition(double *fMdlProb,double *fChat,FLTINFO *pOldFlt,FLTINFO *pNewFlt);

/* �������˲��� */
double Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pFltPred,MODELPARA sMdl,double *Rk,double *pMs,double *Dis,int *Calcu_Dis);

/* �������½���ʽ��ģ���õĿ������˲� */
double IMM_Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,double *Dis,int *Calcu_Dis);

/* �����˲� */
double Parallel_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA pMdl,int iSnr);

/* ����ѹ���˲� */
double Data_Compression_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA pMdl,int iSnr);

/* Bar-Shalom-Campo�ں��㷨 */
double Bar_Shalom_Campo_Fusion(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl);

/* �������������ŷֲ�ʽ�����ں� */
double Optimal_Distributed_Fusion_Without_Ack(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl);

/* ��ʼ��Rk���� */
int Initialize_Rk(MEASINFO *pMeas,RKNS *pRn);

/* ����һ��Ԥ�� */
void Predict_OneStep(FLTINFO *pPred,FLTINFO pFlt,MODELPARA pMdl);

/* ����Ԥ��вЭ�������Լ���Ȼ�����ļ��� */
double Calculate_LikelihoodOld(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum);

/* ����Ԥ��вЭ�������Լ���Ȼ�����ļ��� */
double Calculate_LikelihoodByAugmentedMeas(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum);

/* ������ά */
void Augment_Dimension(MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,double *Hp,double *Rp,double *Zp,int iMeasNum);

/* ������Ȼ���� */
double Calculate_Likelihood(FLTINFO sPred,double *Hp,double *Rp,double *Zp,int iMeasNum);

/* �˲�״̬��Э���������� */
void Update_StateByParallelFiltering(FLTINFO *pFlt,FLTINFO sPred,MODELPARA sMdl,RKNS *pRk,MEASINFO *pMeas,int iMeasNum);

/* ����ģ�͸��ʺ�IMM�˲�״̬ */
void Renew_Fusion(TRACKINFO *pTrk,double *pDk,double *pChat);

/* ����ѹ��,����α���ⷽ����Ϣ */
void Generate_ForgedMeas(COMPINFO *pCi,MEASINFO *pMeas,RKNS *pRk,MODELPARA sMdl,int iMeasNum);

/* ����ѹ���˲����˲�״̬��Э���������� */
void Update_StateByCompressedFiltering(FLTINFO *pFlt,FLTINFO sPred,COMPINFO sCi,int iMeasNum);

/* Ϊ������󲻿����棬��CV״̬��Ϣ��ά */
void Decrease_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum);

/* �ָ�CV״̬��Ϣά�� */
void Increase_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum);

/* ���ݷ����Ͻ�����������Э������ */
double* ErrVar_UpperBound(FLTINFO *pFlt,int iDim,int iNum);

/* ���ݷ����Ͻ�����������Э������,CVģ��ר�� */
double* ErrVar_UpperBoundByCV(CVFLTINFO *pCvFlt,int iDim,int iNum);

/* �ϳɹ�����ϳɵ�λ��������� */
void Generate_CombStateNEye(double *pXk,double *pIk,FLTINFO *pFlt,int iDim,int iNum);

/* CVģ�ͺϳɹ�����ϳɵ�λ��������� */
void Generate_CombStateNEyeByCV(double *pXk,double *pIk,CVFLTINFO *pFlt,int iDim,int iNum);

/* �����Ȼ�����µ��ںϹ��ƺ͹���Э�������ļ��� */
void Update_StateByML(double *Xkk,double *Pkk,double *P,double *Xk,double *Ik,int iDim,int iNum);

/* �������������ŷֲ�ʽ�ں��㷨���������Ĺ�������״̬��Э������� */
void Update_StateByOptimalDistribution(FLTINFO *pNew,FLTINFO *pKf,FLTINFO *pPr,FLTINFO *pCPr,int iDim,int iNum);

/* CVģ�͵Ĳ������������ŷֲ�ʽ�ں��㷨���������Ĺ�������״̬��Э������� */
void Update_StateByOptimalDistributionByCV(CVFLTINFO *pNew,CVFLTINFO *pKf,CVFLTINFO *pPr,CVFLTINFO *pCPr,int iDim,int iNum);

#endif