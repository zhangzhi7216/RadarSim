#include "StdAfx.h"
#include "FusionSocket.h"
#include "Resource.h"
#include "FusionPlaneDlg.h"
#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"

FusionSocket::FusionSocket(CFusionPlaneDlg *dlg)
: PlaneSocket(dlg)
, m_Dlg(dlg)
{
}

FusionSocket::~FusionSocket(void)
{
}

void FusionSocket::OnAccept(int nErrorCode)
{
    m_Dlg->AddPlaneSocket();
    CSocket::OnAccept(nErrorCode);
}

void FusionSocket::Dispatch(int type, CArchive &ar)
{
    switch (type)
    {
    case PacketTypeNoiseData:
        {
            NoiseDataPacket packet;
            ar >> packet;
            m_Dlg->AddNoiseData(packet);
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
            default:
                CString msg;
                msg.AppendFormat(TEXT("未知融合算法类型%d."), type);
                AfxMessageBox(msg);
                break;
            }
        }
        break;
    case PacketTypeKeyTarget:
        {
            int length;
            ar >> length;
            HANDLE file = CreateFile(
                KEY_TARGET_FILE_NAME,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            char *buf = new char[length];
            Receive(&buf[0], length);
            DWORD written = 0;
            WriteFile(file, buf, length, &written, NULL);
            delete[] buf;
            CloseHandle(file);
            m_Dlg->AddNoiseDataPhase2();
        }
        break;
    default:
        PlaneSocket::Dispatch(type, ar);
        break;
    }
}

void FusionSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("与我机的连接断开"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
    CSocket::OnClose(nErrorCode);
}

void FusionSocket::SendKeyTarget(double theta, double phi)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeKeyTarget << theta << phi;
    ar.Flush();
}