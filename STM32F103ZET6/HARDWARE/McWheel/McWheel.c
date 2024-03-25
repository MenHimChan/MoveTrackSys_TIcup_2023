#include "McWheel.h"
#include "sys.h"
static void TIMx_init_freq(TIM_TypeDef * TIMx,uint32_t freq);

TIM_TypeDef * tim_array[6]= {TIM2,TIM2,TIM2,TIM3,TIM4,TIM5};
/*
函数：void McWheel_Init(void)
参数：无
返回值：无
说明：初始化定时器2345输出脉冲
作者：Sheep
日期：22/09/30
*/
void McWheel_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7 ;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure2);
	
	// 方向脚 PC5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure3);

	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	TIMx_init_freq(TIM2,5000);
	TIMx_init_freq(TIM3,5000);
	TIMx_init_freq(TIM4,5000);
	TIMx_init_freq(TIM5,5000);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR为0,一开始就进入比较输出，即开始的初相位为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Disable);

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);

    TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Disable);
	
//	TIM_ARRPreloadConfig(TIM2,DISABLE);
//	TIM_ARRPreloadConfig(TIM3,DISABLE);
//	TIM_ARRPreloadConfig(TIM4,DISABLE);
//	TIM_ARRPreloadConfig(TIM5,DISABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
    TIM_CCxCmd(TIM2,TIM_Channel_1,TIM_CCx_Enable);
    TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
    TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Enable);
    TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE);
    TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
    TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);
    TIM_CCxCmd(TIM5,TIM_Channel_2,TIM_CCx_Enable);


	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
	TIM_Cmd(TIM5, DISABLE);
}

/*
函数：static void TIMx_init_freq(TIM_TypeDef * TIMx,uint32_t freq)
参数：TIM_TypeDef * TIMx 对应使用的定时器，uint32_t freq 初始化的频率
返回值：无
说明：内部初始化函数，不必外部使用
作者：Sheep
日期：22/09/30
*/
static void TIMx_init_freq(TIM_TypeDef * TIMx,uint32_t freq)
{
    uint16_t psc = 0,arr= 0;
    freq = freq*2;
    if(freq<= 50000 && freq>0) //1~50k
    {
        psc = 1439;
        arr = 50000.0/freq-1;
    }
    else if(freq > 50000) //50k~72M
    {
        psc = 0;
        arr = 72000000.0/freq-1;
    }
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
}

