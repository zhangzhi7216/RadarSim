#include "StdAfx.h"
#include "NaviMatlabAlgo.h"

NaviMatlabAlgo::NaviMatlabAlgo(void)
: m_DllHandle(0)
, m_MatlabFunc(0)
{
}

NaviMatlabAlgo::NaviMatlabAlgo(const CString &name, const CString &dllFileName, const CString &funcName)
: NaviAlgo(name)
, m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
, m_MatlabFunc(0)
{
}

NaviMatlabAlgo::~NaviMatlabAlgo(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

bool NaviMatlabAlgo::Init()
{
    m_DllHandle = LoadLibrary(m_DllFileName);
    if (!m_DllHandle)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法装载模块%s."), m_DllFileName);
        return false;
    }
    bool result = false;
    wstring wsFuncName(m_FuncName);
    string sFuncName(wsFuncName.begin(), wsFuncName.end());

    string initializeFuncName = sFuncName;
    initializeFuncName += InitializeFuncPrefix;

    InitializeFunc initializeFunc = (InitializeFunc)GetProcAddress(
        (HMODULE)m_DllHandle,
        initializeFuncName.c_str());

    if (!initializeFunc)
    {
        return false;
    }

    result = initializeFunc();

    if (!result)
    {
        return false;
    }

    string fullFuncName = sFuncName;
    fullFuncName[0] = toupper(fullFuncName[0]);
    fullFuncName = FuncPrefix + fullFuncName;
    m_MatlabFunc = (RealFunc)GetProcAddress(
        m_DllHandle,
        fullFuncName.c_str());
    if (!m_MatlabFunc)
    {
        return false;
    }

    return true;
}

void NaviMatlabAlgo::Output(CArchive &ar)
{
    ar << NaviAlgoTypeMatlab << m_DllFileName << m_FuncName;
    NaviAlgo::Output(ar);
}

void NaviMatlabAlgo::Input(CArchive &ar)
{
    ar >> m_DllFileName >> m_FuncName;
    NaviAlgo::Input(ar);
}

bool NaviMatlabAlgo::Run(const NaviInput &input, NaviOutput &output)
{
    if (!m_MatlabFunc)
    {
        CString msg;
        msg.AppendFormat(TEXT("算法%s地址错误."), m_Name);
        AfxMessageBox(msg);
        return false;
    }
    // FIXME: To implement navi matlab algo.
    return true;
}
