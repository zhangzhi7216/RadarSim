#pragma once

#include "Target.h"
#include <afxmt.h>

#include "MatlabHelper.h"

class __declspec(dllexport) COneTimeMatlabDlg
{
public:
    COneTimeMatlabDlg();
    virtual ~COneTimeMatlabDlg(void);

    void Run(int round);
    void Stop();

    Engine *m_Engine;
};

