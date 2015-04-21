
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
    , m_ShowSensor1Dlg(false)
    , m_Sensor1(Sensor::SensorTypeSource, m_Plane, m_GlobalData)
    , m_Sensor1Ctrl(m_Sensor1)
    , m_Sensor1Dlg(TEXT("雷达"), m_Sensor1, this)
    , m_ShowSensor2Dlg(false)
    , m_Sensor2(Sensor::SensorTypeNonSource, m_Plane, m_GlobalData)
    , m_Sensor2Ctrl(m_Sensor2)
    , m_Sensor2Dlg(TEXT("ESM"), m_Sensor2, this)
    , m_ShowInfraredDlg(false)
    , m_Infrared(Sensor::SensorTypeNonSource, m_Plane, m_GlobalData)
    , m_InfraredCtrl(m_Infrared)
    , m_InfraredDlg(TEXT("红外"), m_Infrared, this)
    , m_ShowDataListDlg(false)
    , m_DataList(m_Sensor1, m_Sensor2, m_Infrared)
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

    m_Sensor1Dlg.m_Dlg = this;
    m_Sensor2Dlg.m_Dlg = this;
    m_InfraredDlg.m_Dlg = this;
    m_DataListDlg.m_Dlg = this;

    m_DataCenterSocket = new DataCenterSocket(this);
    m_FusionSocket = new FusionSocket(this);
}

void CPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENSOR1_CTRL, m_Sensor1Ctrl);
    DDX_Control(pDX, IDC_SENSOR2_CTRL, m_Sensor2Ctrl);
    DDX_Control(pDX, IDC_INFRARED_CTRL, m_InfraredCtrl);
    DDX_Control(pDX, IDC_DATALIST_CTRL, m_DataListCtrl);
}

