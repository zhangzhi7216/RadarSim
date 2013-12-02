// DataCenterDlg.h : 头文件
//

#pragma once

#include <afxmt.h>

#include "../Common/StateMap.h"
#include "../Common/StateMapDlg.h"
#include "PlaneClient.h"

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
	DECLARE_MESSAGE_MAP()

    bool m_ShowStateMapDlg;
    StateMap m_StateMap;
    CStateMapDlg m_StateMapDlg;

    DataCenterSocket *m_DataCenterSocket;
    vector<PlaneSocket *> m_PlaneSockets;
    bool m_FusionConnected;
    CString m_FusionAddr;
    int m_FusionPort;

public:
    void AddPlaneSocket();
    void SetFusionAddr(const CString &addr, int port);
    void ResetSockets();
    void ResetCtrls();
    CCriticalSection m_Lock;
    afx_msg void OnBnClickedOk();
    PlaneClient m_PlaneClients[PLANE_COUNT];
    Target m_Targets[TARGET_COUNT];
    void GeneratePlaneClients();
    void GenerateTargets();
    void GenerateSimData();
    Path m_PlaneDatas[PLANE_COUNT];
    Path m_TargetDatas[TARGET_COUNT];
    void StartSim();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
