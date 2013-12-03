#include "stdafx.h"
#include "Target.h"

Target::Target()
: m_Id(0)
, m_Type(TargetTypeHeli)
, m_Color(TargetColorOrange)
{
}

Target::~Target(void)
{
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
    ar << target.m_Id << type << target.m_Position << color;

    return ar;
}

CArchive & operator >> (CArchive &ar, Target &target)
{
    int type, color;
    ar >> target.m_Id >> type >> target.m_Position >> color;
    target.m_Type = (TargetType)type;
    target.m_Color = (TargetColor)color;

    return ar;
}
