
// DataCenterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataCenter.h"
#include "DataCenterDlg.h"

#include "DataCenterSocket.h"
#include "PlaneSocket.h"

#include <algorithm>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataCenterDlg 对话框


#define TIME_FRAMES 1000
#define WM_TIME_FRAME (WM_USER + 1)


CDataCenterDlg::CDataCenterDlg(CWnd* pParent /*=NULL*/)
	: CCommonDlg(CDataCenterDlg::IDD, pParent)
    , m_FusionConnected(false)
    , m_ConnectedPlanes(0)
    , m_ShowStateMapDlg(true)
    , m_StateMapDlg(TEXT("态势"), m_StateMap, this)
    , m_CurrentFrame(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_DataCenterSocket = new DataCenterSocket(this);
}

void CDataCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataCenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CDataCenterDlg::OnBnClickedOk)
    ON_WM_TIMER()
    ON_WM_MBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDataCenterDlg 消息处理程序

BOOL CDataCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    if (!m_DataCenterSocket->Create(DATA_CENTER_PORT))
    {
        AfxMessageBox(TEXT("套接字创建失败"));
        exit(-1);
    }
    if (!m_DataCenterSocket->Listen())
    {
        AfxMessageBox(TEXT("监听失败"));
        exit(-1);
    }
    if (!m_DataCenterSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("选择失败"));
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
}

void CDataCenterDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    // OnOK();
    StartSim();
}

void CDataCenterDlg::GeneratePlaneClients()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneClients[i].m_Plane.m_Type = (TargetType)i;
        m_PlaneClients[i].m_Plane.m_Position = Position(100, 100 + 200 * i, 100);
        m_PlaneClients[i].m_Plane.m_Color = (TargetColor)i;
        m_PlaneClients[i].m_Radar.m_MaxDis = 300 + i * 10;
        m_PlaneClients[i].m_Radar.m_MaxTheta = 120 + i * 10;
        m_PlaneClients[i].m_Esm.m_MaxDis = 250 + i * 10;
        m_PlaneClients[i].m_Esm.m_MaxTheta = 90 + i * 10;
        m_PlaneClients[i].m_Esm.m_ThetaRangeColor = Color::Red;
        m_PlaneClients[i].m_Esm.m_ShowHeight = FALSE;
        m_PlaneClients[i].m_Infrared.m_MaxDis = 350 + i * 10;
        m_PlaneClients[i].m_Infrared.m_MaxTheta = 60 + i * 10;
        m_PlaneClients[i].m_Infrared.m_ShowScanline = FALSE;
        m_PlaneClients[i].m_Infrared.m_ShowThetaRange = FALSE;
        m_PlaneClients[i].m_Infrared.m_ThetaRangeColor = Color::Yellow;
        m_PlaneClients[i].m_Infrared.m_ShowHeight = FALSE;
        m_PlaneClients[i].m_StateMap.m_Background = StateMapBackground3;
        m_PlaneClients[i].m_StateMap.m_MaxX = 1200;
        m_PlaneClients[i].m_StateMap.m_MaxY = 800;
    }
}

