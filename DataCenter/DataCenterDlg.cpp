
// DataCenterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataCenter.h"
#include "DataCenterDlg.h"

#include "DataCenterSocket.h"

#include <algorithm>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"

#include "OneTimeMatlabDlg.h"

#include "Utility.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataCenterDlg �Ի���


#define TIMES 10
#define TIME_FRAMES 100
#define WM_TIME_FRAME (WM_USER + 1)
#define INTERNAL_INTERVAL_MS 800


CDataCenterDlg::CDataCenterDlg(CWnd* pParent /*=NULL*/)
	: CCommonDlg(CDataCenterDlg::IDD, pParent)
    , m_FusionConnected(false)
    , m_ConnectedPlanes(0)
    , m_ShowStateMapDlg(false)
    , m_StateMapDlg(TEXT("̬��"), m_StateMap, this)
    , m_CurrentFrame(0)
    , m_CurrentRound(0)
    , m_SensorEnable(FALSE)
    , m_SensorMaxDis(TEXT("0"))
    , m_SensorMaxTheta(TEXT("0"))
    , m_SensorMaxPhi(TEXT("0"))
    , m_SensorDisVar(TEXT("0"))
    , m_SensorThetaVar(TEXT("0"))
    , m_SensorPhiVar(TEXT("0"))
    , m_ExtDataEnable(FALSE)
    , m_PlaneId(0)
    , m_PlanePosX(TEXT("0"))
    , m_PlanePosY(TEXT("0"))
    , m_PlanePosZ(TEXT("0"))
    , m_PlaneVelX(TEXT("0"))
    , m_PlaneVelY(TEXT("0"))
    , m_PlaneVelZ(TEXT("0"))
    , m_PlaneAccX(TEXT("0"))
    , m_PlaneAccY(TEXT("0"))
    , m_PlaneAccZ(TEXT("0"))
    , m_PlanePal(TEXT("0"))
    , m_PlaneRadius(TEXT("0"))
    , m_ExtDataPath(_T(""))
    , m_OutputPlaneTrue(_T("PlaneTrue.dat"))
    , m_OutputTargetTrue(_T("TargetTrue.dat"))
    , m_OutputTargetNoise(_T("TargetNoise.dat"))
    , m_OutputFusion(_T("Fusion.dat"))
    , m_EvalName(_T(""))
    , m_EvalDll(_T(""))
    , m_EvalFunc(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_DataCenterSocket = new DataCenterSocket(this);

    m_Sensors[SensorIdRadar].m_Id = SensorIdRadar;
    m_Sensors[SensorIdRadar].m_MaxDis = 350;
    m_Sensors[SensorIdRadar].m_MaxTheta = 120;

    m_Sensors[SensorIdAis].m_Id = SensorIdAis;
    m_Sensors[SensorIdAis].m_MaxDis = 300;
    m_Sensors[SensorIdAis].m_MaxTheta = 90;
    m_Sensors[SensorIdAis].m_ThetaRangeColor = Color::Red;
    m_Sensors[SensorIdAis].m_ShowHeight = FALSE;

    m_Sensors[SensorIdTong].m_Id = SensorIdTong;
    m_Sensors[SensorIdTong].m_MaxDis = 250;
    m_Sensors[SensorIdTong].m_MaxTheta = 60;
    m_Sensors[SensorIdTong].m_ShowScanline = FALSE;
    m_Sensors[SensorIdTong].m_ShowThetaRange = FALSE;
    m_Sensors[SensorIdTong].m_ThetaRangeColor = Color::Yellow;
    m_Sensors[SensorIdTong].m_ShowHeight = FALSE;

    m_Sensors[SensorIdLei].m_Id = SensorIdLei;
    m_Sensors[SensorIdLei].m_MaxDis = 250;
    m_Sensors[SensorIdLei].m_MaxTheta = 60;
    m_Sensors[SensorIdLei].m_ShowScanline = FALSE;
    m_Sensors[SensorIdLei].m_ShowThetaRange = FALSE;
    m_Sensors[SensorIdLei].m_ThetaRangeColor = Color::Blue;
    m_Sensors[SensorIdLei].m_ShowHeight = FALSE;

    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneSockets[i] = NULL;
    }
}

