#include "ServoMotor.h"

void ServoMotor_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = TIM1_ARR;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM1_PSC;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_ARRPreloadConfig(TIM1,ENABLE);

    TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //有效电平在后方
    TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//有效电平为低，即前高后低
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);

    TIM_Cmd(TIM1,ENABLE);
}

/*
函数：void ServoMotor_set_ccr(uint8_t servo_motor_num,uint16_t ccr)
参数：servo_motor_num 取值为1/2/3表示三个通道的舵机，ccr为比较值，取值为500~2500
返回值：无
说明：之后可以考虑对ccr限幅
作者：Sheep
日期：22/10/20
*/
void ServoMotor_set_ccr(uint8_t servo_motor_num,uint16_t ccr)
{
    // ccr = (ccr<=500?500:ccr);
    // ccr = (ccr>=2500?2500:ccr);//限幅
    switch (servo_motor_num)
    {
        case 1:TIM1->CCR1 = ccr;break;
        case 2:TIM1->CCR2 = ccr;break;
        case 3:TIM1->CCR3 = ccr;break;
        default: break;
    }
}
//注意：逻辑分析仪实测是发现，同时使用三个舵机时，c6 c7 步进电机的脉冲上会有不定期的1us的毛刺
