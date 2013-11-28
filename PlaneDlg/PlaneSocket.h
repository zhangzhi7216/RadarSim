#pragma once

class CPlaneDlg;

class PlaneSocket :
    public CSocket
{
public:
    PlaneSocket(CPlaneDlg *dlg);
    ~PlaneSocket(void);

    enum Stage
    {
        STAGE_RECV_FUSION_PLANE,
        STAGE_RECV_PLANE_DATA,
    };

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendId(int id);

public:
    Stage m_Stage;
    CPlaneDlg *m_Dlg;
};

