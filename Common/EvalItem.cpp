#include "StdAfx.h"
#include "EvalItem.h"

#include "MatlabHelper.h"

using namespace MatlabHelper;

EvalItem::EvalItem(void)
: m_Name(TEXT("评估指标"))
, m_Dll(TEXT("Evaluate.dll"))
, m_Func(TEXT("Evaluate"))
{
    m_Name.AppendFormat(TEXT("%d"), s_EvalItemCount);
    s_EvalItemCount++;
}

EvalItem::~EvalItem(void)
{
}

int EvalItem::s_EvalItemCount = 0;

void EvalItem::Run(wstring &planeTrue,
        wstring &targetTrue,
        wstring &targetNoise,
        wstring &fusion,
        wstring &filter)
{
    vector<Array *> output(1);
    vector<Array *> input;

    string planeTrueS(planeTrue.begin(), planeTrue.end());
    Array *a = mxCreateString(planeTrueS.c_str());
    input.push_back(a);

    string targetTrueS(targetTrue.begin(), targetTrue.end());
    a = mxCreateString(targetTrueS.c_str());
    input.push_back(a);

    string targetNoiseS(targetNoise.begin(), targetNoise.end());
    a = mxCreateString(targetNoiseS.c_str());
    input.push_back(a);

    string fusionS(fusion.begin(), fusion.end());
    a = mxCreateString(fusionS.c_str());
    input.push_back(a);

    string filterS(filter.begin(), filter.end());
    a = mxCreateString(filterS.c_str());
    input.push_back(a);

    bool result = RunFunc(m_Dll, m_Func, output, input);

    if (!result)
    {
        CString msg;
        msg.AppendFormat(TEXT("运行指标%s失败"), m_Name);
    }

    for (int i = 0; i < input.size(); ++i)
    {
        DestroyArray(input[i]);
    }
    DestroyArray(output[0]);
}

CArchive & operator << (CArchive &ar, EvalItem &eval)
{
    ar << eval.m_Name << eval.m_Dll << eval.m_Func;
    return ar;
}

CArchive & operator >> (CArchive &ar, EvalItem &eval)
{
    ar >> eval.m_Name >> eval.m_Dll >> eval.m_Func;
    return ar;
}
