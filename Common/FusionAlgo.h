#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) FusionOutput
{
    FusionDataPacket m_FusionData;
    vector<ControlDataPacket> m_ControlDatas;
};

class __declspec(dllexport) FusionAlgo : public GeneralAlgo<vector<NoiseDataPacket>, FusionOutput>
{
public:
    FusionAlgo();
    FusionAlgo(const CString &name);
};
