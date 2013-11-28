#include "StdAfx.h"
#include "PlaneSocket.h"
#include "DataCenter.h"

#include "DataCenterDlg.h"

PlaneSocket::PlaneSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
, m_Stage(STAGE_RECV_ID)
, m_Id(0)
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
    case STAGE_RECV_ID:
        ar >> m_Id;
        ar.Flush();
        m_Dlg->AddPlaneSocket(m_Id, this);
        break;
    }
}

void PlaneSocket::OnClose(int nErrorCode)
{
    Close();
    m_Dlg->RemovePlaneSocket(m_Id);
}

void PlaneSocket::SendPlaneData()
{
}
