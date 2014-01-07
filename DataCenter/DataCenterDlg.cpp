
// DataCenterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataCenter.h"
#include "DataCenterDlg.h"

#include "DataCenterSocket.h"
#include "PlaneSocket.h"

#include <algorithm>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"
#include "FusionMatlabAlgo.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataCenterDlg 对话框


#define TIMES 10
#define TIME_FRAMES 100
#define WM_TIME_FRAME (WM_USER + 1)


CDataCenterDlg::CDataCenterDlg(CWnd* pParent /*=NULL*/)
	: CCommonDlg(CDataCenterDlg::IDD, pParent)
    , m_FusionConnected(false)
    , m_ConnectedPlanes(0)
    , m_ShowStateMapDlg(true)
    , m_ShowMatlabDlg(false)
    , m_StateMapDlg(TEXT("态势"), m_StateMap, this)
    , m_CurrentFrame(0)
    , m_CurrentRound(0)
    , m_SensorEnable(FALSE)
    , m_SensorMaxDis(0)
    , m_SensorMaxTheta(0)
    , m_SensorMaxPhi(0)
    , m_SensorDisVar(0)
    , m_SensorThetaVar(0)
    , m_SensorPhiVar(0)
    , m_SensorProDet(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_DataCenterSocket = new DataCenterSocket(this);

    m_Sensors[SensorIdRadar].m_Type = Sensor::SensorTypeSource;
    m_Sensors[SensorIdRadar].m_MaxDis = 350;
    m_Sensors[SensorIdRadar].m_MaxTheta = 120;

    m_Sensors[SensorIdEsm].m_Type = Sensor::SensorTypeNonSource;
    m_Sensors[SensorIdEsm].m_MaxDis = 300;
    m_Sensors[SensorIdEsm].m_MaxTheta = 90;
    m_Sensors[SensorIdEsm].m_ThetaRangeColor = Color::Red;
    m_Sensors[SensorIdEsm].m_ShowHeight = FALSE;

    m_Sensors[SensorIdInfrared].m_Type = Sensor::SensorTypeNonSource;
    m_Sensors[SensorIdInfrared].m_MaxDis = 250;
    m_Sensors[SensorIdInfrared].m_MaxTheta = 60;
    m_Sensors[SensorIdInfrared].m_ShowScanline = FALSE;
    m_Sensors[SensorIdInfrared].m_ShowThetaRange = FALSE;
    m_Sensors[SensorIdInfrared].m_ThetaRangeColor = Color::Yellow;
    m_Sensors[SensorIdInfrared].m_ShowHeight = FALSE;
}

CDataCenterDlg::~CDataCenterDlg()
{
    for (int i = 0; i < m_FusionAlgos.size(); ++i)
    {
        delete m_FusionAlgos[i];
    }
    for (int i = 0; i < m_NaviAlgos.size(); ++i)
    {
        delete m_NaviAlgos[i];
    }
}

void CDataCenterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_DC_GLOBAL_ROUNDS, m_GlobalData.m_Rounds);
    DDX_Text(pDX, IDC_DC_GLOBAL_CYCLE, m_GlobalData.m_Interval);
    DDX_Text(pDX, IDC_DC_GLOBAL_START_TIME, m_GlobalData.m_StartTime);
    DDX_Text(pDX, IDC_DC_GLOBAL_END_TIME, m_GlobalData.m_EndTime);
    DDX_Control(pDX, IDC_DC_GLOBAL_NOISE, m_NoiseType);
    DDX_Control(pDX, IDC_DC_STATE_MAP_BKG, m_StateMapBkg);
    DDX_Text(pDX, IDC_DC_STATE_MAP_MAX_X, m_StateMap.m_MaxX);
    DDX_Text(pDX, IDC_DC_STATE_MAP_MAX_Y, m_StateMap.m_MaxY);
    DDX_Control(pDX, IDC_DC_GLOBAL_FUSION_ALGO, m_FusionAlgoSel);
    DDX_Control(pDX, IDC_DC_GLOBAL_NAVI_ALGO, m_NaviAlgoSel);
    DDX_Check(pDX, IDC_GLOBAL_WILD_VALUE, m_GlobalData.m_WildValue);
    DDX_Check(pDX, IDC_GLOBAL_DELAY, m_GlobalData.m_Delay);
    DDX_Control(pDX, IDC_DC_SENSOR_ID, m_SensorIdSel);
    DDX_Check(pDX, IDC_DC_SENSOR_ENABLE, m_SensorEnable);
    DDX_Text(pDX, IDC_DC_SENSOR_MAX_DIS, m_SensorMaxDis);
    DDX_Text(pDX, IDC_DC_SENSOR_MAX_THETA, m_SensorMaxTheta);
    DDX_Text(pDX, IDC_DC_SENSOR_MAX_PHI, m_SensorMaxPhi);
    DDX_Text(pDX, IDC_DC_SENSOR_DIS_VAR, m_SensorDisVar);
    DDX_Text(pDX, IDC_DC_SENSOR_THETA_VAR, m_SensorThetaVar);
    DDX_Text(pDX, IDC_DC_SENSOR_PHI_VAR, m_SensorPhiVar);
    DDX_Text(pDX, IDC_DC_SENSOR_PRO_DET, m_SensorProDet);
}

