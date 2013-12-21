#include "stdafx.h"
#include "StateMap.h"

StateMap::StateMap()
: m_MaxX(1200)
, m_MaxY(900)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_ShowHeight(FALSE)
, m_Background(StateMapBackground0)
{
}

StateMap::~StateMap(void)
{
}

void StateMap::Reset()
{
    m_ShowTrack = TRUE;
    m_ShowThetaRange = TRUE;

    m_PlaneTypes.clear();
    m_PlaneColors.clear();
    m_PlanePaths.clear();
    m_TargetTypes.clear();
    m_TargetColors.clear();
    m_TargetPaths.clear();

    m_Radars.clear();
    m_Esms.clear();
    m_Infrareds.clear();
}

void StateMap::AddPlaneData(int plane, Position pos)
{
    m_PlanePaths[plane].push_back(pos);
}

void StateMap::AddPlane(Plane &plane, Sensor *radar, Sensor *esm, Sensor *infrared)
{
    m_PlaneTypes.push_back(plane.m_Type);
    m_PlaneColors.push_back(plane.m_Color);
    m_PlanePaths.push_back(Path());

    m_Radars.push_back(radar);
    m_Esms.push_back(esm);
    m_Infrareds.push_back(infrared);
}

void StateMap::AddTarget(Target &target)
{
    m_TargetTypes.push_back(target.m_Type);
    m_TargetColors.push_back(target.m_Color);
    m_TargetPaths.push_back(Path());
}

void StateMap::AddTargetData(int target, Position pos)
{
    m_TargetPaths[target].push_back(pos);
}

CArchive & operator << (CArchive &ar, StateMap &stateMap)
{
    int background = (int)stateMap.m_Background;
    ar << background << stateMap.m_MaxX << stateMap.m_MaxY << stateMap.m_ShowHeight;

    return ar;
}

CArchive & operator >> (CArchive &ar, StateMap &stateMap)
{
    int background;
    ar >> background >> stateMap.m_MaxX >> stateMap.m_MaxY >> stateMap.m_ShowHeight;
    stateMap.m_Background = (StateMapBackground)background;

    return ar;
}
