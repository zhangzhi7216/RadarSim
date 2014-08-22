// NaviVcAlgoTest1.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "NaviVcAlgoTest1.h"
#include "NaviAlgo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CNaviVcAlgoTest1App

BEGIN_MESSAGE_MAP(CNaviVcAlgoTest1App, CWinApp)
END_MESSAGE_MAP()


// CNaviVcAlgoTest1App 构造

CNaviVcAlgoTest1App::CNaviVcAlgoTest1App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNaviVcAlgoTest1App 对象

CNaviVcAlgoTest1App theApp;


// CNaviVcAlgoTest1App 初始化

BOOL CNaviVcAlgoTest1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllimport) GlobalVarFrame *g_GlobalVar;

extern "C" __declspec(dllexport) bool NaviVcAlgoTest1(const NaviInput &input, NaviOutput &output)
{
    // How to get interval.
    int interval = input.m_Interval;

    // How to use global var.
    g_GlobalVar[0].m_G[0] = 0;
    g_GlobalVar[0].m_G[1] = 0;

   output.m_PlaneTrueData.m_Pos = Position(20000, 30000, 0);
    return true;
}
