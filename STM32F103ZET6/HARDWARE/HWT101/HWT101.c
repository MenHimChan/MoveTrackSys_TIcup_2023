#include "HWT101.h"
#include "string.h"
#include "usart.h"
#include "Delay.h"

//#include "LED.h"

char HWT_RESET[5]={0xFF,0xAA,0x00,0xFF,0x00}; //重启陀螺仪
char CALIYAW[5]={0xFF,0xAA,0x76,0x00,0x00};//z轴置零
struct SGyro 		stcGyro;//w[1]-wz原始值,w[2]-wz矫正后的加速度
struct SAngle 		stcAngle;//只有Angle[2]yaw,


void HWT101_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(HWT101_UART_RCC_PORT, ENABLE); 
#if (HWT101_UART_NUM==1)
	RCC_APB2PeriphClockCmd(HWT101_UART_RCC, ENABLE); 
#elif (HWT101_UART_NUM==2)
	RCC_APB1PeriphClockCmd(HWT101_UART_RCC, ENABLE); 
#endif

    GPIO_InitStructure.GPIO_Pin = HWT101_UART_TXPIN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(HWT101_UART_PORT, &GPIO_InitStructure);  //TX

    GPIO_InitStructure.GPIO_Pin = HWT101_UART_RXPIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HWT101_UART_PORT, &GPIO_InitStructure);   //RX

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(HWT101_UART, &USART_InitStructure); 
    USART_ITConfig(HWT101_UART, USART_IT_RXNE, ENABLE);
    USART_Cmd(HWT101_UART, ENABLE);                     

    NVIC_InitStructure.NVIC_IRQChannel = HWT101_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	 
}


/*
函数：HWT101_reset();
参数：无
返回值：无
说明：用于陀螺仪的重启
作者：Sheep
日期：22/09/11
*/
void HWT101_reset(void)
{
	HWT101_Send_Cmd(HWT_RESET,sizeof(HWT_RESET));
}
/*
函数：void HWT101_clear_yaw(void)
参数：无
返回值：无
说明：可将陀螺仪角度清零，暂时未测试，可以直接使用reset即可，效果一样
作者：Sheep
日期：22/09/11
*/
void HWT101_clear_yaw(void)
{
	HWT101_Send_Cmd(CALIYAW,sizeof(CALIYAW));
}

/*
function :void HWT101_Send_Cmd(char cmd[],char cmd_len)
docunment: HWT101_Send_Cmd设置函数
parament: cmd[] 命令数组
		  cmd_len 命令长度，建议使用sizeof()传入
*/
void HWT101_Send_Cmd(char cmd[],char cmd_len)
{
	char i=0;
	for(i=0;i<cmd_len;i++)
	{
		USART_SendData(HWT101_UART,cmd[i]);
		while(USART_GetFlagStatus(HWT101_UART, USART_FLAG_TC) == RESET);
	}
}

//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。
static void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	

	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			default: break;
		}
		ucRxCnt=0;//清空缓存区
	}
}

//串口中断函数
void HWT101_UART_IRQHanddler(void)
{
//	LED1_ON();
	if(USART_GetITStatus(HWT101_UART, USART_IT_RXNE) != RESET)
	{
		CopeSerial2Data((unsigned char)HWT101_UART->DR);//处理数据
		USART_ClearFlag(HWT101_UART,USART_IT_RXNE);
	}
}





/*读取角度和角速度的示例demo
yaw = YAW;
g_z_ori = W_Z_ORI;
g_z_cre = W_Z_CRE;
printf("yaw = %f\r\n",yaw);
printf("gz_ori = %f\r\n",g_z_ori);
printf("gz_cre = %f\r\n",g_z_cre);
*/

/*重启的demo
HWT101_reset();
//想要在谋个角度把陀螺仪置零，直接挑用该函数，陀螺仪就会以该角度作为0度
//重启后可加适当延时
*/

