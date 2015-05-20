
// FusionCenterDlg.h : ͷ�ļ�
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

#include "FusionSocket.h"
#include "FusionAlgo.h"

using namespace std;

// CFusionCenterDlg �Ի���
class CFusionCenterDlg : public CPlaneDlg
{
// ����
public:
	CFusionCenterDlg(LPCWSTR title
        , bool hasSensor1
        , CString sensor1Title
        , bool hasSensor2
        , CString sensor2Title
        , bool hasStateMap
        , bool hasDataList = true
        , CWnd* pParent = NULL);	// ��׼���캯��
    ~CFusionCenterDlg();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
