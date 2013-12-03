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

public:
    CPlaneDlg *m_Dlg;
};

