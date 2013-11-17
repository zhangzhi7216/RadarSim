#include "StdAfx.h"
#include "RadarCtrl.h"

CRadarCtrl::CRadarCtrl(RadarParam &param)
: m_Param(param)
{
}

CRadarCtrl::~CRadarCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CRadarCtrl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CRadarCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CStatic::OnPaint()
    RECT rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);

    CBrush b;
    b.CreateSolidBrush(RGB(255, 0, 0));
    dc.FillRect(&rect, &b);
}
