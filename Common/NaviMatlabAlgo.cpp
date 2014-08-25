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
    vector<Array *> inputList;
    Array *planeTrueData = CreateDoubleArray(1, MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(planeTrueData);
    Array *missileTrueDatas = CreateDoubleArray(input.m_MissileTrueDatas.size(), MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(missileTrueDatas);
    Array *fusionDatas = CreateDoubleArray(input.m_FusionDatas.size(), MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(fusionDatas);
    Array *filterDatas = CreateDoubleArray(input.m_FilterDatas.size(), MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(filterDatas);
    Array *controlData = CreateDoubleArray(1, MATLAB_CONTROL_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(controlData);
    Array *globalVar = CreateDoubleArray(PLANE_COUNT + TARGET_COUNT_MAX, GLOBAL_VAR_FRAME_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(globalVar);
    Array *interval = CreateDoubleArray(1, 1, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(interval);

    double *p = mxGetPr(planeTrueData);
    p[0] = input.m_PlaneTrueData.m_Time;
    p[1] = input.m_PlaneTrueData.m_Id;
    p[2] = input.m_PlaneTrueData.m_Pos.X;
    p[3] = input.m_PlaneTrueData.m_Pos.Y;
    p[4] = input.m_PlaneTrueData.m_Pos.Z;
    p[5] = input.m_PlaneTrueData.m_Vel.X;
    p[6] = input.m_PlaneTrueData.m_Vel.Y;
    p[7] = input.m_PlaneTrueData.m_Vel.Z;
    p[8] = input.m_PlaneTrueData.m_Acc.X;
    p[9] = input.m_PlaneTrueData.m_Acc.Y;
    p[10] = input.m_PlaneTrueData.m_Acc.Z;

    p = mxGetPr(missileTrueDatas);
    for (int iMissile = 0; iMissile < input.m_MissileTrueDatas.size(); ++iMissile)
    {
        p[iMissile + 0 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Time;
        p[iMissile + 1 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Id;
        p[iMissile + 2 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Pos.X;
        p[iMissile + 3 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Pos.Y;
        p[iMissile + 4 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Pos.Z;
        p[iMissile + 5 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Vel.X;
        p[iMissile + 6 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Vel.Y;
        p[iMissile + 7 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Vel.Z;
        p[iMissile + 8 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Acc.X;
        p[iMissile + 9 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Acc.Y;
        p[iMissile + 10 * input.m_MissileTrueDatas.size()] = input.m_MissileTrueDatas[iMissile].m_Acc.Z;
    }

    p = mxGetPr(fusionDatas);
    for (int iTarget = 0; iTarget < input.m_FusionDatas.size(); ++iTarget)
    {
        p[iTarget + 0 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Time;
        p[iTarget + 1 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Id;
        p[iTarget + 2 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Pos.X;
        p[iTarget + 3 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Pos.Y;
        p[iTarget + 4 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Pos.Z;
        p[iTarget + 5 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Vel.X;
        p[iTarget + 6 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Vel.Y;
        p[iTarget + 7 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Vel.Z;
        p[iTarget + 8 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Acc.X;
        p[iTarget + 9 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Acc.Y;
        p[iTarget + 10 * input.m_FusionDatas.size()] = input.m_FusionDatas[iTarget].m_Acc.Z;
    }

    p = mxGetPr(filterDatas);
    for (int iTarget = 0; iTarget < input.m_FilterDatas.size(); ++iTarget)
    {
        p[iTarget + 0 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Time;
        p[iTarget + 1 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Id;
        p[iTarget + 2 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Pos.X;
        p[iTarget + 3 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Pos.Y;
        p[iTarget + 4 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Pos.Z;
        p[iTarget + 5 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Vel.X;
        p[iTarget + 6 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Vel.Y;
        p[iTarget + 7 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Vel.Z;
        p[iTarget + 8 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Acc.X;
        p[iTarget + 9 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Acc.Y;
        p[iTarget + 10 * input.m_FilterDatas.size()] = input.m_FilterDatas[iTarget].m_Acc.Z;
    }

    p = mxGetPr(controlData);
    p[0] = input.m_ControlData.m_Time;
    p[1] = input.m_ControlData.m_Id;
    p[2] = input.m_ControlData.m_C1;

    p = mxGetPr(globalVar);
    for (int i = 0; i < PLANE_COUNT + TARGET_COUNT_MAX; ++i)
    {
        for (int j = 0; j < GLOBAL_VAR_FRAME_SIZE; ++j)
        {
            p[j * (PLANE_COUNT + TARGET_COUNT_MAX) + i] = g_GlobalVar[i].m_G[j];
        }
    }

    p = mxGetPr(interval);
    *p = input.m_Interval;

    vector<Array *> outputList(3);
    bool result = m_MatlabFunc(3, &outputList[0], inputList.size(), &inputList[0]);
    if (!result)
    {
        DestroyArray(planeTrueData);
        DestroyArray(missileTrueDatas);
        DestroyArray(fusionDatas);
        DestroyArray(filterDatas);
        DestroyArray(controlData);
        DestroyArray(globalVar);
        DestroyArray(interval);

        CString msg;
        msg.AppendFormat(TEXT("算法%s调用失败."), m_Name);

        return false;
    }

    Array *planeTrueDataOutput = outputList[0];
    Array *missileTrueDatasOutput = outputList[1];
    Array *globalVarsOutput = outputList[2];

    p = mxGetPr(planeTrueDataOutput);
   output.m_PlaneTrueData.m_Time = p[0];
   output.m_PlaneTrueData.m_Id = p[1];
   output.m_PlaneTrueData.m_Pos.X = p[2];
   output.m_PlaneTrueData.m_Pos.Y = p[3];
   output.m_PlaneTrueData.m_Pos.Z = p[4];
   output.m_PlaneTrueData.m_Vel.X = p[5];
   output.m_PlaneTrueData.m_Vel.Y = p[6];
   output.m_PlaneTrueData.m_Vel.Z = p[7];
   output.m_PlaneTrueData.m_Acc.X = p[8];
   output.m_PlaneTrueData.m_Acc.Y = p[9];
   output.m_PlaneTrueData.m_Acc.Z = p[10];

    int m = mxGetM(missileTrueDatasOutput), n = mxGetN(missileTrueDatasOutput);
    for (int iMissile = 0; iMissile < m; ++iMissile)
    {
        TrueDataFrame missileTrueData;
        missileTrueData.m_Time = p[iMissile + 0 * m];
        missileTrueData.m_Id = p[iMissile + 1 * m];
        missileTrueData.m_Pos.X = p[iMissile + 2 * m];
        missileTrueData.m_Pos.Y = p[iMissile + 3 * m];
        missileTrueData.m_Pos.Z = p[iMissile + 4 * m];
        missileTrueData.m_Vel.X = p[iMissile + 5 * m];
        missileTrueData.m_Vel.Y = p[iMissile + 6 * m];
        missileTrueData.m_Vel.Z = p[iMissile + 7 * m];
        missileTrueData.m_Acc.X = p[iMissile + 8 * m];
        missileTrueData.m_Acc.Y = p[iMissile + 9 * m];
        missileTrueData.m_Acc.Z = p[iMissile + 10 * m];
        output.m_MissileTrueDatas.push_back(missileTrueData);
    }

    p = mxGetPr(globalVarsOutput);
    m = mxGetM(globalVarsOutput);
    n = mxGetN(globalVarsOutput);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            g_GlobalVar[i].m_G[j] = p[j * m + i];
        }
    }

    DestroyArray(planeTrueData);
    DestroyArray(missileTrueDatas);
    DestroyArray(fusionDatas);
    DestroyArray(filterDatas);
    DestroyArray(controlData);
    DestroyArray(globalVar);
    DestroyArray(interval);

    DestroyArray(planeTrueDataOutput);
    DestroyArray(missileTrueDatasOutput);
    DestroyArray(globalVarsOutput);
    return true;
}
