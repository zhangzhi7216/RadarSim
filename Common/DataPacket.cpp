#include "StdAfx.h"
#include "DataPacket.h"

CArchive & operator << (CArchive &ar, TrueDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Id << frame.m_Pos << frame.m_Vel << frame.m_Acc;
    return ar;
}

CArchive & operator >> (CArchive &ar, TrueDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Id >> frame.m_Pos >> frame.m_Vel >> frame.m_Acc;
    return ar;
}

CArchive & operator << (CArchive &ar, NoiseDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Id << frame.m_Dis << frame.m_Theta << frame.m_Phi
        << frame.m_DisVar << frame.m_ThetaVar << frame.m_PhiVar;
    return ar;
}

CArchive & operator >> (CArchive &ar, NoiseDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Id >> frame.m_Dis >> frame.m_Theta >> frame.m_Phi
        >> frame.m_DisVar >> frame.m_ThetaVar >> frame.m_PhiVar;
    return ar;
}

template <class T>
__declspec(dllexport) CArchive & operator << (CArchive &ar, vector<T> &v);
template <class T>
__declspec(dllexport) CArchive & operator >> (CArchive &ar, vector<T> &v);

CArchive & operator << (CArchive &ar, TrueDataPacket &packet)
{
    ar << packet.m_PlaneTrueData << packet.m_TargetTrueDatas;
    return ar;
}

CArchive & operator >> (CArchive &ar, TrueDataPacket &packet)
{
    ar >> packet.m_PlaneTrueData >> packet.m_TargetTrueDatas;
    return ar;
}

CArchive & operator << (CArchive &ar, NoiseDataPacket &packet)
{
    ar << packet.m_PlaneTrueData << packet.m_TargetNoiseDatas;
    return ar;
}

CArchive & operator >> (CArchive &ar, NoiseDataPacket &packet)
{
    ar >> packet.m_PlaneTrueData >> packet.m_TargetNoiseDatas;
    return ar;
}

CArchive & operator << (CArchive &ar, FusionDataPacket &packet)
{
    ar << packet.m_FusionDatas << packet.m_FilterDatas << packet.m_NoiseDatas;
    return ar;
}

CArchive & operator >> (CArchive &ar, FusionDataPacket &packet)
{
    ar >> packet.m_FusionDatas >> packet.m_FilterDatas >> packet.m_NoiseDatas;
    return ar;
}