void CDataCenterDlg::GenerateTargetClients()
{
    m_TargetClients.clear();
    for (int i = 0; i < TARGET_COUNT; ++i)
    {
        TargetClient client;
        client.m_Target.m_Type = (TargetType)((int)TargetTypeShipboard + rand() % ((int)TargetTypeMissile - (int)TargetTypeShipboard + 1));
        client.m_Target.m_Position = Position(150, 150 + 150 * i, 150) + Position(10 * (rand() % 3), 10 * (rand() % 3), 10 * (rand() % 3));
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
    for (int i = 0; i < TIME_FRAMES; ++i)
    {
        for (int j = 0; j < PLANE_COUNT; ++j)
        {
            m_PlaneClients[j].m_Plane.m_Position = m_PlaneClients[j].m_Plane.m_Position + Position(rand() % 3, (double)rand() / (double)RAND_MAX * cos(j * 3.1415926), rand() % 2);
            // m_PlaneClients[j].m_Plane.m_Position = m_PlaneClients[j].m_Plane.m_Position + Position(3, 3, 3);
            TrueDataFrame frame;
            frame.m_Time = i;
            frame.m_Id = m_PlaneClients[j].m_Plane.m_Id;
            frame.m_Pos = m_PlaneClients[j].m_Plane.m_Position;
            m_PlaneClients[j].m_PlaneTrueDatas.push_back(frame);
        }
        for (int j = 0; j < m_TargetClients.size(); ++j)
        {
            m_TargetClients[j].m_Target.m_Position = m_TargetClients[j].m_Target.m_Position + Position(3, 0, 0);
            TrueDataFrame frame;
            frame.m_Time = i;
            frame.m_Id = m_TargetClients[j].m_Target.m_Id;
            frame.m_Pos = m_TargetClients[j].m_Target.m_Position;
            m_TargetClients[j].m_TargetTrueDatas.push_back(frame);
        }
    }
}

void CDataCenterDlg::AddFusionData(FusionDataPacket &packet)
{
    m_FusionDatas.push_back(packet);
    ResumeSim();
}

void CDataCenterDlg::StartSim()
{
    GeneratePlaneClients();
    GenerateTargetClients();
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneClients[i].m_PlaneSocket->SendReset();
        m_PlaneClients[i].m_PlaneSocket->SendPlane(m_PlaneClients[i].m_Plane);
        m_PlaneClients[i].m_PlaneSocket->SendRadar(m_PlaneClients[i].m_Radar);
        m_PlaneClients[i].m_PlaneSocket->SendEsm(m_PlaneClients[i].m_Esm);
        m_PlaneClients[i].m_PlaneSocket->SendInfrared(m_PlaneClients[i].m_Infrared);
        m_PlaneClients[i].m_PlaneSocket->SendStateMap(m_PlaneClients[i].m_StateMap);
        for (int j = 0; j < m_TargetClients.size(); ++j)
        {
            m_PlaneClients[i].m_PlaneSocket->SendTarget(m_TargetClients[j].m_Target);
        }
    }

    ResetCtrls();

    m_StateMap.m_Background = StateMapBackground3;
    m_StateMap.m_MaxX = 1200;
    m_StateMap.m_MaxY = 800;

    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_StateMap.AddPlane(m_PlaneClients[i].m_Plane, &m_PlaneClients[i].m_Radar, &m_PlaneClients[i].m_Esm, &m_PlaneClients[i].m_Infrared);
        m_StateMapDlg.AddPlane(m_PlaneClients[i].m_Plane);
    }

    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_StateMap.AddTarget(m_TargetClients[i].m_Target);
        m_StateMapDlg.AddTarget(m_TargetClients[i].m_Target);
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
    // Save dat files.
}

void CDataCenterDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnTimer(nIDEvent);
    PauseSim();

    if (nIDEvent == WM_TIME_FRAME)
    {
        if (m_CurrentFrame == TIME_FRAMES)
        {
            m_CurrentFrame == 0;
            KillTimer(WM_TIME_FRAME);
            return;
        }

        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            TrueDataPacket packet;
            packet.m_PlaneTrueData = m_PlaneClients[i].m_PlaneTrueDatas[m_CurrentFrame];
            for (int j = 0; j < m_TargetClients.size(); ++j)
            {
                packet.m_TargetTrueDatas.push_back(m_TargetClients[j].m_TargetTrueDatas[m_CurrentFrame]);
            }
            m_PlaneClients[i].m_PlaneSocket->SendTrueData(packet);
        }

        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            m_StateMap.AddPlaneData(i, m_PlaneClients[i].m_PlaneTrueDatas[m_CurrentFrame].m_Pos);
        }
        for (int i = 0; i < TARGET_COUNT; ++i)
        {
            m_StateMap.AddTargetData(i, m_TargetClients[i].m_TargetTrueDatas[m_CurrentFrame].m_Pos);
        }

        m_StateMapDlg.m_Ctrl.DrawTargets();
        m_StateMapDlg.m_Ctrl.BlendAll();
        m_StateMapDlg.m_Ctrl.Invalidate();

        ++m_CurrentFrame;
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
