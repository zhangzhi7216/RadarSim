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
            if (m_StateMap.m_ShowThetaRange)
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
                if (m_StateMap.m_Planes[i].m_Ais.m_Enable)
                {
                    Pen pen(m_StateMap.m_Planes[i].m_Ais.m_ThetaRangeColor);
                    graphics.DrawPie(&pen,
                        (float)(pt.X - m_StateMap.m_Planes[i].m_Ais.m_MaxDis / m_StateMap.m_MaxX * (double)width),
                        (float)(pt.Y - m_StateMap.m_Planes[i].m_Ais.m_MaxDis / m_StateMap.m_MaxY * (double)height),
                        (float)(m_StateMap.m_Planes[i].m_Ais.m_MaxDis / m_StateMap.m_MaxX * (double)width * 2.0),
                        (float)(m_StateMap.m_Planes[i].m_Ais.m_MaxDis / m_StateMap.m_MaxY * (double)height * 2.0),
                        -m_StateMap.m_Planes[i].m_Ais.m_MaxTheta / 2.0,
                        m_StateMap.m_Planes[i].m_Ais.m_MaxTheta);
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

    // Targets.
    for (map<int, Target>::iterator it = m_StateMap.m_Targets.begin(); it != m_StateMap.m_Targets.end(); it++)
    {
        int i = it->first;
        if (m_StateMap.m_ShowTrack)
        {
            Pen pen(TargetColors[m_StateMap.m_Targets[i].m_Color], TARGET_TRACK_WIDTH);
            // for (map<int, Path>::iterator it = m_StateMap.m_TargetPaths.begin(); it != m_StateMap.m_TargetPaths.end(); it++)
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

            Image *targetImg = TargetTypeImages[m_StateMap.m_Targets[i].m_Type];
            Image *targetMaskImg = TargetTypeMaskImages[m_StateMap.m_Targets[i].m_Type];
#ifdef _DEV
            if (m_StateMap.m_Targets[i].m_State == TargetStateExploding)
            {
                Image *targetImg = m_StateMap.m_Targets[i].m_State == TargetStateExploding ?
                    ExplosionTypeImages[m_StateMap.m_ExplosionType] : TargetTypeImages[m_StateMap.m_Targets[i].m_Type];
            }
#endif
            PointF pt(0.0, 0.0);
            graphics.DrawImage(targetImg, PointF(pt.X - (double)targetImg->GetWidth() / 2.0, pt.Y - (double)targetImg->GetHeight() / 2.0));
            if (m_StateMap.m_Targets[i].m_IsKeyTarget)
            {
                graphics.DrawImage(targetMaskImg, PointF(pt.X - (double)targetImg->GetWidth() / 2.0, pt.Y - (double)targetImg->GetHeight() / 2.0));
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

    // if (m_StateMap.m_ZoomKeyTargetId != -1)
    // {
    //     Image *keyTarget = Image::FromFile(KEY_TARGET_FILE_NAME);
    //     PointF pt(m_StateMap.m_TargetPaths[m_StateMap.m_ZoomKeyTargetId].back().X / m_StateMap.m_MaxX * (double)width,
    //         (double)height - m_StateMap.m_TargetPaths[m_StateMap.m_ZoomKeyTargetId].back().Y / m_StateMap.m_MaxY * (double)height);
    //     graphics.DrawImage(keyTarget, PointF(pt.X - (double)keyTarget->GetWidth() / 2.0, pt.Y - (double)keyTarget->GetHeight() / 2.0));
    //     delete keyTarget;
    // }

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
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
    if (m_Image)
    {
        RECT rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);

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

    // TODO: �ڴ˴������Ϣ����������
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
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (m_Image)
    {
        RECT rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);

        double left = rect.left + (double)(rect.right - rect.left - m_Image->GetWidth()) / 2,
            top = rect.top + (double)(rect.bottom - rect.top - m_Image->GetHeight()) / 2;
        double width = m_Image->GetWidth(),
            height = m_Image->GetHeight();
        point.x -= left;
        point.y -= top;
        double absX = (double)point.x / width * m_StateMap.m_MaxX;
        double absY = m_StateMap.m_MaxY - (double)point.y / height * m_StateMap.m_MaxY;

        m_StateMap.ZoomKeyTarget(absX, absY);
    }

    CStatic::OnRButtonDown(nFlags, point);
}
