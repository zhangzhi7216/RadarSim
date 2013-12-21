#include "StdAfx.h"
#include "FusionVcAlgo.h"

FusionVcAlgo::FusionVcAlgo(void)
: m_DllHandle(0)
{
}

FusionVcAlgo::FusionVcAlgo(const CString &name, const CString &dllFileName, const CString &funcName)
: FusionAlgo(name)
, m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
{
}

FusionVcAlgo::~FusionVcAlgo(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

bool FusionVcAlgo::Init()
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

void FusionVcAlgo::Output(CArchive &ar)
{
    ar << FusionAlgoTypeVc << m_DllFileName << m_FuncName;
    FusionAlgo::Output(ar);
}

void FusionVcAlgo::Input(CArchive &ar)
{
    ar >> m_DllFileName >> m_FuncName;
    FusionAlgo::Input(ar);
}
