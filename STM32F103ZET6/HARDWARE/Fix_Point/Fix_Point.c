#include "Fix_Point.h"
#include "delay.h"
#include "shell.h"
#include "usart.h"
#include "McWheel.h"

void FixPoint_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);	//使能复用功能时钟
    GPIO_InitTypeDef GPIO_InitStructure1;
//	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
//	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE, &GPIO_InitStructure1);

//	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
//	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure1);

	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_9;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);
 
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure1);
}
u8 Xl1,Xl2;//,X3,X4,X5,X6,X7;
u8 FixPoint_1(void)
{
	Xl1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	Xl2 = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8);
	if(Xl1==0&&Xl2==0)
	{
		return 0;//对准了
	}
	else if(Xl1==0&&Xl2==1)
	{
		return 4;//偏右了
	}
	else if(Xl1==1&&Xl2==0)
	{
		return 5;//偏左了
	}
	return 0;
//	X3 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2);
//	X4 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3);
//	X5 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4);
//	X6 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5);
//	X7 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12);
}






