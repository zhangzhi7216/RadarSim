#pragma once

#include "Sensor.h"
#include "SensorClientProxy.h"
#include "SensorCtrl.h"
#include "afxwin.h"

// CSensorDlg 对话框

class __declspec(dllexport) CSensorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSensorDlg)

public:
	CSensorDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
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
    SensorClientProxy &m_ClientProxy;
    CSensorCtrl *m_Ctrl;
    afx_msg void OnClose();
    afx_msg void OnBnClickedSensorShowScanline();

    bool m_Initialized;
    virtual void Resize();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedSensorEnable();
    afx_msg void OnEnChangeSensorMaxDis();
    CComboBox m_TargetId;
    afx_msg void OnBnClickedSensorShowTrack();
    afx_msg void OnCbnSelchangeSensorTargetId();
    CComboBox m_TargetColor;
    afx_msg void OnCbnSelchangeSensorTargetColor();

    void Reset();
    void AddTarget(Target &target);
};
