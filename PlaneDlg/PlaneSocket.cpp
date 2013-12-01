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
}

void PlaneSocket::OnClose(int nErrorCode)
{
    static bool closing = false;
    if (!closing)
    {
        closing = true;
        AfxMessageBox(TEXT("与飞机的连接断开"));
        m_Dlg->ResetSockets();
    }
}
