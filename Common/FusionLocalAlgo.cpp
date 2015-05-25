#include "StdAfx.h"
#include "FusionLocalAlgo.h"
#include <assert.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include "datafusion.h"
#include "globalParameter.h"
#include "structDefinition.h"
#include "CommonProcess.h"



FusionLocalAlgo::FusionLocalAlgo()
: m_Type(FusionLocalAlgoTypeTest1)
{
}

FusionLocalAlgo::FusionLocalAlgo(const CString &name, FusionLocalAlgoType type)
: FusionAlgo(name)
, m_Type(type)
{
}

bool FusionLocalAlgo::Init()
{
    switch (m_Type)
    {
    case FusionLocalAlgoTypeTest1:
        m_Func = &FusionLocalAlgoTest1;
        break;
    case FusionLocalAlgoTypeTest2:
        m_Func = &FusionLocalAlgoTest2;
        break;
    default:
        CString msg;
        msg.AppendFormat(TEXT("未知本地融合算法%d."), m_Type);
        AfxMessageBox(msg);
        return false;
    }
    return true;
}

void FusionLocalAlgo::Output(CArchive &ar)
{
    ar << FusionAlgoTypeLocal << m_Type;
    FusionAlgo::Output(ar);
}

void FusionLocalAlgo::Input(CArchive &ar)
{
    int type;
    ar >> type;
    m_Type = (FusionLocalAlgoType)type;
    FusionAlgo::Input(ar);
}

bool FusionLocalAlgoTest1(FusionInput &input, FusionOutput &output)
{
    // This is how to use global var.
    g_GlobalVar[0].m_G[0] = 0;
    g_GlobalVar[0].m_G[1] = 0;
    map<SensorId, NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
    int interval = input.m_Interval;

    int nTargets = noiseDatas[SensorIdRadar].m_TargetNoiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        TrueDataFrame frame;
        frame.m_Time = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Time;
        frame.m_Id = 100 + iTarget + frame.m_Time / 100 * 100;
        frame.m_Type = iTarget % TargetTypeLast;
        for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; ++iSensor)
        {
            // frame += noiseDatas[(SensorId)iSensor].m_TargetNoiseDatas[iTarget];
        }
        frame.m_Pos = Position(800 - frame.m_Time, 100 + iTarget * 250, 100);
        frame.m_Vel = Velocity(-1, 0, 0);
        frame.m_IsKeyTarget = iTarget % 2 ? true : false;
        // frame /= SensorIdLast;
        output.m_FusionDataPacket.m_FusionDatas.push_back(frame);
        // frame = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget];
    }

    return true;
}

int g_iTrkNum_Org = 0;
int iEchoNum_RadarAIS_Org = 0;
int iEchoNum_TongLei_Org = 0;

int Spt_Org = 1;
int iSpt_Org = 1;
TRACKINFO sTracks_Org[MAX_TRACK];

ECHO sEcho_RadarAIS_Org[MAX_TRACK];
ECHO sEcho_TongLei_Org[MAX_TRACK];     
ECHO_PARAMETER sECHO_Param_Org;
ECHO sEcho_SelfPosition_Org[MAX_TRACK];

TRKINFO Targets[4];
TRKINFO orgTrk[4];
TRKINFO OwnShip[3];

RESULTINFO result[4];
int FusionMethod = 0;

