#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) NaviInput
{
    vector<NoiseDataFrame> m_FusionDatas;
    vector<NoiseDataFrame> m_FilterDatas;
    ControlDataFrame m_ControlData;
    Position m_Position;
};

struct __declspec(dllexport) NaviOutput
{
    Position m_Position;
};

class __declspec(dllexport) NaviAlgo : public GeneralAlgo<NaviInput, NaviOutput>
{
public:
    NaviAlgo();
    NaviAlgo(const CString &name);
};
