#ifndef _GLOBALPARAMETER_H
#define _GLOBALPARAMETER_H

/* 基本参数设定 */
#define SAMPLE_PERIOD					1				//采样周期
#define TARGET_NUM						2				//目标个数
#define MEAS_DIM						3				//测量维数

#define CV_STATE_DIM					6				//降维后的CV滤波状态维数

#define STATE_DIM						9				//滤波状态维数
#define SGL_DIM                         3               //单维滤波的状态维数

#define CV_STATE_DIM					6				//CV模型的滤波状态维数
#define CV_SGL_DIM                      2               //CV模型单维滤波的状态维数
#define MODEL_NUM						3               //IMM滤波模型个数
#define CVMODEL							1				//CV模型
#define NOCVMODEL						0				//非CV模型
#define FILE_NUM                        3               //输入量测的文件个数
#define MAX_FILE_NAME_LENGTH			100				//文件名最大字符数
#define MAX_FILE_INDEX_BIT				32				//

//基本参数设定 均可调整

#define eps					2.2204e-16			//恒定极小量
#define PI					3.141592653589793	//常量PI

// #define T                   1                   //采样时间  */      
#define START_TIME			1					//起始时间帧数
#define END_TIME			1000				//结束时间帧数 
#define MAX_TIME            1000				//最大时间帧数 

// #define MODEL_NUM			3				//滤波选用的模型个数
#define FILTER_DIM			3					//滤波维数
#define ECHO_DIM			3					//量测维数

#define PNOISE_X  			0.0001				//X方向过程噪声
#define PNOISE_Y			0.001				//Y方向过程噪声
#define PNOISE_Z			0.1				    //Z方向过程噪声

#define SCANFORMAT          "%d\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t"	//读取量测数据文件的格式

#define MAX_ECHO			20					//最大回波数  可调整
#define MAX_TRACK			30					//最大目标数  可调整
#define Share_N				20					//航迹共享判断 总帧数
#define ShareConf_Num       19					//航迹共享判断 门限帧数

#define Total_Frame			2997

#define JiaoToHu			57.295779513082323				//角度转弧度系数
#define MAX_SCOPE			10000000				//飞机最远距离




#endif