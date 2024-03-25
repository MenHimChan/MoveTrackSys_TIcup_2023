#ifndef __McWheel_H__
#define __McWheel_H__

#include "sys.h"
#include "stm32f10x.h"

#define PULSE_FLAG_INF     1//无限输出标志
#define PULSE_FLAG_QUIT    2//输出中止标志
#define PULSE_FLAG_START   4//输出开始标志


#define Dir_X PCout(5)	
#define Dir_Y PAout(7)

typedef struct 
{
   uint16_t pulse_limit;
   uint16_t pulse_num;
   uint8_t pulse_flag; 
} pulse;


#define McWheel_pulse_start(TIMX) TIM_Cmd(TIMX,ENABLE)
#define McWheel_pulse_stop(TIMX)  TIM_Cmd(TIMX,DISABLE)
void McWheel_Init(void);
void McWheel_set_pulse_freq(uint8_t tim_num,uint32_t freq);
void McWheel_set_pulse_num(uint8_t McWheel_TIM_num,uint16_t pulse_num);
uint8_t McWheel_pulse_is_end(uint8_t McWheel_TIM_num);
void McWheel_SetHL(u8 McWheel_TIM1,u8 McWheel_TIM2,u8 McWheel_TIM3,u8 McWheel_TIM4);
void McWheel_SetOF(u8 McWheel_ZF);


// My
void Stpmotor_freq_num(uint8_t TIM_num, uint16_t pulse_num, uint32_t freq);

#endif
