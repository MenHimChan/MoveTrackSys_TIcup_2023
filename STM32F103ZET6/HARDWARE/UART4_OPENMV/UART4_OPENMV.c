/***********************************************************************************************************************
 @调试记录：
	2022-09-16 7:50：加入串口四初始化代码。经测试中断中的点灯程序能够正常执行，初步判断是原来的工程文件有问题  
	
	
	
	
***********************************************************************************************************************/
#include "UART4_OPENMV.h"
#include "Delay.h"

u8 receive_data[DATA_MAX_LEN];
u8 receive_num = 0;
u8 rece_ok = 0;
u8 Flag_Finish_Task = 0;

 void UART4_OPENMV_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //UART4 TX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出；
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口C；
		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //UART4 RX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入；
	GPIO_Init(GPIOC, &GPIO_InitStructure); //端口C；

	USART_InitStructure.USART_BaudRate = 9600; //波特率；
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位；
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位；
	USART_InitStructure.USART_Parity = USART_Parity_No; //无校验位；
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//收发模式；

	USART_Init(UART4, &USART_InitStructure);					//配置串口参数；
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; //中断号；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级；
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级；
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE); //使能串口；
}


void UART4_IRQHandler(void)
{
	u8 Res;
	char buf[50];
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);	//清空标志位
		Res = USART_ReceiveData(UART4);
		receive_data[receive_num++] = Res;
	}
	
	if(receive_num == DATA_MAX_LEN)
	{
		// 任务完成标志位
		if(receive_data[0] == 'f')
		{
			sprintf(buf,"page0.t0.txt=\"%s\"",receive_data);
			HMISends((char*)buf);
			HMI_Sendb3times(0xFF);
			receive_num = 0;
			rece_ok = 1;
			Flag_Finish_Task = 1;
		}
		
		// 坐标
		else
		{
			sprintf(buf,"page0.t0.txt=\"%s\"",receive_data);
			HMISends((char*)buf);
			HMI_Sendb3times(0xFF);
			receive_num = 0;
			rece_ok = 1;
		}

	}
	
}


void Send_OpenMV_Cmd(u8 mode)
{
	switch(mode)
	{
		case 1:
		{
			USART_SendData(UART4,'1');
			break;
		}
		
		case 2:
		{
			USART_SendData(UART4,'2');
			break;
		}
		
		case 3:
		{
			USART_SendData(UART4,'3');
			break;
		}
		
		default:
			break;
	}
}


// 串口解析
// 前四位x 后四位y
// 正负号 + 百位 + 十位 + 个位
int Decode_UART_xdata(void)
{
	int retval = 0;

	retval += (receive_data[1] - '0') * 100;
	retval += (receive_data[2] - '0') * 10;
	retval += (receive_data[3] - '0') * 1;
	
	if(receive_data[0] == '+')
		return retval;
	else 
	{
		retval *= -1;
		return retval;
	}
}


// 串口解析
// 前四位x 后四位y
// 正负号 + 百位 + 十位 + 个位
int Decode_UART_ydata(void)
{
	int retval = 0;

	retval += (receive_data[5] - '0') * 100;
	retval += (receive_data[6] - '0') * 10;
	retval += (receive_data[7] - '0') * 1;
	
	if(receive_data[4] == '+')
		return retval;
	else 
	{
		retval *= -1;
		return retval;
	}
}


