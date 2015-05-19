#include "StdAfx.h"
#include "DataCenter.h"
#include "DataCenterSocket.h"

#include "DataCenterDlg.h"

DataCenterSocket::DataCenterSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
, m_IsFusion(false)
, m_IsRadar(false)
, m_IsDetect(false)
, m_FusionAddrSent(false)
{
}

DataCenterSocket::~DataCenterSocket(void)
{
}

void DataCenterSocket::OnAccept(int nErrorCode)
{
    m_Dlg->AddPlaneSocket();
}

void DataCenterSocket::OnReceive(int nErrorCode)
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
    case PacketTypeFusionData:
        {
            FusionDataPacket packet;
            ar >> packet;
            m_Dlg->AddFusionData(packet);
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

void DataCenterSocket::OnClose(int nErrorCode)
{
    m_IsFusion = false;
    m_FusionAddrSent = false;
    m_Dlg->ResetCtrls();
    m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}

void DataCenterSocket::SendFusionAddr(const CString &addr, int port)
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

void DataCenterSocket::SendReset()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeReset;
    ar.Flush();
}

void DataCenterSocket::SendPlane(Plane &plane)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypePlane << plane << plane.m_Radar << plane.m_Ais << plane.m_Tong << plane.m_Lei;
    ar.Flush();
}

void DataCenterSocket::SendTarget(Target &target)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTarget << target;
    ar.Flush();
}

void DataCenterSocket::SendRadar(Sensor &radar)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor1 << radar;
    ar.Flush();
}

void DataCenterSocket::SendAis(Sensor &ais)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor2 << ais;
    ar.Flush();
}

void DataCenterSocket::SendTong(Sensor &tong)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor1 << tong;
    ar.Flush();
}

void DataCenterSocket::SendLei(Sensor &lei)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeSensor2 << lei;
    ar.Flush();
}

void DataCenterSocket::SendStateMap(StateMap &stateMap)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeStateMap << stateMap;
    ar.Flush();
}

void DataCenterSocket::SendFusionAlgo(FusionAlgo *algo)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeFusionAlgo << *algo;
    ar.Flush();
}

void DataCenterSocket::SendGlobalData(GlobalDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeGlobalData << packet;
    ar.Flush();
}

void DataCenterSocket::SendTrueData(TrueDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTrueData << packet;
    ar.Flush();
}

bool DataCenterSocket::IsFusion()
{
    return m_IsFusion;
}

bool DataCenterSocket::IsRadar()
{
    return m_IsRadar;
}

bool DataCenterSocket::IsDetect()
{
    return m_IsDetect;
}
