#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) NaviInput
{
    vector<TrueDataFrame> m_FusionDatas;
    vector<TrueDataFrame> m_FilterDatas;
    ControlDataFrame m_ControlData;
    GlobalDataPacket m_GlobalData;
    Plane m_Plane;
};

struct __declspec(dllexport) NaviOutput
{
    TrueDataFrame m_TrueData;
};

class __declspec(dllexport) NaviAlgo : public GeneralAlgo<NaviInput, NaviOutput>
{
public:
    NaviAlgo();
    NaviAlgo(const CString &name);
};
