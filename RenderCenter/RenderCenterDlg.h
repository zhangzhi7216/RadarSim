
// RenderCenterDlg.h : 头文件
//

#pragma once

#include "CommonDlg.h"
#include "RenderCenterSocket.h"
#include "afxmt.h"

// CRenderCenterDlg 对话框
class CRenderCenterDlg : public CCommonDlg
{
// 构造
public:
	CRenderCenterDlg(CWnd* pParent = NULL);	// 标准构造函数
	Mat img1,img2,img3,imgtarget,imgfusion;
	vector<CString> m_OutputPaths;
	int m_CurrentPath;
//	CCriticalSection m_Lock;
	// 对话框数据
	enum { IDD = IDD_IMAGEFINAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	int rgb2hsi(Mat &image,Mat &hsi);
	int hsi2rgb(Mat &image,Mat &hsi);
	Mat GetImage(double& ,double&); //接受俯仰和方位，返回融合后图像

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboL;
	CStatic m_imageA;
	CStatic m_imageB;
	CStatic m_imagefusion;
	CStatic m_imageturn;
	afx_msg void OnBnClickedInputa();
	afx_msg void OnBnClickedInputb();
	afx_msg void OnBnClickedInputmuban();
	afx_msg void OnBnClickedEnhance();
	afx_msg void OnBnClickedRegistry();
	afx_msg void OnBnClickedImagefusion();
	afx_msg void OnBnClickedLaplace();
	afx_msg void OnBnClickedRate();
	afx_msg void OnBnClickedHisfusion();
	afx_msg void OnBnClickedTarget();
	afx_msg void OnBnClickedStart();
	friend VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc1(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc2(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc3(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc4(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc5(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc6(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc7(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc8(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc9(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc10(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc11(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc12(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc13(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc14(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc15(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc16(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc17(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc18(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc19(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc20(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc21(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc22(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc23(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc24(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc25(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc26(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc27(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc28(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc29(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc30(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	friend VOID CALLBACK TimerProc31(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedSave();

    RenderCenterSocket *m_RenderCenterSocket;
    RenderCenterSocket *m_FusionSocket;
    CCriticalSection m_Lock;
    void SetFusionSocket();
    void ResetFusionSocket();
    void RenderKeyTarget(TrueDataFrame &keyTarget);
};
