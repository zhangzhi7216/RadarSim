#pragma once

#include "GeneralAlgo.h"
#include <map>

using namespace std;

struct __declspec(dllexport) FusionInput
{
    map<SensorId, NoiseDataPacket> m_NoiseDataPackets;

    int m_Interval;

    double m_RadarScopeMax;
    double m_RadarScopeVar;
    double m_RadarThetaMax;
    double m_RadarThetaVar;
    double m_RadarPhiMax;
    double m_RadarPhiVar;

    double m_AisXMax;
    double m_AisXVar;
    double m_AisYMax;
    double m_AisYVar;
    double m_AisZMax;
    double m_AisZVar;

    double m_TongScopeMax;
    double m_TongScopeVar;
    double m_TongThetaMax;
    double m_TongThetaVar;
    double m_TongPhiMax;
    double m_TongPhiVar;

    double m_LeiScopeMax;
    double m_LeiScopeVar;
    double m_LeiThetaMax;
    double m_LeiThetaVar;
    double m_LeiPhiMax;
    double m_LeiPhiVar;
};

struct __declspec(dllexport) FusionOutput
{
    FusionDataPacket m_FusionDataPacket;
};

class __declspec(dllexport) FusionAlgo : public GeneralAlgo<FusionInput, FusionOutput>
{
public:
    FusionAlgo();
    FusionAlgo(const CString &name);
};