CDataCenterDlg::~CDataCenterDlg()
{
    for (int i = 0; i < m_FusionAlgos.size(); ++i)
    {
        delete m_FusionAlgos[i];
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
    DDX_Control(pDX, IDC_DC_SENSOR_INTERVAL, m_SensorInterval);
    DDX_Check(pDX, IDC_DC_EXT_DATA_ENABLE, m_ExtDataEnable);
    DDX_Control(pDX, IDC_EXT_DATA_LOAD_BUTTON, m_ExtDataLoadButton);
    DDX_Control(pDX, IDC_DC_PLANE_ID, m_PlaneIdSel);
    DDX_Text(pDX, IDC_DC_PLANE_ID_SET, m_PlaneId);
    DDX_Control(pDX, IDC_DC_PLANE_COLOR, m_PlaneColor);
    DDX_Control(pDX, IDC_DC_PLANE_TYPE, m_PlaneType);
    DDX_Control(pDX, IDC_DC_PLANE_MOTION, m_PlaneMotion);
    DDX_Text(pDX, IDC_DC_PLANE_POS_X, m_PlanePosX);
    DDX_Text(pDX, IDC_DC_PLANE_POS_Y, m_PlanePosY);
    DDX_Text(pDX, IDC_DC_PLANE_POS_Z, m_PlanePosZ);
    DDX_Text(pDX, IDC_DC_PLANE_VEL_X, m_PlaneVelX);
    DDX_Text(pDX, IDC_DC_PLANE_VEL_Y, m_PlaneVelY);
    DDX_Text(pDX, IDC_DC_PLANE_VEL_Z, m_PlaneVelZ);
    DDX_Text(pDX, IDC_DC_PLANE_ACC_X, m_PlaneAccX);
    DDX_Text(pDX, IDC_DC_PLANE_ACC_Y, m_PlaneAccY);
    DDX_Text(pDX, IDC_DC_PLANE_ACC_Z, m_PlaneAccZ);
    DDX_Text(pDX, IDC_DC_PLANE_PAL, m_PlanePal);
    DDX_Text(pDX, IDC_DC_PLANE_RADIUS, m_PlaneRadius);
    DDX_Text(pDX, IDC_EXT_DATA_PATH, m_ExtDataPath);
    DDX_Text(pDX, IDC_DC_EVAL_PLANE_TRUE, m_OutputPlaneTrue);
    DDX_Text(pDX, IDC_DC_EVAL_TARGET_TRUE, m_OutputTargetTrue);
    DDX_Text(pDX, IDC_DC_TARGET_NOISE, m_OutputTargetNoise);
    DDX_Text(pDX, IDC_DC_EVAL_FUSION, m_OutputFusion);
    DDX_Control(pDX, IDC_DC_EVAL_ID, m_EvalSel);
    DDX_Text(pDX, IDC_DC_EVAL_NAME, m_EvalName);
    DDX_Text(pDX, IDC_DC_EVAL_DLL, m_EvalDll);
    DDX_Text(pDX, IDC_DC_EVAL_FUNC, m_EvalFunc);
    DDX_Control(pDX, IDC_DC_STATE_MAP_EXPLOSION_TYPE, m_StateMapExplosionType);
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
    ON_CBN_SELCHANGE(IDC_DC_STATE_MAP_EXPLOSION_TYPE, &CDataCenterDlg::OnCbnSelchangeDcStateMapExplosionType)
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
    ON_CBN_SELCHANGE(IDC_DC_SENSOR_INTERVAL, &CDataCenterDlg::OnCbnSelchangeDcSensorInterval)
    ON_BN_CLICKED(IDC_CONFIG_LOAD, &CDataCenterDlg::OnBnClickedConfigLoad)
    ON_BN_CLICKED(IDC_CONFIG_SAVE, &CDataCenterDlg::OnBnClickedConfigSave)
    ON_BN_CLICKED(IDC_DC_EVAL_ADD, &CDataCenterDlg::OnBnClickedDcEvalAdd)
    ON_BN_CLICKED(IDC_DC_TARGET_ADD, &CDataCenterDlg::OnBnClickedDcTargetAdd)
    ON_BN_CLICKED(IDC_DC_EXT_DATA_ENABLE, &CDataCenterDlg::OnBnClickedDcExtDataEnable)
    ON_BN_CLICKED(IDC_EXT_DATA_LOAD_BUTTON, &CDataCenterDlg::OnBnClickedExtDataLoadButton)
    ON_CBN_SELCHANGE(IDC_DC_PLANE_ID, &CDataCenterDlg::OnCbnSelchangeDcPlaneId)
    ON_CBN_SELCHANGE(IDC_DC_PLANE_MOTION, &CDataCenterDlg::OnCbnSelchangeDcPlaneMotion)
    ON_CBN_SELCHANGE(IDC_DC_PLANE_COLOR, &CDataCenterDlg::OnCbnSelchangeDcPlaneColor)
    ON_CBN_SELCHANGE(IDC_DC_PLANE_TYPE, &CDataCenterDlg::OnCbnSelchangeDcPlaneType)
    ON_EN_CHANGE(IDC_DC_PLANE_POS_X, &CDataCenterDlg::OnEnChangeDcPlanePosX)
    ON_EN_CHANGE(IDC_DC_PLANE_POS_Y, &CDataCenterDlg::OnEnChangeDcPlanePosY)
    ON_EN_CHANGE(IDC_DC_PLANE_POS_Z, &CDataCenterDlg::OnEnChangeDcPlanePosZ)
    ON_EN_CHANGE(IDC_DC_PLANE_VEL_X, &CDataCenterDlg::OnEnChangeDcPlaneVelX)
    ON_EN_CHANGE(IDC_DC_PLANE_VEL_Y, &CDataCenterDlg::OnEnChangeDcPlaneVelY)
    ON_EN_CHANGE(IDC_DC_PLANE_VEL_Z, &CDataCenterDlg::OnEnChangeDcPlaneVelZ)
    ON_EN_CHANGE(IDC_DC_PLANE_ACC_X, &CDataCenterDlg::OnEnChangeDcPlaneAccX)
    ON_EN_CHANGE(IDC_DC_PLANE_ACC_Y, &CDataCenterDlg::OnEnChangeDcPlaneAccY)
    ON_EN_CHANGE(IDC_DC_PLANE_ACC_Z, &CDataCenterDlg::OnEnChangeDcPlaneAccZ)
    ON_EN_CHANGE(IDC_DC_PLANE_PAL, &CDataCenterDlg::OnEnChangeDcPlanePal)
    ON_EN_CHANGE(IDC_DC_PLANE_RADIUS, &CDataCenterDlg::OnEnChangeDcPlaneRadius)
    ON_EN_CHANGE(IDC_EXT_DATA_PATH, &CDataCenterDlg::OnEnChangeExtDataPath)
    ON_EN_CHANGE(IDC_DC_EVAL_PLANE_TRUE, &CDataCenterDlg::OnEnChangeDcEvalPlaneTrue)
    ON_EN_CHANGE(IDC_DC_EVAL_TARGET_TRUE, &CDataCenterDlg::OnEnChangeDcEvalTargetTrue)
    ON_EN_CHANGE(IDC_DC_TARGET_NOISE, &CDataCenterDlg::OnEnChangeDcTargetNoise)
    ON_EN_CHANGE(IDC_DC_EVAL_FUSION, &CDataCenterDlg::OnEnChangeDcEvalFusion)
    ON_EN_CHANGE(IDC_DC_EVAL_FUNC, &CDataCenterDlg::OnEnChangeDcEvalFunc)
    ON_EN_CHANGE(IDC_DC_EVAL_DLL, &CDataCenterDlg::OnEnChangeDcEvalDll)
    ON_EN_CHANGE(IDC_DC_EVAL_NAME, &CDataCenterDlg::OnEnChangeDcEvalName)
    ON_CBN_SELCHANGE(IDC_DC_EVAL_ID, &CDataCenterDlg::OnCbnSelchangeDcEvalId)
END_MESSAGE_MAP()


// CDataCenterDlg ��Ϣ�������

BOOL CDataCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    for (int i = 0; i < TargetTypeLast; ++i)
    {
        m_PlaneType.InsertString(i, TargetTypeNames[i]);
    }
    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_PlaneColor.InsertString(i, TargetColorNames[i]);
    }
    for (int i = 0; i < TargetMoveTypeLast; ++i)
    {
        m_PlaneMotion.InsertString(i, TargetMoveTypeNames[i]);
    }

    m_Plane.m_Color = TargetColorRed;
    CString s(TEXT("�һ�"));
    m_PlaneIdSel.InsertString(0, s);
    m_PlaneIdSel.SetCurSel(0);
    OnCbnSelchangeDcPlaneId();

    for (int i = SensorIdRadar; i < SensorIdLast; ++i)
    {
        m_SensorIdSel.InsertString(i, SensorIdNames[i]);
    }
    m_SensorIdSel.SetCurSel(0);
    OnCbnSelchangeDcSensorId();

    ReadConfigFile();

    if (m_FusionAlgos.size() == 0)
    {
        AfxMessageBox(TEXT("δ�ܶ�ȡ�κ��ں��㷨."));
        exit(-1);
    }
    for (int i = 0; i < m_FusionAlgos.size(); ++i)
    {
        m_FusionAlgoSel.InsertString(i, m_FusionAlgos[i]->m_Name);
    }
    m_FusionAlgoSel.SetCurSel(0);

    for (int i = 0; i < StateMapBackgroundLast; ++i)
    {
        m_StateMapBkg.InsertString(i, StateMapBackgroundNames[i]);
    }
    m_StateMapBkg.SetCurSel(m_StateMap.m_Background);

    for (int i = 0; i < ExplosionTypeLast; ++i)
    {
        m_StateMapExplosionType.InsertString(i, ExplosionTypeNames[i]);
    }
    m_StateMapExplosionType.SetCurSel(m_StateMap.m_ExplosionType);

    for (int i = 0; i < NoiseTypeLast; ++i)
    {
        m_NoiseType.InsertString(i, NoiseTypeNames[i]);
    }
    m_NoiseType.SetCurSel(m_GlobalData.m_NoiseType);

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    if (!m_DataCenterSocket->Create(DATA_CENTER_PORT))
    {
        AfxMessageBox(TEXT("�׽��ִ���ʧ��."));
        exit(-1);
    }
    if (!m_DataCenterSocket->Listen())
    {
        AfxMessageBox(TEXT("����ʧ��."));
        exit(-1);
    }
    if (!m_DataCenterSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("ѡ��ʧ��."));
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
    m_StateMapDlg.SetWindowTextW(TEXT("���ݿ�������̬��"));

    ResetCtrls();
    ResetSockets();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataCenterDlg::OnPaint()
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
}

void CDataCenterDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    DataCenterSocket *socket = new DataCenterSocket(this);
    if (m_DataCenterSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }

    int i;
    for (i = 0; i < PLANE_COUNT; ++i)
    {
        if (m_PlaneSockets[i] == NULL)
        {
            m_PlaneSockets[i] = socket;
            break;
        }
    }
    if (i == PLANE_COUNT)
    {
        AfxMessageBox(TEXT("��������"));
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
        if (m_PlaneSockets[i])
        {
            m_PlaneSockets[i]->SendFusionAddr(m_FusionAddr, m_FusionPort);
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
        if (m_PlaneSockets[i])
        {
            m_PlaneSockets[i]->Close();
            delete m_PlaneSockets[i];
            m_PlaneSockets[i] = NULL;
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

void CDataCenterDlg::ResetPlanes()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_Plane.Reset();
    }
}

void CDataCenterDlg::ResetTargets()
{
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_TargetClients[i].m_Target.Reset();
    }
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
        m_Plane.m_Type = (TargetType)i;
        m_Plane.m_MoveType = (TargetMoveType)(i % 2);
        m_Plane.m_Position = Position(100, 10 + 200 * i, 100);
        // m_Plane.m_Vel = Position(i + 1, ((double)rand() - (double)RAND_MAX / 2) / ((double)RAND_MAX / 2) * 2, rand() % 3);
        m_Plane.m_Vel = Position(i + 1, 0, rand() % 3);
        m_Plane.m_Acc = Position(rand() % 1, rand() % 1, rand() % 1);
        m_Plane.m_Color = (TargetColor)i;
    }
}

void CDataCenterDlg::GenerateTargetClients()
{
    m_TargetClients.clear();
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        TargetClient client;
        client.m_Target.m_Type = (TargetType)((int)TargetTypeLand + rand() % ((int)TargetTypeAir - (int)TargetTypeLand + 1));
        client.m_Target.m_MoveType = TargetMoveTypeUniVel;
        client.m_Target.m_Position = Position(130, 150 + 150 * i, 150) + Position(10 * (rand() % 3), 10 * (rand() % 3), 10 * (rand() % 3));
        client.m_Target.m_Vel = Position(i + 1, 0, rand() % 3);
        client.m_Target.m_Color = (TargetColor)(i + 1);
        m_TargetClients.push_back(client);
    }
}

