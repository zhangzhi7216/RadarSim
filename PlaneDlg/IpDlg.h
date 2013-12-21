#pragma once


// CIpDlg 对话框

class CIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CIpDlg)

public:
	CIpDlg(LPCWSTR hostName, int port, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIpDlg();

// 对话框数据
	enum { IDD = IDD_IP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_HostName;
    int m_Port;
};
