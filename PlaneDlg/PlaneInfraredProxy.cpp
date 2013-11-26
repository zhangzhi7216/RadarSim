#include "StdAfx.h"
#include "PlaneInfraredProxy.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneInfraredProxy::PlaneInfraredProxy(CPlaneDlg &dlg)
: m_Dlg(dlg)
{
}

PlaneInfraredProxy::~PlaneInfraredProxy(void)
{
}

void PlaneInfraredProxy::OnClose()
{
    m_Dlg.OnStnDblclickInfraredCtrl();
}

void PlaneInfraredProxy::DrawThetaRange()
{
    m_Dlg.m_InfraredCtrl.DrawThetaRange();
}

void PlaneInfraredProxy::DrawTargets()
{
    m_Dlg.m_InfraredCtrl.DrawTargets();
}

void PlaneInfraredProxy::BlendAll()
{
    m_Dlg.m_InfraredCtrl.BlendAll();
}

void PlaneInfraredProxy::Invalidate(BOOL bErase)
{
    m_Dlg.m_InfraredCtrl.Invalidate(bErase);
}
