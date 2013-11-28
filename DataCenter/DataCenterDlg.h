
// DataCenterDlg.h : 头文件
//

#pragma once

class ServerSocket;
class ClientSocket;

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

    ServerSocket *m_Socket;
    vector<ClientSocket *> m_PlaneClients;
    vector<ClientSocket *> m_FusionClients;

public:
    void OnAccept();
};
