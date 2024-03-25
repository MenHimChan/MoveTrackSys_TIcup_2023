#ifndef __USARTHMI_H
#define __USARTHMI_H		   
#include "sys.h"


typedef struct Test_mode   		// 用来PID参数计算变量
{
	u8 Flag_Point1;
	u8 Flag_Point2;
	u8 Flag_Point3;
} Test_mode;


extern Test_mode test_mode;
extern u8 Flag_Fast_Move;


void uart3_init(u32 bound);		// HMI串口3初始化
void HMISends(char *buf1); 		// 发送一个字符串
void HMI_Sendb3times(u8 k);		// 连续三次发送一个字节 用于做字符串发送完毕后的结束标志
void HMISendstart(void);		// 发送一个起始信号，在主函数里面调用。
void TestMode_Flag_Init(Test_mode *ptr);	// 标志位初始化

#endif






