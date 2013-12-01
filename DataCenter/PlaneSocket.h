#pragma once

class CDataCenterDlg;

class PlaneSocket :
    public CSocket
{
public:
    PlaneSocket(CDataCenterDlg *dlg);
    ~PlaneSocket(void);

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendFusionAddr(const CString &addr, int port);
    void SendPlaneData();

protected:
    CDataCenterDlg *m_Dlg;
    bool m_IsFusion;
    bool m_FusionAddrSent;
};

