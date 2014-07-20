
// PlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../PlaneDlg/Resource.h"
#include "PlaneDlg.h"
#include "IpDlg.h"

#include "Utility.h"
#include <algorithm>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneDlg 对话框

CPlaneDlg::CPlaneDlg(LPCWSTR title, CWnd* pParent /*=NULL*/)
    : CCommonDlg(CPlaneDlg::IDD, pParent)
    , m_Title(title)
    , m_Initialized(false)
    , m_DlgType(DlgTypePlane)
    , m_ShowRadarDlg(false)
    , m_Radar(Sensor::SensorTypeSource, m_Plane, m_GlobalData)
    , m_RadarCtrl(m_Radar)
    , m_RadarDlg(TEXT("雷达"), m_Radar, this)
    , m_ShowEsmDlg(false)
    , m_Esm(Sensor::SensorTypeNonSource, m_Plane, m_GlobalData)
    , m_EsmCtrl(m_Esm)
    , m_EsmDlg(TEXT("ESM"), m_Esm, this)
    , m_ShowInfraredDlg(false)
    , m_Infrared(Sensor::SensorTypeNonSource, m_Plane, m_GlobalData)
    , m_InfraredCtrl(m_Infrared)
    , m_InfraredDlg(TEXT("红外"), m_Infrared, this)
    , m_ShowDataListDlg(false)
    , m_DataList(m_Radar, m_Esm, m_Infrared)
    , m_DataListCtrl(m_DataList)
    , m_DataListDlg(TEXT("数据列表"), m_DataList, this)
    , m_MatlabDlg(NULL)
    , m_ShowStateMapDlg(true)
    , m_StateMapDlg(TEXT("态势"), m_StateMap, this)
    , m_DataCenterSocket(0)
    , m_FusionSocket(0)
    , m_AddMissile(false)
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
    ON_WM_LBUTTONDBLCLK()
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

    GetDlgItem(IDC_FUSION_ALGO_STATIC)->ShowWindow(FALSE);
    GetDlgItem(IDC_FUSION_ALGO)->ShowWindow(FALSE);

    GetDlgItem(IDC_NAVI_ALGO_STATIC)->ShowWindow(FALSE);
    GetDlgItem(IDC_NAVI_ALGO)->ShowWindow(FALSE);

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
        if (m_MatlabDlg)
        {
            m_MatlabDlg->Show();
        }
        else
        {
            m_DataListDlg.ShowWindow(SW_SHOW);
        }
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CStateMapDlg::CreateDlg(m_StateMapDlg);

    ResetCtrls();
    ResetSockets();

    ConnectDataCenter();

    if (m_ShowStateMapDlg)
    {
        m_StateMapDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

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

    // Resize fusion & navi algo.
    left = left + PAD;
    top = top + PAD * 2;
    width = width - PAD * 2;
    height = height - PAD * 3;

    if (m_DlgType == DlgTypeFusionPlane)
    {
        GetDlgItem(IDC_FUSION_ALGO_STATIC)->ShowWindow(TRUE);
        GetDlgItem(IDC_FUSION_ALGO)->ShowWindow(TRUE);
        GetDlgItem(IDC_FUSION_ALGO_STATIC)->MoveWindow(left, top, width / 2, 20);
        GetDlgItem(IDC_FUSION_ALGO)->MoveWindow(left + width / 2, top, width / 2, 20);
        top = top + PAD + 20;
        height = height - 20 - PAD;
    }
    else if (m_DlgType == DlgTypeAttackPlane)
    {
        GetDlgItem(IDC_NAVI_ALGO_STATIC)->ShowWindow(TRUE);
        GetDlgItem(IDC_NAVI_ALGO)->ShowWindow(TRUE);
        GetDlgItem(IDC_NAVI_ALGO_STATIC)->MoveWindow(left, top, width / 2, 20);
        GetDlgItem(IDC_NAVI_ALGO)->MoveWindow(left + width / 2, top, width / 2, 20);
        top = top + PAD + 20;
        height = height - 20 - PAD;
    }
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
        if (m_MatlabDlg)
        {
            m_MatlabDlg->Hide();
        }
        else
        {
            m_DataListDlg.ShowWindow(SW_HIDE);
        }
        m_ShowDataListDlg = false;
    }
    else
    {
        if (m_MatlabDlg)
        {
            m_MatlabDlg->Show();
        }
        else
        {
            m_DataListDlg.ShowWindow(SW_SHOW);
        }
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

void CPlaneDlg::AddTrueData(TrueDataPacket &packet)
{
    m_Plane.MoveTo(packet.m_PlaneTrueData.m_Pos);

    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlaneTrueData(0, packet.m_PlaneTrueData.m_Pos);
    }
    m_StateMap.AddPlaneData(0, packet.m_PlaneTrueData.m_Pos);

    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        Position rel = packet.m_TargetTrueDatas[i].m_Pos - packet.m_PlaneTrueData.m_Pos;

        m_Radar.AddTargetData(i, rel);
        m_Esm.AddTargetData(i, rel);
        m_Infrared.AddTargetData(i, rel);

        m_DataList.AddTargetData(i, packet.m_TargetTrueDatas[i].m_Time);

        if (m_MatlabDlg)
        {
            m_MatlabDlg->AddTargetTrueData(i, packet.m_TargetTrueDatas[i].m_Pos);
        }

        NoiseDataFrame frame;
        frame.m_Time = packet.m_PlaneTrueData.m_Time;
        frame.m_Id = packet.m_TargetTrueDatas[i].m_Id;
        frame.m_Dis = m_Radar.m_TargetDistances[i].back();
        frame.m_Theta = m_Infrared.m_TargetThetas[i].back();
        frame.m_Phi = m_Infrared.m_TargetPhis[i].back();
        if (!(frame.m_Dis == 0 && frame.m_Theta == 0 && frame.m_Phi == 0))
        {
            Position noiseAbsPos = packet.m_PlaneTrueData.m_Pos + Utility::Rel(frame.m_Dis, frame.m_Theta, frame.m_Phi);
            m_StateMap.AddTargetData(i, noiseAbsPos);
        }
        else
        {
            m_StateMap.AddTargetData(i, Position(0, 0, 0));
        }
    }

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

