#include "StdAfx.h"
#include "InfraredCtrl.h"

#include <math.h>

CInfraredCtrl::CInfraredCtrl(Sensor &sensor)
: CSensorCtrl(sensor)
{
    m_BackgroundImg = Image::FromFile(TEXT("InfraredBackground.bmp"));
}

CInfraredCtrl::~CInfraredCtrl(void)
{
}

void CInfraredCtrl::DrawBackground()
{
}

void CInfraredCtrl::DrawTargets()
{
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    UINT srcWidth = m_BackgroundImg->GetWidth(), srcHeight = m_BackgroundImg->GetHeight();
    int width = rect.right - rect.left, height = rect.bottom - rect.top;
    if ((double)width / (double)srcWidth > (double)height / (double)srcHeight)
    {
        width = height * ((double)srcWidth / (double)srcHeight);
    }
    else
    {
        height = width * ((double)srcHeight / (double)srcWidth);
    }

    Image *targetsImg = new Bitmap(width, height);

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

    UINT srcWidth = m_BackgroundImg->GetWidth(), srcHeight = m_BackgroundImg->GetHeight();
    int width = rect.right - rect.left, height = rect.bottom - rect.top;
    if ((double)width / (double)srcWidth > (double)height / (double)srcHeight)
    {
        width = height * ((double)srcWidth / (double)srcHeight);
    }
    else
    {
        height = width * ((double)srcHeight / (double)srcWidth);
    }

    Image *img = new Bitmap(width, height);
    Graphics graphics(img);
    graphics.DrawImage(m_BackgroundImg, 0, 0, width, height);
    
    if (m_Sensor.m_Enable)
    {
        if (m_TargetsImg != NULL)
        {
            graphics.DrawImage(m_TargetsImg, Point(0, 0));
        }
    }

    if (m_Image)
    {
        delete m_Image;
    }
    m_Image = img;
}

BEGIN_MESSAGE_MAP(CInfraredCtrl, CSensorCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CInfraredCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CStatic::OnPaint()
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (m_Image)
    {
        double left = rect.left + (double)(rect.right - rect.left - m_Image->GetWidth()) / 2,
            top = rect.top + (double)(rect.bottom - rect.top - m_Image->GetHeight()) / 2;
        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        graphics.DrawImage(m_Image, PointF(left, top));
    }
}

void CInfraredCtrl::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
}
