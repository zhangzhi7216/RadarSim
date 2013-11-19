#include "StdAfx.h"
#include "RadarParam.h"

RadarParam::RadarParam(void)
: Type(TEXT("有源传感器"))
, Enable(TRUE)
, MaxDis(100)
, MaxTheta(120)
, MaxPhi(90)
, DisVar(0)
, ThetaVar(0)
, PhiVar(0)
, ProDet(0)
, ShowScanline(TRUE)
{
}

RadarParam::~RadarParam(void)
{
}
