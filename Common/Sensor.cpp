#include "Sensor.h"

CString Sensor::SensorTypeNames[] = {TEXT("有源传感器"), TEXT("无源传感器")};

Color Sensor::TargetColors[] =
{
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};

CString Sensor::TargetColorNames[] =
{
    TEXT("红色"),
    TEXT("橙色"),
    TEXT("黄色"),
    TEXT("绿色"),
    TEXT("青色"),
    TEXT("蓝色"),
    TEXT("紫色"),
};

Sensor::Sensor(SensorType type, Plane &plane)
: m_Type(type)
, m_Enable(TRUE)
, m_MaxDis(100)
, m_MaxTheta(120)
, m_MaxPhi(90)
, m_DisVar(0)
, m_ThetaVar(0)
, m_PhiVar(0)
, m_ProDet(0)
, m_ShowScanline(TRUE)
, m_ShowTrack(TRUE)
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
    m_MaxPhi = 90;
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
