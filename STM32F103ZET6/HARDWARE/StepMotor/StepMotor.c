#include "StepMotor.h"
#include "usart.h"
#include "delay.h"


uint8_t temp_reset_flag = 0; //用于解决按键误触的变量
uint8_t temp_reset_flag1 = 0; //解决复位1误触的变量
typedef struct 
{
   uint32_t pulse_limit;
   uint32_t pulse_num;
   uint8_t pulse_flag; 
   int32_t pulse_pos; //脉冲位置，给有脉冲复位的电机使用
   int8_t pulse_dir;  //脉冲方向
   uint8_t operate_falg; //正在运动标志位确保是因为调用reset函数而触碰按键重启避免误触，或者有其他用途
} step_pulse;

step_pulse TIM8_CC1_Pulse={
    .pulse_limit=0,
    .pulse_num=0,  //初始化需要为0
    .pulse_flag=PULSE_FLAG_INF,
    .pulse_pos=0,  //这个无所谓，上机后允许的话可能需要复位一次？
    .pulse_dir=1,  //这个无所谓
    .operate_falg=0  //初始化为0,必要，可以确定不调用reset函数时，按下按键执行的函数
};
step_pulse TIM8_CC2_Pulse={
    .pulse_limit=0,
    .pulse_num=0,  //初始化需要为0
    .pulse_flag=PULSE_FLAG_INF,
    .pulse_pos=0,  //这个无所谓，上机后允许的话可能需要复位一次？
    .pulse_dir=1,  //这个无所谓
    .operate_falg=0  //初始化为0,必要，可以确定不调用reset函数时，按下按键执行的函数
};

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


/*
函数：StepMotor_EXTI_Init
参数：无
返回值：无
说明：内部函数，用于初始化外部中断线
作者：Sheep
日期：22/10/17
*/
static void StepMotor_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
    GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure1);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel= EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//按键优先级低于步进电机，方便回位
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void EXTI9_5_IRQHandler(void)
{
	
    if(EXTI_GetITStatus(EXTI_Line8) == SET)
    {
		temp_reset_flag1++;
		if(temp_reset_flag1 < 2)
		{
			//不管电机是否运行，只要单击按键，就停止
			StepMotor_stop(1);
			TIM8_CC1_Pulse.pulse_pos = 0; //位置归零
			StepMotor_generate_pulse(1,1,500);
			while(!StepMotor_pulse_is_end(1));
			printf("sheep_exti_stop1\r\n");
			// if(TIM8_CC1_Pulse.operate_falg == 1) //确保是调用复位后
			// {
			// }
		}
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9) == SET)
    {
		temp_reset_flag++;
		if((temp_reset_flag < 2) || (temp_reset_flag > 4))
		{
			StepMotor_stop(2);
			TIM8_CC2_Pulse.pulse_pos = 0; //位置归零
			StepMotor_generate_pulse(2,1,50);//回位防止误触按键
			while(!StepMotor_pulse_is_end(2));
			printf("sheep_exti_stop2\r\n");
        // if(TIM8_CC2_Pulse.operate_falg == 1)
        // {
        // }
		}
        printf("old_sheep_exti_stop2\r\n");
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}


/*
函数：StepMotor_init
参数：无
返回值：无
说明：用于两个步进电机的初始化
作者：Sheep
日期：22/10/17
*/
void StepMotor_init(void)
{
    StepMotor_EXTI_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG|RCC_APB2Periph_TIM8, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure1;
    GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14;//|GPIO_Pin_13
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure1);
    GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure1);

    TIMx_init_freq(TIM8,6000);//默认初始化为5000Hz

    TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR为0,一开始就进入比较输出，即开始的初相位为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Disable);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Disable);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM8_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_InitTypeDef GPIO_InitStructure;
#if IO_OUTPUT_MODE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    PCout(6)=0;PCout(7)=0;
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
#endif

    TIM_ClearFlag(TIM8, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);
    TIM_ITConfig(TIM8,TIM_IT_CC1,DISABLE);
    TIM_CCxCmd(TIM8,TIM_Channel_1,TIM_CCx_Disable);

    TIM_ClearITPendingBit(TIM8,TIM_IT_CC2);
    TIM_ITConfig(TIM8,TIM_IT_CC2,DISABLE);
    TIM_CCxCmd(TIM8,TIM_Channel_2,TIM_CCx_Disable);

	TIM_Cmd(TIM8, ENABLE);
}




