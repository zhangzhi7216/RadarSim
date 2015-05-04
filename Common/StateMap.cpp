#include "stdafx.h"
#include "StateMap.h"

#include "Utility.h"

StateMap::StateMap()
: m_MaxX(1200)
, m_MaxY(900)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_ShowHeight(FALSE)
, m_Background(StateMapBackground0)
, m_ExplosionType(ExplosionType0)
, m_ZoomKeyTargetId(-1)
{
}

StateMap::~StateMap(void)
{
}

void StateMap::Reset()
{
    m_ShowTrack = TRUE;
    m_ShowThetaRange = TRUE;

    m_Planes.clear();
    m_PlanePaths.clear();

    m_Targets.clear();
    m_TargetPaths.clear();

    m_Missiles.clear();
    m_MissilePaths.clear();

    m_ZoomKeyTargetId = -1;
}

void StateMap::AddPlaneData(int plane, Position pos, Velocity vel, TargetState state)
{
    m_Planes[plane].m_Vel = vel;
    m_Planes[plane].m_State = state;
    m_PlanePaths[plane].push_back(pos);
}

void StateMap::AddPlane(Plane &plane)
{
    m_Planes.push_back(plane);
    m_PlanePaths.push_back(Path());
}

void StateMap::AddTarget(Target &target)
{
    m_Targets.push_back(target);
    m_TargetPaths.push_back(Path());
}

void StateMap::AddTargetData(int target, Position pos, Velocity vel, TargetState state)
{
    m_Targets[target].m_Vel = vel;
    m_Targets[target].m_State = state;
    if (state != TargetStateDestroyed)
    {
        m_TargetPaths[target].push_back(pos);
    }
}

void StateMap::AddMissile(Missile &miss)
{
    m_Missiles.push_back(miss);
    m_MissilePaths.push_back(Path());
}

void StateMap::AddMissileData(int miss, Position pos, Velocity vel, TargetState state)
{
    m_Missiles[miss].m_Vel = vel;
    m_Missiles[miss].m_State = state;
    if (state != TargetStateDestroyed)
    {
        m_MissilePaths[miss].push_back(pos);
    }
}

void StateMap::ZoomKeyTarget(double x, double y)
{
    Position p(x, y, 0);
    for (int i = 0; i < m_TargetPaths.size(); i++)
    {
        p.Z = m_TargetPaths[i].back().Z;
        if (Utility::Distance(m_TargetPaths[i].back(), p) <= KEY_TARGET_HIT_THRESHOLD && m_Targets[i].m_IsKeyTarget)
        {
            m_ZoomKeyTargetId = i;
            return;
        }
    }
    m_ZoomKeyTargetId = -1;
}

CArchive & operator << (CArchive &ar, StateMap &stateMap)
{
    int background = (int)stateMap.m_Background;
    int explosion = (int)stateMap.m_ExplosionType;
    ar << background << explosion << stateMap.m_MaxX << stateMap.m_MaxY << stateMap.m_ShowHeight;

    return ar;
}

CArchive & operator >> (CArchive &ar, StateMap &stateMap)
{
    int background, explosion;
    ar >> background >> explosion >> stateMap.m_MaxX >> stateMap.m_MaxY >> stateMap.m_ShowHeight;
    stateMap.m_Background = (StateMapBackground)background;
    stateMap.m_ExplosionType = (ExplosionType)explosion;

    return ar;
}