BEGIN_MESSAGE_MAP(CDataCenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CDataCenterDlg::OnBnClickedOk)
    ON_WM_TIMER()
    ON_WM_MBUTTONDBLCLK()
    ON_CBN_SELCHANGE(IDC_DC_GLOBAL_NOISE, &CDataCenterDlg::OnCbnSelchangeDcGlobalNoise)
    ON_CBN_SELCHANGE(IDC_DC_STATE_MAP_BKG, &CDataCenterDlg::OnCbnSelchangeDcStateMapBkg)
    ON_BN_CLICKED(IDC_STATE_MAP_DLG_BUTTON, &CDataCenterDlg::OnBnClickedStateMapDlgButton)
    ON_BN_CLICKED(IDC_MATLAB_DLG_BUTTON, &CDataCenterDlg::OnBnClickedMatlabDlgButton)
    ON_CBN_SELCHANGE(IDC_DC_SENSOR_ID, &CDataCenterDlg::OnCbnSelchangeDcSensorId)
    ON_BN_CLICKED(IDC_DC_SENSOR_ENABLE, &CDataCenterDlg::OnBnClickedDcSensorEnable)
    ON_EN_CHANGE(IDC_DC_GLOBAL_ROUNDS, &CDataCenterDlg::OnEnChangeDcGlobalRounds)
    ON_EN_CHANGE(IDC_DC_GLOBAL_CYCLE, &CDataCenterDlg::OnEnChangeDcGlobalCycle)
    ON_EN_CHANGE(IDC_DC_GLOBAL_START_TIME, &CDataCenterDlg::OnEnChangeDcGlobalStartTime)
    ON_EN_CHANGE(IDC_DC_GLOBAL_END_TIME, &CDataCenterDlg::OnEnChangeDcGlobalEndTime)
    ON_BN_CLICKED(IDC_GLOBAL_WILD_VALUE, &CDataCenterDlg::OnBnClickedGlobalWildValue)
    ON_BN_CLICKED(IDC_GLOBAL_DELAY, &CDataCenterDlg::OnBnClickedGlobalDelay)
    ON_EN_CHANGE(IDC_DC_STATE_MAP_MAX_X, &CDataCenterDlg::OnEnChangeDcStateMapMaxX)
    ON_EN_CHANGE(IDC_DC_STATE_MAP_MAX_Y, &CDataCenterDlg::OnEnChangeDcStateMapMaxY)
    ON_EN_CHANGE(IDC_DC_SENSOR_MAX_DIS, &CDataCenterDlg::OnEnChangeDcSensorMaxDis)
    ON_EN_CHANGE(IDC_DC_SENSOR_MAX_THETA, &CDataCenterDlg::OnEnChangeDcSensorMaxTheta)
    ON_EN_CHANGE(IDC_DC_SENSOR_MAX_PHI, &CDataCenterDlg::OnEnChangeDcSensorMaxPhi)
    ON_EN_CHANGE(IDC_DC_SENSOR_DIS_VAR, &CDataCenterDlg::OnEnChangeDcSensorDisVar)
    ON_EN_CHANGE(IDC_DC_SENSOR_THETA_VAR, &CDataCenterDlg::OnEnChangeDcSensorThetaVar)
    ON_EN_CHANGE(IDC_DC_SENSOR_PHI_VAR, &CDataCenterDlg::OnEnChangeDcSensorPhiVar)
    ON_EN_CHANGE(IDC_DC_SENSOR_PRO_DET, &CDataCenterDlg::OnEnChangeDcSensorProDet)
