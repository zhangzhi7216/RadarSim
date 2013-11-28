#pragma once

class CDataCenterDlg;

class PlaneSocket :
    public CSocket
{
public:
    PlaneSocket(CDataCenterDlg *dlg);
    ~PlaneSocket(void);

    enum Stage
    {
        STAGE_RECV_ID,
    };

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendPlaneData();

protected:
    Stage m_Stage;
    int m_Id;
    CDataCenterDlg *m_Dlg;
};

