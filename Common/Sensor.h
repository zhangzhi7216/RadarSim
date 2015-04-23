#pragma once

#include "DataPacket.h"

class Target;

class __declspec(dllexport) Sensor
{
public:
    Sensor(SensorId id, Target &plane, GlobalDataPacket &globalData);
    Sensor();
    virtual ~Sensor(void);

    Sensor &operator =(const Sensor &);

    SensorId m_Id;
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
    GlobalDataPacket &m_GlobalData;
    Target *m_Plane;

    void Reset();
    void AddTarget(Target &target);
    void AddTargetData(int target, Position rel);
    bool IsInRange(int i, double d, double t, double p);
    bool IsInRange(int i, Position rel);
    bool IsShowTargetData(int i, int j);
};

__declspec(dllexport) CArchive & operator << (CArchive &ar, Sensor &sensor);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, Sensor &sensor);
