#pragma once

struct __declspec(dllexport) RadarParam
{
public:
    RadarParam(void);
    virtual ~RadarParam(void);

    BOOL Enable;
    DOUBLE ScanLineAngle;
};
