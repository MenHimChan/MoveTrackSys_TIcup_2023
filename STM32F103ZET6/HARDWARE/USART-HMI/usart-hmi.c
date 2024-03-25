#include "usart-hmi.h"


Test_mode test_mode;
u8 Flag_Fast_Move;				// 快速移动


/*********************************************************
 @函数名称：uart3_init
 @para    : u32 bound 所需要的波特率
 @retval  : NULL
 @addion  : 使用的是PB10 TX PB11 RX
*********************************************************/
void uart3_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//时钟GPIOB、USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
}

/**************************************************************
 @funcName：HMISends
 @para    : char *buf1 字符串的名，字符数组的首地址
 @retval  : NULL
 @function: 通过串口三发送一个字符串给串口屏
 @addion  : 内层调用USART_SendData(USART3,buf1[i])的固件库函数
**************************************************************/
void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!= 0)
	 	{
			USART_SendData(USART3,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else return;
	}
}

/**************************************************************
 @funcName：HMI_Sendb3times
 @para    : char *buf1 字符串的名，字符数组的首地址
 @retval  : NULL
 @function: 通过串口三发送一个字符串给串口屏
 @addion  : 内层调用USART_SendData(USART3,buf1[i])的固件库函数
**************************************************************/
void HMI_Sendb3times(u8 k)		       //字节发送函数
{		 
	 u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART3,k);  //发送一个字节
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
		 return ;
	 } 
}


/**************************************************************
 @funcName：HMISendstart
 @para    : NULL
 @retval  : NULL
 @function: 防止上次要发送的数据卡在对本次发送造成影响
 @addion  : 调用在初始化函数中
**************************************************************/
void HMISendstart(void)
{
	 	Delay_ms(200);
		HMI_Sendb3times(0xff);
		Delay_ms(200);
}


// 接收来自串口屏的printh 打印的16进制数
void USART3_IRQHandler(void)   
{
	u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)   // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//  
			Res = USART_ReceiveData(USART3);				// 读取接收到的数据
			if(Res == 0x01)
			{

				test_mode.Flag_Point1 = 1;
			}
				
			
			else if(Res == 0x02)
				test_mode.Flag_Point2 = 1;
			
			else if(Res == 0x03)
			{
				test_mode.Flag_Point3 = 1;
				Flag_Fast_Move = 1;
			}
				
		}
}



void TestMode_Flag_Init(Test_mode *ptr)
{
	ptr -> Flag_Point1 = 0;
	ptr -> Flag_Point2 = 0;
	ptr -> Flag_Point3 = 0;
}





