#include "stm32f10x.h"
#include "UART5.h"


  
void uart5_init(u32 bound)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOC, &GPIO_InitStructure);  //TX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);   //RX


	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

#if __UART5_RX__
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	 
#endif
    USART_Init(UART5, &USART_InitStructure); 
    USART_Cmd(UART5, ENABLE);                     
}

#if __UART5_RX__
u8 Uart5_Rx_Buf[UART5_REC_LEN];  
u16 Uart5_Rx_Sta=0;  
void UART5_IRQHandler(void)
{ 	
	u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {  //0x0d 0x0a 设置以回车换行结尾
		Res =USART_ReceiveData(UART5);
		// printf("%c",Res); 		
		if((Uart5_Rx_Sta&0x8000)==0)
        {			
			if(Uart5_Rx_Sta&0x4000)
            {				
				if(Res!=0x0a)Uart5_Rx_Sta=0;
				else Uart5_Rx_Sta|=0x8000;	 
			}else{ 					
				if(Res==0x0d)Uart5_Rx_Sta|=0x4000;
				else{
					Uart5_Rx_Buf[Uart5_Rx_Sta&0X3FFF]=Res ; 
					Uart5_Rx_Sta++;	
					if(Uart5_Rx_Sta>(UART5_REC_LEN-1))Uart5_Rx_Sta=0;
				}		 
			}
		}   		 
	} 
} 
#endif

void uart5_printf (char *fmt, ...)
{ 
	char buffer[UART5_REC_LEN+1];  
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, UART5_REC_LEN+1, fmt, arg_ptr);
	while ((i < UART5_REC_LEN) && (i < strlen(buffer)))
    {
        USART_SendData(UART5, (u8) buffer[i++]);
        while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


/*
函数：void uart_screen_show(char * str)
参数：直接将扫描得到的字符串放进来就可以
返回值：无
说明：这个格式是串口屏固定的
作者：Sheep
日期：22/08/04
*/
void uart_screen_show(char * str)
{
    uart5_printf("t0.txt=\"%s\"\xff\xff\xff",str);
}
