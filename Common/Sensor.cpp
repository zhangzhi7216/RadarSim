#include "Sensor.h"

CString Sensor::SensorTypeNames[] = {TEXT("有源传感器"), TEXT("无源传感器")};

Sensor::Sensor(SensorType type, Plane &plane)
: m_Type(type)
, m_Enable(TRUE)
, m_MaxDis(100)
, m_MaxTheta(120)
, m_MaxPhi(120)
, m_DisVar(0)
, m_ThetaVar(0)
, m_PhiVar(0)
, m_ProDet(0)
, m_ShowScanline(TRUE)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_ThetaRangeColor(Color::Green)
, m_ShowHeight(TRUE)
, m_Plane(plane)
{
}

Sensor::~Sensor(void)
{
}

void Sensor::Reset()
{
    m_Enable = TRUE;
    m_MaxDis = 100;
    m_MaxTheta = 120;
    m_MaxPhi = 120;
    m_DisVar = 0;
    m_ThetaVar = 0;
    m_PhiVar = 0;
    m_ProDet = 0;
    m_ShowScanline = TRUE;
    m_ShowTrack = TRUE;
    m_TargetColors.clear();
}

void Sensor::AddTarget(Target &target)
{
    m_TargetColors.push_back((TargetColor)(rand() % TargetColorLast));
}
