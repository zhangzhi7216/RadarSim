#pragma once

struct __declspec(dllexport) TrueDataFrame
{
    int m_Time;
    Position m_Pos;
    Point3D m_Vel;
    Point3D m_Acc;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, TrueDataFrame &frame);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, TrueDataFrame &frame);

struct __declspec(dllexport) NoiseDataFrame
{
    int m_Time;
    double m_Dis, m_Theta, m_Phi, m_DisVar, m_ThetaVar, m_PhiVar;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, NoiseDataFrame &frame);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, NoiseDataFrame &frame);

struct __declspec(dllexport) TrueDataPacket
{
    TrueDataFrame m_PlaneTrueData;
    vector<TrueDataFrame> m_TargetTrueDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, TrueDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, TrueDataPacket &packet);

struct __declspec(dllexport) NoiseDataPacket
{
    TrueDataFrame m_PlaneTrueData;
    vector<NoiseDataFrame> m_TargetNoiseDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, NoiseDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, NoiseDataPacket &packet);
