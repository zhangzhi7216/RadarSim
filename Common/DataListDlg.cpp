// DataListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataListDlg.h"


// CDataListDlg 对话框

IMPLEMENT_DYNAMIC(CDataListDlg, CDialog)

CDataListDlg::CDataListDlg(LPCWSTR title, DataList &dataList, CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialog(CDataListDlg::IDD, pParent)
    , m_Title(title)
    , m_DataList(dataList)
    , m_Dlg(dlg)
    , m_Initialized(false)
{
    m_Ctrl = new CDataListCtrl(m_DataList);
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDataListDlg::~CDataListDlg()
{
    delete m_Ctrl;
}

void CDataListDlg::CreateDlg(CDataListDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_DATALIST_DLG, GetDesktopWindow());
}

void CDataListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DATALIST, *m_Ctrl);
    DDX_Control(pDX, IDC_DATALIST_TARGET_ID, m_TargetId);
    DDX_Control(pDX, IDC_DATALIST_TARGET_COLOR, m_TargetColor);
}


BEGIN_MESSAGE_MAP(CDataListDlg, CDialog)
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_CBN_SELCHANGE(IDC_DATALIST_TARGET_ID, &CDataListDlg::OnCbnSelchangeSensorTargetId)
    ON_CBN_SELCHANGE(IDC_DATALIST_TARGET_COLOR, &CDataListDlg::OnCbnSelchangeSensorTargetColor)
END_MESSAGE_MAP()


// CDataListDlg 消息处理程序

BOOL CDataListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_DATALIST_GRP)->SetWindowTextW(m_Title);

    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, TargetColorNames[i]);
    }

    Resize();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDataListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataListDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}

void CDataListDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = rect.right - rect.left - 2 * PAD, top = rect.top + PAD, height = 20 + PAD * 3;
    GetDlgItem(IDC_DATALIST_PARAM_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = 80;
    top = top + PAD * 2;
    height = 20;
    GetDlgItem(IDC_DATALIST_TARGET_ID_STATIC)->MoveWindow(left, top, width, height);
    left = left + width;
    GetDlgItem(IDC_DATALIST_TARGET_ID)->MoveWindow(left, top, width, height);
    left = left + width + PAD;
    GetDlgItem(IDC_DATALIST_TARGET_COLOR_STATIC)->MoveWindow(left, top, width, height);
    left = left + width;
    GetDlgItem(IDC_DATALIST_TARGET_COLOR)->MoveWindow(left, top, width, height);

    left = rect.left + PAD;
    width = rect.right - rect.left - 2 * PAD;
    top = rect.top + PAD + 20 + PAD * 3 + PAD;
    height = rect.bottom - PAD - top;
    GetDlgItem(IDC_DATALIST_GRP)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_DATALIST)->MoveWindow(left + PAD, top + PAD * 2, width - PAD * 2, height - PAD * 3);
}

void CDataListDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_Initialized)
    {
        Resize();
    }
}

void CDataListDlg::OnCbnSelchangeSensorTargetId()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = GetDlgItemInt(IDC_DATALIST_TARGET_ID);
    if (m_DataList.m_Targets.find(index) != m_DataList.m_Targets.end())
    {
        m_TargetColor.SetCurSel(m_DataList.m_Targets[index].m_Color);
    }
}

void CDataListDlg::OnCbnSelchangeSensorTargetColor()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = GetDlgItemInt(IDC_DATALIST_TARGET_ID);
    if (m_DataList.m_Targets.find(index) != m_DataList.m_Targets.end())
    {
        m_DataList.m_Targets[index].m_Color = (TargetColor)m_TargetColor.GetCurSel();
        m_Ctrl->Invalidate();
        m_Dlg->OnSubDlgTargetColor(this);
    }
}

void CDataListDlg::Reset()
{
    m_Ctrl->Reset();

    m_TargetId.ResetContent();
    m_TargetColor.SetCurSel(CB_ERR);
}

void CDataListDlg::AddTarget(Target &target)
{
    CString str;
    str.AppendFormat(TEXT("%d"), target.m_Id);
    m_TargetId.InsertString(m_TargetId.GetCount(), str);
}
