#include "shell.h"
#include "shell_port.h"
#include "usart.h"
Shell shell;
char shell_buffer[512];

signed short User_Shell_Write(char * data, unsigned short len)              
{
    int i = 0;
    for (i =0;i<len;i++)
    {
        while ((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
        USART1->DR = (u8)data[i];
    }
    return i+1;
}

void User_Shell_Init(uint32_t baudrate)
{
    uart_init(baudrate);
    shell.write = User_Shell_Write;
    shellInit(&shell,shell_buffer,512);
}

uint8_t recv_buf = 0;
void USART1_IRQHandler(void) //串口1中断服务程序
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
		recv_buf = USART_ReceiveData(USART1); //读取接收到的数据
        shellHandler(&shell,recv_buf);
    }
}


/*命令导出表*/
#include "StepMotor.h"

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                StepMotor_stop,StepMotor_stop,\
                "StepMotor_stop 1/2");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                StepMotor_start,StepMotor_start,\
                "StepMotor_start 1/2");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                StepMotor_generate_freq_pulse,StepMotor_generate_freq_pulse,\
                "StepMotor_generate_freq_pulse 1/2 0/1 <频率> <个数>");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                StepMotor_set_pulse_freq,StepMotor_set_pulse_freq,\
                "StepMotor_set_pulse_freq <频率>");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                StepMotor_reset,StepMotor_reset,\
                "StepMotor_reset 1/2");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                StepMotor_move_pos,StepMotor_move_pos,\
                "StepMotor_move_pos 1/2 <位置>)");


#include "ServoMotor.h"
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                ServoMotor_set_ccr,ServoMotor_set_ccr,\
                "ServoMotor_set_ccr 1/2 <ccr>)");

#include "McWheel.h"
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                McWheel_SetHL,McWheel_SetHL,\
                "McWheel_SetHL 0/1 0/1 0/1 0/1)");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                McWheel_SetOF,McWheel_SetOF,\
                "McWheel_SetOF 0/1)");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                McWheel_set_pulse_num,McWheel_set_pulse_num,\
                "McWheel_set_pulse_num 2/3/4/5 <个数>");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                McWheel_set_pulse_freq,McWheel_set_pulse_freq,\
                "McWheel_set_pulse_freq 2/3/4/5 <频率>)");

#include "MCARM.h"
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                MCARM_ACT,MCARM_ACT,\
                "MCARM_ACT <NUM>--action)");

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                MCARM_PRE,MCARM_PRE,\
                "MCARM_PRE <NUM>--preparation)");
				
				
#include "HWT101.h"

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                HWT101_clear_yaw,HWT101_clear_yaw,\
                "clear yaw");

#include "led.h"

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),\
                led_ctrl,led_ctrl,\
                "led_ctrl 0/1");
