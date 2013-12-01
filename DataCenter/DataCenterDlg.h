// DataCenterDlg.h : 头文件
//

#pragma once

#include <afxmt.h>

class DataCenterSocket;
class PlaneSocket;

// CDataCenterDlg 对话框
class CDataCenterDlg : public CDialog
{
// 构造
public:
	CDataCenterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATACENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnNetwork(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

    DataCenterSocket *m_DataCenterSocket;
    vector<PlaneSocket *> m_PlaneSockets;
    bool m_FusionConnected;
    CString m_FusionAddr;
    int m_FusionPort;

public:
    void AddPlaneSocket();
    void SetFusionAddr(const CString &addr, int port);
    void ResetSockets();
    CCriticalSection m_Lock;
};
