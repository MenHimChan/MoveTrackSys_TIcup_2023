#ifndef __HWT101_H__
#define __HWT101_H__

#include "stm32f10x.h"
#include "sys.h"
/*拼接宏*/
#define STRCAT2_(a, b)       a##b
#define STRCAT2(a, b)        STRCAT2_(a, b)
#define STRCAT3_(a, b, c)    a##b##c
#define STRCAT3(a, b, c)     STRCAT3_(a, b, c)
/*串口选择配置宏*/
#define HWT101_UART_NUM 2  //通过更改这个数字选择串口1或串口2,
//注意原子的usart.需要改动，当使用串口1时，printf默认被重定向到串口2

/*相关宏定义*/
#define HWT101_UART 			STRCAT2(USART, HWT101_UART_NUM)
#if (HWT101_UART_NUM==1)
#define HWT101_UART_PORT  		GPIOA 
#define HWT101_UART_TXPIN 		GPIO_Pin_9
#define HWT101_UART_RXPIN 		GPIO_Pin_10
#define HWT101_UART_IRQn 		USART1_IRQn
#define HWT101_UART_IRQHanddler USART1_IRQHandler
#define HWT101_UART_RCC_PORT 	RCC_APB2Periph_GPIOA
#define HWT101_UART_RCC		 	RCC_APB2Periph_USART1
#elif (HWT101_UART_NUM==2)
#define HWT101_UART_PORT  		GPIOA 
#define HWT101_UART_TXPIN 		GPIO_Pin_2
#define HWT101_UART_RXPIN 		GPIO_Pin_3
#define HWT101_UART_IRQn 		USART2_IRQn
#define HWT101_UART_IRQHanddler USART2_IRQHandler
#define HWT101_UART_RCC_PORT 	RCC_APB2Periph_GPIOA
#define HWT101_UART_RCC		 	RCC_APB1Periph_USART2
#endif
// #define HWT101_UART USART2
// #define HWT101_UART_IRQHanddler USART2_IRQHandler


/*外部调用的函数申明*/
void HWT101_init(u32 baudrate);
void HWT101_Send_Cmd(char cmd[],char cmd_len);
void HWT101_reset(void);
void HWT101_clear_yaw(void);
//外部调用的宏申明
#define CALCULATE_ACC(ACC) 		(ACC/32768*16)
#define CALCULATE_GYRO(GYRO) 	(GYRO/32768*2000)
#define CALCULATE_ANGLE(ANGLE) 	(ANGLE/32768*180)
/*外部使用的变量*/
#define YAW_TEMP 		CALCULATE_ANGLE((float)stcAngle.Angle[2])
#define YAW ((YAW_TEMP>120)&&(YAW_TEMP<180)?(YAW_TEMP-360):YAW_TEMP) 
#define W_Z_ORI		CALCULATE_GYRO((float)stcGyro.w[1])
#define W_Z_CRE		CALCULATE_GYRO((float)stcGyro.w[2])

//数据存储结构体
extern struct SGyro 	stcGyro;
extern struct SAngle 	stcAngle;
/*模块内部寄存器*/
#define SAVE 			0x00
#define CALSW 		0x01
#define RSW 			0x02
#define RRATE			0x03
#define BAUD 			0x04
#define AXOFFSET	0x05
#define AYOFFSET	0x06
#define AZOFFSET	0x07
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
#define IICADDR		0x1a
#define LEDOFF 		0x1b
#define GPSBAUD		0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44

#define q0          0x51
#define q1          0x52
#define q2          0x53
#define q3          0x54
      
#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5		

struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};

 
#endif
