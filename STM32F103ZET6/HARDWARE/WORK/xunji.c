#include "stm32f10x.h"                  // Device header
#include "xunji.h"
//#include "motor.h"
#include "delay.h"
#include "HWT101.h"
#include "McWheel.h"
#include "ServoMotor.h"
#include "StepMotor.h"
#include "usart.h"
#include "KEY.h"

//ÉãÏñÍ··½ÏòÎªXÖá£¬ÉãÏñÍ·ÅÄÉã·½ÏòÎª¸º¡£ÕýÃæ¶ÔÉãÏñÍ·£¬ÓÒÊÖ±ßÎªYÕý°ëÖá¡£
u8 _X1,_X2,_X3,_X4,_X5,_X6,_X7;//Õ¾ÔÚ³µÉÏ¿´ÏòX¸º°ëÖá£¬´Ó×óÍùÓÒÊý¡£
u8 _Y1,_Y2,_Y3,_Y4,_Y5,_Y6,_Y7;

u8 X1,X2,X3,X4,X5,X6,X7;
u8 Y1,Y2,Y3,Y4,Y5,Y6,Y7;
extern u8 First;
u8 flag_X,flag_Y;
uint8_t flag_XF,flag_YF;//1ÊÇ¼Ó£¬2ÊÇ¼õ
uint8_t X,Y;
//#define W_C (yaw*150)
/*
uint16_t QJ;//´úÌæÊÖÊäÊý¾Ý
uint16_t XP_ZJia,XP_JShao;//Ð¡Æ«²î
uint16_t ZP_ZJia,ZP_JShao;//ÖÐÆ«²î
uint16_t DP_ZJia,DP_JShao;//´óÆ«²î
*/
void xunji_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure1);
	
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure2);
	
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure3.GPIO_Pin =GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure3);
	
	flag_X=1,flag_Y=1;
}
u8 xunji_Abs(int8_t XP)
{
	if(XP>0) return XP;
	else return -XP;
}
void xunji_Read(void)
{
	
	X1 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0);
	X2 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1);
	X3 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2);
	X4 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3);
	X5 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4);
	X6 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5);
	X7 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12);
	 
	_X1 =GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4);
	_X2 =GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5);
	_X3 =GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11);
	_X4 =GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12);
	_X5 =GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13);
	_X6 =GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14);
	_X7 =GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15);
	
	_Y1 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13);
	_Y2 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12);
	_Y3 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11);
	_Y4 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
	_Y5 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9 );
	_Y6 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8 );
	_Y7 =GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7 );
	
	Y1 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13);
	Y2 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14);
	Y3 = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_15);
	Y4 = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0);
	Y5 = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1);
	Y6 = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2);
	Y7 = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3);
}

void xunji_XY(void)
{
	xunji_Read();
	printf("X=%d,Y=%d\r\n",X,Y);
	if((_Y1==0||Y7==0)&&flag_X==1)
	{
		flag_XF=1;
		flag_X =0;
	}
	else if((Y1==0||_Y7==0)&&flag_X==1)
	{
		flag_XF=2;
		flag_X =0;
	}
	else if (X4==0&&(X6==0||X2==0)&&flag_XF==0)
	{
		flag_X=1;
	}
	
	if (flag_XF==1&&Y4==0)
	{
		X++;printf("X=%d\r\n",X);
		flag_X=0;
		flag_XF=0;
		First=1;
	}
	else if (flag_XF==2&&Y4==0)
	{
		X--;printf("X=%d\r\n",X);
		flag_X=0;
		flag_XF=0;
		First=1;
	}
	
	
	if((_X7==0||X1==0)&&flag_Y==1)
	{
		flag_YF=1;
		flag_Y=0;
	}
	else if((_X1==0||X7==0)&&flag_Y==1)
	{
		flag_YF=2;
		flag_Y=0;
	}
	else if (Y4==0&&(Y6==0||Y2==0)&&flag_YF==0)
	{
		flag_Y=1;
	}
	if (flag_YF==1&&X4==0)
	{
		Y++;printf("Y=%d\r\n",Y);
		flag_YF=0;
		First=1;
	}
	else if (flag_YF==2&&X4==0)
	{
		Y--;printf("Y=%d\r\n",Y);
		flag_YF=0;
		First=1;
	}
}

