/*
***************************************
文件名  ：matrix.c
描述    ：定义矩阵相关处理函数
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrixOperation.h"

/*
***************************************
*         本文件内的函数定义          *
***************************************
*/

/*
***************************************
函数名  ：MtrMtrMulti
功能描述：矩阵1与矩阵2相乘
参数    ：iRow，矩阵1的行数
          iLCol，矩阵1的列数
		  iRCol，矩阵2的列数
		  pMLeft，指向矩阵1的指针
		  pMRight，指向矩阵2的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrMtrMulti(int iRow,int iLCol,int iRCol, double *pMLeft, double *pMRight, double *pMRslt)
{ 
	int i,j,k;

	/* 将输出矩阵所有元素置0.0 */
	for(i=0;i<iRow*iRCol;i++){
		*pMRslt = 0.0;
		pMRslt++;
	}

	/* 重新指向输出矩阵初始位置 */
	pMRslt = pMRslt-iRow*iRCol;

	/* 利用矩阵乘法定义，计算输出矩阵各个元素的值 */
	for(i=0;i<iRow;i++){
		for(j=0;j<iRCol;j++){
			for(k=0;k<iLCol;k++) 
				pMRslt[i*iRCol+j] = pMRslt[i*iRCol+j] + pMLeft[i*iLCol+k] * pMRight[k*iRCol+j];
		}
	}
}

/*
***************************************
函数名  ：MtrCnstMulti
功能描述：矩阵与常数相乘
参数    ：iRow，矩阵的行数
          iCol，矩阵的列数
		  fConst,常数
		  pMCur，指向被乘矩阵的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrCnstMulti(int iRow,int iCol,double fConst,double *pMCur,double *pMRslt)
{
	int i,j;

	/* 将矩阵各个元素分别乘以常数 */
	for(i=0;i<iRow;i++)
		for(j=0;j<iCol;j++)
			pMRslt[i*iCol+j] = fConst * pMCur[i*iCol+j];
}


/*
***************************************
函数名  ：MtrMtrPlus
功能描述：矩阵1与矩阵2相加
参数    ：iRow，矩阵的行数
          iCol，矩阵的列数
		  pMLeft，指向矩阵1的指针
		  pMRight，指向矩阵2的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrMtrPlus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt)
{ 
	int i;
	int iTot = iRow * iCol;

	/* 矩阵中对应元素相加 */
	for(i=0;i<iTot;i++){  
		*pMRslt = *pMLeft + *pMRight;

		pMRslt++;
		pMLeft++;
		pMRight++;
	}
}

/*
***************************************
函数名  ：MtrMtrMinus
功能描述：矩阵1与矩阵2相减
参数    ：iRow，矩阵的行数
          iCol，矩阵的列数
		  pMLeft，指向矩阵1的指针
		  pMRight，指向矩阵2的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrMtrMinus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt)
{	
	int i;
	int iTot = iRow * iCol;

	/* 矩阵中对应元素相减 */
	for(i=0;i<iTot;i++){  
		*pMRslt = *pMLeft - *pMRight;

		pMRslt++;
		pMLeft++;
		pMRight++;
	}
}

/*
***************************************
函数名  ：MtrDiag
功能描述：将n阶矩阵对角化为m*n阶矩阵
参数    ：iOrg，对角化前矩阵的阶数
          iRslt，对角化后矩阵的阶数
		  pMOrg，指向要对角化的矩阵的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrDiag(int iOrg,int iRslt,double *pMOrg,double *pMRslt)
{
	int i,j;
	int iTmp;

	/* 根据矩阵元素位置设定相应的值 */
	for(i=0;i<iRslt;i++){
		iTmp = i/iOrg;

		for(j=0;j<iRslt;j++){
			if(iTmp == (j/iOrg))
				*(pMRslt+i*iRslt+j) = *(pMOrg + (i%iOrg) * iOrg + (j%iOrg));
			else
				*(pMRslt+i*iRslt+j) = 0;
		}
	}
}

