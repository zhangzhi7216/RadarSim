// ZoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZoomDlg.h"


// CZoomDlg �Ի���

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


// CZoomDlg ��Ϣ�������

void CZoomDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_Initialized = true;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZoomDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}
