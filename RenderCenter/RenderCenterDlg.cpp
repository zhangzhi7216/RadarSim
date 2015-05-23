
// RenderCenterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "RenderCenter.h"
#include "RenderCenterDlg.h"
#include "CvvImage.h"
#include "FeatureMatch.h"
#include "LaplacianBlending.h"
#include "RateBlending.h"
#include "Utility.h"

using namespace Utility;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926
string dir_path = "D:\\test\\imageA\\"; 
string dir_path1= "D:\\test\\imageB\\";
string dir_path2="D:\\test\\fusionimage\\";
Directory dir;  
CRenderCenterDlg * _Global_Obj_Ptr=NULL;
CCriticalSection g_Lock;

int i=0;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	

	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);

	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}


}
VOID CALLBACK TimerProc1(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedLaplace();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc2(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];  
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc3(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedRate();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc4(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc5(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedLaplace();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc6(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedHisfusion();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc7(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedRate();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc8(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedImagefusion();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc9(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedLaplace();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc10(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedHisfusion();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc11(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedRate();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc12(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedImagefusion();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc13(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedLaplace();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc14(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedHisfusion();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc15(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRate();	
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc16(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	

	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc17(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	

	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedLaplace();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc18(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	

	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc19(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	

	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i]; 
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();	
		_Global_Obj_Ptr->OnBnClickedRegistry();
		_Global_Obj_Ptr->OnBnClickedRate();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc20(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc21(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedLaplace();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc22(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc23(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		_Global_Obj_Ptr->OnBnClickedRate();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc24(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc25(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedLaplace();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc26(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc27(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRegistry();		
		_Global_Obj_Ptr->OnBnClickedRate();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc28(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc29(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);	
		
		_Global_Obj_Ptr->OnBnClickedLaplace();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc30(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();

	}
	else
	{
		i=0;
	}
}
VOID CALLBACK TimerProc31(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	//	_Global_Obj_Ptr->ImageName=dir.GetListFiles(dir_path2,"*.*",false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];
		string fileName1=fileNames1[i];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		
		_Global_Obj_Ptr->OnBnClickedRate();
		_Global_Obj_Ptr->OnBnClickedTarget();
		string fileFullName2=dir_path2+fileName;
		g_Lock.Lock();
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		_Global_Obj_Ptr->m_OutputPaths.push_back(fileFullName2.c_str());
		i++;
		g_Lock.Unlock();
	}
	else
	{
		i=0;
	}
}

// CRenderCenterDlg �Ի���




CRenderCenterDlg::CRenderCenterDlg(CWnd* pParent /*=NULL*/)
	: CCommonDlg(CRenderCenterDlg::IDD, pParent)
    , m_FusionSocket(NULL)
,	 m_CurrentPath(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_RenderCenterSocket = new RenderCenterSocket(this);
}

void CRenderCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboL);
	DDX_Control(pDX, IDC_IMGA, m_imageA);
	DDX_Control(pDX, IDC_IMGB, m_imageB);
	DDX_Control(pDX, IDC_IMGFUSION, m_imagefusion);
	DDX_Control(pDX, IDC_IMGTURN, m_imageturn);
}

BEGIN_MESSAGE_MAP(CRenderCenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_INPUTA, &CRenderCenterDlg::OnBnClickedInputa)
	ON_BN_CLICKED(IDC_INPUTB, &CRenderCenterDlg::OnBnClickedInputb)
	ON_BN_CLICKED(IDC_INPUTMUBAN, &CRenderCenterDlg::OnBnClickedInputmuban)
	ON_BN_CLICKED(IDC_ENHANCE, &CRenderCenterDlg::OnBnClickedEnhance)
	ON_BN_CLICKED(IDC_REGISTRY, &CRenderCenterDlg::OnBnClickedRegistry)
	ON_BN_CLICKED(IDC_IMAGEFUSION, &CRenderCenterDlg::OnBnClickedImagefusion)
	ON_BN_CLICKED(IDC_LAPLACE, &CRenderCenterDlg::OnBnClickedLaplace)
	ON_BN_CLICKED(IDC_RATE, &CRenderCenterDlg::OnBnClickedRate)
	ON_BN_CLICKED(IDC_HISFUSION, &CRenderCenterDlg::OnBnClickedHisfusion)
	ON_BN_CLICKED(IDC_TARGET, &CRenderCenterDlg::OnBnClickedTarget)
	ON_BN_CLICKED(IDC_START, &CRenderCenterDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CRenderCenterDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SAVE, &CRenderCenterDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CRenderCenterDlg ��Ϣ�������

BOOL CRenderCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	m_ComboL.AddString(_T("ͼ��򵥼�Ȩ�ں�"));
	m_ComboL.AddString(_T("ͼ��������˹�ں�"));
	m_ComboL.AddString(_T("ͼ��HIS�ں�"));
	m_ComboL.AddString(_T("ͼ����ʵ�ͨ�ں�"));
	m_ComboL.SetCurSel(0);
    
    if (!m_RenderCenterSocket->Create(RENDER_CENTER_PORT))
    {
        AfxMessageBox(TEXT("�׽��ִ���ʧ��."));
        exit(-1);
    }
    if (!m_RenderCenterSocket->Listen())
    {
        AfxMessageBox(TEXT("����ʧ��."));
        exit(-1);
    }
    if (!m_RenderCenterSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("ѡ��ʧ��."));
        exit(-1);
    }

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRenderCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRenderCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CRenderCenterDlg::hsi2rgb(Mat &hsi,Mat &image)
{
	if(!hsi.data)
	{  
		cout<<"Miss Data"<<endl;  
		return -1;  
	}  
	int nl = hsi.rows;  
	int nc = hsi.cols;  
	if(hsi.isContinuous())
	{  
		nc = nc*nl;  
		nl = 1;  
	}  
	for(int i = 0;i < nl;i++)
	{
		uchar *src = hsi.ptr<uchar>(i);  
		uchar *dst = image.ptr<uchar>(i);  
		for(int j = 0;j < nc;j++)
		{
			float H = src[j*3]/255.0;  
			float S = src[j*3+1]/255.0;  
			float I = src[j*3+2]/255.0;  
			float b,g,r;
			if(I==0)
			{
				b=g=r=0;
			}
			else
			{
				if(S==0)
				{
					r=g=b=1;
				}
				float temp1,temp2,temp3;
				temp1=(1-S)/3;
				temp2=S*cos(H);
				if(H>=0&&H<(PI*2/3))
				{
					b=temp1;temp3=cos(PI/3-H);
					r=(1+temp2/temp3)/3;
					g=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
				else if(H>=(PI*2/3)&&H<(PI*4/3))
				{
					r=temp1;
					temp3=cos(PI-H);
					g=(1+temp2/temp3)/3;
					b=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
				else if(H>=(PI*4/3)&&H<(PI*2))
				{
					g=temp1;
					temp3=cos(PI*5/3-H);
					b=(1+temp2/temp3)/3;
					r=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
			}
			dst[3*j] = b*255;  
			dst[3*j+1] = g*255;  
			dst[3*j+2] = r*255;  

		}

	}
	return 0;
}
int CRenderCenterDlg::rgb2hsi(Mat &image,Mat &hsi)
{
	if(!image.data)
	{  
		cout<<"Miss Data"<<endl;  
		return -1;  
	}  
	int nl = image.rows;  
	int nc = image.cols;  
	if(image.isContinuous())
	{  
		nc = nc*nl;  
		nl = 1;  
	}  
	for(int i = 0;i < nl;i++)
	{  
		uchar *src = image.ptr<uchar>(i);  
		uchar *dst = hsi.ptr<uchar>(i);  
		for(int j = 0;j < nc;j++)
		{  
			float b = src[j*3]/255.0;  
			float g = src[j*3+1]/255.0;  
			float r = src[j*3+2]/255.0;  

			float num = (float)(0.5*((r-g)+(r-b)));  
			float den = (float)sqrt((r-g)*(r-g)+(r-b)*(g-b));  
			float H,S,I;  
			if(den == 0)
			{   //��ĸ����Ϊ0  
				H = 0;  
			}  
			else
			{  
				double theta = acos(num/den);  
				if(b <= g)  
					H = theta/(PI*2);  
				else  
					H = (2*PI - theta)/(2*PI);  
			}  
			double minRGB = min(min(r,g),b);  
			den = r+g+b;  
			if(den == 0)    //��ĸ����Ϊ0  
				S = 0;  
			else  
				S = 1 - 3*minRGB/den;  
			I = den/3.0;  
			//��S������H���������䵽[0,255]�����Ա�����ʾ;  
			//һ��H������[0,2pi]֮�䣬S��[0,1]֮��  
			dst[3*j] = H*255;  
			dst[3*j+1] = S*255;  
			dst[3*j+2] = I*255;  
		}  
	}  
	return 0;  
}

Mat CRenderCenterDlg::GetImage(double &p,double &q)
{
	//���ܸ����ͷ�λ������һ��ͼƬ
	Mat getimage;
	return getimage;
}
void CRenderCenterDlg::OnBnClickedInputa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=wstring2string(tFileName.GetBuffer(0));
	}
	img1=imread(tstring,1);
	if(!img1.data)
	{
		MessageBox(TEXT("error","no image loaded!"),MB_OK);
		return;
	}


	CWnd *pWnd=GetDlgItem(IDC_IMGA);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGA)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedInputb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=wstring2string(tFileName.GetBuffer(0));
	}
	img2=imread(tstring,1);
	if(!img2.data)
	{
		MessageBox(TEXT("error","no image loaded!"),MB_OK);
		return;
	}
	CWnd *pWnd=GetDlgItem(IDC_IMGB);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img2;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGB)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedInputmuban()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=wstring2string(tFileName.GetBuffer(0));
	}
	imgtarget=imread(tstring,1);
	if(!imgtarget.data)
	{
		MessageBox(TEXT("error"),TEXT("no image loaded!"),MB_OK);
		return;
	}
	CWnd *pWnd=GetDlgItem(IDC_MUBAN);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgtarget;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_MUBAN)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedEnhance()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!img1.data)
	{
		MessageBox(TEXT("no image loaded!"),TEXT("error"),MB_OK);
		return;
	}
	vector<Mat> splitBGR(img1.channels());
	split(img1,splitBGR);
	for(int i=0;i<img1.channels();i++)
		equalizeHist(splitBGR[i],splitBGR[i]);
	merge(splitBGR,img1);	
	CWnd *pWnd=GetDlgItem(IDC_IMGA);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGA)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedRegistry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!img1.data||!img2.data)
	{
		MessageBox(TEXT("no image loaded!"),TEXT("error"),MB_OK);
		return;
	}
	cv::initModule_nonfree();
	FeatureMatch *sp=NULL;
	bool isWarpPerspective = 0;
	double ransacReprojThreshold = 3;
	Ptr<FeatureDetector> detector = FeatureDetector::create( "SIFT");
	Ptr<DescriptorExtractor> descriptorExtractor = DescriptorExtractor::create( "SIFT");
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create( "FlannBased");//"BruteForce");
	int mactherFilterType = sp->getMatcherFilterType( "CrossCheckFilter");
	bool eval = false;
	vector<KeyPoint> keypoints1;
	detector->detect( img1, keypoints1 );
	Mat descriptors1;
	descriptorExtractor->compute( img1, keypoints1, descriptors1 );
	RNG rng = theRNG();
	img3=sp->doIteration( img1, img2, isWarpPerspective, keypoints1, descriptors1,
		detector, descriptorExtractor, descriptorMatcher, mactherFilterType, eval,
		ransacReprojThreshold, rng );
	img1=img3;
	CWnd *pWnd=GetDlgItem(IDC_IMGTURN);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGTURN)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedImagefusion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(img1.size()!=img2.size())
	{
		MessageBox(TEXT("Two images are not the same size!"),TEXT("error"),MB_OK);
		return;
	}
	double alpha=0.6;
	double beta=(1.0-alpha);

	addWeighted(img1,alpha,img2,beta,0.0,imgfusion);
	

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedLaplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(img1.size()!=img2.size())
	{
		MessageBox(TEXT("Two images are not the same size!"),TEXT("error"),MB_OK);
		return;
	}
	LaplacianBlending lp;
	Mat_<Vec3f> l; img1.convertTo(l,CV_32F,1.0/255.0);//Vec3f��ʾ������ͨ������ l[row][column][depth]  
	Mat_<Vec3f> r; img2.convertTo(r,CV_32F,1.0/255.0);
	Mat_<Vec3f> blend =lp.LaplacianBlend(l, r);
	blend.convertTo(imgfusion,CV_8UC3,255);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedRate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(img1.size()!=img2.size())
	{
		MessageBox(TEXT("Two images are not the same size!"),TEXT("error"),MB_OK);
		return;
	}
	RateBlending lp;
	Mat_<Vec3f> l; img1.convertTo(l,CV_32F,1.0/255.0);//Vec3f��ʾ������ͨ������ l[row][column][depth]  
	Mat_<Vec3f> r; img2.convertTo(r,CV_32F,1.0/255.0);
	Mat_<float> m(l.rows,l.cols,0.0);
	m(Range::all(),Range(0,m.cols/2)) = 1.0; 
	Mat_<Vec3f> blend =lp.RateBlend(l, r, m);
	blend.convertTo(imgfusion,CV_8UC3,255);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedHisfusion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(img1.size()!=img2.size())
	{
		MessageBox(TEXT("Two images are not the same size!"),TEXT("error"),MB_OK);
		
		return;
	}
	Mat hsi1,hsi2,hsi3,p1,p2;
	p1=img1;
	p2=img2;
	hsi1.create(p1.rows,img1.cols,CV_8UC3);
	hsi2.create(p2.rows,img2.cols,CV_8UC3);
	hsi3=hsi1;
	rgb2hsi(p1,hsi1);
	rgb2hsi(p2,hsi2);
	int n1=hsi1.rows;
	int nc=hsi1.cols;
	if(hsi1.isContinuous())
	{
		nc=nc*n1;
		n1=1;
	}
	for(int i=0;i<n1;i++)
	{
		uchar *pl=hsi1.ptr<uchar>(i);
		uchar *pr=hsi2.ptr<uchar>(i);		
		for(int j=0;j<nc;j++)
		{
			uchar t;
			t=pl[3*j+2];
			pl[3*j+2]=pr[3*j+2];
			pr[3*j+2]=t;

		}
	}
	hsi2rgb(hsi1,p1);
	hsi2rgb(hsi2,p2);

	double alpha=0.5;
	double beta=(1.0-alpha);	
	addWeighted(p1,alpha,p2,beta,0.0,imgfusion);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedTarget()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!imgtarget.data)
	{
		MessageBox(TEXT("no target image loaded!"),TEXT("error"),MB_OK);
		return;
	}
	SiftFeatureDetector siftdtc;
	vector<KeyPoint>kp1,kp2;
	siftdtc.detect(imgtarget,kp1);
	siftdtc.detect(imgfusion,kp2);

	SiftDescriptorExtractor extractor;
	Mat descriptor1,descriptor2;
	extractor.compute(imgtarget,kp1,descriptor1);
	extractor.compute(imgfusion,kp2,descriptor2);

	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptor1,descriptor2,matches);

	int i,j;
	int pointcount=(int)matches.size();
	Mat point1(pointcount,2,CV_32F);
	Mat point2(pointcount,2,CV_32F);
	Point2f point;
	for(i=0;i<pointcount;i++)
	{
		point=kp1[matches[i].queryIdx].pt;
		point1.at<float>(i,0)=point.x;
		point1.at<float>(i,1)=point.y;

		point=kp2[matches[i].trainIdx].pt;
		point2.at<float>(i,0)=point.x;
		point2.at<float>(i,1)=point.y;
	}
	Mat m_fundamental;
	vector<uchar> m_ransacstatus;
	m_fundamental=findFundamentalMat(point1,point2,m_ransacstatus,FM_RANSAC);

	float hhh[9];
	for(i=0;i<9;i++)
		hhh[i]=0;

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			hhh[i*3+j]=m_fundamental.ptr<float>(i)[j];
		}
	}
	int outlinercount=0;
	for(i=0;i<pointcount;i++)
	{
		if(m_ransacstatus[i]==0)
		{
			outlinercount++;
		}
	}

	vector<Point2f> m_leftinliner;
	vector<Point2f> m_rightinliner;
	vector<DMatch> m_inlinermatches;
	int inlinercount=pointcount-outlinercount;
	m_inlinermatches.resize(inlinercount);
	m_leftinliner.resize(inlinercount);
	m_rightinliner.resize(inlinercount);
	inlinercount=0;
	for(i=0;i<pointcount;i++)
	{
		if(m_ransacstatus[i]!=0)
		{
			m_leftinliner[inlinercount].x=point1.at<float>(i,0);
			m_leftinliner[inlinercount].y=point1.at<float>(i,1);
			m_rightinliner[inlinercount].x=point2.at<float>(i,0);
			m_rightinliner[inlinercount].y=point2.at<float>(i,1);
			m_inlinermatches[inlinercount].queryIdx=inlinercount;
			m_inlinermatches[inlinercount].trainIdx=inlinercount;
			inlinercount++;
		}
	}
	vector<KeyPoint> key1(inlinercount);
	vector<KeyPoint> key2(inlinercount);
	KeyPoint::convert(m_leftinliner,key1);
	KeyPoint::convert(m_rightinliner,key2);

	Mat H=findHomography(m_leftinliner,m_rightinliner,CV_RANSAC);
	std::vector<Point2f> obj_corners(4);
    obj_corners[0]=cv::Point(0,0);obj_corners[1]=cv::Point(imgtarget.cols,0);
    obj_corners[2]=cv::Point(imgtarget.cols,imgtarget.rows);obj_corners[3]=cv::Point(0,imgtarget.rows);
	std::vector<Point2f>scene_corners(4);
	perspectiveTransform(obj_corners,scene_corners,H);
	rectangle(imgfusion,scene_corners[0],scene_corners[2],Scalar(0,0,255,0),1,8,0);


	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{

		switch(m_ComboL.GetCurSel())
		{			
		case 0:SetTimer(1,1000,TimerProc);break;  
		case 1:SetTimer(2,1000,TimerProc1);break;
		case 2:SetTimer(3,1000,TimerProc2);break;
		case 3:SetTimer(4,1000,TimerProc3);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(5,1000,TimerProc4);break;
		case 1:SetTimer(6,1000,TimerProc5);break;
		case 2:SetTimer(7,1000,TimerProc6);break;
		case 3:SetTimer(8,1000,TimerProc7);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(9,1000,TimerProc8);break;
		case 1:SetTimer(10,1000,TimerProc9);break;
		case 2:SetTimer(11,1000,TimerProc10);break;
		case 3:SetTimer(12,1000,TimerProc11);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(13,1000,TimerProc12);break;
		case 1:SetTimer(14,1000,TimerProc13);break;
		case 2:SetTimer(15,1000,TimerProc14);break;
		case 3:SetTimer(16,1000,TimerProc15);break;
		}

	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		if(!imgtarget.data)
		{
			MessageBox(TEXT("no target image loaded!"),TEXT("error"),MB_OK);
			return;
		}
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(17,1000,TimerProc16);break;
		case 1:SetTimer(18,1000,TimerProc17);break;
		case 2:SetTimer(19,1000,TimerProc18);break;
		case 3:SetTimer(20,1000,TimerProc19);break;
		}

	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		if(!imgtarget.data)
		{
			MessageBox(TEXT("no target image loaded!"),TEXT("error"),MB_OK);
			return;
		}		
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(21,1000,TimerProc20);break;
		case 1:SetTimer(22,1000,TimerProc21);break;
		case 2:SetTimer(23,1000,TimerProc22);break;
		case 3:SetTimer(24,1000,TimerProc23);break;
		}
	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		if(!imgtarget.data)
		{
			MessageBox(TEXT("no target image loaded!"),TEXT("error"),MB_OK);
			return;
		}
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(25,1000,TimerProc24);break;
		case 1:SetTimer(26,1000,TimerProc25);break;
		case 2:SetTimer(27,1000,TimerProc26);break;
		case 3:SetTimer(28,1000,TimerProc27);break;
		}
	}
	else
	{
		if(!imgtarget.data)
		{
			MessageBox(TEXT("no target image loaded!"),TEXT("error"),MB_OK);
			return;
		}
		switch(m_ComboL.GetCurSel())
		{
		case 0:SetTimer(29,1000,TimerProc28);break;
		case 1:SetTimer(30,1000,TimerProc29);break;
		case 2:SetTimer(31,1000,TimerProc30);break;
		case 3:SetTimer(32,1000,TimerProc31);break;
		}

	}
	i=0;
	_Global_Obj_Ptr = this;

}


void CRenderCenterDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{

		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(1);break;
		case 1:KillTimer(2);break;
		case 2:KillTimer(3);break;
		case 3:KillTimer(4);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(5);break;
		case 1:KillTimer(6);break;
		case 2:KillTimer(7);break;
		case 3:KillTimer(8);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(9);break;
		case 1:KillTimer(10);break;
		case 2:KillTimer(11);break;
		case 3:KillTimer(12);break;
		}
	}
	else if(BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(13);break;
		case 1:KillTimer(14);break;
		case 2:KillTimer(15);break;
		case 3:KillTimer(16);break;
		}

	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(17);break;
		case 1:KillTimer(18);break;
		case 2:KillTimer(19);break;
		case 3:KillTimer(20);break;
		}
	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(21);break;
		case 1:KillTimer(22);break;
		case 2:KillTimer(23);break;
		case 3:KillTimer(24);break;
		}
	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_CHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(25);break;
		case 1:KillTimer(26);break;
		case 2:KillTimer(27);break;
		case 3:KillTimer(28);break;
		}
	}
	else if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK3)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK1)&&BST_UNCHECKED==IsDlgButtonChecked(IDC_CHECK2))
	{
		switch(m_ComboL.GetCurSel())
		{
		case 0:KillTimer(29);break;
		case 1:KillTimer(30);break;
		case 2:KillTimer(31);break;
		case 3:KillTimer(32);break;
		}
	}

}