/*
***************************************
函数名  ：VectDiag
功能描述：将m维向量化为m阶对角阵
参数    ：iDim，向量的维数
		  pVect，指向向量（数组）的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void VectDiag(int iDim,double *pVect,double *pMRslt)
{
	int i,j;
	for(i=0;i<iDim;i++){
		for(j=0;j<iDim;j++){
			if (i == j)
				*(pMRslt+i*iDim+j) = *(pVect+i);
			else
				*(pMRslt+i*iDim+j) = 0;
		}
	}
}

/*
***************************************
函数名  ：MtrAssign
功能描述：矩阵的赋值
参数    ：iRow，矩阵的行数
          iCol，矩阵的列数
		  pMOrg，指向输入矩阵的指针
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrAssign(int iRow,int iCol,double *pMOrg,double *pMRslt)
{
	int i,j;

	for(i=0;i<iRow;i++)
	{
		for(j=0;j<iCol;j++)
		{
			*(pMRslt+i*iCol+j) = *(pMOrg+i*iCol+j);
		}
	}
}

/*
***************************************
函数名  ：MtrInitialize
功能描述：矩阵的所有元素初始化为0
参数    ：iRow，矩阵的行数
          iCol，矩阵的列数
		  pMRslt，指向输出矩阵的指针
返回值  ：无
***************************************
*/
void MtrInitialize(int iRow,int iCol,double *pMRslt)
{
	int i,j;

	for(i=0;i<iRow;i++)
	{
		for(j=0;j<iCol;j++)
		{
			*(pMRslt+i*iCol+j) = 0;
		}
	}
}

/*
***************************************
函数名  ：MtrTrans
功能描述：矩阵的转置
参数    ：iRow，转置前矩阵的行数
          iCol，转置前矩阵的列数
		  pMOrg，指向转置前的矩阵的指针
		  pMRslt，指向转置后的矩阵的指针
返回值  ：无
***************************************
*/
void MtrTrans(int iRow,int iCol,double *pMOrg,double *pMRslt)
{
	int i,j;
	for(i=0;i<iCol;i++)
	{
		for(j=0;j<iRow;j++)
		{
			*(pMRslt+i*iRow+j) = *(pMOrg+j*iCol+i);
		}
	}
}

/*
***************************************
函数名  ：MtrDet
功能描述：计算n阶矩阵行列式的值
参数    ：iDim，矩阵的阶数
		  pMOrg，指向矩阵的指针
返回值  ：fRelt，行列式的值
***************************************
*/
double MtrDet(int iDim,double *pMOrg)
{
	int i,j,iCount;
	double fRslt = 1.0;

	/* 分配内存 */
	double *pL = (double *)malloc(sizeof(double)*(iDim*iDim));
	double *pU = (double *)malloc(sizeof(double)*(iDim*iDim));
	int    *pP = (int *)malloc(sizeof(int)*iDim);
	
	/* 矩阵LUP分解 */
	LUPDecomp(iDim,pMOrg,pL,pU,pP);

	for(i=0;i<iDim;i++)
		fRslt = fRslt * (*(pU+i*iDim+i));

	/* 判定结果的符号 */
	iCount = 0;
	for(i=0;i<iDim;i++)
		for(j=i+1;j<iDim;j++)
			if(pP[j] < pP[i]) iCount++;

	if((iCount%2) == 1)
		fRslt = -fRslt;

	/* 释放内存 */
	free(pL);
	free(pU);
	free(pP);

	return fRslt;
}

/*
***************************************
函数名  ：MtrInvers
功能描述：计算n阶矩阵的逆矩阵
参数    ：iDim，矩阵的阶数
		  pMOrg，指向输入矩阵的指针
		  pMRslt，指向逆矩阵的指针
返回值  ：无
***************************************
*/
void MtrInvers(int iDim,double *pMOrg,double *pMRslt)
{	
	int i,j;

	double *pL = (double *)malloc(sizeof(double)*(iDim*iDim));
	double *pU = (double *)malloc(sizeof(double)*(iDim*iDim));
	int    *pP = (int *)malloc(sizeof(int)*iDim);
	double *pB = (double *)malloc(sizeof(double)*iDim);
	double *pX = (double *)malloc(sizeof(double)*iDim);
	
	/* 矩阵的LUP分解 */
	LUPDecomp(iDim,pMOrg,pL,pU,pP);
	
	/* 根据LUP分解，计算逆矩阵 */
	for(i=0;i<iDim;i++)
	{
		for(j=0;j<iDim;j++)
			pB[j] = 0;
		pB[i] = 1;

		/* 根据LUP分解的结果，解线性方程，从而获得逆矩阵 
		   即AX=B，如果B为对角阵，那么X极为A的逆矩阵    */
		LUPSolve(iDim,pL,pU,pP,pB,pX);
		
		for(j=0;j<iDim;j++)
			*(pMRslt+j*iDim+i) = *(pX+j);
	}

	//释放内存
	free(pL);
	free(pU);
	free(pP);
	free(pB);
	free(pX);
		
}

