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
    virtual void DrawTargets();
    virtual void BlendAll();
    virtual void Invalidate(BOOL bErase = 1);
    virtual BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL,
        CRgn* prgnUpdate = NULL,
        UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

private:
    CPlaneDlg &m_Dlg;
};
