#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) FusionInput
{
    vector<NoiseDataPacket> m_NoiseDataPackets;
    int m_Interval;
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
