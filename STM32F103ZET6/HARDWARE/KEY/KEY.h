#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"
#include "stm32f10x.h"

#define KEY_PC13 PCin(13)
#define KEY_PD10 PDin(10)

void Key_init(void);
void Key_PC13_Init(void);



#endif

