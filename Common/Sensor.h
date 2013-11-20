#pragma once

#include <afxwin.h>
#include "Target.h"

class Sensor
{
public:
    enum SensorType
    {
        SensorTypeSource = 0,
        SensorTypeNonSource,
        SensorTypeLast,
    };

    static CString SensorTypeNames[SensorTypeLast];

    Sensor(SensorType type, Plane &plane);
    virtual ~Sensor(void);

    SensorType Type;
    BOOL Enable;

    double MaxDis;
    double MaxTheta;
    double MaxPhi;

    double DisVar;
    double ThetaVar;
    double PhiVar;

    double ProDet;

    BOOL ShowScanline;
    BOOL ShowTrack;

    Plane &m_Plane;
};

