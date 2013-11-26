#pragma once

#include "StateMap.h"
#include "StateMapCtrl.h"

class __declspec(dllexport) CStateMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CStateMapDlg)

public:
	CStateMapDlg(LPCWSTR title, StateMap &stateMap, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStateMapDlg();

    static void CreateDlg(CStateMapDlg &dlg);

// 对话框数据
	enum { IDD = IDD_STATEMAP_DLG };

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    CString m_Title;
    StateMap &m_StateMap;
    CStateMapCtrl m_Ctrl;
    bool m_Initialized;
    void Reset();
    void AddTarget(Target &target);
};
