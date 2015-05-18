// StateMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StateMapDlg.h"

// CStateMapDlg �Ի���

IMPLEMENT_DYNAMIC(CStateMapDlg, CDialog)

CStateMapDlg::CStateMapDlg(LPCWSTR title, StateMap &stateMap, CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialog(CStateMapDlg::IDD, pParent)
    , m_Title(title)
    , m_StateMap(stateMap)
    , m_Dlg(dlg)
    , m_Initialized(false)
{
    m_Ctrl = new CStateMapCtrl(m_StateMap);
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStateMapDlg::~CStateMapDlg()
{
    delete m_Ctrl;
}

void CStateMapDlg::CreateDlg(CStateMapDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_STATEMAP_DLG, GetDesktopWindow());
}

void CStateMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATEMAP, *m_Ctrl);
    DDX_Check(pDX, IDC_STATEMAP_SHOW_TRACK, m_StateMap.m_ShowTrack);
    DDX_Check(pDX, IDC_STATEMAP_SHOW_THETA_RANGE, m_StateMap.m_ShowThetaRange);
    DDX_Control(pDX, IDC_STATEMAP_BACKGROUND, m_Background);
    DDX_Control(pDX, IDC_STATEMAP_EXPLOSION_TYPE, m_ExplosionType);
    DDX_Control(pDX, IDC_STATEMAP_PLANE_TYPE, m_PlaneType);
    DDX_Control(pDX, IDC_STATEMAP_PLANE_COLOR, m_PlaneColor);
    DDX_Control(pDX, IDC_STATEMAP_TARGET_ID, m_TargetId);
    DDX_Control(pDX, IDC_STATEMAP_TARGET_TYPE, m_TargetType);
    DDX_Control(pDX, IDC_STATEMAP_TARGET_COLOR, m_TargetColor);
    DDX_Control(pDX, IDC_STATEMAP_PLANE_ID, m_PlaneId);
}
BEGIN_MESSAGE_MAP(CStateMapDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_STATEMAP_SHOW_TRACK, &CStateMapDlg::OnBnClickedStatemapShowTrack)
    ON_BN_CLICKED(IDC_STATEMAP_SHOW_THETA_RANGE, &CStateMapDlg::OnBnClickedStatemapShowThetaRange)
    ON_CBN_SELCHANGE(IDC_STATEMAP_BACKGROUND, &CStateMapDlg::OnCbnSelchangeStatemapBackground)
    ON_CBN_SELCHANGE(IDC_STATEMAP_EXPLOSION_TYPE, &CStateMapDlg::OnCbnSelchangeStatemapExplosionType)
    ON_CBN_SELCHANGE(IDC_STATEMAP_PLANE_TYPE, &CStateMapDlg::OnCbnSelchangeStatemapPlaneType)
    ON_CBN_SELCHANGE(IDC_STATEMAP_PLANE_COLOR, &CStateMapDlg::OnCbnSelchangeStatemapPlaneColor)
    ON_CBN_SELCHANGE(IDC_STATEMAP_TARGET_ID, &CStateMapDlg::OnCbnSelchangeStatemapTargetId)
    ON_CBN_SELCHANGE(IDC_STATEMAP_TARGET_TYPE, &CStateMapDlg::OnCbnSelchangeStatemapTargetType)
    ON_CBN_SELCHANGE(IDC_STATEMAP_TARGET_COLOR, &CStateMapDlg::OnCbnSelchangeStatemapTargetColor)
    ON_CBN_SELCHANGE(IDC_STATEMAP_PLANE_ID, &CStateMapDlg::OnCbnSelchangeStatemapPlaneId)
END_MESSAGE_MAP()


// CStateMapDlg ��Ϣ�������

BOOL CStateMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_STATEMAP_GRP)->SetWindowTextW(m_Title);

    for (int i = 0; i < StateMapBackgroundLast; ++i)
    {
        m_Background.InsertString(i, StateMapBackgroundNames[i]);
    }
    m_Background.SetCurSel(m_StateMap.m_Background);
    for (int i = 0; i < ExplosionTypeLast; ++i)
    {
        m_ExplosionType.InsertString(i, ExplosionTypeNames[i]);
    }
    m_ExplosionType.SetCurSel(m_StateMap.m_ExplosionType);
    for (int i = 0; i < TargetTypeLast; ++i)
    {
        m_PlaneType.InsertString(i, TargetTypeNames[i]);
    }
    // m_PlaneType.SetCurSel(m_StateMap.m_PlaneType);
    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_PlaneColor.InsertString(i, TargetColorNames[i]);
    }
    // m_PlaneColor.SetCurSel(m_StateMap.m_PlaneColor);
    for (int i = 0; i < TargetTypeLast; ++i)
    {
        m_TargetType.InsertString(i, TargetTypeNames[i]);
    }
    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, TargetColorNames[i]);
    }

    Resize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStateMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStateMapDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}

void CStateMapDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = 2 * (80 + PAD), top = rect.top + PAD, height = rect.bottom - rect.top - PAD * 2;
    int left2 = left + width + PAD, width2 = rect.right - PAD - left2, top2 = top, height2 = height;
    if (IsZoomed())
    {
        width = 0;
        left2 = left;
        width2 = rect.right - PAD - left2;
        GetDlgItem(IDC_STATEMAP_PARAM_GRP)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_SHOW_TRACK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_BACKGROUND_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_BACKGROUND)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_SHOW_THETA_RANGE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_ID_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_ID)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_ID_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_ID)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR_STATIC)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR)->ShowWindow(SW_HIDE);
    }
    else
    {
        GetDlgItem(IDC_STATEMAP_PARAM_GRP)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_SHOW_TRACK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_BACKGROUND_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_BACKGROUND)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_SHOW_THETA_RANGE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_ID_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_ID)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_ID_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_ID)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR)->ShowWindow(SW_SHOW);

        GetDlgItem(IDC_STATEMAP_PARAM_GRP)->MoveWindow(left, top, width, height);

        left = left + PAD;
        width = 80;
        top = top + PAD * 2;
        height = 20;
        GetDlgItem(IDC_STATEMAP_SHOW_TRACK)->MoveWindow(left, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_SHOW_THETA_RANGE)->MoveWindow(left, top, width * 2, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_BACKGROUND_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_BACKGROUND)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_EXPLOSION_TYPE)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_PLANE_ID_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_PLANE_ID)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_PLANE_TYPE)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_PLANE_COLOR)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_TARGET_ID_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_TARGET_ID)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_TARGET_TYPE)->MoveWindow(left + width, top, width, height);

        top = top + height + PAD;
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR_STATIC)->MoveWindow(left, top, width, height);
        GetDlgItem(IDC_STATEMAP_TARGET_COLOR)->MoveWindow(left + width, top, width, height);
    }

    GetDlgItem(IDC_STATEMAP_GRP)->MoveWindow(left2, top2, width2, height2);
    GetDlgItem(IDC_STATEMAP)->MoveWindow(left2 + PAD, top2 + PAD * 2, width2 - PAD * 2, height2 - PAD * 3);
}

void CStateMapDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    if (m_Initialized)
    {
        Resize();
    }
}

void CStateMapDlg::Reset()
{
    m_Ctrl->Reset();

    m_Background.SetCurSel(m_StateMap.m_Background);

    m_ExplosionType.SetCurSel(m_StateMap.m_ExplosionType);
    
    m_PlaneId.ResetContent();
    m_PlaneType.SetCurSel(CB_ERR);
    m_PlaneColor.SetCurSel(CB_ERR);

    m_TargetId.ResetContent();
    m_TargetType.SetCurSel(CB_ERR);
    m_TargetColor.SetCurSel(CB_ERR);

    UpdateData(FALSE);
}

void CStateMapDlg::AddPlane(Plane &plane)
{
    CString str;
    str.AppendFormat(TEXT("%d"), plane.m_Id);
    m_PlaneId.InsertString(m_PlaneId.GetCount(), str);
}

void CStateMapDlg::AddTarget(Target &target)
{
    CString str;
    str.AppendFormat(TEXT("%d"), target.m_Id);
    m_TargetId.InsertString(m_TargetId.GetCount(), str);
}

void CStateMapDlg::OnBnClickedStatemapShowTrack()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_StateMap.m_ShowTrack = !m_StateMap.m_ShowTrack;
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgStateMapTargetsChange(this);
}

void CStateMapDlg::OnBnClickedStatemapShowThetaRange()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_StateMap.m_ShowThetaRange = !m_StateMap.m_ShowThetaRange;
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgStateMapTargetsChange(this);
}

void CStateMapDlg::OnCbnSelchangeStatemapBackground()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_Background.GetCurSel();
    int count = m_Background.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_StateMap.m_Background = (StateMapBackground)index;
        m_Ctrl->DrawBackground();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapBackgroundChange(this);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapExplosionType()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_ExplosionType.GetCurSel();
    int count = m_ExplosionType.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_StateMap.m_ExplosionType = (ExplosionType)index;
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapTargetsChange(this);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapPlaneId()
{
    // TODO: Add your control notification handler code here
    int index = m_PlaneId.GetCurSel();
    int count = m_PlaneId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        index = GetDlgItemInt(IDC_STATEMAP_PLANE_ID);
        m_PlaneType.SetCurSel(m_StateMap.m_Planes[index].m_Type);
        m_PlaneColor.SetCurSel(m_StateMap.m_Planes[index].m_Color);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapPlaneType()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneId.GetCurSel();
    int count = m_PlaneId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        index = GetDlgItemInt(IDC_STATEMAP_PLANE_ID);
        m_StateMap.m_Planes[index].m_Type = (TargetType)m_PlaneType.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapTargetsChange(this);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapPlaneColor()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_PlaneId.GetCurSel();
    int count = m_PlaneId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        index = GetDlgItemInt(IDC_STATEMAP_PLANE_ID);
        m_StateMap.m_Planes[index].m_Color = (TargetColor)m_PlaneColor.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapTargetsChange(this);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapTargetId()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = GetDlgItemInt(IDC_STATEMAP_TARGET_ID);
    if (m_StateMap.m_Targets.find(index) != m_StateMap.m_Targets.end())
    {
        m_TargetType.SetCurSel(m_StateMap.m_Targets[index].m_Type);
        m_TargetColor.SetCurSel(m_StateMap.m_Targets[index].m_Color);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapTargetType()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = GetDlgItemInt(IDC_STATEMAP_TARGET_ID);
    if (m_StateMap.m_Targets.find(index) != m_StateMap.m_Targets.end())
    {
        m_StateMap.m_Targets[index].m_Type = (TargetType)m_TargetType.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapTargetsChange(this);
    }
}

void CStateMapDlg::OnCbnSelchangeStatemapTargetColor()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = GetDlgItemInt(IDC_STATEMAP_TARGET_ID);
    if (m_StateMap.m_Targets.find(index) != m_StateMap.m_Targets.end())
    {
        m_StateMap.m_Targets[index].m_Color = (TargetColor)m_TargetColor.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgStateMapTargetsChange(this);
    }
}
