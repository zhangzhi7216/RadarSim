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
        msg.AppendFormat(TEXT("�޷�װ��ģ��%s."), m_DllFileName);
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
        msg.AppendFormat(TEXT("�㷨%s��ַ����."), m_Name);
        AfxMessageBox(msg);
        return false;
    }
    else
    {
#if 0
        vector<Array *> inputList;
        Array *planeTrueDatas = CreateDoubleArray(input.size(), 20, (const unsigned char *)NULL, 0, 0);
        inputList.push_back(planeTrueDatas);
        Array *targetNoiseDatas = CreateDoubleArray(input.size() * input[0].m_TargetNoiseDatas.size(), 20, (const unsigned char *)NULL, 0, 0);
        inputList.push_back(targetNoiseDatas);

        double *p = mxGetPr(planeTrueDatas);
        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            p[iPlane + 0 * input.size()] = input[iPlane].m_PlaneTrueData.m_Time;
            p[iPlane + 1 * input.size()] = input[iPlane].m_PlaneTrueData.m_Id;
            p[iPlane + 2 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.X;
            p[iPlane + 3 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.Y;
            p[iPlane + 4 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.Z;
            p[iPlane + 5 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.X;
            p[iPlane + 6 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.Y;
            p[iPlane + 7 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.Z;
            p[iPlane + 8 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.X;
            p[iPlane + 9 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.Y;
            p[iPlane + 10 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.Z;
        }

        p = mxGetPr(targetNoiseDatas);
        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            for (int iTarget = 0; iTarget < input[iPlane].m_TargetNoiseDatas.size(); ++iTarget)
            {
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 0 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Time;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 1 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Id;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 2 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Dis;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 3 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_DisVar;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 4 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Theta;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 5 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_ThetaVar;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 6 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Phi;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 7 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_PhiVar;
            }
        }

        vector<Array *> outputList(3);
        bool result = m_MatlabFunc(3, &outputList[0], input.size(), &inputList[0]);
        if (!result)
        {
            DestroyArray(planeTrueDatas);
            DestroyArray(targetNoiseDatas);

            CString msg;
            msg.AppendFormat(TEXT("�㷨%s����ʧ��."), m_Name);

            return false;
        }

        Array *NaviDatas = outputList[0];
        Array *filterDatas = outputList[1];
        Array *controlDatas = outputList[2];

        p = mxGetPr(NaviDatas);
        int m = mxGetM(NaviDatas), n = mxGetN(NaviDatas);
        for (int iTarget = 0; iTarget < m; ++iTarget)
        {
            NoiseDataFrame noiseData;
            noiseData.m_Time = p[iTarget + 0 * m];
            noiseData.m_Id = p[iTarget + 1 * m];
            noiseData.m_Dis = p[iTarget + 2 * m];
            noiseData.m_DisVar = p[iTarget + 3 * m];
            noiseData.m_Theta = p[iTarget + 4 * m];
            noiseData.m_ThetaVar = p[iTarget + 5 * m];
            noiseData.m_Phi = p[iTarget + 6 * m];
            noiseData.m_PhiVar = p[iTarget + 7 * m];
            output.m_NaviData.m_NaviDatas.push_back(noiseData);
        }

        p = mxGetPr(filterDatas);
        m = mxGetM(filterDatas);
        n = mxGetN(filterDatas);
        for (int iTarget = 0; iTarget < m; ++iTarget)
        {
            NoiseDataFrame noiseData;
            noiseData.m_Time = p[iTarget + 0 * m];
            noiseData.m_Id = p[iTarget + 1 * m];
            noiseData.m_Dis = p[iTarget + 2 * m];
            noiseData.m_DisVar = p[iTarget + 3 * m];
            noiseData.m_Theta = p[iTarget + 4 * m];
            noiseData.m_ThetaVar = p[iTarget + 5 * m];
            noiseData.m_Phi = p[iTarget + 6 * m];
            noiseData.m_PhiVar = p[iTarget + 7 * m];
            output.m_NaviData.m_FilterDatas.push_back(noiseData);
        }

        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            output.m_NaviData.m_NoiseDatas.push_back(input[iPlane]);
        }

        p = mxGetPr(controlDatas);
        m = mxGetM(controlDatas);
        n = mxGetN(controlDatas);
        for (int iPlane = 0; iPlane < m; ++iPlane)
        {
            ControlDataPacket packet;
            packet.m_ControlData.m_Time = p[iPlane + 0 * m];
            packet.m_ControlData.m_Id = p[iPlane + 1 * m];
            output.m_ControlDatas.push_back(packet);
        }

        DestroyArray(planeTrueDatas);
        DestroyArray(targetNoiseDatas);
        DestroyArray(NaviDatas);
        DestroyArray(filterDatas);
        DestroyArray(controlDatas);
#endif

        return true;
    }
}