/*
***************************************
函数名  ：LUPDecomp
功能描述：矩阵的LUP分解
参数    ：iDim，矩阵的阶数
		  pMtr，指向输入矩阵的指针
		  pL，指向L阵指针
		  pU，指向U阵指针
		  pP，指向P阵指针
返回值  ：无
***************************************
*/
void LUPDecomp(int iDim,double *pMtr,double *pL,double *pU,int *pP)
{	
	int i,j,k,iTmp;

	double fMax;
	double *pMCp = (double *)malloc(sizeof(double)*(iDim*iDim));

	/* 矩阵赋值 */
	for(i=0;i<iDim;i++)
		for(j=0;j<iDim;j++)
			*(pMCp+i*iDim+j) = *(pMtr+i*iDim+j);
	
	/* P阵初始化 */
	for(i=0;i<iDim;i++)
		pP[i] = i;
	
	/* LUP分解算法,具体可见算法导论相应章节 */
	for(k=0;k<iDim;k++)
	{
		/* 找出LU分解矩阵中当前第一列绝对值最大的元素 */
		fMax=0;
		
		for(i=k;i<iDim;i++)
		{
			if (fabs(*(pMCp+i*iDim+k)) > fMax)
			{
				fMax = fabs(*(pMCp+i*iDim+k));
				iTmp = i;
			}
		}
		
		/* 如果该最大元素为0，那么该矩阵为奇异矩阵 */
		if (fMax == 0){
			printf("Singular matrix!");
			system("pause");
		}
		
		/* 交换元素 */
		IntExch(&pP[k],&pP[iTmp]);
		
		/* 矩阵的行交换，选出主元 */
		for(i=0;i<iDim;i++)
			FloatExch(pMCp+k*iDim+i,pMCp+iTmp*iDim+i);
		
		/* 计算出Schur式 */
		for(i=k+1;i<iDim;i++)
		{
			*(pMCp+i*iDim+k) /= *(pMCp+k*iDim+k);
			
			for(j=k+1;j<iDim;j++)
				*(pMCp+i*iDim+j) -= (*(pMCp+i*iDim+k)) * (*(pMCp+k*iDim+j));
		}
		
		/* 获得L、U、P阵 */
		for(i=0;i<iDim;i++)
			for(j=0;j<iDim;j++)
			{
				if(i == j)
				{
					*(pL+i*iDim+j) = 1;
					*(pU+i*iDim+j) = *(pMCp+i*iDim+j);
				}
				else if(i > j)
				{
					*(pL+i*iDim+j) = *(pMCp+i*iDim+j);
					*(pU+i*iDim+j) = 0;
				}
				else
				{
					*(pL+i*iDim+j) = 0;
					*(pU+i*iDim+j) = *(pMCp+i*iDim+j);
				}
			}
	}

	free(pMCp);
}

/*
***************************************
函数名  ：LUPSolve
功能描述：利用矩阵的LUP分解，求解线性方程AX=B
参数    ：iDim，矩阵的阶数
		  pL，指向L阵指针
		  pU，指向U阵指针
		  pP，指向P阵指针
		  pB，指向方程中B阵的指针
		  pX，指向方程的解X阵的指针
返回值  ：无
***************************************
*/
void LUPSolve(int iDim,double *pL,double *pU,int *pP,double *pB,double *pX)
{
	int i,j,iTmp;
	double fSum;
	double *pY = (double *)malloc(sizeof(double)*iDim);
	
	/* 计算Y=inv(L)*B */
	for(i=0;i<iDim;i++)
	{
		fSum=0;
		for(j=0;j<i;j++)
			fSum =fSum + (*(pL+i*iDim+j)) * pY[j];

		iTmp = pP[i];

		pY[i] = pB[iTmp] - fSum;
	}

	/* 计算X=inv(U)*Y */
	for(i=iDim-1;i>-1;i--)
	{
		fSum = 0;
		for(j=i+1;j<iDim;j++)
			fSum = fSum + *(pU+i*iDim+j) * pX[j];

		pX[i] = (pY[i]-fSum)/(*(pU+i*iDim+i));
	}

	free(pY);
}

/*
***************************************
函数名  ：FloatExch
功能描述：交换浮点型数据
参数    ：pLeft，指向浮点型指针1
          pRight，指向浮点型指针2
返回值  ：无
***************************************
*/
void FloatExch(double *pLeft,double *pRight)
{
	double fTmp;
	fTmp    = *pLeft;
	*pLeft  = *pRight;
	*pRight = fTmp;
}

/*
***************************************
函数名  ：IntExch
功能描述：交换整型数据
参数    ：pLeft，指向整型指针1
          pRight，指向整型指针2
返回值  ：无
***************************************
*/
void IntExch(int *pLeft,int *pRight)
{
	int iTmp;
	iTmp    = *pLeft;
	*pLeft  = *pRight;
	*pRight = iTmp;
}