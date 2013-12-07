
// PlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FusionPlane.h"
#include "FusionPlaneDlg.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFusionPlaneDlg 对话框




CFusionPlaneDlg::CFusionPlaneDlg(LPCWSTR title, CWnd* pParent /*=NULL*/)
	: CPlaneDlg(title, pParent)
    , m_FusionSocket(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_FusionSocket = new FusionSocket(this);
}

BEGIN_MESSAGE_MAP(CFusionPlaneDlg, CPlaneDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFusionPlaneDlg 消息处理程序

BOOL CFusionPlaneDlg::OnInitDialog()
{
    if (!m_FusionSocket->Create())
    {
        AfxMessageBox(TEXT("套接字创建失败"));
        exit(-1);
    }
    if (!m_FusionSocket->Listen())
    {
        AfxMessageBox(TEXT("监听失败"));
        exit(-1);
    }
    if (!m_FusionSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("选择失败"));
        exit(-1);
    }

	CPlaneDlg::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFusionPlaneDlg::OnPaint()
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
HCURSOR CFusionPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFusionPlaneDlg::ConnectDataCenter()
{
    CPlaneDlg::ConnectDataCenter();
    CString addr;
    UINT port;
    m_FusionSocket->GetSockName(addr, port);
    m_DataCenterSocket->SendFusionAddr(port);
}

void CFusionPlaneDlg::ConnectFusion(const CString &addr, int port)
{
}

void CFusionPlaneDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    PlaneSocket *socket = new PlaneSocket(this);
    if (m_FusionSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
    m_PlaneSockets.push_back(socket);
    m_Lock.Unlock();
}

void CFusionPlaneDlg::AddNoiseData(SocketPacketPair spp)
{
    m_NoiseDatas.insert(make_pair(spp.second.m_PlaneTrueData.m_Id, spp));
    if (m_NoiseDatas.size() == m_PlaneSockets.size())
    {
        DoFusion();
    }
}

void CFusionPlaneDlg::SendNoiseData(NoiseDataPacket &packet)
{
    AddNoiseData(make_pair((PlaneSocket *)NULL, packet));
}

void CFusionPlaneDlg::DoFusion()
{
    FusionDataPacket packet;
    int nTargets = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas.size();
    int nPlanes = m_NoiseDatas.size();
    for (int i = 0; i < nTargets; ++i)
    {
        NoiseDataFrame frame;
        assert(m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas.size() > i);
        frame.m_Time = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas[i].m_Time;
        frame.m_Id = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas[i].m_Id;
        for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
        {
            frame += it->second.second.m_TargetNoiseDatas[i];
        }
        frame /= nPlanes;
        packet.m_FusionDatas.push_back(frame);
        frame = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas[i];
        packet.m_FilterDatas.push_back(frame);
    }
    for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
    {
        packet.m_NoiseDatas.push_back(it->second.second);
    }

    for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
    {
        ControlDataPacket packet;
        packet.m_ControlData.m_Time = it->second.second.m_PlaneTrueData.m_Time;
        packet.m_ControlData.m_Id = it->second.second.m_PlaneTrueData.m_Id;
        if (it->second.first)
        {
            it->second.first->SendControlData(packet);
        }
    }

    m_NoiseDatas.clear();

    m_DataCenterSocket->SendFusionData(packet);
}

void CFusionPlaneDlg::ResetSockets()
{
    m_Lock.Lock();
    CPlaneDlg::ResetSockets();
    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->Close();
        delete m_PlaneSockets[i];
    }
    m_PlaneSockets.clear();
    m_Lock.Unlock();
}
