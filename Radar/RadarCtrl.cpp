#include "StdAfx.h"
#include "RadarCtrl.h"

#include <math.h>

CRadarCtrl::CRadarCtrl(Sensor &radar)
: m_Radar(radar)
, m_Image(0)
, m_BackgroundImg(NULL)
, m_TargetsImg(NULL)
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
    if (m_TargetsImg)
    {
        delete m_TargetsImg;
        m_TargetsImg = NULL;
    }
    if (m_ScanlineImg)
    {
        delete m_ScanlineImg;
        m_ScanlineImg = NULL;
    }
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

void CRadarCtrl::DrawTargets()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int size = min(rect.right - rect.left, rect.bottom - rect.top);

    Image *targetsImg = new Bitmap(size, size);

    Graphics graphics(targetsImg);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeBicubic);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    for (map<int, Target>::iterator it = m_Radar.m_Plane.m_Targets.begin();
        it != m_Radar.m_Plane.m_Targets.end();
        ++it)
    {
        Pen pen(Target::TargetColors[it->second.m_Color]);
        if (m_Radar.ShowTrack)
        {
            for (int i = 1; i < m_Radar.m_Plane.m_RelPositionPaths[it->first].size(); ++i)
            {
                if (m_Radar.m_Plane.m_DistancePaths[it->first][i - 1] <= m_Radar.MaxDis && m_Radar.m_Plane.m_DistancePaths[it->first][i] <= m_Radar.MaxDis)
                {
                    graphics.DrawLine(&pen, PointF(m_Radar.m_Plane.m_ThetaPaths[it->first][i - 1], m_Radar.m_Plane.m_PhiPaths[it->first][i - 1]), PointF(m_Radar.m_Plane.m_ThetaPaths[it->first][i], m_Radar.m_Plane.m_PhiPaths[it->first][i]));
                }
            }
        }
        if (m_Radar.m_Plane.m_DistancePaths[it->first].size() > 0 && m_Radar.m_Plane.m_DistancePaths[it->first].back() <= m_Radar.MaxDis)
        {
            SolidBrush brush(Target::TargetColors[it->second.m_Color]);
            graphics.FillEllipse(&brush, m_Radar.m_Plane.m_ThetaPaths[it->first].back() - 2, m_Radar.m_Plane.m_PhiPaths[it->first].back() - 2, 4, 4);
            CString str;
            str.AppendFormat(TEXT("%d"), m_Radar.m_Plane.m_RelPositionPaths[it->first].back().Z);
            Font font(TEXT("宋体"), 10);
            graphics.DrawString(str, str.GetLength(), &font, PointF(m_Radar.m_Plane.m_ThetaPaths[it->first].back(), m_Radar.m_Plane.m_PhiPaths[it->first].back()), &brush);
        }
    }

    if (m_TargetsImg)
    {
        delete m_TargetsImg;
    }
    m_TargetsImg = targetsImg;
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
    
    if (m_Radar.Enable)
    {
        graphics.DrawImage(m_TargetsImg, Point(0, 0));
        if (m_Radar.ShowScanline)
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
    ON_WM_CREATE()
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
    DrawTargets();
    BlendAll();
}

void CRadarCtrl::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();

    SetTimer(0, 50, NULL);
}
