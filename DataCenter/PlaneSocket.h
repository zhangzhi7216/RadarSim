#pragma once

#include "Target.h"
#include "Sensor.h"
#include "StateMap.h"
#include "DataPacket.h"

#include "FusionAlgo.h"
#include "NaviAlgo.h"

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
    void SendReset();
    void SendPlane(Plane &plane);
    void SendTarget(Target &target);
    void SendRadar(Sensor &radar);
    void SendEsm(Sensor &esm);
    void SendInfrared(Sensor &infrared);
    void SendStateMap(StateMap &stateMap);
    void SendFusionAlgo(FusionAlgo *algo);
    void SendNaviAlgo(NaviAlgo *algo);
    void SendGlobalData(GlobalDataPacket &packet);
    void SendTrueData(TrueDataPacket &packet);
    bool IsFusion();
    bool IsAttack();

protected:
    CDataCenterDlg *m_Dlg;
    bool m_IsFusion;
    bool m_IsAttack;
    bool m_FusionAddrSent;
};

