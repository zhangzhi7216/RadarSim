
// PlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../PlaneDlg/Resource.h"
#include "PlaneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneDlg 对话框

CPlaneDlg::CPlaneDlg(LPCWSTR title, CWnd* pParent /*=NULL*/)
    : CCommonDlg(CPlaneDlg::IDD, pParent)
    , m_Title(title)
    , m_Initialized(false)
    , m_ShowRadarDlg(false)
    , m_Radar(Sensor::SensorTypeSource)
    , m_RadarCtrl(m_Radar)
    , m_RadarDlg(TEXT("雷达"), m_Radar, this)
    , m_ShowEsmDlg(false)
    , m_Esm(Sensor::SensorTypeNonSource)
    , m_EsmCtrl(m_Esm)
    , m_EsmDlg(TEXT("ESM"), m_Esm, this)
    , m_ShowInfraredDlg(false)
    , m_Infrared(Sensor::SensorTypeNonSource)
    , m_InfraredCtrl(m_Infrared)
    , m_InfraredDlg(TEXT("红外"), m_Infrared, this)
    , m_ShowDataListDlg(false)
    , m_DataList(m_Radar, m_Esm, m_Infrared)
    , m_DataListCtrl(m_DataList)
    , m_DataListDlg(TEXT("数据列表"), m_DataList, this)
    , m_ShowStateMapDlg(true)
    , m_StateMap(m_Radar, m_Esm, m_Infrared)
    , m_StateMapDlg(TEXT("态势"), m_StateMap)
    , m_DataCenterSocket(0)
    , m_FusionSocket(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_RadarDlg.m_Dlg = this;
    m_EsmDlg.m_Dlg = this;
    m_InfraredDlg.m_Dlg = this;
    m_DataListDlg.m_Dlg = this;

    m_DataCenterSocket = new DataCenterSocket(this);
    m_FusionSocket = new FusionSocket(this);
}

void CPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RADAR_CTRL, m_RadarCtrl);
    DDX_Control(pDX, IDC_ESM_CTRL, m_EsmCtrl);
    DDX_Control(pDX, IDC_INFRARED_CTRL, m_InfraredCtrl);
    DDX_Control(pDX, IDC_DATALIST_CTRL, m_DataListCtrl);
}

