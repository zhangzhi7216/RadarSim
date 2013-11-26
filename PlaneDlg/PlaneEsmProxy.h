#pragma once

#include "../Sensor/SensorClientProxy.h"

class CPlaneDlg;

class PlaneEsmProxy :
    public SensorClientProxy
{
public:
    PlaneEsmProxy(CPlaneDlg &dlg);
    virtual ~PlaneEsmProxy(void);

    virtual void OnClose();
    virtual void DrawThetaRange();
    virtual void DrawTargets();
    virtual void BlendAll();
    virtual void Invalidate(BOOL bErase = 1);

private:
    CPlaneDlg &m_Dlg;
};
