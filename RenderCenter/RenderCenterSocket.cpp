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
    case PacketTypeKeyTarget:
        {
            TrueDataFrame keyTarget;
            ar >> keyTarget;
            m_Dlg->RenderKeyTarget(keyTarget);
        }
        break;
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

void RenderCenterSocket::SendKeyTarget(CString &path)
{
    HANDLE h = CreateFile(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    int length = GetFileSize(h, NULL);
    char *buf = new char[length];
    DWORD read = 0;
    ReadFile(h, buf, length, &read, NULL);
    CloseHandle(h);

    PacketType type = PacketTypeKeyTarget;

    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar.Write(&type, sizeof(type));
    ar.Write(&length, sizeof(length));
    ar.Write(&buf[0], length);
}
