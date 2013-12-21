#pragma once

#include "DataPacket.h"

template <class TI, class TO>
class GeneralAlgo
{
public:
    typedef bool (*AlgoFunc)(const TI &, TO &);

    GeneralAlgo()
        : m_Func(0)
    {}
    GeneralAlgo(const CString &name)
        : m_Name(name)
        , m_Func(0)
    {}
    virtual ~GeneralAlgo()
    {}

    virtual bool Init()
    {
        return true;
    }

    bool Run(const TI &input, TO &output)
    {
        if (!m_Func)
        {
            CString msg;
            msg.AppendFormat(TEXT("À„∑®%sµÿ÷∑¥ÌŒÛ."), m_Name);
            AfxMessageBox(msg);
            return false;
        }
        else
        {
            return m_Func(input, output);
        }
    }

    virtual void Output(CArchive &ar)
    {
        ar << m_Name;
    }

    virtual void Input(CArchive &ar)
    {
        ar >> m_Name;
    }

public:
    CString m_Name;
    AlgoFunc m_Func;
};

template <class TI, class TO>
__declspec(dllexport) CArchive & operator << (CArchive &ar, GeneralAlgo<TI, TO> &algo)
{
    algo.Output(ar);
    return ar;
}

template <class TI, class TO>
__declspec(dllexport) CArchive & operator >> (CArchive &ar, GeneralAlgo<TI, TO> &algo)
{
    algo.Input(ar);
    return ar;
}