#ifndef _STRUCTDEFINITION_H
#define _STRUCTDEFINITION_H

/*
***************************************
*            ͷ�ļ�����               *
***************************************
*/
#include "globalParameter.h"

/*
***************************************
*            �ṹ������               *
***************************************
*/

/* �˲�ģ�Ͳ��� */
typedef struct modelpara{
	int isCV;								//�Ƿ�ΪCVģ�ͱ�־
	double fk[STATE_DIM][STATE_DIM];		//״̬ת�ƾ���
	double qk[STATE_DIM][STATE_DIM];		//���������ֲ�����
	double pk[STATE_DIM][STATE_DIM];		//���Э�������
	double hk[MEAS_DIM][STATE_DIM];		    //�������
}MODELPARA;

/* ֡���ݸ�ʽ */
typedef struct measinfo{
	int sidx;								//��������
	int tidx;								//������
	double meas[MEAS_DIM];				    //����ֵ
	double nstd[MEAS_DIM];					//������׼��
	struct measinfo *next;					//��һ������
}MEASINFO;

/* �˲�״̬��Ϣ */
typedef struct fltinfo{
	double xk[STATE_DIM];					//״̬��Ϣ
	double pk[STATE_DIM][STATE_DIM];		//���Э������
}FLTINFO;

/* ��ά���CVģ���˲�״̬��Ϣ */
typedef struct cvfltinfo{
	double xk[CV_STATE_DIM];				//CVģ�͵�״̬��Ϣ
	double pk[CV_STATE_DIM][CV_STATE_DIM];  //CVģ�͵����Э������
}CVFLTINFO;

/* Ŀ��ĺ���״̬��Ϣ */
typedef struct trackinfo{
	int		idx;							    //������
	int     Conf_flag;						    // ȷ�Ϻ�����־λ
	int		ConfNum;							// ȷ�Ϻ�������ʱ �Ѿ����벨�ŵ�֡��
	int		ConfStart;							// ȷ�Ϻ�������ʱ ��ʼʱ�ĵڼ�֡

	int     Del_flag;							// ɾ��������־λ
	int     Del_Init;							// ɾ��������ʼ֡��
	int     DelNum;								// ɾ������ʱ �Ѿ�����û���������벨�ŵ�֡��
	int     DelStart;							// ɾ��������ʼ֡��

	int     Points;								// �Ѹ��µ���

	int		Theat;								// ��в��

	int     Type;								// Ŀ������
	int		Civi_Num;							// �����жϸú���Ŀ���Ƿ�Ϊ�񴬵ļ�����
	double fMdlProb[MODEL_NUM];					 //����ģ�͸���

	FLTINFO fsn;								//�ں������˲���Ϣ
	FLTINFO imm[MODEL_NUM];						//Ŀ��Ľ���ʽ��ģ���˲���Ϣ
}TRACKINFO;

/* ������������ */
typedef struct rkns{
	double Rk[MEAS_DIM][MEAS_DIM];		    //������������
}RKNS;

/* ����ѹ�����̲�����Ϣ */
typedef struct compinfo{
	double Rk[MEAS_DIM][MEAS_DIM];		    //ѹ�����������������
	double Hk[MEAS_DIM][STATE_DIM];		    //ѹ������������
	double Zk[MEAS_DIM];				    //ѹ�����������Ϣ
}COMPINFO;
	
/* �ֿ����ֵ���������Ϣ */
typedef struct blockmatrixinfo{
	int iRowBeg;							//��ʼ�к�
	int iRowFin;							//�����к�+1
	int iColBeg;							//��ʼ�к�
	int iColFin;							//�����к�+1

	int iColDim;							//ԭ�����е�ά��
}BMINFO;


//�������ݽṹ
typedef struct echo{
	double msr[ECHO_DIM];						//����ֵ
	double Fusion_OrNot;                        //�������Ƿ���Ҫ�����ں��㷨  0-����Ҫ 1-��Ҫ
	int		Type;
}ECHO;

