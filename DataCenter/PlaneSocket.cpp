#include "StdAfx.h"
#include "PlaneSocket.h"
#include "DataCenter.h"

#include "DataCenterDlg.h"

PlaneSocket::PlaneSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
, m_IsFusion(false)
, m_FusionAddrSent(false)
{
}

PlaneSocket::~PlaneSocket(void)
{
}

void PlaneSocket::SendFusionAddr(const CString &addr, int port)
{
    if (!m_IsFusion)
    {
        if (!m_FusionAddrSent)
        {
            CSocketFile file(this);
            CArchive ar(&file, CArchive::store);
            ar << PacketTypeFusionAddr << addr << port;
            ar.Flush();
            m_FusionAddrSent = true;
        }
    }
}

void PlaneSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    int type;
    ar >> type;
    switch (type)
    {
    case PacketTypeImFusion:
        {
            CString addr;
            UINT port;
            GetPeerName(addr, port);
            ar >> port;
            ar.Flush();
            m_IsFusion = true;
            m_Dlg->SetFusionAddr(addr, port);
        }
        break;
    default:
        // AfxMessageBox(TEXT("未知数据包类型"));
        break;
    }
}

void PlaneSocket::OnClose(int nErrorCode)
{
    m_IsFusion = false;
    m_FusionAddrSent = false;
    m_Dlg->ResetSockets();
}

void PlaneSocket::SendPlaneData()
{
}