END_MESSAGE_MAP()


// CDataCenterDlg 消息处理程序

BOOL CDataCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    for (int i = SensorIdRadar; i < SensorIdLast; ++i)
    {
        m_SensorIdSel.InsertString(i, SensorIdNames[i]);
    }
    m_SensorIdSel.SetCurSel(0);
    OnCbnSelchangeDcSensorId();

    ReadConfigFile();

    if (m_FusionAlgos.size() == 0)
    {
        AfxMessageBox(TEXT("未能读取任何融合算法."));
        exit(-1);
    }
    for (int i = 0; i < m_FusionAlgos.size(); ++i)
    {
        m_FusionAlgoSel.InsertString(i, m_FusionAlgos[i]->m_Name);
    }
    m_FusionAlgoSel.SetCurSel(0);

    // FIXME
    if (0 && m_NaviAlgos.size() == 0)
    {
        AfxMessageBox(TEXT("未能读取任何导航算法."));
        exit(-1);
    }
    for (int i = 0; i < m_NaviAlgos.size(); ++i)
    {
        m_NaviAlgoSel.InsertString(i, m_NaviAlgos[i]->m_Name);
    }
    m_NaviAlgoSel.SetCurSel(0);

    for (int i = 0; i < StateMapBackgroundLast; ++i)
    {
        m_StateMapBkg.InsertString(i, StateMapBackgroundNames[i]);
    }
    m_StateMapBkg.SetCurSel(m_StateMap.m_Background);

    for (int i = 0; i < NoiseTypeLast; ++i)
    {
        m_NoiseType.InsertString(i, NoiseTypeNames[i]);
    }
    m_NoiseType.SetCurSel(m_GlobalData.m_NoiseType);

    // TODO: 在此添加额外的初始化代码
    if (!m_DataCenterSocket->Create(DATA_CENTER_PORT))
    {
        AfxMessageBox(TEXT("套接字创建失败."));
        exit(-1);
    }
    if (!m_DataCenterSocket->Listen())
    {
        AfxMessageBox(TEXT("监听失败."));
        exit(-1);
    }
    if (!m_DataCenterSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("选择失败."));
        exit(-1);
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataCenterDlg::OnPaint()
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
HCURSOR CDataCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataCenterDlg::OnSubDlgClose(void *subDlg)
{
    /*
    if (subDlg == (void *)&m_DataListDlg)
    {
        OnNMDblclkDatalistCtrl(0, 0);
    }
    */
    if (subDlg == (void *)&m_StateMapDlg)
    {
        OnLButtonDblClk(0, 0);
    }
}

void CDataCenterDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    PlaneSocket *socket = new PlaneSocket(this);
    if (m_DataCenterSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }

    int i;
    for (i = 0; i < PLANE_COUNT; ++i)
    {
        if (m_PlaneClients[i].m_PlaneSocket == NULL)
        {
            m_PlaneClients[i].m_PlaneSocket = socket;
            break;
        }
    }
    if (i == PLANE_COUNT)
    {
        AfxMessageBox(TEXT("连接已满"));
        socket->Close();
        delete socket;
        return;
    }
    m_ConnectedPlanes++;
    if (m_FusionConnected)
    {
        socket->SendFusionAddr(m_FusionAddr, m_FusionPort);
    }
    m_Lock.Unlock();
    if (m_FusionConnected && m_ConnectedPlanes == PLANE_COUNT)
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
}