//#define NOM    5000
#define XAU +150
//#define NOM-XAU  -150
#define MAU  +1150
//#define NOM-MAU  -1150
#define BAU  +2150
//#define NOM-BAU  -2150
#define BBAU +3150
//#define NOM-BBAU -3150
void xunji_adj_XQ(u16 NOM)
{
	if (X1==1&&X2==1&&X3==1&&X4==0&&X5==1&&X6==1&&X7==1)
	{
		McWheel_set_pulse_freq(2,NOM);
		McWheel_set_pulse_freq(5,NOM);
	}
	else if (X1==1&&X2==1&&X3==0&&X4==0&&X5==1&&X6==1&&X7==1)//×óÐ¡Æ«
	{
		McWheel_set_pulse_freq(2,NOM+XAU);
		McWheel_set_pulse_freq(5,NOM+XAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==0&&X5==0&&X6==1&&X7==1)//ÓÒÐ¡Æ«
	{
		McWheel_set_pulse_freq(2,NOM+XAU);
		McWheel_set_pulse_freq(5,NOM-XAU);
	}
	else if (X1==1&&X2==1&&X3==0&&X4==1&&X5==1&&X6==1&&X7==1)//×óÆ«
	{
		McWheel_set_pulse_freq(2,NOM-MAU);
		McWheel_set_pulse_freq(5,NOM+MAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==1&&X5==0&&X6==1&&X7==1)//ÓÒÆ«
	{
		McWheel_set_pulse_freq(2,NOM+MAU);
		McWheel_set_pulse_freq(5,NOM-MAU);
	}	
	else if (X1==1&&X2==0&&X3==0&&X4==1&&X5==1&&X6==1&&X7==1)//×ó´óÆ«
	{
		McWheel_set_pulse_freq(2,NOM-BAU);
		McWheel_set_pulse_freq(5,NOM+BAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==1&&X5==0&&X6==0&&X7==1)//ÓÒ´óÆ«
	{
		McWheel_set_pulse_freq(2,NOM+BAU);
		McWheel_set_pulse_freq(5,NOM-BAU);
	}	
	else if (X1==0&&X4==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(4800 /*-W_C*/);
//		Motor4_SetSpeed(900 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM-BBAU);
		McWheel_set_pulse_freq(5,NOM+BBAU);
	}
	else if (X7==0&&X4==1)//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(900 /*-W_C*/);
//		Motor4_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM+BBAU);
		McWheel_set_pulse_freq(5,NOM-BBAU);
	}
	////////////////////////////////////////////
	////////////////////////////////////////////
	if (_X1==1&&_X2==1&&_X3==1&&_X4==0&&_X5==1&&_X6==1&&_X7==1)
	{
//		Motor2_SetSpeed(1500*K_M/*-W_C*/);
//		Motor3_SetSpeed(1500*K_M/*+W_C*/);
		McWheel_set_pulse_freq(3,NOM);
		McWheel_set_pulse_freq(4,NOM);
	}
	else if (_X1==1&&_X2==1&&_X3==0&&_X4==0&&_X5==1&&_X6==1&&_X7==1)//×óÐ¡Æ«
	{
//		Motor2_SetSpeed(4800/*-W_C*/);
//		Motor3_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM-XAU);
		McWheel_set_pulse_freq(4,NOM+XAU);
	}
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==0&&_X5==0&&_X6==1&&_X7==1)//ÓÒÐ¡Æ«
	{
//		Motor2_SetSpeed(1100 /*-W_C*/);
//		Motor3_SetSpeed(4800/*+W_C*/);
		McWheel_set_pulse_freq(3,NOM+XAU);
		McWheel_set_pulse_freq(4,NOM-XAU);
	}
	else if (_X1==1&&_X2==1&&_X3==0&&_X4==1&&_X5==1&&_X6==1&&_X7==1)//×óÆ«
	{
//		Motor2_SetSpeed(4800 /*-W_C*/);
//		Motor3_SetSpeed(1300 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM-MAU);
		McWheel_set_pulse_freq(4,NOM+MAU);
	}
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==0&&_X6==1&&_X7==1)//ÓÒÆ«
	{
//		Motor2_SetSpeed(1300 /*-W_C*/);
//		Motor3_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM+MAU);
		McWheel_set_pulse_freq(4,NOM-MAU);
	}	
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==0&&_X6==0&&_X7==1)//×ó´óÆ«
	{
//		Motor2_SetSpeed(4800 /*-W_C*/);
//		Motor3_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM-BAU);
		McWheel_set_pulse_freq(4,NOM+BAU);
	}
	else if (_X1==1&&_X2==0&&_X3==0&&_X4==1&&_X5==1&&_X6==1&&_X7==1)//ÓÒ´óÆ«
	{
//		Motor2_SetSpeed(4800 /*-W_C*/);
//		Motor3_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM+BAU);
		McWheel_set_pulse_freq(4,NOM-BAU);
	}
	else if (_X1==0&&_X4==1)//×ó´óÆ«
	{
//		Motor2_SetSpeed(4800 /*-W_C*/);
//		Motor3_SetSpeed(900 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM-BBAU);
		McWheel_set_pulse_freq(4,NOM+BBAU);
	}
	else if (_X7==0&&_X4==1)//ÓÒ´óÆ«
	{
//		Motor2_SetSpeed(900 /*-W_C*/);
//		Motor3_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(3,NOM+BBAU);
		McWheel_set_pulse_freq(4,NOM-BBAU);
	}
}
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
void xunji_adj_XH(u16 NOM)
{
	if (X1==1&&X2==1&&X3==1&&X4==0&&X5==1&&X6==1&&X7==1)
	{
//		Motor1_SetSpeed(1500*K_M/*+W_C*/);
//		Motor4_SetSpeed(1500*K_M/*-W_C*/);
		McWheel_set_pulse_freq(2,NOM);
		McWheel_set_pulse_freq(5,NOM);
	}
	else if (X1==1&&X2==1&&X3==0&&X4==0&&X5==1&&X6==1&&X7==1)//×óÐ¡Æ«
	{
//		Motor1_SetSpeed(1100 /*+W_C*/);
//		Motor4_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM+XAU);
		McWheel_set_pulse_freq(5,NOM-XAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==0&&X5==0&&X6==1&&X7==1)//ÓÒÐ¡Æ«
	{
//		Motor1_SetSpeed(4800 /*+W_C*/);
//		Motor4_SetSpeed(1100 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM-XAU);
		McWheel_set_pulse_freq(5,NOM+XAU);
	}
	else if (X1==1&&X2==1&&X3==0&&X4==1&&X5==1&&X6==1&&X7==1)//×óÆ«
	{
//		Motor1_SetSpeed(1300 /*+W_C*/);
//		Motor4_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM+MAU);
		McWheel_set_pulse_freq(5,NOM-MAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==1&&X5==0&&X6==1&&X7==1)//ÓÒÆ«
	{
//		Motor1_SetSpeed(4800 /*+W_C*/);
//		Motor4_SetSpeed(1300 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM-MAU);
		McWheel_set_pulse_freq(5,NOM+MAU);
	}	
	else if (X1==1&&X2==0&&X3==0&&X4==1&&X5==1&&X6==1&&X7==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(1100 /*+W_C*/);
//		Motor4_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM+BAU);
		McWheel_set_pulse_freq(5,NOM-BAU);
	}
	else if (X1==1&&X2==1&&X3==1&&X4==1&&X5==0&&X6==0&&X7==1)//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(4800 /*+W_C*/);
//		Motor4_SetSpeed(1100 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM-BAU);
		McWheel_set_pulse_freq(5,NOM+BAU);
	}	
	else if ((X1==0||X2==0)&&X4==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(900 /*+W_C*/);
//		Motor4_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM+BBAU);
		McWheel_set_pulse_freq(5,NOM-BBAU);
	}
	else if ((X6==0||X7==0)&&X4==1)//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(4800 /*+W_C*/);
//		Motor4_SetSpeed(900 /*-W_C*/);
		McWheel_set_pulse_freq(2,NOM-BBAU);
		McWheel_set_pulse_freq(5,NOM+BBAU);
	}
		/////////////////////////////////////////////
		/////////////////////////////////////////////
	if (_X1==1&&_X2==1&&_X3==1&&_X4==0&&_X5==1&&_X6==1&&_X7==1)
	{
//		Motor2_SetSpeed(1500*K_M/*+W_C*/);
//		Motor3_SetSpeed(1500*K_M/*-W_C*/);
		McWheel_set_pulse_freq(3,NOM);
		McWheel_set_pulse_freq(4,NOM);
	}
	else if (_X1==1&&_X2==1&&_X3==0&&_X4==0&&_X5==1&&_X6==1&&_X7==1)//×óÐ¡Æ«
	{
//		Motor2_SetSpeed(1500 /*+W_C*/);
//		Motor3_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM+XAU);
		McWheel_set_pulse_freq(4,NOM-XAU);
	}
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==0&&_X5==0&&_X6==1&&_X7==1)//ÓÒÐ¡Æ«
	{
//		Motor2_SetSpeed(4800 /*+W_C*/);
//		Motor3_SetSpeed(1500 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM-XAU);
		McWheel_set_pulse_freq(4,NOM+XAU);
	}
	else if (_X1==1&&_X2==1&&_X3==0&&_X4==1&&_X5==1&&_X6==1&&_X7==1)//×óÆ«
	{
//		Motor2_SetSpeed(1300 /*+W_C*/);
//		Motor3_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM+MAU);
		McWheel_set_pulse_freq(4,NOM-MAU);
	}
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==0&&_X6==1&&_X7==1)//ÓÒÆ«
	{
//		Motor2_SetSpeed(4800 /*+W_C*/);
//		Motor3_SetSpeed(1300 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM-MAU);
		McWheel_set_pulse_freq(4,NOM+MAU);
	}	
	else if (_X1==1&&_X2==0&&_X3==0&&_X4==1&&_X5==1&&_X6==1&&_X7==1)//ÓÒ´óÆ«
	{
//		Motor2_SetSpeed(1100 /*+W_C*/);
//		Motor3_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM+BAU);
		McWheel_set_pulse_freq(4,NOM-BAU);
	}
	else if (_X1==1&&_X2==1&&_X3==1&&_X4==1&&_X5==0&&_X6==0&&_X7==1)//×ó´óÆ«
	{
//		Motor2_SetSpeed(4800 /*+W_C*/);
//		Motor3_SetSpeed(1100 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM-BAU);
		McWheel_set_pulse_freq(4,NOM+BAU);
	}

	else if ((_X1==0||_X2==0)&&_X4==1)//×ó´óÆ«
	{
//		Motor2_SetSpeed(900 /*+W_C*/);
//		Motor3_SetSpeed(4800 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM+BBAU);
		McWheel_set_pulse_freq(4,NOM-BBAU);
	}
	else if ((_X6==0||_X7==0)&&_X4==1)//ÓÒ´óÆ«
	{
//		Motor2_SetSpeed(4800 /*+W_C*/);
//		Motor3_SetSpeed(900 /*-W_C*/);
		McWheel_set_pulse_freq(3,NOM-BBAU);
		McWheel_set_pulse_freq(4,NOM+BBAU);
	}
}
void xunji_adj_YH(u16 NOM)
{
	if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==0&&_Y5==1&&_Y6==1&&_Y7==1)
	{
//		Motor3_SetSpeed(1500*K_M);
//		Motor4_SetSpeed(1500*K_M);
		McWheel_set_pulse_freq(4,NOM);
		McWheel_set_pulse_freq(5,NOM);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==0&&_Y4==0&&_Y5==1&&_Y6==1&&_Y7==1)//×óÐ¡Æ«
	{
//		Motor3_SetSpeed(1100 );
//		Motor4_SetSpeed(4800 );
		McWheel_set_pulse_freq(4,NOM+XAU);
		McWheel_set_pulse_freq(5,NOM-XAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==0&&_Y5==0&&_Y6==1&&_Y7==1)//ÓÒÐ¡Æ«
	{
//		Motor3_SetSpeed(4800 );
//		Motor4_SetSpeed(1100 );
		McWheel_set_pulse_freq(4,NOM-XAU);
		McWheel_set_pulse_freq(5,NOM+XAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==0&&_Y4==1&&_Y5==1&&_Y6==1&&_Y7==1)//×óÆ«
	{
//		Motor3_SetSpeed(1300 );
//		Motor4_SetSpeed(4800 );
		McWheel_set_pulse_freq(4,NOM+MAU);
		McWheel_set_pulse_freq(5,NOM-MAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==1&&_Y5==0&&_Y6==1&&_Y7==1)//ÓÒÆ«
	{
//		Motor3_SetSpeed(4800 );
//		Motor4_SetSpeed(1300 );
		McWheel_set_pulse_freq(4,NOM-MAU);
		McWheel_set_pulse_freq(5,NOM+MAU);
	}	
	else if (_Y1==1&&_Y2==0&&_Y3==0&&_Y4==1&&_Y5==1&&_Y6==1&&_Y7==1)//×ó´óÆ«
	{
		McWheel_set_pulse_freq(4,NOM+BAU);
		McWheel_set_pulse_freq(5,NOM-BAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==1&&_Y5==0&&_Y6==0&&_Y7==1)//ÓÒ´óÆ«
	{
		McWheel_set_pulse_freq(4,NOM-BAU);
		McWheel_set_pulse_freq(5,NOM+BAU);
	}
	else if ((_Y1==0||_Y2==0)&&_Y4==1)//×ó´óÆ«
	{
		McWheel_set_pulse_freq(4,NOM+BBAU);
		McWheel_set_pulse_freq(5,NOM-BBAU);
	}
	else if (_Y4==1&&(_Y7==0||_Y6==0))//ÓÒ´óÆ«
	{
		McWheel_set_pulse_freq(4,NOM-BBAU);
		McWheel_set_pulse_freq(5,NOM+BBAU);
	}
	/////////////////////////////////
	/////////////////////////////////
	if (Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==1&&Y6==1&&Y7==1)
	{
//		Motor1_SetSpeed(1500*K_M);
//		Motor2_SetSpeed(1500*K_M);
		McWheel_set_pulse_freq(2,NOM);
		McWheel_set_pulse_freq(3,NOM);
	}
	else if (Y1==1&&Y2==1&&Y3==0&&Y4==0&&Y5==1&&Y6==1&&Y7==1)//×óÐ¡Æ«
	{
//		Motor1_SetSpeed(4800 );
//		Motor2_SetSpeed(1100 );
		McWheel_set_pulse_freq(2,NOM-XAU);
		McWheel_set_pulse_freq(3,NOM+XAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==0&&Y6==1&&Y7==1)//ÓÒÐ¡Æ«
	{
//		Motor1_SetSpeed(1100 );
//		Motor2_SetSpeed(4800 );
		McWheel_set_pulse_freq(2,NOM+XAU);
		McWheel_set_pulse_freq(3,NOM-XAU);
	}
	else if (Y1==1&&Y2==1&&Y3==0&&Y4==1&&Y5==1&&Y6==1&&Y7==1)//×óÆ«
	{
//		Motor1_SetSpeed(4800 );
//		Motor2_SetSpeed(1300 );
		McWheel_set_pulse_freq(2,NOM-MAU);
		McWheel_set_pulse_freq(3,NOM+MAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==1&&Y5==0&&Y6==1&&Y7==1)//ÓÒÆ«
	{
//		Motor1_SetSpeed(1300 );
//		Motor2_SetSpeed(4800 );
		McWheel_set_pulse_freq(2,NOM+MAU);
		McWheel_set_pulse_freq(3,NOM-MAU);
	}	
	else if (Y1==1&&Y2==0&&Y3==0&&Y4==1&&Y5==1&&Y6==1&&Y7==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(4800 );
//		Motor2_SetSpeed(1100 );
		McWheel_set_pulse_freq(2,NOM-BAU);
		McWheel_set_pulse_freq(3,NOM+BAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==1&&Y5==0&&Y6==0&&Y7==1)//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(1100 );
//		Motor2_SetSpeed(4800 );
		McWheel_set_pulse_freq(2,NOM+BAU);
		McWheel_set_pulse_freq(3,NOM-BAU);
	}
	else if ((Y1==0||Y2==0)&&Y4==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(4800 );
//		Motor2_SetSpeed(900 );
		McWheel_set_pulse_freq(2,NOM-BBAU);
		McWheel_set_pulse_freq(3,NOM+BBAU);
	}
	else if (Y4==1&&(Y6==0||Y7==0))//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(900 );
//		Motor2_SetSpeed(4800 );
		McWheel_set_pulse_freq(2,NOM+BBAU);
		McWheel_set_pulse_freq(3,NOM-BBAU);
	}
}
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////

void xunji_adj_YQ(u16 NOM)
{
	if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==0&&_Y5==1&&_Y6==1&&_Y7==1)
	{
//		Motor3_SetSpeed(1500*K_M/*-W_C*/);
//		Motor4_SetSpeed(1500*K_M/*+W_C*/);
		McWheel_set_pulse_freq(4,NOM);
		McWheel_set_pulse_freq(5,NOM);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==0&&_Y4==0&&_Y5==1&&_Y6==1&&_Y7==1)//×óÐ¡Æ«
	{
//		Motor3_SetSpeed(4800 /*-W_C*/);
//		Motor4_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM-XAU);
		McWheel_set_pulse_freq(5,NOM+XAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==0&&_Y5==0&&_Y6==1&&_Y7==1)//ÓÒÐ¡Æ«
	{
//		Motor3_SetSpeed(1100 /*-W_C*/);
//		Motor4_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM+XAU);
		McWheel_set_pulse_freq(5,NOM-XAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==0&&_Y4==1&&_Y5==1&&_Y6==1&&_Y7==1)//×óÆ«
	{
//		Motor3_SetSpeed(4800 /*-W_C*/);
//		Motor4_SetSpeed(1300 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM-MAU);
		McWheel_set_pulse_freq(5,NOM+MAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==1&&_Y5==0&&_Y6==1&&_Y7==1)//ÓÒÆ«
	{
//		Motor3_SetSpeed(1300 /*-W_C*/);
//		Motor4_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM+MAU);
		McWheel_set_pulse_freq(5,NOM-MAU);
	}	
	else if (_Y1==1&&_Y2==0&&_Y3==0&&_Y4==1&&_Y5==1&&_Y6==1&&_Y7==1)//×ó´óÆ«
	{
//		Motor3_SetSpeed(4800 /*-W_C*/);
//		Motor4_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM-BAU);
		McWheel_set_pulse_freq(5,NOM+BAU);
	}
	else if (_Y1==1&&_Y2==1&&_Y3==1&&_Y4==1&&_Y5==0&&_Y6==0&&_Y7==1)//ÓÒ´óÆ«
	{
//		Motor3_SetSpeed(1100 /*-W_C*/);
//		Motor4_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM+BAU);
		McWheel_set_pulse_freq(5,NOM-BAU);
	}
	else if ((_Y1==0||_Y2==0)&&_Y4==1)//×ó´óÆ«
	{
//		Motor3_SetSpeed(4800 /*-W_C*/);
//		Motor4_SetSpeed(900 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM-BBAU);
		McWheel_set_pulse_freq(5,NOM+BBAU);
	}
	else if (_Y4==1&&(_Y7==0||_Y6==0))//ÓÒ´óÆ«
	{
//		Motor3_SetSpeed(900 /*-W_C*/);
//		Motor4_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(4,NOM+BBAU);
		McWheel_set_pulse_freq(5,NOM-BBAU);
	}
	/////////////////////////////////
	/////////////////////////////////
	if (Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==1&&Y6==1&&Y7==1)
	{
//		Motor1_SetSpeed(1500*K_M/*-W_C*/);
//		Motor2_SetSpeed(1500*K_M/*+W_C*/);
		McWheel_set_pulse_freq(2,NOM);
		McWheel_set_pulse_freq(3,NOM);
	}
	else if (Y1==1&&Y2==1&&Y3==0&&Y4==0&&Y5==1&&Y6==1&&Y7==1)//×óÐ¡Æ«
	{
//		Motor1_SetSpeed(1100 /*-W_C*/);
//		Motor2_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM+XAU);
		McWheel_set_pulse_freq(3,NOM-XAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==0&&Y5==0&&Y6==1&&Y7==1)//ÓÒÐ¡Æ«
	{
//		Motor1_SetSpeed(4800 /*-W_C*/);
//		Motor2_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM-XAU);
		McWheel_set_pulse_freq(3,NOM+XAU);
	}
	else if (Y1==1&&Y2==1&&Y3==0&&Y4==1&&Y5==1&&Y6==1&&Y7==1)//×óÆ«
	{
//		Motor1_SetSpeed(1300 /*-W_C*/);
//		Motor2_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM+MAU);
		McWheel_set_pulse_freq(3,NOM-MAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==1&&Y5==0&&Y6==1&&Y7==1)//ÓÒÆ«
	{
//		Motor1_SetSpeed(4800 /*-W_C*/);
//		Motor2_SetSpeed(1300 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM-MAU);
		McWheel_set_pulse_freq(3,NOM+MAU);
	}	
	else if (Y1==1&&Y2==0&&Y3==0&&Y4==1&&Y5==1&&Y6==1&&Y7==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(1100 /*-W_C*/);
//		Motor2_SetSpeed(4800 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM+BAU);
		McWheel_set_pulse_freq(3,NOM-BAU);
	}
	else if (Y1==1&&Y2==1&&Y3==1&&Y4==1&&Y5==0&&Y6==0&&Y7==1)//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(4800 /*-W_C*/);
//		Motor2_SetSpeed(1100 /*+W_C*/);
		McWheel_set_pulse_freq(2,NOM-BAU);
		McWheel_set_pulse_freq(3,NOM+BAU);
	}
	else if ((Y1==0||Y2==0)&&Y4==1)//×ó´óÆ«
	{
//		Motor1_SetSpeed(900 );
//		Motor2_SetSpeed(4800 );
		McWheel_set_pulse_freq(2,NOM+BBAU);
		McWheel_set_pulse_freq(3,NOM-BBAU);
	}
	else if (Y4==1&&(Y6==0||Y7==0))//ÓÒ´óÆ«
	{
//		Motor1_SetSpeed(4800 );
//		Motor2_SetSpeed(900 );
		McWheel_set_pulse_freq(2,NOM-BBAU);
		McWheel_set_pulse_freq(3,NOM+BBAU);
	}
}
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////

void xunji_TZX(int16_t Count)
{
	uint8_t Time3=0;
	McWheel_SetOF(0);
	McWheel_set_pulse_freq(2,150);
	McWheel_set_pulse_freq(3,150);
	McWheel_set_pulse_freq(4,150);
	McWheel_set_pulse_freq(5,150);
	float yaw_1,g_z_cre;
	static float yaw_2;
	uint16_t Y_S;
	uint16_t Y_P=15;
	Delay_ms(5);
	while(Time3<15)
	{
		g_z_cre = W_Z_CRE;
		yaw_1 = YAW-Count;
		if((yaw_1-yaw_2)>10);//printf("yaw_1 yaw_2 yaw_1-yaw_2:%f,%f,%f\r\n",yaw_1,yaw_2,(yaw_2-yaw_1))
		yaw_2=yaw_1;
		if  (yaw_1>30){yaw_1=30;Time3=0;}
		else if(yaw_1<-30){yaw_1=30;Time3=0;}
		if  (yaw_1>0){Y_S=(uint16_t)((yaw_1*Y_P)+g_z_cre*0.1);}
		else	    {Y_S=(uint16_t)((-1*yaw_1*Y_P)-g_z_cre*0.1);}
		Delay_ms(1);
		printf("gz_cre yaw_1 YAW Y_S:%f,%f,%f,%d\r\n",g_z_cre,yaw_1,YAW,Y_S);
		if(yaw_1>(-0.1)&&yaw_1<(0.1))
		{
			McWheel_SetOF(0);
			Time3++;
		}
		else if(yaw_1<(-0.1))
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
		else if(yaw_1>(0.1))
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

//float speed_rate[7] = {0.5,0.75,1,1.25,1,0.75,0.5};

extern u8 INY,INX,Q_H;
extern _Bool Times_1;
void xunji_XYCount(int8_t XP,int8_t YP,u16 M1,u16 M2)
{ 
	_Bool flag_act=1;
	ServoMotor_set_ccr (1,950);
	u16 count_XY_last=0;
	u8 Xflag=1,Yflag=1,XFflag=0,YFflag=0;
	if(XP==2&&Times_1==0)XFflag=1;
	u8 XPP=0;u8 YPP=0;
	u16 count_XY=400;
	XPP=xunji_Abs(XP);YPP=xunji_Abs(YP);
	if(XPP==1||YPP==1)flag_act=0;
	while(Xflag||Yflag)
	{
		xunji_Read();
		if(count_XY==1&&flag_act){ServoMotor_set_ccr (3,350);StepMotor_move_pos (1,10000);}
		if	   (INY==0&&INX==0){;}//{xunji_XX(M1,M2);}}
		else if(INY==1&&Q_H==1&&count_XY>0){xunji_adj_YQ(M1*0.8);count_XY--;}
		else if(INX==1&&Q_H==0&&count_XY>0){xunji_adj_XH(M1*0.8);count_XY--;}
		else if(INY==1&&Q_H==0&&count_XY>0){xunji_adj_YH(M1*0.8);count_XY--;}
		else if(INX==1&&Q_H==1&&count_XY>0){xunji_adj_XQ(M1*0.8);count_XY--;}
		else if(INY==1&&Q_H==1&&YPP>=2){xunji_adj_YQ(M1*1.3);}
		else if(INX==1&&Q_H==0&&XPP>=2){xunji_adj_XH(M1*1.3);}
		else if(INY==1&&Q_H==0&&YPP>=2){xunji_adj_YH(M1*1.3);}
		else if(INX==1&&Q_H==1&&XPP>=2){xunji_adj_XQ(M1*1.3);}
		else if(INY==1&&Q_H==1&&YPP==1&&count_XY_last<=50){xunji_adj_YQ(M1*1);count_XY_last++;}
		else if(INX==1&&Q_H==0&&XPP==1&&count_XY_last<=50){xunji_adj_XH(M1*1);count_XY_last++;}
		else if(INY==1&&Q_H==0&&YPP==1&&count_XY_last<=50){xunji_adj_YH(M1*1);count_XY_last++;}
		else if(INX==1&&Q_H==1&&XPP==1&&count_XY_last<=50){xunji_adj_XQ(M1*1);count_XY_last++;}
		else if(INY==1&&Q_H==1&&count_XY_last>50){xunji_adj_YQ(M1*0.8);}
		else if(INX==1&&Q_H==0&&count_XY_last>50){xunji_adj_XH(M1*0.8);}
		else if(INY==1&&Q_H==0&&count_XY_last>50){xunji_adj_YH(M1*0.8);}
		else if(INX==1&&Q_H==1&&count_XY_last>50){xunji_adj_XQ(M1*0.8);}
		Delay_ms(1);
		if(count_XY==299)McWheel_SetOF(1);
		if(_X4==0&&X4==0&&YFflag==1&&YPP>0){YPP--;YFflag=0;}
		else if(_X4==0&&_X3==1&&_X5==1&&YFflag==1&&YPP>0){YPP--;YFflag=0;}
		else if(X3==0&&X4==0&&X6==0)XFflag=1;
		
		if(_Y4==0&&Y4==0&&XPP>0&&XFflag==1){XPP--;XFflag=0;}
		else if(_Y4==0&&_Y3==1&&_Y5==1&&XFflag==1&&XPP>0){XPP--;XFflag=0;}
		else if(Y3==0&&Y4==0&&Y6==0)YFflag=1;
		
		if(YPP==0)Yflag=0;
		if(XPP==0)Xflag=0;
	}
	X+=XP;Y+=YP;First=1;
	printf("\r\nX=%d,Y=%d\r\n",X,Y);
	ServoMotor_set_ccr (1,1100);
	StepMotor_move_pos (2,300);
}

void xunji_END(void)
{
	
	McWheel_SetOF(1);
	while(!(X1==0&&X4==0&&X7==0)){xunji_Read();Delay_ms(5);}
	McWheel_SetHL(0,1,0,1);
	McWheel_set_pulse_freq(2,5000);
	McWheel_set_pulse_freq(3,5000);
	McWheel_set_pulse_freq(4,5000);
	McWheel_set_pulse_freq(5,5000);
	while(1)
	{
		xunji_Read();
		Delay_ms(5);
		if(_Y1==0&&_Y4==0&&_Y7==0)
		{
			Delay_ms(900);
			McWheel_SetOF(0);
			while(1);
		}
	}
}

void xunji_Begin(void)
{
	
	McWheel_set_pulse_num(2,0);
	McWheel_set_pulse_num(3,0);
	McWheel_set_pulse_num(4,0);
	McWheel_set_pulse_num(5,0);
	McWheel_SetHL(0,1,0,1);
	McWheel_SetOF(1);
	Delay_ms(900);
	xunji_XYCount(0,1,5000,5000);
	McWheel_SetOF(0);
	INX=1;
	INY=0;
	Q_H=1;
	McWheel_SetHL(1,1,1,1);
	xunji_XYCount(2,0,5000,5000);
	X=2;Y=2;
	McWheel_SetOF(0);
}


_Bool xunji_Read_X(u8 NUM)
{
	if(NUM==1)
	{
		xunji_Read();
		if  (_X4==0&&(_X3==1||_X5==1))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(NUM==0)
	{
		xunji_Read();
		if  (X4==0&&(X3==1||X5==1))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(NUM==2)
	{
		xunji_Read();
		if  (X1==0&&_X4==0&&_X7==0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(NUM==3)
	{
		xunji_Read();
		if  (Y7==1&&_Y1==1&&Y6==1&&_Y2==1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(NUM==4)
	{
		xunji_Read();
		if  (Y7==0&&_Y1==0&&Y6==1&&_Y2==1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(NUM==5)
	{
		xunji_Read();
		if  (Y7==1&&Y6==1&&Y5==1&&Y4==1&&Y3==1&&Y2==1&&_Y7==1&&Y1==1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 1;
}
u8 xunji_Read_YAW(int16_t Count)
{
	float yaw_1;
	yaw_1=YAW-Count;
	if  (yaw_1>-0.1&&yaw_1<0.1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
