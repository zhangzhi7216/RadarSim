
// PlaneDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../Radar/Resource.h"
#include "../Radar/RadarCtrl.h"
#include "../Radar/RadarDlg.h"

#include "PlaneRadarProxy.h"

#include "Target.h"

#include <vector>

using namespace std;

// CPlaneDlg 对话框
class CPlaneDlg : public CDialog
{
// 构造
public:
	CPlaneDlg(CWnd* pParent = NULL);	// 标准构造函数

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
    void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    bool m_ShowRadarDlg;
    Sensor m_Radar;
    CRadarCtrl m_RadarCtrl;
    PlaneRadarProxy m_PlaneRadarProxy;
    CRadarDlg m_RadarDlg;
    afx_msg void OnStnDblclickRadarCtrl();

    Plane m_Plane;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