/*
***************************************
函数名  ：FrameRead_RadarAIS
功能描述：读取RadarAIS的帧量测数据
参数    ：pMeas_RadarAIS，指向量测数据文件的指针
返回值  ：无
***************************************
*/
void FrameRead_RadarAIS(FusionInput &input)
{
	map<SensorId, NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
	int interval = input.m_Interval;

	int Radar_TargetNum = noiseDatas[SensorIdRadar].m_TargetNoiseDatas.size();
	int AIS_TargetNum = noiseDatas[SensorIdAis].m_TargetNoiseDatas.size();

	double fRng,fAzm,fElv,fNULL1,fNULL2,fNULL3,fNULL4,fNULL5,fNULL6,fNULL7,fNULL8;
	int Type;
	int MSpt,iIdx;
	int iCount = 0;
	int iflag = 0;

	int i;
	iEchoNum_RadarAIS_Org = 0;
	/* 读取雷达的观测数据 */
	for (int iTarget = 0; iTarget < Radar_TargetNum; ++iTarget){
		MSpt = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Time + 1;
		Type = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Type;
		fRng = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Dis;
		fAzm = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Theta / (double) JiaoToHu;
		fElv = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Phi / (double) JiaoToHu;

		if ( (fRng == 0) &(fAzm == 0) & (fElv == 0)){
			MSpt = -1;
		}
		if ( ( (0 < fRng) & (fRng <= (double) MAX_SCOPE))  & ( (-2* (double)PI <= fAzm) & (fAzm <= 2* (double)PI)) & ( (-2* (double)PI <= fElv) & (fElv <= 2* (double)PI))){

		}
		else{
			MSpt = -1;
		}

		if (iSpt_Org == MSpt){
			iflag = 1;
				sEcho_RadarAIS_Org[iCount].msr[0] = fRng;
				sEcho_RadarAIS_Org[iCount].msr[1] = fAzm;
				sEcho_RadarAIS_Org[iCount].msr[2] = fElv;
				sEcho_RadarAIS_Org[iCount].Type = Type;
				sEcho_RadarAIS_Org[iCount].Fusion_OrNot = 0;

				++iCount;
		}
	}
	/* 读取AIS的观测数据 */
	for (int iTarget = 0; iTarget < AIS_TargetNum; ++iTarget){
		MSpt = noiseDatas[SensorIdAis].m_TargetNoiseDatas[iTarget].m_Time + 1;
		Type = noiseDatas[SensorIdAis].m_TargetNoiseDatas[iTarget].m_Type;
		fRng = noiseDatas[SensorIdAis].m_TargetNoiseDatas[iTarget].m_Dis;
		fAzm = noiseDatas[SensorIdAis].m_TargetNoiseDatas[iTarget].m_Theta / (double) JiaoToHu;
		fElv = noiseDatas[SensorIdAis].m_TargetNoiseDatas[iTarget].m_Phi / (double) JiaoToHu;

		if ( (fRng == 0) &(fAzm == 0) & (fElv == 0)){
			MSpt = -1;
		}
		if ( ( (0 < fRng) & (fRng <= (double) MAX_SCOPE))  & ( (-2* (double)PI <= fAzm) & (fAzm <= 2* (double)PI)) & ( (-2* (double)PI <= fElv) & (fElv <= 2* (double)PI))){

		}
		else{
			MSpt = -1;
		}

		/* 孙工，我在这里认为TargetTypeLast表示为民船的类型；若判断AIS观测的数据不是来自民船的，就不读入来融合 */
		if ( Type != TargetTypeLast ){
			MSpt = -1;
		}

		if (iSpt_Org == MSpt){
			iflag = 1;
			sEcho_RadarAIS_Org[iCount].msr[0] = fRng;
			sEcho_RadarAIS_Org[iCount].msr[1] = fAzm;
			sEcho_RadarAIS_Org[iCount].msr[2] = fElv;
			sEcho_RadarAIS_Org[iCount].Type = Type;
			sEcho_RadarAIS_Org[iCount].Fusion_OrNot = 0;

			++iCount;
		}
	}

		if (iflag == 1){
			/* 储存当前帧的量测个数 */
			iEchoNum_RadarAIS_Org = iCount;
			if ( iSpt_Org == 1 ){
				g_iTrkNum_Org = iCount;
			}
		}

		/*            读取第一帧数据 即量测数据的参数         */
		if (iSpt_Org == 1){

				/*            雷达参数         */
				sECHO_Param_Org.Radar_RgeScope = input.m_RadarScopeMax;
				sECHO_Param_Org.Radar_Azi_MaxScope = input.m_RadarThetaMax / (double) JiaoToHu;
				sECHO_Param_Org.Radar_Azi_MinScope = 0;
				sECHO_Param_Org.Radar_Ele_MaxScope = input.m_RadarPhiMax / (double) JiaoToHu;
				sECHO_Param_Org.Radar_Ele_MinScope = 0;

				sECHO_Param_Org.Radar_rgeMN = input.m_RadarScopeVar;
				sECHO_Param_Org.Radar_aziMN = input.m_RadarThetaVar / (double) JiaoToHu;
				sECHO_Param_Org.Radar_eleMN = input.m_RadarPhiVar / (double) JiaoToHu;
				/*            AIS参数         */
				sECHO_Param_Org.AIS_RgeScope = input.m_AisXMax;
				sECHO_Param_Org.AIS_Azi_MaxScope = input.m_AisYMax / (double) JiaoToHu;
				sECHO_Param_Org.AIS_Azi_MinScope = 0;
				sECHO_Param_Org.AIS_Ele_MaxScope = input.m_AisYVar / (double) JiaoToHu;
				sECHO_Param_Org.AIS_Ele_MinScope = 0;

				sECHO_Param_Org.AIS_rgeMN = input.m_AisXVar;
				sECHO_Param_Org.AIS_aziMN = input.m_AisYVar / (double) JiaoToHu;
				sECHO_Param_Org.AIS_eleMN = input.m_AisZVar / (double) JiaoToHu;
		}
	
}

