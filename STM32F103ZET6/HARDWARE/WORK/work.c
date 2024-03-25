#include "stm32f10x.h"                  // Device header
#include "xunji.h"
#include "Delay.h"
#include "McWheel.h"
#include "usart.h"
#include "HWT101.h"
#include "Fix_Point.h"
extern u8 _X1,_X2,_X3,_X4,_X5,_X6,_X7,_Y1,_Y2,_Y3,_Y4,_Y5,_Y6,_Y7,X1,X2,X3,X4,X5,X6,X7,Y1,Y2,Y3,Y4,Y5,Y6,Y7;;//站在车上看向X负半轴，从左往右数。
extern uint8_t X,Y;
u8 count=0;
void work_TZX(int16_t Count)
{
	printf("turn\r\n");
	uint8_t Time3=0;
	McWheel_SetOF(0);
	McWheel_set_pulse_freq(2,1000);
	McWheel_set_pulse_freq(3,1000);
	McWheel_set_pulse_freq(4,1000);
	McWheel_set_pulse_freq(5,1000);
	float yaw_1,g_z_cre;
	uint16_t Y_S;
	uint16_t Y_P=15;
	Delay_ms(50);
	while(Time3<15)
	{
		g_z_cre = W_Z_CRE;
		yaw_1 = YAW-Count;
		if  (yaw_1>30){yaw_1=30;Time3=0;}
		else if(yaw_1<-30){yaw_1=30;Time3=0;}
		if  (yaw_1>0){Y_S=(uint16_t)((yaw_1*Y_P)+g_z_cre*0.1);}
		else	    {Y_S=(uint16_t)((-1*yaw_1*Y_P)-g_z_cre*0.1);}
		Delay_ms(1);
		printf("gz_cre yaw_1 YAW Y_S:%f,%f,%f,%d\r\n",g_z_cre,yaw_1,YAW,Y_S);
		if(yaw_1>(-2)&&yaw_1<(2))
		{
			McWheel_SetOF(0);
//			printf("run1\r\n");
			Time3++;
		}
		else if(yaw_1<(-2))
		{
			if(Y_S==0)McWheel_SetOF(0);//McWheel_SetOF(0);
			else
			{
				McWheel_SetHL(0,0,1,1);
				McWheel_set_pulse_num(2,Y_S);
				McWheel_set_pulse_num(3,Y_S);
				McWheel_set_pulse_num(4,Y_S);
				McWheel_set_pulse_num(5,Y_S);
				McWheel_SetOF(1);
				Time3=0;
			}
		}
		else if(yaw_1>(2))
		{
			if(Y_S==0)McWheel_SetOF(0);
			else
			{
				McWheel_SetHL(1,1,0,0);
				McWheel_set_pulse_num(2,Y_S);
				McWheel_set_pulse_num(3,Y_S);
				McWheel_set_pulse_num(4,Y_S);
				McWheel_set_pulse_num(5,Y_S);
				
				McWheel_SetOF(1);
				Time3=0;
			}
		}
	}
}
_Bool work_Read_YAW(int16_t Count)
{
	float yaw_1;
	yaw_1=YAW-Count;
	if  (yaw_1>-2&&yaw_1<2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void work_Place_J(int16_t AIM)//左
{
	printf("turn1\r\n");
	McWheel_SetOF(0);
	McWheel_set_pulse_freq(2,5000);
	McWheel_set_pulse_freq(3,5000);
	McWheel_set_pulse_freq(4,5000);
	McWheel_set_pulse_freq(5,5000);
	McWheel_SetHL(1,1,0,0);
	McWheel_set_pulse_num(2,3600);
	McWheel_set_pulse_num(3,3600);
	McWheel_set_pulse_num(4,3600);
	McWheel_set_pulse_num(5,3600);
	McWheel_SetOF(1);
	while(!McWheel_pulse_is_end(2));
	while(work_Read_YAW(AIM))work_TZX(AIM);
	McWheel_set_pulse_num(2,0);
	McWheel_set_pulse_num(3,0);
	McWheel_set_pulse_num(4,0);
	McWheel_set_pulse_num(5,0);
}
                           
void work_Place_T(int16_t AIM)//右
{
	
	McWheel_SetOF(0);
	McWheel_SetHL(0,0,1,1);
	McWheel_set_pulse_freq(2,3000);
	McWheel_set_pulse_freq(3,3000);
	McWheel_set_pulse_freq(4,3000);
	McWheel_set_pulse_freq(5,3000);
	McWheel_set_pulse_num(2,3600);
	McWheel_set_pulse_num(3,3600);
	McWheel_set_pulse_num(4,3600);
	McWheel_set_pulse_num(5,3600);
	McWheel_SetOF(1);
	while(!McWheel_pulse_is_end(2));
	while(work_Read_YAW(AIM))work_TZX(AIM);
	McWheel_set_pulse_num(2,0);
	McWheel_set_pulse_num(3,0);
	McWheel_set_pulse_num(4,0);
	McWheel_set_pulse_num(5,0);
}

void work_HTui(u8 route)//0为车中心对准十字，1为车头碰到黑线
{
	Delay_ms(100);
	u8 Flag=1;
	McWheel_SetOF(0);
	McWheel_SetHL(0,0,0,0);
	McWheel_set_pulse_freq(2,2000);
	McWheel_set_pulse_freq(3,2000);
	McWheel_set_pulse_freq(4,2000);
	McWheel_set_pulse_freq(5,2000);
	McWheel_SetOF(1);
	if(route==0)
	{
		
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(Y3==1&&(Y4==0||_Y4==0)&& _Y5==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==1)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(X2==0&&X4==0&&X6==0)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==2)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(Y7==0&&_Y1==0&&Y6==1&&_Y2==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==3)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(_Y7==0&&Y1==0&&_Y6==1&&Y2==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
}
//0为车中心对准十字，1为车头碰到黑线，2为两边最前面循迹碰到线
//3为后面循迹碰到线,4为两边最后一个循迹碰到线
void work_QJin(u8 route)
{
	Delay_ms(50);
	u8 Flag=1;
	McWheel_SetOF(0);
	McWheel_SetHL(1,1,1,1);
	McWheel_set_pulse_freq(2,2000);
	McWheel_set_pulse_freq(3,2000);
	McWheel_set_pulse_freq(4,2000);
	McWheel_set_pulse_freq(5,2000);
	McWheel_SetOF(1);
	if(route==0)
	{
		while(Flag)
		{
			Delay_ms(5);
			xunji_Read();
			// xunji_adj_XQ();
			if(Y3==1&&Y4==0&&Y5==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==1)
	{
		while(Flag)
		{
			Delay_ms(5);
			xunji_Read();
			// xunji_adj_XQ();
			if(X4==0&&(X2==0||X6==0))
			{
				Delay_ms(5);
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==2)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(Y7==0&&_Y1==0&&Y6==1&&_Y2==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==3)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(_X2==0&&_X4==0&&_X6==0)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
	else if(route==4)
	{
		while(Flag)
		{
			Delay_ms(10);
			xunji_Read();
			if(_Y7==0&&Y1==0&&_Y6==1&&Y2==1)
			{
				McWheel_SetOF(0);
				Flag=0;
			}
		}
	}
}
void work_BZ_X(u8 route)
{
	u8 Time2=0;
	_Bool flag_left_right=0;
	McWheel_set_pulse_freq(2,1500);
	McWheel_set_pulse_freq(3,1500);
	McWheel_set_pulse_freq(4,1500);
	McWheel_set_pulse_freq(5,1500);
	if(route==0)
	{
		while(Time2<20)
		{
			xunji_Read();
			if(_X1==0||_X2==0||_X3==0)
			{
				McWheel_SetHL(1,0,1,0);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=0;
			}
			else if(_X5==0||_X6==0||_X7==0)
			{
				McWheel_SetHL(0,1,0,1);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=1;
			}
			else if(_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==1&&_X6==1&&_X7==1)
			{
				if(flag_left_right==0)
				{
					McWheel_SetHL(1,0,1,0);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
				else if(flag_left_right==1)
				{
					McWheel_SetHL(0,1,0,1);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
			}
			else if(_X4==0&&_X3==1&&_X5==1)
			{
				McWheel_SetOF(0);
				Delay_ms(1);
				Time2++;
			}
			
		}
		Delay_ms(5);
	}
	else if(route==1)
	{
		while(Time2<20)
		{
			xunji_Read();
			if(X5==0||X6==0||X7==0)
			{
				McWheel_SetHL(1,0,1,0);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=0;
			}
			else if(X1==0||X2==0||X3==0)
			{
				McWheel_SetHL(0,1,0,1);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=1;
			}
			else if(X1==1&&X2==1&&X3==1&&X4==1&&X5==1&&X6==1&&X7==1)
			{
				if(flag_left_right==0)
				{
					McWheel_SetHL(1,0,1,0);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
				else if(flag_left_right==1)
				{
					McWheel_SetHL(0,1,0,1);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
			}
			else if(X4==0&&X3==1&&X5==1)
			{
				McWheel_SetOF(0);
				Delay_ms(1);
				Time2++;
			}
			
		}
		Delay_ms(5);
	}
	else if(route==2)
	{
		McWheel_set_pulse_freq(2,300);
		McWheel_set_pulse_freq(3,300);
		McWheel_set_pulse_freq(4,300);
		McWheel_set_pulse_freq(5,300);
		while(Time2<20)
		{
			xunji_Read();
			if(_X1==0||_X2==0||_X3==0)
			{
				McWheel_SetHL(0,0,1,1);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=0;
			}
			else if(_X5==0||_X6==0||_X7==0)
			{
				McWheel_SetHL(1,1,0,0);
				McWheel_set_pulse_num(2,50);
				McWheel_set_pulse_num(3,50);
				McWheel_set_pulse_num(4,50);
				McWheel_set_pulse_num(5,50);
				McWheel_SetOF(1);
				while(!McWheel_pulse_is_end(2));
				Time2=0;
				flag_left_right=1;
			}
			else if(_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==1&&_X6==1&&_X7==1)
			{
				if(flag_left_right==0)
				{
					McWheel_SetHL(0,0,1,1);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
				else if(flag_left_right==1)
				{
					McWheel_SetHL(1,1,0,0);
					McWheel_set_pulse_num(2,50);
					McWheel_set_pulse_num(3,50);
					McWheel_set_pulse_num(4,50);
					McWheel_set_pulse_num(5,50);
					McWheel_SetOF(1);
					while(!McWheel_pulse_is_end(2));
					Time2=0;
				}
			}
			else if(_X4==0&&_X3==1&&_X5==1)
			{
				McWheel_SetOF(0);
				Delay_ms(1);
				Time2++;
			}
			
		}
		Delay_ms(5);
	}
}

void work_BZY(void)
{
	u8 Time2=0;
	McWheel_set_pulse_freq(2,1500);
	McWheel_set_pulse_freq(3,1500);
	McWheel_set_pulse_freq(4,1500);
	McWheel_set_pulse_freq(5,1500);
	while(Time2<20)
	{
		xunji_Read();
		if(Y1==0&&_Y1==0&&Y2==1&&_Y2==1)
		{
			McWheel_SetOF(0);
			Delay_ms(1);
			Time2++;
		}
		else if(X4==0&&X3==0&&X5==0)
		{
			McWheel_SetHL(1,1,1,1);
			McWheel_set_pulse_num(2,1000);
			McWheel_set_pulse_num(3,1000);
			McWheel_set_pulse_num(4,1000);
			McWheel_set_pulse_num(5,1000);
			McWheel_SetOF(1);
			Delay_ms(500);
			Time2=0;
		}
		else 
		{
			McWheel_SetHL(0,0,0,0);
			McWheel_set_pulse_num(2,50);
			McWheel_set_pulse_num(3,50);
			McWheel_set_pulse_num(4,50);
			McWheel_set_pulse_num(5,50);
			McWheel_SetOF(1);
			Delay_ms(10);
			Time2=0;
		}
	}
}
u8 FixPoint_Fix(void)
{
	xunji_Read();
	if(FixPoint_1()!=1)
	{
		return FixPoint_1();//4偏右了,5偏左了
	}
	else if(_Y1==0&&FixPoint_1()==0)
	{
		return 1;//车停好了
	}
//	else if ((_Y2==0||_Y3==0)&&FixPoint_1()==0)
//	{
//		return 2;//车偏前面了
//	}
//	else if ((_Y2==1&&_Y1==1)&&FixPoint_1()==0)
//	{
//		return 3;//车偏后面了
//	}
	return 0;
}
u8 FixPoint_adjust(void)
{
	printf("run9\r\n");
	
	if(FixPoint_Fix()==1)
	{
		printf("run10\r\n");
		count+=1;
		if(count>=20)return 1;
	}

	else if(FixPoint_Fix()==4)
	{
		McWheel_SetHL(0,1,0,1);
		McWheel_set_pulse_freq(2,750);
		McWheel_set_pulse_freq(3,750);
		McWheel_set_pulse_freq(4,750);
		McWheel_set_pulse_freq(5,750);
		McWheel_set_pulse_num(2,0);
		McWheel_set_pulse_num(3,0);
		McWheel_set_pulse_num(4,0);
		McWheel_set_pulse_num(5,0);
		McWheel_SetOF(1);
		while(FixPoint_Fix()==4);
		McWheel_SetOF(0);
	}
	else if(FixPoint_Fix()==5)
	{
		McWheel_SetHL(1,0,1,0);
		McWheel_set_pulse_freq(2,750);
		McWheel_set_pulse_freq(3,750);
		McWheel_set_pulse_freq(4,750);
		McWheel_set_pulse_freq(5,750);
		McWheel_set_pulse_num(2,0);
		McWheel_set_pulse_num(3,0);
		McWheel_set_pulse_num(4,0);
		McWheel_set_pulse_num(5,0);
		McWheel_SetOF(1);
		while(FixPoint_Fix()==5);
		McWheel_SetOF(0);
	}
	return 0;
}
/*
void work_BZY(void)
{
	while(1)
	{
		if(Y1==0||Y2==0||Y3==0)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay_ms(5);
			Motor_SetOF(0,0,0,0);
		}
		else if(Y5==0||Y6==0||Y7==0)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Delay_ms(5);
			Motor_SetOF(0,0,0,0);
		}
		else if(Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==1&&Y6==1&&Y7==1)
		{
			Motor_SetOF(0,0,0,0);
			return ;
		}
	}
}
void work_BZX(void)
{
	while(1)
	{
		xunji_Read();
		if(X1==0||X2==0||X3==0)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Delay_ms(5);
			Motor_SetOF(0,0,0,0);
		}
		else if(X5==0||X6==0||X7==0)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay_ms(5);
			Motor_SetOF(0,0,0,0);
		}
		else if(X1==1&&X2==1&&X3==1&&X4==0&&X5==1&&X6==1&&X7==1)
		{
			Motor_SetOF(0,0,0,0);
			return;
		}
	}
}

void work_BZ(void)
{
	u8 XZ=0,YZ=0;
	while(1)
	{
		xunji_Read();
		if(X1==1||X2==1||X3==1)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Motor_SetOF(0,0,0,0);
		}
		else if(X5==1||X6==1||X7==1)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Motor_SetOF(0,0,0,0);
		}
		else if(X1==1&&X2==1&&X3==1&&X4==0&&X5==1&&X6==1&&X7==1)
		{
			Motor_SetOF(0,0,0,0);
			XZ=1;
		}
		else if((Y1==1||Y2==1||Y3==1)&&XZ==1)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Motor_SetOF(0,0,0,0);
		}
		else if(Y5==1||Y6==1||Y7==1)
		{
			Motor_SetOF(1,1,1,1);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Motor_SetOF(0,0,0,0);
		}
		else if(Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==1&&Y6==1&&Y7==1)
		{
			Motor_SetOF(0,0,0,0);
			YZ=1;
		}
		else if(XZ==1&&YZ==1)
		{
			return ;
		}
	}
}*/

/*
void work_LeftUp(void)
{
	Motor_SetOF(0,1,1,1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	while(1)
	{
		if(X==7&&Y==4)
		{
			Motor_SetOF(0,0,0,0);
			return ;
		}
	}
}
*/
