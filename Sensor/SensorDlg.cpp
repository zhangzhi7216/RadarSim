// SensorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sensor.h"
#include "SensorDlg.h"


// CSensorDlg �Ի���

IMPLEMENT_DYNAMIC(CSensorDlg, CDialog)

CSensorDlg::CSensorDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent /*=NULL*/)
	: CDialog(CSensorDlg::IDD, pParent)
    , m_Title(title)
    , m_Sensor(sensor)
    , m_ClientProxy(clientProxy)
    , m_Ctrl(m_Sensor)
    , m_Initialized(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSensorDlg::~CSensorDlg()
{
}

void CSensorDlg::CreateDlg(CSensorDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_SENSOR_DLG, GetDesktopWindow());
}

void CSensorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENSOR, m_Ctrl);
    DDX_Text(pDX, IDC_SENSOR_TYPE, Sensor::SensorTypeNames[m_Sensor.Type]);
    DDX_Check(pDX, IDC_SENSOR_ENABLE, m_Sensor.Enable);
    DDX_Text(pDX, IDC_SENSOR_MAX_DIS, m_Sensor.MaxDis);
    DDX_Text(pDX, IDC_SENSOR_MAX_THETA, m_Sensor.MaxTheta);
    DDX_Text(pDX, IDC_SENSOR_MAX_PHI, m_Sensor.MaxPhi);
    DDX_Text(pDX, IDC_SENSOR_DIS_VAR, m_Sensor.DisVar);
    DDX_Text(pDX, IDC_SENSOR_THETA_VAR, m_Sensor.ThetaVar);
    DDX_Text(pDX, IDC_SENSOR_PHI_VAR, m_Sensor.PhiVar);
    DDX_Text(pDX, IDC_SENSOR_PRO_DET, m_Sensor.ProDet);
    DDX_Check(pDX, IDC_SENSOR_SHOW_SCANLINE, m_Sensor.ShowScanline);
    DDX_Check(pDX, IDC_SENSOR_SHOW_TRACK, m_Sensor.ShowTrack);
    DDX_Control(pDX, IDC_SENSOR_TARGET_ID, m_TargetId);
    DDX_Control(pDX, IDC_SENSOR_TARGET_COLOR, m_TargetColor);
}


BEGIN_MESSAGE_MAP(CSensorDlg, CDialog)
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_SENSOR_SHOW_SCANLINE, &CSensorDlg::OnBnClickedSensorShowScanline)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_SENSOR_ENABLE, &CSensorDlg::OnBnClickedSensorEnable)
    ON_BN_CLICKED(IDC_SENSOR_SHOW_TRACK, &CSensorDlg::OnBnClickedSensorShowTrack)
    ON_EN_CHANGE(IDC_SENSOR_MAX_DIS, &CSensorDlg::OnEnChangeSensorMaxDis)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_ID, &CSensorDlg::OnCbnSelchangeSensorTargetId)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_COLOR, &CSensorDlg::OnCbnSelchangeSensorTargetColor)
END_MESSAGE_MAP()


// CSensorDlg ��Ϣ��������

BOOL CSensorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ����Ӷ���ĳ�ʼ������
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_SENSOR_GRP)->SetWindowTextW(m_Title);

    for (map<int, Target>::iterator it = m_Sensor.m_Plane.m_Targets.begin();
        it != m_Sensor.m_Plane.m_Targets.end();
        ++it)
    {
        CString str;
        str.AppendFormat(TEXT("%d"), it->first);
        m_TargetId.InsertString(m_TargetId.GetCount(), str);
        m_TargetId.SetItemData(m_TargetId.GetCount() - 1, it->first);
    }

    for (int i = 0; i < Target::TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, Target::TargetColorNames[i]);
    }

    Resize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSensorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSensorDlg::OnClose()
{
    // TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

    // CDialog::OnClose();
    m_ClientProxy.OnClose();
}

void CSensorDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = 2 * (80 + PAD), top = rect.top + PAD, height = rect.bottom - rect.top - PAD * 2;
    int left2 = left + width + PAD, width2 = rect.right - PAD - left2, top2 = top, height2 = height;

    GetDlgItem(IDC_SENSOR_PARAM_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = 80;
    top = top + PAD * 2;
    height = 20;
    GetDlgItem(IDC_SENSOR_TYPE_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TYPE)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_ENABLE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_DIS_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_DIS)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_THETA_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_THETA)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_PHI_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_PHI)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_DIS_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_DIS_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_THETA_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_THETA_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PHI_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PHI_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PRO_DET_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PRO_DET)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_SCANLINE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_TRACK)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_ID_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_ID)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_COLOR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_COLOR)->MoveWindow(left + width, top, width, height);

    GetDlgItem(IDC_SENSOR_GRP)->MoveWindow(left2, top2, width2, height2);
    GetDlgItem(IDC_SENSOR)->MoveWindow(left2 + PAD, top2 + PAD * 2, width2 - PAD * 2, height2 - PAD * 3);
}

void CSensorDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: �ڴ˴�������Ϣ�����������
    if (m_Initialized)
    {
        Resize();
    }
}

void CSensorDlg::OnBnClickedSensorEnable()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    m_Sensor.Enable = !m_Sensor.Enable;
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnBnClickedSensorShowScanline()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    m_Sensor.ShowScanline = !m_Sensor.ShowScanline;
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnBnClickedSensorShowTrack()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    m_Sensor.ShowTrack = !m_Sensor.ShowTrack;
    m_Ctrl.DrawTargets();
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.DrawTargets();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnEnChangeSensorMaxDis()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ����ӿؼ�֪ͨ�����������
    m_Sensor.MaxDis = GetDlgItemInt(IDC_SENSOR_MAX_DIS);
    m_Ctrl.BlendAll();
    m_Ctrl.Invalidate();
    m_ClientProxy.BlendAll();
    m_ClientProxy.Invalidate();
}

void CSensorDlg::OnCbnSelchangeSensorTargetId()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        int targetId = m_TargetId.GetItemData(index);
        m_TargetColor.SetCurSel(m_Sensor.m_Plane.m_Targets[targetId].m_Color);
    }
}

void CSensorDlg::OnCbnSelchangeSensorTargetColor()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        int targetId = m_TargetId.GetItemData(index);
        m_Sensor.m_Plane.m_Targets[targetId].m_Color = (Target::TargetColor)m_TargetColor.GetCurSel();
        m_Ctrl.DrawTargets();
        m_Ctrl.BlendAll();
        m_Ctrl.Invalidate();
        m_ClientProxy.DrawTargets();
        m_ClientProxy.BlendAll();
        m_ClientProxy.Invalidate();
    }
}