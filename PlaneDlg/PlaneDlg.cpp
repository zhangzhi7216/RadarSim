
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

CPlaneDlg::CPlaneDlg(PacketType planeType
                     , LPCWSTR title
                     , bool hasSensor1
                     , CString sensor1Title
                     , bool hasSensor2
                     , CString sensor2Title
                     , bool hasStateMap
                     , bool hasDataList
                     , CWnd* pParent /*=NULL*/)
    : CCommonDlg(CPlaneDlg::IDD, pParent)
    , m_PlaneType(planeType)
    , m_Title(title)
    , m_Initialized(false)
    , m_HasSensor1(hasSensor1)
    , m_ShowSensor1Dlg(false)
    , m_Sensor1Title(sensor1Title)
    , m_Sensor1(SensorIdRadar, m_Plane, m_GlobalData)
    , m_Sensor1Ctrl(m_Sensor1)
    , m_Sensor1Dlg(m_Sensor1Title, m_Sensor1, this)
    , m_HasSensor2(hasSensor2)
    , m_ShowSensor2Dlg(false)
    , m_Sensor2Title(sensor2Title)
    , m_Sensor2(SensorIdEsm, m_Plane, m_GlobalData)
    , m_Sensor2Ctrl(m_Sensor2)
    , m_Sensor2Dlg(m_Sensor2Title, m_Sensor2, this)
    , m_HasStateMap(hasStateMap)
    , m_ShowStateMapDlg(false)
    , m_StateMapCtrl(m_StateMap)
    , m_StateMapDlg(TEXT("态势"), m_StateMap, this)
    , m_HasDataList(hasDataList)
    , m_ShowDataListDlg(false)
    , m_DataList(m_Sensor1, TEXT(""), m_Sensor2, TEXT(""))
    , m_DataListCtrl(m_DataList)
    , m_DataListDlg(TEXT("数据列表"), m_DataList, this)
    , m_ShowZoomDlg(false)
    , m_ZoomDlg(this)
    , m_DataCenterSocket(0)
    , m_FusionSocket(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_Sensor1Dlg.m_Dlg = this;
    m_Sensor2Dlg.m_Dlg = this;
    m_StateMapDlg.m_Dlg = this;
    m_DataListDlg.m_Dlg = this;
}

void CPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENSOR1_CTRL, m_Sensor1Ctrl);
    DDX_Control(pDX, IDC_SENSOR2_CTRL, m_Sensor2Ctrl);
    DDX_Control(pDX, IDC_STATEMAP_CTRL, m_StateMapCtrl);
    DDX_Control(pDX, IDC_DATALIST_CTRL, m_DataListCtrl);
}