void TIM8_CC_IRQHandler()
{
    if(TIM_GetITStatus(TIM8,TIM_IT_CC1))
    {
#if IO_OUTPUT_MODE 
        PCout(6)=!PCout(6);
#endif
        TIM8_CC1_Pulse.pulse_pos += TIM8_CC1_Pulse.pulse_dir;
        if(TIM8_CC1_Pulse.pulse_flag == PULSE_FLAG_START)TIM8_CC1_Pulse.pulse_num++;
        if((TIM8_CC1_Pulse.pulse_num == TIM8_CC1_Pulse.pulse_limit) && (TIM8_CC1_Pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            TIM_CCxCmd(TIM8,TIM_Channel_1,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            TIM_ITConfig(TIM8,TIM_IT_CC1,DISABLE);
            // TIM8_CC1_Pulse.pulse_num = 0;
            TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_QUIT;
            TIM8_CC1_Pulse.operate_falg = 0;
        }
        TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC2))
    {
#if IO_OUTPUT_MODE 
    PCout(7)=!PCout(7);
#endif
        TIM8_CC2_Pulse.pulse_pos +=  TIM8_CC2_Pulse.pulse_dir;
        if(TIM8_CC2_Pulse.pulse_flag == PULSE_FLAG_START)TIM8_CC2_Pulse.pulse_num++;
        if((TIM8_CC2_Pulse.pulse_num == TIM8_CC2_Pulse.pulse_limit) && (TIM8_CC2_Pulse.pulse_flag != PULSE_FLAG_INF))//不是无限输出就需要计数
        {
            TIM_CCxCmd(TIM8,TIM_Channel_2,TIM_CCx_Disable);//输出个数已到，失能输出通道。
            TIM_ITConfig(TIM8,TIM_IT_CC2,DISABLE);
            // TIM8_CC2_Pulse.pulse_num = 0;
            TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_QUIT;
            TIM8_CC2_Pulse.operate_falg = 0;
        }
        TIM_ClearITPendingBit(TIM8,TIM_IT_CC2);
    }
}


/*
函数：StepMotor_set_pulse_num
参数：Step_motor_num 1/2，对应两个通道电机，
        pulse_num脉冲个数，最多支持到 2,147,483,647 个
返回值：无
说明：注意这个函数只是单纯设置，没有启动电机
作者：Sheep
日期：22/10/17
*/
void StepMotor_set_pulse_num(uint8_t Step_motor_num,uint32_t pulse_num)
{
    switch (Step_motor_num)
    {
    case 1:
        if(pulse_num == 0) TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_INF; //无限产生脉冲。
        else 
        {
            TIM8_CC1_Pulse.pulse_num = 0;
            TIM8_CC1_Pulse.pulse_limit = pulse_num*2;
            TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_START;
        }
        break;
    case 2:
        if(pulse_num == 0) TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_INF; //无限产生脉冲。
        else 
        {
            TIM8_CC2_Pulse.pulse_num = 0;
            TIM8_CC2_Pulse.pulse_limit = pulse_num*2;
            TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_START;
        }
        break;
    default:
        break;
    }
}


/*
函数：StepMotor_start
参数：step_motor_num 1/2
返回值：无
说明：用于启动步进电机
作者：Sheep
日期：22/10/17
*/
void StepMotor_start(uint8_t step_motor_num)
{   
    switch (step_motor_num)
    {
    case 1:
        if(TIM8_CC1_Pulse.operate_falg == 0)//如果时静止的，就至启动标志位
        {
            TIM8_CC1_Pulse.operate_falg = 1;  //中断函数中会检测该标志位
        }
        TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);//清除中断标志位，避免一开启通道就进入中断
        TIM_CCxCmd(TIM8,TIM_Channel_1,TIM_CCx_Enable);
        TIM_ITConfig(TIM8,TIM_IT_CC1,ENABLE);
        break;
    case 2:
        if(TIM8_CC2_Pulse.operate_falg == 0)//如果时静止的，就至启动标志位
        {
            TIM8_CC2_Pulse.operate_falg = 1;//中断函数中会检测该标志位
        }
        TIM_ClearITPendingBit(TIM8,TIM_IT_CC2);
        TIM_CCxCmd(TIM8,TIM_Channel_2,TIM_CCx_Enable);
        TIM_ITConfig(TIM8,TIM_IT_CC2,ENABLE);
        break;
    default:
        break;
    }
}

