#include "StdAfx.h"
#include "FusionLocalAlgo.h"
#include <assert.h>
#include <math.h>

FusionLocalAlgo::FusionLocalAlgo()
: m_Type(FusionLocalAlgoTypeTest1)
{
}

FusionLocalAlgo::FusionLocalAlgo(const CString &name, FusionLocalAlgoType type)
: FusionAlgo(name)
, m_Type(type)
{
}

bool FusionLocalAlgo::Init()
{
    switch (m_Type)
    {
    case FusionLocalAlgoTypeTest1:
        m_Func = &FusionLocalAlgoTest1;
        break;
    case FusionLocalAlgoTypeTest2:
        m_Func = &FusionLocalAlgoTest2;
        break;
    default:
        CString msg;
        msg.AppendFormat(TEXT("未知本地融合算法%d."), m_Type);
        AfxMessageBox(msg);
        return false;
    }
    return true;
}

void FusionLocalAlgo::Output(CArchive &ar)
{
    ar << FusionAlgoTypeLocal << m_Type;
    FusionAlgo::Output(ar);
}

void FusionLocalAlgo::Input(CArchive &ar)
{
    int type;
    ar >> type;
    m_Type = (FusionLocalAlgoType)type;
    FusionAlgo::Input(ar);
}

bool FusionLocalAlgoTest1(FusionInput &input, FusionOutput &output)
{
    return true;
}

bool FusionLocalAlgoTest2(FusionInput &input, FusionOutput &output)
{
	return true;
}