void CDataCenterDlg::GenerateTrueData()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneTrueDatas.clear();
    }
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_TargetClients[i].m_TargetTrueDatas.clear();
    }
    if (m_ExtDataEnable)
    {
        wifstream ifs(m_ExtDataPath);
        if (!ifs)
        {
            CString msg;
            msg.AppendFormat(TEXT("���ⲿ�ļ�%sʧ��"), m_ExtDataPath);
            AfxMessageBox(msg);
        }
        ifs >> m_GlobalData.m_Interval >> m_GlobalData.m_StartTime >> m_GlobalData.m_EndTime;
        UpdateData(FALSE);

        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            int size = 0;
            ifs >> size;
            for (int j = 0; j < size; ++j)
            {
                TrueDataFrame frame;
                ifs >> frame;
                m_PlaneTrueDatas.push_back(frame);
            }
        }
        for (int i = 0; i < m_TargetClients.size(); ++i)
        {
            int size = 0;
            ifs >> size;
            for (int j = 0; j < size; ++j)
            {
                TrueDataFrame frame;
                ifs >> frame;
                m_TargetClients[i].m_TargetTrueDatas.push_back(frame);
            }
        }

        ifs.close();
    }
    else
    {
        for (int i = m_GlobalData.m_StartTime; i < m_GlobalData.m_EndTime + 1; i += m_GlobalData.m_Interval)
        {
            for (int j = 0; j < PLANE_COUNT; ++j)
            {
                if (i != m_GlobalData.m_StartTime)
                {
                    m_Plane.Move(m_GlobalData.m_Interval);
                }
                // m_Plane.m_Position = m_Plane.m_Position + Position(rand() % 3, (double)rand() / (double)RAND_MAX * cos(j * 3.1415926), rand() % 2);
                // m_Plane.m_Position = m_Plane.m_Position + Position(3, 3, 3);
                TrueDataFrame frame;
                frame.m_Time = i;
                frame.m_Id = m_Plane.m_Id;
                frame.m_Type = m_Plane.m_Type;
                frame.m_Pos = m_Plane.m_Position;
                frame.m_Vel = m_Plane.m_Vel;
                frame.m_Acc = m_Plane.m_Acc;
                m_PlaneTrueDatas.push_back(frame);
            }
            for (int j = 0; j < m_TargetClients.size(); ++j)
            {
                if (i != m_GlobalData.m_StartTime)
                {
                    m_TargetClients[j].m_Target.Move(m_GlobalData.m_Interval);
                }
                // m_TargetClients[j].m_Target.m_Position = m_TargetClients[j].m_Target.m_Position + Position(3, 0, 0);
                TrueDataFrame frame;
                frame.m_Time = i;
                frame.m_Id = m_TargetClients[j].m_Target.m_Id;
                frame.m_Type = m_TargetClients[j].m_Target.m_Type;
                frame.m_Pos = m_TargetClients[j].m_Target.m_Position;
                frame.m_Vel = m_TargetClients[j].m_Target.m_Vel;
                frame.m_Acc = m_TargetClients[j].m_Target.m_Acc;
                m_TargetClients[j].m_TargetTrueDatas.push_back(frame);
            }
        }
        // Dump dat files at last.
        DumpTrueData(TEXT("dump_true_data.dat"));
    }
}

void CDataCenterDlg::GenerateGlobalData()
{
}

void CDataCenterDlg::AddFusionData(FusionDataPacket &packet)
{
    int index = (m_CurrentFrame - m_GlobalData.m_StartTime) / m_GlobalData.m_Interval;

    /////// ����Ϊ��֡������ʾ
    // ��ʾ��֡�һ�
    m_StateMap.AddPlaneData(0, m_PlaneTrueDatas[index].m_Pos, m_PlaneTrueDatas[index].m_Vel);

    vector<TrueDataFrame *> targetTrueDatas;
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        targetTrueDatas.push_back(&m_TargetClients[i].m_TargetTrueDatas[index]);
    }

    ResumeSim();
}

void CDataCenterDlg::StartSim()
{
    if (m_TargetClients.size() == 0)
    {
        AfxMessageBox(TEXT("��Ҫ�������һ�ܵл�"));
        return;
    }

    ResetCtrls();
    ResetPlanes();
    ResetTargets();

    // m_CurrentFrame = m_GlobalData.m_StartTime;

    // GeneratePlaneClients();
    // GenerateTargetClients();
    m_Plane.m_Radar = m_Sensors[SensorIdRadar];
    m_Plane.m_Ais = m_Sensors[SensorIdAis];
    m_Plane.m_Tong = m_Sensors[SensorIdTong];
    m_Plane.m_Lei = m_Sensors[SensorIdLei];
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        m_PlaneSockets[i]->SendReset();
        m_PlaneSockets[i]->SendPlane(m_Plane);
        if (m_PlaneSockets[i]->IsRadar())
        {
            m_PlaneSockets[i]->SendRadar(m_Sensors[SensorIdRadar]);
            m_PlaneSockets[i]->SendAis(m_Sensors[SensorIdAis]);
        }
        if (m_PlaneSockets[i]->IsDetect())
        {
            m_PlaneSockets[i]->SendTong(m_Sensors[SensorIdTong]);
            m_PlaneSockets[i]->SendLei(m_Sensors[SensorIdLei]);
        }
        if (m_PlaneSockets[i]->IsFusion())
        {
            m_PlaneSockets[i]->SendStateMap(m_StateMap);
            int index = m_FusionAlgoSel.GetCurSel();
            int count = m_FusionAlgoSel.GetCount();
            if ((index != CB_ERR) && (count >= 1))
            {
            }
            else
            {
                index = 0;
            }
            m_PlaneSockets[i]->SendFusionAlgo(m_FusionAlgos[index]);
        }
        for (int j = 0; j < m_TargetClients.size(); ++j)
        {
            m_PlaneSockets[i]->SendTarget(m_TargetClients[j].m_Target);
        }
        m_PlaneSockets[i]->SendGlobalData(m_GlobalData);
    }

    m_StateMap.AddPlane(m_Plane);
    m_StateMapDlg.AddPlane(m_Plane);

    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        m_StateMap.AddTarget(m_TargetClients[i].m_Target);
        m_StateMapDlg.AddTarget(m_TargetClients[i].m_Target);
    }

    m_StateMapDlg.m_Ctrl->DrawBackground();
    m_StateMapDlg.m_Ctrl->DrawTargets();
    m_StateMapDlg.m_Ctrl->BlendAll();
    m_StateMapDlg.m_Ctrl->Invalidate();

    m_FusionDatas.clear();
    GenerateTrueData();
    // m_CurrentFrame = 0;
    m_CurrentFrame = m_GlobalData.m_StartTime;
    SetTimer(WM_TIME_FRAME, INTERNAL_INTERVAL_MS, NULL);
}

