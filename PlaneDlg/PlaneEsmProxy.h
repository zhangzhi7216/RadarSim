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
    virtual void DrawTargets();
    virtual void BlendAll();
    virtual void Invalidate(BOOL bErase = 1);
    virtual BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL,
        CRgn* prgnUpdate = NULL,
        UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

private:
    CPlaneDlg &m_Dlg;
};
