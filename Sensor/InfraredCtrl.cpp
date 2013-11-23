#include "StdAfx.h"
#include "InfraredCtrl.h"

#include <math.h>

CInfraredCtrl::CInfraredCtrl(Sensor &sensor)
: CSensorCtrl(sensor)
{
}

CInfraredCtrl::~CInfraredCtrl(void)
{
}

void CInfraredCtrl::DrawBackground()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    Image *backgroundImg = new Bitmap(rect.right - rect.left, rect.bottom - rect.top);

    Graphics graphics(backgroundImg);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    Image *img = Image::FromFile(TEXT("InfraredBackground.bmp"));
    graphics.DrawImage(img, 0, 0, rect.right - rect.left, rect.bottom - rect.top);

    if (m_BackgroundImg)
    {
        delete m_BackgroundImg;
    }
    m_BackgroundImg = backgroundImg;
}

void CInfraredCtrl::DrawTargets()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    Image *targetsImg = new Bitmap(rect.right - rect.left, rect.bottom - rect.top);

    Graphics graphics(targetsImg);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    for (int i = 0; i < m_Sensor.m_Plane.m_Targets.size(); ++i)
    {
        Pen pen(TargetColors[m_Sensor.m_TargetColors[i]], TARGET_TRACK_WIDTH);
        if (m_Sensor.m_ShowTrack)
        {
            for (int j = 1; j < m_Sensor.m_Plane.m_RelPositionPaths[i].size(); ++j)
            {
                if (m_Sensor.m_Plane.m_DistancePaths[i][j - 1] <= m_Sensor.m_MaxDis && m_Sensor.m_Plane.m_DistancePaths[i][j] <= m_Sensor.m_MaxDis)
                {
                    graphics.DrawLine(&pen, PointF(m_Sensor.m_Plane.m_ThetaPaths[i][j - 1], m_Sensor.m_Plane.m_PhiPaths[i][j - 1]), PointF(m_Sensor.m_Plane.m_ThetaPaths[i][j], m_Sensor.m_Plane.m_PhiPaths[i][j]));
                }
            }
        }
        if (m_Sensor.m_Plane.m_DistancePaths[i].size() > 0 && m_Sensor.m_Plane.m_DistancePaths[i].back() <= m_Sensor.m_MaxDis)
        {
            SolidBrush brush(TargetColors[m_Sensor.m_TargetColors[i]]);
            graphics.FillEllipse(&brush, m_Sensor.m_Plane.m_ThetaPaths[i].back() - TARGET_RADIUS, m_Sensor.m_Plane.m_PhiPaths[i].back() - TARGET_RADIUS, TARGET_RADIUS * 2, TARGET_RADIUS * 2);
            CString str;
            str.AppendFormat(TEXT("%d"), (int)(m_Sensor.m_Plane.m_RelPositionPaths[i].back().Z));
            Font font(TEXT("Calibri"), 9);
            graphics.DrawString(str, str.GetLength(), &font, PointF(m_Sensor.m_Plane.m_ThetaPaths[i].back(), m_Sensor.m_Plane.m_PhiPaths[i].back() - TARGET_TITLE_OFFSET), &brush);
        }
    }

    if (m_TargetsImg)
    {
        delete m_TargetsImg;
    }
    m_TargetsImg = targetsImg;
}

void CInfraredCtrl::BlendAll()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    Image *img = new Bitmap(rect.right - rect.left, rect.bottom - rect.top);
    Graphics graphics(img);
    graphics.DrawImage(m_BackgroundImg, Point(0, 0));
    
    if (m_Sensor.m_Enable)
    {
        graphics.DrawImage(m_TargetsImg, Point(0, 0));
    }

    if (m_Image)
    {
        delete m_Image;
    }
    m_Image = img;
}

BEGIN_MESSAGE_MAP(CInfraredCtrl, CSensorCtrl)
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()

void CInfraredCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CStatic::OnPaint()
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    int width = rect.right - rect.left, height = rect.bottom - rect.top;
    int left = rect.left, top = rect.top;

    if (m_Image)
    {
        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        graphics.DrawImage(m_Image, Point(left, top));
    }
}

void CInfraredCtrl::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    DrawBackground();
    DrawScanline();
    DrawTargets();
    BlendAll();
}

void CInfraredCtrl::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
}

void CInfraredCtrl::Reset()
{
    DrawBackground();
    DrawTargets();
    BlendAll();
    Invalidate();
}

void CInfraredCtrl::AddTarget(Target &target)
{
}
