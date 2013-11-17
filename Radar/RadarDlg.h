#pragma once

#include "RadarClientProxy.h"

// CRadarDlg 对话框

class CRadarDlg : public CDialog
{
	DECLARE_DYNAMIC(CRadarDlg)

public:
	CRadarDlg(RadarClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRadarDlg();

// 对话框数据
	enum { IDD = IDD_RADAR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    RadarClientProxy &m_ClientProxy;
};
