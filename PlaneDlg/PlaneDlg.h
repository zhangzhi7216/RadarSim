
// PlaneDlg.h : 头文件
//

#pragma once
#include "../Common/Resource.h"
#include "../Common/SensorCtrl.h"
#include "../Common/SensorDlg.h"
#include "../Common/DataListCtrl.h"
#include "../Common/DataListDlg.h"
#include "../Common/StateMapCtrl.h"
#include "../Common/StateMapDlg.h"
#include "../Common/ZoomDlg.h"
#include "../Common/CommonDlg.h"

#include "Target.h"

#include <vector>
#include "CommonDlg.h"

#include "PlaneSocket.h"

#include "DataPacket.h"

#include "FusionAlgo.h"

using namespace std;

// CPlaneDlg 对话框
class CPlaneDlg : public CCommonDlg
{
// 构造
public:
	CPlaneDlg(PacketType planeType
        , LPCWSTR title
        , bool hasSensor1
        , CString sensor1Title
        , bool hasSensor2
        , CString sensor2Title
        , bool hasStateMap
        , bool hasDataList = true
        , CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLANE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    bool m_Initialized;
    CString m_Title;
    PacketType m_PlaneType;
    void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    virtual void ResetCtrls();
    void ResetSensors();
    void AddPlane(Plane &plane);
    void AddTarget(Target &target);
    void AddMissile(int id);

    GlobalDataPacket m_GlobalData;
    Plane m_Plane;
    vector<Target> m_Targets;

    bool m_HasSensor1;
    CString m_Sensor1Title;
    bool m_ShowSensor1Dlg;
    Sensor m_Sensor1;
    CSensorCtrl m_Sensor1Ctrl;
    CSensorDlg m_Sensor1Dlg;
    virtual afx_msg void OnStnDblclickSensor1Ctrl();

    bool m_HasSensor2;
    CString m_Sensor2Title;
    bool m_ShowSensor2Dlg;
    Sensor m_Sensor2;
    CSensorCtrl m_Sensor2Ctrl;
    CSensorDlg m_Sensor2Dlg;
    afx_msg void OnStnDblclickSensor2Ctrl();

    bool m_HasStateMap;;
    bool m_ShowStateMapDlg;
    StateMap m_StateMap;
    CStateMapCtrl m_StateMapCtrl;
    CStateMapDlg m_StateMapDlg;
    afx_msg void OnStnDblclickStateMapCtrl();

    bool m_HasDataList;;
    bool m_ShowDataListDlg;
    DataList m_DataList;
    CDataListCtrl m_DataListCtrl;
    CDataListDlg m_DataListDlg;
    afx_msg void OnNMDblclkDatalistCtrl(NMHDR *pNMHDR, LRESULT *pResult);

    bool m_ShowZoomDlg;
    CZoomDlg m_ZoomDlg;
    void OnCloseZoomDlg();

    virtual void OnSubDlgClose(void *subDlg);

    virtual void OnSubDlgEnable(void *subDlg);

    virtual void OnSubDlgShowScanline(void *subDlg);

    virtual void OnSubDlgShowTrack(void *subDlg);

    virtual void OnSubDlgTargetColor(void *subDlg);

    virtual void OnSubDlgMaxDis(void *subDlg);

    virtual void OnSubDlgMaxTheta(void *subDlg);

    virtual void OnSubDlgMaxPhi(void *subDlg);

    virtual void OnSubDlgDisVar(void *subDlg);

    virtual void OnSubDlgThetaVar(void *subDlg);

    virtual void OnSubDlgPhiVar(void *subDlg);

    virtual void OnSubDlgProDet(void *subDlg);

    virtual void OnSubDlgStateMapBackgroundChange(void *subDlg);

    virtual void OnSubDlgStateMapTargetsChange(void *subDlg);

    virtual void ResetSockets();
    virtual void CreateDataCenterSocket();
    virtual void ConnectDataCenter();
    virtual void ConnectFusion(const CString &addr, int port);
    virtual void SendPlaneType();
    virtual void SetPlane(Plane &plane);
    virtual void SetSensor1(Sensor &sensor1);
    virtual void SetSensor2(Sensor &sensor2);
    virtual void SetStateMap(StateMap &stateMap);
    virtual void SetGlobalData(GlobalDataPacket &packet);
    virtual void AddTrueData(TrueDataPacket &packet);
    virtual void SendNoiseDatas(TrueDataPacket &packet);

    PlaneSocket *m_DataCenterSocket;
    PlaneSocket *m_FusionSocket;
};
