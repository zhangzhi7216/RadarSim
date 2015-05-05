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

void RenderCenterSocket::SendKeyTarget()
{
    HANDLE file = CreateFile(
        KEY_TARGET_FILE_NAME,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    int length = GetFileSize(file, NULL);
    PacketType type = PacketTypeKeyTarget;
    Send(&type, sizeof(type));
    // Send(&length, 4);
    // TransmitFile(
    //     this->m_hSocket,
    //     file,
    //     0,
    //     0,
    //     NULL,
    //     NULL,
    //     TF_USE_KERNEL_APC
    //     );
    CloseHandle(file);
}