void CDataCenterDlg::SetFusionAddr(const CString &addr, int port)
{
    m_FusionAddr = addr;
    m_FusionPort = port;
    m_FusionConnected = true;

    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        if (m_PlaneClients[i].m_PlaneSocket)
        {
            m_PlaneClients[i].m_PlaneSocket->SendFusionAddr(m_FusionAddr, m_FusionPort);
        }
    }
    if (m_FusionConnected && m_ConnectedPlanes == PLANE_COUNT)
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
}

void CDataCenterDlg::ResetSockets()
{
    m_Lock.Lock();
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        if (m_PlaneClients[i].m_PlaneSocket)
        {
            m_PlaneClients[i].m_PlaneSocket->Close();
            delete m_PlaneClients[i].m_PlaneSocket;
            m_PlaneClients[i].m_PlaneSocket = NULL;
        }
    }

    m_FusionConnected = false;
    m_ConnectedPlanes = 0;
    m_Lock.Unlock();

    GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void CDataCenterDlg::ResetCtrls()
{
    if (m_FusionConnected && m_ConnectedPlanes != PLANE_COUNT)
    {
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }

    // m_DataList.Reset();
    // m_DataListCtrl.Reset();
    // m_DataListDlg.Reset();

    m_StateMap.Reset();
    m_StateMapDlg.Reset();
    m_MatlabDlg.Reset();
}

void CDataCenterDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    // OnOK();
    m_CurrentRound = 0;
    UpdateData();
    StartSim();
}

void CDataCenterDlg::GeneratePlaneClients()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneClients[i].m_Plane.m_Type = (TargetType)i;
        m_PlaneClients[i].m_Plane.m_MoveType = (TargetMoveType)(i % 2);
        m_PlaneClients[i].m_Plane.m_Position = Position(100, 10 + 200 * i, 100);
        // m_PlaneClients[i].m_Plane.m_Vel = Position(i + 1, ((double)rand() - (double)RAND_MAX / 2) / ((double)RAND_MAX / 2) * 2, rand() % 3);
        m_PlaneClients[i].m_Plane.m_Vel = Position(i + 1, 0, rand() % 3);
        m_PlaneClients[i].m_Plane.m_Acc = Position(rand() % 1, rand() % 1, rand() % 1);
        m_PlaneClients[i].m_Plane.m_Color = (TargetColor)i;
    }
}

void CDataCenterDlg::GenerateTargetClients()
{
    m_TargetClients.clear();
    for (int i = 0; i < TARGET_COUNT; ++i)
    {
        TargetClient client;
        client.m_Target.m_Type = (TargetType)((int)TargetTypeShipboard + rand() % ((int)TargetTypeMissile - (int)TargetTypeShipboard + 1));
        client.m_Target.m_MoveType = TargetMoveTypeUniVel;
        client.m_Target.m_Position = Position(130, 150 + 150 * i, 150) + Position(10 * (rand() % 3), 10 * (rand() % 3), 10 * (rand() % 3));
        client.m_Target.m_Vel = Position(i + 1, 0, rand() % 3);
        client.m_Target.m_Color = (TargetColor)(i + PLANE_COUNT);
        m_TargetClients.push_back(client);
    }
}

void CDataCenterDlg::GenerateTrueData()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneClients[i].m_PlaneTrueDatas.clear();
    }
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_TargetClients[i].m_TargetTrueDatas.clear();
    }
    for (int i = m_GlobalData.m_StartTime; i < m_GlobalData.m_EndTime + 1; i += m_GlobalData.m_Interval)
    {
        for (int j = 0; j < PLANE_COUNT; ++j)
        {
            m_PlaneClients[j].m_Plane.Move(m_GlobalData.m_Interval);
            // m_Plane.m_Position = m_PlaneClients[j].m_Plane.m_Position + Position(rand() % 3, (double)rand() / (double)RAND_MAX * cos(j * 3.1415926), rand() % 2);
            // m_PlaneClients[j].m_Plane.m_Position = m_PlaneClients[j].m_Plane.m_Position + Position(3, 3, 3);
            TrueDataFrame frame;
            frame.m_Time = i;
            frame.m_Id = m_PlaneClients[j].m_Plane.m_Id;
            frame.m_Pos = m_PlaneClients[j].m_Plane.m_Position;
            m_PlaneClients[j].m_PlaneTrueDatas.push_back(frame);
        }
        for (int j = 0; j < m_TargetClients.size(); ++j)
        {
            m_TargetClients[j].m_Target.Move(m_GlobalData.m_Interval);
            // m_TargetClients[j].m_Target.m_Position = m_TargetClients[j].m_Target.m_Position + Position(3, 0, 0);
            TrueDataFrame frame;
            frame.m_Time = i;
            frame.m_Id = m_TargetClients[j].m_Target.m_Id;
            frame.m_Pos = m_TargetClients[j].m_Target.m_Position;
            m_TargetClients[j].m_TargetTrueDatas.push_back(frame);
        }
    }
}

