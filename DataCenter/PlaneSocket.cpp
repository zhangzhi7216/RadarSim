#include "StdAfx.h"
#include "PlaneSocket.h"
#include "DataCenter.h"

#include "DataCenterDlg.h"

PlaneSocket::PlaneSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
, m_IsFusion(false)
, m_IsAttack(false)
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

void PlaneSocket::SendReset()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeReset;
    ar.Flush();
}

void PlaneSocket::SendPlane(Plane &plane)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypePlane << plane;
    ar.Flush();
}

void PlaneSocket::SendOtherPlane(Plane &plane)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeOtherPlane << plane;
    ar.Flush();
}

void PlaneSocket::SendTarget(Target &target)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTarget << target;
    ar.Flush();
}

void PlaneSocket::SendRadar(Sensor &radar)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeRadar << radar;
    ar.Flush();
}

void PlaneSocket::SendEsm(Sensor &esm)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeEsm << esm;
    ar.Flush();
}

void PlaneSocket::SendInfrared(Sensor &infrared)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeInfrared << infrared;
    ar.Flush();
}

void PlaneSocket::SendStateMap(StateMap &stateMap)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeStateMap << stateMap;
    ar.Flush();
}

void PlaneSocket::SendFusionAlgo(FusionAlgo *algo)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeFusionAlgo << *algo;
    ar.Flush();
}

void PlaneSocket::SendNaviAlgo(NaviAlgo *algo)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeNaviAlgo << *algo;
    ar.Flush();
}

void PlaneSocket::SendGlobalData(GlobalDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeGlobalData << packet;
    ar.Flush();
}

void PlaneSocket::SendTrueData(TrueDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTrueData << packet;
    ar.Flush();
}

void PlaneSocket::SendOtherTrueData(int i, TrueDataFrame &frame)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeOtherTrueData << i << frame;
    ar.Flush();
}

bool PlaneSocket::IsFusion()
{
    return m_IsFusion;
}

bool PlaneSocket::IsAttack()
{
    return m_IsAttack;
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
            m_IsFusion = true;
            m_Dlg->SetFusionAddr(addr, port);
        }
        break;
    case PacketTypeImAttack:
        {
            m_IsAttack = true;
        }
        break;
    case PacketTypeFusionData:
        {
            FusionDataPacket packet;
            ar >> packet;
            m_Dlg->AddFusionData(packet);
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

void PlaneSocket::OnClose(int nErrorCode)
{
    m_IsFusion = false;
    m_FusionAddrSent = false;
    m_Dlg->ResetCtrls();
    m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}
