#pragma once

#include "Target.h"

#include "MatlabHelper.h"

class __declspec(dllexport) CMatlabDlg
{
public:
    CMatlabDlg(LPCWSTR dllFileName, LPCWSTR funcName);
    virtual ~CMatlabDlg(void);

    vector<Path> m_PlanePaths;
    vector<Path> m_TargetPaths;

    void Show();
    void Hide();
    void Reset();
    void Run();
    void AddPlane(Plane &plane);
    void AddPlaneData(int plane, Position pos);
    void AddTarget(Target &target);
    void AddTargetData(int target, Position pos);
    void SetSize(int size) { m_Size = size; }

    LPCWSTR m_DllFileName;
    LPCWSTR m_FuncName;
    Array *m_Input;

    int m_Size;

    Engine *m_Engine;
    HANDLE m_Thread;
    static DWORD WINAPI MatlabRun(LPVOID lparam);
};

