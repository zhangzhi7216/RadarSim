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
    PlaneSocket *socket = new PlaneSocket(m_Dlg);
    if (Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
}