void CDataCenterDlg::PauseSim()
{
    KillTimer(WM_TIME_FRAME);
}

void CDataCenterDlg::ResumeSim()
{
    m_CurrentFrame += m_GlobalData.m_Interval;
    SetTimer(WM_TIME_FRAME, INTERNAL_INTERVAL_MS, NULL);
}

void CDataCenterDlg::FinishSim()
{
    m_CurrentRound++;

    // Output datas.
    OutputPlaneTrueData();
    OutputTargetTrueData();
    OutputTargetNoiseData();
    OutputFusionData();

    COneTimeMatlabDlg dlg;
    dlg.Run(m_CurrentRound);

    if (m_CurrentRound >= m_GlobalData.m_Rounds)
    {
        for (int i = 0; i < m_EvalItems.size(); ++i)
        {
            m_EvalItems[i].Run(wstring(m_OutputPlaneTrue),
                wstring(m_OutputTargetTrue),
                wstring(m_OutputTargetNoise),
                wstring(m_OutputFusion));
        }

        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
    else
    {
        StartSim();
    }
}

void CDataCenterDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
            packet.m_PlaneTrueData = m_PlaneTrueDatas[index];
            for (int j = 0; j < m_TargetClients.size(); ++j)
            {
                packet.m_TargetTrueDatas.push_back(m_TargetClients[j].m_TargetTrueDatas[index]);
            }
            m_PlaneSockets[i]->SendTrueData(packet);

        }
    }
}

/*
void CDataCenterDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
*/

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
            default:
                CString msg;
                msg.AppendFormat(TEXT("δ֪�ں��㷨����%d."), type);
                AfxMessageBox(msg);
                break;
            }
        }
    }

    in.close();
}

void CDataCenterDlg::OnCbnSelchangeDcGlobalNoise()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_NoiseType.GetCurSel();
    int count = m_NoiseType.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_GlobalData.m_NoiseType = (NoiseType)index;
    }
}

void CDataCenterDlg::OnCbnSelchangeDcStateMapBkg()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_StateMapBkg.GetCurSel();
    int count = m_StateMapBkg.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_StateMap.m_Background = (StateMapBackground)index;
    }
}

void CDataCenterDlg::OnCbnSelchangeDcStateMapExplosionType()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_StateMapExplosionType.GetCurSel();
    int count = m_StateMapExplosionType.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_StateMap.m_ExplosionType = (ExplosionType)index;
    }
}

void CDataCenterDlg::OnBnClickedStateMapDlgButton()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDataCenterDlg::OnCbnSelchangeDcSensorId()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        m_SensorEnable = sensor.m_Enable;
        m_SensorMaxDis.Format(TEXT("%lf"), sensor.m_MaxDis);
        m_SensorMaxTheta.Format(TEXT("%lf"), sensor.m_MaxTheta);
        m_SensorMaxPhi.Format(TEXT("%lf"), sensor.m_MaxPhi);
        m_SensorDisVar.Format(TEXT("%lf"), sensor.m_DisVar);
        m_SensorThetaVar.Format(TEXT("%lf"), sensor.m_ThetaVar);
        m_SensorPhiVar.Format(TEXT("%lf"), sensor.m_PhiVar);
        m_SensorInterval.SetCurSel(sensor.m_Interval - 1);

        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnBnClickedDcSensorEnable()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_Rounds = GetDlgItemInt(IDC_DC_GLOBAL_ROUNDS);
}

void CDataCenterDlg::OnEnChangeDcGlobalCycle()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_Interval = GetDlgItemInt(IDC_DC_GLOBAL_CYCLE);
}

void CDataCenterDlg::OnEnChangeDcGlobalStartTime()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_StartTime = GetDlgItemInt(IDC_DC_GLOBAL_START_TIME);
}

void CDataCenterDlg::OnEnChangeDcGlobalEndTime()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_EndTime = GetDlgItemInt(IDC_DC_GLOBAL_END_TIME);
}

void CDataCenterDlg::OnBnClickedGlobalWildValue()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_WildValue = !m_GlobalData.m_WildValue;
}

void CDataCenterDlg::OnBnClickedGlobalDelay()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_GlobalData.m_Delay = !m_GlobalData.m_Delay;
}

void CDataCenterDlg::OnEnChangeDcStateMapMaxX()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_StateMap.m_MaxX = GetDlgItemInt(IDC_DC_STATE_MAP_MAX_X);
}

void CDataCenterDlg::OnEnChangeDcStateMapMaxY()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_StateMap.m_MaxY = GetDlgItemInt(IDC_DC_STATE_MAP_MAX_Y);
}

void CDataCenterDlg::OnEnChangeDcSensorMaxDis()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_MAX_DIS, m_SensorMaxDis);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxDis = _tcstod((LPCWSTR)m_SensorMaxDis, 0);
    }
}

void CDataCenterDlg::OnEnChangeDcSensorMaxTheta()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_MAX_THETA, m_SensorMaxTheta);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxTheta = _tcstod((LPCWSTR)m_SensorMaxTheta, 0);
    }
}

void CDataCenterDlg::OnEnChangeDcSensorMaxPhi()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_MAX_PHI, m_SensorMaxPhi);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_MaxPhi = _tcstod((LPCWSTR)m_SensorMaxPhi, 0);
    }
}

