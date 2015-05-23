#ifndef _GLOBALPARAMETER_H
#define _GLOBALPARAMETER_H

/* ���������趨 */
#define SAMPLE_PERIOD					1				//��������
#define TARGET_NUM						2				//Ŀ�����
#define MEAS_DIM						3				//����ά��

#define CV_STATE_DIM					6				//��ά���CV�˲�״̬ά��

#define STATE_DIM						9				//�˲�״̬ά��
#define SGL_DIM                         3               //��ά�˲���״̬ά��

#define CV_STATE_DIM					6				//CVģ�͵��˲�״̬ά��
#define CV_SGL_DIM                      2               //CVģ�͵�ά�˲���״̬ά��
#define MODEL_NUM						3               //IMM�˲�ģ�͸���
#define CVMODEL							1				//CVģ��
#define NOCVMODEL						0				//��CVģ��
#define FILE_NUM                        3               //����������ļ�����
#define MAX_FILE_NAME_LENGTH			100				//�ļ�������ַ���
#define MAX_FILE_INDEX_BIT				32				//

//���������趨 ���ɵ���

#define eps					2.2204e-16			//�㶨��С��
#define PI					3.141592653589793	//����PI

// #define T                   1                   //����ʱ��  */      
#define START_TIME			1					//��ʼʱ��֡��
#define END_TIME			1000				//����ʱ��֡�� 
#define MAX_TIME            1000				//���ʱ��֡�� 

// #define MODEL_NUM			3				//�˲�ѡ�õ�ģ�͸���
#define FILTER_DIM			3					//�˲�ά��
#define ECHO_DIM			3					//����ά��

#define PNOISE_X  			0.0001				//X�����������
#define PNOISE_Y			0.001				//Y�����������
#define PNOISE_Z			0.1				    //Z�����������

#define SCANFORMAT          "%d\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t"	//��ȡ���������ļ��ĸ�ʽ

#define MAX_ECHO			20					//���ز���  �ɵ���
#define MAX_TRACK			30					//���Ŀ����  �ɵ���
#define Share_N				20					//���������ж� ��֡��
#define ShareConf_Num       19					//���������ж� ����֡��

#define Total_Frame			2997

#define JiaoToHu			57.295779513082323				//�Ƕ�ת����ϵ��
#define MAX_SCOPE			10000000				//�ɻ���Զ����




#endif