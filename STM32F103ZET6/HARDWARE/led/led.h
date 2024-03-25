#ifndef __LED_H__
#define __LED_H__



#include "sys.h"
#include "stm32f10x.h"


#define LED PBout(1)


void led_init(void);
void LED_init(void);

void led_ctrl(uint8_t on_or_off);


#endif
