#pragma once

#include "GeneralAlgo.h"
#include <map>

using namespace std;

struct __declspec(dllexport) FusionInput
{
    map<SensorId, NoiseDataPacket> m_NoiseDataPackets;
    int m_Interval;
    double m_InfraredMaxDis;
};

struct __declspec(dllexport) FusionOutput
{
    FusionDataPacket m_FusionData;
    vector<ControlDataPacket> m_ControlDatas;
};

class __declspec(dllexport) FusionAlgo : public GeneralAlgo<FusionInput, FusionOutput>
{
public:
    FusionAlgo();
    FusionAlgo(const CString &name);
};
