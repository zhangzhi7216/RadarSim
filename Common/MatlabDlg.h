#pragma once

#include "Target.h"
#include <afxmt.h>

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
    void SetSize(int size);

    LPCWSTR m_DllFileName;
    LPCWSTR m_FuncName;
    Array *m_PlaneInput;
    Array *m_TargetInput;

    int m_Size;

    Engine *m_Engine;
    HANDLE m_Thread;
    CCriticalSection m_Lock;
    static DWORD WINAPI MatlabRun(LPVOID lparam);
};

