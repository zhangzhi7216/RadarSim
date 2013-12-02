#pragma once

#include "Sensor.h"
#include "Target.h"

class __declspec(dllexport) StateMap
{
public:
    StateMap();
    virtual ~StateMap(void);

    double m_MaxX, m_MaxY;

    BOOL m_ShowTrack;
    BOOL m_ShowThetaRange;

    StateMapBackground m_Background;

    vector<TargetType> m_PlaneTypes;
    vector<TargetColor> m_PlaneColors;
    vector<Path> m_PlanePaths;

    vector<Sensor *> m_Radars;
    vector<Sensor *> m_Esms;
    vector<Sensor *> m_Infrareds;

    vector<TargetType> m_TargetTypes;
    vector<TargetColor> m_TargetColors;
    vector<Path> m_TargetPaths;

    void Reset();
    void AddPlane(Plane &plane, Sensor *radar, Sensor *esm, Sensor *infrared);
    void AddPlaneData(int plane, Position pos);
    void AddTarget(Target &target);
    void AddTargetData(int target, Position pos);
};

__declspec(dllexport) CArchive & operator << (CArchive &ar, StateMap &stateMap);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, StateMap &stateMap);
