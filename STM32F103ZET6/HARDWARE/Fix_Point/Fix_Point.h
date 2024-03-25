#ifndef __Fix_Point_H__
#define __Fix_Point_H__

#include "sys.h"
#include "stm32f10x.h"

#define FP_READ_D7      PDin(7)
#define FP_READ_B9      PBin(9)
#define FP_READ_D9      PDin(9)
#define FP_READ_E1      PEin(1)
#define FP_READ_E0      PEin(0)
#define FP_READ_E2      PEin(2)
#define FP_READ_G8      PGin(8)
#define FP_READ_B13     PBin(13)

void FixPoint_Init(void);
u8 FixPoint_1(void);

#endif

