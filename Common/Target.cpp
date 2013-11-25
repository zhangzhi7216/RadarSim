#include "Target.h"

Image *Target::TargetTypeImages[] =
{
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
    Image::FromFile(TEXT("")),
};

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
