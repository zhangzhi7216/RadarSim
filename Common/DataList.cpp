#include "stdafx.h"
#include "DataList.h"

DataList::DataList(Sensor &sensor1, CString sensor1Title, Sensor &sensor2, CString sensor2Title)
: m_Sensor1(sensor1)
, m_Sensor1Title(sensor1Title)
, m_Sensor2(sensor2)
, m_Sensor2Title(sensor2Title)
{
}

DataList::~DataList(void)
{
}

void DataList::Reset()
{
    m_TargetColors.clear();
    m_Times.clear();
}

void DataList::AddTarget(Target &target)
{
    m_TargetColors.push_back(target.m_Color);
    m_Times.push_back(vector<int>());
}

void DataList::AddTargetData(int target, int time)
{
    m_Times[target].push_back(time);
}
