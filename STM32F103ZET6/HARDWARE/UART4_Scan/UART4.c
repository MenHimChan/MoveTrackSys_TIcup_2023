#include "stm32f10x.h"
#include "UART4.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	

char Uart4_Rx_Buf[UART4_REC_LEN];  
u16 Uart4_Rx_Sta=0;    


void uart4_init(u32 bound)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOC, &GPIO_InitStructure);  //TX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);   //RX


	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(UART4, &USART_InitStructure); 
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);                     

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	 
}



void UART4_IRQHandler(void)
{ 	
	u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {  //0x0d 0x0a 设置以回车换行结尾
		Res =USART_ReceiveData(UART4);
		// printf("%c",Res); 		
		if((Uart4_Rx_Sta&0x8000)==0)
        {			
			if(Uart4_Rx_Sta&0x4000)
            {				
				if(Res!=0x0a)Uart4_Rx_Sta=0;
				else Uart4_Rx_Sta|=0x8000;	 
			}else{ 					
				if(Res==0x0d)Uart4_Rx_Sta|=0x4000;
				else{
					Uart4_Rx_Buf[Uart4_Rx_Sta&0X3FFF]=Res ; 
					Uart4_Rx_Sta++;	
					if(Uart4_Rx_Sta>(UART4_REC_LEN-1))Uart4_Rx_Sta=0;
				}		 
			}
		}   		 
	} 
} 

void uart4_printf (char *fmt, ...)
{ 
	char buffer[UART4_REC_LEN+1];  
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, UART4_REC_LEN+1, fmt, arg_ptr);
	while ((i < UART4_REC_LEN) && (i < strlen(buffer)))
    {
        USART_SendData(UART4, (u8) buffer[i++]);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}

