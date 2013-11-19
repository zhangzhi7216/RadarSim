#pragma once

struct __declspec(dllexport) RadarParam
{
public:
    RadarParam(void);
    virtual ~RadarParam(void);

    CString Type;
    BOOL Enable;

    double MaxDis;
    double MaxTheta;
    double MaxPhi;

    double DisVar;
    double ThetaVar;
    double PhiVar;

    double ProDet;

    BOOL ShowScanline;
};