bool NoiseDataFrameComp(const NoiseDataFrame &f1, const NoiseDataFrame f2)
{
    if (f1.m_Dis == 0 &&
        f1.m_DisVar == 0 &&
        f1.m_Theta == 0 &&
        f1.m_ThetaVar == 0 &&
        f1.m_Phi == 0 &&
        f1.m_PhiVar == 0)
    {
        return false;
    }
    if (f2.m_Dis == 0 &&
        f2.m_DisVar == 0 &&
        f2.m_Theta == 0 &&
        f2.m_ThetaVar == 0 &&
        f2.m_Phi == 0 &&
        f2.m_PhiVar == 0)
    {
        return true;
    }

    return f1.m_Dis < f2.m_Dis;
}

void CPlaneDlg::PackNoiseData(TrueDataPacket &packet, NoiseDataPacket &noisePacket)
{
    noisePacket.m_PlaneTrueData = packet.m_PlaneTrueData;
    noisePacket.m_TargetNoiseDatas.clear();
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        NoiseDataFrame frame;
        // Noise data doesn't know the target ID.
        // Possibly the same for time.
        // frame.m_Time = noisePacket.m_PlaneTrueData.m_Time;
        // frame.m_Id = packet.m_TargetTrueDatas[i].m_Id;
        frame.m_Dis = m_Radar.m_TargetDistances[i].back();
        frame.m_DisVar = m_Radar.m_DisVar;
        frame.m_Theta = m_Infrared.m_TargetThetas[i].back();
        frame.m_ThetaVar = m_Infrared.m_ThetaVar;
        frame.m_Phi = m_Infrared.m_TargetPhis[i].back();
        frame.m_PhiVar = m_Infrared.m_PhiVar;
        noisePacket.m_TargetNoiseDatas.push_back(frame);
    }

    sort(noisePacket.m_TargetNoiseDatas.begin(), noisePacket.m_TargetNoiseDatas.end(), &NoiseDataFrameComp);
}

void CPlaneDlg::SendNoiseData(NoiseDataPacket &packet)
{
    m_FusionSocket->SendNoiseData(packet);
}

void CPlaneDlg::AddNoiseData(SocketPacketPair spp)
{
}

void CPlaneDlg::AddControlData(ControlDataPacket &packet)
{
}

void CPlaneDlg::AddControlDataAck(TrueDataFrame &frame)
{
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
    if (m_MatlabDlg)
    {
        m_MatlabDlg->Reset();
    }
    m_DataListDlg.Reset();

    m_StateMapDlg.Reset();
}

void CPlaneDlg::ResetSensors()
{
    m_Radar.Reset();
    m_Esm.Reset();
    m_Infrared.Reset();
    m_DataList.Reset();

    m_StateMap.Reset();

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

    m_StateMap.m_Background = StateMapBackground0;
}

void CPlaneDlg::AddPlane(Plane &plane, Sensor *radar, Sensor *esm, Sensor *infrared)
{
    CString newTitle = m_Title;
    newTitle.AppendFormat(TEXT("%d"), plane.m_Id);
    SetWindowTextW(newTitle);
    CString newSubDlgTitle = newTitle + TEXT("雷达");
    m_RadarDlg.SetWindowTextW(newSubDlgTitle);
    newSubDlgTitle = newTitle + TEXT("ESM");
    m_EsmDlg.SetWindowTextW(newSubDlgTitle);
    newSubDlgTitle = newTitle + TEXT("红外");
    m_InfraredDlg.SetWindowTextW(newSubDlgTitle);
    newSubDlgTitle = newTitle + TEXT("数据列表");
    m_DataListDlg.SetWindowTextW(newSubDlgTitle);
    newSubDlgTitle = newTitle + TEXT("态势");
    m_StateMapDlg.SetWindowTextW(newSubDlgTitle);

    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlane(plane);
    }
    m_StateMap.AddPlane(plane, radar, esm, infrared);
    m_StateMapDlg.AddPlane(plane);
}