void CDataCenterDlg::GenerateGlobalData()
{
}

void CDataCenterDlg::AddFusionData(FusionDataPacket &packet)
{
    m_FusionDatas.push_back(packet);
    ResumeSim();
}

void CDataCenterDlg::StartSim()
{
    m_CurrentFrame = m_GlobalData.m_StartTime;

    GeneratePlaneClients();
    GenerateTargetClients();
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneClients[i].m_PlaneSocket->SendReset();
        m_PlaneClients[i].m_PlaneSocket->SendPlane(m_PlaneClients[i].m_Plane);
        m_PlaneClients[i].m_PlaneSocket->SendRadar(m_Sensors[SensorIdRadar]);
        m_PlaneClients[i].m_PlaneSocket->SendEsm(m_Sensors[SensorIdEsm]);
        m_PlaneClients[i].m_PlaneSocket->SendInfrared(m_Sensors[SensorIdInfrared]);
        m_PlaneClients[i].m_PlaneSocket->SendStateMap(m_StateMap);
        if (m_PlaneClients[i].m_PlaneSocket->IsFusion())
        {
            int index = m_FusionAlgoSel.GetCurSel();
            int count = m_FusionAlgoSel.GetCount();
            if ((index != CB_ERR) && (count >= 1))
            {
            }
            else
            {
                index = 0;
            }
            m_PlaneClients[i].m_PlaneSocket->SendFusionAlgo(m_FusionAlgos[index]);
        }
        if (m_PlaneClients[i].m_PlaneSocket->IsAttack())
        {
            int index = m_NaviAlgoSel.GetCurSel();
            int count = m_NaviAlgoSel.GetCount();
            if ((index != CB_ERR) && (count >= 1))
            {
            }
            else
            {
                index = 0;
            }
            // m_PlaneClients[i].m_PlaneSocket->SendNaviAlgo(m_NaviAlgos[index]);
        }
        for (int j = 0; j < m_TargetClients.size(); ++j)
        {
            m_PlaneClients[i].m_PlaneSocket->SendTarget(m_TargetClients[j].m_Target);
        }
        m_PlaneClients[i].m_PlaneSocket->SendGlobalData(m_GlobalData);
    }

    ResetCtrls();

    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_StateMap.AddPlane(m_PlaneClients[i].m_Plane, &m_Sensors[SensorIdRadar], &m_Sensors[SensorIdEsm], &m_Sensors[SensorIdInfrared]);
        m_StateMapDlg.AddPlane(m_PlaneClients[i].m_Plane);
        m_MatlabDlg.AddPlane(m_PlaneClients[i].m_Plane);
    }

    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_StateMap.AddTarget(m_TargetClients[i].m_Target);
        m_StateMapDlg.AddTarget(m_TargetClients[i].m_Target);
        m_MatlabDlg.AddTarget(m_TargetClients[i].m_Target);
    }

    m_MatlabDlg.SetSize((m_GlobalData.m_EndTime - m_GlobalData.m_StartTime + 1) / m_GlobalData.m_Interval);
    if (m_ShowMatlabDlg)
    {
        m_MatlabDlg.Show();
    }

    m_StateMapDlg.m_Ctrl.DrawBackground();
    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();

    m_FusionDatas.clear();
    GenerateTrueData();
    m_CurrentFrame = 0;
    SetTimer(WM_TIME_FRAME, 800, NULL);
}

