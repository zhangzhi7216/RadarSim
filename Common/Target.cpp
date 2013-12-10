#include "stdafx.h"
#include "Target.h"

Target::Target()
: m_Id(0)
, m_Type(TargetTypeHeli)
, m_Color(TargetColorOrange)
, m_HeadDir(1, 1, 1)
{
}

Target::~Target(void)
{
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
    m_Id = 0;
    m_Type = TargetTypeHeli;
    m_Position = Position();
}

CArchive & operator << (CArchive &ar, Target &target)
{
    int type = (int)target.m_Type;
    int color = (int)target.m_Color;
    ar << target.m_Id << type << target.m_Position << target.m_HeadDir << color;

    return ar;
}

CArchive & operator >> (CArchive &ar, Target &target)
{
    int type, color;
    ar >> target.m_Id >> type >> target.m_Position >> target.m_HeadDir >> color;
    target.m_Type = (TargetType)type;
    target.m_Color = (TargetColor)color;

    return ar;
}
