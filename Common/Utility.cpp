#include "stdafx.h"
#include "Utility.h"

#include <math.h>
#include <locale.h>
#include <strsafe.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

namespace Utility
{
    double Distance(const Position &rel)
    {
        return sqrt(pow(rel.X, 2) + pow(rel.Y, 2) + pow(rel.Z, 2));
    }

    double Theta(const Position &rel)
    {
        return atan(rel.Y / rel.X) * 57.2957795130823208768;
    }

    double Phi(const Position &rel)
    {
        return asin(rel.Z / Distance(rel)) * 57.2957795130823208768;
    }

    double Distance(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Distance(rel);
    }

    double Theta(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Theta(rel);
    }

    double Phi(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Phi(rel);
    }

    Position Rel(double dis, double theta, double phi)
    {
        double z = sin(phi / 57.2957795130823208768) * dis;
        double ydx = tan(theta / 57.2957795130823208768);
        double x = sqrt((pow(dis, 2) - pow(z, 2)) / (1 + pow(ydx, 2)));
        double y = x * ydx;
        return Position(x, y, z);
    }

    double WhiteNoise(double value, double var)
    {
		float v0,v1, v2, s;     
		float nextNextGaussian;    
		//int haveNextNextGaussian=0;    
		do  
		{    
			do     
			{      
				v1 = 2 * (((double)rand()/(double)RAND_MAX))-1;   // between -1.0 and 1.0
				v2 = 2 * (((double)rand()/(double)RAND_MAX))-1;   // between -1.0 and 1.0
				s = v1 * v1 + v2 * v2;
			} while (s >= 1 || s == 0);
			double multiplier = double(sqrt(-2*log(s)/s));
			nextNextGaussian = v2 * multiplier;
			//haveNextNextGaussian = 1;
			v0=v1 * multiplier;
			//��ֵΪ0������Ϊ1�ı�׼��̬�ֲ�
		}while(s >= 1 || s == 0);
		return value + v0 * var * var;                        //������ֵΪa,����Ϊb������ź�
        //return value + (double)rand() / (double)RAND_MAX * var;
	}
    double ColorNoise(double value, double var)
    {
        // �������ɫ����.
        return value + (double)rand() / (double)RAND_MAX * var;
    }
    double MultNoise(double value, double var)
    {
        // ������ӳ�������.
        return value + (double)rand() / (double)RAND_MAX * var;
    }

    //��stringת����wstring
    wstring string2wstring(string str)
    {
        wstring result;
        //��ȡ��������С��������ռ䣬��������С���ַ�����
        int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
        TCHAR* buffer = new TCHAR[len + 1];
        //���ֽڱ���ת���ɿ��ֽڱ���
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
        buffer[len] = '\0';				//����ַ�����β
        //ɾ��������������ֵ
        result.append(buffer);
        delete[] buffer;
        return result;
    }

    //��wstringת����string
    string wstring2string(wstring wstr)
    {
        string result;
        //��ȡ��������С��������ռ䣬��������С�°��ֽڼ����
        int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
        char* buffer = new char[len + 1];
        //���ֽڱ���ת���ɶ��ֽڱ���
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
        buffer[len] = '\0';
        //ɾ��������������ֵ
        result.append(buffer);
        delete[] buffer;
        return result;
    }

    void PromptLastErrorMessage()
    {
        LPVOID msg;
        LPVOID disp;
        DWORD dw = GetLastError(); 

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &msg,
            0, NULL );

        // Display the error message and exit the process

        disp = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
            (lstrlen((LPCTSTR)msg) + 100) * sizeof(TCHAR)); 
        StringCchPrintf((LPTSTR)disp, 
            LocalSize(disp) / sizeof(TCHAR),
            TEXT("Error code %d: %s"), 
            dw, msg); 
        MessageBox(NULL, (LPCTSTR)disp, TEXT("Error"), MB_OK); 

        LocalFree(msg);
        LocalFree(disp);
        // ExitProcess(dw); 
    }
};