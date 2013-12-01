#pragma once

class CPlaneDlg;

class DataCenterSocket :
    public CSocket
{
public:
    DataCenterSocket(CPlaneDlg *dlg);
    ~DataCenterSocket(void);

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendFusionAddr(int port);

public:
    CPlaneDlg *m_Dlg;
};