BEGIN_MESSAGE_MAP(CPlaneDlg, CCommonDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_STN_DBLCLK(IDC_RADAR_CTRL, &CPlaneDlg::OnStnDblclickRadarCtrl)
    ON_STN_DBLCLK(IDC_ESM_CTRL, &CPlaneDlg::OnStnDblclickEsmCtrl)
    ON_STN_DBLCLK(IDC_INFRARED_CTRL, &CPlaneDlg::OnStnDblclickInfraredCtrl)
    ON_WM_TIMER()
    ON_NOTIFY(NM_DBLCLK, IDC_DATALIST_CTRL, &CPlaneDlg::OnNMDblclkDatalistCtrl)
END_MESSAGE_MAP()


// CPlaneDlg 消息处理程序

BOOL CPlaneDlg::OnInitDialog()
{
	CCommonDlg::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

    SetWindowTextW(m_Title);

    Resize();

    CSensorDlg::CreateDlg(m_RadarDlg);
    if (m_ShowRadarDlg)
    {
        m_RadarDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CSensorDlg::CreateDlg(m_EsmDlg);
    if (m_ShowEsmDlg)
    {
        m_EsmDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CInfraredDlg::CreateDlg(m_InfraredDlg);
    if (m_ShowInfraredDlg)
    {
        m_InfraredDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CDataListDlg::CreateDlg(m_DataListDlg);
    if (m_ShowDataListDlg)
    {
        m_DataListDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CStateMapDlg::CreateDlg(m_StateMapDlg);
    if (m_ShowStateMapDlg)
    {
        m_StateMapDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    ResetCtrls();
    ResetSockets();
    ConnectDataCenter();

#if 0
    // 初始化我机和目标
    Target target0, target1;

    target0.m_Id = 3;
    target1.m_Id = 4;

    target0.m_Position = Position(150, 150, 150);
    target1.m_Position = Position(200, 200, 200);

    AddPlane(m_Plane);
    AddTarget(target0);
    AddTarget(target1);

    m_Plane.m_Position = Position(100, 100, 100);

    // Debug用的Timer
    SetTimer(0, 800, NULL);
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlaneDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    // Resize Radar.
    int left = rect.left + PAD, width = (rect.right - rect.left) / 2 - PAD * 2, top = rect.top + PAD, height = (rect.bottom - rect.top) / 2 - PAD * 2;
    if (width > height)
    {
        width = height;
    }
    GetDlgItem(IDC_RADAR_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD *2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_RadarCtrl.MoveWindow(left, top, width, height);

    // Resize ESM.
    left = rect.left + PAD;
    width = (rect.right - rect.left) / 2 - PAD * 2;
    top = (rect.bottom - rect.top) / 2 + PAD;
    height = (rect.bottom - rect.top) / 2 - PAD * 2;
    if (width > height)
    {
        width = height;
    }
    GetDlgItem(IDC_ESM_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD *2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_EsmCtrl.MoveWindow(left, top, width, height);

    // Resize DataList.
    left = left + width + PAD + PAD * 2;
    width = rect.right - PAD - left;
    top = rect.top + PAD;
    height = (rect.bottom - rect.top) / 2 - PAD * 2;
    GetDlgItem(IDC_DATALIST_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD * 2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_DataListCtrl.MoveWindow(left, top, width, height);

    // Resize Infrared.
    left = left - PAD;
    width = width + PAD * 2;
    top = (rect.bottom - rect.top) / 2 + PAD;
    height = (rect.bottom - rect.top) / 2 - PAD * 2;
    GetDlgItem(IDC_INFRARED_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD * 2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_InfraredCtrl.MoveWindow(left, top, width, height);
}

void CPlaneDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_Initialized)
    {
        Resize();
    }
}

void CPlaneDlg::OnStnDblclickRadarCtrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowRadarDlg)
    {
        m_RadarDlg.ShowWindow(SW_HIDE);
        m_ShowRadarDlg = false;
    }
    else
    {
        m_RadarDlg.ShowWindow(SW_SHOW);
        m_ShowRadarDlg = true;
    }
}

void CPlaneDlg::OnStnDblclickEsmCtrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowEsmDlg)
    {
        m_EsmDlg.ShowWindow(SW_HIDE);
        m_ShowEsmDlg = false;
    }
    else
    {
        m_EsmDlg.ShowWindow(SW_SHOW);
        m_ShowEsmDlg = true;
    }
}

void CPlaneDlg::OnStnDblclickInfraredCtrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowInfraredDlg)
    {
        m_InfraredDlg.ShowWindow(SW_HIDE);
        m_ShowInfraredDlg = false;
    }
    else
    {
        m_InfraredDlg.ShowWindow(SW_SHOW);
        m_ShowInfraredDlg = true;
    }
}

void CPlaneDlg::OnNMDblclkDatalistCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    // pResult = 0;
    if (m_ShowDataListDlg)
    {
        m_DataListDlg.ShowWindow(SW_HIDE);
        m_ShowDataListDlg = false;
    }
    else
    {
        m_DataListDlg.ShowWindow(SW_SHOW);
        m_ShowDataListDlg = true;
    }
}

void CPlaneDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnTimer(nIDEvent);

    static int count = 0;

    if (++count <= 500)
    {
#if 1
        m_Targets[0].m_Position = m_Targets[0].m_Position + Position(rand() % 3, rand() % 3, rand() % 3);
        m_Targets[1].m_Position = m_Targets[1].m_Position + Position(rand() % 3, rand() % 3, rand() % 3);

        m_Plane.m_Position = m_Plane.m_Position + Position(rand() % 3, rand() % 3, rand() % 3);
#else
        m_Targets[0].m_Position = m_Targets[0].m_Position + Position(0, 1, -1);
        m_Targets[1].m_Position = m_Targets[1].m_Position + Position(-1, 1, 1);
#endif

        m_Plane.m_Position = m_Plane.m_Position + Position(0, 0, 0);

        Position rel0 = m_Targets[0].m_Position - m_Plane.m_Position;
        Position rel1 = m_Targets[1].m_Position - m_Plane.m_Position;

        m_Radar.AddTargetData(0, rel0);
        m_Esm.AddTargetData(0, rel0);
        m_Infrared.AddTargetData(0, rel0);
        m_Radar.AddTargetData(1, rel1);
        m_Esm.AddTargetData(1, rel1);
        m_Infrared.AddTargetData(1, rel1);
        m_StateMap.AddPlaneData(0, m_Plane.m_Position);
        m_StateMap.AddTargetData(0, m_Targets[0].m_Position);
        m_StateMap.AddTargetData(1, m_Targets[1].m_Position);

        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();

        m_RadarDlg.m_Ctrl->DrawTargets();
        m_RadarDlg.m_Ctrl->BlendAll();
        m_RadarDlg.m_Ctrl->Invalidate();

        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();

        m_EsmDlg.m_Ctrl->DrawTargets();
        m_EsmDlg.m_Ctrl->BlendAll();
        m_EsmDlg.m_Ctrl->Invalidate();

        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();

        m_InfraredDlg.m_Ctrl->DrawTargets();
        m_InfraredDlg.m_Ctrl->BlendAll();
        m_InfraredDlg.m_Ctrl->Invalidate();

        m_DataListCtrl.AddTargetData();
        m_DataListDlg.m_Ctrl->AddTargetData();

        m_StateMapDlg.m_Ctrl.DrawTargets();
        m_StateMapDlg.m_Ctrl.BlendAll();
        m_StateMapDlg.m_Ctrl.Invalidate();
    }
    else
    {
        KillTimer(0);
        ResetCtrls();
    }
}

