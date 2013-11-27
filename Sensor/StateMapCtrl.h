#pragma once
#include <afxwin.h>
#include "StateMap.h"

class __declspec(dllexport) CStateMapCtrl :
    public CStatic
{
public:
    CStateMapCtrl(StateMap &stateMap);
    virtual ~CStateMapCtrl(void);

public:
    StateMap &m_StateMap;

    Image *m_Image;
    Image *m_BackgroundImg;
    Image *m_OldBackgroundImg;
    Image *m_TargetsImg;

    virtual void DrawBackground();
    virtual void DrawTargets();
    virtual void BlendAll();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    void Reset();
    void AddTarget(Target &target);
};
