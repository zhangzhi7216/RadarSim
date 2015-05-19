#include "stdafx.h"
#include "DataList.h"

DataList::DataList()
{
}

DataList::~DataList(void)
{
}

void DataList::Reset()
{
    m_Targets.clear();
}

void DataList::AddTarget(Target &target)
{
    m_Targets[target.m_Id] = target;
}
