#include "StdAfx.h"
#include "FusionLocalAlgo.h"
#include <assert.h>

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

bool FusionLocalAlgoTest1(const vector<NoiseDataPacket> &noiseDatas, FusionOutput &output)
{
    int nTargets = noiseDatas.front().m_TargetNoiseDatas.size();
    int nPlanes = noiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        NoiseDataFrame frame;
        assert(noiseDatas.front().m_TargetNoiseDatas.size() > iTarget);
        frame.m_Time = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Time;
        frame.m_Id = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Id;
        for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
        {
            frame += noiseDatas[iPlane].m_TargetNoiseDatas[iTarget];
        }
        frame /= nPlanes;
        output.m_FusionData.m_FusionDatas.push_back(frame);
        frame = noiseDatas.front().m_TargetNoiseDatas[iTarget];
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
    return true;
}

bool FusionLocalAlgoTest2(const vector<NoiseDataPacket> &noiseDatas, FusionOutput &output)
{
    int nTargets = noiseDatas.front().m_TargetNoiseDatas.size();
    int nPlanes = noiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        NoiseDataFrame frame;
        assert(noiseDatas.front().m_TargetNoiseDatas.size() > iTarget);
        frame.m_Time = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Time;
        frame.m_Id = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Id;
        for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
        {
            frame += noiseDatas[iPlane].m_TargetNoiseDatas[iTarget];
        }
        frame /= nPlanes;
        output.m_FusionData.m_FusionDatas.push_back(frame);
        frame = noiseDatas.back().m_TargetNoiseDatas[iTarget];
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
    return true;
}