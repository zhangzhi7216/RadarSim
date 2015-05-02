#include "StdAfx.h"
#include "StateMapCtrl.h"
#include "Utility.h"
using namespace Utility;

#include <math.h>

CStateMapCtrl::CStateMapCtrl(StateMap &stateMap)
: m_StateMap(stateMap)
, m_Image(0)
, m_OldBackgroundImg(NULL)
, m_BackgroundImg(NULL)
, m_TargetsImg(NULL)
{
    m_BackgroundImg = StateMapBackgrounds[m_StateMap.m_Background];
}

CStateMapCtrl::~CStateMapCtrl(void)
{
    if (m_Image)
    {
        delete m_Image;
        m_Image = NULL;
    }
    // Don't delete bkg. GlobalShut() will do.
    /*
    if (m_BackgroundImg)
    {
        delete m_BackgroundImg;
        m_BackgroundImg = NULL;
    }
    */
    if (m_TargetsImg)
    {
        delete m_TargetsImg;
        m_TargetsImg = NULL;
    }
}

void CStateMapCtrl::DrawBackground()
{
    m_OldBackgroundImg = m_BackgroundImg;
    m_BackgroundImg = StateMapBackgrounds[m_StateMap.m_Background];
}

void CStateMapCtrl::DrawTargets()
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

    for (int i = 0; i < m_StateMap.m_Planes.size(); ++i)
    {
        Pen pen(TargetColors[m_StateMap.m_Planes[i].m_Color], TARGET_TRACK_WIDTH);
        if (m_StateMap.m_ShowTrack)
        {
            for (int j = 1; j < m_StateMap.m_PlanePaths[i].size(); ++j)
            {
                PointF pt0(m_StateMap.m_PlanePaths[i][j - 1].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_PlanePaths[i][j - 1].Y / m_StateMap.m_MaxY * (double)height);
                PointF pt1(m_StateMap.m_PlanePaths[i][j].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_PlanePaths[i][j].Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawLine(&pen, pt0, pt1);
            }
        }

        if (m_StateMap.m_PlanePaths[i].size() > 0)
        {
            Position end = m_StateMap.m_PlanePaths[i].back();
            if (m_StateMap.m_Planes[i].m_Vel.X == 0
                && m_StateMap.m_Planes[i].m_Vel.Y == 0
                && m_StateMap.m_Planes[i].m_Vel.Z == 0)
            {
                if (m_StateMap.m_PlanePaths[i].size() > 1)
                {
                    Position start = m_StateMap.m_PlanePaths[i][m_StateMap.m_PlanePaths[i].size() - 2];
                    Position physRel = Position((end - start).X / m_StateMap.m_MaxX * (double)width, (end - start).Y / m_StateMap.m_MaxY * (double)height, 0.0);
                    double angle = -Theta(physRel);
                    if (end.X < start.X)
                    {
                        angle += 180;
                    }
                    graphics.RotateTransform(angle, MatrixOrderAppend);
                }
            }
            else
            {
                Position physDir = Position(m_StateMap.m_Planes[i].m_Vel.X / m_StateMap.m_MaxX * (double)width, m_StateMap.m_Planes[i].m_Vel.Y / m_StateMap.m_MaxY * (double)height, 0.0);
                double angle = -Theta(physDir);
                if (physDir.X < 0)
                {
                    angle += 180;
                }
                graphics.RotateTransform(angle, MatrixOrderAppend);
            }
            graphics.TranslateTransform(end.X / m_StateMap.m_MaxX * (double)width, (double)height - end.Y / m_StateMap.m_MaxY * (double)height, MatrixOrderAppend);
            Image *planeImg = TargetTypeImages[m_StateMap.m_Planes[i].m_Type];
            PointF pt(0.0, 0.0);
            graphics.DrawImage(planeImg, PointF(pt.X - (double)planeImg->GetWidth() / 2.0, pt.Y - (double)planeImg->GetHeight() / 2.0));
            if (m_StateMap.m_ShowThetaRange &&
                (m_StateMap.m_Planes[i].m_Type != TargetTypeFighter &&
                 m_StateMap.m_Planes[i].m_Type != TargetTypeBomber))
            {
                if (m_StateMap.m_Planes[i].m_Radar.m_Enable)
                {
                    Pen pen(m_StateMap.m_Planes[i].m_Radar.m_ThetaRangeColor);
                    graphics.DrawPie(&pen,
                        (float)(pt.X - m_StateMap.m_Planes[i].m_Radar.m_MaxDis / m_StateMap.m_MaxX * (double)width),
                        (float)(pt.Y - m_StateMap.m_Planes[i].m_Radar.m_MaxDis / m_StateMap.m_MaxY * (double)height),
                        (float)(m_StateMap.m_Planes[i].m_Radar.m_MaxDis / m_StateMap.m_MaxX * (double)width * 2.0),
                        (float)(m_StateMap.m_Planes[i].m_Radar.m_MaxDis / m_StateMap.m_MaxY * (double)height * 2.0),
                        -m_StateMap.m_Planes[i].m_Radar.m_MaxTheta / 2.0,
                        m_StateMap.m_Planes[i].m_Radar.m_MaxTheta);
                }
                if (m_StateMap.m_Planes[i].m_Esm.m_Enable)
                {
                    Pen pen(m_StateMap.m_Planes[i].m_Esm.m_ThetaRangeColor);
                    graphics.DrawPie(&pen,
                        (float)(pt.X - m_StateMap.m_Planes[i].m_Esm.m_MaxDis / m_StateMap.m_MaxX * (double)width),
                        (float)(pt.Y - m_StateMap.m_Planes[i].m_Esm.m_MaxDis / m_StateMap.m_MaxY * (double)height),
                        (float)(m_StateMap.m_Planes[i].m_Esm.m_MaxDis / m_StateMap.m_MaxX * (double)width * 2.0),
                        (float)(m_StateMap.m_Planes[i].m_Esm.m_MaxDis / m_StateMap.m_MaxY * (double)height * 2.0),
                        -m_StateMap.m_Planes[i].m_Esm.m_MaxTheta / 2.0,
                        m_StateMap.m_Planes[i].m_Esm.m_MaxTheta);
                }
                if (m_StateMap.m_Planes[i].m_Tong.m_Enable)
                {
                    Pen pen(m_StateMap.m_Planes[i].m_Tong.m_ThetaRangeColor);
                    graphics.DrawPie(&pen,
                        (float)(pt.X - m_StateMap.m_Planes[i].m_Tong.m_MaxDis / m_StateMap.m_MaxX * (double)width),
                        (float)(pt.Y - m_StateMap.m_Planes[i].m_Tong.m_MaxDis / m_StateMap.m_MaxY * (double)height),
                        (float)(m_StateMap.m_Planes[i].m_Tong.m_MaxDis / m_StateMap.m_MaxX * (double)width * 2.0),
                        (float)(m_StateMap.m_Planes[i].m_Tong.m_MaxDis / m_StateMap.m_MaxY * (double)height * 2.0),
                        -m_StateMap.m_Planes[i].m_Tong.m_MaxTheta / 2.0,
                        m_StateMap.m_Planes[i].m_Tong.m_MaxTheta);
                }
                if (m_StateMap.m_Planes[i].m_Lei.m_Enable)
                {
                    Pen pen(m_StateMap.m_Planes[i].m_Lei.m_ThetaRangeColor);
                    graphics.DrawPie(&pen,
                        (float)(pt.X - m_StateMap.m_Planes[i].m_Lei.m_MaxDis / m_StateMap.m_MaxX * (double)width),
                        (float)(pt.Y - m_StateMap.m_Planes[i].m_Lei.m_MaxDis / m_StateMap.m_MaxY * (double)height),
                        (float)(m_StateMap.m_Planes[i].m_Lei.m_MaxDis / m_StateMap.m_MaxX * (double)width * 2.0),
                        (float)(m_StateMap.m_Planes[i].m_Lei.m_MaxDis / m_StateMap.m_MaxY * (double)height * 2.0),
                        -m_StateMap.m_Planes[i].m_Lei.m_MaxTheta / 2.0,
                        m_StateMap.m_Planes[i].m_Lei.m_MaxTheta);
                }
            }

            graphics.ResetTransform();

            if (m_StateMap.m_ShowHeight)
            {
                SolidBrush brush(TargetColors[m_StateMap.m_Planes[i].m_Color]);
                CString str;
                str.AppendFormat(TEXT("%d"), (int)m_StateMap.m_PlanePaths[i].back().Z);
                Font font(TEXT("Calibri"), 9);
                PointF pt(m_StateMap.m_PlanePaths[i].back().X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_PlanePaths[i].back().Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawString(str, str.GetLength(), &font, PointF(pt.X, pt.Y - planeImg->GetHeight()), &brush);
            }
        }
    }

    for (int i = 0; i < m_StateMap.m_Targets.size(); ++i)
    {
        if (m_StateMap.m_ShowTrack)
        {
            Pen pen(TargetColors[m_StateMap.m_Targets[i].m_Color], TARGET_TRACK_WIDTH);
            for (int j = 1; j < m_StateMap.m_TargetPaths[i].size(); ++j)
            {
                if ((m_StateMap.m_TargetPaths[i][j - 1].X == 0 &&
                    m_StateMap.m_TargetPaths[i][j - 1].Y == 0 &&
                    m_StateMap.m_TargetPaths[i][j - 1].Z == 0) ||
                    (m_StateMap.m_TargetPaths[i][j].X == 0 &&
                    m_StateMap.m_TargetPaths[i][j].Y == 0 &&
                    m_StateMap.m_TargetPaths[i][j].Z == 0))
                {
                    continue;
                }
                PointF pt0(m_StateMap.m_TargetPaths[i][j - 1].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_TargetPaths[i][j - 1].Y / m_StateMap.m_MaxY * (double)height);
                PointF pt1(m_StateMap.m_TargetPaths[i][j].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_TargetPaths[i][j].Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawLine(&pen, pt0, pt1);
            }
        }
        if (m_StateMap.m_TargetPaths[i].size() > 0 &&
            !(m_StateMap.m_TargetPaths[i].back().X == 0 &&
              m_StateMap.m_TargetPaths[i].back().Y == 0 &&
              m_StateMap.m_TargetPaths[i].back().Z == 0))
        {
            Position end = m_StateMap.m_TargetPaths[i].back();
            if (m_StateMap.m_Targets[i].m_Vel.X == 0
                && m_StateMap.m_Targets[i].m_Vel.Y == 0
                && m_StateMap.m_Targets[i].m_Vel.Z == 0)
            {
                if (m_StateMap.m_TargetPaths[i].size() > 1)
                {
                    Position start = m_StateMap.m_TargetPaths[i][m_StateMap.m_TargetPaths[i].size() - 2];
                    Position physRel = Position((end - start).X / m_StateMap.m_MaxX * (double)width, (end - start).Y / m_StateMap.m_MaxY * (double)height, 0.0);
                    double angle = -Theta(physRel);
                    if (end.X < start.X)
                    {
                        angle += 180;
                    }
                    graphics.RotateTransform(angle, MatrixOrderAppend);
                }
            }
            else
            {
                Position physDir = Position(m_StateMap.m_Targets[i].m_Vel.X / m_StateMap.m_MaxX * (double)width, m_StateMap.m_Targets[i].m_Vel.Y / m_StateMap.m_MaxY * (double)height, 0.0);
                double angle = -Theta(physDir);
                if (physDir.X < 0)
                {
                    angle += 180;
                }
                graphics.RotateTransform(angle, MatrixOrderAppend);
            }
            graphics.TranslateTransform(end.X / m_StateMap.m_MaxX * (double)width, (double)height - end.Y / m_StateMap.m_MaxY * (double)height, MatrixOrderAppend);

            Image *targetImg = m_StateMap.m_Targets[i].m_State == TargetStateExploding ?
                ExplosionTypeImages[m_StateMap.m_ExplosionType] : TargetTypeImages[m_StateMap.m_Targets[i].m_Type];
#ifdef _DEV
            if (m_StateMap.m_Targets[i].m_State == TargetStateExploding)
            {
                Image *targetImg = m_StateMap.m_Targets[i].m_State == TargetStateExploding ?
                    ExplosionTypeImages[m_StateMap.m_ExplosionType] : TargetTypeImages[m_StateMap.m_Targets[i].m_Type];
            }
#endif
            if (m_StateMap.m_Targets[i].m_State != TargetStateDestroyed)
            {
                PointF pt(0.0, 0.0);
                graphics.DrawImage(targetImg, PointF(pt.X - (double)targetImg->GetWidth() / 2.0, pt.Y - (double)targetImg->GetHeight() / 2.0));
            }

            graphics.ResetTransform();

            if (m_StateMap.m_ShowHeight)
            {
                SolidBrush brush(TargetColors[m_StateMap.m_Targets[i].m_Color]);
                CString str;
                str.AppendFormat(TEXT("%d"), (int)m_StateMap.m_TargetPaths[i].back().Z);
                Font font(TEXT("Calibri"), 9);
                PointF pt(m_StateMap.m_TargetPaths[i].back().X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_TargetPaths[i].back().Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawString(str, str.GetLength(), &font, PointF(pt.X, pt.Y - targetImg->GetHeight()), &brush);
            }
        }
    }

    for (int i = 0; i < m_StateMap.m_MissilePaths.size(); ++i)
    {
        if (m_StateMap.m_ShowTrack)
        {
            Pen pen(TargetColors[m_StateMap.m_Missiles[i].m_Color], TARGET_TRACK_WIDTH);
            for (int j = 1; j < m_StateMap.m_MissilePaths[i].size(); ++j)
            {
                if ((m_StateMap.m_MissilePaths[i][j - 1].X == 0 &&
                    m_StateMap.m_MissilePaths[i][j - 1].Y == 0 &&
                    m_StateMap.m_MissilePaths[i][j - 1].Z == 0) ||
                    (m_StateMap.m_MissilePaths[i][j].X == 0 &&
                    m_StateMap.m_MissilePaths[i][j].Y == 0 &&
                    m_StateMap.m_MissilePaths[i][j].Z == 0))
                {
                    continue;
                }
                PointF pt0(m_StateMap.m_MissilePaths[i][j - 1].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_MissilePaths[i][j - 1].Y / m_StateMap.m_MaxY * (double)height);
                PointF pt1(m_StateMap.m_MissilePaths[i][j].X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_MissilePaths[i][j].Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawLine(&pen, pt0, pt1);
            }
        }
        if (m_StateMap.m_MissilePaths[i].size() > 0 &&
            !(m_StateMap.m_MissilePaths[i].back().X == 0 &&
              m_StateMap.m_MissilePaths[i].back().Y == 0 &&
              m_StateMap.m_MissilePaths[i].back().Z == 0))
        {
            Position end = m_StateMap.m_MissilePaths[i].back();
            if (m_StateMap.m_Missiles[i].m_Vel.X == 0
                && m_StateMap.m_Missiles[i].m_Vel.Y == 0
                && m_StateMap.m_Missiles[i].m_Vel.Z == 0)
            {
                if (m_StateMap.m_MissilePaths[i].size() > 1)
                {
                    Position start = m_StateMap.m_MissilePaths[i][m_StateMap.m_MissilePaths[i].size() - 2];
                    Position physRel = Position((end - start).X / m_StateMap.m_MaxX * (double)width, (end - start).Y / m_StateMap.m_MaxY * (double)height, 0.0);
                    double angle = -Theta(physRel);
                    if (end.X < start.X)
                    {
                        angle += 180;
                    }
                    graphics.RotateTransform(angle, MatrixOrderAppend);
                }
            }
            else
            {
                Position physDir = Position(m_StateMap.m_Missiles[i].m_Vel.X / m_StateMap.m_MaxX * (double)width, m_StateMap.m_Missiles[i].m_Vel.Y / m_StateMap.m_MaxY * (double)height, 0.0);
                double angle = -Theta(physDir);
                if (physDir.X < 0)
                {
                    angle += 180;
                }
                graphics.RotateTransform(angle, MatrixOrderAppend);
            }
                graphics.TranslateTransform(end.X / m_StateMap.m_MaxX * (double)width, (double)height - end.Y / m_StateMap.m_MaxY * (double)height, MatrixOrderAppend);
            Image *targetImg = TargetTypeImages[m_StateMap.m_Missiles[i].m_Type];// m_StateMap.m_Missiles[i].m_State == TargetStateExploding ?
                // ExplosionTypeImages[m_StateMap.m_ExplosionType] : TargetTypeImages[m_StateMap.m_Missiles[i].m_Type];
#ifdef _DEV
            if (m_StateMap.m_Missiles[i].m_State == TargetStateExploding)
            {
                Image *targetImg = m_StateMap.m_Missiles[i].m_State == TargetStateExploding ?
                    ExplosionTypeImages[m_StateMap.m_ExplosionType] : TargetTypeImages[m_StateMap.m_Missiles[i].m_Type];
            }
#endif
            if (m_StateMap.m_Missiles[i].m_State != TargetStateDestroyed)
            {
                PointF pt(0.0, 0.0);
                graphics.DrawImage(targetImg, PointF(pt.X - (double)targetImg->GetWidth() / 2.0, pt.Y - (double)targetImg->GetHeight() / 2.0));
            }

            graphics.ResetTransform();

            if (m_StateMap.m_ShowHeight)
            {
                SolidBrush brush(TargetColors[m_StateMap.m_Missiles[i].m_Color]);
                CString str;
                str.AppendFormat(TEXT("%d"), (int)m_StateMap.m_MissilePaths[i].back().Z);
                Font font(TEXT("Calibri"), 9);
                PointF pt(m_StateMap.m_MissilePaths[i].back().X / m_StateMap.m_MaxX * (double)width,
                    (double)height - m_StateMap.m_MissilePaths[i].back().Y / m_StateMap.m_MaxY * (double)height);
                graphics.DrawString(str, str.GetLength(), &font, PointF(pt.X, pt.Y - targetImg->GetHeight()), &brush);
            }
        }
    }

    if (m_TargetsImg)
    {
        delete m_TargetsImg;
    }
    m_TargetsImg = targetsImg;
}

void CStateMapCtrl::BlendAll()
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
    graphics.DrawImage(m_TargetsImg, Point(0, 0));

    if (m_Image)
    {
        delete m_Image;
    }
    m_Image = img;
}

BEGIN_MESSAGE_MAP(CStateMapCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CStateMapCtrl::OnPaint()
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
        double left = rect.left + (double)(rect.right - rect.left - m_Image->GetWidth()) / 2,
            top = rect.top + (double)(rect.bottom - rect.top - m_Image->GetHeight()) / 2;
        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        if (m_OldBackgroundImg)
        {
            Color color;
            color.SetFromCOLORREF(GetSysColor(COLOR_3DFACE));
            graphics.Clear(color);
            m_OldBackgroundImg = NULL;
        }
        graphics.DrawImage(m_Image, PointF(left, top));
    }
}

void CStateMapCtrl::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    DrawBackground();
    DrawTargets();
    BlendAll();
}

void CStateMapCtrl::Reset()
{
    DrawBackground();
    BlendAll();
    Invalidate();
}

void CStateMapCtrl::AddTarget(Target &target)
{
}

void CStateMapCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    double absX = point.x / m_StateMap.m_MaxX;
    double absY = point.y / m_StateMap.m_MaxY;

    m_StateMap.ZoomKeyTarget(absX, absY);

    CStatic::OnRButtonDown(nFlags, point);
}
