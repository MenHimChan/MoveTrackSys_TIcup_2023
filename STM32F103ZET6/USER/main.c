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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	// �ж����ȼ�����
	Key_init();											// ������ʼ��
	Key_PC13_Init();
	PID_Parameter_Init(&ShotPid);						// PID������ʼ��
	TestMode_Flag_Init(&test_mode);						// ģʽѡ���־λ��ʼ��
	LED_init();											
	McWheel_Init();
	uart3_init(9600);									// ��������ʼ��
	UART4_OPENMV_Init();
	
	// ����ѡ��
	
	while(1)
	{
		// ���Ե�1
		if(test_mode.Flag_Point1 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;						// ʵ��������
			u8 buf1[50];
			u8 buf2[50];
			Shot_Sequence = 1;
			Send_OpenMV_Cmd(1);								// ���Ͳ��Ե�1��ʶ����Ϣ
			
			// �е㸴λ
			while(1)
			{
				while(!rece_ok);										// �����˸���������
				// ��������
				xerror = Decode_UART_xdata();							// ��ȡx�᷽���ϵ����
				yerror = Decode_UART_ydata();							// ��ȡy�᷽�������
				
				if(Flag_Finish_Task)			// �����ֵ̫С ������Ϊ�Ѿ��ﵽĿ��λ��
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;				// ׼��������һ������
					break;
				}
				
				// PID X
				xerror /= 4;												// �Ŵ����
				Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
				Stp_x_real = range_protect(Stp_x_real, -50, 50);			// ����޷�
				
				// PID Y
				yerror /= 4;												// �Ŵ����
				Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
				Stp_y_real = range_protect(Stp_y_real, -50, 50);			// ����޷�
				
				// ���Ͳ�����Ϣ����������
				sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
				HMISends((char*)buf1);
				HMI_Sendb3times(0xFF);
				sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
				HMISends((char*)buf2);
				HMI_Sendb3times(0xFF);
				
				// ȷ��x���˶�����
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// ȷ��y���˶�����
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// �����������
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
				while(!McWheel_pulse_is_end(2));		 	// ��֤���嶼����
				while(!McWheel_pulse_is_end(5));
				rece_ok = 0;							
			}
			
			test_mode.Flag_Point1 = 0;						// ���Ա�־λ��λ
		}
		
		
		// ���Ե�2
		else if(test_mode.Flag_Point2 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;						// ʵ��������
			u8 buf1[50];
			u8 buf2[50];
			
			Shot_Sequence = 1;
			Send_OpenMV_Cmd(2);								// ���Ͳ��Ե�2��ʶ����Ϣ
			
			// ѭ����50*50����
			while(1)
			{
				while(!rece_ok);										// �����˸���������
				
				if(Flag_Finish_Task)
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;
					break;
				}
				
				// ��������
				xerror = Decode_UART_xdata();							// ��ȡx�᷽���ϵ����
				yerror = Decode_UART_ydata();							// ��ȡy�᷽���ϵ����
				
				// PID X
				xerror /= 4;												// �Ŵ����
				Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
				Stp_x_real = range_protect(Stp_x_real, -100, 100);			// ����޷�
				
				// PID Y
				yerror /= 4;												// �Ŵ����
				Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
				Stp_y_real = range_protect(Stp_y_real, -100, 100);			// ����޷�
				
				sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
				HMISends((char*)buf1);
				
				sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
				HMISends((char*)buf2);
				HMI_Sendb3times(0xFF);
				
				// ȷ��x�ķ���
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// ȷ��y�ķ���
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// �����˶�
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
//				while(!McWheel_pulse_is_end(2));		 	// ��֤���嶼����
//				while(!McWheel_pulse_is_end(5));
				Delay_ms(2);
				rece_ok = 0;					
			}
			
			test_mode.Flag_Point2 = 0;
		}	 

		
		// ���Ե�3
		else if(test_mode.Flag_Point3 == 1)
		{
			int xerror,yerror;
			int Stp_x_real,Stp_y_real;									// ʵ��������
			int Hz_num;													// Ƶ��
			u8 buf1[50];
			u8 buf2[50];
			// �����趨
			Shot_Sequence = 0;											// pid����ѡ��
			Hz_num = 500;												// �ٶ�ѡ��
			Send_OpenMV_Cmd(3);											// ���Ͳ��Ե�2��ʶ����Ϣ
			
			// �ߺ�ɫ�߱߿�
			while(1)
			{
				while(!rece_ok);										// �����˸���������
				
				if(Flag_Finish_Task)
				{
					Flag_Finish_Task = 0;
					rece_ok = 0;
					break;
				}
				
				
				// ��������
				xerror = Decode_UART_xdata();							// ��ȡx�᷽���ϵ����
				yerror = Decode_UART_ydata();							// ��ȡy�᷽���ϵ����
				
				
				if(my_abs(xerror) < 30 && my_abs(yerror) < 30) 			// �����ֵС��ĳ����ֵ
				{	
					Flag_Fast_Move = 0;									// �����ƶ�����
					Hz_num = 800;										// ����ѭ�����εĳ���
					// Hz_num 
					// 500			���ȶ�
					// 1000			�ܶ�
				}
					
				
				
				// ��û��λ�����εĵ�һ��ԭ��
				if(Flag_Fast_Move)
				{
					// PID X
					xerror /= 1;												// �Ŵ����
					Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence + 1], xerror);
					Stp_x_real = range_protect(Stp_x_real, -100, 100);			// ����޷�
					
					// PID Y
					yerror /= 1;												// �Ŵ����
					Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence + 1], yerror);
					Stp_y_real = range_protect(Stp_y_real, -100, 100);			// ����޷�
					
					sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
					HMISends((char*)buf1);
					HMI_Sendb3times(0xFF);
					
					sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
					HMISends((char*)buf2);
					HMI_Sendb3times(0xFF);
				}
				else
				{
					xerror /= 1;												// �Ŵ����
					Stp_x_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], xerror);
					Stp_x_real = range_protect(Stp_x_real, -4, 4);			// ����޷�
					
					// PID Y
					yerror /= 1;												// �Ŵ����
					Stp_y_real = PlacePID_Control(&ShotPid, Shot_Pid[Shot_Sequence], yerror);
					Stp_y_real = range_protect(Stp_y_real, -4, 4);			// ����޷�
					
					sprintf((char*)buf1,"page0.t1.txt=\"%d\"",Stp_x_real);
					HMISends((char*)buf1);
					HMI_Sendb3times(0xFF);
					
					sprintf((char*)buf2,"page0.t2.txt=\"%d\"",Stp_y_real);
					HMISends((char*)buf2);
					HMI_Sendb3times(0xFF);
				}
				
				// ȷ��x�ķ���
				if(Stp_x_real > 0)
					Dir_X = 0;
				else 
				{
					Dir_X = 1;
					Stp_x_real *= -1;
				}
				
				// ȷ��y�ķ���
				if(Stp_y_real > 0)
					Dir_Y = 0;
				else 
				{
					Dir_Y = 1;
					Stp_y_real *= -1;
				}
				
				
				// �����˶�
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
				
//				while(!McWheel_pulse_is_end(2));		 	// ��֤���嶼����
//				while(!McWheel_pulse_is_end(5));
				
				Delay_ms(1);
				rece_ok = 0;					
			}
			
			test_mode.Flag_Point3 = 0;
		}
	}	 
}

