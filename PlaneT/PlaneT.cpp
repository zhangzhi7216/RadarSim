
// PlaneT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PlaneT.h"
#include "../PlaneDlg/Resource.h"
#include "PlaneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneTApp

BEGIN_MESSAGE_MAP(CPlaneTApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPlaneTApp ����

CPlaneTApp::CPlaneTApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

    GlobalInit();
}

CPlaneTApp::~CPlaneTApp()
{
    GlobalShut();
}

// Ψһ��һ�� CPlaneTApp ����

CPlaneTApp theApp;


// CPlaneTApp ��ʼ��

BOOL CPlaneTApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CPlaneDlg dlg(PacketTypeImDetect, TEXT("ͨ��������"), true, TEXT("ͨ��"), true, TEXT("����"), false, true);

    dlg.m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeInt, CString(TEXT("ʱ��(s)"))));
    dlg.m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("ͨ�췽λ��(��)"))));
    dlg.m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("ͨ�츩����(��)"))));
    dlg.m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("���췽λ��(��)"))));
    dlg.m_DataList.m_ColumnItems.push_back(DataList::ColumnItem(DataList::ColumnTypeDouble, CString(TEXT("���츩����(��)"))));

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
