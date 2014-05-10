#include "StdAfx.h"
#include "MatlabDlg.h"

using namespace MatlabHelper;

CMatlabDlg::CMatlabDlg(const char *fileName,
        const char *planeTrue,
        const char *targetTrue,
        const char *targetFusion,
        const char *targetFilter)
: m_FileName(fileName)
, m_PlaneTrue(planeTrue)
, m_TargetTrue(targetTrue)
, m_TargetFusion(targetFusion)
, m_TargetFilter(targetFilter)
, m_PlaneTrueInput(NULL)
, m_TargetTrueInput(NULL)
, m_TargetFusionInput(NULL)
, m_TargetFilterInput(NULL)
, m_Size(50)
, m_Thread(NULL)
, m_Engine(NULL)
{
}

CMatlabDlg::~CMatlabDlg(void)
{
    Stop();
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

    /*
    if (m_Engine)
    {
        if (engClose(m_Engine))
        {
            AfxMessageBox(TEXT("Close engine WTF!"));
        }
        m_Engine = 0;
    }
    */
}

void CMatlabDlg::Reset()
{
    Stop();

    m_PlaneTrueDatas.clear();
    m_TargetTrueDatas.clear();
    m_TargetFusionDatas.clear();
    m_TargetFilterDatas.clear();
}

