#pragma once

#include "../Radar/RadarClientProxy.h"

class PlaneRadarProxy :
    public RadarClientProxy
{
public:
    PlaneRadarProxy(void);
    virtual ~PlaneRadarProxy(void);
};