/*
***************************************
函数名  ：FrameRead_TongLei
功能描述：读取ESM的帧量测数据
参数    ：pMeas_TongLei，指向量测数据文件的指针
返回值  ：无
***************************************
*/
void FrameRead_TongLei(FusionInput &input)
{
	map<SensorId, NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
	int interval = input.m_Interval;

	int Tong_TargetNum = noiseDatas[SensorIdTong].m_TargetNoiseDatas.size();
	int Lei_TargetNum = noiseDatas[SensorIdLei].m_TargetNoiseDatas.size();
	double fRng,fAzm,fElv,fNULL1,fNULL2,fNULL3,fNULL4,fNULL5,fNULL6,fNULL7,fNULL8;
	int MSpt, Type;
	int iCount = 0;
	int iflag = 0;

	iEchoNum_TongLei_Org = 0;
	/* 读取通信侦察的观测数据 */
	for (int iTarget = 0; iTarget < Tong_TargetNum; ++iTarget){
		MSpt = noiseDatas[SensorIdTong].m_TargetNoiseDatas[iTarget].m_Time + 1;
		Type = noiseDatas[SensorIdTong].m_TargetNoiseDatas[iTarget].m_Type;
		fRng = noiseDatas[SensorIdTong].m_TargetNoiseDatas[iTarget].m_Dis;
		fAzm = noiseDatas[SensorIdTong].m_TargetNoiseDatas[iTarget].m_Theta / (double) JiaoToHu;
		fElv = noiseDatas[SensorIdTong].m_TargetNoiseDatas[iTarget].m_Phi / (double) JiaoToHu;

		if ( (fAzm == 0) & (fElv == 0)){
			MSpt = -1;
		}
	
		if (  ( (-2* (double)PI <= fAzm) & (fAzm <= 2* (double)PI)) & ( (-2* (double)PI <= fElv) & (fElv <= 2* (double)PI))){

		}
		else{
			MSpt = -1;
		}

		if (iSpt_Org == MSpt){
			iflag = 1;
			sEcho_TongLei_Org[iCount].msr[0] = fRng;
			sEcho_TongLei_Org[iCount].msr[1] = fAzm;
			sEcho_TongLei_Org[iCount].msr[2] = fElv;
			sEcho_TongLei_Org[iCount].Type = Type;
			sEcho_TongLei_Org[iCount].Fusion_OrNot = 0;

			++iCount;
		}
	}
	/* 读取雷达侦察的观测数据 */
	for (int iTarget = 0; iTarget < Lei_TargetNum; ++iTarget){
		MSpt = noiseDatas[SensorIdLei].m_TargetNoiseDatas[iTarget].m_Time + 1;
		Type = noiseDatas[SensorIdLei].m_TargetNoiseDatas[iTarget].m_Type;
		fRng = noiseDatas[SensorIdLei].m_TargetNoiseDatas[iTarget].m_Dis;
		fAzm = noiseDatas[SensorIdLei].m_TargetNoiseDatas[iTarget].m_Theta / (double) JiaoToHu;
		fElv = noiseDatas[SensorIdLei].m_TargetNoiseDatas[iTarget].m_Phi / (double) JiaoToHu;

		if ( (fAzm == 0) & (fElv == 0)){
			MSpt = -1;
		}

		if (  ( (-2* (double)PI <= fAzm) & (fAzm <= 2* (double)PI)) & ( (-2* (double)PI <= fElv) & (fElv <= 2* (double)PI))){

		}
		else{
			MSpt = -1;
		}

		if (iSpt_Org == MSpt){
			iflag = 1;
			sEcho_TongLei_Org[iCount].msr[0] = fRng;
			sEcho_TongLei_Org[iCount].msr[1] = fAzm;
			sEcho_TongLei_Org[iCount].msr[2] = fElv;
			sEcho_TongLei_Org[iCount].Type = Type;
			sEcho_TongLei_Org[iCount].Fusion_OrNot = 0;

			++iCount;
		}
	}

		if (iflag == 1){
			// 储存当前帧的量测个数 
			iEchoNum_TongLei_Org = iCount;
		}

		/*            读取第一帧数据 即量测数据的参数         */
		if (iSpt_Org == 1){

			/*            通信侦察参数         */
			sECHO_Param_Org.Tong_RgeScope = input.m_TongScopeMax;
			sECHO_Param_Org.Tong_Azi_MaxScope = input.m_TongThetaMax / (double) JiaoToHu;
			sECHO_Param_Org.Tong_Azi_MinScope = 0;
			sECHO_Param_Org.Tong_Ele_MaxScope = input.m_TongPhiMax / (double) JiaoToHu;
			sECHO_Param_Org.Tong_Ele_MinScope = 0;

			sECHO_Param_Org.Tong_aziMN = input.m_TongThetaVar / (double) JiaoToHu;
			sECHO_Param_Org.Tong_eleMN = input.m_TongPhiVar / (double) JiaoToHu;

			/*            雷达侦察参数         */
			sECHO_Param_Org.Lei_RgeScope = input.m_LeiScopeMax;
			sECHO_Param_Org.Lei_Azi_MaxScope = input.m_LeiThetaMax / (double) JiaoToHu;
			sECHO_Param_Org.Lei_Azi_MinScope = 0;
			sECHO_Param_Org.Lei_Ele_MaxScope = input.m_LeiPhiMax / (double) JiaoToHu;
			sECHO_Param_Org.Lei_Ele_MinScope = 0;

			sECHO_Param_Org.Lei_aziMN = input.m_LeiThetaVar / (double) JiaoToHu;
			sECHO_Param_Org.Lei_eleMN = input.m_LeiPhiVar / (double) JiaoToHu;
		}
}