/*
函数：StepMotor_stop
参数：step_motor_num 1/2
返回值：int32_t last_operate_pulse_num 从上次启动至此次停止运动的绝对脉冲数
说明：用于停止电机
作者：Sheep
日期：22/10/17
*/
int32_t StepMotor_stop(uint8_t step_motor_num)
{
    switch (step_motor_num)
    {
    case 1:
        TIM8_CC1_Pulse.operate_falg = 0;
        TIM_CCxCmd(TIM8,TIM_Channel_1,TIM_CCx_Disable);
        TIM_ITConfig(TIM8,TIM_IT_CC1,DISABLE);
        break;
    case 2:
        TIM8_CC2_Pulse.operate_falg = 0;
        TIM_CCxCmd(TIM8,TIM_Channel_2,TIM_CCx_Disable);
        TIM_ITConfig(TIM8,TIM_IT_CC2,DISABLE);
        break;
    default:
        break;
    }
    return 0;
}

/*
函数：StepMotor_pulse_is_end
参数：step_motor_num 1/2
返回值：1表示电机已经停止，0未停止
说明：可用于固定脉冲输出时等待脉冲结束
作者：Sheep
日期：22/10/17
*/
uint8_t StepMotor_pulse_is_end(uint8_t step_motor_num)
{
    uint8_t ret=0;
    switch (step_motor_num)
    {
    case 1:
        ret = (TIM8_CC1_Pulse.pulse_flag == PULSE_FLAG_QUIT);
        break;
    case 2:
        ret = (TIM8_CC2_Pulse.pulse_flag == PULSE_FLAG_QUIT);
        break;
    default:
        break;
    }
		return ret;
}


 /*
 函数：void McWheel_set_pulse_freq(TIM_TypeDef * TIMx,uint32_t freq)
 参数：TIM_TypeDef * TIMx要设置的定时器,uint32_t freq,设置的频率
 返回值：无
 说明：可以随时脉冲输出的频率，改变时的第一个脉冲持续时长不稳定。
 作者：Sheep
 日期：22/09/30
 */
