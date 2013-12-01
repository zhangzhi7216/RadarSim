#include "StdAfx.h"
#include "FusionSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

FusionSocket::FusionSocket(CPlaneDlg *dlg)
: m_Dlg(dlg)
{
}

FusionSocket::~FusionSocket(void)
{
}

void FusionSocket::OnAccept(int nErrorCode)
{
    m_Dlg->AddPlaneSocket();
}

void FusionSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);
}

void FusionSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("与融合机的连接断开"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
}
