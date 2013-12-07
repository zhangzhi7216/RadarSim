#pragma once

#include "Defines.h"
#include "Sensor.h"
#include "Target.h"

class __declspec(dllexport) DataList
{
public:
    DataList(Sensor &radar, Sensor &esm, Sensor &infrared);
    virtual ~DataList(void);

    Sensor &m_Radar, &m_Esm, &m_Infrared;

    vector<TargetColor> m_TargetColors;
    vector<vector<int>> m_Times;

    void Reset();
    void AddTarget(Target &target);
    void AddTargetData(int target, int time);
};
