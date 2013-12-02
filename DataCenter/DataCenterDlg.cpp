
// DataCenterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataCenter.h"
#include "DataCenterDlg.h"

#include "DataCenterSocket.h"
#include "PlaneSocket.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataCenterDlg 对话框




CDataCenterDlg::CDataCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataCenterDlg::IDD, pParent)
    , m_FusionConnected(false)
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

void CDataCenterDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    PlaneSocket *socket = new PlaneSocket(this);
    if (m_DataCenterSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
    m_PlaneSockets.push_back(socket);
    if (m_FusionConnected)
    {
        socket->SendFusionAddr(m_FusionAddr, m_FusionPort);
    }
    m_Lock.Unlock();
    if (m_FusionConnected && m_PlaneSockets.size() == PLANE_COUNT)
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
}

void CDataCenterDlg::SetFusionAddr(const CString &addr, int port)
{
    m_FusionAddr = addr;
    m_FusionPort = port;
    m_FusionConnected = true;

    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->SendFusionAddr(m_FusionAddr, m_FusionPort);
    }
    if (m_FusionConnected && m_PlaneSockets.size() == PLANE_COUNT)
    {
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
}

void CDataCenterDlg::ResetSockets()
{
    m_Lock.Lock();
    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->Close();
        delete m_PlaneSockets[i];
    }
    m_PlaneSockets.clear();

    m_FusionConnected = false;
    m_Lock.Unlock();
}

void CDataCenterDlg::ResetCtrls()
{
    GetDlgItem(IDOK)->EnableWindow(FALSE);
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
        m_PlaneClients[i].m_Plane.m_Id = i;
        m_PlaneClients[i].m_Plane.m_Type = (TargetType)i;
        m_PlaneClients[i].m_Radar.m_MaxDis += i * 10;
        m_PlaneClients[i].m_Radar.m_MaxTheta += i * 10;
        m_PlaneClients[i].m_Esm.m_MaxDis = 250 + i * 10;
        m_PlaneClients[i].m_Esm.m_MaxTheta = 90 + i * 10;
        m_PlaneClients[i].m_Infrared.m_MaxDis = 350 + i * 10;
        m_PlaneClients[i].m_Infrared.m_MaxTheta = 60 + i * 10;
        m_PlaneClients[i].m_StateMap.m_Background = StateMapBackground3;
        m_PlaneClients[i].m_StateMap.m_MaxX = 800;
        m_PlaneClients[i].m_StateMap.m_MaxY = 800;
    }
}

void CDataCenterDlg::StartSim()
{
    GeneratePlaneClients();
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneSockets[i]->SendPlane(m_PlaneClients[i].m_Plane);
        m_PlaneSockets[i]->SendRadar(m_PlaneClients[i].m_Radar);
        m_PlaneSockets[i]->SendEsm(m_PlaneClients[i].m_Esm);
        m_PlaneSockets[i]->SendInfrared(m_PlaneClients[i].m_Infrared);
        m_PlaneSockets[i]->SendStateMap(m_PlaneClients[i].m_StateMap);
    }
}
