#include "StdAfx.h"
#include "PlaneSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneSocket::PlaneSocket(CPlaneDlg *dlg)
: m_Dlg(dlg)
{
}

PlaneSocket::~PlaneSocket(void)
{
}

void PlaneSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    ar.Flush();
    CSocket::OnReceive(nErrorCode);
}

void PlaneSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("与飞机的连接断开"));
    m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}
