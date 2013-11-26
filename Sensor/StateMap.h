#pragma once

#include "Sensor.h"
#include "Target.h"

class __declspec(dllexport) StateMap
{
public:
    StateMap(Sensor &radar, Sensor &esm, Sensor &infrared, Plane &plane);
    virtual ~StateMap(void);

    double m_MaxX, m_MaxY;

    BOOL m_ShowTrack;
    BOOL m_ShowThetaRange;

    StateMapBackground m_Background;

    TargetType m_PlaneType;
    TargetColor m_PlaneColor;
    Path m_PlanePath;

    vector<TargetType> m_TargetTypes;
    vector<TargetColor> m_TargetColors;
    vector<Path> m_TargetPaths;

    Sensor &m_Radar, &m_Esm, &m_Infrared;

    void Reset();
    void AddPlaneData(Position pos);
    void AddTarget(Target &target);
    void AddTargetData(int target, Position pos);
};
