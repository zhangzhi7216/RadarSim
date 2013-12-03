#include "StdAfx.h"
#include "DataPacket.h"

CArchive & operator << (CArchive &ar, TrueDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Pos << frame.m_Vel << frame.m_Acc;
    return ar;
}

CArchive & operator >> (CArchive &ar, TrueDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Pos >> frame.m_Vel >> frame.m_Acc;
    return ar;
}

CArchive & operator << (CArchive &ar, NoiseDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Dis << frame.m_Theta << frame.m_Phi
        << frame.m_DisVar << frame.m_ThetaVar << frame.m_PhiVar;
    return ar;
}

CArchive & operator >> (CArchive &ar, NoiseDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Dis >> frame.m_Theta >> frame.m_Phi
        >> frame.m_DisVar >> frame.m_ThetaVar >> frame.m_PhiVar;
    return ar;
}

CArchive & operator << (CArchive &ar, TrueDataPacket &packet)
{
    ar << packet.m_PlaneTrueData << packet.m_TargetTrueDatas.size();
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        ar << packet.m_TargetTrueDatas[i];
    }
    return ar;
}

CArchive & operator >> (CArchive &ar, TrueDataPacket &packet)
{
    int n = 0;
    ar >> packet.m_PlaneTrueData >> n;
    for (int i = 0; i < n; ++i)
    {
        TrueDataFrame frame;
        ar >> frame;
        packet.m_TargetTrueDatas.push_back(frame);
    }
    return ar;
}

CArchive & operator << (CArchive &ar, NoiseDataPacket &packet)
{
    ar << packet.m_PlaneTrueData << packet.m_TargetNoiseDatas.size();
    for (int i = 0; i < packet.m_TargetNoiseDatas.size(); ++i)
    {
        ar << packet.m_TargetNoiseDatas[i];
    }
    return ar;
}

CArchive & operator >> (CArchive &ar, NoiseDataPacket &packet)
{
    int n = 0;
    ar >> packet.m_PlaneTrueData >> n;
    for (int i = 0; i < n; ++i)
    {
        NoiseDataFrame frame;
        ar >> frame;
        packet.m_TargetNoiseDatas.push_back(frame);
    }
    return ar;
}
