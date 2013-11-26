#pragma once

#include "StateMapCtrl.h"

class __declspec(dllexport) SensorClientProxy
{
public:
    SensorClientProxy(CStateMapCtrl &stateMapCtrl);
    virtual ~SensorClientProxy(void);

    virtual void OnClose() = 0;
    virtual void DrawThetaRange() = 0;
    virtual void DrawTargets() = 0;
    virtual void BlendAll() = 0;
    virtual void Invalidate(BOOL bErase = 1) = 0;
    CStateMapCtrl &m_StateMapCtrl;
};
