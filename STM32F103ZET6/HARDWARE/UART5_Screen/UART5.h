#ifndef __UART5_H__
#define __UART5_H__


#include "stm32f10x.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#define UART5_REC_LEN  100 //100 byte buffer


#define __UART5_RX__ 0 //是否开启串口接收程序,因为对于串口屏来说，没有必要接收
#if __UART5_RX__
#define LENGTH_OF_UART5(s) (s & 0x3fff)
#define AVAILIBLE_UART5(s) (s & 0x8000)
extern u8 Uart5_Rx_Buf[UART5_REC_LEN];  
extern u16 Uart5_Rx_Sta;  
#endif

//注意串口屏初始的波特率为115200
void uart5_init(u32 bound);
void uart5_printf (char *fmt, ...);
void uart_screen_show(char * str);

#endif
