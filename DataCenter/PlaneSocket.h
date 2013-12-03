#pragma once

#include "Target.h"
#include "Sensor.h"
#include "StateMap.h"
#include "DataPacket.h"

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
    void SendPlane(Plane &plane);
    void SendTarget(Target &target);
    void SendRadar(Sensor &radar);
    void SendEsm(Sensor &esm);
    void SendInfrared(Sensor &infrared);
    void SendStateMap(StateMap &stateMap);
    void SendTrueData(TrueDataPacket &packet);

protected:
    CDataCenterDlg *m_Dlg;
    bool m_IsFusion;
    bool m_FusionAddrSent;
};

