#include "StdAfx.h"
#include "DataCenter.h"

#include "DataCenterSocket.h"

#include "DataCenterDlg.h"

#include "PlaneSocket.h"

DataCenterSocket::DataCenterSocket(CDataCenterDlg *dlg)
: m_Dlg(dlg)
{
}

DataCenterSocket::~DataCenterSocket(void)
{
}

void DataCenterSocket::OnAccept(int nErrorCode)
{
    m_Dlg->AddPlaneSocket();
}
