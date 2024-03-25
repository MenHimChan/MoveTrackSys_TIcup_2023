#ifndef __XunJi_H
#define __XunJi_H

void xunji_Init(void);
u8 xunji_Abs(int8_t XP);
void xunji_Read(void);
//void xunji_SetInit(void);
void xunji_XY(void);
void xunji_adj_XQ(u16 NOM);
void xunji_adj_YQ(u16 NOM);
void xunji_adj_XH(u16 NOM);
void xunji_adj_YH(u16 NOM);
void xunji_XX(u16 M1,u16 M2);
	
void xunji_END(void);
void xunji_Begin(void);
void xunji_TZX(int16_t Count);
void xunji_XYCount(int8_t XP,int8_t YP,u16 M1,u16 M2);
_Bool xunji_Read_X(u8 NUM);
u8 xunji_Read_YAW(int16_t Count);
#endif 
