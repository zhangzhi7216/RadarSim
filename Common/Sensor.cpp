#include "Sensor.h"

CString Sensor::SensorTypeNames[] = {TEXT("有源传感器"), TEXT("无源传感器")};

Sensor::Sensor(SensorType type, Plane &plane)
: Type(type)
, Enable(TRUE)
, MaxDis(100)
, MaxTheta(120)
, MaxPhi(90)
, DisVar(0)
, ThetaVar(0)
, PhiVar(0)
, ProDet(0)
, ShowScanline(TRUE)
, ShowTrack(TRUE)
, m_Plane(plane)
{
}

Sensor::~Sensor(void)
{
}
