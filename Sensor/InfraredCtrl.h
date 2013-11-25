#pragma once
#include "SensorCtrl.h"

class __declspec(dllexport) CInfraredCtrl :
    public CSensorCtrl
{
public:
    CInfraredCtrl(Sensor &sensor);
    virtual ~CInfraredCtrl(void);

public:
    virtual void DrawBackground();
    virtual void DrawTargets();
    virtual void BlendAll();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    virtual void PreSubclassWindow();
};