void StepMotor_set_pulse_freq(uint32_t freq)
{
    freq = freq*2;
    if(freq<= 50000 && freq>0) //1~50k
    {
        TIM_PrescalerConfig(TIM8,1439,TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(TIM8,50000.0/freq-1);
    }
    else if(freq > 50000) //50k~72M
    {
        TIM_PrescalerConfig(TIM8,0,TIM_PSCReloadMode_Immediate);
        TIM_SetAutoreload(TIM8,72000000.0/freq-1);
    }
}

/*
函数：StepMotor_generate_pulse
参数：Step_motor_num 1/2；
        direction  0反转（靠近按键方向）/1正转（远离按键方向）；
        pulse_num 脉冲个数，最多支持到2,147,483,647个；
返回值：
说明：
作者：Sheep
日期：22/10/17
*/
void StepMotor_generate_pulse(uint8_t Step_motor_num,uint8_t direction,uint32_t pulse_num)
{
    switch (Step_motor_num)
    {
    case 1:
        TIM8_CC1_Pulse.pulse_dir = (direction*2-1);
        PBout(14) = direction;//0反转；1正转
        if(pulse_num == 0) TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_INF; //无限产生脉冲。
        else 
        {
            TIM8_CC1_Pulse.pulse_num = 0;
            TIM8_CC1_Pulse.pulse_limit = pulse_num*2;
            TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_START;
        }
        break;
    case 2:
        TIM8_CC2_Pulse.pulse_dir = (direction*2-1);
        PGout(9) = direction;//0反转；1正转
        if(pulse_num == 0) TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_INF; //无限产生脉冲。
        else 
        {
            TIM8_CC2_Pulse.pulse_num = 0;
            TIM8_CC2_Pulse.pulse_limit = pulse_num*2;
            TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_START;
        }
        break;
    default:
        break;
    }
    StepMotor_start(Step_motor_num);
}

/*
函数：StepMotor_generate_freq_pulse
参数：Step_motor_num 1/2；
        direction  0反转（靠近按键方向）/1正转（远离按键方向）；
        freq 设置频率
        pulse_num 脉冲个数，最多支持到2,147,483,647个；
返回值：无
说明：可以同时设置运行频率
作者：Sheep
日期：22/10/17
*/
void StepMotor_generate_freq_pulse(uint8_t Step_motor_num,uint8_t direction,uint32_t freq,uint32_t pulse_num)
{
    StepMotor_set_pulse_freq(freq);
    StepMotor_generate_pulse(Step_motor_num,direction,pulse_num);
}

/*
函数：StepMotor_reset
参数：Step_motor_num 1/2
返回值：无
说明：用于重启，支持异步重启，即调用该函数后，可以执行其他工作，电机碰到按键停止，并且pos归零
作者：Sheep
日期：22/10/17
*/
void StepMotor_reset(uint8_t Step_motor_num)
{
    switch (Step_motor_num)
    {
    case 1:
		temp_reset_flag1 = 0;
        TIM8_CC1_Pulse.pulse_flag = PULSE_FLAG_INF;
        TIM8_CC1_Pulse.pulse_dir = -1; 
        PBout(14) = 0; //正反转可能需要调整
        break;
    case 2:
		temp_reset_flag = 0;
        TIM8_CC2_Pulse.pulse_flag = PULSE_FLAG_INF;
        TIM8_CC2_Pulse.pulse_dir = -1; 
        PGout(9) = 0; //正反转可能需要调整
        break;
    default:
        break;
    }
    StepMotor_start(Step_motor_num);
}


/*
函数：StepMotor_move_pos
参数：Step_motor_num 1/2 ，position，可设置运行的位置
返回值：uint32_t 返回运行后的脉冲数位置
说明：可用于运行至指定位置，已reset为0点
作者：Sheep
日期：22/10/17
*/
int32_t StepMotor_move_pos(uint8_t Step_motor_num,int32_t position)
{
    uint32_t move_pulse = 0;
	int32_t pre_pos = 0;
    uint8_t move_dir = 0;
    int8_t move_sign = 0;
    switch (Step_motor_num)
    {
    case 1:
        pre_pos = TIM8_CC1_Pulse.pulse_pos;
        move_dir = position*2>TIM8_CC1_Pulse.pulse_pos;
        move_sign = move_dir*2-1;
        move_pulse = move_sign*(position*2 - TIM8_CC1_Pulse.pulse_pos);
        break;
    case 2:
        pre_pos = TIM8_CC2_Pulse.pulse_pos;
        move_dir = position*2>TIM8_CC2_Pulse.pulse_pos;
        move_sign = move_dir*2-1;
        move_pulse = move_sign*(position*2 - TIM8_CC2_Pulse.pulse_pos);
        break;
    default:
        break;
    }
    if(move_pulse == 0) StepMotor_stop(Step_motor_num);
    else 
        StepMotor_generate_pulse(Step_motor_num,move_dir,(move_pulse/2));
//    printf("dir-pos-sign-move-pulse:%+d,%d,%d,%d\r\n",TIM8_CC2_Pulse.pulse_dir,TIM8_CC2_Pulse.pulse_pos,move_sign,move_pulse);
	printf("dir-pos-sign-move-pulse:%+d,%d,%d,%d\r\n",TIM8_CC2_Pulse.pulse_dir,TIM8_CC2_Pulse.pulse_pos,move_dir,move_pulse);
    return (pre_pos/2);
}


void StepMotor_clear_pos(uint8_t step_motor_num)
{
    if(step_motor_num == 1) TIM8_CC1_Pulse.pulse_pos = 0;
    else if(step_motor_num == 2) TIM8_CC2_Pulse.pulse_pos = 0;
}

