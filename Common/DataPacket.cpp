#include "StdAfx.h"
#include "DataPacket.h"

CArchive & operator << (CArchive &ar, TrueDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Id << frame.m_Pos << frame.m_Vel << frame.m_Acc << frame.m_State;
    return ar;
}

CArchive & operator >> (CArchive &ar, TrueDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Id >> frame.m_Pos >> frame.m_Vel >> frame.m_Acc >> frame.m_State;
    return ar;
}

wofstream & operator << (wofstream &os, TrueDataFrame &frame)
{
    os << frame.m_Time << TEXT(" ") << frame.m_Id << TEXT(" ");
    os << frame.m_Pos << TEXT(" ");
    os << frame.m_Vel << TEXT(" ");
    os << frame.m_Acc << TEXT(" ");
    os << frame.m_State << TEXT(" ");
    return os;
}

wifstream & operator >> (wifstream &is, TrueDataFrame &frame)
{
    is >> frame.m_Time >> frame.m_Id;
    is >> frame.m_Pos >> frame.m_Vel >> frame.m_Acc >> frame.m_State;
    return is;
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

wofstream & operator << (wofstream &os, NoiseDataFrame &frame)
{
    os << frame.m_Time << TEXT(" ") << frame.m_Id << TEXT(" ")
        << frame.m_Dis << TEXT(" ") << frame.m_Theta << TEXT(" ") << frame.m_Phi << TEXT(" ")
        << frame.m_DisVar << TEXT(" ") << frame.m_ThetaVar << TEXT(" ") << frame.m_PhiVar;
    return os;
}

wifstream & operator >> (wifstream &is, NoiseDataFrame &frame)
{
    is >> frame.m_Time >> frame.m_Id
        >> frame.m_Dis >> frame.m_Theta >> frame.m_Phi
        >> frame.m_DisVar >> frame.m_ThetaVar >> frame.m_PhiVar;
    return is;
}

CArchive & operator << (CArchive &ar, ControlDataFrame &frame)
{
    ar << frame.m_Time << frame.m_Id << frame.m_C1;
    return ar;
}

CArchive & operator >> (CArchive &ar, ControlDataFrame &frame)
{
    ar >> frame.m_Time >> frame.m_Id >> frame.m_C1;
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
    ar << packet.m_FusionDatas << packet.m_FilterDatas << packet.m_NoiseDatas << packet.m_PlaneTrueDatas << packet.m_MissileTrueDatas;
    return ar;
}

CArchive & operator >> (CArchive &ar, FusionDataPacket &packet)
{
    ar >> packet.m_FusionDatas >> packet.m_FilterDatas >> packet.m_NoiseDatas >> packet.m_PlaneTrueDatas >> packet.m_MissileTrueDatas;
    return ar;
}

CArchive & operator << (CArchive &ar, ControlDataPacket &packet)
{
    ar << packet.m_FusionData << packet.m_ControlData;
    return ar;
}

CArchive & operator >> (CArchive &ar, ControlDataPacket &packet)
{
    ar >> packet.m_FusionData >> packet.m_ControlData;
    return ar;
}

CArchive & operator << (CArchive &ar, ControlDataAckPacket &packet)
{
    ar << packet.m_PlaneTrueData << packet.m_MissileTrueDatas;
    return ar;
}

CArchive & operator >> (CArchive &ar, ControlDataAckPacket &packet)
{
    ar >> packet.m_PlaneTrueData >> packet.m_MissileTrueDatas;
    return ar;
}

CArchive & operator << (CArchive &ar, GlobalDataPacket &packet)
{
    ar << packet.m_Rounds << packet.m_Interval << packet.m_NoiseType << packet.m_WildValue << packet.m_Delay << packet.m_StartTime << packet.m_EndTime;
    return ar;
}

CArchive & operator >> (CArchive &ar, GlobalDataPacket &packet)
{
    int noiseType = 0;
    ar >> packet.m_Rounds >> packet.m_Interval >> noiseType >> packet.m_WildValue >> packet.m_Delay >> packet.m_StartTime >> packet.m_EndTime;
    packet.m_NoiseType = (NoiseType)noiseType;
    return ar;
}

__declspec(dllexport) GlobalVarFrame **g_GlobalVar = NULL;
