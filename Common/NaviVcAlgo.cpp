#include "StdAfx.h"
#include "NaviVcAlgo.h"

NaviVcAlgo::NaviVcAlgo(void)
: m_DllHandle(0)
{
}

NaviVcAlgo::NaviVcAlgo(const CString &name, const CString &dllFileName, const CString &funcName)
: NaviAlgo(name)
, m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
{
}

NaviVcAlgo::~NaviVcAlgo(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

bool NaviVcAlgo::Init()
{
    m_DllHandle = LoadLibrary(m_DllFileName);
    if (!m_DllHandle)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法装载模块%s."), m_DllFileName);
        return false;
    }
    wstring ws = m_FuncName;
    string s(ws.begin(), ws.end());
    m_Func = (AlgoFunc)GetProcAddress(
        m_DllHandle,
        s.c_str());

    if (!m_Func)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法找到函数%s."), m_FuncName);
        return false;
    }
    return true;
}

void NaviVcAlgo::Output(CArchive &ar)
{
    ar << NaviAlgoTypeVc << m_DllFileName << m_FuncName;
    NaviAlgo::Output(ar);
}

void NaviVcAlgo::Input(CArchive &ar)
{
    ar >> m_DllFileName >> m_FuncName;
    NaviAlgo::Input(ar);
}
