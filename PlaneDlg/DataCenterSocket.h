#pragma once

#include "DataPacket.h"

class CPlaneDlg;

class DataCenterSocket :
    public CSocket
{
public:
    DataCenterSocket(CPlaneDlg *dlg);
    ~DataCenterSocket(void);

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendImFusion(int port);
    void SendImRadar();
    void SendImDetect();
    void SendFusionData(FusionDataPacket &packet);

public:
    CPlaneDlg *m_Dlg;
};

