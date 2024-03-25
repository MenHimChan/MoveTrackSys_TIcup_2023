#ifndef __ServoMotor_H__
#define __ServoMotor_H__

#include "stm32f10x.h"
#include "sys.h"

#define TIM1_ARR (10000-1)
#define TIM1_PSC (144-1)
//72M/(10000*144) = 50HZ
void ServoMotor_init(void);
void ServoMotor_set_ccr(uint8_t servo_motor_num,uint16_t ccr);

#endif
