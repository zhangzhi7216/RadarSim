#pragma once

#include "Target.h"
#include "DataPacket.h"
#include <afxmt.h>

#include "MatlabHelper.h"

using namespace MatlabHelper;

class __declspec(dllexport) CMatlabDlg3
{
public:
    CMatlabDlg3(const CString dllFileName = "matlab_dialog.dll", CString funcName = "matlab_dialog");
    virtual ~CMatlabDlg3(void);

    void Reset();

    void Show();
    void Hide();
    bool m_IsShowing;

    void Update();

    void SetSize(int size);

    int m_Size;

    void AddPlaneTrueData(int plane, Position pos);
    void AddTargetTrueData(int target, Position pos);

    void AddTargetFusionData(int target, const TrueDataFrame &frame);
    void AddTargetFilterData(int target, const TrueDataFrame &frame);

    vector<Path> m_PlaneTrueDatas;
    vector<Path> m_TargetTrueDatas;

    vector<vector<TrueDataFrame>> m_TargetFusionDatas;
    vector<vector<TrueDataFrame>> m_TargetFilterDatas;

    void UpdateGlobalVar();

    void AddPlane(Plane &plane);
    void AddTarget(Target &target);

    Array *m_PlaneTrueInput;
    Array *m_TargetTrueInput;
    Array *m_TargetFusionInput;
    Array *m_TargetFilterInput;
    Array *m_GlobalVarInput;

    CString m_DllFileName, m_FuncName;
    HMODULE m_DllHandle;
    RealFunc m_MatlabFunc;
};
