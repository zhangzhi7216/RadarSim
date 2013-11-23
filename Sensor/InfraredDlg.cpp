// InfraredDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sensor.h"
#include "InfraredDlg.h"


// CInfraredDlg 对话框

CInfraredDlg::CInfraredDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent /*=NULL*/)
	: CSensorDlg(title, sensor, clientProxy, pParent)
{
    delete m_Ctrl;
    m_Ctrl = new CInfraredCtrl(sensor);
}

CInfraredDlg::~CInfraredDlg()
{
}

void CInfraredDlg::CreateDlg(CInfraredDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_SENSOR_DLG, GetDesktopWindow());
}

void CInfraredDlg::DoDataExchange(CDataExchange* pDX)
{
    CSensorDlg::DoDataExchange(pDX);
    // DDX_Control(pDX, IDC_INFRARED, m_Ctrl);
}

// CInfraredDlg 消息处理程序

BOOL CInfraredDlg::OnInitDialog()
{
    CSensorDlg::OnInitDialog();
    GetDlgItem(IDC_SENSOR_SHOW_SCANLINE)->ShowWindow(SW_HIDE);

    return TRUE;
}

void CInfraredDlg::Resize()
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
