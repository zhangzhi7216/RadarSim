/*
***************************************
�ļ���  ��matrix.c
����    �����������ش�����
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrixOperation.h"

/*
***************************************
*         ���ļ��ڵĺ�������          *
***************************************
*/

/*
***************************************
������  ��MtrMtrMulti
��������������1�����2���
����    ��iRow������1������
          iLCol������1������
		  iRCol������2������
		  pMLeft��ָ�����1��ָ��
		  pMRight��ָ�����2��ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
***************************************
*/
void MtrMtrMulti(int iRow,int iLCol,int iRCol, double *pMLeft, double *pMRight, double *pMRslt)
{ 
	int i,j,k;

	/* �������������Ԫ����0.0 */
	for(i=0;i<iRow*iRCol;i++){
		*pMRslt = 0.0;
		pMRslt++;
	}

	/* ����ָ����������ʼλ�� */
	pMRslt = pMRslt-iRow*iRCol;

	/* ���þ���˷����壬��������������Ԫ�ص�ֵ */
	for(i=0;i<iRow;i++){
		for(j=0;j<iRCol;j++){
			for(k=0;k<iLCol;k++) 
				pMRslt[i*iRCol+j] = pMRslt[i*iRCol+j] + pMLeft[i*iLCol+k] * pMRight[k*iRCol+j];
		}
	}
}

/*
***************************************
������  ��MtrCnstMulti
���������������볣�����
����    ��iRow�����������
          iCol�����������
		  fConst,����
		  pMCur��ָ�򱻳˾����ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
***************************************
*/
void MtrCnstMulti(int iRow,int iCol,double fConst,double *pMCur,double *pMRslt)
{
	int i,j;

	/* ���������Ԫ�طֱ���Գ��� */
	for(i=0;i<iRow;i++)
		for(j=0;j<iCol;j++)
			pMRslt[i*iCol+j] = fConst * pMCur[i*iCol+j];
}


/*
***************************************
������  ��MtrMtrPlus
��������������1�����2���
����    ��iRow�����������
          iCol�����������
		  pMLeft��ָ�����1��ָ��
		  pMRight��ָ�����2��ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
***************************************
*/
void MtrMtrPlus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt)
{ 
	int i;
	int iTot = iRow * iCol;

	/* �����ж�ӦԪ����� */
	for(i=0;i<iTot;i++){  
		*pMRslt = *pMLeft + *pMRight;

		pMRslt++;
		pMLeft++;
		pMRight++;
	}
}

/*
***************************************
������  ��MtrMtrMinus
��������������1�����2���
����    ��iRow�����������
          iCol�����������
		  pMLeft��ָ�����1��ָ��
		  pMRight��ָ�����2��ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
***************************************
*/
void MtrMtrMinus(int iRow,int iCol, double *pMLeft, double *pMRight, double *pMRslt)
{	
	int i;
	int iTot = iRow * iCol;

	/* �����ж�ӦԪ����� */
	for(i=0;i<iTot;i++){  
		*pMRslt = *pMLeft - *pMRight;

		pMRslt++;
		pMLeft++;
		pMRight++;
	}
}

/*
***************************************
������  ��MtrDiag
������������n�׾���Խǻ�Ϊm*n�׾���
����    ��iOrg���Խǻ�ǰ����Ľ���
          iRslt���Խǻ������Ľ���
		  pMOrg��ָ��Ҫ�Խǻ��ľ����ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
***************************************
*/
void MtrDiag(int iOrg,int iRslt,double *pMOrg,double *pMRslt)
{
	int i,j;
	int iTmp;

	/* ���ݾ���Ԫ��λ���趨��Ӧ��ֵ */
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
������  ��VectDiag
������������mά������Ϊm�׶Խ���
����    ��iDim��������ά��
		  pVect��ָ�����������飩��ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
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
������  ��MtrAssign
��������������ĸ�ֵ
����    ��iRow�����������
          iCol�����������
		  pMOrg��ָ����������ָ��
		  pMRslt��ָ����������ָ��
����ֵ  ����
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
������  ��MtrInitialize
�������������������Ԫ�س�ʼ��Ϊ0
����    ��iRow�����������
          iCol�����������
		  pMRslt��ָ����������ָ��
����ֵ  ����
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
������  ��MtrTrans
���������������ת��
����    ��iRow��ת��ǰ���������
          iCol��ת��ǰ���������
		  pMOrg��ָ��ת��ǰ�ľ����ָ��
		  pMRslt��ָ��ת�ú�ľ����ָ��
����ֵ  ����
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
������  ��MtrDet
��������������n�׾�������ʽ��ֵ
����    ��iDim������Ľ���
		  pMOrg��ָ������ָ��
����ֵ  ��fRelt������ʽ��ֵ
***************************************
*/
double MtrDet(int iDim,double *pMOrg)
{
	int i,j,iCount;
	double fRslt = 1.0;

	/* �����ڴ� */
	double *pL = (double *)malloc(sizeof(double)*(iDim*iDim));
	double *pU = (double *)malloc(sizeof(double)*(iDim*iDim));
	int    *pP = (int *)malloc(sizeof(int)*iDim);
	
	/* ����LUP�ֽ� */
	LUPDecomp(iDim,pMOrg,pL,pU,pP);

	for(i=0;i<iDim;i++)
		fRslt = fRslt * (*(pU+i*iDim+i));

	/* �ж�����ķ��� */
	iCount = 0;
	for(i=0;i<iDim;i++)
		for(j=i+1;j<iDim;j++)
			if(pP[j] < pP[i]) iCount++;

	if((iCount%2) == 1)
		fRslt = -fRslt;

	/* �ͷ��ڴ� */
	free(pL);
	free(pU);
	free(pP);

	return fRslt;
}

/*
***************************************
������  ��MtrInvers
��������������n�׾���������
����    ��iDim������Ľ���
		  pMOrg��ָ����������ָ��
		  pMRslt��ָ��������ָ��
����ֵ  ����
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
	
	/* �����LUP�ֽ� */
	LUPDecomp(iDim,pMOrg,pL,pU,pP);
	
	/* ����LUP�ֽ⣬��������� */
	for(i=0;i<iDim;i++)
	{
		for(j=0;j<iDim;j++)
			pB[j] = 0;
		pB[i] = 1;

		/* ����LUP�ֽ�Ľ���������Է��̣��Ӷ��������� 
		   ��AX=B�����BΪ�Խ�����ôX��ΪA�������    */
		LUPSolve(iDim,pL,pU,pP,pB,pX);
		
		for(j=0;j<iDim;j++)
			*(pMRslt+j*iDim+i) = *(pX+j);
	}

	//�ͷ��ڴ�
	free(pL);
	free(pU);
	free(pP);
	free(pB);
	free(pX);
		
}

