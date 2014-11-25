#pragma once

#include "Target.h"
#include "DataPacket.h"
#include <afxmt.h>

#include "MatlabHelper.h"

class __declspec(dllexport) CMatlabDlg2
{
public:
    CMatlabDlg2(const char *fileName = "matlab_dialog",
        const char *planeTrue = "plane_true",
        const char *targetTrue = "target_true",
        const char *targetFusion = "target_fusion",
        const char *targetFilter = "target_filter",
        const char *globalVar = "global_var");
    virtual ~CMatlabDlg2(void);

    void Reset();

    void Show();
    void Hide();
    bool m_IsShowing;

    void Update();

    const char *m_FileName;
    const char *m_PlaneTrue;
    const char *m_TargetTrue;
    const char *m_TargetFusion;
    const char *m_TargetFilter;
    const char *m_GlobalVar;

    void SetSize(int size);

    int m_Size;
    Engine *m_Engine;

    void AddPlaneTrueData(int plane, Position pos);
    void AddTargetTrueData(int target, Position pos);

    void AddTargetFusionData(int target, const TrueDataFrame &frame);
    void AddTargetFilterData(int target, const TrueDataFrame &frame);
    int m_PlaneDataCount;
    int m_TargetDataCount;
    int m_TargetFusionDataCount;
    int m_TargetFilterDataCount;

    void UpdateGlobalVar();

    void AddPlane(Plane &plane);
    int m_PlaneCount;
    void AddTarget(Target &target);
    int m_TargetCount;

    Array *m_PlaneTrueInput;
    Array *m_TargetTrueInput;
    Array *m_TargetFusionInput;
    Array *m_TargetFilterInput;
    Array *m_GlobalVarInput;
};