void CRenderCenterDlg::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string tstring;
	CString tFileName;
	CFileDialog tDlg(FALSE,TEXT("jpg"),NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		TEXT("ͼ���ļ���ʽ(*.bmp)|*.bmp|JPG file format(*.jpg)|*.jpg|",NULL));
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=wstring2string(tFileName.GetBuffer(0));
	}
	if(!imgfusion.data)
	{
		MessageBox(TEXT("error"),TEXT("imgfusion is empty!"),MB_OK);
		return;
	}
	imwrite(tstring,imgfusion);
}

void CRenderCenterDlg::SetFusionSocket()
{
    m_Lock.Lock();
    if (m_FusionSocket != NULL)
    {
        AfxMessageBox(TEXT("��������"));
        m_Lock.Unlock();
        return;
    }
    m_FusionSocket = new RenderCenterSocket(this);
    if (m_RenderCenterSocket->Accept(*m_FusionSocket))
    {
        m_FusionSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
    m_Lock.Unlock();
}

void CRenderCenterDlg::ResetFusionSocket()
{
    m_Lock.Lock();
    if (m_FusionSocket != NULL)
    {
        m_FusionSocket->Close();
        delete m_FusionSocket;
    }
    m_Lock.Unlock();
}

void CRenderCenterDlg::RenderKeyTarget(TrueDataFrame &keyTarget)
{
    // TODO: Render key target image.
	g_Lock.Lock();
	CString path = KEY_TARGET_FILE_NAME;
	if (m_OutputPaths.size() > 0)
	{
		if (m_CurrentPath >= m_OutputPaths.size())
		{
			path = m_OutputPaths.back();
		}
		else
		{
			path = m_OutputPaths[m_CurrentPath];
			m_CurrentPath++;
		}
	}
    m_FusionSocket->SendKeyTarget(path);
	g_Lock.Unlock();
}
