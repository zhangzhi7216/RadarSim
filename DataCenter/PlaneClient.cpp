#include "StdAfx.h"
#include "PlaneClient.h"

PlaneClient::PlaneClient(void)
: m_Radar(Sensor::SensorTypeSource)
, m_Esm(Sensor::SensorTypeNonSource)
, m_Infrared(Sensor::SensorTypeNonSource)
, m_PlaneSocket(NULL)
{
    m_Plane.m_Id = s_PlaneCount++;
}

PlaneClient::~PlaneClient(void)
{
}

int PlaneClient::s_PlaneCount = 0;
