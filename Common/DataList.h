#pragma once

#include "Defines.h"
#include "Sensor.h"
#include "Target.h"

class DataList
{
public:
    DataList(Sensor &radar, Sensor &esm, Sensor &infrared, Plane &plane);
    virtual ~DataList(void);

    Sensor &m_Radar, &m_Esm, &m_Infrared;

    vector<TargetColor> m_TargetColors;

    Plane &m_Plane;

    void Reset();
    void AddTarget(Target &target);
};
