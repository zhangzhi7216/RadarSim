#include "StdAfx.h"
#include "PlaneSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

PlaneSocket::PlaneSocket(CPlaneDlg *dlg)
: m_Dlg(dlg)
, m_Stage(STAGE_RECV_FUSION_PLANE)
{
}

PlaneSocket::~PlaneSocket(void)
{
}

void PlaneSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);
    switch (m_Stage)
    {
    case STAGE_RECV_FUSION_PLANE:
        CString addr;
        int port;
        ar >> addr;
        ar >> port;
        ar.Flush();
        m_Dlg->ConnectFusionPlane(addr, port);
        break;
    }
}

void PlaneSocket::OnClose(int nErrorCode)
{
    Close();
    m_Dlg->ConnectDataCenter();
}

void PlaneSocket::SendId(int id)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);

    ar << id;
    ar.Flush();
}
