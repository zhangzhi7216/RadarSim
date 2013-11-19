#include "StdAfx.h"
#include "RadarCtrl.h"

#include <math.h>

CRadarCtrl::CRadarCtrl(RadarParam &param)
: m_Param(param)
, m_Image(0)
, m_BackgroundImg(NULL)
, m_ScanlineImg(NULL)
, m_CurrentAngle(0)
{
}

CRadarCtrl::~CRadarCtrl(void)
{
    if (m_Image)
    {
        delete m_Image;
        m_Image = NULL;
    }
    if (m_BackgroundImg)
    {
        delete m_BackgroundImg;
        m_BackgroundImg = NULL;
    }
    if (m_ScanlineImg)
    {
        delete m_ScanlineImg;
        m_ScanlineImg = NULL;
    }
}

bool CRadarCtrl::Init()
{
    SetTimer(0, 50, NULL);

    DrawBackground();
    DrawScanline();
    BlendAll();

    return true;
}

void CRadarCtrl::DrawBackground()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int size = min(rect.right - rect.left, rect.bottom - rect.top);

    Image *backgroundImg = new Bitmap(size, size);

    Color topColor(0, 120, 0);
    Color bottomColor(0, 40, 0);
    Color lineColor(0, 255, 0);

    Graphics graphics(backgroundImg);
    Pen pen(lineColor);

    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeBicubic);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    graphics.FillEllipse(&LinearGradientBrush(Point(size / 2, 0), Point(size / 2, size - 1), topColor, bottomColor), 0, 0, size - 1, size - 1);

    graphics.DrawEllipse(&pen, 0, 0, size - 1, size - 1);

    int interval = size / 3;
    graphics.DrawEllipse(&pen, (size - interval) / 2, (size - interval) / 2, interval, interval);

    interval *= 2;
    graphics.DrawEllipse(&pen, (size - interval) / 2, (size - interval) / 2, interval, interval);

    graphics.DrawLine(&pen, Point(0, size / 2), Point(size - 1, size / 2));
    graphics.DrawLine(&pen, Point(size / 2, 0), Point(size / 2, size - 1));

    if (m_BackgroundImg)
    {
        delete m_BackgroundImg;
    }
    m_BackgroundImg = backgroundImg;
}

PointF AzEl2XY(int size, int azimuth, int elevation)
{
    double angle = (270.0 + (double)azimuth);

    angle *= 0.0174532925;

    double r, x, y;

    r = (double)size * 0.5;
    r -= (r * (double)elevation / 90);

    x = (((double)size * 0.5) + (r * cos(angle)));
    y = (((double)size * 0.5) + (r * sin(angle)));

    return PointF((float)x, (float)y);
}

void CRadarCtrl::DrawScanline()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int size = min(rect.right - rect.left, rect.bottom - rect.top);

    Image *scanlineImg = new Bitmap(size, size);

    PointF pt = AzEl2XY(size, m_CurrentAngle, 0);
    PointF pt2 = AzEl2XY(size, m_CurrentAngle - 20, 0);
    PointF pt3 = AzEl2XY(size, m_CurrentAngle - 10, -10);

    Graphics graphics(scanlineImg);
    Color lineColor(0, 255, 0);
    GraphicsPath gp(FillModeWinding);
    gp.AddLine(PointF((float)size / 2, (float)size / 2), pt2);
    PointF pts[] = { pt2, pt3, pt };
    gp.AddCurve(pts, 3);
    gp.AddLine(pt, PointF((float)size / 2, (float)size / 2));
    PathGradientBrush pgb(&gp);
    pgb.SetCenterPoint(pt);
    pgb.SetCenterColor(Color(128, lineColor.GetR(), lineColor.GetG(), lineColor.GetB()));
    Color colors[] = { Color(0, 0, 0, 0) };
    int count = 1;
    pgb.SetSurroundColors(colors, &count);
    graphics.FillPath(&pgb, &gp);
    graphics.DrawLine(&Pen(lineColor), PointF((float)size / 2, (float)size / 2), pt);

    if (m_ScanlineImg)
    {
        delete m_ScanlineImg;
    }
    m_ScanlineImg = scanlineImg;
}

void CRadarCtrl::BlendAll()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int size = min(rect.right - rect.left, rect.bottom - rect.top);

    Image *img = new Bitmap(size, size);
    Graphics graphics(img);
    GraphicsPath pathRadar;
    pathRadar.SetFillMode(FillModeWinding);
    pathRadar.AddEllipse(-1.0, -1.0, (float)(size + 1), (float)(size + 1));
    graphics.SetClip(&Region(&pathRadar));
    graphics.DrawImage(m_BackgroundImg, Point(0, 0));
    
    if (m_Param.Enable)
    {
        if (m_Param.ShowScanline)
        {
            graphics.DrawImage(m_ScanlineImg, Point(0, 0));
        }
    }

    if (m_Image)
    {
        delete m_Image;
    }
    m_Image = img;
}

BEGIN_MESSAGE_MAP(CRadarCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_SIZE()
END_MESSAGE_MAP()

void CRadarCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CStatic::OnPaint()
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int width = rect.right - rect.left, height = rect.bottom - rect.top;
    int left = rect.left, top = rect.top;
    if (width >= height)
    {
        left += (width - height) / 2;
    }
    else
    {
        top += (height - width) / 2;
    }

    if (m_Image)
    {
        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        graphics.DrawImage(m_Image, Point(left, top));
    }
}

void CRadarCtrl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CStatic::OnTimer(nIDEvent);

    m_CurrentAngle++;
    if (m_CurrentAngle >= 360)
    {
        m_CurrentAngle = 0;
    }

    DrawScanline();
    BlendAll();

    Invalidate();
}

void CRadarCtrl::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    DrawBackground();
    DrawScanline();
    BlendAll();
}
