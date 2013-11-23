#pragma once

#include "DataList.h"
#include "DataListClientProxy.h"
#include "DataListCtrl.h"

// CDataListDlg 对话框

class __declspec(dllexport) CDataListDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataListDlg)

public:
	CDataListDlg(LPCWSTR title, DataList &dataList, DataListClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataListDlg();

    static void CreateDlg(CDataListDlg &dlg);

// 对话框数据
	enum { IDD = IDD_DATALIST_DLG };

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    CString m_Title;
    DataList &m_DataList;
    DataListClientProxy &m_ClientProxy;
    CDataListCtrl *m_Ctrl;
    afx_msg void OnClose();

    bool m_Initialized;
    virtual void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    CComboBox m_TargetId;
    afx_msg void OnCbnSelchangeSensorTargetId();
    CComboBox m_TargetColor;
    afx_msg void OnCbnSelchangeSensorTargetColor();

    void Reset();
    void AddTarget(Target &target);
};
