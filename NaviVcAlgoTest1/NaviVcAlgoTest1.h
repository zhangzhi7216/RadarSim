// NaviVcAlgoTest1.h : NaviVcAlgoTest1 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CNaviVcAlgoTest1App
// 有关此类实现的信息，请参阅 NaviVcAlgoTest1.cpp
//

class CNaviVcAlgoTest1App : public CWinApp
{
public:
	CNaviVcAlgoTest1App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
