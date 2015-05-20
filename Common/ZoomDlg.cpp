// ZoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZoomDlg.h"


// CZoomDlg 对话框

IMPLEMENT_DYNAMIC(CZoomDlg, CDialog)

CZoomDlg::CZoomDlg(CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
    , m_Dlg(dlg)
    , m_Image(NULL)
    , m_Initialized(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::CreateDlg(CZoomDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_ZOOM_DLG, GetDesktopWindow());
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CZoomDlg::DrawTarget()
{
    if (m_Image)
    {
        delete m_Image;
    }
    Image *keyTarget = Image::FromFile(KEY_TARGET_FILE_NAME);
    UINT width = keyTarget->GetWidth(), height = keyTarget->GetHeight();
    m_Image = new Bitmap(width, height);
    Graphics graphics(m_Image);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.DrawImage(keyTarget, PointF(0, 0));
    delete keyTarget;
    RECT rect;
    GetWindowRect(&rect);
    if (rect.right - rect.left < width)
    {
        rect.right = rect.left + width;
    }
    if (rect.bottom - rect.top < height)
    {
        rect.bottom = rect.top + height;
    }
    MoveWindow(&rect);
}

BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CZoomDlg 消息处理程序

void CZoomDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CStatic::OnPaint()
    if (m_Image)
    {
        RECT rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);

        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        graphics.DrawImage(m_Image, PointF(0, 0));
    }
}

BOOL CZoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZoomDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}
