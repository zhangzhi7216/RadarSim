#pragma once

#include "Defines.h"
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

    SensorType m_Type;
    BOOL m_Enable;

    double m_MaxDis;
    double m_MaxTheta;
    double m_MaxPhi;

    double m_DisVar;
    double m_ThetaVar;
    double m_PhiVar;

    double m_ProDet;

    BOOL m_ShowScanline;
    BOOL m_ShowTrack;
    BOOL m_ShowThetaRange;
    Color m_ThetaRangeColor;
    BOOL m_ShowHeight;

    vector<TargetColor> m_TargetColors;

    Plane &m_Plane;

    void Reset();
    void AddTarget(Target &target);
};

