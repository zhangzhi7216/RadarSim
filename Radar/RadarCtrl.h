#pragma once
#include "afxwin.h"
#include "RadarParam.h"

class __declspec(dllexport) CRadarCtrl :
    public CStatic
{
public:
    CRadarCtrl(RadarParam &param);
    ~CRadarCtrl(void);

private:
    RadarParam &m_Param;

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
};
