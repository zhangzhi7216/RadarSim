#ifndef _COMMONPROCESS_H
#define _COMMONPROCESS_H

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

/* ��ʼ���˲�ģ�Ͳ��� */
void Initialize_Models(MODELPARA *pMdl);

//void FrameRead_RadarAIS(FILE *pMeas, int iSpt);
//void FrameRead_TongLei(FILE *pMeas, int iSpt);
/* ���ļ��ж�ȡ��ǰ֡���������� */
// MEASINFO* Read_Data(FILE *pMeas[],int *iEOF);

/* ѡ������ĳһ������ĳһ������������Ϣ */
MEASINFO* Select_Measure(MEASINFO *pMeas,int iTgtIdx,int iSnsIdx,int *iSnrNum);

/* ������Ϣ��ֵ */
void Assign_MeasInfo(MEASINFO *pLeft,MEASINFO *pRight);

/* �ͷ�������Ϣ��ռ�ڴ�ռ� */
void Release_MeasInfo(MEASINFO *pMeas);

/* ��ʼ���˲�״̬ */
void Initialize_State(TRACKINFO *pTrk,MEASINFO *pMeas,MODELPARA *pMdl,int iSpt);

/* ����������pRight����pLeft���ض����ϣ��ÿ��ά����pRight������ͬ */
void Assign_Matrix2Block(int iRowBeg,int iRowFin,int iColBeg,int iColFin,int iLCol,int iRCol,double *pLeft,double *pRight);

/* ������pRight���ض��鸳��pLeft���ض�����,���Ǿ��������ͬ�ֿ�ĸ�ֵ */
void Assign_Block2Block(BMINFO sLeft,BMINFO sRight,double *pLeft,double *pRight);

/* �˲���Ϣ��ֵ */
void Assign_FltInfo(FLTINFO *pLeft,FLTINFO *pRight);

/* ��ʾ������Ϣ */
void Display_TrackInfo(TRACKINFO *pTrk);

/* ��ʾ������Ϣ */
void Display_MeasInfo(MEASINFO *pMeas);

/* ��ʾ�����;��� */
void Display_DblMatrix(double *pMtr,int iRow,int iCol);

/* ��ʾ���;��� */
void Display_IntMatrix(int *pMtr,int iRow,int iCol);

/* �����ں�����ж� */
void MTTORFusion_Decision();

/* ��ʼ���˲�����ģ�� */
void InitiateTrack(void);

/* ��ʼһ���º��� */
void InitNewTrack(int iSpt,int MeasN);

/* ��ʼ���˲�����ģ�� */
void FilterPrediction(void);

/* ��������ݹ��� */
void GNN_Asso(int *iMethod);

/* ����ͳ�ƾ��� */
void Statistic_Distance(TRACKINFO *pTrkTmp,ECHO *pEcho,double *Dis,double *Lis);

/* ������������ */
void Track_Trim();

/* ������ֵ */
void TrksAssign(TRACKINFO *pTLeft, TRACKINFO *pTRight);

/* �������鸳ֵ */
void IntArrAssign(int *pLeft,int *pRight,int iLen);

/* ˫���ȸ��������鸳ֵ */
void FloatArrAssign(double *pLeft,double *pRight,int iLen);

/* �����껯Ϊֱ������ */
double *PolToDesc(double *pPol);

/* ֱ�����껯Ϊ������ */
double *DescToPol(double *pDesc);

/* �������������굽ֱ�����������ƫת�� */
void Unbias_Conv(MEASINFO *pEcho_Fusion,ECHO *pEcho);

/* ���������������굽ֱ����������е�R�� */
double *Calculate_R(ECHO *pEcho);

/* �����ⲿ����DLL�Ĳ��� */
void Receive_Parameter(ECHO *sEcho_TL,ECHO *sEcho_RA,int *Echo_RA,int *Echo_TL,TRACKINFO *pTmp,ECHO_PARAMETER *sEcho_Param,int *TrkNum);

/* ����ɾ�� */
void TrkDel(void);

/* ������� */
void Transmit_Trk(TRACKINFO *pTrk);

//void OutFile(int iSpt,FILE *pOutp);
#endif