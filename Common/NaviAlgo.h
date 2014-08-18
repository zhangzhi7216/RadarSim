#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) NaviInput
{
    TrueDataFrame m_PlaneTrueData;
    vector<TrueDataFrame> m_MissileTrueDatas;
    vector<TrueDataFrame> m_FusionDatas;
    vector<TrueDataFrame> m_FilterDatas;
    ControlDataFrame m_ControlData;
    int m_Interval;
};

struct __declspec(dllexport) NaviOutput
{
    TrueDataFrame m_PlaneTrueData;
    vector<TrueDataFrame> m_MissileTrueDatas;
};

class __declspec(dllexport) NaviAlgo : public GeneralAlgo<NaviInput, NaviOutput>
{
public:
    NaviAlgo();
    NaviAlgo(const CString &name);
};
