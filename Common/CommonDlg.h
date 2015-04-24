#pragma once

class __declspec(dllexport) CCommonDlg : public CDialog
{
public:
    CCommonDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    virtual ~CCommonDlg(void);

    virtual void OnSubDlgClose(void *subDlg) {}

    virtual void OnSubDlgEnable(void *subDlg) {}

    virtual void OnSubDlgShowScanline(void *subDlg) {}

    virtual void OnSubDlgShowTrack(void *subDlg) {}

    virtual void OnSubDlgTargetColor(void *subDlg) {}

    virtual void OnSubDlgMaxDis(void *subDlg) {}

    virtual void OnSubDlgMaxTheta(void *subDlg) {}

    virtual void OnSubDlgMaxPhi(void *subDlg) {}

    virtual void OnSubDlgDisVar(void *subDlg) {}

    virtual void OnSubDlgThetaVar(void *subDlg) {}

    virtual void OnSubDlgPhiVar(void *subDlg) {}

    virtual void OnSubDlgProDet(void *subDlg) {}

    virtual void OnSubDlgStateMapBackgroundChange(void *subDlg) {}

    virtual void OnSubDlgStateMapTargetsChange(void *subDlg) {}
};

