#include "sys.h"
#include "USART3_MV.h"

/*
USART3
*/
#if USART3_MV_EN   
u8  USART3_MV_RX_BUF[USART3_MV_REC_LEN];  
u16 USART3_MV_RX_STA=0;       	  


void USART3_printf (char *fmt, ...){ 
	char buffer[USART3_MV_REC_LEN+1];  
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_MV_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART3_MV_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
/*
函数：void USART3_Init(u32 BaudRate) 
参数：波特率
返回值：无
说明：复用作MV的初始化函数，函数名未改动
作者：Sheep
日期：22/09/12
*/
void USART3_Init(u32 BaudRate)
{ 
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure; 

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   
   USART_InitStructure.USART_BaudRate = BaudRate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

   USART_Init(USART3, &USART_InitStructure);
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
   USART_Cmd(USART3, ENABLE);

   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}


void USART3_IRQHandler(void)
{ 	
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{  
		Res =USART_ReceiveData(USART3);
		// USART3_printf("%c",Res); 		
		if((USART3_MV_RX_STA&0x8000)==0){			
			if(USART3_MV_RX_STA&0x4000){				
				if(Res!=0x0a)USART3_MV_RX_STA=0;
				else USART3_MV_RX_STA|=0x8000;	 
			}else{ 					
				if(Res==0x0d)USART3_MV_RX_STA|=0x4000;
				else{
					USART3_MV_RX_BUF[USART3_MV_RX_STA&0X3FFF]=Res ; 
					USART3_MV_RX_STA++;	
					if(USART3_MV_RX_STA>(USART3_MV_REC_LEN-1))USART3_MV_RX_STA=0;	  
				}		 
			}
		}   		 
	}
} 

/*
函数：u8 USART3_MV_Read(char* ret,u8 length) 
参数：ret 结果指针，用于读出数据，length 要读取的长度，使用sizeof(ret)
返回值：0/1 表示是否有读取到数据
说明：注意，ret才是真正的读取buff的输出
作者：Sheep
日期：22/09/12
*/
u8 USART3_MV_Read(char* ret,u8 length)
{
	u8 i =0;
	if(AVAILIBLE_UART(USART3_MV_RX_STA))
	{
		for (i=0;i<length;i++)
		{
			ret[i] = USART3_MV_RX_BUF[i];
		} 
		USART3_MV_RX_STA = 0;
		return 0; //表示读取成功
	}
	return 1; //表示没有读到数据
}

/*
函数：void Compare_Range(char* scan_range,char* MV_range,char* grasp_range,char num_or_ascii)
参数：char* scan_range, 扫码得到的123 ascii码序列
	  char* MV_range, openmv输出的123 ascii码序列
	  char* grasp_range, 要抓取的顺序序列
	  char num_or_ascii ，1_or_0,要抓取的顺序输出是ascii码序列 还是 下标顺序（0 1 2）
返回值：无
说明：注意ascii码和数字顺序输出的不同。
作者：Sheep
日期：22/09/12
*/
void Compare_Range(char* scan_range,char* MV_range,char* grasp_range,char num_or_ascii)
{
    char i = 0,j = 0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(scan_range[i] == MV_range[j])
            {
                if (num_or_ascii) //1
                {
                    grasp_range[i]=j+1;
                    break;
                }
                else  //0
                {
                    grasp_range[i]=j+1+'0';
                    break;
                }
            }
        }
    }
}


#endif	

/**************
 * 读取MV的数据，并比较
 * 	char scan_ret[4]={'1','2','3',0};
	char MV_ret[4]={0};
	char grasp_ret[4]={0};
if(!USART3_MV_Read(MV_ret,3))
{
    MV_ret[3]='\0';
    Compare_Range(scan_ret,MV_ret,grasp_ret,0);
    grasp_ret[3]='\0';
    printf("MV_recv = %s\r\n",MV_ret);
    printf("grasp_recv = %s\r\n",grasp_ret);
}
//打印时直接使用USART3_printf("\r\n");即可
//对应openmv使用
    // if uart3.any():
    //     data = uart3.readline()
    //     data[:-1]
    //     print(data)
onnpemv发送时，注意加上新行\r\n即可
    // a = str(132).encode('utf-8')  #示例123
    // a_p = struct.pack('<3sss',a,b'\r',b'\n')
    // print(f"tyepe={type(a_p)},a_p={a_p}")
    // uart3.write(a_p)
******************/

