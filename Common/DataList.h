#pragma once

#include "Defines.h"
#include "Sensor.h"
#include "Target.h"

#include <map>

class __declspec(dllexport) DataList
{
public:
    enum ColumnType
    {
        ColumnTypeInt = 0,
        ColumnTypeDouble,
        ColumnTypeString,
    };
    struct ColumnItem
    {
        ColumnType type;
        CString title;
        ColumnItem(ColumnType tp, CString tt)
        {
            type = tp;
            title = tt;
        }
        template<class T>
        CString MakeData(T d)
        {
            CString str;
            if (type == ColumnTypeInt)
            {
                str.AppendFormat(TEXT("%d"), d);
                return str;
            }
            if (type == ColumnTypeDouble)
            {
                str.AppendFormat(TEXT("%f"), d);
                return str;
            }
            if (type == ColumnTypeString)
            {
                str.AppendFormat(TEXT("%s"), d);
                return str;
            }
        }
    };

    DataList();
    virtual ~DataList(void);

    vector<ColumnItem> m_ColumnItems;
    map<int, Target> m_Targets;

    void Reset();
    void AddTarget(Target &target);
};
