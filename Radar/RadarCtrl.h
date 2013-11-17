#pragma once
#include "afxwin.h"
#include "RadarParam.h"

class RadarCtrl :
    public CStatic
{
public:
    RadarCtrl(RadarParam &param);
    ~RadarCtrl(void);

private:
    RadarParam &m_Param;

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
};
