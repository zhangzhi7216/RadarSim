#include "stdafx.h"
#include "DataList.h"

DataList::DataList(Sensor &radar, Sensor &esm, Sensor &infrared)
: m_Radar(radar)
, m_Esm(esm)
, m_Infrared(infrared)
{
}

DataList::~DataList(void)
{
}

void DataList::Reset()
{
    m_TargetColors.clear();
}

void DataList::AddTarget(Target &target)
{
    m_TargetColors.push_back((TargetColor)(rand() % TargetColorLast));
}
