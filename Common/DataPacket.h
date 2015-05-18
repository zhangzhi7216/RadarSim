#pragma once

#include <iostream>
#include <fstream>

using namespace std;

struct __declspec(dllexport) TrueDataFrame
{
    int m_Time;
    int m_Id;
    Position m_Pos;
    Point3D m_Vel;
    Point3D m_Acc;
    int m_State;
    int m_Type;
    bool m_IsKeyTarget;
    TrueDataFrame() : m_Time(0), m_Id(0), m_State(0), m_IsKeyTarget(false) {}
    TrueDataFrame &operator += (const TrueDataFrame &frame)
    {
        m_Pos += frame.m_Pos;
        m_Vel += frame.m_Vel;
        m_Acc += frame.m_Acc;
        return *this;
    }
    TrueDataFrame &operator -= (const TrueDataFrame &frame)
    {
        m_Pos -= frame.m_Pos;
        m_Vel -= frame.m_Vel;
        m_Acc -= frame.m_Acc;
        return *this;
    }
    TrueDataFrame &operator *= (double n)
    {
        m_Pos *= n;
        m_Vel *= n;
        m_Acc *= n;
        return *this;
    }
    TrueDataFrame &operator /= (double n)
    {
        m_Pos /= n;
        m_Vel /= n;
        m_Acc /= n;
        return *this;
    }
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, TrueDataFrame &frame);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, TrueDataFrame &frame);

__declspec(dllexport) wofstream & operator << (wofstream &os, TrueDataFrame &frame);
__declspec(dllexport) wifstream & operator >> (wifstream &is, TrueDataFrame &frame);

struct __declspec(dllexport) NoiseDataFrame
{
    int m_Time;
    int m_Id;
    double m_Dis, m_Theta, m_Phi, m_DisVar, m_ThetaVar, m_PhiVar;
    NoiseDataFrame() : m_Time(0), m_Id(0), m_Dis(0), m_Theta(0), m_Phi(0), m_DisVar(0), m_ThetaVar(0), m_PhiVar(0) {}
    NoiseDataFrame &operator += (const NoiseDataFrame &frame)
    {
        m_Dis += frame.m_Dis;
        m_Theta += frame.m_Theta;
        m_Phi += frame.m_Phi;
        m_DisVar += frame.m_DisVar;
        m_ThetaVar += frame.m_ThetaVar;
        m_PhiVar += frame.m_PhiVar;
        return *this;
    }
    NoiseDataFrame &operator -= (const NoiseDataFrame &frame)
    {
        m_Dis -= frame.m_Dis;
        m_Theta -= frame.m_Theta;
        m_Phi -= frame.m_Phi;
        m_DisVar -= frame.m_DisVar;
        m_ThetaVar -= frame.m_ThetaVar;
        m_PhiVar -= frame.m_PhiVar;
        return *this;
    }
    NoiseDataFrame &operator *= (double n)
    {
        m_Dis *= n;
        m_Theta *= n;
        m_Phi *= n;
        m_DisVar *= n;
        m_ThetaVar *= n;
        m_PhiVar *= n;
        return *this;
    }
    NoiseDataFrame &operator /= (double n)
    {
        m_Dis /= n;
        m_Theta /= n;
        m_Phi /= n;
        m_DisVar /= n;
        m_ThetaVar /= n;
        m_PhiVar /= n;
        return *this;
    }
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, NoiseDataFrame &frame);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, NoiseDataFrame &frame);

__declspec(dllexport) wofstream & operator << (wofstream &os, NoiseDataFrame &frame);
__declspec(dllexport) wifstream & operator >> (wifstream &is, NoiseDataFrame &frame);

struct __declspec(dllexport) ControlDataFrame
{
    int m_Time;
    int m_Id;
    double m_C1;
    ControlDataFrame() : m_Time(0), m_Id(0), m_C1(1) {}
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, ControlDataFrame &frame);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, ControlDataFrame &frame);

template <class T>
__declspec(dllexport) CArchive & operator << (CArchive &ar, vector<T> &v)
{
    ar << v.size();
    for (int i = 0; i < v.size(); ++i)
    {
        ar << v[i];
    }
    return ar;
}
template <class T>
__declspec(dllexport) CArchive & operator >> (CArchive &ar, vector<T> &v)
{
    int n = 0;
    ar >> n;
    for (int i = 0; i < n; ++i)
    {
        T t;
        ar >> t;
        v.push_back(t);
    }
    return ar;
}

template <class T>
__declspec(dllexport) wofstream & operator << (wofstream &ar, vector<T> &v)
{
    ar << v.size() << endl;
    for (int i = 0; i < v.size(); ++i)
    {
        ar << v[i] << TEXT(" ");
    }
    return ar;
}
template <class T>
__declspec(dllexport) wifstream & operator >> (wifstream &ar, vector<T> &v)
{
    int n = 0;
    ar >> n;
    for (int i = 0; i < n; ++i)
    {
        T t;
        ar >> t;
        v.push_back(t);
    }
    return ar;
}

struct __declspec(dllexport) TrueDataPacket
{
    TrueDataFrame m_PlaneTrueData;
    vector<TrueDataFrame> m_TargetTrueDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, TrueDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, TrueDataPacket &packet);

struct __declspec(dllexport) NoiseDataPacket
{
    int m_SensorId;
    vector<NoiseDataFrame> m_TargetNoiseDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, NoiseDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, NoiseDataPacket &packet);

struct __declspec(dllexport) FusionDataPacket
{
    vector<TrueDataFrame> m_FusionDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, FusionDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, FusionDataPacket &packet);

struct __declspec(dllexport) ControlDataPacket
{
    FusionDataPacket m_FusionData;
    ControlDataFrame m_ControlData;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, ControlDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, ControlDataPacket &packet);

struct __declspec(dllexport) ControlDataAckPacket
{
    TrueDataFrame m_PlaneTrueData;
    vector<TrueDataFrame> m_MissileTrueDatas;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, ControlDataAckPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, ControlDataAckPacket &packet);

struct __declspec(dllexport) GlobalDataPacket
{
    int m_Rounds;
    int m_Interval;
    NoiseType m_NoiseType;
    BOOL m_WildValue;
    BOOL m_Delay;
    int m_StartTime;
    int m_EndTime;
    GlobalDataPacket()
        : m_Rounds(10)
        , m_Interval(20)
        , m_NoiseType(NoiseTypeWhite)
        , m_WildValue(false)
        , m_Delay(false)
        , m_StartTime(0)
        , m_EndTime(600)
    {}
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, GlobalDataPacket &packet);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, GlobalDataPacket &packet);

struct __declspec(dllexport) GlobalVarFrame
{
    double m_G[GLOBAL_VAR_FRAME_SIZE];
};

extern __declspec(dllexport) GlobalVarFrame *g_GlobalVar;
