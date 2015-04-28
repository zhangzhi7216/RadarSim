#include "StdAfx.h"
#include "RenderCenter.h"
#include "RenderCenterSocket.h"

#include "RenderCenterDlg.h"

RenderCenterSocket::RenderCenterSocket(CRenderCenterDlg *dlg)
: m_Dlg(dlg)
, m_IsFusion(false)
, m_IsRadar(false)
, m_IsDetect(false)
, m_FusionAddrSent(false)
{
}

RenderCenterSocket::~RenderCenterSocket(void)
{
}

void RenderCenterSocket::OnAccept(int nErrorCode)
{
    // m_Dlg->AddPlaneSocket();
}

void RenderCenterSocket::OnReceive(int nErrorCode)
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
            m_IsFusion = true;
            // m_Dlg->SetFusionAddr(addr, port);
        }
        break;
    case PacketTypeFusionData:
        {
            FusionDataPacket packet;
            ar >> packet;
            // m_Dlg->AddFusionData(packet);
        }
        break;
    case PacketTypeImRadar:
        {
            m_IsRadar = true;
        }
        break;
    case PacketTypeImDetect:
        {
            m_IsDetect = true;
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
    m_IsFusion = false;
    m_FusionAddrSent = false;
    // m_Dlg->ResetCtrls();
    // m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}

void RenderCenterSocket::SendFusionAddr(const CString &addr, int port)
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

void RenderCenterSocket::SendReset()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeReset;
    ar.Flush();
}

void RenderCenterSocket::SendPlane(Plane &plane)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypePlane << plane << plane.m_Radar << plane.m_Esm << plane.m_Tong << plane.m_Lei;
    ar.Flush();
}

void RenderCenterSocket::SendTarget(Target &target)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTarget << target;
    ar.Flush();
}

void RenderCenterSocket::SendRadar(Sensor &radar)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor1 << radar;
    ar.Flush();
}

void RenderCenterSocket::SendEsm(Sensor &esm)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor2 << esm;
    ar.Flush();
}

void RenderCenterSocket::SendTong(Sensor &tong)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor1 << tong;
    ar.Flush();
}

void RenderCenterSocket::SendLei(Sensor &lei)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor2 << lei;
    ar.Flush();
}

void RenderCenterSocket::SendStateMap(StateMap &stateMap)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeStateMap << stateMap;
    ar.Flush();
}

void RenderCenterSocket::SendFusionAlgo(FusionAlgo *algo)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeFusionAlgo << *algo;
    ar.Flush();
}

void RenderCenterSocket::SendGlobalData(GlobalDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeGlobalData << packet;
    ar.Flush();
}

void RenderCenterSocket::SendTrueData(TrueDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTrueData << packet;
    ar.Flush();
}

bool RenderCenterSocket::IsFusion()
{
    return m_IsFusion;
}

bool RenderCenterSocket::IsRadar()
{
    return m_IsRadar;
}

bool RenderCenterSocket::IsDetect()
{
    return m_IsDetect;
}
