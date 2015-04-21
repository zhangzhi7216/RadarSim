#include "StdAfx.h"
#include "DataCenterSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"
#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"
#include "FusionMatlabAlgo.h"
#include "NaviLocalAlgo.h"
#include "NaviVcAlgo.h"
#include "NaviMatlabAlgo.h"

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
    case PacketTypeReset:
        {
            m_Dlg->ResetCtrls();
        }
        break;
    case PacketTypePlane:
        {
            Plane plane;
            ar >> plane;
            m_Dlg->SetPlane(plane);
        }
        break;
    case PacketTypeOtherPlane:
        {
            Plane plane;
            ar >> plane;
            m_Dlg->AddOtherPlane(plane);
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
            Plane foo;
            GlobalDataPacket bar;
            Sensor s(Sensor::SensorTypeNonSource, foo, bar);
            ar >> s;
            m_Dlg->SetSensor1(s);
        }
        break;
    case PacketTypeSensor2:
        {
            Plane foo;
            GlobalDataPacket bar;
            Sensor s(Sensor::SensorTypeNonSource, foo, bar);
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
    case PacketTypeFusionAlgo:
        {
            int type;
            ar >> type;
            switch (type)
            {
            case FusionAlgoTypeLocal:
                {
                    FusionAlgo *algo = new FusionLocalAlgo;
                    ar >> *algo;
                    m_Dlg->SetFusionAlgo(algo);
                }
                break;
            case FusionAlgoTypeVc:
                {
                    FusionAlgo *algo = new FusionVcAlgo;
                    ar >> *algo;
                    m_Dlg->SetFusionAlgo(algo);
                }
                break;
            case FusionAlgoTypeMatlab:
                {
                    FusionAlgo *algo = new FusionMatlabAlgo;
                    ar >> *algo;
                    m_Dlg->SetFusionAlgo(algo);
                }
                break;
            default:
                CString msg;
                msg.AppendFormat(TEXT("未知融合算法类型%d."), type);
                AfxMessageBox(msg);
                break;
            }
        }
        break;
    case PacketTypeNaviAlgo:
        {
            int type;
            ar >> type;
            switch (type)
            {
            case NaviAlgoTypeLocal:
                {
                    NaviAlgo *algo = new NaviLocalAlgo;
                    ar >> *algo;
                    m_Dlg->SetNaviAlgo(algo);
                }
                break;
            case NaviAlgoTypeVc:
                {
                    NaviAlgo *algo = new NaviVcAlgo;
                    ar >> *algo;
                    m_Dlg->SetNaviAlgo(algo);
                }
                break;
            case NaviAlgoTypeMatlab:
                {
                    NaviAlgo *algo = new NaviMatlabAlgo;
                    ar >> *algo;
                    m_Dlg->SetNaviAlgo(algo);
                }
                break;
            default:
                CString msg;
                msg.AppendFormat(TEXT("未知导航算法类型%d."), type);
                AfxMessageBox(msg);
                break;
            }
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
            NoiseDataPacket noisePacket;
            m_Dlg->PackNoiseData(packet, noisePacket);
            m_Dlg->SendNoiseData(noisePacket);
        }
        break;
    case PacketTypeOtherTrueData:
        {
            int i;
            TrueDataFrame frame;
            ar >> i >> frame;
            m_Dlg->AddOtherTrueData(i, frame);
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

void DataCenterSocket::SendImFusion(int port)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImFusion << port;
    ar.Flush();
}

void DataCenterSocket::SendImRadar()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImRadar;
    ar.Flush();
}

void DataCenterSocket::SendImDetect()
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeImDetect;
    ar.Flush();
}

void DataCenterSocket::SendFusionData(FusionDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << PacketTypeFusionData << packet;
    ar.Flush();
}
