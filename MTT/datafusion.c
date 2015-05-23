#define DATAFUSION_API _declspec(dllexport) 


#include "datafusion.h"
#include "CommonProcess.h"
#include "structDefinition.h"
#include "globalParameter.h"

void data_fusion(ECHO *Echo_TongLei,ECHO *Echo_RadarAIS,int *Echo_RA,int *Echo_TL,int *iSpt, TRACKINFO *pTrk,ECHO_PARAMETER *Echo_Param,int *TrkNum,int *FusionMethod)
{	
	/* 量测输入文件和处理结果输出文件指针 */
//	FILE *pMeas1, *pMeas2, *pOutp;
//	TRACKINFO *pTrk = NULL;                                          // 临时变量
	/* iSpt表示当前帧数 */
//	int iSpt = 1; 
//	int FusionMethod = 1;

//	int A = 0;
//	int DD = 0;
	/* 判断是否在当前帧，起始新航迹的标志位 */
//	int iAdd_Track_Flag = 0;

	/* 打开储存量测的数据文件 */
/*	if ( (pMeas1 = fopen("Meas_RadarAIS.dat","r")) == NULL ){
		printf("Can't not open Meas_RadarAIS.dat!\n");
		exit(1);
	}

	if ( (pMeas2 = fopen("Meas_TongLei.dat","r")) == NULL ){
		printf("Can't not open Meas_TongLei.dat!\n");
		exit(1);
	}*/

	/* 打开输出处理结果的文件 */
/*	if ( (pOutp = fopen("Result.dat","w")) == NULL ){
		printf("Can't not open Result.dat!\n");
		exit(1);
	}*/

//	while (iSpt <= (int) Total_Frame) {	


//		printf("--------- %d ----------\n",iSpt);                // 显示当前帧号


//		FrameRead_RadarAIS(pMeas1, iSpt);
//		FrameRead_TongLei(pMeas2, iSpt);
		Receive_Parameter(Echo_TongLei,Echo_RadarAIS,Echo_RA,Echo_TL,pTrk,Echo_Param,TrkNum);
		MTTORFusion_Decision();
		if ((*iSpt) == 1){
			/* 设置滤波的模型参数 */
			InitiateTrack();
			/* 航迹滤波			  */
			FilterPrediction();
		} 
		else{
			/* 航迹与量测的关联   */
			GNN_Asso(FusionMethod);
			/* 航迹滤后期整理	  */
			Track_Trim();
		}
			Transmit_Trk(pTrk);

		/* 输出Result文件 */
	//	OutFile(iSpt,pOutp);
	//	iSpt++;
	

//	fclose(pMeas1);
//	fclose(pMeas2);
//	fclose(pOutp);
//	system("pause");
//	return 0;
}

