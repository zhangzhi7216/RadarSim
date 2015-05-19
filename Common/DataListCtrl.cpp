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

    InsertColumn(ColumnAisTheta, TEXT("ʱ��(s)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnAisTheta, TEXT("AIS��λ��(��)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnInfraredTheta, TEXT("���ⷽλ��(��)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnInfraredPhi, TEXT("���⸩����(��)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarDistance, TEXT("�״����(km)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarTheta, TEXT("�״﷽λ��(��)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
    InsertColumn(ColumnRadarPhi, TEXT("�״︩����(��)"), LVCFMT_CENTER, LIST_ITEM_WIDTH);
}

void CDataListCtrl::Reset()
{
    DeleteAllItems();
}

void CDataListCtrl::AddTargetData()
{
    //for (int i = 0; i < m_DataList.m_Radar.m_TargetDistances.size(); ++i)
    //{
    //    int j = m_DataList.m_Radar.m_TargetDistances[i].size() - 1;
    //    if (!m_DataList.m_Ais.IsShowTargetData(i, j) &&
    //        !m_DataList.m_Infrared.IsShowTargetData(i, j) &&
    //        !m_DataList.m_Infrared.IsShowTargetData(i, j))
    //    {
    //        continue;
    //    }
    //    int row = InsertItem(0, TEXT(""));
    //    SetItemData(row, TargetColors[m_DataList.m_TargetColors[i]].ToCOLORREF());
    //    CString str;
    //    str.AppendFormat(TEXT("%d"), m_DataList.m_Times[i].back());
    //    SetItemText(row, ColumnTime, str);
    //    if (m_DataList.m_Ais.m_Enable)
    //    {
    //        if (m_DataList.m_Ais.IsShowTargetData(i, j))
    //        {
    //            CString str;
    //            str.AppendFormat(TEXT("%f"), m_DataList.m_Ais.m_TargetThetas[i].back());
    //            SetItemText(row, ColumnAisTheta, str);
    //        }
    //    }
    //    if (m_DataList.m_Infrared.m_Enable)
    //    {
    //        if (m_DataList.m_Infrared.IsShowTargetData(i, j))
    //        {
    //            {
    //                CString str;
    //                str.AppendFormat(TEXT("%f"), m_DataList.m_Infrared.m_TargetThetas[i].back());
    //                SetItemText(row, ColumnInfraredTheta, str);
    //            }
    //            {
    //                CString str;
    //                str.AppendFormat(TEXT("%f"), m_DataList.m_Infrared.m_TargetPhis[i].back());
    //                SetItemText(row, ColumnInfraredPhi, str);
    //            }
    //        }
    //    }
    //    if (m_DataList.m_Radar.m_Enable)
    //    {
    //        if (m_DataList.m_Radar.IsShowTargetData(i, j))
    //        {
    //            {
    //                CString str;
    //                str.AppendFormat(TEXT("%f"), m_DataList.m_Radar.m_TargetDistances[i].back());
    //                SetItemText(row, ColumnRadarDistance, str);
    //            }
    //            {
    //                CString str;
    //                str.AppendFormat(TEXT("%f"), m_DataList.m_Radar.m_TargetThetas[i].back());
    //                SetItemText(row, ColumnRadarTheta, str);
    //            }
    //            {
    //                CString str;
    //                str.AppendFormat(TEXT("%f"), m_DataList.m_Radar.m_TargetPhis[i].back());
    //                SetItemText(row, ColumnRadarPhi, str);
    //            }
    //        }
    //    }
    //}
}

BEGIN_MESSAGE_MAP(CDataListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CDataListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

void CDataListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
