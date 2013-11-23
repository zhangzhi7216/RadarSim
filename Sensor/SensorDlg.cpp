// SensorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sensor.h"
#include "SensorDlg.h"


// CSensorDlg 对话框

IMPLEMENT_DYNAMIC(CSensorDlg, CDialog)

CSensorDlg::CSensorDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent /*=NULL*/)
	: CDialog(CSensorDlg::IDD, pParent)
    , m_Title(title)
    , m_Sensor(sensor)
    , m_ClientProxy(clientProxy)
    , m_Initialized(false)
{
    m_Ctrl = new CSensorCtrl(m_Sensor);
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSensorDlg::~CSensorDlg()
{
    delete m_Ctrl;
}

void CSensorDlg::CreateDlg(CSensorDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_SENSOR_DLG, GetDesktopWindow());
}

void CSensorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENSOR, *m_Ctrl);
    DDX_Text(pDX, IDC_SENSOR_TYPE, Sensor::SensorTypeNames[m_Sensor.m_Type]);
    DDX_Check(pDX, IDC_SENSOR_ENABLE, m_Sensor.m_Enable);
    DDX_Text(pDX, IDC_SENSOR_MAX_DIS, m_Sensor.m_MaxDis);
    DDX_Text(pDX, IDC_SENSOR_MAX_THETA, m_Sensor.m_MaxTheta);
    DDX_Text(pDX, IDC_SENSOR_MAX_PHI, m_Sensor.m_MaxPhi);
    DDX_Text(pDX, IDC_SENSOR_DIS_VAR, m_Sensor.m_DisVar);
    DDX_Text(pDX, IDC_SENSOR_THETA_VAR, m_Sensor.m_ThetaVar);
    DDX_Text(pDX, IDC_SENSOR_PHI_VAR, m_Sensor.m_PhiVar);
    DDX_Text(pDX, IDC_SENSOR_PRO_DET, m_Sensor.m_ProDet);
    DDX_Check(pDX, IDC_SENSOR_SHOW_SCANLINE, m_Sensor.m_ShowScanline);
    DDX_Check(pDX, IDC_SENSOR_SHOW_TRACK, m_Sensor.m_ShowTrack);
    DDX_Control(pDX, IDC_SENSOR_TARGET_ID, m_TargetId);
    DDX_Control(pDX, IDC_SENSOR_TARGET_COLOR, m_TargetColor);
}


BEGIN_MESSAGE_MAP(CSensorDlg, CDialog)
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_SENSOR_SHOW_SCANLINE, &CSensorDlg::OnBnClickedSensorShowScanline)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_SENSOR_ENABLE, &CSensorDlg::OnBnClickedSensorEnable)
    ON_BN_CLICKED(IDC_SENSOR_SHOW_TRACK, &CSensorDlg::OnBnClickedSensorShowTrack)
    ON_EN_CHANGE(IDC_SENSOR_MAX_DIS, &CSensorDlg::OnEnChangeSensorMaxDis)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_ID, &CSensorDlg::OnCbnSelchangeSensorTargetId)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_COLOR, &CSensorDlg::OnCbnSelchangeSensorTargetColor)
END_MESSAGE_MAP()


// CSensorDlg 消息处理程序

BOOL CSensorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_SENSOR_GRP)->SetWindowTextW(m_Title);

    for (int i = 0; i < Sensor::TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, Sensor::TargetColorNames[i]);
    }

    Resize();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSensorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSensorDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // CDialog::OnClose();
    m_ClientProxy.OnClose();
}

void CSensorDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = 2 * (80 + PAD), top = rect.top + PAD, height = rect.bottom - rect.top - PAD * 2;
    int left2 = left + width + PAD, width2 = rect.right - PAD - left2, top2 = top, height2 = height;

    GetDlgItem(IDC_SENSOR_PARAM_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = 80;
    top = top + PAD * 2;
    height = 20;
    GetDlgItem(IDC_SENSOR_TYPE_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TYPE)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_ENABLE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_DIS_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_DIS)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_THETA_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_THETA)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_PHI_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_PHI)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_DIS_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_DIS_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_THETA_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_THETA_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PHI_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PHI_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PRO_DET_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PRO_DET)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_SCANLINE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_TRACK)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_ID_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_ID)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_COLOR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_COLOR)->MoveWindow(left + width, top, width, height);

    GetDlgItem(IDC_SENSOR_GRP)->MoveWindow(left2, top2, width2, height2);
    GetDlgItem(IDC_SENSOR)->MoveWindow(left2 + PAD, top2 + PAD * 2, width2 - PAD * 2, height2 - PAD * 3);
}

void CSensorDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_Initialized)
    {
        Resize();
    }
}

void CSensorDlg::OnBnClickedSensorEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Sensor.m_Enable = !m_Sensor.m_Enable;
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnBnClickedSensorShowScanline()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Sensor.m_ShowScanline = !m_Sensor.m_ShowScanline;
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnBnClickedSensorShowTrack()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Sensor.m_ShowTrack = !m_Sensor.m_ShowTrack;
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();
    m_ClientProxy.DrawTargets();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnEnChangeSensorMaxDis()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_Sensor.m_MaxDis = GetDlgItemInt(IDC_SENSOR_MAX_DIS);
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnCbnSelchangeSensorTargetId()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_TargetColor.SetCurSel(m_Sensor.m_TargetColors[index]);
    }
}

void CSensorDlg::OnCbnSelchangeSensorTargetColor()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_Sensor.m_TargetColors[index] = (Sensor::TargetColor)m_TargetColor.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();
        m_ClientProxy.DrawTargets();
        m_ClientProxy.BlendAll();
        m_ClientProxy.Invalidate();
    }
}

void CSensorDlg::Reset()
{
    m_Ctrl->Reset();

    m_TargetId.ResetContent();
    m_TargetColor.SetCurSel(CB_ERR);

    UpdateData(FALSE);
}

void CSensorDlg::AddTarget(Target &target)
{
    CString str;
    str.AppendFormat(TEXT("%d"), target.m_Id);
    m_TargetId.InsertString(m_TargetId.GetCount(), str);
}