/*
***************************************
函数名    OutFile
功能描述：融合结果输出
参数    ：无
返回值  ：无
***************************************
*/
void OutFile(int iSpt, FusionOutput &output)
{

	TRACKINFO *pTrk,*pTmp;
	int i,t,j;
	double k,D;
	double Tmp1,Tmp2;
	int Idx = 0;
	
	t = iSpt;

	for (i=0;i<g_iTrkNum_Org;i++){
		pTrk = &sTracks_Org[i];
		if ( pTrk->Conf_flag == 1){
			if ( iSpt<10 ){
				D = 0.0000006;
			}
			else{
				D = 0.0000008;
			}
			k = pTrk->Points;
			if ( k > D*t ){
				if ( (pTrk->Type == 3) && (pTrk->Civi_Num > 5) ){
					pTrk->Type = 4;
				}
				if ( (pTrk->Type == 3) || (pTrk->Type == 4) ){
					TrueDataFrame frame1;
					frame1.m_Time = iSpt;
					frame1.m_Id = pTrk->idx;
					frame1.m_Pos = Position(pTrk->fsn.xk[0],pTrk->fsn.xk[3],0.0);
				//	frame1.m_Vel = Velocity(pTrk->fsn.xk[1],pTrk->fsn.xk[4],0.0);
					frame1.m_Type = pTrk->Type;
					frame1.m_IsKeyTarget = pTrk->Theat;
					output.m_FusionDataPacket.m_FusionDatas.push_back(frame1);
					//fprintf(pOutp,"%d\t %lf\t %lf\t %lf\t %d %d %d\n",iSpt,pTrk->fsn.xk[0] ,pTrk->fsn.xk[3],0.0,pTrk->idx, pTrk->Type, pTrk->Theat);
				} 
				else{
					TrueDataFrame frame2;
					frame2.m_Time = iSpt;
					frame2.m_Id = pTrk->idx;
					frame2.m_Pos = Position(pTrk->fsn.xk[0],pTrk->fsn.xk[3],pTrk->fsn.xk[6]);
					//	frame2.m_Vel = Velocity(pTrk->fsn.xk[1],pTrk->fsn.xk[4],0.0);
					frame2.m_Type = pTrk->Type;
					frame2.m_IsKeyTarget = pTrk->Theat;
					output.m_FusionDataPacket.m_FusionDatas.push_back(frame2);
					//fprintf(pOutp,"%d\t %lf\t %lf\t %lf\t %d %d %d\n",iSpt,pTrk->fsn.xk[0] ,pTrk->fsn.xk[3],pTrk->fsn.xk[6],pTrk->idx, pTrk->Type, pTrk->Theat);
				}
			}
		}

	}

	// 判断出哪个目标的威胁度最大

	for (i=0;i<g_iTrkNum_Org;i++){
		pTrk = &sTracks_Org[i];
		if ( pTrk->Type == 3){
			Tmp1 = sqrt( pow(pTrk->fsn.xk[0],2) + pow(pTrk->fsn.xk[3],2) );
			Idx = i;
			break;
		} 
		else{
			Tmp1 = sqrt( pow(pTrk->fsn.xk[0],2) + pow(pTrk->fsn.xk[3],2) + pow(pTrk->fsn.xk[6],2) );
			Idx = i;
			break;
		}
	}
	for (j=0;j<g_iTrkNum_Org;j++){
		pTmp = &sTracks_Org[j];
		pTmp->Theat = 0;

		if ( pTmp->Conf_flag == 1){
			if ( iSpt<10 ){
				D = 0.0000006;
			}
			else{
				D = 0.0000008;
			}
			k = pTmp->Points;
			if ( k > D*t ){

				if ( pTmp->Type == 3){
					Tmp2 = sqrt( pow(pTmp->fsn.xk[0],2) + pow(pTmp->fsn.xk[3],2) );
					//	Idx = i;
				} 
				else{
					Tmp2 = sqrt( pow(pTmp->fsn.xk[0],2) + pow(pTmp->fsn.xk[3],2) + pow(pTmp->fsn.xk[6],2) );
					//	Idx = i;
				}

				if ( Tmp2 < Tmp1){
					Tmp1 = Tmp2;
					Idx = j;
				}
			}
		}
	}
	sTracks_Org[Idx].Theat = 1;

	// 对储存量测的结构体清零 
	for(i=0; i< (int) MAX_TRACK; i++){
		sEcho_RadarAIS_Org[i].Fusion_OrNot = 0;
		sEcho_RadarAIS_Org[i].Type = 0;
		sEcho_RadarAIS_Org[i].msr[0] = 0;
		sEcho_RadarAIS_Org[i].msr[1] = 0;
		sEcho_RadarAIS_Org[i].msr[2] = 0;

		sEcho_TongLei_Org[i].Fusion_OrNot = 0;
		sEcho_TongLei_Org[i].Type = 0;
		sEcho_TongLei_Org[i].msr[0] = 0;
		sEcho_TongLei_Org[i].msr[1] = 0;
		sEcho_TongLei_Org[i].msr[2] = 0;
	}

}


