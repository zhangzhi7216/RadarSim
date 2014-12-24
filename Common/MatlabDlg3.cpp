#include "StdAfx.h"
#include "MatlabDlg3.h"

CMatlabDlg3::CMatlabDlg3(const CString dllFileName, const CString funcName)
: m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
, m_MatlabFunc(0)
, m_PlaneTrueInput(NULL)
, m_TargetTrueInput(NULL)
, m_TargetFusionInput(NULL)
, m_TargetFilterInput(NULL)
, m_GlobalVarInput(NULL)
, m_Size(50)
, m_IsShowing(false)
{
    m_DllHandle = LoadLibrary(m_DllFileName);
    if (!m_DllHandle)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法装载模块%s."), m_DllFileName);
        AfxMessageBox(msg);
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
        CString msg;
        msg.AppendFormat(TEXT("无法初始化模块%s."), m_DllFileName);
        AfxMessageBox(msg);
    }

    result = initializeFunc();

    if (!result)
    {
        CString msg;
        msg.AppendFormat(TEXT("初始化模块%s错误."), m_DllFileName);
        AfxMessageBox(msg);
    }

    string fullFuncName = sFuncName;
    fullFuncName[0] = toupper(fullFuncName[0]);
    fullFuncName = FuncPrefix + fullFuncName;
    m_MatlabFunc = (RealFunc)GetProcAddress(
        m_DllHandle,
        fullFuncName.c_str());
    if (!m_MatlabFunc)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法找到入口函数%s."), m_FuncName);
        AfxMessageBox(msg);
    }
}

CMatlabDlg3::~CMatlabDlg3(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

void CMatlabDlg3::Reset()
{
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

    m_IsShowing = false;
}

void CMatlabDlg3::Show()
{
    m_IsShowing = true;
}

void CMatlabDlg3::Hide()
{
    m_IsShowing = false;
}

void CMatlabDlg3::Update()
{
    if (!m_IsShowing)
    {
        return;
    }

    if (m_PlaneTrueInput == NULL)
    {
        m_PlaneTrueInput = CreateDoubleArray(m_PlaneTrueDatas.size(), m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    // }
    // {
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
    }
    if (m_TargetTrueInput == NULL)
    {
        m_TargetTrueInput = CreateDoubleArray(m_TargetTrueDatas.size(), m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    // }
    // {
        double *data = mxGetPr(m_TargetTrueInput);
        for (int target = 0; target < m_TargetTrueDatas.size(); ++target)
        {
            for (int pos = 0; pos < m_TargetTrueDatas[target].size(); ++pos)
            {
                data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].X;
                data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Y;
                data[(pos * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_TargetTrueDatas.size() + target] = m_TargetTrueDatas[target][pos].Z;
            }
        }
    }
    if (m_TargetFusionInput == NULL)
    {
        m_TargetFusionInput = CreateDoubleArray(m_TargetFusionDatas.size(), m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    // }
    // {
        double *data = mxGetPr(m_TargetFusionInput);
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
    }
    if (m_TargetFilterInput == NULL)
    {
        m_TargetFilterInput = CreateDoubleArray(m_TargetFilterDatas.size(), m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    // }
    // {
        double *data = mxGetPr(m_TargetFilterInput);
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
    }
    if (m_GlobalVarInput == NULL)
    {
        m_GlobalVarInput = CreateDoubleArray(PLANE_COUNT + TARGET_COUNT_MAX, GLOBAL_VAR_FRAME_SIZE, (const unsigned char *)NULL, 0, 0);
    // }
    // {
        double *data = mxGetPr(m_GlobalVarInput);
        for (int i = 0; i < PLANE_COUNT + TARGET_COUNT_MAX; ++i)
        {
            for (int j = 0; j < GLOBAL_VAR_FRAME_SIZE; ++j)
            {
                data[j * (PLANE_COUNT + TARGET_COUNT_MAX) + i] = g_GlobalVar[i].m_G[j];
            }
        }
    }
    vector<Array *> inputList;
    inputList.push_back(m_PlaneTrueInput);
    inputList.push_back(m_TargetTrueInput);
    inputList.push_back(m_TargetFusionInput);
    inputList.push_back(m_TargetFilterInput);
    inputList.push_back(m_GlobalVarInput);

    vector<Array *> outputList(1);

    bool result = m_MatlabFunc(0, &outputList[0], inputList.size(), &inputList[0]);
    if (!result)
    {
        CString msg;
        msg.AppendFormat(TEXT("函数%s调用失败."), m_FuncName);
        AfxMessageBox(msg);
    }
}

void CMatlabDlg3::SetSize(int size)
{
    m_Size = size;
}

void CMatlabDlg3::AddPlane(Plane &plane)
{
    m_PlaneTrueDatas.push_back(Path());
}

void CMatlabDlg3::AddTarget(Target &target)
{
    m_TargetTrueDatas.push_back(Path());
    m_TargetFusionDatas.push_back(vector<TrueDataFrame>());
    m_TargetFilterDatas.push_back(vector<TrueDataFrame>());
}

void CMatlabDlg3::AddPlaneTrueData(int plane, Position pos)
{
    if (m_PlaneTrueDatas[plane].size() >= m_Size)
    {
        return;
    }

    if (m_PlaneTrueInput)
    {
        double *data = mxGetPr(m_PlaneTrueInput);
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_PlaneTrueDatas.size() + plane] = pos.X;
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_PlaneTrueDatas.size() + plane] = pos.Y;
        data[(m_PlaneTrueDatas[plane].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_PlaneTrueDatas.size() + plane] = pos.Z;
    }

    m_PlaneTrueDatas[plane].push_back(pos);
}

void CMatlabDlg3::AddTargetTrueData(int target, Position pos)
{
    if (m_TargetTrueDatas[target].size() >= m_Size)
    {
        return;
    }

    if (m_TargetTrueInput)
    {
        double *data = mxGetPr(m_TargetTrueInput);
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_TargetTrueDatas.size() + target] = pos.X;
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_TargetTrueDatas.size() + target] = pos.Y;
        data[(m_TargetTrueDatas[target].size() * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_TargetTrueDatas.size() + target] = pos.Z;
    }

    m_TargetTrueDatas[target].push_back(pos);
}

void CMatlabDlg3::AddTargetFusionData(int target, const TrueDataFrame &frame)
{
    if (m_TargetFusionDatas[target].size() >= m_Size)
    {
        return;
    }

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
}

void CMatlabDlg3::AddTargetFilterData(int target, const TrueDataFrame &frame)
{
    if (m_TargetFilterDatas[target].size() >= m_Size)
    {
        return;
    }

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
}

void CMatlabDlg3::UpdateGlobalVar()
{
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
}
