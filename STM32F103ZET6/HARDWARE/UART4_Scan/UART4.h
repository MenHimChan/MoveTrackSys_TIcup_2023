#ifndef __UART4_H__
#define __UART4_H__


#include "stm32f10x.h"

#define UART4_REC_LEN  100 //200 byte buffer
#define LENGTH_OF_UART(s) (s & 0x3fff)
#define AVAILIBLE_UART(s) (s & 0x8000)



extern char Uart4_Rx_Buf[UART4_REC_LEN];  
extern u16 Uart4_Rx_Sta;  
void uart4_init(u32 bound);
void uart4_printf (char *fmt, ...);


#endif


/*
扫码模块接收，串口屏显示的使用例子
		if(AVAILIBLE_UART(Uart4_Rx_Sta))
		{
			length = LENGTH_OF_UART(Uart4_Rx_Sta);
			Uart4_Rx_Buf[length-1] = '\0';
            //去除草料二维码末尾的0A，没有的话，不用这一行
			uart_screen_show((char*)Uart4_Rx_Buf);
			Uart4_Rx_Sta = 0;
		}

*/

