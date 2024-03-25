#ifndef __UART4__OPENMV__H
#define __UART4__OPENMV__H


#include "sys.h"
#include "led.h"

#define DATA_MAX_LEN 8

extern u8 receive_data[DATA_MAX_LEN];
extern u8 receive_num;
extern u8 rece_ok;
extern u8 Flag_Finish_Task;

void UART4_OPENMV_Init(void);
void Send_OpenMV_Cmd(u8 mode);
int Decode_UART_xdata(void);
int Decode_UART_ydata(void);
//void OpenMV_Send_Cmd(void);
//void OpenMV_Send_Cmd_Down(void);
//void OpenMV_Send_Cmd_Up(void);
#endif 



