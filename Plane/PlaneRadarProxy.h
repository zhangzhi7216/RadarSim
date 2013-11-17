#pragma once

#include "../Radar/RadarClientProxy.h"

class CPlaneDlg;

class PlaneRadarProxy :
    public RadarClientProxy
{
public:
    PlaneRadarProxy(CPlaneDlg &dlg);
    virtual ~PlaneRadarProxy(void);

    virtual void OnClose();
    virtual void SetEnable(BOOL enable);
    virtual void SetScanLineAngle(DOUBLE scanLineAngle);

private:
    CPlaneDlg &m_Dlg;
};
