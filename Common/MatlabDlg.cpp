#include "StdAfx.h"
#include "MatlabDlg.h"

using namespace MatlabHelper;

static char s_Msg[256];

CMatlabDlg::CMatlabDlg(const char *fileName,
        const char *planeTrue,
        const char *targetTrue,
        const char *targetFusion,
        const char *targetFilter,
        const char *globalVar)
: m_FileName(fileName)
, m_PlaneTrue(planeTrue)
, m_TargetTrue(targetTrue)
, m_TargetFusion(targetFusion)
, m_TargetFilter(targetFilter)
, m_GlobalVar(globalVar)
, m_PlaneTrueInput(NULL)
, m_TargetTrueInput(NULL)
, m_TargetFusionInput(NULL)
, m_TargetFilterInput(NULL)
, m_GlobalVarInput(NULL)
, m_Size(50)
, m_Thread(NULL)
, m_Engine(NULL)
{
}

CMatlabDlg::~CMatlabDlg(void)
{
    Stop();
    if (engClose(m_Engine))
    {
        AfxMessageBox(TEXT("Close engine engine WTF!"));
    }
}

void CMatlabDlg::Show()
{
    Run();
}

void CMatlabDlg::Hide()
{
    Stop();
}

void CMatlabDlg::Stop()
{
    /*
    m_ThreadLock.Lock();
    if (m_Thread)
    {
        if (!TerminateThread(m_Thread, 0))
        {
            AfxMessageBox(TEXT("Close thread WTF!"));
        }
        m_Thread = 0;
    }
    m_ThreadLock.Unlock();

    if (m_PlaneTrueInput)
    {
        DestroyArray(m_PlaneTrueInput);
        m_PlaneTrueInput = NULL;
    }
    if (m_TargetTrueInput)
    {
        DestroyArray(m_TargetTrueInput);
        m_TargetTrueInput = NULL;
    }
    if (m_TargetFusionInput)
    {
        DestroyArray(m_TargetFusionInput);
        m_TargetFusionInput = NULL;
    }
    if (m_TargetFilterInput)
    {
        DestroyArray(m_TargetFilterInput);
        m_TargetFilterInput = NULL;
    }
    if (m_GlobalVarInput)
    {
        DestroyArray(m_GlobalVarInput);
        m_GlobalVarInput = NULL;
    }
    */
}

void CMatlabDlg::Reset()
{
    // Stop();
    if (m_PlaneTrueInput)
    {
        DestroyArray(m_PlaneTrueInput);
        m_PlaneTrueInput = NULL;
    }
    if (m_TargetTrueInput)
    {
        DestroyArray(m_TargetTrueInput);
        m_TargetTrueInput = NULL;
    }
    if (m_TargetFusionInput)
    {
        DestroyArray(m_TargetFusionInput);
        m_TargetFusionInput = NULL;
    }
    if (m_TargetFilterInput)
    {
        DestroyArray(m_TargetFilterInput);
        m_TargetFilterInput = NULL;
    }
    if (m_GlobalVarInput)
    {
        DestroyArray(m_GlobalVarInput);
        m_GlobalVarInput = NULL;
    }

    m_PlaneTrueDatas.clear();
    m_TargetTrueDatas.clear();
    m_TargetFusionDatas.clear();
    m_TargetFilterDatas.clear();
}

