// PlaneDlg.cpp : 实现文件
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


// CFusionCenterDlg 对话框




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
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeInt, CString(TEXT("时间(s)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeInt, CString(TEXT("批号"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("坐标X(km)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("坐标Y(km)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("坐标Z(km)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("速度X(m/s)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("速度Y(m/s)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("速度Z(m/s)"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeString, CString(TEXT("属性"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeString, CString(TEXT("威胁评级"))));
    m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeString, CString(TEXT("类型"))));
}

CFusionCenterDlg::~CFusionCenterDlg()
{
}

BEGIN_MESSAGE_MAP(CFusionCenterDlg, CPlaneDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFusionCenterDlg 消息处理程序

BOOL CFusionCenterDlg::OnInitDialog()
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

    m_RenderCenterSocket = new FusionSocket(this);
	CPlaneDlg::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
  //  ConnectRenderCenter();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFusionCenterDlg::OnPaint()
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
            AfxMessageBox(TEXT("连接到渲染中心失败."));
            exit(-1);
        }
    }
    else
    {
        exit(-1);
    }

    m_RenderCenterSocket->AsyncSelect(FD_READ);
    // m_DataCenterSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    // AfxMessageBox(TEXT("连接到数据中心"));
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

		// 恢复绝对坐标
		for (int iTarget = 0; iTarget < m_FusionOutput.m_FusionDataPacket.m_FusionDatas.size(); iTarget++)
		{
			TrueDataFrame &frame = m_FusionOutput.m_FusionDataPacket.m_FusionDatas[iTarget];
			frame.m_Pos += m_Plane.m_Position;
			if (frame.m_Type == TargetTypeSea)
			{
				frame.m_Pos.Z = 0;
			}
		}

        // 添加噪声数据到融合输出.
        m_FusionOutput.m_FusionDataPacket.m_TargetNoisePackets.clear();
        for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; iSensor++)
        {
            m_FusionOutput.m_FusionDataPacket.m_TargetNoisePackets.push_back(m_FusionInput.m_NoiseDataPackets[(SensorId)iSensor]);
        }

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

            if (m_DataList.m_Targets.find(frame.m_Id) == m_DataList.m_Targets.end())
            {
                Target t;
                t.m_InitPosition = frame.m_Pos;
                t.m_Position = frame.m_Pos;
                t.m_InitVel = frame.m_Vel;
                t.m_Vel = frame.m_Vel;
                t.m_Id = frame.m_Id;
                t.m_Type = (TargetType)frame.m_Type;
                t.m_Color = (TargetColor)(m_DataList.m_Targets.size() % (int)TargetColorLast);
                m_DataList.AddTarget(t);
                m_DataListDlg.AddTarget(t);
            }
            vector<CString> data;
            data.push_back(m_DataList.m_ColumnItems[0].MakeData(frame.m_Time));
            data.push_back(m_DataList.m_ColumnItems[1].MakeData(0));
            data.push_back(m_DataList.m_ColumnItems[2].MakeData(frame.m_Pos.X));
            data.push_back(m_DataList.m_ColumnItems[3].MakeData(frame.m_Pos.Y));
            data.push_back(m_DataList.m_ColumnItems[4].MakeData(frame.m_Pos.Z));
            data.push_back(m_DataList.m_ColumnItems[5].MakeData(frame.m_Vel.X));
            data.push_back(m_DataList.m_ColumnItems[6].MakeData(frame.m_Vel.Y));
            data.push_back(m_DataList.m_ColumnItems[7].MakeData(frame.m_Vel.Z));
            data.push_back(m_DataList.m_ColumnItems[8].MakeData(CString(TEXT(""))));
            data.push_back(m_DataList.m_ColumnItems[9].MakeData(frame.m_IsKeyTarget ? CString(TEXT("Y")) : CString(TEXT("N"))));
            data.push_back(m_DataList.m_ColumnItems[10].MakeData(TargetTypeNames[frame.m_Type]));
            m_DataListCtrl.AddTargetData(frame.m_Id, data);
            m_DataListDlg.m_Ctrl->AddTargetData(frame.m_Id, data);;
        }

        if (m_StateMap.m_ZoomKeyTargetId != -1)
        {
            bool found = false;
            for (int i = 0; i < m_FusionOutput.m_FusionDataPacket.m_FusionDatas.size(); i++)
            {
                TrueDataFrame &frame = m_FusionOutput.m_FusionDataPacket.m_FusionDatas[i];
                if (frame.m_Id == m_StateMap.m_ZoomKeyTargetId)
                {
                    m_RenderCenterSocket->SendKeyTarget(frame);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                m_StateMap.m_ZoomKeyTargetId = -1;
            }
        }

        if (m_StateMap.m_ZoomKeyTargetId == -1)
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
        msg.AppendFormat(TEXT("融合算法%s初始化失败."), m_FusionAlgo->m_Name);
    }
    GetDlgItem(IDC_FUSION_ALGO)->SetWindowText(m_FusionAlgo->m_Name);
}

