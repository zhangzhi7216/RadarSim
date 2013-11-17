#pragma once

struct RadarParam
{
public:
    RadarParam(void);
    virtual ~RadarParam(void);

    BOOL Enable;
    DOUBLE ScanLineAngle;
};
