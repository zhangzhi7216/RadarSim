
// PlaneDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../Sensor/Resource.h"
#include "../Sensor/SensorCtrl.h"
#include "../Sensor/SensorDlg.h"

#include "PlaneRadarProxy.h"
#include "PlaneEsmProxy.h"

#include "Target.h"

#include <vector>

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
    CSensorDlg m_EsmDlg;
    afx_msg void OnStnDblclickEsmCtrl();

    Plane m_Plane;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    void Reset();
    void AddTarget(Target &target);
};
