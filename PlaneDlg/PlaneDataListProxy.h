#pragma once

#include "../Sensor/DataListClientProxy.h"

class CPlaneDlg;

class PlaneDataListProxy :
    public DataListClientProxy
{
public:
    PlaneDataListProxy(CPlaneDlg &dlg);
    virtual ~PlaneDataListProxy(void);

    virtual void OnClose();
    virtual void Invalidate(BOOL bErase = 1);

private:
    CPlaneDlg &m_Dlg;
};
