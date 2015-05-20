// FusionVcAlgoTest1.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FusionVcAlgoTest1.h"
#include "FusionAlgo.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CFusionVcAlgoTest1App

BEGIN_MESSAGE_MAP(CFusionVcAlgoTest1App, CWinApp)
END_MESSAGE_MAP()


// CFusionVcAlgoTest1App ����

CFusionVcAlgoTest1App::CFusionVcAlgoTest1App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFusionVcAlgoTest1App ����

CFusionVcAlgoTest1App theApp;


// CFusionVcAlgoTest1App ��ʼ��

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