void CPlaneDlg::ResetCtrls()
{
    m_Plane.Reset();

    ResetSensors();

    m_RadarCtrl.Reset();
    m_EsmCtrl.Reset();
    m_InfraredCtrl.Reset();
    m_DataListCtrl.Reset();

    m_RadarDlg.Reset();
    m_EsmDlg.Reset();
    m_InfraredDlg.Reset();
    m_DataListDlg.Reset();
}

void CPlaneDlg::ResetSensors()
{
    m_Radar.Reset();
    m_Esm.Reset();
    m_Infrared.Reset();
    m_DataList.Reset();

    m_Esm.m_MaxDis = 250;
    m_Esm.m_MaxTheta = 90;
    m_Infrared.m_MaxDis = 350;
    m_Infrared.m_MaxTheta = 60;

    m_Esm.m_ThetaRangeColor = Color::Red;
    m_Esm.m_ShowHeight = FALSE;
    m_Infrared.m_ShowScanline = FALSE;
    m_Infrared.m_ShowThetaRange = FALSE;
    m_Infrared.m_ThetaRangeColor = Color::Yellow;
    m_Infrared.m_ShowHeight = FALSE;
}

void CPlaneDlg::AddPlane(Plane &plane)
{
    m_StateMap.AddPlane(plane);
    m_StateMapDlg.AddPlane(plane);
}

void CPlaneDlg::AddTarget(Target &target)
{
    m_Targets.push_back(target);

    m_Radar.AddTarget(target);
    m_Esm.AddTarget(target);
    m_Infrared.AddTarget(target);
    m_DataList.AddTarget(target);
    m_StateMap.AddTarget(target);

    m_RadarCtrl.AddTarget(target);
    m_EsmCtrl.AddTarget(target);
    m_InfraredCtrl.AddTarget(target);
    m_DataListCtrl.AddTarget(target);

    m_RadarDlg.AddTarget(target);
    m_EsmDlg.AddTarget(target);
    m_InfraredDlg.AddTarget(target);
    m_DataListDlg.AddTarget(target);
    m_StateMapDlg.AddTarget(target);
}

void CPlaneDlg::OnSubDlgClose(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        OnStnDblclickRadarCtrl();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        OnStnDblclickEsmCtrl();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        OnStnDblclickInfraredCtrl();
    }
    else if (subDlg == (void *)&m_DataListDlg)
    {
        OnNMDblclkDatalistCtrl(0, 0);
    }
}

