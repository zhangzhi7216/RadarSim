#include "StdAfx.h"
#include "TargetClient.h"

TargetClient::TargetClient(void)
{
    m_Target.m_Id = s_TargetCount++;
}

TargetClient::~TargetClient(void)
{
}

int TargetClient::s_TargetCount = 100;