/*
***************************************
������  ��LUPDecomp
���������������LUP�ֽ�
����    ��iDim������Ľ���
		  pMtr��ָ����������ָ��
		  pL��ָ��L��ָ��
		  pU��ָ��U��ָ��
		  pP��ָ��P��ָ��
����ֵ  ����
***************************************
*/
void LUPDecomp(int iDim,double *pMtr,double *pL,double *pU,int *pP)
{	
	int i,j,k,iTmp;

	double fMax;
	double *pMCp = (double *)malloc(sizeof(double)*(iDim*iDim));

	/* ����ֵ */
	for(i=0;i<iDim;i++)
		for(j=0;j<iDim;j++)
			*(pMCp+i*iDim+j) = *(pMtr+i*iDim+j);
	
	/* P���ʼ�� */
	for(i=0;i<iDim;i++)
		pP[i] = i;
	
	/* LUP�ֽ��㷨,����ɼ��㷨������Ӧ�½� */
	for(k=0;k<iDim;k++)
	{
		/* �ҳ�LU�ֽ�����е�ǰ��һ�о���ֵ����Ԫ�� */
		fMax=0;
		
		for(i=k;i<iDim;i++)
		{
			if (fabs(*(pMCp+i*iDim+k)) > fMax)
			{
				fMax = fabs(*(pMCp+i*iDim+k));
				iTmp = i;
			}
		}
		
		/* ��������Ԫ��Ϊ0����ô�þ���Ϊ������� */
		if (fMax == 0){
			printf("Singular matrix!");
			system("pause");
		}
		
		/* ����Ԫ�� */
		IntExch(&pP[k],&pP[iTmp]);
		
		/* ������н�����ѡ����Ԫ */
		for(i=0;i<iDim;i++)
			FloatExch(pMCp+k*iDim+i,pMCp+iTmp*iDim+i);
		
		/* �����Schurʽ */
		for(i=k+1;i<iDim;i++)
		{
			*(pMCp+i*iDim+k) /= *(pMCp+k*iDim+k);
			
			for(j=k+1;j<iDim;j++)
				*(pMCp+i*iDim+j) -= (*(pMCp+i*iDim+k)) * (*(pMCp+k*iDim+j));
		}
		
		/* ���L��U��P�� */
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
������  ��LUPSolve
�������������þ����LUP�ֽ⣬������Է���AX=B
����    ��iDim������Ľ���
		  pL��ָ��L��ָ��
		  pU��ָ��U��ָ��
		  pP��ָ��P��ָ��
		  pB��ָ�򷽳���B���ָ��
		  pX��ָ�򷽳̵Ľ�X���ָ��
����ֵ  ����
***************************************
*/
void LUPSolve(int iDim,double *pL,double *pU,int *pP,double *pB,double *pX)
{
	int i,j,iTmp;
	double fSum;
	double *pY = (double *)malloc(sizeof(double)*iDim);
	
	/* ����Y=inv(L)*B */
	for(i=0;i<iDim;i++)
	{
		fSum=0;
		for(j=0;j<i;j++)
			fSum =fSum + (*(pL+i*iDim+j)) * pY[j];

		iTmp = pP[i];

		pY[i] = pB[iTmp] - fSum;
	}

	/* ����X=inv(U)*Y */
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
������  ��FloatExch
������������������������
����    ��pLeft��ָ�򸡵���ָ��1
          pRight��ָ�򸡵���ָ��2
����ֵ  ����
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
������  ��IntExch
����������������������
����    ��pLeft��ָ������ָ��1
          pRight��ָ������ָ��2
����ֵ  ����
***************************************
*/
void IntExch(int *pLeft,int *pRight)
{
	int iTmp;
	iTmp    = *pLeft;
	*pLeft  = *pRight;
	*pRight = iTmp;
}