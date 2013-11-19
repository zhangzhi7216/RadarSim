#pragma once
#include "afxwin.h"
#include "RadarParam.h"

#include <GdiPlus.h>
using namespace Gdiplus;

class __declspec(dllexport) CRadarCtrl :
    public CStatic
{
public:
    CRadarCtrl(RadarParam &param);
    ~CRadarCtrl(void);

    bool Init();

public:
    RadarParam &m_Param;

    Image *m_Image;
    Image *m_BackgroundImg;
    Image *m_ScanlineImg;
    int m_CurrentAngle;

    void DrawBackground();
    void DrawScanline();
    void BlendAll();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
