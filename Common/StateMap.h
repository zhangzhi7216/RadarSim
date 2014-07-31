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
    BOOL m_ShowHeight;

    StateMapBackground m_Background;

    vector<TargetType> m_PlaneTypes;
    vector<TargetColor> m_PlaneColors;
    vector<Path> m_PlanePaths;
    vector<TargetState> m_PlaneStates;

    vector<Sensor *> m_Radars;
    vector<Sensor *> m_Esms;
    vector<Sensor *> m_Infrareds;

    vector<TargetType> m_TargetTypes;
    vector<TargetColor> m_TargetColors;
    vector<Path> m_TargetPaths;
    vector<TargetState> m_TargetStates;

    vector<TargetType> m_MissileTypes;
    vector<TargetColor> m_MissileColors;
    vector<Path> m_MissilePaths;
    vector<TargetState> m_MissileStates;

    void Reset();
    void AddPlane(Plane &plane, Sensor *radar, Sensor *esm, Sensor *infrared);
    void AddPlaneData(int plane, Position pos, TargetState state);
    void AddTarget(Target &target);
    void AddTargetData(int target, Position pos, TargetState state);
    void AddMissile(Missile &miss);
    void AddMissileData(int miss, Position pos, TargetState state);
};

__declspec(dllexport) CArchive & operator << (CArchive &ar, StateMap &stateMap);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, StateMap &stateMap);