void CDataCenterDlg::OnEnChangeDcSensorDisVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_DIS_VAR, m_SensorDisVar);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_DisVar = _tcstod((LPCWSTR)m_SensorDisVar, 0);
    }
}

void CDataCenterDlg::OnEnChangeDcSensorThetaVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_THETA_VAR, m_SensorThetaVar);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_ThetaVar = _tcstod((LPCWSTR)m_SensorThetaVar, 0);
    }
}

void CDataCenterDlg::OnEnChangeDcSensorPhiVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_DC_SENSOR_PHI_VAR, m_SensorPhiVar);
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        sensor.m_PhiVar = _tcstod((LPCWSTR)m_SensorPhiVar, 0);
    }
}

void CDataCenterDlg::OnCbnSelchangeDcSensorInterval()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_SensorIdSel.GetCurSel();
    int count = m_SensorIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        SensorId sensorId = (SensorId)index;
        Sensor &sensor = m_Sensors[sensorId];
        {
            int index = m_SensorInterval.GetCurSel();
            int count = m_SensorInterval.GetCount();
            if ((index != CB_ERR) && (count >= 1))
            {
                sensor.m_Interval = index + 1;
            }
        }
    }
}

void CDataCenterDlg::OnBnClickedConfigLoad()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST, TEXT("Data Files (*.cfg)|*.cfg|All Files (*.*)|*.*||"));
    INT_PTR ret = dlg.DoModal();
    if (IDOK == ret)
    {
        // Cleanup.
        for (int i = 0; i < m_TargetClients.size(); ++i)
        {
            int count = m_PlaneIdSel.GetCount();
            m_PlaneIdSel.DeleteString(count - 1);
        }
        m_TargetClients.clear();

        CString filePath = dlg.GetPathName();
        CFile file(filePath, CFile::modeRead);
        CArchive ar(&file, CArchive::load);
        ar >> m_Plane;
        int targetSize = 0;
        ar >> targetSize;
        for (int i = 0; i < targetSize; ++i)
        {
            int count = m_PlaneIdSel.GetCount();
            TargetClient client;
            ar >> client.m_Target;
            m_TargetClients.push_back(client);

            CString s;
            s.AppendFormat(TEXT("�л�%d"), client.m_Target.m_Id);
            m_PlaneIdSel.InsertString(count, s);
        }
        ar >> TargetClient::s_TargetCount;

        for (int i = SensorIdRadar; i < SensorIdLast; ++i)
        {
            ar >> m_Sensors[i];
        }
        ar >> m_StateMap;
        ar >> m_GlobalData;
        int index = 0;
        ar >> index;
        m_FusionAlgoSel.SetCurSel(index);
        ar >> index;
        m_NaviAlgoSel.SetCurSel(index);

#if 1
        int evalItemSize = 0;
        ar >> evalItemSize;
        for (int i = 0; i < evalItemSize; ++i)
        {
            EvalItem eval;
            ar >> eval;
            m_EvalItems.push_back(eval);
            m_EvalSel.InsertString(i, eval.m_Name);
        }
#endif

        ar.Close();
        file.Close();

        OnCbnSelchangeDcPlaneId();
        OnCbnSelchangeDcSensorId();
        m_StateMapBkg.SetCurSel(m_StateMap.m_Background);
        m_StateMapExplosionType.SetCurSel(m_StateMap.m_ExplosionType);
        m_NoiseType.SetCurSel(m_GlobalData.m_NoiseType);

        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnBnClickedConfigSave()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, TEXT("Data Files (*.cfg)|*.cfg|All Files (*.*)|*.*||"));
    INT_PTR ret = dlg.DoModal();
    if (IDOK == ret)
    {
        CString filePath = dlg.GetPathName();
        CFile file(filePath, CFile::modeWrite | CFile::modeCreate);
        CArchive ar(&file, CArchive::store);
        ar << m_Plane;
        ar << m_TargetClients.size();
        for (int i = 0; i < m_TargetClients.size(); ++i)
        {
            ar << m_TargetClients[i].m_Target;
        }
        ar << TargetClient::s_TargetCount;

        for (int i = SensorIdRadar; i < SensorIdLast; ++i)
        {
            ar << m_Sensors[i];
        }
        ar << m_StateMap;
        ar << m_GlobalData;
        int index = m_FusionAlgoSel.GetCurSel();
        ar << index;
        index = m_NaviAlgoSel.GetCurSel();
        ar << index;

        ar << m_EvalItems.size();
        for (int i = 0; i < m_EvalItems.size(); ++i)
        {
            ar << m_EvalItems[i];
        }

        ar.Close();
        file.Close();
    }
}

void CDataCenterDlg::OnBnClickedDcEvalAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    EvalItem eval;
    m_EvalItems.push_back(eval);

    int count = m_EvalSel.GetCount();
    m_EvalSel.InsertString(count, eval.m_Name);

    m_EvalSel.SetCurSel(count);
    OnCbnSelchangeDcEvalId();
}

void CDataCenterDlg::OnBnClickedDcTargetAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    TargetClient client;
    int count = m_PlaneIdSel.GetCount();
    client.m_Target.m_Color = (TargetColor)(count % TargetColorLast);
    m_TargetClients.push_back(client);

    CString s;
    s.AppendFormat(TEXT("�л�%d"), client.m_Target.m_Id);
    m_PlaneIdSel.InsertString(count, s);

    m_PlaneIdSel.SetCurSel(count);
    OnCbnSelchangeDcPlaneId();
}

void CDataCenterDlg::OnBnClickedDcExtDataEnable()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_ExtDataEnable = !m_ExtDataEnable;
    if (m_ExtDataEnable)
    {
        m_ExtDataLoadButton.EnableWindow();
        GetDlgItem(IDC_EXT_DATA_PATH)->EnableWindow();
    }
    else
    {
        m_ExtDataLoadButton.EnableWindow(FALSE);
        GetDlgItem(IDC_EXT_DATA_PATH)->EnableWindow(FALSE);
    }
}

