#ifndef _STRUCTDEFINITION_H
#define _STRUCTDEFINITION_H

/*
***************************************
*            头文件引用               *
***************************************
*/
#include "globalParameter.h"

/*
***************************************
*            结构体声明               *
***************************************
*/

/* 滤波模型参数 */
typedef struct modelpara{
	int isCV;								//是否为CV模型标志
	double fk[STATE_DIM][STATE_DIM];		//状态转移矩阵
	double qk[STATE_DIM][STATE_DIM];		//过程噪声分布矩阵
	double pk[STATE_DIM][STATE_DIM];		//误差协方差矩阵
	double hk[MEAS_DIM][STATE_DIM];		    //量测矩阵
}MODELPARA;

/* 帧数据格式 */
typedef struct measinfo{
	int sidx;								//传感器号
	int tidx;								//航迹号
	double meas[MEAS_DIM];				    //量测值
	double nstd[MEAS_DIM];					//噪声标准差
	struct measinfo *next;					//下一个量测
}MEASINFO;

/* 滤波状态信息 */
typedef struct fltinfo{
	double xk[STATE_DIM];					//状态信息
	double pk[STATE_DIM][STATE_DIM];		//误差协方差阵
}FLTINFO;

/* 降维后的CV模型滤波状态信息 */
typedef struct cvfltinfo{
	double xk[CV_STATE_DIM];				//CV模型的状态信息
	double pk[CV_STATE_DIM][CV_STATE_DIM];  //CV模型的误差协方差阵
}CVFLTINFO;

/* 目标的航迹状态信息 */
typedef struct trackinfo{
	int		idx;							    //航迹号
	int     Conf_flag;						    // 确认航迹标志位
	int		ConfNum;							// 确认航迹滑窗时 已经落入波门的帧数
	int		ConfStart;							// 确认航迹滑窗时 开始时的第几帧

	int     Del_flag;							// 删除航迹标志位
	int     Del_Init;							// 删除航迹开始帧数
	int     DelNum;								// 删除航迹时 已经连续没有量测落入波门的帧数
	int     DelStart;							// 删除航迹开始帧数

	int     Points;								// 已更新点数

	int		Theat;								// 威胁度

	int     Type;								// 目标类型
	int		Civi_Num;							// 用于判断该海上目标是否为民船的计数器
	double fMdlProb[MODEL_NUM];					 //各个模型概率

	FLTINFO fsn;								//融合中心滤波信息
	FLTINFO imm[MODEL_NUM];						//目标的交互式多模型滤波信息
}TRACKINFO;

/* 量测噪声矩阵 */
typedef struct rkns{
	double Rk[MEAS_DIM][MEAS_DIM];		    //量测噪声矩阵
}RKNS;

/* 数据压缩方程参数信息 */
typedef struct compinfo{
	double Rk[MEAS_DIM][MEAS_DIM];		    //压缩后的量测噪声矩阵
	double Hk[MEAS_DIM][STATE_DIM];		    //压缩后的量测矩阵
	double Zk[MEAS_DIM];				    //压缩后的量测信息
}COMPINFO;
	
/* 分块矩阵赋值所需参数信息 */
typedef struct blockmatrixinfo{
	int iRowBeg;							//初始行号
	int iRowFin;							//结束行号+1
	int iColBeg;							//初始列号
	int iColFin;							//结束列号+1

	int iColDim;							//原矩阵列的维数
}BMINFO;


//量测数据结构
typedef struct echo{
	double msr[ECHO_DIM];						//量测值
	double Fusion_OrNot;                        //该量测是否需要进入融合算法  0-不需要 1-需要
	int		Type;
}ECHO;

//量测参数数据结构
typedef struct echo_parameter{

	double Radar_RgeScope;								   //我方飞机的距离探测范围
	double Radar_Azi_MaxScope;			                   //我方飞机的方位角探测范围最大值
	double Radar_Azi_MinScope;							   //我方飞机的方位角探测范围最小值
	double Radar_Ele_MaxScope;			                   //我方飞机的俯仰角探测范围最大值
	double Radar_Ele_MinScope;							   //我方飞机的俯仰角探测范围最小值

	double AIS_RgeScope;								//我方飞机的AIS距离探测范围
	double AIS_Azi_MaxScope;							//我方飞机的AIS方位角探测范围最大值
	double AIS_Azi_MinScope;							//我方飞机的AIS方位角探测范围最小值
	double AIS_Ele_MaxScope;							//我方飞机的AIS俯仰角探测范围最大值
	double AIS_Ele_MinScope;							//我方飞机的AIS俯仰角探测范围最小值

	double Tong_RgeScope;								//我方飞机的通信侦查距离探测范围
	double Tong_Azi_MaxScope;							//我方飞机的通信侦查方位角探测范围最大值
	double Tong_Azi_MinScope;							//我方飞机的通信侦查方位角探测范围最小值
	double Tong_Ele_MaxScope;							//我方飞机的通信侦查俯仰角探测范围最大值
	double Tong_Ele_MinScope;							//我方飞机的通信侦查俯仰角探测范围最小值

	double Lei_RgeScope;								//我方飞机的雷达侦查距离探测范围
	double Lei_Azi_MaxScope;							//我方飞机的雷达侦查方位角探测范围最大值
	double Lei_Azi_MinScope;							//我方飞机的雷达侦查方位角探测范围最小值
	double Lei_Ele_MaxScope;							//我方飞机的雷达侦查俯仰角探测范围最大值
	double Lei_Ele_MinScope;							//我方飞机的雷达侦查俯仰角探测范围最小值

	double Radar_rgeMN;								//雷达的距离测量噪声
	double Radar_aziMN;								 //雷达的方位角测量噪声
	double Radar_eleMN;								//雷达的俯仰角测量噪声


	double AIS_rgeMN;							   //AIS的距离测量噪声
	double AIS_aziMN;			                   //AIS的方位角测量噪声
	double AIS_eleMN;			                   //AIS的俯仰角测量噪声

	double Tong_aziMN;							   //通信侦察的方位角测量噪声
	double Tong_eleMN;							   //通信侦察的俯仰角测量噪声

	double Lei_aziMN;							   //雷达侦察的方位角测量噪声 
	double Lei_eleMN;			                   //雷达侦察的俯仰角测量噪声 

	double NOISEandOUTLIER_TYPE;					//第一位噪声类型 1-白噪声 2-色噪声 第二位是否有野值 1-有 2-无

}ECHO_PARAMETER;

typedef struct trkinfo{
	double X[3][MAX_TIME];
}TRKINFO;

/* 接口函数的融合输出结果 */
typedef struct evaluationresult{
	double tavg[3];				
	double tstd[3];
	double rms[3][MAX_TIME];
	double davg[3];
}RESULTINFO;

#endif