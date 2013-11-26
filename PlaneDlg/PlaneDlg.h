
// PlaneDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../Sensor/Resource.h"
#include "../Sensor/SensorCtrl.h"
#include "../Sensor/SensorDlg.h"
#include "../Sensor/EsmDlg.h"
#include "../Sensor/InfraredCtrl.h"
#include "../Sensor/InfraredDlg.h"
#include "../Sensor/DataListCtrl.h"
#include "../Sensor/DataListDlg.h"
#include "../Sensor/StateMapCtrl.h"
#include "../Sensor/StateMapDlg.h"

#include "PlaneRadarProxy.h"
#include "PlaneEsmProxy.h"
#include "PlaneInfraredProxy.h"
#include "PlaneDataListProxy.h"

#include "Target.h"

#include <vector>
#include "afxcmn.h"

using namespace std;

// CPlaneDlg 对话框
class CPlaneDlg : public CDialog
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
    PlaneRadarProxy m_PlaneRadarProxy;
    CSensorDlg m_RadarDlg;
    afx_msg void OnStnDblclickRadarCtrl();

    bool m_ShowEsmDlg;
    Sensor m_Esm;
    CSensorCtrl m_EsmCtrl;
    PlaneEsmProxy m_PlaneEsmProxy;
    CEsmDlg m_EsmDlg;
    afx_msg void OnStnDblclickEsmCtrl();

    bool m_ShowInfraredDlg;
    Sensor m_Infrared;
    CInfraredCtrl m_InfraredCtrl;
    PlaneInfraredProxy m_PlaneInfraredProxy;
    CInfraredDlg m_InfraredDlg;
    afx_msg void OnStnDblclickInfraredCtrl();

    bool m_ShowDataListDlg;
    DataList m_DataList;
    CDataListCtrl m_DataListCtrl;
    PlaneDataListProxy m_PlaneDataListProxy;
    CDataListDlg m_DataListDlg;
    afx_msg void OnNMDblclkDatalistCtrl(NMHDR *pNMHDR, LRESULT *pResult);

    bool m_ShowStateMapDlg;
    StateMap m_StateMap;
    CStateMapDlg m_StateMapDlg;

    void Reset();
    void ResetSensors();
    void AddTarget(Target &target);
};
