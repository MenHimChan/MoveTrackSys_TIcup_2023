#ifndef __USART3_MV_H__
#define __USART3_MV_H__
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 



#define USART3_MV_REC_LEN  			200  	
#define USART3_MV_EN 			    1
#define LENGTH_OF_UART(s) (s & 0x3fff)
#define AVAILIBLE_UART(s) (s & 0x8000)		
extern u8  USART3_MV_RX_BUF[USART3_MV_REC_LEN];
extern u16 USART3_MV_RX_STA;         			


void USART3_Init(u32 bound);
void USART3_printf(char* fmt,...); 
void Compare_Range(char* scan_range,char* MV_range,char* grasp_range,char num_or_ascii);
u8 USART3_MV_Read(char* ret,u8 length);

#endif


