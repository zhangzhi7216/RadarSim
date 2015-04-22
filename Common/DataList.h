#pragma once

#include "Defines.h"
#include "Sensor.h"
#include "Target.h"

class __declspec(dllexport) DataList
{
public:
    DataList(Sensor &sensor1, CString sensor1Title, Sensor &sensor2, CString sensor2Title);
    virtual ~DataList(void);

    Sensor &m_Sensor1, &m_Sensor2;
    CString m_Sensor1Title, m_Sensor2Title;

    vector<TargetColor> m_TargetColors;
    vector<vector<int>> m_Times;

    void Reset();
    void AddTarget(Target &target);
    void AddTargetData(int target, int time);
};
