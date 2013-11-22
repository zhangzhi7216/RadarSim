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

void Target::AddTarget(Target &target)
{
    m_Targets.push_back(target);
    m_RelPositionPaths.push_back(Path());
    m_DistancePaths.push_back(vector<double>());
    m_ThetaPaths.push_back(vector<double>());
    m_PhiPaths.push_back(vector<double>());
}

void Target::MoveTo(const Position &position)
{
    m_Position = position;
    m_Path.push_back(position);
    for (int i = 0; i < m_Targets.size(); ++i)
    {
        m_RelPositionPaths[i].push_back(m_Targets[i].m_Position - m_Position);
        // 相对坐标到球面坐标
        m_DistancePaths[i].push_back(m_RelPositionPaths[i].back().X);
        m_ThetaPaths[i].push_back(m_RelPositionPaths[i].back().Y);
        m_PhiPaths[i].push_back(m_RelPositionPaths[i].back().Z);
    }
}

void Target::Reset()
{
    m_Id = 0;
    m_Type = TargetTypeHeli;
    m_Position = Position();
    m_Path.clear();
    m_Targets.clear();
    m_RelPositionPaths.clear();
    m_DistancePaths.clear();
    m_ThetaPaths.clear();
    m_PhiPaths.clear();
}