bool FusionLocalAlgoTest2(FusionInput &input, FusionOutput &output)
{
	// This is how to use global var.
	/*
	g_GlobalVar[0].m_G[0] = 0;
	g_GlobalVar[0].m_G[1] = 0;
	map<SensorId, NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
	int interval = input.m_Interval;

	int nTargets = noiseDatas[SensorIdRadar].m_TargetNoiseDatas.size();
	for (int iTarget = 0; iTarget < nTargets; ++iTarget)
	{
		TrueDataFrame frame;
		frame.m_Time = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Time;
		frame.m_Id = 100 + iTarget + frame.m_Time / 100 * 100;
		frame.m_Type = iTarget % TargetTypeLast;
		for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; ++iSensor)
		{
			// frame += noiseDatas[(SensorId)iSensor].m_TargetNoiseDatas[iTarget];
		}
		frame.m_Pos = Position(800 - frame.m_Time, 100 + iTarget * 250, 100);
		frame.m_Vel = Velocity(-1, 0, 0);
		frame.m_IsKeyTarget = iTarget % 2 ? true : false;
		// frame /= SensorIdLast;
		output.m_FusionDataPacket.m_FusionDatas.push_back(frame);
		// frame = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget];
	} 
	*/

	FusionMethod = 2;
	FrameRead_RadarAIS(input);
	FrameRead_TongLei(input);
	data_fusion(&sEcho_TongLei_Org[0],&sEcho_RadarAIS_Org[0],&iEchoNum_RadarAIS_Org,&iEchoNum_TongLei_Org,&iSpt_Org,&sTracks_Org[0],&sECHO_Param_Org,&g_iTrkNum_Org,&FusionMethod);
	OutFile(iSpt_Org, output);
	iSpt_Org++;
	return true;
}

