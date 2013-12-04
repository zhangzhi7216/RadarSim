#pragma once

#include "Target.h"
#include "DataPacket.h"

class TargetClient
{
public:
    TargetClient(void);
    virtual ~TargetClient(void);

    Target m_Target;
    vector<TrueDataFrame> m_TargetTrueDatas;

    static int s_TargetCount;
};

