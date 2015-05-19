// PlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FusionCenter.h"
#include "FusionCenterDlg.h"
#include "IpDlg.h"

#include "Utility.h"
#include <algorithm>
#include <fstream>
#include <sstream>

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFusionCenterDlg �Ի���




CFusionCenterDlg::CFusionCenterDlg(LPCWSTR title
                                 , bool hasSensor1
                                 , CString sensor1Title
                                 , bool hasSensor2
                                 , CString sensor2Title
                                 , bool hasStateMap
                                 , bool hasDataList
                                 , CWnd* pParent /*=NULL*/)
	: CPlaneDlg(PacketTypeImFusion, title, hasSensor1, sensor1Title, hasSensor2, sensor2Title, hasStateMap, hasDataList, pParent)
    , m_FusionSocket(NULL)
    , m_RenderCenterSocket(NULL)
    , m_FusionAlgo(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_FusionSocket = new FusionSocket(this);
}

CFusionCenterDlg::~CFusionCenterDlg()
{
}

BEGIN_MESSAGE_MAP(CFusionCenterDlg, CPlaneDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFusionCenterDlg ��Ϣ�������

BOOL CFusionCenterDlg::OnInitDialog()
{
    if (!m_FusionSocket->Create())
    {
        AfxMessageBox(TEXT("�׽��ִ���ʧ��"));
        exit(-1);
    }
    if (!m_FusionSocket->Listen())
    {
        AfxMessageBox(TEXT("����ʧ��"));
        exit(-1);
    }
    if (!m_FusionSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("ѡ��ʧ��"));
        exit(-1);
    }

    m_RenderCenterSocket = new FusionSocket(this);
	CPlaneDlg::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    ConnectRenderCenter();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFusionCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFusionCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFusionCenterDlg::CreateDataCenterSocket()
{
    m_DataCenterSocket = new FusionSocket(this);
}

void CFusionCenterDlg::SendPlaneType()
{
    CString addr;
    UINT port;
    m_FusionSocket->GetSockName(addr, port);
    m_DataCenterSocket->SendImFusion(port);
}

void CFusionCenterDlg::ConnectRenderCenter()
{
    wstring hostName = RENDER_CENTER_ADDR;
    int port = RENDER_CENTER_PORT;

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
        if (key == TEXT("RENDER_CENTER_IP"))
        {
            wstring ip;
            ist >> ip;
            hostName = ip;
        }
        else if (key == TEXT("RENDER_CENTER_PORT"))
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
        if (!m_RenderCenterSocket->Connect(dlg.m_HostName, dlg.m_Port))
        {
            Utility::PromptLastErrorMessage();
            AfxMessageBox(TEXT("���ӵ���Ⱦ����ʧ��."));
            exit(-1);
        }
    }
    else
    {
        exit(-1);
    }

    m_RenderCenterSocket->AsyncSelect(FD_READ);
    // m_DataCenterSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    // AfxMessageBox(TEXT("���ӵ���������"));
}

void CFusionCenterDlg::ConnectFusion(const CString &addr, int port)
{
}

void CFusionCenterDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    FusionSocket *socket = new FusionSocket(this);
    if (m_FusionSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
    m_PlaneSockets.push_back(socket);
    m_Lock.Unlock();
}

void CFusionCenterDlg::AddNoiseData(NoiseDataPacket &packet)
{
    m_FusionInput.m_NoiseDataPackets[(SensorId)packet.m_SensorId] = packet;
    if (m_FusionInput.m_NoiseDataPackets.size() == SensorIdLast)
    {
        DoFusion();

        for (int i = 0; i < m_FusionOutput.m_FusionDataPacket.m_FusionDatas.size(); ++i)
        {
            TrueDataFrame &frame = m_FusionOutput.m_FusionDataPacket.m_FusionDatas[i];
            if (m_StateMap.m_Targets.find(frame.m_Id) == m_StateMap.m_Targets.end())
            {
                Target t;
                t.m_InitPosition = frame.m_Pos;
                t.m_Position = frame.m_Pos;
                t.m_InitVel = frame.m_Vel;
                t.m_Vel = frame.m_Vel;
                t.m_Id = frame.m_Id;
                t.m_Type = (TargetType)frame.m_Type;
                t.m_Color = (TargetColor)(m_StateMap.m_Targets.size() % (int)TargetColorLast);
                m_StateMap.AddTarget(t);
                m_StateMapDlg.AddTarget(t);
            }
            m_StateMap.AddTargetData(frame.m_Id, frame.m_Pos, frame.m_Vel);
            m_StateMap.m_Targets[frame.m_Id].m_IsKeyTarget = frame.m_IsKeyTarget;
        }

        if (m_StateMap.m_ZoomKeyTargetId != -1)
        {
            TrueDataFrame &frame = m_FusionOutput.m_FusionDataPacket.m_FusionDatas[m_StateMap.m_ZoomKeyTargetId];
            m_RenderCenterSocket->SendKeyTarget(frame);
        }
        else
        {
            AddNoiseDataPhase2();
        }
    }
}

