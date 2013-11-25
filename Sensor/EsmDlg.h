#pragma once


// CInfraredDlg 对话框

#include "SensorDlg.h"

#include "EsmCtrl.h"

class __declspec(dllexport) CEsmDlg : public CSensorDlg
{
public:
	CEsmDlg(LPCWSTR title, Sensor &sensor, SensorClientProxy &clientProxy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEsmDlg();

    static void CreateDlg(CEsmDlg &dlg);

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};
