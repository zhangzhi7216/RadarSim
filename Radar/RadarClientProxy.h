#pragma once

class __declspec(dllexport) RadarClientProxy
{
public:
    RadarClientProxy(void);
    virtual ~RadarClientProxy(void);

    virtual void OnClose() = 0;
    virtual void SetEnable(BOOL enable) = 0;
    virtual void SetScanLineAngle(DOUBLE scanLineAngle) = 0;
};
