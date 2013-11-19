// RadarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Radar.h"
#include "RadarDlg.h"


// CRadarDlg 对话框

IMPLEMENT_DYNAMIC(CRadarDlg, CDialog)

CRadarDlg::CRadarDlg(RadarParam &param, RadarClientProxy &clientProxy, CWnd* pParent /*=NULL*/)
	: CDialog(CRadarDlg::IDD, pParent)
    , m_Param(param)
    , m_ClientProxy(clientProxy)
    , m_Ctrl(m_Param)
    , m_Initialized(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CRadarDlg::~CRadarDlg()
{
}

void CRadarDlg::CreateDlg(CRadarDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_RADAR_DLG, GetDesktopWindow());

    dlg.SetIcon(dlg.m_hIcon, TRUE);
    dlg.SetIcon(dlg.m_hIcon, FALSE);

    dlg.m_Initialized = true;

    if (!dlg.m_Ctrl.Init())
    {
        ASSERT(0);
    }
    dlg.Resize();
}

void CRadarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RADAR, m_Ctrl);
    DDX_Text(pDX, IDC_RADAR_TYPE, m_Param.Type);
    DDX_Check(pDX, IDC_RADAR_ENABLE, m_Param.Enable);
    DDX_Text(pDX, IDC_RADAR_MAX_DIS, m_Param.MaxDis);
    DDX_Text(pDX, IDC_RADAR_MAX_THETA, m_Param.MaxTheta);
    DDX_Text(pDX, IDC_RADAR_MAX_PHI, m_Param.MaxPhi);
    DDX_Text(pDX, IDC_RADAR_DIS_VAR, m_Param.DisVar);
    DDX_Text(pDX, IDC_RADAR_THETA_VAR, m_Param.ThetaVar);
    DDX_Text(pDX, IDC_RADAR_PHI_VAR, m_Param.PhiVar);
    DDX_Text(pDX, IDC_RADAR_PRO_DET, m_Param.ProDet);
    DDX_Check(pDX, IDC_RADAR_SHOW_SCANLINE, m_Param.ShowScanline);
}


BEGIN_MESSAGE_MAP(CRadarDlg, CDialog)
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_RADAR_SHOW_SCANLINE, &CRadarDlg::OnBnClickedRadarShowScanline)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_RADAR_ENABLE, &CRadarDlg::OnBnClickedRadarEnable)
END_MESSAGE_MAP()


// CRadarDlg 消息处理程序


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRadarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRadarDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // CDialog::OnClose();
    m_ClientProxy.OnClose();
}

void CRadarDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = 2 * (80 + PAD), top = rect.top + PAD, height = rect.bottom - rect.top - PAD * 2;
    int left2 = left + width + PAD, width2 = rect.right - PAD - left2, top2 = top, height2 = height;

    GetDlgItem(IDC_RADAR_PARAM_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = 80;
    top = top + PAD * 2;
    height = 20;
    GetDlgItem(IDC_RADAR_TYPE_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_TYPE)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_ENABLE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_MAX_DIS_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_MAX_DIS)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_MAX_THETA_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_MAX_THETA)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_MAX_PHI_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_MAX_PHI)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_DIS_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_DIS_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_THETA_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_THETA_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_PHI_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_PHI_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_PRO_DET_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_RADAR_PRO_DET)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_RADAR_SHOW_SCANLINE)->MoveWindow(left, top, width, height);

    GetDlgItem(IDC_RADAR_GRP)->MoveWindow(left2, top2, width2, height2);
    GetDlgItem(IDC_RADAR)->MoveWindow(left2 + PAD, top2 + PAD * 2, width2 - PAD * 2, height2 - PAD * 3);
}

void CRadarDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_Initialized)
    {
        Resize();
    }
}

void CRadarDlg::OnBnClickedRadarEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Param.Enable = !m_Param.Enable;
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.Invalidate();
}

void CRadarDlg::OnBnClickedRadarShowScanline()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Param.ShowScanline = !m_Param.ShowScanline;
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.Invalidate();
}
