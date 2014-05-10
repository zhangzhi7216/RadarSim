#pragma once

#include "NaviAlgo.h"

class __declspec(dllexport) NaviVcAlgo : public NaviAlgo
{
public:
    NaviVcAlgo(void);
    NaviVcAlgo(const CString &name, const CString &dllFileName, const CString &funcName);
    ~NaviVcAlgo(void);

    bool Init();

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    CString m_DllFileName, m_FuncName;
    HMODULE m_DllHandle;
};

