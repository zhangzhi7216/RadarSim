#include "StdAfx.h"
#include "OneTimeMatlabDlg.h"

using namespace MatlabHelper;

COneTimeMatlabDlg::COneTimeMatlabDlg()
: m_Engine(NULL)
{
}

COneTimeMatlabDlg::~COneTimeMatlabDlg(void)
{
    Stop();
}

void COneTimeMatlabDlg::Stop()
{
}

static char s_Msg[256];

void COneTimeMatlabDlg::Run(int round)
{
    if (!(m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("��Matlab�������"));
    }
    else
    {
        if (engSetVisible(m_Engine, false))
        {
            AfxMessageBox(TEXT("Hide engine WTF!"));
        }
        else
        {
            engOutputBuffer(m_Engine, s_Msg, 256);
        }
    }
    int result = 0;

    Array *roundA = CreateDoubleArray(1, 1, (const unsigned char *)NULL, 0, 0);
    double *p = mxGetPr(roundA);
    *p = round;
    result = engPutVariable(m_Engine, "round", roundA);
    if (result)
    {
        AfxMessageBox(TEXT("Put var round engine WTF!"));
    }

    wchar_t buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    wstring wsCurPath(buf);
    string curPath(wsCurPath.begin(), wsCurPath.end());
    string cmd = "cd \'" + curPath + "\\bin\'";
    result = engEvalString(m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Cd engine WTF!"));
    }

    cmd = "onetime_matlab_dialog(round)";
    result = engEvalString(m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Call func engine WTF!"));
    }
    /*
    cmd = "clear";
    result = engEvalString(m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Call func engine WTF!"));
    }
    */

    /*
    if (m_Engine)
    {
        if (engClose(m_Engine))
        {
            AfxMessageBox(TEXT("Close engine WTF!"));
        }
        m_Engine = 0;
    }
    */
}
