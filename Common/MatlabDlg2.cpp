#include "StdAfx.h"
#include "MatlabDlg2.h"

using namespace MatlabHelper;

static char s_Msg[256];

CMatlabDlg2::CMatlabDlg2(const char *fileName,
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
, m_Engine(NULL)
, m_IsShowing(false)
, m_PlaneCount(0)
, m_TargetCount(0)
, m_PlaneDataCount(0)
, m_TargetDataCount(0)
, m_TargetFusionDataCount(0)
, m_TargetFilterDataCount(0)
{
}

CMatlabDlg2::~CMatlabDlg2(void)
{
    if (engClose(m_Engine))
    {
        AfxMessageBox(TEXT("Close engine engine WTF!"));
    }
}

void CMatlabDlg2::Reset()
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

    m_PlaneCount = 0;
    m_TargetCount = 0;

    m_PlaneDataCount = 0;
    m_TargetDataCount = 0;
    m_TargetFusionDataCount = 0;
    m_TargetFilterDataCount = 0;

    m_IsShowing = false;
}

void CMatlabDlg2::Show()
{
    m_IsShowing = true;
}

void CMatlabDlg2::Hide()
{
    m_IsShowing = false;
}

void CMatlabDlg2::Update()
{
    if (!m_IsShowing)
    {
        return;
    }

    if (!(m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("´ò¿ªMatlabÒýÇæ´íÎó"));
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
}

void CMatlabDlg2::SetSize(int size)
{
    m_Size = size;
    
    if (m_PlaneTrueInput == NULL)
    {
        m_PlaneTrueInput = CreateDoubleArray(m_PlaneCount, m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    }
    if (m_TargetTrueInput == NULL)
    {
        m_TargetTrueInput = CreateDoubleArray(m_TargetCount, m_Size * MATLAB_DRAW_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    }
    if (m_TargetFusionInput == NULL)
    {
        m_TargetFusionInput = CreateDoubleArray(m_TargetCount, m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    }
    if (m_TargetFilterInput == NULL)
    {
        m_TargetFilterInput = CreateDoubleArray(m_TargetCount, m_Size * MATLAB_DRAW_FUSION_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
    }
    if (m_GlobalVarInput == NULL)
    {
        m_GlobalVarInput = CreateDoubleArray(PLANE_COUNT + TARGET_COUNT_MAX, GLOBAL_VAR_FRAME_SIZE, (const unsigned char *)NULL, 0, 0);
    }
}

void CMatlabDlg2::AddPlane(Plane &plane)
{
    m_PlaneCount++;
}

void CMatlabDlg2::AddTarget(Target &target)
{
    m_TargetCount++;
}

void CMatlabDlg2::AddPlaneTrueData(int plane, Position pos)
{
    if (m_PlaneDataCount >= m_Size)
    {
        return;
    }

    if (m_PlaneTrueInput)
    {
        double *data = mxGetPr(m_PlaneTrueInput);
        data[(m_PlaneDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_PlaneCount + plane] = pos.X;
        data[(m_PlaneDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_PlaneCount + plane] = pos.Y;
        data[(m_PlaneDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_PlaneCount + plane] = pos.Z;
    }

    m_PlaneDataCount++;
}

void CMatlabDlg2::AddTargetTrueData(int target, Position pos)
{
    if (m_TargetDataCount >= m_Size)
    {
        return;
    }

    if (m_TargetTrueInput)
    {
        double *data = mxGetPr(m_TargetTrueInput);
        data[(m_TargetDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 0) * m_TargetCount + target] = pos.X;
        data[(m_TargetDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 1) * m_TargetCount + target] = pos.Y;
        data[(m_TargetDataCount * MATLAB_DRAW_TRUE_DATA_SIZE + 2) * m_TargetCount + target] = pos.Z;
    }

    m_TargetDataCount++;
}

void CMatlabDlg2::AddTargetFusionData(int target, const TrueDataFrame &frame)
{
    if (m_TargetFusionDataCount >= m_Size)
    {
        return;
    }

    if (m_TargetFusionInput)
    {
        double *data = mxGetPr(m_TargetFusionInput);
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetCount + target] = frame.m_Pos.X;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetCount + target] = frame.m_Pos.Y;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetCount + target] = frame.m_Pos.Z;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetCount + target] = frame.m_Vel.X;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetCount + target] = frame.m_Vel.Y;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetCount + target] = frame.m_Vel.Z;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetCount + target] = frame.m_Acc.X;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetCount + target] = frame.m_Acc.Y;
        data[(m_TargetFusionDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetCount + target] = frame.m_Acc.Z;
    }

    m_TargetFusionDataCount++;
}

void CMatlabDlg2::AddTargetFilterData(int target, const TrueDataFrame &frame)
{
    if (m_TargetDataCount >= m_Size)
    {
        return;
    }

    if (m_TargetFilterInput)
    {
        double *data = mxGetPr(m_TargetFilterInput);
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 0) * m_TargetCount + target] = frame.m_Pos.X;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 1) * m_TargetCount + target] = frame.m_Pos.Y;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 2) * m_TargetCount + target] = frame.m_Pos.Z;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 3) * m_TargetCount + target] = frame.m_Vel.X;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 4) * m_TargetCount + target] = frame.m_Vel.Y;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 5) * m_TargetCount + target] = frame.m_Vel.Z;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 6) * m_TargetCount + target] = frame.m_Acc.X;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 7) * m_TargetCount + target] = frame.m_Acc.Y;
        data[(m_TargetFilterDataCount * MATLAB_DRAW_FUSION_DATA_SIZE + 8) * m_TargetCount + target] = frame.m_Acc.Z;
    }

    m_TargetFilterDataCount++;
}

void CMatlabDlg2::UpdateGlobalVar()
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
