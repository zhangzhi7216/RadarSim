#pragma once

#include "FusionAlgo.h"

#include "MatlabHelper.h"

using namespace MatlabHelper;

class __declspec(dllexport) FusionMatlabAlgo : public FusionAlgo
{
public:
    FusionMatlabAlgo(void);
    FusionMatlabAlgo(const CString &name, const CString &dllFileName, const CString &funcName);
    ~FusionMatlabAlgo(void);

    bool Init();
    bool Run(const FusionInput &input, FusionOutput &output);

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    CString m_DllFileName, m_FuncName;
    HMODULE m_DllHandle;
    RealFunc m_MatlabFunc;
};

