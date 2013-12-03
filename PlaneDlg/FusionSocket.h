#pragma once

class CPlaneDlg;

class FusionSocket :
    public CSocket
{
public:
    FusionSocket(CPlaneDlg *dlg);
    ~FusionSocket(void);

    void OnAccept(int nErrorCode);
    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    // void SendNoiseData(vector<>

public:
    CPlaneDlg *m_Dlg;
};

