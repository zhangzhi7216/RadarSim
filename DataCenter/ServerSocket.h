#pragma once

class CDataCenterDlg;

class ServerSocket :
    public CSocket
{
public:
    ServerSocket(CDataCenterDlg *dlg);
    ~ServerSocket(void);

    void OnAccept(int nErrorCode);

protected:
    CDataCenterDlg *m_Dlg;
};

