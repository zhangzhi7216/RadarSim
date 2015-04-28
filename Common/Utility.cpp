#include "stdafx.h"
#include "Utility.h"

#include <math.h>

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
			//均值为0，方差为1的标准正态分布
		}while(s >= 1 || s == 0);
		return value + v0 * var * var;                        //产生均值为a,方差为b的随机信号
        //return value + (double)rand() / (double)RAND_MAX * var;
	}
    double ColorNoise(double value, double var)
    {
        // 在这里加色噪声.
        return value + (double)rand() / (double)RAND_MAX * var;
    }
    double MultNoise(double value, double var)
    {
        // 在这里加乘性噪声.
        return value + (double)rand() / (double)RAND_MAX * var;
    }

    void CheckMissileHit(vector<Missile> &missiles, vector<TrueDataFrame> &targetTrueDatas)
    {
        for (int i = 0; i < missiles.size(); ++i)
        {
            if (missiles[i].m_State == TargetStateExploding)
            {
                missiles[i].m_State = TargetStateDestroyed;
            }
        }
        for (int i = 0; i < targetTrueDatas.size(); ++i)
        {
            if (targetTrueDatas[i].m_State == TargetStateExploding)
            {
                targetTrueDatas[i].m_State = TargetStateDestroyed;
            }
        }

        for (int i = 0; i < missiles.size(); ++i)
        {
            Missile &miss = missiles[i];
            if (miss.m_State != TargetStateAlive)
            {
                continue;
            }
            for (int j = 0; j < targetTrueDatas.size(); ++j)
            {
                TrueDataFrame &frame = targetTrueDatas[j];
                if (frame.m_State == TargetStateAlive && Utility::Distance(frame.m_Pos - miss.m_Position) <= MISSILE_HIT_THRESHOLD)
                {
                    miss.m_State = TargetStateExploding;
                    frame.m_State = TargetStateExploding;
                    break;
                }
            }
        }
    }

    void CheckMissileHit(vector<Missile> &missiles, vector<TrueDataFrame *> &targetTrueDatas)
    {
        for (int i = 0; i < missiles.size(); ++i)
        {
            if (missiles[i].m_State == TargetStateExploding)
            {
                missiles[i].m_State = TargetStateDestroyed;
            }
        }
        for (int i = 0; i < targetTrueDatas.size(); ++i)
        {
            if (targetTrueDatas[i]->m_State == TargetStateExploding)
            {
                targetTrueDatas[i]->m_State = TargetStateDestroyed;
            }
        }

        for (int i = 0; i < missiles.size(); ++i)
        {
            Missile &miss = missiles[i];
            if (miss.m_State != TargetStateAlive)
            {
                continue;
            }
            for (int j = 0; j < targetTrueDatas.size(); ++j)
            {
                TrueDataFrame &frame = *targetTrueDatas[j];
                if (frame.m_State == TargetStateAlive && Utility::Distance(frame.m_Pos - miss.m_Position) <= MISSILE_HIT_THRESHOLD)
                {
                    miss.m_State = TargetStateExploding;
                    frame.m_State = TargetStateExploding;
                    break;
                }
            }
        }
    }

    //将string转换成wstring
    wstring string2wstring(string str)
    {
        wstring result;
        //获取缓冲区大小，并申请空间，缓冲区大小按字符计算
        int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
        TCHAR* buffer = new TCHAR[len + 1];
        //多字节编码转换成宽字节编码
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
        buffer[len] = '\0';				//添加字符串结尾
        //删除缓冲区并返回值
        result.append(buffer);
        delete[] buffer;
        return result;
    }

    //将wstring转换成string
    string wstring2string(wstring wstr)
    {
        string result;
        //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
        int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
        char* buffer = new char[len + 1];
        //宽字节编码转换成多字节编码
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
        buffer[len] = '\0';
        //删除缓冲区并返回值
        result.append(buffer);
        delete[] buffer;
        return result;
    }
};