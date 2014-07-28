#pragma once

#include "StateMap.h"
#include "StateMapCtrl.h"

#include "Common.h"
#include "CommonDlg.h"
#include "afxwin.h"

class __declspec(dllexport) CStateMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CStateMapDlg)

public:
	CStateMapDlg(LPCWSTR title, StateMap &stateMap, CCommonDlg *dlg, CWnd* pParent = NULL);   // 标准构造函数
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
    afx_msg void OnClose();

    bool m_Initialized;
    virtual void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void Reset();
    void AddPlane(Plane &plane);
    void AddTarget(Target &target);
    void AddMissile(Missile &miss);
    CComboBox m_Background;
    CComboBox m_PlaneType;
    CComboBox m_PlaneColor;
    CComboBox m_TargetId;
    CComboBox m_TargetType;
    CComboBox m_TargetColor;
    CComboBox m_MissileId;
    CComboBox m_MissileType;
    CComboBox m_MissileColor;
    afx_msg void OnBnClickedStatemapShowTrack();
    afx_msg void OnBnClickedStatemapShowThetaRange();
    afx_msg void OnCbnSelchangeStatemapBackground();
    afx_msg void OnCbnSelchangeStatemapPlaneType();
    afx_msg void OnCbnSelchangeStatemapPlaneColor();
    afx_msg void OnCbnSelchangeStatemapTargetId();
    afx_msg void OnCbnSelchangeStatemapTargetType();
    afx_msg void OnCbnSelchangeStatemapTargetColor();
    afx_msg void OnCbnSelchangeStatemapMissileId();
    afx_msg void OnCbnSelchangeStatemapMissileType();
    afx_msg void OnCbnSelchangeStatemapMissileColor();
    CComboBox m_PlaneId;
    afx_msg void OnCbnSelchangeStatemapPlaneId();

    CCommonDlg *m_Dlg;
};
