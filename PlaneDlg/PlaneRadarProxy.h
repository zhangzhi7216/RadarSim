#pragma once

#include "../Common/SensorClientProxy.h"

class CPlaneDlg;

class PlaneRadarProxy :
    public SensorClientProxy
{
public:
    PlaneRadarProxy(CPlaneDlg &dlg);
    virtual ~PlaneRadarProxy(void);

    virtual void OnClose();
    virtual void DrawThetaRange();
    virtual void DrawTargets();
    virtual void BlendAll();
    virtual void Invalidate(BOOL bErase = 1);

private:
    CPlaneDlg &m_Dlg;
};
