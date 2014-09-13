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
	TrueDataFrame decomposition,missile;//用真值数据格式分解绝对坐标系下攻击机的融合值
	//TrueDataFrame Missile;//用真值数据格式分解绝对坐标系下导弹的融合值
	unsigned int TargetCount;
	double rphi,rtheta,qphi,qtheta,yitatphi,yitattheta,yitapphi,yitaptheta,sigmatphi,sigmattheta,sigmapphi,sigmaptheta;//我机与敌机距离，我机与敌机视线角，我机与敌机速度与视线的夹角，我机与敌机绝对坐标系下速度夹角在俯仰与方位上的分量
	double rphideri,rthetaderi,qphideri,qthetaderi,sigmaphideri,sigmathetaderi;//距离，视线角， 速度绝对坐标系下的夹角在俯仰与方位上的变化量
	double attackdistance,attackdistancemin,attackdistancelim;//攻击机距离阈值
	double Kphi,Ktheta;//比例导引常数
	double TargetChoice;
	double relativedistance,relativedistancemin;//相对距离，进行攻击的距离阈值
	double MissileVel;//导弹速度
	Kphi = 6.0; Ktheta = 2.0;
	relativedistancemin = 1500;
	attackdistancelim = 2*pow(10.0,5);
	MissileVel = 1000;
	TargetCount = input.m_FusionDatas.size();
		attackdistancemin = sqrt(pow((input.m_FusionDatas[0].m_Pos.X - input.m_PlaneTrueData.m_Pos.X),2)+pow((input.m_FusionDatas[0].m_Pos.Y - input.m_PlaneTrueData.m_Pos.Y),2)+pow((input.m_FusionDatas[0].m_Pos.Z - input.m_PlaneTrueData.m_Pos.Z),2));
		for (unsigned int i = 0 ; i<TargetCount ; i++)
		{
			attackdistance = sqrt(pow((input.m_PlaneTrueData.m_Pos.X - input.m_FusionDatas[i].m_Pos.X),2)+pow((input.m_PlaneTrueData.m_Pos.Y - input.m_FusionDatas[i].m_Pos.Y),2)+pow((input.m_PlaneTrueData.m_Pos.Z - input.m_FusionDatas[i].m_Pos.Z),2)); 
			if (attackdistance<attackdistancemin)
			{
				attackdistancemin = attackdistance;
			}
		}
		if (attackdistancemin>attackdistancelim)
		{
			decomposition.m_Pos.X = 0;
			decomposition.m_Pos.Y = 0;
			decomposition.m_Pos.Z = 0;
			decomposition.m_Vel.X = 0;
			decomposition.m_Vel.Y = 0;
			decomposition.m_Vel.Z = 0;
			decomposition.m_Acc.X = 0;
			decomposition.m_Acc.Y = 0;
			decomposition.m_Acc.Z = 0;
			for (unsigned int i=0;i<TargetCount;i++)
			{
				decomposition.m_Pos.X += input.m_FusionDatas[i].m_Pos.X / TargetCount;
				decomposition.m_Pos.Y += input.m_FusionDatas[i].m_Pos.Y / TargetCount;
				decomposition.m_Pos.Z += input.m_FusionDatas[i].m_Pos.Z / TargetCount;
				decomposition.m_Vel.X += input.m_FusionDatas[i].m_Vel.X / TargetCount;
				decomposition.m_Vel.Y += input.m_FusionDatas[i].m_Vel.Y / TargetCount;
				decomposition.m_Vel.Z += input.m_FusionDatas[i].m_Vel.Z / TargetCount;
				decomposition.m_Acc.X += input.m_FusionDatas[i].m_Acc.X / TargetCount;
				decomposition.m_Acc.Y += input.m_FusionDatas[i].m_Acc.Y / TargetCount;
				decomposition.m_Acc.Z += input.m_FusionDatas[i].m_Acc.Z / TargetCount;
			}
			relativedistance = sqrt(pow((input.m_PlaneTrueData.m_Pos.X-decomposition.m_Pos.X),2)+pow((input.m_PlaneTrueData.m_Pos.Y-decomposition.m_Pos.Y),2)+pow((input.m_PlaneTrueData.m_Pos.Z-decomposition.m_Pos.Z),2));
			rphi = sqrt(pow((decomposition.m_Pos.Z - input.m_PlaneTrueData.m_Pos.Z),2));
			rtheta = sqrt(pow((decomposition.m_Pos.X - input.m_PlaneTrueData.m_Pos.X),2)+pow((decomposition.m_Pos.Y - input.m_PlaneTrueData.m_Pos.Y),2));
			qphi = atan((decomposition.m_Pos.Z-input.m_PlaneTrueData.m_Pos.Z)/sqrt(pow((decomposition.m_Pos.X-input.m_PlaneTrueData.m_Pos.X),2)+pow((decomposition.m_Pos.Y-input.m_PlaneTrueData.m_Pos.Y),2)));
			qtheta = atan((decomposition.m_Pos.Y-input.m_PlaneTrueData.m_Pos.Y)/(decomposition.m_Pos.X-input.m_PlaneTrueData.m_Pos.X));
			sigmatphi = atan(decomposition.m_Vel.Z/sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2)));
			sigmattheta = atan(decomposition.m_Vel.Y/decomposition.m_Vel.X);
			sigmapphi = atan(input.m_PlaneTrueData.m_Vel.Z/sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)));
			sigmaptheta = atan(input.m_PlaneTrueData.m_Vel.Y/input.m_PlaneTrueData.m_Vel.X);
			yitatphi = qphi - sigmatphi;
			yitattheta = qtheta - sigmattheta;
			yitapphi = qphi - sigmapphi;
			yitaptheta = qtheta - sigmaptheta;
			rphideri = abs(decomposition.m_Vel.Z)*cos(yitatphi)-abs(input.m_PlaneTrueData.m_Vel.Z)*cos(yitapphi);
			rthetaderi = sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*cos(yitattheta)-sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2))*cos(yitaptheta);
			rphi += rphideri * input.m_Interval;
			rtheta += rthetaderi * input.m_Interval;
			qphideri = (abs(input.m_PlaneTrueData.m_Vel.Z)*sin(yitapphi)-abs(decomposition.m_Vel.Z)*sin(yitatphi))/rphi;
			/*qphideri = 0;*/
			qthetaderi = (sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2))*sin(yitattheta)-sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*sin(yitattheta))/rtheta;
			sigmaphideri = Kphi*qphideri;
			sigmathetaderi = Ktheta*qthetaderi;
			sigmapphi += sigmaphideri * input.m_Interval;
			sigmaptheta += sigmathetaderi * input.m_Interval;
			output.m_PlaneTrueData.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
			output.m_PlaneTrueData.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
			output.m_PlaneTrueData.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
			output.m_PlaneTrueData.m_Vel.X = 	sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)+pow(input.m_PlaneTrueData.m_Vel.Z,2))*cos(sigmapphi)*cos(sigmaptheta);//这里先按照匀速导引
			output.m_PlaneTrueData.m_Vel.Y = sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)+pow(input.m_PlaneTrueData.m_Vel.Z,2))*cos(sigmapphi)*sin(sigmaptheta);
			output.m_PlaneTrueData.m_Vel.Z = sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)+pow(input.m_PlaneTrueData.m_Vel.Z,2))*sin(sigmapphi);
			output.m_PlaneTrueData.m_Pos.X = input.m_PlaneTrueData.m_Pos.X+output.m_PlaneTrueData.m_Vel.X * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y+output.m_PlaneTrueData.m_Vel.Y * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z+output.m_PlaneTrueData.m_Vel.Z * input.m_Interval; 
		} 
		else
		{
			output.m_PlaneTrueData.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
			output.m_PlaneTrueData.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
			output.m_PlaneTrueData.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
			output.m_PlaneTrueData.m_Vel.X = input.m_PlaneTrueData.m_Vel.X;//这里先按照匀速导引
			output.m_PlaneTrueData.m_Vel.Y = input.m_PlaneTrueData.m_Vel.Y;
			output.m_PlaneTrueData.m_Vel.Z = input.m_PlaneTrueData.m_Vel.Z;
			output.m_PlaneTrueData.m_Pos.X = input.m_PlaneTrueData.m_Pos.X+output.m_PlaneTrueData.m_Vel.X * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y+output.m_PlaneTrueData.m_Vel.Y * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z+output.m_PlaneTrueData.m_Vel.Z * input.m_Interval; 
			for (unsigned int i=0;i<TargetCount;i++)
			{
				if (input.m_MissileTrueDatas[i].m_State == TargetStateAlive)
				{
					if (input.m_MissileTrueDatas[0].m_Pos.X == 0 && input.m_MissileTrueDatas[0].m_Pos.Y == 0 && input.m_MissileTrueDatas[0].m_Pos.Z == 0)
					{
						missile.m_Pos.X = input.m_PlaneTrueData.m_Pos.X;
						missile.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y;
						missile.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z;
						missile.m_Vel.X = input.m_PlaneTrueData.m_Vel.X;
						missile.m_Vel.Y = input.m_PlaneTrueData.m_Vel.Y;
						missile.m_Vel.Z = input.m_PlaneTrueData.m_Vel.Z;
						missile.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
						missile.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
						missile.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
					}
					else
					{
						missile.m_Pos.X = input.m_MissileTrueDatas[i].m_Pos.X;
						missile.m_Pos.Y = input.m_MissileTrueDatas[i].m_Pos.Y;
						missile.m_Pos.Z = input.m_MissileTrueDatas[i].m_Pos.Z;
						missile.m_Vel.X = input.m_MissileTrueDatas[i].m_Vel.X;
						missile.m_Vel.Y = input.m_MissileTrueDatas[i].m_Vel.Y;
						missile.m_Vel.Z = input.m_MissileTrueDatas[i].m_Vel.Z;
						missile.m_Acc.X = input.m_MissileTrueDatas[i].m_Acc.X;
						missile.m_Acc.Y = input.m_MissileTrueDatas[i].m_Acc.X;
						missile.m_Acc.Z = input.m_MissileTrueDatas[i].m_Acc.X;
					}
					decomposition.m_Pos.X = input.m_FusionDatas[i].m_Pos.X;
					decomposition.m_Pos.Y = input.m_FusionDatas[i].m_Pos.Y;
					decomposition.m_Pos.Z = input.m_FusionDatas[i].m_Pos.Z;
					decomposition.m_Vel.X = input.m_FusionDatas[i].m_Vel.X;
					decomposition.m_Vel.Y = input.m_FusionDatas[i].m_Vel.Y;
					decomposition.m_Vel.Z = input.m_FusionDatas[i].m_Vel.Z;
					decomposition.m_Acc.X = input.m_FusionDatas[i].m_Acc.X;
					decomposition.m_Acc.Y = input.m_FusionDatas[i].m_Acc.Y;
					decomposition.m_Acc.Z = input.m_FusionDatas[i].m_Acc.Z;

					relativedistance = sqrt(pow((missile.m_Pos.X-decomposition.m_Pos.X),2)+pow((missile.m_Pos.Y-decomposition.m_Pos.Y),2)+pow((missile.m_Pos.Z-decomposition.m_Pos.Z),2));
					rphi = sqrt(pow((decomposition.m_Pos.Z - missile.m_Pos.Z),2));
					rtheta = sqrt(pow((decomposition.m_Pos.X - missile.m_Pos.X),2)+pow((decomposition.m_Pos.Y - missile.m_Pos.Y),2));
					qphi = atan((decomposition.m_Pos.Z-missile.m_Pos.Z)/sqrt(pow((decomposition.m_Pos.X-missile.m_Pos.X),2)+pow((decomposition.m_Pos.Y-missile.m_Pos.Y),2)));
					qtheta = atan((decomposition.m_Pos.Y-missile.m_Pos.Y)/(decomposition.m_Pos.X-missile.m_Pos.X));
					sigmatphi = atan(decomposition.m_Vel.Z/sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2)));
					sigmattheta = atan(decomposition.m_Vel.Y/decomposition.m_Vel.X);
					sigmapphi = atan(missile.m_Vel.Z/sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2)));
					sigmaptheta = atan(missile.m_Vel.Y/missile.m_Vel.X);
					yitatphi = qphi - sigmatphi;
					yitattheta = qtheta - sigmattheta;
					yitapphi = qphi - sigmapphi;
					yitaptheta = qtheta - sigmaptheta;
					rphideri = abs(decomposition.m_Vel.Z)*cos(yitatphi)-abs(missile.m_Vel.Z)*cos(yitapphi);
					rthetaderi = sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*cos(yitattheta)-sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2))*cos(yitaptheta);
					rphi += rphideri * input.m_Interval;
					rtheta += rthetaderi * input.m_Interval;
					qphideri = (abs(missile.m_Vel.Z)*sin(yitapphi)-abs(decomposition.m_Vel.Z)*sin(yitatphi))/rphi;
					/*qphideri = 0;*/
					qthetaderi = (sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2))*sin(yitattheta)-sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*sin(yitattheta))/rtheta;
					sigmaphideri = Kphi*qphideri;
					sigmathetaderi = Ktheta*qthetaderi;
					sigmapphi += sigmaphideri * input.m_Interval;
					sigmaptheta += sigmathetaderi * input.m_Interval;
					output.m_MissileTrueDatas[i].m_Acc.X = input.m_MissileTrueDatas[i].m_Acc.X;
					output.m_MissileTrueDatas[i].m_Acc.Y = input.m_MissileTrueDatas[i].m_Acc.Y;
					output.m_MissileTrueDatas[i].m_Acc.Z = input.m_MissileTrueDatas[i].m_Acc.Z;
					output.m_MissileTrueDatas[i].m_Vel.X = MissileVel*cos(sigmapphi)*cos(sigmaptheta);//这里先按照匀速导引
					output.m_MissileTrueDatas[i].m_Vel.Y = MissileVel*cos(sigmapphi)*sin(sigmaptheta);
					output.m_MissileTrueDatas[i].m_Vel.Z = MissileVel*sin(sigmapphi);
					output.m_MissileTrueDatas[i].m_Pos = missile.m_Pos + output.m_MissileTrueDatas[i].m_Vel* input.m_Interval;
				}

			}	
		} 
	return true;
}

