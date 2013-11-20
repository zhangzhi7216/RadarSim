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

Color Target::TargetColors[] =
{
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};

CString Target::TargetColorNames[] =
{
    TEXT("红色"),
    TEXT("橙色"),
    TEXT("黄色"),
    TEXT("绿色"),
    TEXT("青色"),
    TEXT("蓝色"),
    TEXT("紫色"),
};

Target::Target(Target *plane)
: m_Id(0)
, m_Type(TargetTypeHeli)
, m_Color(TargetColorRed)
{
}

Target::~Target(void)
{
}

void Target::AddTarget(Target &target)
{
    m_Targets[target.m_Id] = target;
    m_RelPositionPaths[target.m_Id] = Path();
    m_DistancePaths[target.m_Id] = vector<double>();
    m_ThetaPaths[target.m_Id] = vector<double>();
    m_PhiPaths[target.m_Id] = vector<double>();
}

void Target::MoveTo(const Position &position)
{
    m_Position = position;
    m_Path.push_back(position);
    for (map<int, Target>::iterator it = m_Targets.begin();
        it != m_Targets.end();
        ++it)
    {
        m_RelPositionPaths[it->first].push_back(it->second.m_Position - m_Position);
        // 相对坐标到球面坐标
        m_DistancePaths[it->first].push_back(m_RelPositionPaths[it->first].back().X);
        m_ThetaPaths[it->first].push_back(m_RelPositionPaths[it->first].back().Y);
        m_PhiPaths[it->first].push_back(m_RelPositionPaths[it->first].back().Z);
    }
}
