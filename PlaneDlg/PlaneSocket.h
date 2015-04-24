#pragma once

#include "DataPacket.h"

class CPlaneDlg;

class PlaneSocket :
    public CSocket
{
public:
    PlaneSocket(CPlaneDlg *dlg);
    ~PlaneSocket(void);

    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendNoiseData(NoiseDataPacket &packet);
    void SendImFusion(int port);
    void SendImRadar();
    void SendImDetect();
    void SendFusionData(FusionDataPacket &packet);

public:
    CPlaneDlg *m_Dlg;
};

