#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "sys.h"
#include "stm32f10x.h"

//版本说明：v0.11
// 去除脉冲差计数部分，对按键停止步进电机，去除步进电机运行条件部分

#define TEST_LOG_EN 1
//测试时的log打印,若使能，每次电机停止时均会在串口一内打印 上次启动到此次停止的脉冲数
#define IO_OUTPUT_MODE 0
/*
StepMotor1 C6 -  B14 - C8  
StepMotor2 C7 - G9 -C9
其中0表示反转（靠近按键），1正转（远离按键）
*/

// #define PULSE_ABS(a) (a>=0?a:-1*a)
#define PULSE_FLAG_INF     1//无限输出标志
#define PULSE_FLAG_QUIT    2//输出中止标志
#define PULSE_FLAG_START   4//输出开始标志


void StepMotor_init(void);
void StepMotor_set_pulse_num(uint8_t Step_motor_num,uint32_t pulse_num);
void StepMotor_start(uint8_t step_motor_num);
int32_t StepMotor_stop(uint8_t step_motor_num);
uint8_t StepMotor_pulse_is_end(uint8_t step_motor_num);
void StepMotor_set_pulse_freq(uint32_t freq);
void StepMotor_generate_pulse(uint8_t Step_motor_num,uint8_t direction,uint32_t pulse_num);
void StepMotor_generate_freq_pulse(uint8_t Step_motor_num,uint8_t direction,uint32_t freq,uint32_t pulse_num);
void StepMotor_reset(uint8_t Step_motor_num);
int32_t StepMotor_move_pos(uint8_t Step_motor_num,int32_t position);
void StepMotor_clear_pos(uint8_t step_motor_num);
#endif
