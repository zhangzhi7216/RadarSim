
// FusionPlaneDlg.h : 头文件
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

#include "FusionAlgo.h"

using namespace std;

// CFusionPlaneDlg 对话框
class CFusionPlaneDlg : public CPlaneDlg
{
// 构造
public:
	CFusionPlaneDlg(LPCWSTR title, CWnd* pParent = NULL);	// 标准构造函数
    ~CFusionPlaneDlg();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    virtual void ConnectDataCenter();
    virtual void ConnectFusion(const CString &addr, int port);
    virtual void AddPlaneSocket();
    virtual void AddNoiseData(SocketPacketPair spp);
    virtual void SendNoiseData(NoiseDataPacket &packet);
    virtual void SetFusionAlgo(FusionAlgo *algo);
    virtual void DoFusion();
    map<int, SocketPacketPair> m_NoiseDatas;
    virtual void ResetSockets();
    virtual void AddControlDataAck();

    FusionSocket *m_FusionSocket;
    vector<PlaneSocket *> m_PlaneSockets;
    CCriticalSection m_Lock;

    FusionAlgo *m_FusionAlgo;

    FusionOutput m_FusionOutput;
};
