#include "StdAfx.h"
#include "DataCenter.h"

#include "ServerSocket.h"

#include "DataCenterDlg.h"

ServerSocket::ServerSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
{
}

ServerSocket::~ServerSocket(void)
{
}

void ServerSocket::OnAccept(int nErrorCode)
{
    m_Dlg->OnAccept();
    CSocket::OnAccept(nErrorCode);
}
