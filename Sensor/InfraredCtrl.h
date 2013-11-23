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
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual void PreSubclassWindow();

public:
    void Reset();
    void AddTarget(Target &target);
};

