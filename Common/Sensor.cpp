#include "stdafx.h"
#include "Sensor.h"
#include "Target.h"
#include "Utility.h"
using namespace Utility;

Sensor::Sensor()
: m_Id(SensorIdRadar)
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
, m_Plane(NULL)
{
}

Sensor::Sensor(SensorId id, Plane &plane, GlobalDataPacket &globalData)
: m_Id(id)
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
, m_Plane(&plane)
{
}

Sensor::~Sensor(void)
{
}

Sensor &Sensor::operator =(const Sensor &s)
{
    if (this == &s)
    {
        return *this;
    }

    m_Id = s.m_Id;
    m_Enable = s.m_Enable;
    m_MaxDis = s.m_MaxDis;
    m_MaxTheta = s.m_MaxTheta;
    m_MaxPhi = s.m_MaxPhi;
    m_DisVar = s.m_DisVar;
    m_ThetaVar = s.m_ThetaVar;
    m_PhiVar = s.m_PhiVar;
    m_ProDet = s.m_ProDet;
    m_ShowScanline = s.m_ShowScanline;
    m_ShowTrack = s.m_ShowTrack;
    m_ShowThetaRange = s.m_ShowThetaRange;
    m_ThetaRangeColor = s.m_ThetaRangeColor;
    m_ShowHeight = s.m_ShowHeight;

    return *this;
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
    double d = Distance(rel), t = Theta(rel) - Theta(m_Plane->m_HeadDir), p = Phi(rel) - Phi(m_Plane->m_HeadDir);
    if (IsInRange(target, d, t, p))
    {
        double a = d, b = t, c = p;
        // AIS直接对直角坐标系加噪
        if (m_Id == SensorIdAis)
        {
            a = rel.X;
            b = rel.Y;
            c = rel.Z;
        }
        // double sample = (double)rand();
        // if (sample <= (double)RAND_MAX * m_ProDet / 100.0)
        if (m_GlobalData.m_Delay)
        {
            // 在这里加延迟.
        }
        if (m_GlobalData.m_WildValue)
        {
            // 在这里加野值.
        }
        double na = a, nb = b, nc = c;
        switch (m_GlobalData.m_NoiseType)
        {
        case NoiseTypeWhite:
            na = WhiteNoise(a, m_DisVar);
            nb = WhiteNoise(b, m_ThetaVar);
            nc = WhiteNoise(c, m_PhiVar);
            break;
        case NoiseTypeColor:
            na = ColorNoise(a, m_DisVar);
            nb = ColorNoise(b, m_ThetaVar);
            nc = ColorNoise(c, m_PhiVar);
            break;
        case NoiseTypeMult:
            na = MultNoise(a, m_DisVar);
            nb = MultNoise(b, m_ThetaVar);
            nc = MultNoise(c, m_PhiVar);
            break;
        }
        // AIS加噪后换算球面坐标
        if (m_Id == SensorIdAis)
        {
            Position p = Position(na, nb, nc);
            na = Distance(p);
            nb = Theta(p) - Theta(m_Plane->m_HeadDir);
            nc = Phi(p) - Phi(m_Plane->m_HeadDir);
        }
        m_TargetDistances[target].push_back(na);
        m_TargetThetas[target].push_back(nb);
        m_TargetPhis[target].push_back(nc);
    }
    else
    {
        m_TargetDistances[target].push_back(0);
        m_TargetThetas[target].push_back(0);
        m_TargetPhis[target].push_back(0);
    }
}

NoiseDataPacket Sensor::GetNoiseData(TrueDataPacket &packet)
{
    NoiseDataPacket np;
    np.m_SensorId = m_Id;
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        NoiseDataFrame frame;
        frame.m_Time = packet.m_TargetTrueDatas[i].m_Time;
        frame.m_Id = packet.m_TargetTrueDatas[i].m_Id;
        frame.m_Dis = m_TargetDistances[i].back();
        frame.m_Theta = m_TargetThetas[i].back();
        frame.m_Phi = m_TargetPhis[i].back();
        frame.m_DisVar = m_DisVar;
        frame.m_ThetaVar = m_ThetaVar;
        frame.m_PhiVar = m_PhiVar;
        np.m_TargetNoiseDatas.push_back(frame);
    }
    return np;
}

bool Sensor::IsInRange(int i, double d, double t, double p)
{
    if (i >= m_TargetDistances.size() || i >= m_TargetThetas.size() || i >= m_TargetPhis.size())
    {
        return false;
    }

    return d <= m_MaxDis && abs(t) <= m_MaxTheta / 2 && abs(p) <= m_MaxPhi / 2;
}

bool Sensor::IsInRange(int i, Position rel)
{
    if (i >= m_TargetDistances.size() || i >= m_TargetThetas.size() || i >= m_TargetPhis.size())
    {
        return false;
    }
    double d = Distance(rel), t = Theta(rel) - Theta(m_Plane->m_HeadDir), p = Phi(rel) - Phi(m_Plane->m_HeadDir);

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
    int id = (int)sensor.m_Id;
    COLORREF color = sensor.m_ThetaRangeColor.ToCOLORREF();
    ar << id << sensor.m_Enable
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
    int id;
    COLORREF color;
    ar >> id >> sensor.m_Enable
        >> sensor.m_MaxDis >> sensor.m_MaxTheta >> sensor.m_MaxPhi
        >> sensor.m_DisVar >> sensor.m_ThetaVar >> sensor.m_PhiVar
        >> sensor.m_ProDet
        >> color
        >> sensor.m_ShowThetaRange
        >> sensor.m_ShowHeight;
    sensor.m_Id = (SensorId)id;
    sensor.m_ThetaRangeColor.SetFromCOLORREF(color);

    return ar;
}
