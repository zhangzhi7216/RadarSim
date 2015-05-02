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

    ExplosionType m_ExplosionType;

    vector<Plane> m_Planes;
    vector<Path> m_PlanePaths;

    vector<Target> m_Targets;
    vector<Path> m_TargetPaths;

    vector<Missile> m_Missiles;
    vector<Path> m_MissilePaths;

    void Reset();
    void AddPlane(Plane &plane);
    void AddPlaneData(int plane, Position pos, Velocity vel, TargetState state);
    void AddTarget(Target &target);
    void AddTargetData(int target, Position pos, Velocity vel, TargetState state);
    void AddMissile(Missile &miss);
    void AddMissileData(int miss, Position pos, Velocity vel, TargetState state);

    int m_ZoomKeyTargetId;
    void ZoomKeyTarget(double x, double y);
};

__declspec(dllexport) CArchive & operator << (CArchive &ar, StateMap &stateMap);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, StateMap &stateMap);
