
// AttackPlaneDlg.h : 头文件
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

using namespace std;

// CAttackPlaneDlg 对话框
class CAttackPlaneDlg : public CPlaneDlg
{
// 构造
public:
	CAttackPlaneDlg(LPCWSTR title, CWnd* pParent = NULL);	// 标准构造函数
	~CAttackPlaneDlg();	// 标准构造函数

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    virtual void ResetCtrls();

    virtual void ConnectDataCenter();
    virtual void SendNoiseData(NoiseDataPacket &packet);
    virtual void SetNaviAlgo(NaviAlgo *algo);
    NaviAlgo *m_NaviAlgo;
    void DoNavi(const ControlDataPacket &packet);
    NaviOutput m_NaviOutput;

    virtual void AddTrueData(TrueDataPacket &packet);
    virtual void AddControlData(ControlDataPacket &packet);

    vector<TargetState> m_TargetStates;
};
