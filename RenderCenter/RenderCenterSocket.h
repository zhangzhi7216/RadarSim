#pragma once

#include "Target.h"
#include "Sensor.h"
#include "StateMap.h"
#include "DataPacket.h"

#include "FusionAlgo.h"

class CDataCenterDlg;

class DataCenterSocket :
    public CSocket
{
public:
    DataCenterSocket(CDataCenterDlg *dlg);
    ~DataCenterSocket(void);

    void OnAccept(int nErrorCode);
    void OnReceive(int nErrorCode);
    void OnClose(int nErrorCode);

    void SendFusionAddr(const CString &addr, int port);
    void SendReset();
    void SendPlane(Plane &plane);
    void SendTarget(Target &target);
    void SendRadar(Sensor &radar);
    void SendEsm(Sensor &esm);
    void SendTong(Sensor &Tong);
    void SendLei(Sensor &lei);
    void SendStateMap(StateMap &stateMap);
    void SendFusionAlgo(FusionAlgo *algo);
    void SendGlobalData(GlobalDataPacket &packet);
    void SendTrueData(TrueDataPacket &packet);
    bool IsFusion();
    bool IsRadar();
    bool IsDetect();

protected:
    CDataCenterDlg *m_Dlg;
    bool m_IsFusion;
    bool m_IsRadar;
    bool m_IsDetect;
    bool m_FusionAddrSent;
};