BEGIN_MESSAGE_MAP(CPlaneDlg, CCommonDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_STN_DBLCLK(IDC_SENSOR1_CTRL, &CPlaneDlg::OnStnDblclickSensor1Ctrl)
    ON_STN_DBLCLK(IDC_SENSOR2_CTRL, &CPlaneDlg::OnStnDblclickSensor2Ctrl)
    ON_STN_DBLCLK(IDC_STATEMAP_CTRL, &CPlaneDlg::OnStnDblclickStateMapCtrl)
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

    GetDlgItem(IDC_FUSION_ALGO_STATIC)->ShowWindow(FALSE);
    GetDlgItem(IDC_FUSION_ALGO)->ShowWindow(FALSE);

    GetDlgItem(IDC_NAVI_ALGO_STATIC)->ShowWindow(FALSE);
    GetDlgItem(IDC_NAVI_ALGO)->ShowWindow(FALSE);

    Resize();

    CreateDataCenterSocket();
    m_FusionSocket = new PlaneSocket(this);

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

    CZoomDlg::CreateDlg(m_ZoomDlg);
    if (m_ShowZoomDlg)
    {
        m_ZoomDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

    ResetCtrls();
    ResetSockets();

    ConnectDataCenter();
    SendPlaneType();

    if (m_ShowStateMapDlg)
    {
        m_StateMapDlg.ShowWindow(SW_SHOW);
    }
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
    }

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

    int left = rect.left + PAD, width = (rect.right - rect.left) / 2 - PAD * 2, top = rect.top + PAD, height = 0;
    if (m_HasStateMap)
    {
        // Resize StateMap..
        height = rect.bottom - rect.top - PAD * 2;
        if (width > height)
        {
            width = height;
        }
        GetDlgItem(IDC_STATEMAP_CTRL_GRP)->ShowWindow(TRUE);
        GetDlgItem(IDC_STATEMAP_CTRL_GRP)->MoveWindow(left, top, width, height);
        
        left = left + PAD;
        width = width - PAD *2;
        top = top + PAD * 2;
        height = height - PAD * 3;
        m_StateMapCtrl.ShowWindow(TRUE);
        m_StateMapCtrl.MoveWindow(left, top, width, height);
    }
    else
    {
        // Resize Sensor1.
        height = (rect.bottom - rect.top) / 2 - PAD * 2;
        if (width > height)
        {
            width = height;
        }
        GetDlgItem(IDC_SENSOR1_CTRL_GRP)->SetWindowTextW(m_Sensor1Title);
        GetDlgItem(IDC_SENSOR1_CTRL_GRP)->ShowWindow(TRUE);
        GetDlgItem(IDC_SENSOR1_CTRL_GRP)->MoveWindow(left, top, width, height);

        left = left + PAD;
        width = width - PAD *2;
        top = top + PAD * 2;
        height = height - PAD * 3;
        m_Sensor1Ctrl.ShowWindow(TRUE);
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
        GetDlgItem(IDC_SENSOR2_CTRL_GRP)->SetWindowTextW(m_Sensor2Title);
        GetDlgItem(IDC_SENSOR2_CTRL_GRP)->ShowWindow(TRUE);
        GetDlgItem(IDC_SENSOR2_CTRL_GRP)->MoveWindow(left, top, width, height);

        left = left + PAD;
        width = width - PAD *2;
        top = top + PAD * 2;
        height = height - PAD * 3;
        m_Sensor2Ctrl.ShowWindow(TRUE);
        m_Sensor2Ctrl.MoveWindow(left, top, width, height);
    }

    // Resize DataList.
    left = left + width + PAD + PAD * 2;
    width = rect.right - PAD - left;
    top = rect.top + PAD;
    height = rect.bottom - rect.top - PAD * 2;
    GetDlgItem(IDC_DATALIST_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    top = top + PAD * 2;
    width = width - PAD * 2;
    height = height - PAD * 3;

    m_DataListCtrl.MoveWindow(left, top, width, height);
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

void CPlaneDlg::OnStnDblclickStateMapCtrl()
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

void CPlaneDlg::OnCloseZoomDlg()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_ZoomDlg.ShowWindow(SW_HIDE);
    m_ShowZoomDlg = false;
    m_StateMap.m_ZoomKeyTargetId = -1;
}

void CPlaneDlg::AddTrueData(TrueDataPacket &packet)
{
    m_Plane.MoveTo(packet.m_PlaneTrueData.m_Pos);

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
        if (!m_HasStateMap)
        {
            m_DataList.AddTargetData(i, packet.m_TargetTrueDatas[i].m_Time);
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

    if (!m_HasStateMap)
    {
        m_DataListCtrl.AddTargetData();
        m_DataListDlg.m_Ctrl->AddTargetData();
    }

    if (m_HasStateMap)
    {
        m_StateMapCtrl.DrawTargets();
        m_StateMapCtrl.BlendAll();
        m_StateMapCtrl.Invalidate();

        m_StateMapDlg.m_Ctrl->DrawTargets();
        m_StateMapDlg.m_Ctrl->BlendAll();
        m_StateMapDlg.m_Ctrl->Invalidate();
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

void CPlaneDlg::SendNoiseDatas(TrueDataPacket &packet)
{
    NoiseDataPacket p1 = m_Sensor1.GetNoiseData(packet);
    NoiseDataPacket p2 = m_Sensor2.GetNoiseData(packet);
    m_FusionSocket->SendNoiseData(p1);
    m_FusionSocket->SendNoiseData(p2);
}

void CPlaneDlg::ResetCtrls()
{
    m_Plane.Reset();

    ResetSensors();

    m_Sensor1Ctrl.Reset();
    m_Sensor2Ctrl.Reset();
    m_DataListCtrl.Reset();
    m_StateMapCtrl.Reset();

    m_Sensor1Dlg.Reset();
    m_Sensor2Dlg.Reset();
    m_DataListDlg.Reset();
    m_StateMapDlg.Reset();
}

void CPlaneDlg::ResetSensors()
{
    m_Sensor1.Reset();
    m_Sensor2.Reset();
    m_DataList.Reset();

    m_StateMap.Reset();

    m_Sensor2.m_MaxDis = 250;
    m_Sensor2.m_MaxTheta = 90;

    m_Sensor2.m_ThetaRangeColor = Color::Red;
    m_Sensor2.m_ShowHeight = FALSE;

    m_StateMap.m_Background = StateMapBackground0;
    m_StateMap.m_ExplosionType = ExplosionType0;
}

void CPlaneDlg::AddPlane(Plane &plane)
{
    m_StateMap.AddPlane(plane);
    m_StateMapDlg.AddPlane(plane);
}

void CPlaneDlg::AddTarget(Target &target)
{
    m_Targets.push_back(target);

    m_Sensor1.AddTarget(target);
    m_Sensor2.AddTarget(target);
    if (!m_HasStateMap)
    {
        m_DataList.AddTarget(target);
    }

    m_Sensor1Dlg.AddTarget(target);
    m_Sensor2Dlg.AddTarget(target);
    if (!m_HasStateMap)
    {
        m_DataListDlg.AddTarget(target);
    }
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
    else if (subDlg == (void *)&m_DataListDlg)
    {
        OnNMDblclkDatalistCtrl(0, 0);
    }
    else if (subDlg == (void *)&m_StateMapDlg)
    {
        OnStnDblclickStateMapCtrl();
    }
    else if (subDlg == (void *)&m_ZoomDlg)
    {
        OnCloseZoomDlg();
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
        m_Sensor2Ctrl.BlendAll();
        m_Sensor2Ctrl.Invalidate();
    }

    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
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

    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
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

    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
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
}

void CPlaneDlg::OnSubDlgStateMapBackgroundChange(void *subDlg)
{
    m_StateMapCtrl.DrawBackground();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
}

void CPlaneDlg::OnSubDlgStateMapTargetsChange(void *subDlg)
{
    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
}

void CPlaneDlg::CreateDataCenterSocket()
{
    m_DataCenterSocket = new PlaneSocket(this);
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
        if (key == TEXT("DATA_CENTER_IP"))
        {
            wstring ip;
            ist >> ip;
            hostName = ip;
        }
        else if (key == TEXT("DATA_CENTER_PORT"))
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
    m_DataCenterSocket->AsyncSelect(FD_READ);
    // m_DataCenterSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    // AfxMessageBox(TEXT("连接到数据中心"));
}

void CPlaneDlg::SendPlaneType()
{
    if (m_PlaneType == PacketTypeImRadar)
    {
        m_DataCenterSocket->SendImRadar();
    }
    else if (m_PlaneType == PacketTypeImDetect)
    {
        m_DataCenterSocket->SendImDetect();
    }
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

void CPlaneDlg::SetPlane(Plane &plane)
{
    m_Plane = plane;

    AddPlane(m_Plane);
}

void CPlaneDlg::SetSensor1(Sensor &sensor1)
{
    m_Sensor1 = sensor1;

    m_Sensor1Ctrl.DrawThetaRange();
    m_Sensor1Ctrl.DrawTargets();
    m_Sensor1Ctrl.BlendAll();
    m_Sensor1Ctrl.Invalidate();

    m_Sensor1Dlg.m_Ctrl->DrawThetaRange();
    m_Sensor1Dlg.m_Ctrl->DrawTargets();
    m_Sensor1Dlg.m_Ctrl->BlendAll();
    m_Sensor1Dlg.m_Ctrl->Invalidate();

    m_Sensor1Dlg.UpdateData(FALSE);

    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
}

void CPlaneDlg::SetSensor2(Sensor &sensor2)
{
    m_Sensor2 = sensor2;

    m_Sensor2Ctrl.DrawThetaRange();
    m_Sensor2Ctrl.DrawTargets();
    m_Sensor2Ctrl.BlendAll();
    m_Sensor2Ctrl.Invalidate();

    m_Sensor2Dlg.m_Ctrl->DrawThetaRange();
    m_Sensor2Dlg.m_Ctrl->DrawTargets();
    m_Sensor2Dlg.m_Ctrl->BlendAll();
    m_Sensor2Dlg.m_Ctrl->Invalidate();

    m_Sensor2Dlg.UpdateData(FALSE);

    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
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

    m_StateMapCtrl.DrawBackground();
    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawBackground();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();
}

void CPlaneDlg::SetGlobalData(GlobalDataPacket &packet)
{
    m_GlobalData = packet;
}