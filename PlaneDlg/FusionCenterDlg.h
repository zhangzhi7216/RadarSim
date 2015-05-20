
// FusionCenterDlg.h : 头文件
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

#include "FusionSocket.h"
#include "FusionAlgo.h"

using namespace std;

// CFusionCenterDlg 对话框
class CFusionCenterDlg : public CPlaneDlg
{
// 构造
public:
	CFusionCenterDlg(LPCWSTR title
        , bool hasSensor1
        , CString sensor1Title
        , bool hasSensor2
        , CString sensor2Title
        , bool hasStateMap
        , bool hasDataList = true
        , CWnd* pParent = NULL);	// 标准构造函数
    ~CFusionCenterDlg();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    virtual void CreateDataCenterSocket();
    virtual void ConnectRenderCenter();
    virtual void ConnectFusion(const CString &addr, int port);
    virtual void SendPlaneType();
    virtual void AddPlaneSocket();
    virtual void AddNoiseData(NoiseDataPacket &packet);
    virtual void AddNoiseDataPhase2();
    virtual void SetFusionAlgo(FusionAlgo *algo);
    virtual void DoFusion();
    virtual void SendNoiseDatas(TrueDataPacket &packet);
    virtual void ResetSockets();
    virtual void SetPlane(Plane &plane);
    virtual void SetGlobalData(GlobalDataPacket &packet);

    FusionSocket *m_FusionSocket;
    vector<FusionSocket *> m_PlaneSockets;
    CCriticalSection m_Lock;
    FusionSocket *m_RenderCenterSocket;
    FusionAlgo *m_FusionAlgo;
    FusionInput m_FusionInput;
    FusionOutput m_FusionOutput;
};
