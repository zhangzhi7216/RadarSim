#pragma once

#include "Sensor.h"
#include "Target.h"
#include "SensorCtrl.h"
#include "CommonDlg.h"

#include "Common.h"

// CSensorDlg 对话框

class __declspec(dllexport) CSensorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSensorDlg)

public:
	CSensorDlg(LPCWSTR title, Sensor &sensor, CCommonDlg *dlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSensorDlg();

    static void CreateDlg(CSensorDlg &dlg);

// 对话框数据
	enum { IDD = IDD_SENSOR_DLG };

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    CString m_Title;
    Sensor &m_Sensor;
    CCommonDlg *m_Dlg;
    CSensorCtrl *m_Ctrl;
    afx_msg void OnClose();
    afx_msg void OnBnClickedSensorShowScanline();

    bool m_Initialized;
    virtual void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedSensorEnable();
    CComboBox m_TargetId;
    afx_msg void OnBnClickedSensorShowTrack();
    afx_msg void OnCbnSelchangeSensorTargetId();
    CComboBox m_TargetColor;
    afx_msg void OnCbnSelchangeSensorTargetColor();

    void Reset();
    void AddTarget(Target &target);
    afx_msg void OnEnChangeSensorMaxDis();
    afx_msg void OnEnChangeSensorMaxTheta();
    afx_msg void OnEnChangeSensorMaxPhi();
    afx_msg void OnEnChangeSensorDisVar();
    afx_msg void OnEnChangeSensorThetaVar();
    afx_msg void OnEnChangeSensorPhiVar();
    afx_msg void OnEnChangeSensorProDet();

    CString m_SensorMaxDis;
    CString m_SensorMaxTheta;
    CString m_SensorMaxPhi;
    CString m_SensorDisVar;
    CString m_SensorThetaVar;
    CString m_SensorPhiVar;
    CString m_SensorProDet;
};
