#pragma once

class __declspec(dllexport) EvalItem
{
public:
    EvalItem(void);
    virtual ~EvalItem(void);

    CString m_Name, m_Dll, m_Func;

    void Run(wstring &planeTrue,
        wstring &targetTrue,
        wstring &targetNoise,
        wstring &fusion,
        wstring &filter);

    static int s_EvalItemCount;
};
__declspec(dllexport) CArchive & operator << (CArchive &ar, EvalItem &eval);
__declspec(dllexport) CArchive & operator >> (CArchive &ar, EvalItem &eval);