void CFusionCenterDlg::DoFusion()
{
    if (!m_FusionAlgo)
    {
        AfxMessageBox(TEXT("尚未指定融合算法."));
        return;
    }
    if (!m_FusionAlgo->Run(m_FusionInput, m_FusionOutput))
    {
        AfxMessageBox(TEXT("融合算法运行错误."));
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
    //     msg.AppendFormat(TEXT("融合算法输出的融合数据个数(%d)不等于实际敌机个数(%d)！请检查你的融合算法！"), m_FusionOutput.m_FusionData.m_FusionDatas.size(), targetSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_FusionData.m_FilterDatas.size() != targetSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("融合算法输出的滤波数据个数(%d)不等于实际敌机个数(%d)！请检查你的融合算法！"), m_FusionOutput.m_FusionData.m_FilterDatas.size(), targetSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_FusionData.m_NoiseDatas.size() != planeSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("融合算法输出的噪声数据个数(%d)不等于实际我机个数(%d)！请检查你的融合算法！"), m_FusionOutput.m_FusionData.m_NoiseDatas.size(), planeSize);
    //     AfxMessageBox(msg);
    //     return;
    // }
    // if (m_FusionOutput.m_ControlDatas.size() != planeSize)
    // {
    //     CString msg;
    //     msg.AppendFormat(TEXT("融合算法输出的控制数据个数(%d)不等于实际我机个数(%d)！请检查你的融合算法！"), m_FusionOutput.m_ControlDatas.size(), planeSize);
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
        AfxMessageBox(TEXT("创建到渲染中心的套接字失败"));
    }
    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->Close();
        delete m_PlaneSockets[i];
    }
    m_PlaneSockets.clear();
    m_Lock.Unlock();
}

void CFusionCenterDlg::SetPlane(Plane &plane)
{
    CPlaneDlg::SetPlane(plane);

    m_FusionInput.m_RadarScopeMax = plane.m_Radar.m_MaxDis;
    m_FusionInput.m_RadarScopeVar = plane.m_Radar.m_DisVar;
    m_FusionInput.m_RadarThetaMax = plane.m_Radar.m_MaxTheta;
    m_FusionInput.m_RadarThetaVar = plane.m_Radar.m_ThetaVar;
    m_FusionInput.m_RadarPhiMax = plane.m_Radar.m_MaxPhi;
    m_FusionInput.m_RadarPhiVar = plane.m_Radar.m_PhiVar;
    m_FusionInput.m_RadarInterval = plane.m_Radar.m_Interval;

    m_FusionInput.m_AisXMax = plane.m_Ais.m_MaxDis;
    m_FusionInput.m_AisXVar = plane.m_Ais.m_DisVar;
    m_FusionInput.m_AisYMax = plane.m_Ais.m_MaxTheta;
    m_FusionInput.m_AisYVar = plane.m_Ais.m_ThetaVar;
    m_FusionInput.m_AisZMax = plane.m_Ais.m_MaxPhi;
    m_FusionInput.m_AisZVar = plane.m_Ais.m_PhiVar;
    m_FusionInput.m_AisInterval = plane.m_Ais.m_Interval;

    m_FusionInput.m_TongScopeMax = plane.m_Tong.m_MaxDis;
    m_FusionInput.m_TongScopeVar = plane.m_Tong.m_DisVar;
    m_FusionInput.m_TongThetaMax = plane.m_Tong.m_MaxTheta;
    m_FusionInput.m_TongThetaVar = plane.m_Tong.m_ThetaVar;
    m_FusionInput.m_TongPhiMax = plane.m_Tong.m_MaxPhi;
    m_FusionInput.m_TongPhiVar = plane.m_Tong.m_PhiVar;
    m_FusionInput.m_TongInterval = plane.m_Tong.m_Interval;

    m_FusionInput.m_LeiScopeMax = plane.m_Lei.m_MaxDis;
    m_FusionInput.m_LeiScopeVar = plane.m_Lei.m_DisVar;
    m_FusionInput.m_LeiThetaMax = plane.m_Lei.m_MaxTheta;
    m_FusionInput.m_LeiThetaVar = plane.m_Lei.m_ThetaVar;
    m_FusionInput.m_LeiPhiMax = plane.m_Lei.m_MaxPhi;
    m_FusionInput.m_LeiPhiVar = plane.m_Lei.m_PhiVar;
    m_FusionInput.m_LeiInterval = plane.m_Lei.m_Interval;
}

void CFusionCenterDlg::SetGlobalData(GlobalDataPacket &packet)
{
    CPlaneDlg::SetGlobalData(packet);
    m_FusionInput.m_Interval = m_GlobalData.m_Interval;
}