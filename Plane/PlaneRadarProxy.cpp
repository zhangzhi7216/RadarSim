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

void PlaneRadarProxy::SetEnable(BOOL enable)
{
}

void PlaneRadarProxy::SetScanLineAngle(DOUBLE scanLineAngle)
{
}
