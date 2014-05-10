#pragma once

#include "NaviAlgo.h"

#include "MatlabHelper.h"

using namespace MatlabHelper;

class __declspec(dllexport) NaviMatlabAlgo : public NaviAlgo
{
public:
    NaviMatlabAlgo(void);
    NaviMatlabAlgo(const CString &name, const CString &dllFileName, const CString &funcName);
    ~NaviMatlabAlgo(void);

    bool Init();
    bool Run(const NaviInput &input, NaviOutput &output);

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    CString m_DllFileName, m_FuncName;
    HMODULE m_DllHandle;
    RealFunc m_MatlabFunc;
};

