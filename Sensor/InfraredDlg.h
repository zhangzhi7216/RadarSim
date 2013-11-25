#pragma once


// CInfraredDlg 对话框

#include "SensorDlg.h"

#include "InfraredCtrl.h"

class __declspec(dllexport) CInfraredDlg : public CSensorDlg
{
public:
	CInfraredDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfraredDlg();

    static void CreateDlg(CInfraredDlg &dlg);

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};
