#ifndef _FUSIONFILTER_H
#define _FUSIONFILTER_H

/*
***************************************
*            头文件引用               *
***************************************
*/
#include "structDefinition.h"

/*
***************************************
*         本文件内的函数声明          *
***************************************
*/

/* 交互式多模型滤波 */
void IMM_Filtering(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSnr,int iMethod,int iSpt);

/* 模型条件重初始化 */
void Reinitialize_Condition(double *fMdlProb,double *fChat,FLTINFO *pOldFlt,FLTINFO *pNewFlt);

/* 卡尔曼滤波器 */
double Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pFltPred,MODELPARA sMdl,double *Rk,double *pMs,double *Dis,int *Calcu_Dis);

/* 单量测下交互式多模型用的卡尔曼滤波 */
double IMM_Kalman_Filtering(FLTINFO *pFlt,FLTINFO *pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,double *Dis,int *Calcu_Dis);

/* 并行滤波 */
double Parallel_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA pMdl,int iSnr);

/* 数据压缩滤波 */
double Data_Compression_Filtering(FLTINFO *pFlt,FLTINFO pMix,MEASINFO *pMeas,MODELPARA pMdl,int iSnr);

/* Bar-Shalom-Campo融合算法 */
double Bar_Shalom_Campo_Fusion(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl);

/* 不带反馈的最优分布式估计融合 */
double Optimal_Distributed_Fusion_Without_Ack(TRACKINFO *pTrk,FLTINFO pMix,MEASINFO *pMeas,MODELPARA sMdl,int iSnr,int iMdl);

/* 初始化Rk矩阵 */
int Initialize_Rk(MEASINFO *pMeas,RKNS *pRn);

/* 进行一步预测 */
void Predict_OneStep(FLTINFO *pPred,FLTINFO pFlt,MODELPARA pMdl);

/* 量测预测残差、协方差阵以及似然函数的计算 */
double Calculate_LikelihoodOld(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum);

/* 量测预测残差、协方差阵以及似然函数的计算 */
double Calculate_LikelihoodByAugmentedMeas(FLTINFO sPred,MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,int iMeasNum);

/* 量测扩维 */
void Augment_Dimension(MEASINFO *pMeas,MODELPARA sMdl,RKNS *pRk,double *Hp,double *Rp,double *Zp,int iMeasNum);

/* 计算似然函数 */
double Calculate_Likelihood(FLTINFO sPred,double *Hp,double *Rp,double *Zp,int iMeasNum);

/* 滤波状态和协方差矩阵计算 */
void Update_StateByParallelFiltering(FLTINFO *pFlt,FLTINFO sPred,MODELPARA sMdl,RKNS *pRk,MEASINFO *pMeas,int iMeasNum);

/* 更新模型概率和IMM滤波状态 */
void Renew_Fusion(TRACKINFO *pTrk,double *pDk,double *pChat);

/* 数据压缩,生成伪量测方程信息 */
void Generate_ForgedMeas(COMPINFO *pCi,MEASINFO *pMeas,RKNS *pRk,MODELPARA sMdl,int iMeasNum);

/* 数据压缩滤波的滤波状态和协方差矩阵计算 */
void Update_StateByCompressedFiltering(FLTINFO *pFlt,FLTINFO sPred,COMPINFO sCi,int iMeasNum);

/* 为避免矩阵不可求逆，将CV状态信息降维 */
void Decrease_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum);

/* 恢复CV状态信息维数 */
void Increase_Dimension(FLTINFO *pFlt,CVFLTINFO *pCvFlt,int iNum);

/* 根据方差上界计算联合误差协方差阵 */
double* ErrVar_UpperBound(FLTINFO *pFlt,int iDim,int iNum);

/* 根据方差上界计算联合误差协方差阵,CV模型专用 */
double* ErrVar_UpperBoundByCV(CVFLTINFO *pCvFlt,int iDim,int iNum);

/* 合成估计与合成单位矩阵的生成 */
void Generate_CombStateNEye(double *pXk,double *pIk,FLTINFO *pFlt,int iDim,int iNum);

/* CV模型合成估计与合成单位矩阵的生成 */
void Generate_CombStateNEyeByCV(double *pXk,double *pIk,CVFLTINFO *pFlt,int iDim,int iNum);

/* 最大似然意义下的融合估计和估计协方差矩阵的计算 */
void Update_StateByML(double *Xkk,double *Pkk,double *P,double *Xk,double *Ik,int iDim,int iNum);

/* 不带反馈的最优分布式融合算法，更新中心估计器的状态和协方差矩阵 */
void Update_StateByOptimalDistribution(FLTINFO *pNew,FLTINFO *pKf,FLTINFO *pPr,FLTINFO *pCPr,int iDim,int iNum);

/* CV模型的不带反馈的最优分布式融合算法，更新中心估计器的状态和协方差矩阵 */
void Update_StateByOptimalDistributionByCV(CVFLTINFO *pNew,CVFLTINFO *pKf,CVFLTINFO *pPr,CVFLTINFO *pCPr,int iDim,int iNum);

#endif