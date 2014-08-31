#include "StdAfx.h"
#include "FusionLocalAlgo.h"
#include <assert.h>
#include <math.h>

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

bool FusionLocalAlgoTest1(const FusionInput &input, FusionOutput &output)
{
    // This is how to use global var.
    //g_GlobalVar[0][0].m_G1 = 0;
    //g_GlobalVar[0][0].m_G2 = 0;
    //g_GlobalVar[0][0]
    const vector<NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
    int interval = input.m_Interval;
    int nTargets = noiseDatas.front().m_TargetNoiseDatas.size();
    int nPlanes = noiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        TrueDataFrame decomposition;
        TrueDataFrame addition;
        int PlaneCount = 0;
        assert(noiseDatas.front().m_TargetNoiseDatas.size() > iTarget);
        addition.m_Time = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Time;
        addition.m_Id = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Id;
        addition.m_Pos.X = 0;
        addition.m_Pos.Y = 0;
        addition.m_Pos.Z = 0;
        addition.m_Vel.X = 0;//初始时间速度设为零
        addition.m_Vel.Y = 0;
        addition.m_Vel.Z = 0;
        for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
        {
            if(noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis != 0)
            {  
                if(addition.m_Time==0)
                {
                    decomposition.m_Pos.X = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Theta * 0.017453292 + atan(noiseDatas[iPlane].m_PlaneTrueData.m_Vel.Y/noiseDatas[iPlane].m_PlaneTrueData.m_Vel.X)) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.X;
                    decomposition.m_Pos.Y = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) * sin( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Theta * 0.017453292 + atan(noiseDatas[iPlane].m_PlaneTrueData.m_Vel.Y/noiseDatas[iPlane].m_PlaneTrueData.m_Vel.X)) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Y;
                    decomposition.m_Pos.Z = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * sin( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Z;
                    addition.m_Pos.X += decomposition.m_Pos.X;
                    addition.m_Pos.Y += decomposition.m_Pos.Y;
                    addition.m_Pos.Z += decomposition.m_Pos.Z;
                    // g_GlobalVar[iTarget][iPlane].m_G1 = decomposition.m_Pos.X;//G1保存X坐标
                    // g_GlobalVar[iTarget][iPlane].m_G2 = decomposition.m_Pos.Y;//G2保存Y坐标
                    // g_GlobalVar[iTarget][iPlane].m_G3 = decomposition.m_Pos.Z;//G3保存Z坐标
                    // g_GlobalVar[iTarget][iPlane].m_G4 = decomposition.m_Time;//G4保存时间
                    PlaneCount++;
                }
                else
                {            
                    decomposition.m_Pos.X = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Theta * 0.017453292 + atan(noiseDatas[iPlane].m_PlaneTrueData.m_Vel.Y/noiseDatas[iPlane].m_PlaneTrueData.m_Vel.X)) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.X;
                    decomposition.m_Pos.Y = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * cos( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) * sin( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Theta * 0.017453292 + atan(noiseDatas[iPlane].m_PlaneTrueData.m_Vel.Y/noiseDatas[iPlane].m_PlaneTrueData.m_Vel.X)) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Y;
                    decomposition.m_Pos.Z = noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Dis * sin( noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi * 0.017453292) + noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Z;
                    // decomposition.m_Vel.X = (decomposition.m_Pos.X - g_GlobalVar[iTarget][iPlane].m_G1)/(addition.m_Time-g_GlobalVar[iTarget][iPlane].m_G4);
                    // decomposition.m_Vel.Y = (decomposition.m_Pos.Y - g_GlobalVar[iTarget][iPlane].m_G2)/(addition.m_Time-g_GlobalVar[iTarget][iPlane].m_G4);
                    // decomposition.m_Vel.Z = (decomposition.m_Pos.Z - g_GlobalVar[iTarget][iPlane].m_G3)/(addition.m_Time-g_GlobalVar[iTarget][iPlane].m_G4);
                    addition.m_Pos.X += decomposition.m_Pos.X;
                    addition.m_Pos.Y += decomposition.m_Pos.Y;
                    addition.m_Pos.Z += decomposition.m_Pos.Z;
                    addition.m_Vel.X += decomposition.m_Vel.X;
                    addition.m_Vel.Y += decomposition.m_Vel.Y;
                    addition.m_Vel.Z += decomposition.m_Vel.Z;
                    // g_GlobalVar[iTarget][iPlane].m_G1 = decomposition.m_Pos.X;//G1保存X坐标
                    // g_GlobalVar[iTarget][iPlane].m_G2 = decomposition.m_Pos.Y;//G2保存Y坐标
                    // g_GlobalVar[iTarget][iPlane].m_G3 = decomposition.m_Pos.Z;//G3保存Z坐标
                    // g_GlobalVar[iTarget][iPlane].m_G4 = addition.m_Time;//G4保存时间      
                    //frame.m_Dis = sqrt(pow(decomposition.m_Pos.X,2)+pow(decomposition.m_Pos.Y,2)+pow(decomposition.m_Pos.Z,2));
                    //frame.m_Phi = atan(noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Z / sqrt(pow(noiseDatas[iPlane].m_PlaneTrueData.m_Pos.X,2)+pow(noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Y,2))) + noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Phi;
                    //frame.m_Theta = atan(noiseDatas[iPlane].m_PlaneTrueData.m_Pos.X / noiseDatas[iPlane].m_PlaneTrueData.m_Pos.Y) + noiseDatas[iPlane].m_TargetNoiseDatas[iTarget].m_Theta;
                    PlaneCount++;
                }
            }
        }
        if(PlaneCount != 0)addition /= PlaneCount;
        output.m_FusionData.m_FusionDatas.push_back(addition);
        output.m_FusionData.m_FilterDatas.push_back(addition);
    }
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        output.m_FusionData.m_NoiseDatas.push_back(noiseDatas[iPlane]);
    }

    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        ControlDataPacket packet;
        packet.m_FusionData = output.m_FusionData;
        packet.m_ControlData.m_Time = noiseDatas[iPlane].m_PlaneTrueData.m_Time;
        packet.m_ControlData.m_Id = noiseDatas[iPlane].m_PlaneTrueData.m_Id;
        packet.m_ControlData.m_C1 = 1.234;
        output.m_ControlDatas.push_back(packet);
    }
    return true;
}

