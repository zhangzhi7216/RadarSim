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

void PlaneSocket::SendPlane(Plane &plane)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypePlane << plane;
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

void PlaneSocket::SendTrueData(int plane, int index)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeTrueData << m_Dlg->m_PlaneDatas[plane][index];
    for (int i = 0; i < TARGET_COUNT; ++i)
    {
        ar << m_Dlg->m_TargetDatas[i][index];
    }
    ar.Flush();
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
    default:
        // AfxMessageBox(TEXT("未知数据包类型"));
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