bool NaviLocalAlgoTest2(const NaviInput &input, NaviOutput &output)
{
	TrueDataFrame decomposition,missile;//用真值数据格式分解绝对坐标系下攻击机的融合值
	//TrueDataFrame Missile;//用真值数据格式分解绝对坐标系下导弹的融合值
	unsigned int TargetCount;
	double rphi,rtheta,qphi,qtheta,yitatphi,yitattheta,yitapphi,yitaptheta,sigmatphi,sigmattheta,sigmapphi,sigmaptheta;//我机与敌机距离，我机与敌机视线角，我机与敌机速度与视线的夹角，我机与敌机绝对坐标系下速度夹角在俯仰与方位上的分量
	double rphideri,rthetaderi,qphideri,qthetaderi,sigmaphideri,sigmathetaderi;//距离，视线角，速度绝对坐标系下的夹角在俯仰与方位上的变化量
	double attackdistance,attackdistancemin,attackdistancelim;//攻击机距离阈值
	double Kphi,Ktheta;//比例导引常数
	double TargetChoice;
	double relativedistance,relativedistancemin;//相对距离，进行攻击的距离阈值
	double MissileVel;//导弹速度
	Kphi = 6.0; Ktheta = 2.0;
	relativedistancemin = 1500;
	attackdistancelim = pow(10.0,5);
	MissileVel = 300;
	TargetCount = input.m_FusionDatas.size();
	if(input.m_FusionDatas[0].m_Vel.X==0&&input.m_FusionDatas[0].m_Vel.Y==0&&input.m_FusionDatas[0].m_Vel.Z==0)
	{
		output.m_PlaneTrueData.m_Vel.X = input.m_PlaneTrueData.m_Vel.X;
		output.m_PlaneTrueData.m_Vel.Y = input.m_PlaneTrueData.m_Vel.Y;
		output.m_PlaneTrueData.m_Vel.Z = input.m_PlaneTrueData.m_Vel.Z;
		output.m_PlaneTrueData.m_Pos.X = input.m_PlaneTrueData.m_Pos.X;
		output.m_PlaneTrueData.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y;
		output.m_PlaneTrueData.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z;
		output.m_PlaneTrueData.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
		output.m_PlaneTrueData.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
		output.m_PlaneTrueData.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
		g_GlobalVar[0].m_G[24] = sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)+pow(input.m_PlaneTrueData.m_Vel.Z,2));//全局变量存储初始状态速度大小
	}
	else
	{
		attackdistancemin = sqrt(pow((input.m_FusionDatas[0].m_Pos.X - input.m_PlaneTrueData.m_Pos.X),2)+pow((input.m_FusionDatas[0].m_Pos.Y - input.m_PlaneTrueData.m_Pos.Y),2)+pow((input.m_FusionDatas[0].m_Pos.Z - input.m_PlaneTrueData.m_Pos.Z),2));
		for (unsigned int i = 0 ; i<TargetCount ; i++)
		{
			attackdistance = sqrt(pow((input.m_PlaneTrueData.m_Pos.X - input.m_FusionDatas[i].m_Pos.X),2)+pow((input.m_PlaneTrueData.m_Pos.Y - input.m_FusionDatas[i].m_Pos.Y),2)+pow((input.m_PlaneTrueData.m_Pos.Z - input.m_FusionDatas[i].m_Pos.Z),2)); 
			if (attackdistance<attackdistancemin)
			{
				attackdistancemin = attackdistance;
			}
		}
		if (attackdistancemin>attackdistancelim)
		{
			decomposition.m_Pos.X = 0;
			decomposition.m_Pos.Y = 0;
			decomposition.m_Pos.Z = 0;
			decomposition.m_Vel.X = 0;
			decomposition.m_Vel.Y = 0;
			decomposition.m_Vel.Z = 0;
			decomposition.m_Acc.X = 0;
			decomposition.m_Acc.Y = 0;
			decomposition.m_Acc.Z = 0;
			for (unsigned int i=0;i<TargetCount;i++)
			{
				decomposition.m_Pos.X += input.m_FusionDatas[i].m_Pos.X / TargetCount;
				decomposition.m_Pos.Y += input.m_FusionDatas[i].m_Pos.Y / TargetCount;
				decomposition.m_Pos.Z += input.m_FusionDatas[i].m_Pos.Z / TargetCount;
				decomposition.m_Vel.X += input.m_FusionDatas[i].m_Vel.X / TargetCount;
				decomposition.m_Vel.Y += input.m_FusionDatas[i].m_Vel.Y / TargetCount;
				decomposition.m_Vel.Z += input.m_FusionDatas[i].m_Vel.Z / TargetCount;
				decomposition.m_Acc.X += input.m_FusionDatas[i].m_Acc.X / TargetCount;
				decomposition.m_Acc.Y += input.m_FusionDatas[i].m_Acc.Y / TargetCount;
				decomposition.m_Acc.Z += input.m_FusionDatas[i].m_Acc.Z / TargetCount;
			}
			relativedistance = sqrt(pow((input.m_PlaneTrueData.m_Pos.X-decomposition.m_Pos.X),2)+pow((input.m_PlaneTrueData.m_Pos.Y-decomposition.m_Pos.Y),2)+pow((input.m_PlaneTrueData.m_Pos.Z-decomposition.m_Pos.Z),2));
			rphi = sqrt(pow((decomposition.m_Pos.Z - input.m_PlaneTrueData.m_Pos.Z),2));
			rtheta = sqrt(pow((decomposition.m_Pos.X - input.m_PlaneTrueData.m_Pos.X),2)+pow((decomposition.m_Pos.Y - input.m_PlaneTrueData.m_Pos.Y),2));
			qphi = atan((decomposition.m_Pos.Z-input.m_PlaneTrueData.m_Pos.Z)/sqrt(pow((decomposition.m_Pos.X-input.m_PlaneTrueData.m_Pos.X),2)+pow((decomposition.m_Pos.Y-input.m_PlaneTrueData.m_Pos.Y),2)));
			qtheta = atan((decomposition.m_Pos.Y-input.m_PlaneTrueData.m_Pos.Y)/(decomposition.m_Pos.X-input.m_PlaneTrueData.m_Pos.X));
			sigmatphi = atan(decomposition.m_Vel.Z/sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2)));
			sigmattheta = atan(decomposition.m_Vel.Y/decomposition.m_Vel.X);
			sigmapphi = atan(input.m_PlaneTrueData.m_Vel.Z/sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2)));
			sigmaptheta = atan(input.m_PlaneTrueData.m_Vel.Y/input.m_PlaneTrueData.m_Vel.X);
			yitatphi = qphi - sigmatphi;
			yitattheta = qtheta - sigmattheta;
			yitapphi = qphi - sigmapphi;
			yitaptheta = qtheta - sigmaptheta;
			rphideri = abs(decomposition.m_Vel.Z)*cos(yitatphi)-abs(input.m_PlaneTrueData.m_Vel.Z)*cos(yitapphi);
			rthetaderi = sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*cos(yitattheta)-sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2))*cos(yitaptheta);
			rphi += rphideri * input.m_Interval;
			rtheta += rthetaderi * input.m_Interval;
			qphideri = (abs(input.m_PlaneTrueData.m_Vel.Z)*sin(yitapphi)-abs(decomposition.m_Vel.Z)*sin(yitatphi))/rphi;
			/*qphideri = 0;*/
			qthetaderi = (sqrt(pow(input.m_PlaneTrueData.m_Vel.X,2)+pow(input.m_PlaneTrueData.m_Vel.Y,2))*sin(yitattheta)-sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*sin(yitattheta))/rtheta;
			sigmaphideri = Kphi*qphideri;
			sigmathetaderi = Ktheta*qthetaderi;
			sigmapphi += sigmaphideri * input.m_Interval;
			sigmaptheta += sigmathetaderi * input.m_Interval;
			output.m_PlaneTrueData.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
			output.m_PlaneTrueData.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
			output.m_PlaneTrueData.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
			output.m_PlaneTrueData.m_Vel.X = g_GlobalVar[0].m_G[24]*cos(sigmapphi)*cos(sigmaptheta);//这里先按照匀速导引
			output.m_PlaneTrueData.m_Vel.Y = g_GlobalVar[0].m_G[24]*cos(sigmapphi)*sin(sigmaptheta);
			output.m_PlaneTrueData.m_Vel.Z = g_GlobalVar[0].m_G[24]*sin(sigmapphi);
			output.m_PlaneTrueData.m_Pos.X = input.m_PlaneTrueData.m_Pos.X+output.m_PlaneTrueData.m_Vel.X * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y+output.m_PlaneTrueData.m_Vel.Y * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z+output.m_PlaneTrueData.m_Vel.Z * input.m_Interval; 
		} 
		else
		{
			output.m_PlaneTrueData.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
			output.m_PlaneTrueData.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
			output.m_PlaneTrueData.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
			output.m_PlaneTrueData.m_Vel.X = input.m_PlaneTrueData.m_Vel.X;//这里先按照匀速导引
			output.m_PlaneTrueData.m_Vel.Y = input.m_PlaneTrueData.m_Vel.Y;
			output.m_PlaneTrueData.m_Vel.Z = input.m_PlaneTrueData.m_Vel.Z;
			output.m_PlaneTrueData.m_Pos.X = input.m_PlaneTrueData.m_Pos.X+output.m_PlaneTrueData.m_Vel.X * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y+output.m_PlaneTrueData.m_Vel.Y * input.m_Interval;
			output.m_PlaneTrueData.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z+output.m_PlaneTrueData.m_Vel.Z * input.m_Interval; 
			for (unsigned int i=0;i<TargetCount;i++)
			{
				if (input.m_MissileTrueDatas[i].m_State == TargetStateAlive)
				{
					if (input.m_MissileTrueDatas[0].m_Pos.X == 0 && input.m_MissileTrueDatas[0].m_Pos.Y == 0 && input.m_MissileTrueDatas[0].m_Pos.Z == 0)
					{
						missile.m_Pos.X = input.m_PlaneTrueData.m_Pos.X;
						missile.m_Pos.Y = input.m_PlaneTrueData.m_Pos.Y;
						missile.m_Pos.Z = input.m_PlaneTrueData.m_Pos.Z;
						missile.m_Vel.X = input.m_PlaneTrueData.m_Vel.X;
						missile.m_Vel.Y = input.m_PlaneTrueData.m_Vel.Y;
						missile.m_Vel.Z = input.m_PlaneTrueData.m_Vel.Z;
						missile.m_Acc.X = input.m_PlaneTrueData.m_Acc.X;
						missile.m_Acc.Y = input.m_PlaneTrueData.m_Acc.Y;
						missile.m_Acc.Z = input.m_PlaneTrueData.m_Acc.Z;
					}
					else
					{
						missile.m_Pos.X = input.m_MissileTrueDatas[i].m_Pos.X;
						missile.m_Pos.Y = input.m_MissileTrueDatas[i].m_Pos.Y;
						missile.m_Pos.Z = input.m_MissileTrueDatas[i].m_Pos.Z;
						missile.m_Vel.X = input.m_MissileTrueDatas[i].m_Vel.X;
						missile.m_Vel.Y = input.m_MissileTrueDatas[i].m_Vel.Y;
						missile.m_Vel.Z = input.m_MissileTrueDatas[i].m_Vel.Z;
						missile.m_Acc.X = input.m_MissileTrueDatas[i].m_Acc.X;
						missile.m_Acc.Y = input.m_MissileTrueDatas[i].m_Acc.X;
						missile.m_Acc.Z = input.m_MissileTrueDatas[i].m_Acc.X;
					}
					decomposition.m_Pos.X = input.m_FusionDatas[i].m_Pos.X;
					decomposition.m_Pos.Y = input.m_FusionDatas[i].m_Pos.Y;
					decomposition.m_Pos.Z = input.m_FusionDatas[i].m_Pos.Z;
					decomposition.m_Vel.X = input.m_FusionDatas[i].m_Vel.X;
					decomposition.m_Vel.Y = input.m_FusionDatas[i].m_Vel.Y;
					decomposition.m_Vel.Z = input.m_FusionDatas[i].m_Vel.Z;
					decomposition.m_Acc.X = input.m_FusionDatas[i].m_Acc.X;
					decomposition.m_Acc.Y = input.m_FusionDatas[i].m_Acc.Y;
					decomposition.m_Acc.Z = input.m_FusionDatas[i].m_Acc.Z;

					relativedistance = sqrt(pow((missile.m_Pos.X-decomposition.m_Pos.X),2)+pow((missile.m_Pos.Y-decomposition.m_Pos.Y),2)+pow((missile.m_Pos.Z-decomposition.m_Pos.Z),2));
					rphi = sqrt(pow((decomposition.m_Pos.Z - missile.m_Pos.Z),2));
					rtheta = sqrt(pow((decomposition.m_Pos.X - missile.m_Pos.X),2)+pow((decomposition.m_Pos.Y - missile.m_Pos.Y),2));
					qphi = atan((decomposition.m_Pos.Z-missile.m_Pos.Z)/sqrt(pow((decomposition.m_Pos.X-missile.m_Pos.X),2)+pow((decomposition.m_Pos.Y-missile.m_Pos.Y),2)));
					qtheta = atan((decomposition.m_Pos.Y-missile.m_Pos.Y)/(decomposition.m_Pos.X-missile.m_Pos.X));
					sigmatphi = atan(decomposition.m_Vel.Z/sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2)));
					sigmattheta = atan(decomposition.m_Vel.Y/decomposition.m_Vel.X);
					sigmapphi = atan(missile.m_Vel.Z/sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2)));
					sigmaptheta = atan(missile.m_Vel.Y/missile.m_Vel.X);
					yitatphi = qphi - sigmatphi;
					yitattheta = qtheta - sigmattheta;
					yitapphi = qphi - sigmapphi;
					yitaptheta = qtheta - sigmaptheta;
					rphideri = abs(decomposition.m_Vel.Z)*cos(yitatphi)-abs(missile.m_Vel.Z)*cos(yitapphi);
					rthetaderi = sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*cos(yitattheta)-sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2))*cos(yitaptheta);
					rphi += rphideri * input.m_Interval;
					rtheta += rthetaderi * input.m_Interval;
					qphideri = (abs(missile.m_Vel.Z)*sin(yitapphi)-abs(decomposition.m_Vel.Z)*sin(yitatphi))/rphi;
					/*qphideri = 0;*/
					qthetaderi = (sqrt(pow(missile.m_Vel.X,2)+pow(missile.m_Vel.Y,2))*sin(yitattheta)-sqrt(pow(decomposition.m_Vel.X,2)+pow(decomposition.m_Vel.Y,2))*sin(yitattheta))/rtheta;
					sigmaphideri = Kphi*qphideri;
					sigmathetaderi = Ktheta*qthetaderi;
					sigmapphi += sigmaphideri * input.m_Interval;
					sigmaptheta += sigmathetaderi * input.m_Interval;
					output.m_MissileTrueDatas[i].m_Acc.X = input.m_MissileTrueDatas[i].m_Acc.X;
					output.m_MissileTrueDatas[i].m_Acc.Y = input.m_MissileTrueDatas[i].m_Acc.Y;
					output.m_MissileTrueDatas[i].m_Acc.Z = input.m_MissileTrueDatas[i].m_Acc.Z;
					output.m_MissileTrueDatas[i].m_Vel.X = MissileVel*cos(sigmapphi)*cos(sigmaptheta);//这里先按照匀速导引
					output.m_MissileTrueDatas[i].m_Vel.Y = MissileVel*cos(sigmapphi)*sin(sigmaptheta);
					output.m_MissileTrueDatas[i].m_Vel.Z = MissileVel*sin(sigmapphi);
					output.m_MissileTrueDatas[i].m_Pos.X = input.m_MissileTrueDatas[i].m_Pos.X+output.m_MissileTrueDatas[i].m_Vel.X * input.m_Interval;
					output.m_MissileTrueDatas[i].m_Pos.Y = input.m_MissileTrueDatas[i].m_Pos.Y+output.m_MissileTrueDatas[i].m_Vel.Y * input.m_Interval;
					output.m_MissileTrueDatas[i].m_Pos.Z = input.m_MissileTrueDatas[i].m_Pos.Z+output.m_MissileTrueDatas[i].m_Vel.Z * input.m_Interval;
				}

			}	
		} 
	}
	return true;
}
