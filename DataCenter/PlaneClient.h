#pragma once

#include "Target.h"
#include "Sensor.h"
#include "StateMap.h"
#include "PlaneSocket.h"
#include "DataPacket.h"

class PlaneClient
{
public:
    PlaneClient(void);
    virtual ~PlaneClient(void);

    Plane m_Plane;
    Sensor m_Radar, m_Esm, m_Infrared;
    StateMap m_StateMap;

    PlaneSocket *m_PlaneSocket;
    vector<TrueDataFrame> m_PlaneTrueDatas;

    static int s_PlaneCount;
};
