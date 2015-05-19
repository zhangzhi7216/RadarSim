#pragma once

#include "PlaneSocket.h"

class CFusionCenterDlg;

class FusionSocket :
    public PlaneSocket
{
public:
    FusionSocket(CFusionCenterDlg *dlg);
    ~FusionSocket(void);

    void OnAccept(int nErrorCode);
    void OnClose(int nErrorCode);
    void Dispatch(int type, CArchive &ar);

    void SendKeyTarget(TrueDataFrame &keyTarget);

public:
    CFusionCenterDlg *m_Dlg;
};

