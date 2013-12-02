
// PlaneDlg.h : 头文件
//

#pragma once
#include "../Common/Resource.h"
#include "../Common/SensorCtrl.h"
#include "../Common/SensorDlg.h"
#include "../Common/EsmDlg.h"
#include "../Common/InfraredCtrl.h"
#include "../Common/InfraredDlg.h"
#include "../Common/DataListCtrl.h"
#include "../Common/DataListDlg.h"
#include "../Common/StateMapCtrl.h"
#include "../Common/StateMapDlg.h"
#include "../Common/CommonDlg.h"

#include "Target.h"

#include <vector>
#include "CommonDlg.h"

#include "DataCenterSocket.h"
#include "FusionSocket.h"
#include "PlaneSocket.h"

using namespace std;

// CPlaneDlg 对话框
class CPlaneDlg : public CCommonDlg
{
// 构造
public:
	CPlaneDlg(LPCWSTR title, CWnd* pParent = NULL);	// 标准构造函数

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
    void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    Plane m_Plane;
    vector<Target> m_Targets;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    bool m_ShowRadarDlg;
    Sensor m_Radar;
    CSensorCtrl m_RadarCtrl;
    CSensorDlg m_RadarDlg;
    virtual afx_msg void OnStnDblclickRadarCtrl();

    bool m_ShowEsmDlg;
    Sensor m_Esm;
    CSensorCtrl m_EsmCtrl;
    CEsmDlg m_EsmDlg;
    afx_msg void OnStnDblclickEsmCtrl();

    bool m_ShowInfraredDlg;
    Sensor m_Infrared;
    CInfraredCtrl m_InfraredCtrl;
    CInfraredDlg m_InfraredDlg;
    afx_msg void OnStnDblclickInfraredCtrl();

    bool m_ShowDataListDlg;
    DataList m_DataList;
    CDataListCtrl m_DataListCtrl;
    CDataListDlg m_DataListDlg;
    afx_msg void OnNMDblclkDatalistCtrl(NMHDR *pNMHDR, LRESULT *pResult);

    bool m_ShowStateMapDlg;
    StateMap m_StateMap;
    CStateMapDlg m_StateMapDlg;

    void ResetCtrls();
    void ResetSensors();
    void AddPlane(Plane &plane);
    void AddTarget(Target &target);

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

    virtual void ResetSockets();
    virtual void ConnectDataCenter();
    virtual void ConnectFusion(const CString &addr, int port);
    virtual void AddPlaneSocket();
    virtual void SetPlane(Plane &plane);
    virtual void SetRadar(Sensor &radar);
    virtual void SetEsm(Sensor &esm);
    virtual void SetInfrared(Sensor &infrared);
    virtual void SetStateMap(StateMap &stateMap);

    DataCenterSocket *m_DataCenterSocket;
    FusionSocket *m_FusionSocket;
};
