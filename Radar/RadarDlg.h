#pragma once

#include "RadarParam.h"
#include "RadarClientProxy.h"
#include "RadarCtrl.h"

// CRadarDlg 对话框

class __declspec(dllexport) CRadarDlg : public CDialog
{
	DECLARE_DYNAMIC(CRadarDlg)

public:
	CRadarDlg(RadarParam &param, RadarClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRadarDlg();

    static void CreateDlg(CRadarDlg &dlg);

// 对话框数据
	enum { IDD = IDD_RADAR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    RadarParam &m_Param;
    RadarClientProxy &m_ClientProxy;
    CRadarCtrl m_Ctrl;
    afx_msg void OnClose();
};
