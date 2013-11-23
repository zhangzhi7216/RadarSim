#include "StdAfx.h"
#include "PlaneDataListProxy.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneDataListProxy::PlaneDataListProxy(CPlaneDlg &dlg)
: m_Dlg(dlg)
{
}

PlaneDataListProxy::~PlaneDataListProxy(void)
{
}

void PlaneDataListProxy::OnClose()
{
    m_Dlg.OnNMDblclkDatalistCtrl(NULL, NULL);
}

void PlaneDataListProxy::Invalidate(BOOL bErase)
{
    m_Dlg.m_DataListCtrl.Invalidate(bErase);
}
