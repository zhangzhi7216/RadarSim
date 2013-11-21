#include "StdAfx.h"
#include "PlaneRadarProxy.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneRadarProxy::PlaneRadarProxy(CPlaneDlg &dlg)
: m_Dlg(dlg)
{
}

PlaneRadarProxy::~PlaneRadarProxy(void)
{
}

void PlaneRadarProxy::OnClose()
{
    m_Dlg.OnStnDblclickRadarCtrl();
}

void PlaneRadarProxy::DrawTargets()
{
    m_Dlg.m_RadarCtrl.DrawTargets();
}

void PlaneRadarProxy::BlendAll()
{
    m_Dlg.m_RadarCtrl.BlendAll();
}

void PlaneRadarProxy::Invalidate(BOOL bErase)
{
    m_Dlg.m_RadarCtrl.Invalidate(bErase);
}

BOOL PlaneRadarProxy::RedrawWindow(LPCRECT lpRectUpdate,
                                   CRgn* prgnUpdate,
                                   UINT flags)
{
    return m_Dlg.m_RadarCtrl.RedrawWindow(lpRectUpdate, prgnUpdate, flags);
}
