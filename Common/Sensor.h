#pragma once

#include "Target.h"

class __declspec(dllexport) Sensor
{
public:
    enum SensorType
    {
        SensorTypeSource = 0,
        SensorTypeNonSource,
        SensorTypeLast,
    };

    static CString SensorTypeNames[SensorTypeLast];

    Sensor(SensorType type);
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
    vector<vector<double>> m_TargetDistances, m_TargetThetas, m_TargetPhis;

    void Reset();
    void AddTarget(Target &target);
    void AddTargetData(int target, Position rel);
    bool IsShowTargetData(int i, int j);
};

__declspec(dllexport) CArchive & operator << (CArchive &ar, Sensor &sensor);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, Sensor &sensor);