void CPlaneDlg::OnSubDlgEnable(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::OnSubDlgShowScanline(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgShowTrack(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgTargetColor(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgMaxDis(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::OnSubDlgMaxTheta(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawThetaRange();
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawThetaRange();
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawThetaRange();
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::OnSubDlgMaxPhi(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgDisVar(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgThetaVar(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgPhiVar(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgProDet(void *subDlg)
{
    if (subDlg == (void *)&m_RadarDlg)
    {
        m_RadarCtrl.DrawTargets();
        m_RadarCtrl.BlendAll();
        m_RadarCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_EsmDlg)
    {
        m_EsmCtrl.DrawTargets();
        m_EsmCtrl.BlendAll();
        m_EsmCtrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_InfraredCtrl.DrawTargets();
        m_InfraredCtrl.BlendAll();
        m_InfraredCtrl.Invalidate();
    }
}

void CPlaneDlg::ConnectDataCenter()
{
    while (!m_DataCenterSocket->Connect(DATA_CENTER_ADDR, DATA_CENTER_PORT))
    {
    }
    m_DataCenterSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    AfxMessageBox(TEXT("连接到数据中心"));
}

void CPlaneDlg::ConnectFusion(const CString &addr, int port)
{
    while (!m_FusionSocket->Connect(addr, port))
    {
    }
    m_FusionSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    AfxMessageBox(TEXT("连接到融合机"));
}

void CPlaneDlg::ResetSockets()
{
    m_DataCenterSocket->Close();
    m_FusionSocket->Close();
    BOOL reuse = TRUE;
    m_DataCenterSocket->SetSockOpt(SO_REUSEADDR, (void *)&reuse, sizeof(reuse), SOL_SOCKET);
    m_FusionSocket->SetSockOpt(SO_REUSEADDR, (void *)&reuse, sizeof(reuse), SOL_SOCKET);
    if (!m_DataCenterSocket->Create())
    {
        AfxMessageBox(TEXT("创建到数据中心的套接字失败"));
    }
    if (!m_FusionSocket->Create())
    {
        AfxMessageBox(TEXT("创建到融合机的套接字失败"));
    }
}

void CPlaneDlg::AddPlaneSocket()
{
}

void CPlaneDlg::SetPlane(Plane &plane)
{
    m_Plane.m_Id = plane.m_Id;
    m_Plane.m_Type = plane.m_Type;
    m_Plane.m_Position = plane.m_Position;

    AddPlane(m_Plane);
}

void CPlaneDlg::SetRadar(Sensor &radar)
{
    m_Radar.m_Type = radar.m_Type;
    m_Radar.m_Enable = radar.m_Enable;
    m_Radar.m_MaxDis = radar.m_MaxDis;
    m_Radar.m_MaxTheta = radar.m_MaxTheta;
    m_Radar.m_MaxPhi = radar.m_MaxPhi;
    m_Radar.m_DisVar = radar.m_DisVar;
    m_Radar.m_ThetaVar = radar.m_ThetaVar;
    m_Radar.m_PhiVar = radar.m_PhiVar;
    m_Radar.m_ProDet = radar.m_ProDet;

    m_RadarCtrl.DrawThetaRange();
    m_RadarCtrl.DrawTargets();
    m_RadarCtrl.BlendAll();
    m_RadarCtrl.Invalidate();

    m_RadarDlg.m_Ctrl->DrawThetaRange();
    m_RadarDlg.m_Ctrl->DrawTargets();
    m_RadarDlg.m_Ctrl->BlendAll();
    m_RadarDlg.m_Ctrl->Invalidate();

    m_RadarDlg.UpdateData(FALSE);

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetEsm(Sensor &esm)
{
    m_Esm.m_Type = esm.m_Type;
    m_Esm.m_Enable = esm.m_Enable;
    m_Esm.m_MaxDis = esm.m_MaxDis;
    m_Esm.m_MaxTheta = esm.m_MaxTheta;
    m_Esm.m_MaxPhi = esm.m_MaxPhi;
    m_Esm.m_DisVar = esm.m_DisVar;
    m_Esm.m_ThetaVar = esm.m_ThetaVar;
    m_Esm.m_PhiVar = esm.m_PhiVar;
    m_Esm.m_ProDet = esm.m_ProDet;

    m_EsmCtrl.DrawThetaRange();
    m_EsmCtrl.DrawTargets();
    m_EsmCtrl.BlendAll();
    m_EsmCtrl.Invalidate();

    m_EsmDlg.m_Ctrl->DrawThetaRange();
    m_EsmDlg.m_Ctrl->DrawTargets();
    m_EsmDlg.m_Ctrl->BlendAll();
    m_EsmDlg.m_Ctrl->Invalidate();

    m_EsmDlg.UpdateData(FALSE);

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetInfrared(Sensor &infrared)
{
    m_Infrared.m_Type = infrared.m_Type;
    m_Infrared.m_Enable = infrared.m_Enable;
    m_Infrared.m_MaxDis = infrared.m_MaxDis;
    m_Infrared.m_MaxTheta = infrared.m_MaxTheta;
    m_Infrared.m_MaxPhi = infrared.m_MaxPhi;
    m_Infrared.m_DisVar = infrared.m_DisVar;
    m_Infrared.m_ThetaVar = infrared.m_ThetaVar;
    m_Infrared.m_PhiVar = infrared.m_PhiVar;
    m_Infrared.m_ProDet = infrared.m_ProDet;

    m_InfraredCtrl.DrawThetaRange();
    m_InfraredCtrl.DrawTargets();
    m_InfraredCtrl.BlendAll();
    m_InfraredCtrl.Invalidate();

    m_InfraredDlg.m_Ctrl->DrawThetaRange();
    m_InfraredDlg.m_Ctrl->DrawTargets();
    m_InfraredDlg.m_Ctrl->BlendAll();
    m_InfraredDlg.m_Ctrl->Invalidate();

    m_InfraredDlg.UpdateData(FALSE);

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetStateMap(StateMap &stateMap)
{
    m_StateMap.m_Background = stateMap.m_Background;
    m_StateMap.m_MaxX = stateMap.m_MaxX;
    m_StateMap.m_MaxY = stateMap.m_MaxY;

    m_StateMapDlg.m_Ctrl.DrawBackground();
    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}
