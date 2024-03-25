#include "Delay.h"
#include "sys.h"
#include "usart.h"
#include "StepMotor.h"
#include "KEY.h"
#include "led.h"
#include "McWheel.h"
#include "function.h"
#include "usart-hmi.h"
#include "UART4_OPENMV.h"


	int main(void)
{
	Delay_init();	    		  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	// 中断优先级分组
	Key_init();											// 按键初始化
	Key_PC13_Init();
	PID_Parameter_Init(&ShotPid);						// PID参数初始化
	TestMode_Flag_Init(&test_mode);						// 模式选择标志位初始化
	LED_init();											
	McWheel_Init();
	uart3_init(9600);									// 串口屏初始化
	UART4_OPENMV_Init();
	
	// 参数选择
	
	while(1)
	{
		// 测试点1
		if(test_mode.Flag_Point1 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;						// 实际脉冲数
			u8 buf1[50];
			u8 buf2[50];
			Shot_Sequence = 1;
			Send_OpenMV_Cmd(1);								// 发送测试点1的识别信息
			
			// 靶点复位
			while(1)
			{
				while(!rece_ok);										// 收满八个即可跳出
				// 解析数据
				xerror = Decode_UART_xdata();							// 获取x轴方向上的误差
				yerror = Decode_UART_ydata();							// 获取y轴方向上误差
				
				if(Flag_Finish_Task)			// 如果差值太小 可以认为已经达到目标位置
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;				// 准备接收下一个数据
					break;
				}
				
				// PID X
				xerror /= 4;												// 放大误差
				Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
				Stp_x_real = range_protect(Stp_x_real, -50, 50);			// 输出限幅
				
				// PID Y
				yerror /= 4;												// 放大误差
				Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
				Stp_y_real = range_protect(Stp_y_real, -50, 50);			// 输出限幅
				
				// 发送步数信息到串口屏上
				sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
				HMISends((char*)buf1);
				HMI_Sendb3times(0xFF);
				sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
				HMISends((char*)buf2);
				HMI_Sendb3times(0xFF);
				
				// 确定x的运动方向
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// 确定y的运动方向
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// 步进电机动作
				if(Stp_x_real != 0)
				{
					Stpmotor_freq_num(2,Stp_x_real,1000);
					McWheel_pulse_start(TIM2);
				}
				if(Stp_y_real != 0)
				{
					Stpmotor_freq_num(5,Stp_y_real,1000);
					McWheel_pulse_start(TIM5);
				}
				while(!McWheel_pulse_is_end(2));		 	// 保证脉冲都发完
				while(!McWheel_pulse_is_end(5));
				rece_ok = 0;							
			}
			
			test_mode.Flag_Point1 = 0;						// 测试标志位复位
		}
		
		
		// 测试点2
		else if(test_mode.Flag_Point2 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;						// 实际脉冲数
			u8 buf1[50];
			u8 buf2[50];
			
			Shot_Sequence = 1;
			Send_OpenMV_Cmd(2);								// 发送测试点2的识别信息
			
			// 循迹走50*50的线
			while(1)
			{
				while(!rece_ok);										// 收满八个即可跳出
				
				if(Flag_Finish_Task)
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;
					break;
				}
				
				// 解析数据
				xerror = Decode_UART_xdata();							// 获取x轴方向上的误差
				yerror = Decode_UART_ydata();							// 获取y轴方向上的误差
				
				// PID X
				xerror /= 4;												// 放大误差
				Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
				Stp_x_real = range_protect(Stp_x_real, -100, 100);			// 输出限幅
				
				// PID Y
				yerror /= 4;												// 放大误差
				Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
				Stp_y_real = range_protect(Stp_y_real, -100, 100);			// 输出限幅
				
				sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
				HMISends((char*)buf1);
				
				sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
				HMISends((char*)buf2);
				HMI_Sendb3times(0xFF);
				
				// 确定x的方向
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// 确定y的方向
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// 步进运动
				if(Stp_x_real != 0)
				{
					Stpmotor_freq_num(2,Stp_x_real,400);
					McWheel_pulse_start(TIM2);
				}
				if(Stp_y_real != 0)
				{
					Stpmotor_freq_num(5,Stp_y_real,400);
					McWheel_pulse_start(TIM5);
				}
//				while(!McWheel_pulse_is_end(2));		 	// 保证脉冲都发完
//				while(!McWheel_pulse_is_end(5));
				Delay_ms(2);
				rece_ok = 0;					
			}
			
			test_mode.Flag_Point2 = 0;
		}	 

		
		// 测试点3
		else if(test_mode.Flag_Point3 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;									// 实际脉冲数
			int Hz_num;													// 频率
			u8 buf1[50];
			u8 buf2[50];
			// 参数设定
			Shot_Sequence = 0;											// pid参数选择
			Hz_num = 500;												// 速度选择
			Send_OpenMV_Cmd(3);											// 发送测试点2的识别信息
			
			// 走黑色线边框
			while(1)
			{
				while(!rece_ok);										// 收满八个即可跳出
				
				if(Flag_Finish_Task)
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;
					break;
				}
				
				
				// 解析数据
				xerror = Decode_UART_xdata();							// 获取x轴方向上的误差
				yerror = Decode_UART_ydata();							// 获取y轴方向上的误差
				
				
				if(my_abs(xerror) < 30 && my_abs(yerror) < 30) 			// 若误差值小于某个阈值
				{	
					Flag_Fast_Move = 0;									// 快速移动结束
					Hz_num = 800;										// 进入循迹矩形的程序
					// Hz_num 
					// 500			很稳定
					// 1000			很抖
				}
					
				
				
				// 还没定位到矩形的第一个原点
				if(Flag_Fast_Move)
				{
					// PID X
					xerror /= 1;												// 放大误差
					Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence + 1], xerror);
					Stp_x_real = range_protect(Stp_x_real, -100, 100);			// 输出限幅
					
					// PID Y
					yerror /= 1;												// 放大误差
					Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence + 1], yerror);
					Stp_y_real = range_protect(Stp_y_real, -100, 100);			// 输出限幅
					
					sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
					HMISends((char*)buf1);
					HMI_Sendb3times(0xFF);
					
					sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
					HMISends((char*)buf2);
					HMI_Sendb3times(0xFF);
				}
				else
				{
					xerror /= 1;												// 放大误差
					Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
					Stp_x_real = range_protect(Stp_x_real, -4, 4);			// 输出限幅
					
					// PID Y
					yerror /= 1;												// 放大误差
					Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
					Stp_y_real = range_protect(Stp_y_real, -4, 4);			// 输出限幅
					
					sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
					HMISends((char*)buf1);
					HMI_Sendb3times(0xFF);
					
					sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
					HMISends((char*)buf2);
					HMI_Sendb3times(0xFF);
				}
				
				// 确定x的方向
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// 确定y的方向
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// 步进运动
				if(Stp_x_real != 0)
				{
					Stpmotor_freq_num(2,Stp_x_real,Hz_num);
					McWheel_pulse_start(TIM2);
				}
				if(Stp_y_real != 0)
				{
					Stpmotor_freq_num(5,Stp_y_real,Hz_num);
					McWheel_pulse_start(TIM5);
				}
				
//				while(!McWheel_pulse_is_end(2));		 	// 保证脉冲都发完
//				while(!McWheel_pulse_is_end(5));
				
				Delay_ms(1);
				rece_ok = 0;					
			}
			
			test_mode.Flag_Point3 = 0;
		}
	}	 
}

