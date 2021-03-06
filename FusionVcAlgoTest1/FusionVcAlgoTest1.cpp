// FusionVcAlgoTest1.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FusionVcAlgoTest1.h"
#include "FusionAlgo.h"
#include <assert.h>

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

// CFusionVcAlgoTest1App

BEGIN_MESSAGE_MAP(CFusionVcAlgoTest1App, CWinApp)
END_MESSAGE_MAP()


// CFusionVcAlgoTest1App 构造

CFusionVcAlgoTest1App::CFusionVcAlgoTest1App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFusionVcAlgoTest1App 对象

CFusionVcAlgoTest1App theApp;


// CFusionVcAlgoTest1App 初始化

BOOL CFusionVcAlgoTest1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllimport) GlobalVarFrame *g_GlobalVar;

extern "C" __declspec(dllexport) bool FusionVcAlgoTest1(FusionInput &input, FusionOutput &output)
{
    // This is how to use global var.
    g_GlobalVar[0].m_G[0] = 0;
    g_GlobalVar[0].m_G[1] = 0;
    map<SensorId, NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
    int interval = input.m_Interval;

    int nTargets = noiseDatas[SensorIdRadar].m_TargetNoiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        TrueDataFrame frame;
        frame.m_Time = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget].m_Time;
        frame.m_Id = 100 + iTarget + frame.m_Time / 100 * 100;
        for (int iSensor = SensorIdRadar; iSensor < SensorIdLast; ++iSensor)
        {
            // frame += noiseDatas[(SensorId)iSensor].m_TargetNoiseDatas[iTarget];
        }
        // frame /= SensorIdLast;
        output.m_FusionDataPacket.m_FusionDatas.push_back(frame);
        // frame = noiseDatas[SensorIdRadar].m_TargetNoiseDatas[iTarget];
    }

    return true;
}