void CDataCenterDlg::OnBnClickedExtDataLoadButton()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST, TEXT("Data Files (*.dat)|*.dat|All Files (*.*)|*.*||"));
    INT_PTR ret = dlg.DoModal();
    if (IDOK == ret)
    {
        m_ExtDataPath = dlg.GetPathName();
        UpdateData(FALSE);
    }
    else
    {
        return;
    }
}

void CDataCenterDlg::OnCbnSelchangeDcPlaneId()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;
        m_PlaneId = target.m_Id;
        m_PlaneMotion.SetCurSel(target.m_MoveType);
        m_PlaneType.SetCurSel(target.m_Type);
        m_PlaneColor.SetCurSel(target.m_Color);
        m_PlanePosX.Format(TEXT("%lf"), target.m_InitPosition.X);
        m_PlanePosY.Format(TEXT("%lf"), target.m_InitPosition.Y);
        m_PlanePosZ.Format(TEXT("%lf"), target.m_InitPosition.Z);
        m_PlaneVelX.Format(TEXT("%lf"), target.m_InitVel.X);
        m_PlaneVelY.Format(TEXT("%lf"), target.m_InitVel.Y);
        m_PlaneVelZ.Format(TEXT("%lf"), target.m_InitVel.Z);
        m_PlaneAccX.Format(TEXT("%lf"), target.m_InitAcc.X);
        m_PlaneAccY.Format(TEXT("%lf"), target.m_InitAcc.Y);
        m_PlaneAccZ.Format(TEXT("%lf"), target.m_InitAcc.Z);
        m_PlanePal.Format(TEXT("%lf"), target.m_Pal);
        m_PlaneRadius.Format(TEXT("%lf"), target.m_Radius);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnCbnSelchangeDcPlaneMotion()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        target.m_MoveType = (TargetMoveType)m_PlaneMotion.GetCurSel();
    }
}

void CDataCenterDlg::OnCbnSelchangeDcPlaneColor()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        target.m_Color = (TargetColor)m_PlaneColor.GetCurSel();
    }
}

void CDataCenterDlg::OnCbnSelchangeDcPlaneType()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        target.m_Type = (TargetType)m_PlaneType.GetCurSel();
    }
}

void CDataCenterDlg::OnEnChangeDcPlanePosX()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_POS_X, m_PlanePosX);
        target.m_InitPosition.X = _tcstod((LPCWSTR)m_PlanePosX, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlanePosY()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_POS_Y, m_PlanePosY);
        target.m_InitPosition.Y = _tcstod((LPCWSTR)m_PlanePosY, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlanePosZ()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_POS_Z, m_PlanePosZ);
        target.m_InitPosition.Z = _tcstod((LPCWSTR)m_PlanePosZ, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneVelX()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_VEL_X, m_PlaneVelX);
        target.m_InitVel.X = _tcstod((LPCWSTR)m_PlaneVelX, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneVelY()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_VEL_Y, m_PlaneVelY);
        target.m_InitVel.Y = _tcstod((LPCWSTR)m_PlaneVelY, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneVelZ()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_VEL_Z, m_PlaneVelZ);
        target.m_InitVel.Z = _tcstod((LPCWSTR)m_PlaneVelZ, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneAccX()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_ACC_X, m_PlaneAccX);
        target.m_InitAcc.X = _tcstod((LPCWSTR)m_PlaneAccX, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneAccY()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_ACC_Y, m_PlaneAccY);
        target.m_InitAcc.Y = _tcstod((LPCWSTR)m_PlaneAccY, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneAccZ()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_ACC_Z, m_PlaneAccZ);
        target.m_InitAcc.Z = _tcstod((LPCWSTR)m_PlaneAccZ, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlanePal()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_PAL, m_PlanePal);
        target.m_Pal = _tcstod((LPCWSTR)m_PlanePal, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcPlaneRadius()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneIdSel.GetCurSel();
    int count = m_PlaneIdSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        Target *p = NULL;
        if (index == 0)
        {
            p = &m_Plane;
        }
        else
        {
            p = &m_TargetClients[index - 1].m_Target;
        }
        Target &target = *p;

        GetDlgItemTextW(IDC_DC_PLANE_RADIUS, m_PlaneRadius);
        target.m_Radius = _tcstod((LPCWSTR)m_PlaneRadius, 0);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeExtDataPath()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
}

void CDataCenterDlg::DumpTrueData(LPCWSTR path)
{
    wofstream ofs(path);
    if (!ofs)
    {
        CString msg;
        msg.AppendFormat(TEXT("���ⲿ�ļ�%sʧ��"), m_ExtDataPath);
        AfxMessageBox(msg);
    }
    ofs << m_GlobalData.m_Interval << endl;
    ofs << m_GlobalData.m_StartTime << endl;
    ofs << m_GlobalData.m_EndTime << endl;
    ofs << endl;

    ofs << m_PlaneTrueDatas.size() << endl;
    ofs << endl;
    for (int j = 0; j < m_PlaneTrueDatas.size(); ++j)
    {
        ofs << m_PlaneTrueDatas[j] << endl;
    }
    ofs << endl;
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        ofs << m_TargetClients[i].m_TargetTrueDatas.size() << endl;
        ofs << endl;
        for (int j = 0; j < m_TargetClients[i].m_TargetTrueDatas.size(); ++j)
        {
            ofs << m_TargetClients[i].m_TargetTrueDatas[j] << endl;
        }
        ofs << endl;
    }
    ofs << endl;

    ofs.close();
}

void CDataCenterDlg::OnEnChangeDcEvalPlaneTrue()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
}

void CDataCenterDlg::OnEnChangeDcEvalTargetTrue()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
}

void CDataCenterDlg::OnEnChangeDcTargetNoise()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
}

void CDataCenterDlg::OnEnChangeDcEvalFusion()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
}