pulse TIM2_CC1_pulse;
void TIM2_IRQHandler()
{
    if(TIM_GetITStatus(TIM2,TIM_IT_CC1))
    {
        if(TIM2_CC1_pulse.pulse_flag == PULSE_FLAG_START)TIM2_CC1_pulse.pulse_num++;
        if((TIM2_CC1_pulse.pulse_num == TIM2_CC1_pulse.pulse_limit) && (TIM2_CC1_pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            // TIM_CCxCmd(TIM2,TIM_Channel_1,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            // TIM_ITConfig(TIM2,TIM_IT_CC1,DISABLE);
            TIM_Cmd(TIM2,DISABLE);//由于该定时器只使用一个通道，因此可以直接关掉定时器
            TIM2_CC1_pulse.pulse_num = 0;
            TIM2_CC1_pulse.pulse_flag = PULSE_FLAG_QUIT;
        }
        TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
    }
}

pulse TIM3_CC3_pulse;
void TIM3_IRQHandler()
{
    if(TIM_GetITStatus(TIM3,TIM_IT_CC3))
    {
        if(TIM3_CC3_pulse.pulse_flag == PULSE_FLAG_START)TIM3_CC3_pulse.pulse_num++;
        if((TIM3_CC3_pulse.pulse_num == TIM3_CC3_pulse.pulse_limit) && (TIM3_CC3_pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            // TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            // TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
            TIM_Cmd(TIM3,DISABLE);//由于该定时器只使用一个通道，因此可以直接关掉定时器
            TIM3_CC3_pulse.pulse_num = 0;
            TIM3_CC3_pulse.pulse_flag = PULSE_FLAG_QUIT;
        }
        TIM_ClearITPendingBit(TIM3,TIM_IT_CC3);
    }
}


pulse TIM4_CC3_pulse;
void TIM4_IRQHandler()
{
    if(TIM_GetITStatus(TIM4,TIM_IT_CC3))
    {
        if(TIM4_CC3_pulse.pulse_flag == PULSE_FLAG_START)TIM4_CC3_pulse.pulse_num++;
        if((TIM4_CC3_pulse.pulse_num == TIM4_CC3_pulse.pulse_limit) && (TIM4_CC3_pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            // TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            // TIM_ITConfig(TIM4,TIM_IT_CC3,DISABLE);
            TIM_Cmd(TIM4,DISABLE);//由于该定时器只使用一个通道，因此可以直接关掉定时器
            TIM4_CC3_pulse.pulse_num = 0;
            TIM4_CC3_pulse.pulse_flag = PULSE_FLAG_QUIT;
        }
        TIM_ClearITPendingBit(TIM4,TIM_IT_CC3);
    }
}

pulse TIM5_CC2_pulse;
void TIM5_IRQHandler()
{
    if(TIM_GetITStatus(TIM5,TIM_IT_CC2))
    {
        if(TIM5_CC2_pulse.pulse_flag == PULSE_FLAG_START)TIM5_CC2_pulse.pulse_num++;
        if((TIM5_CC2_pulse.pulse_num == TIM5_CC2_pulse.pulse_limit) && (TIM5_CC2_pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            // TIM_CCxCmd(TIM5,TIM_Channel_3,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            // TIM_ITConfig(TIM5,TIM_IT_CC2,DISABLE);
            TIM_Cmd(TIM5,DISABLE);//由于该定时器只使用一个通道，因此可以直接关掉定时器
            TIM5_CC2_pulse.pulse_num = 0;
            TIM5_CC2_pulse.pulse_flag = PULSE_FLAG_QUIT;
        }
        TIM_ClearITPendingBit(TIM5,TIM_IT_CC2);
    }
}

 /*
 函数：void McWheel_set_pulse_freq(TIM_TypeDef * TIMx,uint32_t freq)
 参数：TIM_TypeDef * TIMx要设置的定时器,uint32_t freq,设置的频率
 返回值：无
 说明：可以随时脉冲输出的频率，改变时的第一个脉冲持续时长不稳定。
 作者：Sheep
 日期：22/09/30
 */
void McWheel_set_pulse_freq(uint8_t tim_num,uint32_t freq)
{
    freq = freq*2;
    if(freq<= 50000 && freq>0) //1~50k
    {
        TIM_PrescalerConfig(tim_array[tim_num],1439,TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(tim_array[tim_num],50000.0/freq-1);
    }
    else if(freq > 50000) //50k~72M
    {
        TIM_PrescalerConfig(tim_array[tim_num],0,TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(tim_array[tim_num],72000000.0/freq-1);
    }
}


/*
函数：void McWheel_set_pulse_num(uint8_t McWheel_TIM_num,uint16_t pulse_num)
参数：uint8_t McWheel_TIM_num  2/3/4/5 ，注意不是TIM2这样的指针，uint16_t pulse_num，要输出的脉冲数，0表示无限脉冲，
返回值：无
说明：注意设置个数后，需要使用McWheel_pulse_start开启脉冲，若不想输出脉冲使用McWheel_pulse_stop,而不是让pulse_num = 0
作者：Sheep
日期：22/09/30
*/
void McWheel_set_pulse_num(uint8_t McWheel_TIM_num,uint16_t pulse_num)
{
    switch (McWheel_TIM_num)
    {
    case 2:
        if(pulse_num==0)TIM2_CC1_pulse.pulse_flag = PULSE_FLAG_INF;
        else 
        {
			TIM2_CC1_pulse.pulse_num =0;
            TIM2_CC1_pulse.pulse_flag = PULSE_FLAG_START;
            TIM2_CC1_pulse.pulse_limit = pulse_num*2;
        }
        break;
    case 3:
        if(pulse_num==0)TIM3_CC3_pulse.pulse_flag = PULSE_FLAG_INF;
        else 
        {
			TIM3_CC3_pulse.pulse_num =0;
            TIM3_CC3_pulse.pulse_flag = PULSE_FLAG_START;
            TIM3_CC3_pulse.pulse_limit = pulse_num*2;
        }
        break;
    case 4:
        if(pulse_num==0)TIM4_CC3_pulse.pulse_flag = PULSE_FLAG_INF;
        else 
        {
			TIM4_CC3_pulse.pulse_num =0;
            TIM4_CC3_pulse.pulse_flag = PULSE_FLAG_START;
            TIM4_CC3_pulse.pulse_limit = pulse_num*2;
        }
        break;
    case 5:
        if(pulse_num==0)TIM5_CC2_pulse.pulse_flag = PULSE_FLAG_INF;
        else 
        {
			TIM5_CC2_pulse.pulse_num =0;
            TIM5_CC2_pulse.pulse_flag = PULSE_FLAG_START;
            TIM5_CC2_pulse.pulse_limit = pulse_num*2;
        }
        break;
    default:
        break;
    }
}
/*
函数：uint8_t McWheel_pulse_is_end(uint8_t McWheel_TIM_num)
参数：2/3/4/5表示使用哪个定时器的脉轮
返回值：该脉轮产生的脉冲是否结束，返回1表示已结束，返回0表示未结束。
说明：该脉轮产生的脉冲是否结束，用在产生固定个数的脉冲之后
作者：Sheep
日期：22/09/30
*/
uint8_t McWheel_pulse_is_end(uint8_t McWheel_TIM_num)
{
    uint8_t ret=0;
    switch (McWheel_TIM_num)
    {
    case 2: ret = (TIM2_CC1_pulse.pulse_flag == PULSE_FLAG_QUIT);break;
    case 3: ret = (TIM3_CC3_pulse.pulse_flag == PULSE_FLAG_QUIT);break;
    case 4: ret = (TIM4_CC3_pulse.pulse_flag == PULSE_FLAG_QUIT);break;
    case 5: ret = (TIM5_CC2_pulse.pulse_flag == PULSE_FLAG_QUIT);break;
    default:
        break;
    }
    return ret;
}
void McWheel_SetHL(u8 McWheel_TIM1,u8 McWheel_TIM2,u8 McWheel_TIM3,u8 McWheel_TIM4)
{
	PCout(5)=McWheel_TIM1;
	PAout(5)=McWheel_TIM2;
	PAout(6)=McWheel_TIM3;
	PAout(7)=McWheel_TIM4;
}
void McWheel_SetOF(u8 McWheel_ZF)
{
	if(McWheel_ZF)
	{
		McWheel_pulse_start(TIM2);
		McWheel_pulse_start(TIM3);
		McWheel_pulse_start(TIM4);
		McWheel_pulse_start(TIM5);
	}
	else
	{
		McWheel_pulse_stop(TIM2);
		McWheel_pulse_stop(TIM3);
		McWheel_pulse_stop(TIM4);
		McWheel_pulse_stop(TIM5);
	}
}


void Stpmotor_freq_num(uint8_t TIM_num, uint16_t pulse_num, uint32_t freq)
{
	McWheel_set_pulse_freq(TIM_num, freq);					// 设定频率
	McWheel_set_pulse_num(TIM_num, pulse_num);				// 设定脉冲数
}







/*
使用简介：
McWheel_set_pulse_freq(2,1000);//设置频率
McWheel_set_pulse_num(2,500);//设置个数
McWheel_pulse_start(TIM2);//启动，
//如果需要多个同时启动，最好配置好再一起启动，配置的两个函数实测耗时10ms
while(!McWheel_pulse_is_end(2));//查看其是否做完指定个数的脉冲，可用于需要顺序脉冲的场合
McWheel_pulse_stop(TIM2) //即使是无限脉冲，也可以使用这个函数关闭
*/
