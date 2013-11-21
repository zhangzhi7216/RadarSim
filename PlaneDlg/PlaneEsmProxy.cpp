#include "StdAfx.h"
#include "PlaneEsmProxy.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneEsmProxy::PlaneEsmProxy(CPlaneDlg &dlg)
: m_Dlg(dlg)
{
}

PlaneEsmProxy::~PlaneEsmProxy(void)
{
}

void PlaneEsmProxy::OnClose()
{
    m_Dlg.OnStnDblclickEsmCtrl();
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

BOOL PlaneEsmProxy::RedrawWindow(LPCRECT lpRectUpdate,
                                   CRgn* prgnUpdate,
                                   UINT flags)
{
    return m_Dlg.m_EsmCtrl.RedrawWindow(lpRectUpdate, prgnUpdate, flags);
}
