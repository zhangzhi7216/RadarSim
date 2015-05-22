// DataCenterDlg.h : 头文件
//

#pragma once

#include <afxmt.h>

#include "../Common/StateMap.h"
#include "../Common/StateMapDlg.h"
#include "TargetClient.h"
#include "../Common/DataPacket.h"
// #include "../Common/MatlabDlg3.h"
#include "../Common/MatlabDlg.h"

#include "FusionAlgo.h"

#include "CommonDlg.h"
#include "afxwin.h"

#include "EvalItem.h"

class DataCenterSocket;

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
    Plane m_Plane;
    vector<TrueDataFrame> m_PlaneTrueDatas;
    DataCenterSocket *m_PlaneSockets[PLANE_COUNT];
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
    CComboBox m_NoiseType;
    afx_msg void OnCbnSelchangeDcGlobalNoise();
    CComboBox m_StateMapBkg;
    afx_msg void OnCbnSelchangeDcStateMapBkg();
    CComboBox m_StateMapExplosionType;
    afx_msg void OnCbnSelchangeDcStateMapExplosionType();

    CComboBox m_FusionAlgoSel;
    CComboBox m_NaviAlgoSel;
    afx_msg void OnBnClickedStateMapDlgButton();
    afx_msg void OnBnClickedMatlabDlgButton();
    CComboBox m_SensorIdSel;
    afx_msg void OnCbnSelchangeDcSensorId();
    BOOL m_SensorEnable;
    CString m_SensorMaxDis;
    CString m_SensorMaxTheta;
    CString m_SensorMaxPhi;
    CString m_SensorDisVar;
    CString m_SensorThetaVar;
    CString m_SensorPhiVar;
    CComboBox m_SensorInterval;
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
    afx_msg void OnCbnSelchangeDcSensorInterval();
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
    CString m_PlanePosX;
    CString m_PlanePosY;
    CString m_PlanePosZ;
    CString m_PlaneVelX;
    CString m_PlaneVelY;
    CString m_PlaneVelZ;
    CString m_PlaneAccX;
    CString m_PlaneAccY;
    CString m_PlaneAccZ;
    CString m_PlanePal;
    CString m_PlaneRadius;
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
    CString m_ExtDataPath;
    afx_msg void OnEnChangeExtDataPath();

    void DumpTrueData(LPCWSTR path);
    CString m_OutputPlaneTrue;
    CString m_OutputTargetTrue;
    CString m_OutputTargetNoise;
    CString m_OutputFusion;
    CComboBox m_EvalSel;
    CString m_EvalName;
    CString m_EvalDll;
    afx_msg void OnEnChangeDcEvalPlaneTrue();
    afx_msg void OnEnChangeDcEvalTargetTrue();
    afx_msg void OnEnChangeDcTargetNoise();
    afx_msg void OnEnChangeDcEvalFusion();

    void OutputPlaneTrueData();
    void OutputTargetTrueData();
    void OutputTargetNoiseData();
    void OutputFusionData();
    CString m_EvalFunc;
    afx_msg void OnEnChangeDcEvalFunc();
    afx_msg void OnEnChangeDcEvalDll();
    afx_msg void OnEnChangeDcEvalName();
    afx_msg void OnCbnSelchangeDcEvalId();

    vector<EvalItem> m_EvalItems;
};
