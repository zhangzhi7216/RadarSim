#pragma once

#include "../Sensor/SensorClientProxy.h"

class CPlaneDlg;

class PlaneInfraredProxy :
    public SensorClientProxy
{
public:
    PlaneInfraredProxy(CPlaneDlg &dlg);
    virtual ~PlaneInfraredProxy(void);

    virtual void OnClose();
    virtual void DrawThetaRange();
    virtual void DrawTargets();
    virtual void BlendAll();
    virtual void Invalidate(BOOL bErase = 1);

private:
    CPlaneDlg &m_Dlg;
};
