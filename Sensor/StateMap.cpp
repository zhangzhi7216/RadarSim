#include "stdafx.h"
#include "StateMap.h"

StateMap::StateMap(Sensor &radar, Sensor &esm, Sensor &infrared, Plane &plane)
: m_MaxX(1000)
, m_MaxY(1000)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_Background(StateMapBackground0)
, m_Radar(radar)
, m_Esm(esm)
, m_Infrared(infrared)
{
    m_PlaneType = plane.m_Type;
    m_PlaneColor = (TargetColor)(rand() % TargetColorLast);
}

StateMap::~StateMap(void)
{
}

void StateMap::Reset()
{
    m_ShowTrack = TRUE;
    m_ShowThetaRange = TRUE;

    m_PlanePath.clear();
    m_TargetTypes.clear();
    m_TargetColors.clear();
    m_TargetPaths.clear();
}

void StateMap::AddPlaneData(Position pos)
{
    m_PlanePath.push_back(pos);
}

void StateMap::AddTarget(Target &target)
{
    m_TargetTypes.push_back(target.m_Type);
    m_TargetColors.push_back((TargetColor)(rand() % TargetColorLast));
    m_TargetPaths.push_back(Path());
}

void StateMap::AddTargetData(int target, Position pos)
{
    m_TargetPaths[target].push_back(pos);
}
