#include "StdAfx.h"
#include "PlaneClient.h"

PlaneClient::PlaneClient(void)
: m_Radar(Sensor::SensorTypeSource)
, m_Esm(Sensor::SensorTypeNonSource)
, m_Infrared(Sensor::SensorTypeNonSource)
, m_StateMap(m_Radar, m_Esm, m_Infrared)
{
}

PlaneClient::~PlaneClient(void)
{
}