BEGIN_MESSAGE_MAP(CPlaneDlg, CCommonDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_STN_DBLCLK(IDC_SENSOR1_CTRL, &CPlaneDlg::OnStnDblclickSensor1Ctrl)
    ON_STN_DBLCLK(IDC_SENSOR2_CTRL, &CPlaneDlg::OnStnDblclickSensor2Ctrl)
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

    CSensorDlg::CreateDlg(m_Sensor1Dlg);
    if (m_ShowSensor1Dlg)
    {
        m_Sensor1Dlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    CSensorDlg::CreateDlg(m_Sensor2Dlg);
    if (m_ShowSensor2Dlg)
    {
        m_Sensor2Dlg.ShowWindow(SW_SHOW);
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

    // Resize Sensor1.
    int left = rect.left + PAD, width = (rect.right - rect.left) / 2 - PAD * 2, top = rect.top + PAD, height = (rect.bottom - rect.top) / 2 - PAD * 2;
    if (width > height)
    {
        width = height;
    }
    GetDlgItem(IDC_SENSOR1_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD *2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_Sensor1Ctrl.MoveWindow(left, top, width, height);

    // Resize Sensor2..
    left = rect.left + PAD;
    width = (rect.right - rect.left) / 2 - PAD * 2;
    top = (rect.bottom - rect.top) / 2 + PAD;
    height = (rect.bottom - rect.top) / 2 - PAD * 2;
    if (width > height)
    {
        width = height;
    }
    GetDlgItem(IDC_SENSOR2_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD *2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_Sensor2Ctrl.MoveWindow(left, top, width, height);

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

void CPlaneDlg::OnStnDblclickSensor1Ctrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowSensor1Dlg)
    {
        m_Sensor1Dlg.ShowWindow(SW_HIDE);
        m_ShowSensor1Dlg = false;
    }
    else
    {
        m_Sensor1Dlg.ShowWindow(SW_SHOW);
        m_ShowSensor1Dlg = true;
    }
}

void CPlaneDlg::OnStnDblclickSensor2Ctrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowSensor2Dlg)
    {
        m_Sensor2Dlg.ShowWindow(SW_HIDE);
        m_ShowSensor2Dlg = false;
    }
    else
    {
        m_Sensor2Dlg.ShowWindow(SW_SHOW);
        m_ShowSensor2Dlg = true;
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

        m_Sensor1.AddTargetData(0, rel0);
        m_Sensor2.AddTargetData(0, rel0);
        m_Infrared.AddTargetData(0, rel0);
        m_Sensor1.AddTargetData(1, rel1);
        m_Sensor2.AddTargetData(1, rel1);
        m_Infrared.AddTargetData(1, rel1);
        m_StateMap.AddPlaneData(0, m_Plane.m_Position, m_Plane.m_Vel, m_Plane.m_State);
        m_StateMap.AddTargetData(0, m_Targets[0].m_Position, m_Targets[0].m_Vel, m_Targets[0].m_State);
        m_StateMap.AddTargetData(1, m_Targets[1].m_Position, m_Targets[0].m_Vel, m_Targets[1].m_State);

        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();

        m_Sensor1Dlg.m_Ctrl->DrawTargets();
        m_Sensor1Dlg.m_Ctrl->BlendAll();
        m_Sensor1Dlg.m_Ctrl->Invalidate();

        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();

        m_Sensor2Dlg.m_Ctrl->DrawTargets();
        m_Sensor2Dlg.m_Ctrl->BlendAll();
        m_Sensor2Dlg.m_Ctrl->Invalidate();

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
    m_StateMap.AddPlaneData(0, packet.m_PlaneTrueData.m_Pos, packet.m_PlaneTrueData.m_Vel, (TargetState)packet.m_PlaneTrueData.m_State);

    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        if (packet.m_TargetTrueDatas[i].m_State != TargetStateAlive)
        {
            continue;
        }
        Position rel = packet.m_TargetTrueDatas[i].m_Pos - packet.m_PlaneTrueData.m_Pos;

        m_Sensor1.AddTargetData(i, rel);
        m_Sensor2.AddTargetData(i, rel);
        m_Infrared.AddTargetData(i, rel);

        m_DataList.AddTargetData(i, packet.m_TargetTrueDatas[i].m_Time);

        if (m_MatlabDlg)
        {
            m_MatlabDlg->AddTargetTrueData(i, packet.m_TargetTrueDatas[i].m_Pos);
        }

        NoiseDataFrame frame;
        frame.m_Time = packet.m_PlaneTrueData.m_Time;
        frame.m_Id = packet.m_TargetTrueDatas[i].m_Id;
        frame.m_Dis = m_Sensor1.m_TargetDistances[i].back();
        frame.m_Theta = m_Infrared.m_TargetThetas[i].back();
		if (!m_Infrared.IsInRange(i, rel))
		{
			frame.m_Theta = m_Sensor2.m_TargetThetas[i].back();
		}
        frame.m_Phi = m_Infrared.m_TargetPhis[i].back();
        if (!(frame.m_Dis == 0 && frame.m_Theta == 0 && frame.m_Phi == 0))
        {
            Position noiseAbsPos = packet.m_PlaneTrueData.m_Pos + Utility::Rel(frame.m_Dis, frame.m_Theta, frame.m_Phi);
            m_StateMap.AddTargetData(i, noiseAbsPos, /*use true vel*/packet.m_TargetTrueDatas[i].m_Vel, TargetStateAlive);
        }
        else
        {
            m_StateMap.AddTargetData(i, Position(0, 0, 0), Velocity(0, 0, 0), TargetStateDestroyed);
        }
    }

    m_Sensor1Ctrl.DrawTargets();
    m_Sensor1Ctrl.BlendAll();
    m_Sensor1Ctrl.Invalidate();

    m_Sensor1Dlg.m_Ctrl->DrawTargets();
    m_Sensor1Dlg.m_Ctrl->BlendAll();
    m_Sensor1Dlg.m_Ctrl->Invalidate();

    m_Sensor2Ctrl.DrawTargets();
    m_Sensor2Ctrl.BlendAll();
    m_Sensor2Ctrl.Invalidate();

    m_Sensor2Dlg.m_Ctrl->DrawTargets();
    m_Sensor2Dlg.m_Ctrl->BlendAll();
    m_Sensor2Dlg.m_Ctrl->Invalidate();

    m_InfraredCtrl.DrawTargets();
    m_InfraredCtrl.BlendAll();
    m_InfraredCtrl.Invalidate();

    m_InfraredDlg.m_Ctrl->DrawTargets();
    m_InfraredDlg.m_Ctrl->BlendAll();
    m_InfraredDlg.m_Ctrl->Invalidate();

    m_DataListCtrl.AddTargetData();
    m_DataListDlg.m_Ctrl->AddTargetData();

    if (m_MatlabDlg)
    {
        m_MatlabDlg->Update();
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::AddOtherTrueData(int i, TrueDataFrame &frame)
{
    if (m_MatlabDlg && i < PLANE_COUNT - 2 /*忽略攻击机*/)
    {
        m_MatlabDlg->AddPlaneTrueData(i + 1, frame.m_Pos);
        // m_MatlabDlg->Update();
    }

    if (i < PLANE_COUNT - 2 /*忽略攻击机*/)
    {
        m_StateMap.AddPlaneData(i + 1, frame.m_Pos, frame.m_Vel, (TargetState)frame.m_State);
        m_StateMapDlg.m_Ctrl.DrawTargets();
        m_StateMapDlg.m_Ctrl.BlendAll();
        m_StateMapDlg.m_Ctrl.Invalidate();
    }
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
        if (packet.m_TargetTrueDatas[i].m_State == TargetStateAlive)
        {
            frame.m_Time = noisePacket.m_PlaneTrueData.m_Time;
            frame.m_Id = packet.m_TargetTrueDatas[i].m_Id;
            frame.m_Dis = m_Sensor1.m_TargetDistances[i].back();
            frame.m_DisVar = m_Sensor1.m_DisVar;
            frame.m_Theta = m_Infrared.m_TargetThetas[i].back();
            frame.m_ThetaVar = m_Infrared.m_ThetaVar;
            Position rel = packet.m_TargetTrueDatas[i].m_Pos - packet.m_PlaneTrueData.m_Pos;
            if (!m_Infrared.IsInRange(i, rel))
            {
                frame.m_Theta = m_Sensor2.m_TargetThetas[i].back();
                frame.m_ThetaVar = m_Sensor2.m_ThetaVar;
            }
            frame.m_Phi = m_Infrared.m_TargetPhis[i].back();
            frame.m_PhiVar = m_Infrared.m_PhiVar;
        }
        noisePacket.m_TargetNoiseDatas.push_back(frame);
    }

#ifndef _DEV
    sort(noisePacket.m_TargetNoiseDatas.begin(), noisePacket.m_TargetNoiseDatas.end(), &NoiseDataFrameComp);
#endif
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

void CPlaneDlg::AddControlDataAck(ControlDataAckPacket &packet)
{
}

void CPlaneDlg::ResetCtrls()
{
    m_Plane.Reset();

    ResetSensors();

    m_Sensor1Ctrl.Reset();
    m_Sensor2Ctrl.Reset();
    m_InfraredCtrl.Reset();
    m_DataListCtrl.Reset();

    m_Sensor1Dlg.Reset();
    m_Sensor2Dlg.Reset();
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
    m_Sensor1.Reset();
    m_Sensor2.Reset();
    m_Infrared.Reset();
    m_DataList.Reset();

    m_StateMap.Reset();

    m_Sensor2.m_MaxDis = 250;
    m_Sensor2.m_MaxTheta = 90;
    m_Infrared.m_MaxDis = 350;
    m_Infrared.m_MaxTheta = 60;

    m_Sensor2.m_ThetaRangeColor = Color::Red;
    m_Sensor2.m_ShowHeight = FALSE;
    m_Infrared.m_ShowScanline = FALSE;
    m_Infrared.m_ShowThetaRange = FALSE;
    m_Infrared.m_ThetaRangeColor = Color::Yellow;
    m_Infrared.m_ShowHeight = FALSE;

    m_StateMap.m_Background = StateMapBackground0;
    m_StateMap.m_ExplosionType = ExplosionType0;
}

void CPlaneDlg::AddPlane(Plane &plane, Sensor *sensor1, Sensor *sensor2, Sensor *infrared)
{
    CString newTitle = m_Title;
    newTitle.AppendFormat(TEXT("%d"), plane.m_Id);
    SetWindowTextW(newTitle);
    CString newSubDlgTitle = newTitle + TEXT("雷达");
    m_Sensor1Dlg.SetWindowTextW(newSubDlgTitle);
    newSubDlgTitle = newTitle + TEXT("ESM");
    m_Sensor2Dlg.SetWindowTextW(newSubDlgTitle);
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
    m_StateMap.AddPlane(plane, sensor1, sensor2, infrared);
    m_StateMapDlg.AddPlane(plane);
}

void CPlaneDlg::AddOtherPlane(Plane &plane)
{
    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlane(plane);
    }
    m_StateMap.AddPlane(plane, &m_Sensor1, &m_Sensor2, &m_Infrared);
    m_StateMapDlg.AddPlane(plane);
}

void CPlaneDlg::AddTarget(Target &target)
{
    m_Targets.push_back(target);

    m_Sensor1.AddTarget(target);
    m_Sensor2.AddTarget(target);
    m_Infrared.AddTarget(target);
    m_DataList.AddTarget(target);
    m_StateMap.AddTarget(target);

    m_Sensor1Ctrl.AddTarget(target);
    m_Sensor2Ctrl.AddTarget(target);
    m_InfraredCtrl.AddTarget(target);
    m_DataListCtrl.AddTarget(target);

    m_Sensor1Dlg.AddTarget(target);
    m_Sensor2Dlg.AddTarget(target);
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
    miss.m_Type = TargetTypeMissile;
    miss.m_Color = TargetColorRed;
    m_Missiles.push_back(miss);
    m_StateMap.AddMissile(m_Missiles.back());
    m_StateMapDlg.AddMissile(m_Missiles.back());
}

void CPlaneDlg::OnSubDlgClose(void *subDlg)
{
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        OnStnDblclickSensor1Ctrl();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        OnStnDblclickSensor2Ctrl();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_InfraredDlg)
    {
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::OnSubDlgShowScanline(void *subDlg)
{
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
    }
}

void CPlaneDlg::OnSubDlgShowTrack(void *subDlg)
{
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawThetaRange();
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawThetaRange();
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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
    if (subDlg == (void *)&m_Sensor1Dlg)
    {
        m_Sensor1Ctrl.DrawTargets();
        m_Sensor1Ctrl.BlendAll();
        m_Sensor1Ctrl.Invalidate();
    }
    else if (subDlg == (void *)&m_Sensor2Dlg)
    {
        m_Sensor2Ctrl.DrawTargets();
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
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

    AddPlane(m_Plane, &m_Sensor1, &m_Sensor2, &m_Infrared);
}

void CPlaneDlg::SetSensor1(Sensor &sensor1)
{
    m_Sensor1.m_Type = sensor1.m_Type;
    m_Sensor1.m_Enable = sensor1.m_Enable;
    m_Sensor1.m_MaxDis = sensor1.m_MaxDis;
    m_Sensor1.m_MaxTheta = sensor1.m_MaxTheta;
    m_Sensor1.m_MaxPhi = sensor1.m_MaxPhi;
    m_Sensor1.m_DisVar = sensor1.m_DisVar;
    m_Sensor1.m_ThetaVar = sensor1.m_ThetaVar;
    m_Sensor1.m_PhiVar = sensor1.m_PhiVar;
    m_Sensor1.m_ProDet = sensor1.m_ProDet;
    m_Sensor1.m_ThetaRangeColor = sensor1.m_ThetaRangeColor;
    m_Sensor1.m_ShowHeight = sensor1.m_ShowHeight;

    m_Sensor1Ctrl.DrawThetaRange();
    m_Sensor1Ctrl.DrawTargets();
    m_Sensor1Ctrl.BlendAll();
    m_Sensor1Ctrl.Invalidate();

    m_Sensor1Dlg.m_Ctrl->DrawThetaRange();
    m_Sensor1Dlg.m_Ctrl->DrawTargets();
    m_Sensor1Dlg.m_Ctrl->BlendAll();
    m_Sensor1Dlg.m_Ctrl->Invalidate();

    m_Sensor1Dlg.UpdateData(FALSE);

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetSensor2(Sensor &sensor2)
{
    m_Sensor2.m_Type = sensor2.m_Type;
    m_Sensor2.m_Enable = sensor2.m_Enable;
    m_Sensor2.m_MaxDis = sensor2.m_MaxDis;
    m_Sensor2.m_MaxTheta = sensor2.m_MaxTheta;
    m_Sensor2.m_MaxPhi = sensor2.m_MaxPhi;
    m_Sensor2.m_DisVar = sensor2.m_DisVar;
    m_Sensor2.m_ThetaVar = sensor2.m_ThetaVar;
    m_Sensor2.m_PhiVar = sensor2.m_PhiVar;
    m_Sensor2.m_ProDet = sensor2.m_ProDet;
    m_Sensor2.m_ThetaRangeColor = sensor2.m_ThetaRangeColor;
    m_Sensor2.m_ShowHeight = sensor2.m_ShowHeight;

    m_Sensor2Ctrl.DrawThetaRange();
    m_Sensor2Ctrl.DrawTargets();
    m_Sensor2Ctrl.BlendAll();
    m_Sensor2Ctrl.Invalidate();

    m_Sensor2Dlg.m_Ctrl->DrawThetaRange();
    m_Sensor2Dlg.m_Ctrl->DrawTargets();
    m_Sensor2Dlg.m_Ctrl->BlendAll();
    m_Sensor2Dlg.m_Ctrl->Invalidate();

    m_Sensor2Dlg.UpdateData(FALSE);

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CPlaneDlg::SetStateMap(StateMap &stateMap)
{
    m_StateMap.m_Background = stateMap.m_Background;
    m_StateMap.m_ExplosionType = stateMap.m_ExplosionType;
    m_StateMapDlg.m_Background.SetCurSel(m_StateMap.m_Background);
    m_StateMapDlg.m_ExplosionType.SetCurSel(m_StateMap.m_ExplosionType);

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
