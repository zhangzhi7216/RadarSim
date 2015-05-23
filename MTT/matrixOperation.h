#ifndef _MATRIX_H
#define _MATRIX_H

/*
***************************************
*         ���ļ��ڵĺ�������          *
***************************************
*/

/* ������� */
void MtrMtrMulti(int iRow,int iLCol,int iRCol, double *pMLeft, double *pMRight, double *pMRslt);

/* ������Գ��� */
void MtrCnstMulti(int iRow,int iCol,double fConst,double *pMCur,double *pMRslt);	

/* ������� */
void MtrMtrPlus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt);

/* ������� */
void MtrMtrMinus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt);

/* ������Խǻ� */
void MtrDiag(int iOrg,int iRslt,double *pMOrg,double *pMRslt);

/* ������Խǻ� */
void VectDiag(int iDim,double *pVect,double *pMRslt);

/* ����ֵ */
void MtrAssign(int iRow,int iCol,double *pMOrg,double *pMRslt);		

/* �����ʼ�� */
void MtrInitialize(int iRow,int iCol,double *pMRslt);

/* ����ת�� */
void MtrTrans(int iRow,int iCol,double *pMOrg,double *pMRslt);	

/* ��������ʽ��ֵ */							
double MtrDet(int iDim,double *pMOrg);

/* �������� */
void MtrInvers(int iDim,double *pMOrg,double *pMRslt);	

/* LUP�ֽ� */
void LUPDecomp(int iDim,double *pMtr,double *pL,double *pU,int *pP);

/* LUP�ֽ�����Է��� */
void LUPSolve(int iDim,double *pL,double *pU,int *pP,double *pB,double *pX);

/* �������ݽ��� */
void IntExch(int *pLeft,int *pRight);	

/* ���������ݽ��� */
void FloatExch(double *pLeft,double *pRight);								

#endif