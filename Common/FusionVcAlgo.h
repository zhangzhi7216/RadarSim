#pragma once

#include "FusionAlgo.h"

class __declspec(dllexport) FusionVcAlgo : public FusionAlgo
{
public:
    FusionVcAlgo(void);
    FusionVcAlgo(const CString &name, const CString &dllFileName, const CString &funcName);
    ~FusionVcAlgo(void);

    bool Init();

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    CString m_DllFileName, m_FuncName;
    HMODULE m_DllHandle;
};

