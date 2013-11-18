// RadarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Radar.h"
#include "RadarDlg.h"


// CRadarDlg 对话框

IMPLEMENT_DYNAMIC(CRadarDlg, CDialog)

CRadarDlg::CRadarDlg(RadarParam &param, RadarClientProxy &clientProxy, CWnd* pParent /*=NULL*/)
	: CDialog(CRadarDlg::IDD, pParent)
    , m_Param(param)
    , m_ClientProxy(clientProxy)
    , m_Ctrl(m_Param)
{

}

CRadarDlg::~CRadarDlg()
{
}

void CRadarDlg::CreateDlg(CRadarDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_RADAR_DLG, GetDesktopWindow());
    dlg.m_Ctrl.Init();
}

void CRadarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CTRL, m_Ctrl);
}


BEGIN_MESSAGE_MAP(CRadarDlg, CDialog)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRadarDlg 消息处理程序

void CRadarDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // CDialog::OnClose();
    m_ClientProxy.OnClose();
}
