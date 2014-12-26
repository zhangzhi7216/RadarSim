#pragma once

#include "Target.h"
#include "DataPacket.h"
#include <afxmt.h>

#include "MatlabHelper.h"

class __declspec(dllexport) CMatlabDlg
{
public:
    CMatlabDlg(const char *fileName = "matlab_dialog",
        const char *planeTrue = "plane_true",
        const char *targetTrue = "target_true",
        const char *targetFusion = "target_fusion",
        const char *targetFilter = "target_filter",
        const char *globalVar = "global_var");
    virtual ~CMatlabDlg(void);

    const char *m_FileName;
    const char *m_PlaneTrue;
    const char *m_TargetTrue;
    const char *m_TargetFusion;
    const char *m_TargetFilter;
    const char *m_GlobalVar;

    void Show();
    void Hide();
    void Reset();
    void Run();
    void MatlabRunSync();
    void Stop();
    void Update();
    void AddPlane(Plane &plane);
    void AddTarget(Target &target);

    void SetSize(int size);

    int m_Size;

    bool m_IsShowing;
    bool m_HasRan;

    CCriticalSection m_ThreadLock;
    Engine *m_Engine;
    volatile HANDLE m_Thread;
    CCriticalSection m_Lock;
    static DWORD WINAPI MatlabRun(LPVOID lparam);

    void AddPlaneTrueData(int plane, Position pos);
    void AddTargetTrueData(int target, Position pos);

    void AddTargetFusionData(int target, const TrueDataFrame &frame);
    void AddTargetFilterData(int target, const TrueDataFrame &frame);

    void UpdateGlobalVar();

    vector<Path> m_PlaneTrueDatas;
    vector<Path> m_TargetTrueDatas;

    vector<vector<TrueDataFrame>> m_TargetFusionDatas;
    vector<vector<TrueDataFrame>> m_TargetFilterDatas;

    Array *m_PlaneTrueInput;
    Array *m_TargetTrueInput;
    Array *m_TargetFusionInput;
    Array *m_TargetFilterInput;
    Array *m_GlobalVarInput;
};

