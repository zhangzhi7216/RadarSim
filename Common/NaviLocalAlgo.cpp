#include "StdAfx.h"
#include "NaviLocalAlgo.h"
#include <assert.h>
#include <math.h>

NaviLocalAlgo::NaviLocalAlgo()
: m_Type(NaviLocalAlgoTypeTest1)
{
}

NaviLocalAlgo::NaviLocalAlgo(const CString &name, NaviLocalAlgoType type)
: NaviAlgo(name)
, m_Type(type)
{
}

bool NaviLocalAlgo::Init()
{
    switch (m_Type)
    {
    case NaviLocalAlgoTypeTest1:
        m_Func = &NaviLocalAlgoTest1;
        break;
    case NaviLocalAlgoTypeTest2:
        m_Func = &NaviLocalAlgoTest2;
        break;
    default:
        CString msg;
        msg.AppendFormat(TEXT("未知本地导航算法%d."), m_Type);
        AfxMessageBox(msg);
        return false;
    }
    return true;
}

void NaviLocalAlgo::Output(CArchive &ar)
{
    ar << NaviAlgoTypeLocal << m_Type;
    NaviAlgo::Output(ar);
}

void NaviLocalAlgo::Input(CArchive &ar)
{
    int type;
    ar >> type;
    m_Type = (NaviLocalAlgoType)type;
    NaviAlgo::Input(ar);
}

