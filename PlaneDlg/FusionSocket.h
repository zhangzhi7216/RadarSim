#pragma once

#include "DataPacket.h"

class CFusionPlaneDlg;

class FusionSocket :
    public CSocket
{
public:
    FusionSocket(CFusionPlaneDlg *dlg);
    ~FusionSocket(void);

    void OnAccept(int nErrorCode);
    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

public:
    CFusionPlaneDlg *m_Dlg;
};

