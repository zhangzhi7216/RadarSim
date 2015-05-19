#include "StdAfx.h"
#include "FusionSocket.h"
#include "Resource.h"
#include "FusionCenterDlg.h"
#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"

FusionSocket::FusionSocket(CFusionCenterDlg *dlg)
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
                msg.AppendFormat(TEXT("δ֪�ں��㷨����%d."), type);
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
            ar.Read(&buf[0], length);
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
    AfxMessageBox(TEXT("���һ������ӶϿ�"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
    CSocket::OnClose(nErrorCode);
}

void FusionSocket::SendKeyTarget(TrueDataFrame &keyTarget)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeKeyTarget << keyTarget;
    ar.Flush();
}