#include "stdafx.h"
#include "Sensor.h"
#include "Utility.h"
using namespace Utility;

CString Sensor::SensorTypeNames[] = {TEXT("有源传感器"), TEXT("无源传感器")};

Sensor::Sensor()
: m_Type(SensorTypeSource)
, m_Enable(TRUE)
, m_MaxDis(300)
, m_MaxTheta(120)
, m_MaxPhi(90)
, m_DisVar(0)
, m_ThetaVar(0)
, m_PhiVar(0)
, m_ProDet(100)
, m_ShowScanline(TRUE)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_ThetaRangeColor(Color::Green)
, m_ShowHeight(TRUE)
, m_GlobalData(GlobalDataPacket())
, m_Plane(Plane())
{
}

Sensor::Sensor(SensorType type, Plane &plane, GlobalDataPacket &globalData)
: m_Type(type)
, m_Enable(TRUE)
, m_MaxDis(300)
, m_MaxTheta(120)
, m_MaxPhi(360)
, m_DisVar(0)
, m_ThetaVar(0)
, m_PhiVar(0)
, m_ProDet(100)
, m_ShowScanline(TRUE)
, m_ShowTrack(TRUE)
, m_ShowThetaRange(TRUE)
, m_ThetaRangeColor(Color::Green)
, m_ShowHeight(TRUE)
, m_GlobalData(globalData)
, m_Plane(plane)
{
}

Sensor::~Sensor(void)
{
}

void Sensor::Reset()
{
    m_Enable = TRUE;
    m_MaxDis = 300;
    m_MaxTheta = 120;
    m_MaxPhi = 360;
    m_DisVar = 1;
    m_ThetaVar = 1;
    m_PhiVar = 1;
    m_ProDet = 0;
    m_ShowScanline = TRUE;
    m_ShowTrack = TRUE;
    m_TargetColors.clear();
    m_TargetDistances.clear();
    m_TargetThetas.clear();
    m_TargetPhis.clear();
}

void Sensor::AddTarget(Target &target)
{
    m_TargetColors.push_back(target.m_Color);
    m_TargetDistances.push_back(vector<double>());
    m_TargetThetas.push_back(vector<double>());
    m_TargetPhis.push_back(vector<double>());
}

void Sensor::AddTargetData(int target, Position rel)
{
    // 角度采与机头的相对夹角
    double d = Distance(rel), t = Theta(rel) - Theta(m_Plane.m_HeadDir), p = Phi(rel) - Phi(m_Plane.m_HeadDir);
    if (IsInRange(target, d, t, p))
    {
        double sample = (double)rand();
        if (sample <= (double)RAND_MAX * m_ProDet / 100.0)
        {
            if (m_GlobalData.m_Delay)
            {
                // 在这里加延迟.
            }
            if (m_GlobalData.m_WildValue)
            {
                // 在这里加野值.
            }
            switch (m_GlobalData.m_NoiseType)
            {
            case NoiseTypeWhite:
                m_TargetDistances[target].push_back(WhiteNoise(d, m_DisVar));
                m_TargetThetas[target].push_back(WhiteNoise(t, m_ThetaVar));
                m_TargetPhis[target].push_back(WhiteNoise(p, m_PhiVar));
                break;
            case NoiseTypeColor:
                m_TargetDistances[target].push_back(ColorNoise(d, m_DisVar));
                m_TargetThetas[target].push_back(ColorNoise(t, m_ThetaVar));
                m_TargetPhis[target].push_back(ColorNoise(p, m_PhiVar));
                break;
            case NoiseTypeMult:
                m_TargetDistances[target].push_back(MultNoise(d, m_DisVar));
                m_TargetThetas[target].push_back(MultNoise(t, m_ThetaVar));
                m_TargetPhis[target].push_back(MultNoise(p, m_PhiVar));
                break;
            default:
                m_TargetDistances[target].push_back(d);
                m_TargetThetas[target].push_back(t);
                m_TargetPhis[target].push_back(p);
                break;
            }
        }
    }
    else
    {
        m_TargetDistances[target].push_back(0);
        m_TargetThetas[target].push_back(0);
        m_TargetPhis[target].push_back(0);
    }
}

bool Sensor::IsInRange(int i, double d, double t, double p)
{
    if (i >= m_TargetDistances.size() || i >= m_TargetThetas.size() || i >= m_TargetPhis.size())
    {
        return false;
    }

    return d <= m_MaxDis && abs(t) <= m_MaxTheta / 2 && abs(p) <= m_MaxPhi / 2;
}

bool Sensor::IsShowTargetData(int i, int j)
{
    if (i >= m_TargetDistances.size() || i >= m_TargetThetas.size() || i >= m_TargetPhis.size()
        || j >= m_TargetDistances[i].size() || j >= m_TargetThetas[i].size() || j >= m_TargetPhis[i].size())
    {
        return false;
    }

    double d = m_TargetDistances[i][j], t = m_TargetThetas[i][j], p = m_TargetPhis[i][j];
    return !(d == 0 && t == 0 && p == 0);
}

CArchive & operator << (CArchive &ar, Sensor &sensor)
{
    int type = (int)sensor.m_Type;
    COLORREF color = sensor.m_ThetaRangeColor.ToCOLORREF();
    ar << type << sensor.m_Enable
        << sensor.m_MaxDis << sensor.m_MaxTheta << sensor.m_MaxPhi
        << sensor.m_DisVar << sensor.m_ThetaVar << sensor.m_PhiVar
        << sensor.m_ProDet
        << color
        << sensor.m_ShowThetaRange
        << sensor.m_ShowHeight;

    return ar;
}

CArchive & operator >> (CArchive &ar, Sensor &sensor)
{
    int type;
    COLORREF color;
    ar >> type >> sensor.m_Enable
        >> sensor.m_MaxDis >> sensor.m_MaxTheta >> sensor.m_MaxPhi
        >> sensor.m_DisVar >> sensor.m_ThetaVar >> sensor.m_PhiVar
        >> sensor.m_ProDet
        >> color
        >> sensor.m_ShowThetaRange
        >> sensor.m_ShowHeight;
    sensor.m_Type = (Sensor::SensorType)type;
    sensor.m_ThetaRangeColor.SetFromCOLORREF(color);

    return ar;
}
