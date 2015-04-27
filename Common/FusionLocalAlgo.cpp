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
        frame.m_Id = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Id;
        for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; ++iSensor)
        {
            // frame += noiseDatas[(SensorId)iSensor].m_TargetNoiseDatas[iTarget];
        }
        // frame /= SensorIdLast;
        output.m_FusionDataPacket.m_FusionDatas.push_back(frame);
        // frame = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget];
    }

    return true;
}

bool FusionLocalAlgoTest2(FusionInput &input, FusionOutput &output)
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
        frame.m_Id = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Id;
        for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; ++iSensor)
        {
            // frame += noiseDatas[(SensorId)iSensor].m_TargetNoiseDatas[iTarget];
        }
        // frame /= SensorIdLast;
        output.m_FusionDataPacket.m_FusionDatas.push_back(frame);
        // frame = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget];
    }

    return true;
}