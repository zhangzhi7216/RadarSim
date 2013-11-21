#pragma once
#include "afxwin.h"
#include "Sensor.h"

#include <GdiPlus.h>
using namespace Gdiplus;

class __declspec(dllexport) CRadarCtrl :
    public CStatic
{
public:
    CRadarCtrl(Sensor &radar);
    ~CRadarCtrl(void);

public:
    Sensor &m_Radar;

    Image *m_Image;
    Image *m_BackgroundImg;
    Image *m_TargetsImg;
    Image *m_ScanlineImg;
    int m_CurrentAngle;

    void DrawBackground();
    void DrawScanline();
    void DrawTargets();
    void BlendAll();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void PreSubclassWindow();
};
