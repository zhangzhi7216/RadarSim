// DataCenterDlg.h : 头文件
//

#pragma once

#include <afxmt.h>

#include "../Common/StateMap.h"
#include "../Common/StateMapDlg.h"
#include "PlaneClient.h"
#include "TargetClient.h"
#include "../Common/DataPacket.h"
#include "../Common/MatlabDlg.h"

#include "FusionAlgo.h"
#include "NaviAlgo.h"

#include "CommonDlg.h"
#include "afxwin.h"

class DataCenterSocket;
class PlaneSocket;

// CDataCenterDlg 对话框
class CDataCenterDlg : public CCommonDlg
{
// 构造
public:
	CDataCenterDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CDataCenterDlg();

// 对话框数据
	enum { IDD = IDD_DATACENTER_DLG };

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

    bool m_ShowStateMapDlg;
    StateMap m_StateMap;
    CStateMapDlg m_StateMapDlg;

    bool m_ShowMatlabDlg;
    CMatlabDlg m_MatlabDlg;

    DataCenterSocket *m_DataCenterSocket;
    bool m_FusionConnected;
    CString m_FusionAddr;
    int m_FusionPort;
    int m_ConnectedPlanes;
    void OnSubDlgClose(void *subDlg);

public:
    void AddPlaneSocket();
    void SetFusionAddr(const CString &addr, int port);
    void ResetSockets();
    void ResetCtrls();
    void ResetPlanes();
    void ResetTargets();
    CCriticalSection m_Lock;
    afx_msg void OnBnClickedOk();

    Sensor m_Sensors[SensorIdLast];
    PlaneClient m_PlaneClients[PLANE_COUNT];
    vector<TargetClient> m_TargetClients;

    void GeneratePlaneClients();
    void GenerateTargetClients();
    void GenerateTrueData();
    void GenerateGlobalData();
    GlobalDataPacket m_GlobalData;
    void AddFusionData(FusionDataPacket &packet);
    vector<FusionDataPacket> m_FusionDatas;

    void StartSim();
    void PauseSim();
    void ResumeSim();
    void FinishSim();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    // afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
    int m_CurrentFrame;
    int m_CurrentRound;

    void ReadConfigFile();

    vector<FusionAlgo *> m_FusionAlgos;
    vector<NaviAlgo *> m_NaviAlgos;
    CComboBox m_NoiseType;
    afx_msg void OnCbnSelchangeDcGlobalNoise();
    CComboBox m_StateMapBkg;
    afx_msg void OnCbnSelchangeDcStateMapBkg();
    CComboBox m_FusionAlgoSel;
    CComboBox m_NaviAlgoSel;
    afx_msg void OnBnClickedStateMapDlgButton();
    afx_msg void OnBnClickedMatlabDlgButton();
    CComboBox m_SensorIdSel;
    afx_msg void OnCbnSelchangeDcSensorId();
    BOOL m_SensorEnable;
    int m_SensorMaxDis;
    int m_SensorMaxTheta;
    int m_SensorMaxPhi;
    int m_SensorDisVar;
    int m_SensorThetaVar;
    int m_SensorPhiVar;
    int m_SensorProDet;
    afx_msg void OnBnClickedDcSensorEnable();
    afx_msg void OnEnChangeDcGlobalRounds();
    afx_msg void OnEnChangeDcGlobalCycle();
    afx_msg void OnEnChangeDcGlobalStartTime();
    afx_msg void OnEnChangeDcGlobalEndTime();
    afx_msg void OnBnClickedGlobalWildValue();
    afx_msg void OnBnClickedGlobalDelay();
    afx_msg void OnEnChangeDcStateMapMaxX();
    afx_msg void OnEnChangeDcStateMapMaxY();
    afx_msg void OnEnChangeDcSensorMaxDis();
    afx_msg void OnEnChangeDcSensorMaxTheta();
    afx_msg void OnEnChangeDcSensorMaxPhi();
    afx_msg void OnEnChangeDcSensorDisVar();
    afx_msg void OnEnChangeDcSensorThetaVar();
    afx_msg void OnEnChangeDcSensorPhiVar();
    afx_msg void OnEnChangeDcSensorProDet();
    afx_msg void OnBnClickedConfigLoad();
    afx_msg void OnBnClickedConfigSave();
    afx_msg void OnBnClickedDcEvalAdd();
    afx_msg void OnBnClickedDcTargetAdd();
    BOOL m_ExtDataEnable;
    afx_msg void OnBnClickedDcExtDataEnable();
    CButton m_ExtDataLoadButton;
    afx_msg void OnBnClickedExtDataLoadButton();
    CComboBox m_PlaneIdSel;
    afx_msg void OnCbnSelchangeDcPlaneId();
    int m_PlaneId;
    CComboBox m_PlaneColor;
    CComboBox m_PlaneType;
    CComboBox m_PlaneMotion;
    int m_PlanePosX;
    int m_PlanePosY;
    int m_PlanePosZ;
    int m_PlaneVelX;
    int m_PlaneVelY;
    int m_PlaneVelZ;
    int m_PlaneAccX;
    int m_PlaneAccY;
    int m_PlaneAccZ;
    int m_PlanePal;
    int m_PlaneRadius;
    afx_msg void OnCbnSelchangeDcPlaneMotion();
    afx_msg void OnCbnSelchangeDcPlaneColor();
    afx_msg void OnCbnSelchangeDcPlaneType();
    afx_msg void OnEnChangeDcPlanePosX();
    afx_msg void OnEnChangeDcPlanePosY();
    afx_msg void OnEnChangeDcPlanePosZ();
    afx_msg void OnEnChangeDcPlaneVelX();
    afx_msg void OnEnChangeDcPlaneVelY();
    afx_msg void OnEnChangeDcPlaneVelZ();
    afx_msg void OnEnChangeDcPlaneAccX();
    afx_msg void OnEnChangeDcPlaneAccY();
    afx_msg void OnEnChangeDcPlaneAccZ();
    afx_msg void OnEnChangeDcPlanePal();
    afx_msg void OnEnChangeDcPlaneRadius();
};
