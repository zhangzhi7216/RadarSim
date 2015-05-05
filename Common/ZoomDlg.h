#pragma once

#include <afxwin.h>

#include "Common.h"
#include "CommonDlg.h"

// CZoomDlg 对话框

class __declspec(dllexport) CZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CCommonDlg *dlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZoomDlg();

    static void CreateDlg(CZoomDlg &dlg);

// 对话框数据
	enum { IDD = IDD_ZOOM_DLG };

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnClose();

public:
    CCommonDlg *m_Dlg;
    Image *m_Image;
    bool m_Initialized;

    void DrawTarget();
};