void CDataCenterDlg::OutputPlaneTrueData()
{
    wofstream ofs(m_OutputPlaneTrue);
    if (!ofs)
    {
        CString msg;
        msg.AppendFormat(TEXT("���һ���ֵ����ļ�%sʧ��"), m_OutputPlaneTrue);
        AfxMessageBox(msg);
    }

    vector<TrueDataFrame> &trueDatas = m_PlaneTrueDatas;
    ofs << trueDatas.size() << endl;
    ofs << endl;
    for (int j = 0; j < trueDatas.size(); ++j)
    {
        ofs << trueDatas[j] << endl;
    }

    ofs.close();
}

void CDataCenterDlg::OutputTargetTrueData()
{
    wofstream ofs(m_OutputTargetTrue);
    if (!ofs)
    {
        CString msg;
        msg.AppendFormat(TEXT("��Ŀ����ֵ����ļ�%sʧ��"), m_OutputTargetTrue);
        AfxMessageBox(msg);
    }

    // ��ʱ����
    int nData = m_TargetClients[0].m_TargetTrueDatas.size();
    int nTarget = m_TargetClients.size();
    ofs << nData << endl;
    // Ŀ�����
    ofs << nTarget << endl;
    ofs << endl;

    for (int iData = 0; iData < nData; iData++)
    {
        for (int iTarget = 0; iTarget < nTarget; iTarget++)
        {
            TrueDataFrame &trueDataFrame = m_TargetClients[iTarget].m_TargetTrueDatas[iData];
            ofs << trueDataFrame << endl;
        }
        ofs << endl;
    }

    ofs << m_TargetClients.size() << endl;
    ofs << endl;
    for (int i = 0; i < m_TargetClients.size(); ++i)
    {
        vector<TrueDataFrame> &trueDatas = m_TargetClients[i].m_TargetTrueDatas;
        ofs << trueDatas.size() << endl;
        ofs << endl;
        for (int j = 0; j < trueDatas.size(); ++j)
        {
            ofs << trueDatas[j] << endl;
        }
        ofs << endl;
    }

    ofs.close();
}

void CDataCenterDlg::OutputTargetNoiseData()
{
    wofstream ofs(m_OutputTargetNoise);
    if (!ofs)
    {
        CString msg;
        msg.AppendFormat(TEXT("��Ŀ�����ֵ����ļ�%sʧ��"), m_OutputTargetNoise);
        AfxMessageBox(msg);
    }

    // ��ʱ����
    ofs << m_FusionDatas.size() << endl;
    ofs << endl;

    for (int iData = 0; iData < m_FusionDatas.size(); ++iData)
    {
        FusionDataPacket &fusionDataPacket = m_FusionDatas[iData];
        for (int iSensor = 0; iSensor < fusionDataPacket.m_TargetNoisePackets.size(); iSensor++)
        {
            NoiseDataPacket &noiseDataPacket = fusionDataPacket.m_TargetNoisePackets[iSensor];
            // ������ID + Ŀ�����
            ofs << noiseDataPacket.m_SensorId << endl;
            ofs << noiseDataPacket.m_TargetNoiseDatas.size() << endl;
            ofs << endl;

            for (int iTarget = 0; iTarget < noiseDataPacket.m_TargetNoiseDatas.size(); iTarget++)
            {
                // Ŀ������
                ofs << noiseDataPacket.m_TargetNoiseDatas[iTarget] << endl;
                ofs << endl;
            }
            ofs << endl;
        }
        ofs << endl;
    }

    ofs.close();
}

void CDataCenterDlg::OutputFusionData()
{
    wofstream ofs(m_OutputFusion);
    if (!ofs)
    {
        CString msg;
        msg.AppendFormat(TEXT("���ں�����ļ�%sʧ��"), m_OutputFusion);
        AfxMessageBox(msg);
    }

    // ��ʱ����
    ofs << m_FusionDatas.size() << endl;
    ofs << endl;

    for (int iData = 0; iData < m_FusionDatas.size(); ++iData)
    {
        FusionDataPacket &fusionDataPacket = m_FusionDatas[iData];
        // Ŀ�����
        ofs << fusionDataPacket.m_FusionDatas.size() << endl;
        ofs << endl;

        for (int iTarget = 0; iTarget < fusionDataPacket.m_FusionDatas.size(); iTarget++)
        {
            // Ŀ���ں�
            ofs << fusionDataPacket.m_FusionDatas[iTarget];
            ofs << endl;
        }
        ofs << endl;
    }

    ofs.close();
}

void CDataCenterDlg::OnEnChangeDcEvalFunc()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_EvalSel.GetCurSel();
    int count = m_EvalSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        EvalItem &eval = m_EvalItems[index];

        GetDlgItem(IDC_DC_EVAL_FUNC)->GetWindowText(m_EvalFunc);
        eval.m_Func = m_EvalFunc;
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcEvalDll()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_EvalSel.GetCurSel();
    int count = m_EvalSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        EvalItem &eval = m_EvalItems[index];

        GetDlgItem(IDC_DC_EVAL_DLL)->GetWindowText(m_EvalDll);
        eval.m_Dll = m_EvalDll;
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnEnChangeDcEvalName()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CCommonDlg::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int index = m_EvalSel.GetCurSel();
    int count = m_EvalSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        EvalItem &eval = m_EvalItems[index];

        GetDlgItem(IDC_DC_EVAL_NAME)->GetWindowText(m_EvalName);
        eval.m_Name = m_EvalName;
        m_EvalSel.DeleteString(index);
        m_EvalSel.InsertString(index, m_EvalName);
        m_EvalSel.SetCurSel(index);
        UpdateData(FALSE);
    }
}

void CDataCenterDlg::OnCbnSelchangeDcEvalId()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_EvalSel.GetCurSel();
    int count = m_EvalSel.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        EvalItem &eval = m_EvalItems[index];

        m_EvalName = eval.m_Name;
        m_EvalDll = eval.m_Dll;
        m_EvalFunc = eval.m_Func;
        UpdateData(FALSE);
    }
}
