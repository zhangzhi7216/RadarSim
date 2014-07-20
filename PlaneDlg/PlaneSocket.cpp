#include "StdAfx.h"
#include "PlaneSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

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
    switch (type)
    {
    case PacketTypeNoiseData:
        {
            NoiseDataPacket packet;
            ar >> packet;
            m_Dlg->AddNoiseData(make_pair(this, packet));
        }
        break;
    case PacketTypeControlData:
        {
            ControlDataPacket packet;
            ar >> packet;
            m_Dlg->AddControlData(packet);
        }
        break;
    case PacketTypeControlDataAck:
        {
            TrueDataFrame frame;
            ar >> frame;
            m_Dlg->AddControlDataAck(frame);
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
    AfxMessageBox(TEXT("与飞机的连接断开"));
    m_Dlg->ResetSockets();
    CSocket::OnClose(nErrorCode);
}

void PlaneSocket::SendControlData(ControlDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeControlData << packet;
    ar.Flush();
}
