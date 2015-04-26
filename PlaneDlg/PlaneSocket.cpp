#include "StdAfx.h"
#include "PlaneSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"
#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"

PlaneSocket::PlaneSocket(CPlaneDlg *dlg)
: m_Dlg(dlg)
{
}

PlaneSocket::~PlaneSocket(void)
{
}

void PlaneSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    int type;
    ar >> type;

    Dispatch(type, ar);

    ar.Flush();
    AsyncSelect(FD_READ);
    CSocket::OnReceive(nErrorCode);
}

void PlaneSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("与数据中心或融合机的连接断开"));
    m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}

void PlaneSocket::Dispatch(int type, CArchive &ar)
{
    switch (type)
    {
    case PacketTypeFusionAddr:
        {
            CString addr;
            int port;
            ar >> addr >> port;
            m_Dlg->ConnectFusion(addr, port);
        }
        break;
    case PacketTypeReset:
        {
            m_Dlg->ResetCtrls();
        }
        break;
    case PacketTypePlane:
        {
            Plane plane;
            ar >> plane;
            ar >> plane.m_Radar;
            ar >> plane.m_Esm;
            ar >> plane.m_Tong;
            ar >> plane.m_Lei;
            m_Dlg->SetPlane(plane);
        }
        break;
    case PacketTypeTarget:
        {
            Target target;
            ar >> target;
            m_Dlg->AddTarget(target);
        }
        break;
    case PacketTypeSensor1:
        {
            Sensor s;
            ar >> s;
            m_Dlg->SetSensor1(s);
        }
        break;
    case PacketTypeSensor2:
        {
            Sensor s;
            ar >> s;
            m_Dlg->SetSensor2(s);
        }
        break;
    case PacketTypeStateMap:
        {
            StateMap stateMap;
            ar >> stateMap;
            m_Dlg->SetStateMap(stateMap);
        }
        break;
    case PacketTypeGlobalData:
        {
            GlobalDataPacket packet;
            ar >> packet;
            m_Dlg->SetGlobalData(packet);
        }
        break;
    case PacketTypeTrueData:
        {
            TrueDataPacket packet;
            ar >> packet;
            m_Dlg->AddTrueData(packet);
            m_Dlg->SendNoiseDatas(packet);
        }
        break;
    default:
        AfxMessageBox(TEXT("未知数据包类型"));
        break;
    }
}

void PlaneSocket::SendNoiseData(NoiseDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeNoiseData << packet;
    ar.Flush();
}

void PlaneSocket::SendImFusion(int port)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImFusion << port;
    ar.Flush();
}

void PlaneSocket::SendImRadar()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImRadar;
    ar.Flush();
}

void PlaneSocket::SendImDetect()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImDetect;
    ar.Flush();
}

void PlaneSocket::SendFusionData(FusionDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeFusionData << packet;
    ar.Flush();
}
