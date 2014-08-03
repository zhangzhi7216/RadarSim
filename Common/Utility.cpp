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
        return atan(rel.Y / rel.X) * 57.2957795785523;
    }

    double Phi(const Position &rel)
    {
        return asin(rel.Z / Distance(rel)) * 57.2957795785523;
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
        double z = sin(phi / 57.2957795785523) * dis;
        double ydx = tan(theta / 57.2957795785523);
        double x = sqrt((pow(dis, 2) - pow(z, 2)) / (1 + pow(ydx, 2)));
        double y = x * ydx;
        return Position(x, y, z);
    }

    double WhiteNoise(double value, double var)
    {
        return value + (double)rand() / (double)RAND_MAX * var;
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
};