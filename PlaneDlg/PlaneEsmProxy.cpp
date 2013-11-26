#include "StdAfx.h"
#include "PlaneEsmProxy.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneEsmProxy::PlaneEsmProxy(CPlaneDlg &dlg)
: SensorClientProxy(dlg.m_StateMapDlg.m_Ctrl)
, m_Dlg(dlg)
{
}

PlaneEsmProxy::~PlaneEsmProxy(void)
{
}

void PlaneEsmProxy::OnClose()
{
    m_Dlg.OnStnDblclickEsmCtrl();
}

void PlaneEsmProxy::DrawThetaRange()
{
    m_Dlg.m_EsmCtrl.DrawThetaRange();
}

void PlaneEsmProxy::DrawTargets()
{
    m_Dlg.m_EsmCtrl.DrawTargets();
}

void PlaneEsmProxy::BlendAll()
{
    m_Dlg.m_EsmCtrl.BlendAll();
}

void PlaneEsmProxy::Invalidate(BOOL bErase)
{
    m_Dlg.m_EsmCtrl.Invalidate(bErase);
}
