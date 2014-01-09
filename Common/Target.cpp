#include "stdafx.h"
#include "Target.h"

Target::Target()
: m_Id(0)
, m_Type(TargetTypeHeli)
, m_InitPosition()
, m_InitVel(1, 1, 1)
, m_InitAcc(1, 1, 1)
, m_Color(TargetColorOrange)
, m_MoveType(TargetMoveTypeUniAcc)
, m_HeadDir(1, 1, 1)
, m_Pal(1)
, m_Radius(100)
{
}

Target::~Target(void)
{
}

void Target::Move(double t)
{
    Position pos;
    if (TargetMoveTypeUniVel == m_MoveType)
    {
        pos = m_Position + m_Vel * t;
    }
    else if (TargetMoveTypeUniAcc == m_MoveType)
    {
        pos = m_Position + m_Vel * t + m_Acc / 2 * t * t;
        m_Vel += m_Acc * t;
    }
    else if (TargetMoveTypeUniCir == m_MoveType)
    {
        // 在这里加圆周运动
    }
    MoveTo(pos);
}

void Target::MoveTo(const Position &pos)
{
    if (!m_Position.Equals(pos))
    {
        m_HeadDir = pos - m_Position;
    }
    m_Position = pos;
}

void Target::Reset()
{
    m_Position = m_InitPosition;
    m_Vel = m_InitVel;
    m_Acc = m_InitAcc;

    m_Center = m_InitPosition;
}

CArchive & operator << (CArchive &ar, Target &target)
{
    int type = (int)target.m_Type;
    int moveType = (int)target.m_MoveType;
    int color = (int)target.m_Color;
    ar << target.m_Id << type << moveType << color
        << target.m_InitPosition << target.m_InitVel << target.m_InitAcc
        << target.m_Position << target.m_Vel << target.m_Acc
        << target.m_Center << target.m_Pal << target.m_Radius
        << target.m_HeadDir;

    return ar;
}

CArchive & operator >> (CArchive &ar, Target &target)
{
    int type, moveType, color;
    ar >> target.m_Id >> type >> moveType >> color
        >> target.m_InitPosition >> target.m_InitVel >> target.m_InitAcc
        >> target.m_Position >> target.m_Vel >> target.m_Acc
        >> target.m_Center >> target.m_Pal >> target.m_Radius
        >> target.m_HeadDir;
    target.m_Type = (TargetType)type;
    target.m_MoveType = (TargetMoveType)moveType;
    target.m_Color = (TargetColor)color;

    return ar;
}
