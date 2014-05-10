#pragma once

#include "NaviAlgo.h"

class __declspec(dllexport) NaviLocalAlgo : public NaviAlgo
{
public:
    NaviLocalAlgo();
    NaviLocalAlgo(const CString &name, NaviLocalAlgoType type);
    bool Init();

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    NaviLocalAlgoType m_Type;
};

bool NaviLocalAlgoTest1(const NaviInput &input, NaviOutput &output);
bool NaviLocalAlgoTest2(const NaviInput &input, NaviOutput &output);
