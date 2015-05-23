#define DATAFUSION_API _declspec(dllexport) 


#include "datafusion.h"
#include "CommonProcess.h"
#include "structDefinition.h"
#include "globalParameter.h"

void data_fusion(ECHO *Echo_TongLei,ECHO *Echo_RadarAIS,int *Echo_RA,int *Echo_TL,int *iSpt, TRACKINFO *pTrk,ECHO_PARAMETER *Echo_Param,int *TrkNum,int *FusionMethod)
{	
	/* ���������ļ��ʹ���������ļ�ָ�� */
//	FILE *pMeas1, *pMeas2, *pOutp;
//	TRACKINFO *pTrk = NULL;                                          // ��ʱ����
	/* iSpt��ʾ��ǰ֡�� */
//	int iSpt = 1; 
//	int FusionMethod = 1;

//	int A = 0;
//	int DD = 0;
	/* �ж��Ƿ��ڵ�ǰ֡����ʼ�º����ı�־λ */
//	int iAdd_Track_Flag = 0;

	/* �򿪴�������������ļ� */
/*	if ( (pMeas1 = fopen("Meas_RadarAIS.dat","r")) == NULL ){
		printf("Can't not open Meas_RadarAIS.dat!\n");
		exit(1);
	}

	if ( (pMeas2 = fopen("Meas_TongLei.dat","r")) == NULL ){
		printf("Can't not open Meas_TongLei.dat!\n");
		exit(1);
	}*/

	/* ��������������ļ� */
/*	if ( (pOutp = fopen("Result.dat","w")) == NULL ){
		printf("Can't not open Result.dat!\n");
		exit(1);
	}*/

//	while (iSpt <= (int) Total_Frame) {	


//		printf("--------- %d ----------\n",iSpt);                // ��ʾ��ǰ֡��


//		FrameRead_RadarAIS(pMeas1, iSpt);
//		FrameRead_TongLei(pMeas2, iSpt);
		Receive_Parameter(Echo_TongLei,Echo_RadarAIS,Echo_RA,Echo_TL,pTrk,Echo_Param,TrkNum);
		MTTORFusion_Decision();
		if ((*iSpt) == 1){
			/* �����˲���ģ�Ͳ��� */
			InitiateTrack();
			/* �����˲�			  */
			FilterPrediction();
		} 
		else{
			/* ����������Ĺ���   */
			GNN_Asso(FusionMethod);
			/* �����˺�������	  */
			Track_Trim();
		}
			Transmit_Trk(pTrk);

		/* ���Result�ļ� */
	//	OutFile(iSpt,pOutp);
	//	iSpt++;
	

//	fclose(pMeas1);
//	fclose(pMeas2);
//	fclose(pOutp);
//	system("pause");
//	return 0;
}