void CDataCenterDlg::PauseSim()
{
    KillTimer(WM_TIME_FRAME);
}

void CDataCenterDlg::ResumeSim()
{
    SetTimer(WM_TIME_FRAME, 800, NULL);
}

void CDataCenterDlg::FinishSim()
{
    m_CurrentRound++;
    if (m_CurrentRound >= m_GlobalData.m_Rounds)
    {
    }
    else
    {
        StartSim();
    }
    // Save dat files at last.
}

void CDataCenterDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnTimer(nIDEvent);
    PauseSim();

    if (nIDEvent == WM_TIME_FRAME)
    {
        if (m_CurrentFrame >= m_GlobalData.m_EndTime)
        {
            FinishSim();
            return;
        }

        int index = m_CurrentFrame / m_GlobalData.m_Interval;

        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            TrueDataPacket packet;
            packet.m_PlaneTrueData = m_PlaneClients[i].m_PlaneTrueDatas[index];
            for (int j = 0; j < m_TargetClients.size(); ++j)
            {
                packet.m_TargetTrueDatas.push_back(m_TargetClients[j].m_TargetTrueDatas[index]);
            }
            m_PlaneClients[i].m_PlaneSocket->SendTrueData(packet);
        }

        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            m_StateMap.AddPlaneData(i, m_PlaneClients[i].m_PlaneTrueDatas[index].m_Pos);
            m_MatlabDlg.AddPlaneData(i, m_PlaneClients[i].m_PlaneTrueDatas[index].m_Pos);
        }
        for (int i = 0; i < TARGET_COUNT; ++i)
        {
            m_StateMap.AddTargetData(i, m_TargetClients[i].m_TargetTrueDatas[index].m_Pos);
            m_MatlabDlg.AddTargetData(i, m_TargetClients[i].m_TargetTrueDatas[index].m_Pos);
        }

        m_StateMapDlg.m_Ctrl.DrawTargets();
        m_StateMapDlg.m_Ctrl.BlendAll();
        m_StateMapDlg.m_Ctrl.Invalidate();

        m_CurrentFrame += m_GlobalData.m_Interval;
    }
}

void CDataCenterDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
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

    CCommonDlg::OnMButtonDblClk(nFlags, point);
}

void CDataCenterDlg::ReadConfigFile()
{
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
        if (key == TEXT("FUSION_ALGO"))
        {
            int type;
            ist >> type;
            switch (type)
            {
            case FusionAlgoTypeLocal:
                {
                    wstring name;
                    int localType;
                    ist >> name;
                    ist >> localType;
                    FusionAlgo *algo = new FusionLocalAlgo(name.c_str(), (FusionLocalAlgoType)localType);
                    m_FusionAlgos.push_back(algo);
                }
                break;
            case FusionAlgoTypeVc:
                {
                    wstring name, dllFileName, funcName;
                    ist >> name >> dllFileName >> funcName;
                    FusionAlgo *algo = new FusionVcAlgo(name.c_str(), dllFileName.c_str(), funcName.c_str());
                    m_FusionAlgos.push_back(algo);
                }
                break;
            case FusionAlgoTypeMatlab:
                {
                    wstring name, dllFileName, funcName;
                    ist >> name >> dllFileName >> funcName;
                    FusionAlgo *algo = new FusionMatlabAlgo(name.c_str(), dllFileName.c_str(), funcName.c_str());
                    m_FusionAlgos.push_back(algo);
                }
                break;
            default:
                CString msg;
                msg.AppendFormat(TEXT("未知融合算法类型%d."), type);
                AfxMessageBox(msg);
                break;
            }
        }
        else if (key == TEXT("NAVI_ALGO"))
        {
        }
    }

    in.close();
}

void CDataCenterDlg::OnCbnSelchangeDcGlobalNoise()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = m_NoiseType.GetCurSel();
    int count = m_NoiseType.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_GlobalData.m_NoiseType = (NoiseType)index;
    }
}

void CDataCenterDlg::OnCbnSelchangeDcStateMapBkg()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = m_StateMapBkg.GetCurSel();
    int count = m_StateMapBkg.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_StateMap.m_Background = (StateMapBackground)index;
    }
}

