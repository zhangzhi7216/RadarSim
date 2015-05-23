#pragma once

#include "DataPacket.h"

class CRenderCenterDlg;

class RenderCenterSocket :
    public CSocket
{
public:
    RenderCenterSocket(CRenderCenterDlg *dlg);
    ~RenderCenterSocket(void);

    void OnAccept(int nErrorCode);
    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendKeyTarget(CString &path);

protected:
    CRenderCenterDlg *m_Dlg;
};

