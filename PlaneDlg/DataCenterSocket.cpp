#include "StdAfx.h"
#include "DataCenterSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

DataCenterSocket::DataCenterSocket(CPlaneDlg *dlg)
: m_Dlg(dlg)
{
}

DataCenterSocket::~DataCenterSocket(void)
{
}

void DataCenterSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    int type;
    ar >> type;
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
    case PacketTypePlane:
        {
            Plane plane;
            ar >> plane;
            m_Dlg->SetPlane(plane);
        }
        break;
    case PacketTypeRadar:
        {
            Sensor radar(Sensor::SensorTypeNonSource);
            ar >> radar;
            m_Dlg->SetRadar(radar);
        }
        break;
    case PacketTypeEsm:
        {
            Sensor esm(Sensor::SensorTypeNonSource);
            ar >> esm;
            m_Dlg->SetEsm(esm);
        }
        break;
    case PacketTypeInfrared:
        {
            Sensor infrared(Sensor::SensorTypeNonSource);
            ar >> infrared;
            m_Dlg->SetInfrared(infrared);
        }
        break;
    case PacketTypeStateMap:
        {
            Sensor foo(Sensor::SensorTypeNonSource);
            StateMap stateMap(foo, foo, foo);
            ar >> stateMap;
            m_Dlg->SetStateMap(stateMap);
        }
        break;
    default:
        AfxMessageBox(TEXT("未知数据包类型"));
        break;
    }
    ar.Flush();
}

void DataCenterSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("与数据中心的连接断开"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
}

void DataCenterSocket::SendFusionAddr(int port)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImFusion << port;
    ar.Flush();
}