void CDataCenterDlg::OnBnClickedStateMapDlgButton()
{
    // TODO: 在此添加控件通知处理程序代码
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

void CDataCenterDlg::OnBnClickedMatlabDlgButton()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowMatlabDlg)
    {
        m_MatlabDlg.Hide();

        m_ShowMatlabDlg = false;
    }
    else
    {
        m_MatlabDlg.Show();
        m_ShowMatlabDlg = true;
    }
}

void CDataCenterDlg::OnCbnSelchangeDcSensorId()
{
    // TODO: 在此添加控件通知处理程序代码
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        m_SensorEnable = sensor.m_Enable;
        m_SensorMaxDis = sensor.m_MaxDis;
        m_SensorMaxTheta = sensor.m_MaxTheta;
        m_SensorMaxPhi = sensor.m_MaxPhi;
        m_SensorDisVar = sensor.m_DisVar;
        m_SensorThetaVar = sensor.m_ThetaVar;
        m_SensorPhiVar = sensor.m_PhiVar;
        m_SensorProDet = sensor.m_ProDet;
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnBnClickedDcSensorEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SensorEnable = !m_SensorEnable;
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_Enable = m_SensorEnable;
    }
}

void CDataCenterDlg::OnEnChangeDcGlobalRounds()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_GlobalData.m_Rounds = GetDlgItemInt(IDC_DC_GLOBAL_ROUNDS);
}

void CDataCenterDlg::OnEnChangeDcGlobalCycle()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_GlobalData.m_Interval = GetDlgItemInt(IDC_DC_GLOBAL_CYCLE);
}

void CDataCenterDlg::OnEnChangeDcGlobalStartTime()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_GlobalData.m_StartTime = GetDlgItemInt(IDC_DC_GLOBAL_START_TIME);
}

void CDataCenterDlg::OnEnChangeDcGlobalEndTime()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_GlobalData.m_EndTime = GetDlgItemInt(IDC_DC_GLOBAL_END_TIME);
}

void CDataCenterDlg::OnBnClickedGlobalWildValue()
{
    // TODO: 在此添加控件通知处理程序代码
    m_GlobalData.m_WildValue = !m_GlobalData.m_WildValue;
}

void CDataCenterDlg::OnBnClickedGlobalDelay()
{
    // TODO: 在此添加控件通知处理程序代码
    m_GlobalData.m_Delay = !m_GlobalData.m_Delay;
}

void CDataCenterDlg::OnEnChangeDcStateMapMaxX()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_StateMap.m_MaxX = GetDlgItemInt(IDC_DC_STATE_MAP_MAX_X);
}

void CDataCenterDlg::OnEnChangeDcStateMapMaxY()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_StateMap.m_MaxY = GetDlgItemInt(IDC_DC_STATE_MAP_MAX_Y);
}

void CDataCenterDlg::OnEnChangeDcSensorMaxDis()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorMaxDis = GetDlgItemInt(IDC_DC_SENSOR_MAX_DIS);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxDis = m_SensorMaxDis;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorMaxTheta()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorMaxTheta = GetDlgItemInt(IDC_DC_SENSOR_MAX_THETA);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxTheta = m_SensorMaxTheta;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorMaxPhi()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorMaxPhi = GetDlgItemInt(IDC_DC_SENSOR_MAX_PHI);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxPhi = m_SensorMaxPhi;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorDisVar()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorDisVar = GetDlgItemInt(IDC_DC_SENSOR_DIS_VAR);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_DisVar = m_SensorDisVar;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorThetaVar()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorThetaVar = GetDlgItemInt(IDC_DC_SENSOR_THETA_VAR);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_ThetaVar = m_SensorThetaVar;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorPhiVar()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorPhiVar = GetDlgItemInt(IDC_DC_SENSOR_PHI_VAR);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_PhiVar = m_SensorPhiVar;
    }
}

void CDataCenterDlg::OnEnChangeDcSensorProDet()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CCommonDlg::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    m_SensorProDet = GetDlgItemInt(IDC_DC_SENSOR_PRO_DET);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_ProDet = m_SensorProDet;
    }
}