void CPlaneDlg::AddOtherPlane(Plane &plane)
{
    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlane(plane);
    }
    m_StateMap.AddPlane(plane, &m_Radar, &m_Esm, &m_Infrared);
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

    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddTarget(target);
    }

    if (m_AddMissile)
    {
        AddMissile(target.m_Id + 100);
    }
}

void CPlaneDlg::AddMissile(int id)
{
    Missile miss;
    miss.m_Id = id;
    miss.m_Color = TargetColorRed;
    m_Missiles.push_back(miss);
    m_StateMap.AddMissile(m_Missiles.back());
    m_StateMapDlg.AddMissile(m_Missiles.back());
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
    else if (subDlg == (void *)&m_StateMapDlg)
    {
        OnLButtonDblClk(0, 0);
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
    wstring hostName = DATA_CENTER_ADDR;
    int port = DATA_CENTER_PORT;

#ifdef _DEV
    if (!m_DataCenterSocket->Connect(hostName.c_str(), port))
    {
        AfxMessageBox(TEXT("连接到数据中心失败."));
        exit(-1);
    }
#else
    wifstream in(ConfigFileName);
    in.imbue(locale("chs"));

    wstring nextLine = TEXT("");

    while(in || nextLine.length() > 0)
    {
        wstring line;
        if(nextLine.length() > 0)
        {
            line = nextLine;  // we read ahead; use it now
            nextLine = L"";
        }
        else
        {
            getline(in, line);
        }

        line = line.substr(0, line.find(TEXT("#")));

        if (line.length() == 0)
        {
            continue;
        }

        wistringstream ist(line);
        wstring key;
        ist >> key;
        if (key == TEXT("IP"))
        {
            wstring ip;
            ist >> ip;
            hostName = ip;
        }
        else if (key == TEXT("PORT"))
        {
            int configPort;
            ist >> configPort;
            port = configPort;
        }
    }

    in.close();
    CIpDlg dlg(hostName.c_str(), port);
    if (IDOK == dlg.DoModal())
    {
        if (!m_DataCenterSocket->Connect(dlg.m_HostName, dlg.m_Port))
        {
            AfxMessageBox(TEXT("连接到数据中心失败."));
            exit(-1);
        }
    }
    else
    {
        exit(-1);
    }
#endif
    m_DataCenterSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    // AfxMessageBox(TEXT("连接到数据中心"));
}

void CPlaneDlg::ConnectFusion(const CString &addr, int port)
{
    while (!m_FusionSocket->Connect(addr, port))
    {
    }
    m_FusionSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    // AfxMessageBox(TEXT("连接到融合机"));
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
    m_Plane = plane;

    AddPlane(m_Plane, &m_Radar, &m_Esm, &m_Infrared);
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
    m_Radar.m_ThetaRangeColor = radar.m_ThetaRangeColor;
    m_Radar.m_ShowHeight = radar.m_ShowHeight;

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
    m_Esm.m_ThetaRangeColor = esm.m_ThetaRangeColor;
    m_Esm.m_ShowHeight = esm.m_ShowHeight;

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
    m_Infrared.m_ThetaRangeColor = infrared.m_ThetaRangeColor;
    m_Infrared.m_ShowHeight = infrared.m_ShowHeight;

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
    m_StateMap.m_ShowHeight = stateMap.m_ShowHeight;

    m_StateMapDlg.m_Ctrl.DrawBackground();
    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetFusionAlgo(FusionAlgo *algo)
{
}

void CPlaneDlg::SetNaviAlgo(NaviAlgo *algo)
{
}

void CPlaneDlg::SetGlobalData(GlobalDataPacket &packet)
{
    m_GlobalData = packet;
    if (m_MatlabDlg)
    {
        m_MatlabDlg->SetSize((m_GlobalData.m_EndTime - m_GlobalData.m_StartTime + 1) / m_GlobalData.m_Interval);
        if (m_ShowDataListDlg)
        {
            m_MatlabDlg->Show();
        }
    }
}

void CPlaneDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_ShowStateMapDlg)
    {
        m_StateMapDlg.ShowWindow(SW_HIDE);
        m_ShowStateMapDlg = false;
    }
    else
    {
        m_StateMapDlg.ShowWindow(SW_SHOW);
        m_ShowStateMapDlg = true;
    }
}