void CMatlabDlg::Run()
{
    m_Lock.Lock();
    if (m_PlaneTrueInput == NULL) m_PlaneTrueInput = CreateDoubleArray(m_PlaneTrueDatas.size(), m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    if (m_TargetTrueInput == NULL) m_TargetTrueInput = CreateDoubleArray(m_TargetTrueDatas.size(), m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    if (m_TargetFusionInput == NULL) m_TargetFusionInput = CreateDoubleArray(m_TargetFusionDatas.size(), m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    if (m_TargetFilterInput == NULL) m_TargetFilterInput = CreateDoubleArray(m_TargetFilterDatas.size(), m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    if (m_GlobalVarInput == NULL) m_GlobalVarInput = CreateDoubleArray(PLANE_COUNT + TARGET_COUNT_MAX, GLOBAL_VAR_FRAME_SIZE, (const unsigned char *)NULL, 0, 0);

    double *data = mxGetPr(m_PlaneTrueInput);
    for (int plane = 0; plane < m_PlaneTrueDatas.size(); ++plane)
    {
        for (int pos = 0; pos < m_PlaneTrueDatas[plane].size(); ++pos)
        {
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].X;
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].Y;
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].Z;
        }
    }
    data = mxGetPr(m_TargetTrueInput);
    for (int target = 0; target < m_TargetTrueDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetTrueDatas[target].size(); ++pos)
        {
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].X;
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Y;
            data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Z;
        }
    }
    data = mxGetPr(m_TargetFusionInput);
    for (int target = 0; target < m_TargetFusionDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetFusionDatas[target].size(); ++pos)
        {
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Pos.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Pos.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Pos.Z;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Vel.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Vel.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Vel.Z;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Acc.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Acc.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Acc.Z;
        }
    }
    data = mxGetPr(m_TargetFilterInput);
    for (int target = 0; target < m_TargetFilterDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetFilterDatas[target].size(); ++pos)
        {
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Pos.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Pos.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Pos.Z;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Vel.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Vel.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Vel.Z;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Acc.X;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Acc.Y;
            data[(pos * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Acc.Z;
        }
    }
    data = mxGetPr(m_GlobalVarInput);
    for (int i = 0; i < PLANE_COUNT + TARGET_COUNT_MAX; ++i)
    {
        for (int j = 0; j < GLOBAL_VAR_FRAME_SIZE; ++j)
        {
            data[j * (PLANE_COUNT + TARGET_COUNT_MAX) + i] = g_GlobalVar[i].m_G[j];
        }
    }

    /*
    m_Thread = CreateThread(NULL,
        10 * 1024 * 1024,
        MatlabRun,
        (LPVOID)this,
        CREATE_SUSPENDED,
        0);
    SetThreadPriority(m_Thread, THREAD_PRIORITY_NORMAL);
    ResumeThread(m_Thread);
    */
    MatlabRunSync();
    m_Lock.Unlock();
}

void CMatlabDlg::MatlabRunSync()
{
    m_Lock.Lock();
    if (!(m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("打开Matlab引擎错误"));
    }
    else
    {
        if (engSetVisible(m_Engine, false))
        {
            AfxMessageBox(TEXT("Hide engine WTF!"));
        }
        else
        {
            engOutputBuffer(m_Engine, s_Msg, 256);
        }
    }
    int result = 0;
    wchar_t buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    wstring wsCurPath(buf);
    string curPath(wsCurPath.begin(), wsCurPath.end());

    string cmd = "cd \'" + curPath + "\\bin\'";
    result = engEvalString(m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Cd engine WTF!"));
    }
    // while (true)
    // {
        /*
        cmd += "clear";
        result = engEvalString(m_Engine, cmd.c_str());
        if (result)
        {
            AfxMessageBox(TEXT("Call clear engine WTF!"));
        }
        */
        result = engPutVariable(m_Engine, m_PlaneTrue, m_PlaneTrueInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 1 engine WTF!"));
        }
        result = engPutVariable(m_Engine, m_TargetTrue, m_TargetTrueInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 2 engine WTF!"));
        }
        result = engPutVariable(m_Engine, m_TargetFusion, m_TargetFusionInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 3 engine WTF!"));
        }
        result = engPutVariable(m_Engine, m_TargetFilter, m_TargetFilterInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 4 engine WTF!"));
        }
        result = engPutVariable(m_Engine, m_GlobalVar, m_GlobalVarInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 5 engine WTF!"));
        }

        cmd = m_FileName;
        cmd += "(";
        cmd += m_PlaneTrue;
        cmd += ", ";
        cmd += m_TargetTrue;
        cmd += ", ";
        cmd += m_TargetFusion;
        cmd += ", ";
        cmd += m_TargetFilter;
        cmd += ", ";
        cmd += m_GlobalVar;
        cmd += ")";
        result = engEvalString(m_Engine, cmd.c_str());
        if (result)
        {
            AfxMessageBox(TEXT("Call func engine WTF!"));
        }
        if (result)
        {
            // break;
        }

        // Sleep(1);
    // }


    m_Lock.Unlock();

    m_Engine = 0;

    /*
    m_Lock.Lock();
    m_Thread = 0;
    m_Lock.Unlock();
    return result;
    */
}

