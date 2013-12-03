#include "StdAfx.h"
#include "DataCenterSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"
#include "DataPacket.h"

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
    case PacketTypeTarget:
        {
            Target target;
            ar >> target;
            m_Dlg->AddTarget(target);
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
            StateMap stateMap;
            ar >> stateMap;
            m_Dlg->SetStateMap(stateMap);
        }
        break;
    case PacketTypeTrueData:
        {
            TrueDataPacket packet;
            ar >> packet;
            m_Dlg->AddTrueData(packet);
            NoiseDataPacket noisePacket;
            m_Dlg->PackNoiseData(packet, noisePacket);
            m_Dlg->SendNoiseData(noisePacket);
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
    AfxMessageBox(TEXT("与数据中心的连接断开"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();

    CSocket::OnClose(nErrorCode);
}

void DataCenterSocket::SendFusionAddr(int port)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImFusion << port;
    ar.Flush();
}

void DataCenterSocket::SendFusionData()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeFusionData;
    ar.Flush();
}