void CFusionCenterDlg::AddNoiseDataPhase2()
{
    m_StateMapCtrl.DrawTargets();
    m_StateMapCtrl.BlendAll();
    m_StateMapCtrl.Invalidate();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();

    if (m_StateMap.m_ZoomKeyTargetId != -1)
    {
        m_ShowZoomDlg = true;
        m_ZoomDlg.DrawTarget();
        m_ZoomDlg.ShowWindow(SW_SHOW);
        m_ZoomDlg.Invalidate();
    }

    m_DataCenterSocket->SendFusionData(m_FusionOutput.m_FusionDataPacket);

    m_FusionInput.m_NoiseDataPackets.clear();
    m_FusionOutput.m_FusionDataPacket = FusionDataPacket();
}

void CFusionCenterDlg::SendNoiseDatas(TrueDataPacket &packet)
{
}

void CFusionCenterDlg::SetFusionAlgo(FusionAlgo *algo)
{
    if (m_FusionAlgo)
    {
        delete m_FusionAlgo;
        m_FusionAlgo = NULL;
    }
    m_FusionAlgo = algo;
    if (!m_FusionAlgo->Init())
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨%s��ʼ��ʧ��."), m_FusionAlgo->m_Name);
    }
    GetDlgItem(IDC_FUSION_ALGO)->SetWindowText(m_FusionAlgo->m_Name);
}

void CFusionCenterDlg::DoFusion()
{
    if (!m_FusionAlgo)
    {
        AfxMessageBox(TEXT("��δָ���ں��㷨."));
        return;
    }
    if (!m_FusionAlgo->Run(m_FusionInput, m_FusionOutput))
    {
        AfxMessageBox(TEXT("�ں��㷨���д���."));
        return;
    }

    // Set Plane true data.
    // for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
    // {
    //     m_FusionOutput.m_FusionData.m_PlaneTrueDatas.push_back(it->second.second.m_PlaneTrueData);
    // }

    // Check fusion output.
    // int planeSize = m_NoiseDatas.size();
    // int targetSize = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas.size();
    // if (m_FusionOutput.m_FusionData.m_FusionDatas.size() != targetSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("�ں��㷨������ں����ݸ���(%d)������ʵ�ʵл�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_FusionDatas.size(), targetSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_FusionData.m_FilterDatas.size() != targetSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("�ں��㷨������˲����ݸ���(%d)������ʵ�ʵл�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_FilterDatas.size(), targetSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_FusionData.m_NoiseDatas.size() != planeSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("�ں��㷨������������ݸ���(%d)������ʵ���һ�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_NoiseDatas.size(), planeSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_ControlDatas.size() != planeSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("�ں��㷨����Ŀ������ݸ���(%d)������ʵ���һ�����(%d)����������ں��㷨��"), m_FusionOutput.m_ControlDatas.size(), planeSize);
    //     AfxMessageBox(msg);
    //     return;
    // }

#ifdef _DEV
    for (int i = 0; i < m_FusionOutput.m_FusionData.m_FusionDatas.size(); ++i)
    {
        TrueDataFrame &frame = m_FusionOutput.m_FusionData.m_FusionDatas[i];
        TrueDataFrame &planeTrue = m_FusionOutput.m_FusionData.m_PlaneTrueDatas[i % m_FusionOutput.m_FusionData.m_PlaneTrueDatas.size()];
        frame.m_Pos = planeTrue.m_Pos + Position(300 * (i % 3), 300 * ((i + 1) % 3), 0);
    }
    for (int i = 0; i < m_FusionOutput.m_ControlDatas.size(); ++i)
    {
        m_FusionOutput.m_ControlDatas[i].m_FusionData = m_FusionOutput.m_FusionData;
    }
#endif
}

void CFusionCenterDlg::ResetSockets()
{
    m_Lock.Lock();
    CPlaneDlg::ResetSockets();
    m_RenderCenterSocket->Close();
    BOOL reuse = TRUE;
    m_RenderCenterSocket->SetSockOpt(SO_REUSEADDR, (void *)&reuse, sizeof(reuse), SOL_SOCKET);
    if (!m_RenderCenterSocket->Create())
    {
        AfxMessageBox(TEXT("��������Ⱦ���ĵ��׽���ʧ��"));
    }
    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->Close();
        delete m_PlaneSockets[i];
    }
    m_PlaneSockets.clear();
    m_Lock.Unlock();
}