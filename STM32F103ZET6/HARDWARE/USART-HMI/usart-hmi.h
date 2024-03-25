#ifndef __USARTHMI_H
#define __USARTHMI_H		   
#include "sys.h"


typedef struct Test_mode   		// ����PID�����������
{
	u8 Flag_Point1;
	u8 Flag_Point2;
	u8 Flag_Point3;
} Test_mode;


extern Test_mode test_mode;
extern u8 Flag_Fast_Move;


void uart3_init(u32 bound);		// HMI����3��ʼ��
void HMISends(char *buf1); 		// ����һ���ַ���
void HMI_Sendb3times(u8 k);		// �������η���һ���ֽ� �������ַ���������Ϻ�Ľ�����־
void HMISendstart(void);		// ����һ����ʼ�źţ���������������á�
void TestMode_Flag_Init(Test_mode *ptr);	// ��־λ��ʼ��

#endif






