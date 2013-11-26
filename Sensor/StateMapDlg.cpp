// StateMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SensorApp.h"
#include "StateMapDlg.h"


// CStateMapDlg 对话框

IMPLEMENT_DYNAMIC(CStateMapDlg, CDialog)

CStateMapDlg::CStateMapDlg(LPCWSTR title, StateMap &stateMap, CWnd* pParent /*=NULL*/)
	: CDialog(CStateMapDlg::IDD, pParent)
    , m_Title(title)
    , m_StateMap(stateMap)
    , m_Initialized(false)
    , m_Ctrl(stateMap)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStateMapDlg::~CStateMapDlg()
{
}

void CStateMapDlg::CreateDlg(CStateMapDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_STATEMAP_DLG, GetDesktopWindow());
}

void CStateMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATEMAP, m_Ctrl);
}
BEGIN_MESSAGE_MAP(CStateMapDlg, CDialog)
END_MESSAGE_MAP()


// CStateMapDlg 消息处理程序

BOOL CStateMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_STATEMAP_GRP)->SetWindowTextW(m_Title);

    /*
    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, TargetColorNames[i]);
    }

    Resize();
    */

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStateMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStateMapDlg::Reset()
{
    m_Ctrl.Reset();

    UpdateData(FALSE);
}

void CStateMapDlg::AddTarget(Target &target)
{
}
