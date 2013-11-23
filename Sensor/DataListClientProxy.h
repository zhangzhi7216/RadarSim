#pragma once

class __declspec(dllexport) DataListClientProxy
{
public:
    DataListClientProxy(void);
    virtual ~DataListClientProxy(void);

    virtual void OnClose() = 0;
    virtual void Invalidate(BOOL bErase = 1) = 0;
};
