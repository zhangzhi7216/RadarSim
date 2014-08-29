#include "StdAfx.h"
#include "FusionMatlabAlgo.h"

FusionMatlabAlgo::FusionMatlabAlgo(void)
: m_DllHandle(0)
, m_MatlabFunc(0)
{
}

FusionMatlabAlgo::FusionMatlabAlgo(const CString &name, const CString &dllFileName, const CString &funcName)
: FusionAlgo(name)
, m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
, m_MatlabFunc(0)
{
}

FusionMatlabAlgo::~FusionMatlabAlgo(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

bool FusionMatlabAlgo::Init()
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

void FusionMatlabAlgo::Output(CArchive &ar)
{
    ar << FusionAlgoTypeMatlab << m_DllFileName << m_FuncName;
    FusionAlgo::Output(ar);
}

void FusionMatlabAlgo::Input(CArchive &ar)
{
    ar >> m_DllFileName >> m_FuncName;
    FusionAlgo::Input(ar);
}

bool FusionMatlabAlgo::Run(const FusionInput &input, FusionOutput &output)
{
    if (!m_MatlabFunc)
    {
        CString msg;
        msg.AppendFormat(TEXT("算法%s地址错误."), m_Name);
        AfxMessageBox(msg);
        return false;
    }
    int planeSize = input.m_NoiseDataPackets.size();
    int targetSize = input.m_NoiseDataPackets[0].m_TargetNoiseDatas.size();
    vector<Array *> inputList;
    Array *planeTrueDatas = CreateDoubleArray(planeSize, MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(planeTrueDatas);
    Array *targetNoiseDatas = CreateDoubleArray(planeSize * targetSize, MATLAB_FUSION_NOISE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(targetNoiseDatas);
    Array *globalVar = CreateDoubleArray(PLANE_COUNT + TARGET_COUNT_MAX, GLOBAL_VAR_FRAME_SIZE, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(globalVar);
    Array *interval = CreateDoubleArray(1, 1, (const unsigned char *)NULL, 0, 0);
    inputList.push_back(interval);

    double *p = mxGetPr(planeTrueDatas);
    for (int iPlane = 0; iPlane < planeSize; ++iPlane)
    {
        p[iPlane + 0 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Time;
        p[iPlane + 1 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Id;
        p[iPlane + 2 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Pos.X;
        p[iPlane + 3 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Pos.Y;
        p[iPlane + 4 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Pos.Z;
        p[iPlane + 5 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Vel.X;
        p[iPlane + 6 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Vel.Y;
        p[iPlane + 7 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Vel.Z;
        p[iPlane + 8 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Acc.X;
        p[iPlane + 9 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Acc.Y;
        p[iPlane + 10 * planeSize] = input.m_NoiseDataPackets[iPlane].m_PlaneTrueData.m_Acc.Z;
    }

    p = mxGetPr(targetNoiseDatas);
    for (int iPlane = 0; iPlane < planeSize; ++iPlane)
    {
        for (int iTarget = 0; iTarget < targetSize; ++iTarget)
        {
            p[(iPlane * targetSize + iTarget) + 0 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_Time;
            p[(iPlane * targetSize + iTarget) + 1 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_Id;
            p[(iPlane * targetSize + iTarget) + 2 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_Dis;
            p[(iPlane * targetSize + iTarget) + 3 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_Theta;
            p[(iPlane * targetSize + iTarget) + 4 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_Phi;
            p[(iPlane * targetSize + iTarget) + 5 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_DisVar;
            p[(iPlane * targetSize + iTarget) + 6 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_ThetaVar;
            p[(iPlane * targetSize + iTarget) + 7 * (planeSize * targetSize)] = input.m_NoiseDataPackets[iPlane].m_TargetNoiseDatas[iTarget].m_PhiVar;
        }
    }

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

    vector<Array *> outputList(4);
    bool result = m_MatlabFunc(4, &outputList[0], inputList.size(), &inputList[0]);
    if (!result)
    {
        DestroyArray(planeTrueDatas);
        DestroyArray(targetNoiseDatas);
        DestroyArray(globalVar);
        DestroyArray(interval);

        CString msg;
        msg.AppendFormat(TEXT("算法%s调用失败."), m_Name);

        return false;
    }

    Array *fusionDatas = outputList[0];
    Array *filterDatas = outputList[1];
    Array *controlDatas = outputList[2];
    Array *outputGlobalVar = outputList[3];

    p = mxGetPr(fusionDatas);
    int m = mxGetM(fusionDatas), n = mxGetN(fusionDatas);
    if (m != targetSize || n != 11)
    {
        CString msg;
        msg.AppendFormat(TEXT("Matlab融合算法输出的融合数据维度(%d, %d)错误！应该是(%d, %d)！请检查你的融合算法！"), m, n, targetSize, 11);
        AfxMessageBox(msg);
        return false;
    }
    for (int iTarget = 0; iTarget < m; ++iTarget)
    {
        TrueDataFrame fusionData;
        fusionData.m_Time = p[iTarget + 0 * m];
        fusionData.m_Id = p[iTarget + 1 * m];
        fusionData.m_Pos.X = p[iTarget + 2 * m];
        fusionData.m_Pos.Y = p[iTarget + 3 * m];
        fusionData.m_Pos.Z = p[iTarget + 4 * m];
        fusionData.m_Vel.X = p[iTarget + 5 * m];
        fusionData.m_Vel.Y = p[iTarget + 6 * m];
        fusionData.m_Vel.Z = p[iTarget + 7 * m];
        fusionData.m_Acc.X = p[iTarget + 8 * m];
        fusionData.m_Acc.Y = p[iTarget + 9 * m];
        fusionData.m_Acc.Z = p[iTarget + 10 * m];
        output.m_FusionData.m_FusionDatas.push_back(fusionData);
    }

    p = mxGetPr(filterDatas);
    m = mxGetM(filterDatas);
    n = mxGetN(filterDatas);
    if (m != targetSize || n != 11)
    {
        CString msg;
        msg.AppendFormat(TEXT("Matlab融合算法输出的滤波数据维度(%d, %d)错误！应该是(%d, %d)！请检查你的融合算法！"), m, n, targetSize, 11);
        AfxMessageBox(msg);
        return false;
    }
    for (int iTarget = 0; iTarget < m; ++iTarget)
    {
        TrueDataFrame filterData;
        filterData.m_Time = p[iTarget + 0 * m];
        filterData.m_Id = p[iTarget + 1 * m];
        filterData.m_Pos.X = p[iTarget + 2 * m];
        filterData.m_Pos.Y = p[iTarget + 3 * m];
        filterData.m_Pos.Z = p[iTarget + 4 * m];
        filterData.m_Vel.X = p[iTarget + 5 * m];
        filterData.m_Vel.Y = p[iTarget + 6 * m];
        filterData.m_Vel.Z = p[iTarget + 7 * m];
        filterData.m_Acc.X = p[iTarget + 8 * m];
        filterData.m_Acc.Y = p[iTarget + 9 * m];
        filterData.m_Acc.Z = p[iTarget + 10 * m];
        output.m_FusionData.m_FilterDatas.push_back(filterData);
    }

    for (int iPlane = 0; iPlane < planeSize; ++iPlane)
    {
        output.m_FusionData.m_NoiseDatas.push_back(input.m_NoiseDataPackets[iPlane]);
    }

    p = mxGetPr(controlDatas);
    m = mxGetM(controlDatas);
    n = mxGetN(controlDatas);
    if (m != planeSize || n != 3)
    {
        CString msg;
        msg.AppendFormat(TEXT("Matlab融合算法输出的控制数据维度(%d, %d)错误！应该是(%d, %d)！请检查你的融合算法！"), m, n, targetSize, 3);
        AfxMessageBox(msg);
        return false;
    }
    for (int iPlane = 0; iPlane < m; ++iPlane)
    {
        ControlDataPacket packet;
        packet.m_FusionData = output.m_FusionData;
        packet.m_ControlData.m_Time = p[iPlane + 0 * m];
        packet.m_ControlData.m_Id = p[iPlane + 1 * m];
        packet.m_ControlData.m_C1 = p[iPlane + 2 * m];
        output.m_ControlDatas.push_back(packet);
    }

    p = mxGetPr(outputGlobalVar);
    m = mxGetM(outputGlobalVar);
    n = mxGetN(outputGlobalVar);
    if (m != planeSize + targetSize || n != GLOBAL_VAR_FRAME_SIZE)
    {
        CString msg;
        msg.AppendFormat(TEXT("Matlab融合算法输出的全局变量维度(%d, %d)错误！应该是(%d, %d)！请检查你的融合算法！"), m, n, planeSize + targetSize, GLOBAL_VAR_FRAME_SIZE);
        AfxMessageBox(msg);
        return false;
    }
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            g_GlobalVar[i].m_G[j] = p[j * m + i];
        }
    }

    DestroyArray(planeTrueDatas);
    DestroyArray(targetNoiseDatas);
    DestroyArray(globalVar);
    DestroyArray(interval);

    DestroyArray(fusionDatas);
    DestroyArray(filterDatas);
    DestroyArray(controlDatas);
    DestroyArray(outputGlobalVar);

    return true;
}