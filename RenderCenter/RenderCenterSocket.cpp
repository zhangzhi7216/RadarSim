#include "StdAfx.h"
#include "RenderCenter.h"
#include "RenderCenterSocket.h"

#include "RenderCenterDlg.h"

RenderCenterSocket::RenderCenterSocket(CRenderCenterDlg *dlg)
: m_Dlg(dlg)
{
}

RenderCenterSocket::~RenderCenterSocket(void)
{
}

void RenderCenterSocket::OnAccept(int nErrorCode)
{
    m_Dlg->SetFusionSocket();
}

void RenderCenterSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    int type;
    ar >> type;
    switch (type)
    {
    default:
        AfxMessageBox(TEXT("未知数据包类型"));
        break;
    }

    ar.Flush();
    AsyncSelect(FD_READ);
    CSocket::OnReceive(nErrorCode);
}

void RenderCenterSocket::OnClose(int nErrorCode)
{
    m_Dlg->ResetFusionSocket();
    CSocket::OnClose(nErrorCode);
}