DWORD WINAPI CMatlabDlg::MatlabRun(LPVOID lparam)
{
    CMatlabDlg *dlg = (CMatlabDlg *)lparam;

    dlg->m_ThreadLock.Lock();
    if (!(dlg->m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("打开Matlab引擎错误"));
    }
    else
    {
        if (engSetVisible(dlg->m_Engine, false))
        {
            AfxMessageBox(TEXT("Hide engine WTF!"));
        }
        else
        {
            engOutputBuffer(dlg->m_Engine, s_Msg, 256);
        }
    }
    int result = 0;
    wchar_t buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    wstring wsCurPath(buf);
    string curPath(wsCurPath.begin(), wsCurPath.end());

    string cmd = "cd \'" + curPath + "\\bin\'";
    result = engEvalString(dlg->m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Cd engine WTF!"));
    }
    dlg->m_ThreadLock.Unlock();
    while (true)
    {
        /*
        cmd += "clear";
        result = engEvalString(dlg->m_Engine, cmd.c_str());
        if (result)
        {
            AfxMessageBox(TEXT("Call clear engine WTF!"));
        }
        */
        dlg->m_ThreadLock.Lock();
        result = engPutVariable(dlg->m_Engine, dlg->m_PlaneTrue, dlg->m_PlaneTrueInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 1 engine WTF!"));
        }
        result = engPutVariable(dlg->m_Engine, dlg->m_TargetTrue, dlg->m_TargetTrueInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 2 engine WTF!"));
        }
        result = engPutVariable(dlg->m_Engine, dlg->m_TargetFusion, dlg->m_TargetFusionInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 3 engine WTF!"));
        }
        result = engPutVariable(dlg->m_Engine, dlg->m_TargetFilter, dlg->m_TargetFilterInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 4 engine WTF!"));
        }
        result = engPutVariable(dlg->m_Engine, dlg->m_GlobalVar, dlg->m_GlobalVarInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 5 engine WTF!"));
        }

        cmd = dlg->m_FileName;
        cmd += "(";
        cmd += dlg->m_PlaneTrue;
        cmd += ", ";
        cmd += dlg->m_TargetTrue;
        cmd += ", ";
        cmd += dlg->m_TargetFusion;
        cmd += ", ";
        cmd += dlg->m_TargetFilter;
        cmd += ", ";
        cmd += dlg->m_GlobalVar;
        cmd += ")";
        result = engEvalString(dlg->m_Engine, cmd.c_str());
        if (result)
        {
            AfxMessageBox(TEXT("Call func engine WTF!"));
        }
        if (result)
        {
            break;
        }
        dlg->m_ThreadLock.Unlock();

        Sleep(1);
    }

    /*
    if (engClose(dlg->m_Engine))
    {
        AfxMessageBox(TEXT("Close engine engine WTF!"));
    }
    dlg->m_Engine = 0;
    */

    dlg->m_Lock.Lock();
    dlg->m_Thread = 0;
    dlg->m_Lock.Unlock();
    return result;
}

void CMatlabDlg::AddPlane(Plane &plane)
{
    m_PlaneTrueDatas.push_back(Path());
}

