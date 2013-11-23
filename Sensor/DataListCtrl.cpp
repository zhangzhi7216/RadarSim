#include "StdAfx.h"
#include "DataListCtrl.h"

CDataListCtrl::CDataListCtrl(DataList &dataList)
: m_DataList(dataList)
{
}

CDataListCtrl::~CDataListCtrl(void)
{
}

void CDataListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

    InsertColumn(ColumnEsmTheta, TEXT("ESM方位角(°)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnInfraredTheta, TEXT("红外方位角(°)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnInfraredPhi, TEXT("红外俯仰角(°)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarDistance, TEXT("雷达距离(km)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarTheta, TEXT("雷达方位角(°)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarPhi, TEXT("雷达俯仰角(°)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
}

void CDataListCtrl::Reset()
{
    DeleteAllItems();
}

void CDataListCtrl::AddTarget(Target &target)
{
}

void CDataListCtrl::AddTargetData()
{
    for (int i = 0; i < m_DataList.m_Plane.m_Targets.size(); ++i)
    {
        int row = InsertItem(0, TEXT(""));
        SetItemData(row, TargetColors[m_DataList.m_TargetColors[i]].ToCOLORREF());
        if (m_DataList.m_Esm.m_Enable)
        {
            if (m_DataList.m_Plane.m_ThetaPaths[i].back() <= m_DataList.m_Esm.m_MaxTheta)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_ThetaPaths[i].back());
                SetItemText(row, ColumnEsmTheta, str);
            }
        }
        if (m_DataList.m_Infrared.m_Enable)
        {
            if (m_DataList.m_Plane.m_ThetaPaths[i].back() <= m_DataList.m_Infrared.m_MaxTheta)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_ThetaPaths[i].back());
                SetItemText(row, ColumnInfraredTheta, str);
            }
            if (m_DataList.m_Plane.m_PhiPaths[i].back() <= m_DataList.m_Infrared.m_MaxPhi)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_PhiPaths[i].back());
                SetItemText(row, ColumnInfraredPhi, str);
            }
        }
        if (m_DataList.m_Radar.m_Enable)
        {
            if (m_DataList.m_Plane.m_DistancePaths[i].back() <= m_DataList.m_Radar.m_MaxDis)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_DistancePaths[i].back());
                SetItemText(row, ColumnRadarDistance, str);
            }
            if (m_DataList.m_Plane.m_ThetaPaths[i].back() <= m_DataList.m_Radar.m_MaxTheta)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_ThetaPaths[i].back());
                SetItemText(row, ColumnRadarTheta, str);
            }
            if (m_DataList.m_Plane.m_PhiPaths[i].back() <= m_DataList.m_Radar.m_MaxPhi)
            {
                CString str;
                str.AppendFormat(TEXT("%f"), m_DataList.m_Plane.m_PhiPaths[i].back());
                SetItemText(row, ColumnRadarPhi, str);
            }
        }
    }
}

BEGIN_MESSAGE_MAP(CDataListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CDataListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

void CDataListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
    NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
    *pResult = CDRF_DODEFAULT;
    switch(nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            break;
        }
    case CDDS_ITEMPREPAINT:
        {
            int item = nmcd.dwItemSpec;
            COLORREF color = GetItemData(item);
            lplvdr->clrText = color;
        }
        break;
    }
}