void CMatlabDlg::Run()
{
    m_Lock.Lock();
    m_PlaneTrueInput = CreateDoubleArray(m_PlaneTrueDatas.size(), m_Size * 3, (const unsigned char *)NULL, 0, 0);
    m_TargetTrueInput = CreateDoubleArray(m_TargetTrueDatas.size(), m_Size * 3, (const unsigned char *)NULL, 0, 0);
    m_TargetFusionInput = CreateDoubleArray(m_TargetFusionDatas.size(), m_Size * 6, (const unsigned char *)NULL, 0, 0);
    m_TargetFilterInput = CreateDoubleArray(m_TargetFilterDatas.size(), m_Size * 6, (const unsigned char *)NULL, 0, 0);

    double *data = mxGetPr(m_PlaneTrueInput);
    for (int plane = 0; plane < m_PlaneTrueDatas.size(); ++plane)
    {
        for (int pos = 0; pos < m_PlaneTrueDatas[plane].size(); ++pos)
        {
            data[(pos * 3 + 0) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].X;
            data[(pos * 3 + 1) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].Y;
            data[(pos * 3 + 2) * m_PlaneTrueDatas.size() + plane] = m_PlaneTrueDatas[plane][pos].Z;
        }
    }
    data = mxGetPr(m_TargetTrueInput);
    for (int target = 0; target < m_TargetTrueDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetTrueDatas[target].size(); ++pos)
        {
            data[(pos * 3 + 0) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].X;
            data[(pos * 3 + 1) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Y;
            data[(pos * 3 + 2) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Z;
        }
    }
    data = mxGetPr(m_TargetFusionInput);
    for (int target = 0; target < m_TargetFusionDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetFusionDatas[target].size(); ++pos)
        {
            data[(pos * 6 + 0) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Dis;
            data[(pos * 6 + 1) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Theta;
            data[(pos * 6 + 2) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_Phi;
            data[(pos * 6 + 3) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_DisVar;
            data[(pos * 6 + 4) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_ThetaVar;
            data[(pos * 6 + 5) * m_TargetFusionDatas.size() + target] = m_TargetFusionDatas[target][pos].m_PhiVar;
        }
    }
    data = mxGetPr(m_TargetFilterInput);
    for (int target = 0; target < m_TargetFilterDatas.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetFilterDatas[target].size(); ++pos)
        {
            data[(pos * 6 + 0) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Dis;
            data[(pos * 6 + 1) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Theta;
            data[(pos * 6 + 2) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_Phi;
            data[(pos * 6 + 3) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_DisVar;
            data[(pos * 6 + 4) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_ThetaVar;
            data[(pos * 6 + 5) * m_TargetFilterDatas.size() + target] = m_TargetFilterDatas[target][pos].m_PhiVar;
        }
    }

    m_Thread = CreateThread(NULL,
        NULL,
        MatlabRun,
        (LPVOID)this,
        CREATE_SUSPENDED,
        0);
    SetThreadPriority(m_Thread, THREAD_PRIORITY_NORMAL);
    ResumeThread(m_Thread);
    m_Lock.Unlock();
}

DWORD WINAPI CMatlabDlg::MatlabRun(LPVOID lparam)
{
    CMatlabDlg *dlg = (CMatlabDlg *)lparam;
    dlg->m_ThreadLock.Lock();
    if (!(dlg->m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("´ò¿ªMatlabÒýÇæ´íÎó"));
    }
    else
    {
        if (engSetVisible(dlg->m_Engine, false))
        {
            AfxMessageBox(TEXT("Hide engine WTF!"));
        }
    }
    dlg->m_ThreadLock.Unlock();
    int result = 0;
    wchar_t buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    wstring wsCurPath(buf);
    string curPath(wsCurPath.begin(), wsCurPath.end());
    string cmd = "cd " + curPath + "\\bin";
    result = engEvalString(dlg->m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Cd engine WTF!"));
    }
    while (true)
    {
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

        string cmd = dlg->m_FileName;
        cmd += "(";
        cmd += dlg->m_PlaneTrue;
        cmd += ", ";
        cmd += dlg->m_TargetTrue;
        cmd += ", ";
        cmd += dlg->m_TargetFusion;
        cmd += ", ";
        cmd += dlg->m_TargetFilter;
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
    m_TargetFusionDatas.push_back(vector<NoiseDataFrame>());
    m_TargetFilterDatas.push_back(vector<NoiseDataFrame>());
}

void CMatlabDlg::SetSize(int size)
{
    m_Size = size;
}

void CMatlabDlg::AddPlaneTrueData(int plane, Position pos)
{
    m_Lock.Lock();
    if (m_PlaneTrueDatas[plane].size() >= m_Size)
    {
        return;
    }
    if (m_PlaneTrueInput)
    {
        double *data = mxGetPr(m_PlaneTrueInput);
        data[(m_PlaneTrueDatas[plane].size() * 3 + 0) * m_PlaneTrueDatas.size() + plane] = pos.X;
        data[(m_PlaneTrueDatas[plane].size() * 3 + 1) * m_PlaneTrueDatas.size() + plane] = pos.Y;
        data[(m_PlaneTrueDatas[plane].size() * 3 + 2) * m_PlaneTrueDatas.size() + plane] = pos.Z;
    }
    m_PlaneTrueDatas[plane].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetTrueData(int target, Position pos)
{
    m_Lock.Lock();
    if (m_TargetTrueDatas[target].size() >= m_Size)
    {
        return;
    }
    if (m_TargetTrueInput)
    {
        double *data = mxGetPr(m_TargetTrueInput);
        data[(m_TargetTrueDatas[target].size() * 3 + 0) * m_TargetTrueDatas.size() + target] = pos.X;
        data[(m_TargetTrueDatas[target].size() * 3 + 1) * m_TargetTrueDatas.size() + target] = pos.Y;
        data[(m_TargetTrueDatas[target].size() * 3 + 2) * m_TargetTrueDatas.size() + target] = pos.Z;
    }
    m_TargetTrueDatas[target].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetFusionData(int target, const NoiseDataFrame &frame)
{
    m_Lock.Lock();
    if (m_TargetFusionDatas[target].size() >= m_Size)
    {
        return;
    }
    if (m_TargetFusionInput)
    {
        double *data = mxGetPr(m_TargetFusionInput);
        data[(m_TargetFusionDatas[target].size() * 6 + 0) * m_TargetFusionDatas.size() + target] = frame.m_Dis;
        data[(m_TargetFusionDatas[target].size() * 6 + 1) * m_TargetFusionDatas.size() + target] = frame.m_Theta;
        data[(m_TargetFusionDatas[target].size() * 6 + 2) * m_TargetFusionDatas.size() + target] = frame.m_Phi;
        data[(m_TargetFusionDatas[target].size() * 6 + 3) * m_TargetFusionDatas.size() + target] = frame.m_DisVar;
        data[(m_TargetFusionDatas[target].size() * 6 + 4) * m_TargetFusionDatas.size() + target] = frame.m_ThetaVar;
        data[(m_TargetFusionDatas[target].size() * 6 + 5) * m_TargetFusionDatas.size() + target] = frame.m_PhiVar;
    }
    m_TargetFusionDatas[target].push_back(frame);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTargetFilterData(int target, const NoiseDataFrame &frame)
{
    m_Lock.Lock();
    if (m_TargetFilterDatas[target].size() >= m_Size)
    {
        return;
    }
    if (m_TargetFilterInput)
    {
        double *data = mxGetPr(m_TargetFilterInput);
        data[(m_TargetFilterDatas[target].size() * 6 + 0) * m_TargetFilterDatas.size() + target] = frame.m_Dis;
        data[(m_TargetFilterDatas[target].size() * 6 + 1) * m_TargetFilterDatas.size() + target] = frame.m_Theta;
        data[(m_TargetFilterDatas[target].size() * 6 + 2) * m_TargetFilterDatas.size() + target] = frame.m_Phi;
        data[(m_TargetFilterDatas[target].size() * 6 + 3) * m_TargetFilterDatas.size() + target] = frame.m_DisVar;
        data[(m_TargetFilterDatas[target].size() * 6 + 4) * m_TargetFilterDatas.size() + target] = frame.m_ThetaVar;
        data[(m_TargetFilterDatas[target].size() * 6 + 5) * m_TargetFilterDatas.size() + target] = frame.m_PhiVar;
    }
    m_TargetFilterDatas[target].push_back(frame);
    m_Lock.Unlock();
}
