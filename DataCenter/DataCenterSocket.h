#pragma once

class CDataCenterDlg;

class DataCenterSocket :
    public CSocket
{
public:
    DataCenterSocket(CDataCenterDlg *dlg);
    ~DataCenterSocket(void);

    void OnAccept(int nErrorCode);

protected:
    CDataCenterDlg *m_Dlg;
};

