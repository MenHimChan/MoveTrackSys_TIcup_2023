#ifndef __WORK_H
#define __WORK_H

void work_Place_J(int16_t AIM);
void work_Place_T(int16_t AIM);
void work_HTui(u8 route);
void work_QJin(u8 route);
void work_BZ_X(u8 route);
void work_BZY(void);
void work_TZX(int16_t Count);
_Bool work_Read_YAW(int16_t Count);
u8 FixPoint_Fix(void);
/*
void work_BZX(void);
void work_BZY(void);

void work_BZ(void);摆正车身

void work_LeftUp(void);需要计算，公式在微信里面
*/
#endif
