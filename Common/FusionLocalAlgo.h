#pragma once

#include "FusionAlgo.h"

class __declspec(dllexport) FusionLocalAlgo : public FusionAlgo
{
public:
    FusionLocalAlgo();
    FusionLocalAlgo(const CString &name, FusionLocalAlgoType type);
    bool Init();

    void Output(CArchive &ar);
    void Input(CArchive &ar);

    FusionLocalAlgoType m_Type;
};

bool FusionLocalAlgoTest1(const FusionInput &input, FusionOutput &output);
bool FusionLocalAlgoTest2(const FusionInput &input, FusionOutput &output);
