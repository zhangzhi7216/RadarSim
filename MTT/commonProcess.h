#ifndef _COMMONPROCESS_H
#define _COMMONPROCESS_H

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

/* 初始化滤波模型参数 */
void Initialize_Models(MODELPARA *pMdl);

//void FrameRead_RadarAIS(FILE *pMeas, int iSpt);
//void FrameRead_TongLei(FILE *pMeas, int iSpt);
/* 从文件中读取当前帧的量测数据 */
// MEASINFO* Read_Data(FILE *pMeas[],int *iEOF);

/* 选择属于某一传感器某一航迹的量测信息 */
MEASINFO* Select_Measure(MEASINFO *pMeas,int iTgtIdx,int iSnsIdx,int *iSnrNum);

/* 量测信息赋值 */
void Assign_MeasInfo(MEASINFO *pLeft,MEASINFO *pRight);

/* 释放量测信息所占内存空间 */
void Release_MeasInfo(MEASINFO *pMeas);

/* 初始化滤波状态 */
void Initialize_State(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSpt);

/* 将整个矩阵pRight赋给pLeft的特定块上，该块的维数与pRight矩阵相同 */
void Assign_Matrix2Block(int iRowBeg,int iRowFin,int iColBeg,int iColFin,int iLCol,int iRCol,double *pLeft,double *pRight);

/* 将矩阵pRight的特定块赋给pLeft的特定块上,考虑矩阵与矩阵不同分块的赋值 */
void Assign_Block2Block(BMINFO sLeft,BMINFO sRight,double *pLeft,double *pRight);

/* 滤波信息赋值 */
void Assign_FltInfo(FLTINFO *pLeft,FLTINFO *pRight);

/* 显示航迹信息 */
void Display_TrackInfo(TRACKINFO *pTrk);

/* 显示量测信息 */
void Display_MeasInfo(MEASINFO *pMeas);

/* 显示浮点型矩阵 */
void Display_DblMatrix(double *pMtr,int iRow,int iCol);

/* 显示整型矩阵 */
void Display_IntMatrix(int *pMtr,int iRow,int iCol);

/* 航迹融合与否判定 */
void MTTORFusion_Decision();

/* 初始化滤波参数模型 */
void InitiateTrack(void);

/* 起始一条新航迹 */
void InitNewTrack(int iSpt,int MeasN);

/* 初始化滤波参数模型 */
void FilterPrediction(void);

/* 最近邻数据关联 */
void GNN_Asso(int *iMethod);

/* 计算统计距离 */
void Statistic_Distance(TRACKINFO *pTrkTmp,ECHO *pEcho,double *Dis,double *Lis);

/* 航迹后期整理 */
void Track_Trim();

/* 航迹赋值 */
void TrksAssign(TRACKINFO *pTLeft, TRACKINFO *pTRight);

/* 整型数组赋值 */
void IntArrAssign(int *pLeft,int *pRight,int iLen);

/* 双精度浮点型数组赋值 */
void FloatArrAssign(double *pLeft,double *pRight,int iLen);

/* 极坐标化为直角坐标 */
double *PolToDesc(double *pPol);

/* 直角坐标化为极坐标 */
double *DescToPol(double *pDesc);

/* 对量测由球坐标到直角坐标进行无偏转换 */
void Unbias_Conv(MEASINFO *pEcho_Fusion,ECHO *pEcho);

/* 计算量测由球坐标到直角坐标过程中的R阵 */
double *Calculate_R(ECHO *pEcho);

/* 接受外部进入DLL的参数 */
void Receive_Parameter(ECHO *sEcho_TL,ECHO *sEcho_RA,int *Echo_RA,int *Echo_TL,TRACKINFO *pTmp,ECHO_PARAMETER *sEcho_Param,int *TrkNum);

/* 航迹删除 */
void TrkDel(void);

/* 航迹输出 */
void Transmit_Trk(TRACKINFO *pTrk);

//void OutFile(int iSpt,FILE *pOutp);
#endif