void CMatlabDlg::AddTarget(Target &target)
{
    m_TargetTrueDatas.push_back(Path());
    m_TargetFusionDatas.push_back(vector<TrueDataFrame>());
    m_TargetFilterDatas.push_back(vector<TrueDataFrame>());
}

void CMatlabDlg::SetSize(int size)
{
    m_Size = size;
}

void CMatlabDlg::AddPlaneTrueData(int plane, Position pos)
{
    if (m_PlaneTrueDatas[plane].size() >= m_Size)
    {
        return;
    }
    m_Lock.Lock();
    if (m_PlaneTrueInput)
    {
        double *data = mxGetPr(m_PlaneTrueInput);
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_PlaneTrueDatas.size() + plane] = pos.X;
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_PlaneTrueDatas.size() + plane] = pos.Y;
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_PlaneTrueDatas.size() + plane] = pos.Z;
    }
    m_PlaneTrueDatas[plane].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetTrueData(int target, Position pos)
{
    if (m_TargetTrueDatas[target].size() >= m_Size)
    {
        return;
    }
    m_Lock.Lock();
    if (m_TargetTrueInput)
    {
        double *data = mxGetPr(m_TargetTrueInput);
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_TargetTrueDatas.size() + target] = pos.X;
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_TargetTrueDatas.size() + target] = pos.Y;
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_TargetTrueDatas.size() + target] = pos.Z;
    }
    m_TargetTrueDatas[target].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetFusionData(int target, const TrueDataFrame &frame)
{
    if (m_TargetFusionDatas[target].size() >= m_Size)
    {
        return;
    }
    m_Lock.Lock();
    if (m_TargetFusionInput)
    {
        double *data = mxGetPr(m_TargetFusionInput);
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetFusionDatas.size() + target] = frame.m_Pos.X;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetFusionDatas.size() + target] = frame.m_Pos.Y;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetFusionDatas.size() + target] = frame.m_Pos.Z;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetFusionDatas.size() + target] = frame.m_Vel.X;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetFusionDatas.size() + target] = frame.m_Vel.Y;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetFusionDatas.size() + target] = frame.m_Vel.Z;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetFusionDatas.size() + target] = frame.m_Acc.X;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetFusionDatas.size() + target] = frame.m_Acc.Y;
        data[(m_TargetFusionDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetFusionDatas.size() + target] = frame.m_Acc.Z;
    }
    m_TargetFusionDatas[target].push_back(frame);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetFilterData(int target, const TrueDataFrame &frame)
{
    if (m_TargetFilterDatas[target].size() >= m_Size)
    {
        return;
    }
    m_Lock.Lock();
    if (m_TargetFilterInput)
    {
        double *data = mxGetPr(m_TargetFilterInput);
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetFilterDatas.size() + target] = frame.m_Pos.X;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetFilterDatas.size() + target] = frame.m_Pos.Y;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetFilterDatas.size() + target] = frame.m_Pos.Z;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetFilterDatas.size() + target] = frame.m_Vel.X;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetFilterDatas.size() + target] = frame.m_Vel.Y;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetFilterDatas.size() + target] = frame.m_Vel.Z;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetFilterDatas.size() + target] = frame.m_Acc.X;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetFilterDatas.size() + target] = frame.m_Acc.Y;
        data[(m_TargetFilterDatas[target].size() * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetFilterDatas.size() + target] = frame.m_Acc.Z;
    }
    m_TargetFilterDatas[target].push_back(frame);
    m_Lock.Unlock();
}

void CMatlabDlg::UpdateGlobalVar()
{
    m_Lock.Lock();
    if (m_GlobalVarInput)
    {
        double *data = mxGetPr(m_GlobalVarInput);
        int m = mxGetM(m_GlobalVarInput);
        int n = mxGetN(m_GlobalVarInput);
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                g_GlobalVar[i].m_G[j] = data[j * m + i];
            }
        }
    }
    m_Lock.Unlock();
}