bool FusionLocalAlgoTest2(const FusionInput &input, FusionOutput &output)
{
    const vector<NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
    int interval = input.m_Interval;

    int nTargets = noiseDatas.front().m_TargetNoiseDatas.size();
    int nPlanes = noiseDatas.size();
	static int t = 0;
	for (int iTarget = 0; iTarget < nTargets; ++iTarget)
	{
		TrueDataFrame frame;
		assert(noiseDatas.front().m_TargetNoiseDatas.size() > iTarget);
		frame.m_Time = t * interval;
		frame.m_Id = 100 + iTarget;
		if (iTarget == 0)
		{
			frame.m_Pos = Position(400000, 110, 23000);
			frame.m_Vel = Position(-440, 10, 0);
		}
		else if (iTarget == 1)
		{
			frame.m_Pos = Position(410000, 24000, 19000);
			frame.m_Vel = Position(-460, 5, 0);
		}
		else if (iTarget == 2)
		{
			frame.m_Pos = Position(420000, 40000, 18000);
			frame.m_Vel = Position(-480, -5, 0);
		}
		else if (iTarget == 3)
		{
			frame.m_Pos = Position(430000, 30300, 17000);
			frame.m_Vel = Position(-480, 0, 0);
		}
		frame.m_Pos += (frame.m_Vel * t * interval);
		output.m_FusionData.m_FusionDatas.push_back(frame);
		output.m_FusionData.m_FilterDatas.push_back(frame);
	}
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        output.m_FusionData.m_NoiseDatas.push_back(noiseDatas[iPlane]);
    }

    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        ControlDataPacket packet;
        packet.m_FusionData = output.m_FusionData;
        packet.m_ControlData.m_Time = noiseDatas[iPlane].m_PlaneTrueData.m_Time;
        packet.m_ControlData.m_Id = noiseDatas[iPlane].m_PlaneTrueData.m_Id;
        output.m_ControlDatas.push_back(packet);
    }
	t++;
	return true;
}