//����������ݽṹ
typedef struct echo_parameter{

	double Radar_RgeScope;								   //�ҷ��ɻ��ľ���̽�ⷶΧ
	double Radar_Azi_MaxScope;			                   //�ҷ��ɻ��ķ�λ��̽�ⷶΧ���ֵ
	double Radar_Azi_MinScope;							   //�ҷ��ɻ��ķ�λ��̽�ⷶΧ��Сֵ
	double Radar_Ele_MaxScope;			                   //�ҷ��ɻ��ĸ�����̽�ⷶΧ���ֵ
	double Radar_Ele_MinScope;							   //�ҷ��ɻ��ĸ�����̽�ⷶΧ��Сֵ

	double AIS_RgeScope;								//�ҷ��ɻ���AIS����̽�ⷶΧ
	double AIS_Azi_MaxScope;							//�ҷ��ɻ���AIS��λ��̽�ⷶΧ���ֵ
	double AIS_Azi_MinScope;							//�ҷ��ɻ���AIS��λ��̽�ⷶΧ��Сֵ
	double AIS_Ele_MaxScope;							//�ҷ��ɻ���AIS������̽�ⷶΧ���ֵ
	double AIS_Ele_MinScope;							//�ҷ��ɻ���AIS������̽�ⷶΧ��Сֵ

	double Tong_RgeScope;								//�ҷ��ɻ���ͨ��������̽�ⷶΧ
	double Tong_Azi_MaxScope;							//�ҷ��ɻ���ͨ����鷽λ��̽�ⷶΧ���ֵ
	double Tong_Azi_MinScope;							//�ҷ��ɻ���ͨ����鷽λ��̽�ⷶΧ��Сֵ
	double Tong_Ele_MaxScope;							//�ҷ��ɻ���ͨ����鸩����̽�ⷶΧ���ֵ
	double Tong_Ele_MinScope;							//�ҷ��ɻ���ͨ����鸩����̽�ⷶΧ��Сֵ

	double Lei_RgeScope;								//�ҷ��ɻ����״�������̽�ⷶΧ
	double Lei_Azi_MaxScope;							//�ҷ��ɻ����״���鷽λ��̽�ⷶΧ���ֵ
	double Lei_Azi_MinScope;							//�ҷ��ɻ����״���鷽λ��̽�ⷶΧ��Сֵ
	double Lei_Ele_MaxScope;							//�ҷ��ɻ����״���鸩����̽�ⷶΧ���ֵ
	double Lei_Ele_MinScope;							//�ҷ��ɻ����״���鸩����̽�ⷶΧ��Сֵ

	double Radar_rgeMN;								//�״�ľ����������
	double Radar_aziMN;								 //�״�ķ�λ�ǲ�������
	double Radar_eleMN;								//�״�ĸ����ǲ�������


	double AIS_rgeMN;							   //AIS�ľ����������
	double AIS_aziMN;			                   //AIS�ķ�λ�ǲ�������
	double AIS_eleMN;			                   //AIS�ĸ����ǲ�������

	double Tong_aziMN;							   //ͨ�����ķ�λ�ǲ�������
	double Tong_eleMN;							   //ͨ�����ĸ����ǲ�������

	double Lei_aziMN;							   //�״����ķ�λ�ǲ������� 
	double Lei_eleMN;			                   //�״����ĸ����ǲ������� 

	double NOISEandOUTLIER_TYPE;					//��һλ�������� 1-������ 2-ɫ���� �ڶ�λ�Ƿ���Ұֵ 1-�� 2-��

}ECHO_PARAMETER;

typedef struct trkinfo{
	double X[3][MAX_TIME];
}TRKINFO;

/* �ӿں������ں������� */
typedef struct evaluationresult{
	double tavg[3];				
	double tstd[3];
	double rms[3][MAX_TIME];
	double davg[3];
}RESULTINFO;

#endif