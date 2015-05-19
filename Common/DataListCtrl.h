#pragma once

#include "DataList.h"

class __declspec(dllexport) CDataListCtrl : public CListCtrl
{
public:
    CDataListCtrl(DataList &dataList);
    virtual ~CDataListCtrl(void);

    virtual void PreSubclassWindow();

    DataList &m_DataList;

    void Reset();
    void AddTargetData(int target, vector<CString> &data);
    DECLARE_MESSAGE_MAP()
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

