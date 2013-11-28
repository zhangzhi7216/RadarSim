
// FusionPlaneDlg.h : 头文件
//

#pragma once
#include "PlaneDlg.h"

// CFusionPlaneDlg 对话框
class CFusionPlaneDlg : public CPlaneDlg
{
// 构造
public:
	CFusionPlaneDlg(LPCWSTR title, CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FUSION_PLANE_DIALOG };

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
