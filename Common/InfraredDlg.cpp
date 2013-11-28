// InfraredDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InfraredDlg.h"


// CInfraredDlg 对话框

CInfraredDlg::CInfraredDlg(LPCWSTR title, Sensor &sensor, CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CSensorDlg(title, sensor, dlg, pParent)
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
    GetDlgItem(IDC_SENSOR_SHOW_SCANLINE)->EnableWindow(FALSE);

    return TRUE;
}
