#pragma once

#include "DataList.h"

class __declspec(dllexport) CDataListCtrl : public CListCtrl
{
public:
    enum ColumnIndex
    {
        ColumnTime = 0,
        ColumnEsmTheta,
        ColumnInfraredTheta,
        ColumnInfraredPhi,
        ColumnRadarDistance,
        ColumnRadarTheta,
        ColumnRadarPhi,
    };

    CDataListCtrl(DataList &dataList);
    virtual ~CDataListCtrl(void);

    virtual void PreSubclassWindow();

    DataList &m_DataList;

    void Reset();
    void AddTarget(Target &target);
    void AddTargetData();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

