#include "StdAfx.h"
#include "MatlabDlg.h"

using namespace MatlabHelper;

CMatlabDlg::CMatlabDlg(LPCWSTR dllFileName, LPCWSTR funcName)
: m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_Input(NULL)
, m_Size(200)
, m_Thread(NULL)
{
    if (!(m_Engine = engOpen(NULL)))
    {
        // Utility::PromptErrorMessage(TEXT("Matlab引擎初始化失败, 绘图功能将不可用."));
    }
    else
    {
        // engSetVisible(m_Engine, false);
    }
}

CMatlabDlg::~CMatlabDlg(void)
{
    if (m_Thread)
    {
        TerminateThread(m_Thread, 0);
        // delete m_Thread;
        m_Thread = 0;
    }

    if (m_Input)
    {
        DestroyArray(m_Input);
        m_Input = NULL;
    }

    if (m_Engine)
    {
        engClose(m_Engine);
    }
}

void CMatlabDlg::Show()
{
    Run();
}

void CMatlabDlg::Hide()
{
    if (m_Thread)
    {
        TerminateThread(m_Thread, 0);
        // delete m_Thread;
        m_Thread = 0;
    }

    if (m_Input)
    {
        DestroyArray(m_Input);
        m_Input = NULL;
    }
}

void CMatlabDlg::Reset()
{
    if (m_Thread)
    {
        TerminateThread(m_Thread, 0);
        // delete m_Thread;
        m_Thread = 0;
    }

    if (m_Input)
    {
        DestroyArray(m_Input);
        m_Input = NULL;
    }

    m_PlanePaths.clear();
    m_TargetPaths.clear();
}

void CMatlabDlg::Run()
{
    m_Input = CreateDoubleArray(m_TargetPaths.size(), m_Size * 3, (const unsigned char *)NULL, 0, 0);
    double *data = mxGetPr(m_Input);
    for (int target = 0; target < m_TargetPaths.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetPaths[target].size(); ++pos)
        {
            data[(pos * 3 + 0) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].X;
            data[(pos * 3 + 1) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].Y;
            data[(pos * 3 + 2) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].Z;
            /*
            data[target * m_Size * 3 + pos] = m_TargetPaths[target][pos].X;
            data[target * m_Size * 3 + pos + 1] = m_TargetPaths[target][pos].Y;
            data[target * m_Size * 3 + pos + 2] = m_TargetPaths[target][pos].Z;
            */
        }
    }

    // result = engEvalString(m_Engine, "matlab_dialog(testtest)");
    m_Thread = CreateThread(NULL,
        NULL,
        MatlabRun,
        (LPVOID)this,
        CREATE_SUSPENDED,
        0);
    SetThreadPriority(m_Thread, THREAD_PRIORITY_NORMAL);
    ResumeThread(m_Thread);
}

DWORD WINAPI CMatlabDlg::MatlabRun(LPVOID lparam)
{
    CMatlabDlg *dlg = (CMatlabDlg *)lparam;
    if (!(dlg->m_Engine = engOpen(NULL)))
    {
        // Utility::PromptErrorMessage(TEXT("Matlab引擎初始化失败, 绘图功能将不可用."));
    }
    int result = 0;
    result = engEvalString(dlg->m_Engine, "cd C:\\Users\\zanmato\\Documents\\GitHub\\RadarSim\\Matlab\\dialog");
    int i = 1;
    while (true)
    {
        result = engPutVariable(dlg->m_Engine, "testtest", dlg->m_Input);
        if (i > dlg->m_TargetPaths.size())
        {
            i = 1;
        }
        char buf[128];
        itoa(i, buf, 10);
        string cmd = "matlab_dialog(testtest, ";
        cmd += buf;
        cmd += ")";
        // result = engEvalString(dlg->m_Engine, "cd C:\Users\zanmato\Documents\GitHub\RadarSim\Matlab\dialog");
        // result = engPutVariable(dlg->m_Engine, "testtest", dlg->m_Input);
        result = engEvalString(dlg->m_Engine, cmd.c_str());
        Sleep(1);
    }
    /*
    vector<Array *> inputList, outputList;
    inputList.push_back(dlg->m_Input);
    outputList.push_back(NULL);
    if (RunFunc(dlg->m_DllFileName, dlg->m_FuncName, outputList, inputList))
    {
        return 0;
    }
    else
    {
        return 1;
    }*/
    return result;
}

void CMatlabDlg::AddPlane(Plane &plane)
{
    m_PlanePaths.push_back(Path());
}

void CMatlabDlg::AddPlaneData(int plane, Position pos)
{
    if (m_PlanePaths[plane].size() >= m_Size)
    {
        return;
    }
    m_PlanePaths[plane].push_back(pos);
}

void CMatlabDlg::AddTarget(Target &target)
{
    m_TargetPaths.push_back(Path());
}

void CMatlabDlg::AddTargetData(int target, Position pos)
{
    if (m_TargetPaths[target].size() >= m_Size)
    {
        return;
    }
    m_TargetPaths[target].push_back(pos);
    if (m_Input)
    {
        double *data = mxGetPr(m_Input);
        data[(m_TargetPaths[target].size() * 3 + 0) * m_TargetPaths.size() + target] = pos.X;
        data[(m_TargetPaths[target].size() * 3 + 1) * m_TargetPaths.size() + target] = pos.Y;
        data[(m_TargetPaths[target].size() * 3 + 2) * m_TargetPaths.size() + target] = pos.Z;
        /*
        data[target * m_Size * 3 + m_TargetPaths[target].size() * 3] = pos.X;
        data[target * m_Size * 3 + m_TargetPaths[target].size() * 3 + 1] = pos.Y;
        data[target * m_Size * 3 + m_TargetPaths[target].size() * 3 + 2] = pos.Z;
        */
    }
}