bool NaviLocalAlgoTest1(const NaviInput &input, NaviOutput &output)
{
    TrueDataFrame decomposition;//用真值数据格式分解绝对坐标系下的融合值
    unsigned int TargetCount;
    double distance,distancemin,rphi,rtheta,qphi,qtheta,yitatphi,yitattheta,yitapphi,yitaptheta,sigmatphi,sigmattheta,sigmapphi,sigmaptheta;//我机与敌机距离，我机与敌机视线角，我机与敌机速度与视线的夹角，我机与敌机绝对坐标系下速度夹角在俯仰与方位上的分量
    double rphideri,rthetaderi,qphideri,qthetaderi,sigmaphideri,sigmathetaderi;//距离，视线角，速度绝对坐标系下的夹角在俯仰与方位上的变化量
    double attackdistance;//攻击机距离阈值
    double Kphi,Ktheta;//比例导引常数
    double TargetChoice;
    double relativedistance,relativedistancemin;//相对距离，进行攻击的距离阈值
    Kphi = 6.0; Ktheta = 3.0;
    relativedistancemin = 1500;
    TargetCount = input.m_FusionDatas.size();
    if(input.m_FilterDatas[1].m_Vel.X==0&&input.m_FusionDatas[1].m_Vel.Y==0&&input.m_FusionDatas[1].m_Vel.Z==0)
    {
        distancemin = sqrt(pow((input.m_FusionDatas[0].m_Pos.X - input.m_Plane.m_Position.X),2)+pow((input.m_FusionDatas[0].m_Pos.Y - input.m_Plane.m_Position.Y),2)+pow((input.m_FusionDatas[0].m_Pos.Z - input.m_Plane.m_Position.Z),2));
        output.m_TrueData.m_Vel.X = input.m_Plane.m_Vel.X;
        output.m_TrueData.m_Vel.Y = input.m_Plane.m_Vel.Y;
        output.m_TrueData.m_Vel.Z = input.m_Plane.m_Vel.Z;
        output.m_TrueData.m_Pos.X = input.m_Plane.m_Position.X;
        output.m_TrueData.m_Pos.Y = input.m_Plane.m_Position.Y;
        output.m_TrueData.m_Pos.Z = input.m_Plane.m_Position.Z;
        output.m_TrueData.m_Acc.X = input.m_Plane.m_Acc.X;
        output.m_TrueData.m_Acc.Y = input.m_Plane.m_Acc.Y;
        output.m_TrueData.m_Acc.Z = input.m_Plane.m_Acc.Z;
        g_GlobalVar[0][0].m_G8 = sqrt(pow(input.m_Plane.m_Vel.X,2)+pow(input.m_Plane.m_Vel.Y,2)+pow(input.m_Plane.m_Vel.Z,2));//全局变量G8存储初始状态速度大小
        for(unsigned int i=1;i<=TargetCount-1;i++)
        {
            distance = sqrt(pow((input.m_FusionDatas[i].m_Pos.X - input.m_Plane.m_Position.X),2)+pow((input.m_FusionDatas[i].m_Pos.Y - input.m_Plane.m_Position.Y),2)+pow((input.m_FusionDatas[i].m_Pos.Z - input.m_Plane.m_Position.Z),2));
            if(distance <= distancemin)
            {
                distancemin = distance;
                TargetChoice = i;
                g_GlobalVar[0][0].m_G9 = TargetChoice;//通过第一个时间点确定攻击的敌机，之后可以直接选择敌机进行攻击。
            }
        }

    }
    else
    {
        relativedistance = sqrt(pow((input.m_Plane.m_Position.X-input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.X),2)+pow((input.m_Plane.m_Position.Y-input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.Y),2)+pow((input.m_Plane.m_Position.Z-input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.Z),2));
        if(relativedistance>relativedistancemin)
        {
            decomposition.m_Pos.X = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.X;
            decomposition.m_Pos.Y = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.Y;
            decomposition.m_Pos.Z = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Pos.Z;
            decomposition.m_Vel.X = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Vel.X;
            decomposition.m_Vel.Y = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Vel.Y;
            decomposition.m_Vel.Z = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Vel.Z;
            decomposition.m_Acc.X = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Acc.X;
            decomposition.m_Acc.Y = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Acc.Y;
            decomposition.m_Acc.Z = input.m_FusionDatas[g_GlobalVar[0][0].m_G9].m_Acc.Z;
            rphi = sqrt(pow((decomposition.m_Pos.Z - input.m_Plane.m_Position.Z),2));
            rtheta = sqrt(pow((decomposition.m_Pos.X - input.m_Plane.m_Position.X),2)+pow((decomposition.m_Pos.Y - input.m_Plane.m_Position.Y),2));
            qphi = atan((decomposition.m_Pos.Z-input.m_Plane.m_Position.Z)/sqrt(pow((decomposition.m_Pos.X-input.m_Plane.m_Position.X),2)+pow((decomposition.m_Pos.Y-input.m_Plane.m_Position.Y),2)));
            qtheta = atan((decomposition.m_Pos.Y-input.m_Plane.m_Position.Y)/(decomposition.m_Pos.X-input.m_Plane.m_Position.X));
            sigmatphi = atan(decomposition.m_Vel.Z/sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2)));
            sigmattheta = atan(decomposition.m_Vel.Y/decomposition.m_Vel.X);
            sigmapphi = atan(input.m_Plane.m_Vel.Z/sqrt(pow(input.m_Plane.m_Vel.X,2)+pow(input.m_Plane.m_Vel.Y,2)));
            sigmaptheta = atan(input.m_Plane.m_Vel.Y/input.m_Plane.m_Vel.X);
            yitatphi = qphi - sigmatphi;
            yitattheta = qtheta - sigmattheta;
            yitapphi = qphi - sigmapphi;
            yitaptheta = qtheta - sigmaptheta;
            rphideri = abs(decomposition.m_Vel.Z)*cos(yitatphi)-abs(input.m_Plane.m_Vel.Z)*cos(yitapphi);
            rthetaderi = sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*cos(yitattheta)-sqrt(pow(input.m_Plane.m_Vel.X,2)+pow(input.m_Plane.m_Vel.Y,2))*cos(yitaptheta);
            rphi += rphideri * input.m_GlobalData.m_Interval;
            rtheta += rthetaderi * input.m_GlobalData.m_Interval;
            //qphideri = (abs(input.m_Plane.m_Vel.Z)*sin(yitapphi)-abs(decomposition.m_Vel.Z)*sin(yitatphi))/rphi;
            qphideri = 0;
            qthetaderi = (sqrt(pow(input.m_Plane.m_Vel.X,2)+pow(input.m_Plane.m_Vel.Y,2))*sin(yitattheta)-sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*sin(yitattheta))/rtheta;
            sigmaphideri = Kphi*qphideri;
            sigmathetaderi = Ktheta*qthetaderi;
            sigmapphi += sigmaphideri * input.m_GlobalData.m_Interval;
            sigmaptheta += sigmathetaderi * input.m_GlobalData.m_Interval;
            output.m_TrueData.m_Acc.X = input.m_Plane.m_Acc.X;
            output.m_TrueData.m_Acc.Y = input.m_Plane.m_Acc.Y;
            output.m_TrueData.m_Acc.Z = input.m_Plane.m_Acc.Z;
            output.m_TrueData.m_Vel.X = g_GlobalVar[0][0].m_G8*cos(sigmapphi)*cos(sigmaptheta);//这里先按照匀速导引
            output.m_TrueData.m_Vel.Y = g_GlobalVar[0][0].m_G8*cos(sigmapphi)*sin(sigmaptheta);
            output.m_TrueData.m_Vel.Z = g_GlobalVar[0][0].m_G8*sin(sigmapphi);
            output.m_TrueData.m_Pos.X = input.m_Plane.m_Position.X+output.m_TrueData.m_Vel.X * input.m_GlobalData.m_Interval;
            output.m_TrueData.m_Pos.Y = input.m_Plane.m_Position.Y+output.m_TrueData.m_Vel.Y * input.m_GlobalData.m_Interval;
            output.m_TrueData.m_Pos.Z = input.m_Plane.m_Position.Z+output.m_TrueData.m_Vel.Z * input.m_GlobalData.m_Interval;    
        }
        else
        {
            output.m_TrueData.m_Acc.X = input.m_Plane.m_Acc.X;
            output.m_TrueData.m_Acc.Y = input.m_Plane.m_Acc.Y;
            output.m_TrueData.m_Acc.Z = input.m_Plane.m_Acc.Z;
            output.m_TrueData.m_Vel.X = input.m_Plane.m_Vel.X;
            output.m_TrueData.m_Vel.Y = input.m_Plane.m_Vel.Y;
            output.m_TrueData.m_Vel.Z = input.m_Plane.m_Vel.Z;
            output.m_TrueData.m_Pos.X = input.m_Plane.m_Position.X;
            output.m_TrueData.m_Pos.Y = input.m_Plane.m_Position.Y;
            output.m_TrueData.m_Pos.Z = input.m_Plane.m_Position.Z;    
        }
    }

    output.m_TrueData.m_Pos.X += 200;
    output.m_TrueData.m_Pos.Y += 200;
    output.m_TrueData.m_Pos.Z += 200;

    for (int i = 0; i < input.m_Missiles.size(); ++i)
    {
        Missile miss = input.m_Missiles[i];
        miss.m_Position.X += 2000 + i * 50;
        miss.m_Position.Y += 2000 + i * 100;
        miss.m_Position.Z += 2000 + i * 200;
        miss.m_Vel.X += 200 + i * 20;
        miss.m_Vel.Y += 200 + i * 20;
        miss.m_Vel.Z += 200 + i * 20;
        miss.m_Acc.X += 200 + i * 20;
        miss.m_Acc.Y += 200 + i * 20;
        miss.m_Acc.Z += 200 + i * 20;
        output.m_Missiles.push_back(miss);
    }

    return true;
}

bool NaviLocalAlgoTest2(const NaviInput &input, NaviOutput &output)
{
    // How to get interval.
    int interval = input.m_GlobalData.m_Interval;

    // How to use global var.
    g_GlobalVar[0][0].m_G1 = 0;
    g_GlobalVar[0][0].m_G2 = 0;

    output.m_TrueData.m_Pos = Position(40000, 10000, 0);
    return true;
}