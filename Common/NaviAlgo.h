#pragma once

#include "GeneralAlgo.h"

struct __declspec(dllexport) NaviOutput
{
};

class __declspec(dllexport) NaviAlgo : public GeneralAlgo<vector<TrueDataPacket>, NaviOutput>
{
public:
    NaviAlgo(const CString &name);
};
