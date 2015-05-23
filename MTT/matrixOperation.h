#ifndef _MATRIX_H
#define _MATRIX_H

/*
***************************************
*         本文件内的函数声明          *
***************************************
*/

/* 矩阵相乘 */
void MtrMtrMulti(int iRow,int iLCol,int iRCol, double *pMLeft, double *pMRight, double *pMRslt);

/* 矩阵乘以常数 */
void MtrCnstMulti(int iRow,int iCol,double fConst,double *pMCur,double *pMRslt);	

/* 矩阵相加 */
void MtrMtrPlus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt);

/* 矩阵相减 */
void MtrMtrMinus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt);

/* 将矩阵对角化 */
void MtrDiag(int iOrg,int iRslt,double *pMOrg,double *pMRslt);

/* 将数组对角化 */
void VectDiag(int iDim,double *pVect,double *pMRslt);

/* 矩阵赋值 */
void MtrAssign(int iRow,int iCol,double *pMOrg,double *pMRslt);		

/* 矩阵初始化 */
void MtrInitialize(int iRow,int iCol,double *pMRslt);

/* 矩阵转置 */
void MtrTrans(int iRow,int iCol,double *pMOrg,double *pMRslt);	

/* 计算行列式的值 */							
double MtrDet(int iDim,double *pMOrg);

/* 矩阵求逆 */
void MtrInvers(int iDim,double *pMOrg,double *pMRslt);	

/* LUP分解 */
void LUPDecomp(int iDim,double *pMtr,double *pL,double *pU,int *pP);

/* LUP分解解线性方程 */
void LUPSolve(int iDim,double *pL,double *pU,int *pP,double *pB,double *pX);

/* 整型数据交换 */
void IntExch(int *pLeft,int *pRight);	

/* 浮点型数据交换 */
void FloatExch(double *pLeft,double *pRight);								

#endif