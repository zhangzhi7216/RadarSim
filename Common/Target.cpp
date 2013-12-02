#include "stdafx.h"
#include "Target.h"

Target::Target()
: m_Id(0)
, m_Type(TargetTypeHeli)
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
    ar << target.m_Id << type << target.m_Position;

    return ar;
}

CArchive & operator >> (CArchive &ar, Target &target)
{
    int type;
    ar >> target.m_Id >> type >> target.m_Position;
    target.m_Type = (TargetType)type;

    return ar;
}
