#include "KEY.h"
#include "delay.h"
#include "usart.h"



void Key_init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
    GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure1);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel= EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Key_PC13_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void EXTI15_10_IRQHandler(void)
{
	while(!KEY_PD10)
		Delay_ms(10);
	
	
    if(EXTI_GetITStatus(EXTI_Line10) == SET)
    {
		McWheel_pulse_stop(TIM2); 			//即使是无限脉冲，也可以使用这个函数关闭
		McWheel_pulse_stop(TIM5); 			//即使是无限脉冲，也可以使用这个函数关闭
		while(1)
		{
			LED = 0;
			if(!KEY_PD10)
				break;
		}
    }
	
	LED = 1;
	
	while(!KEY_PD10)
		Delay_ms(10);
	
	Stpmotor_freq_num(2,1,1000);
	Stpmotor_freq_num(5,1,1000);
	
	McWheel_pulse_start(TIM2);
	McWheel_pulse_start(TIM5);
	EXTI_ClearITPendingBit(EXTI_Line10);
}



