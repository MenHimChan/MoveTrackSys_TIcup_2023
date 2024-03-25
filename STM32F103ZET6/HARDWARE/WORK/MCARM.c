#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "StepMotor.h"
#include "ServoMotor.h"
#include "usart.h"
#include "McWheel.h"
#include "work.h"
#include "xunji.h"
void MCARM_RET(void)
{
	ServoMotor_set_ccr(1,650);
	ServoMotor_set_ccr(2,630);
	StepMotor_reset(2);
	while(!StepMotor_pulse_is_end(2));
	StepMotor_reset(1);
	while(!StepMotor_pulse_is_end(1));
	StepMotor_move_pos(1,9500);
	while(!StepMotor_pulse_is_end(1));
}
void MCARM_PRE(u8 NUM)
{
	if(NUM==0)
	{
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_num(2,0);
		McWheel_set_pulse_num(3,0);
		McWheel_set_pulse_num(4,0);
		McWheel_set_pulse_num(5,0);
		McWheel_set_pulse_freq(2,1500);
		McWheel_set_pulse_freq(3,1500);
		McWheel_set_pulse_freq(4,1500);
		McWheel_set_pulse_freq(5,1500);
		McWheel_SetOF(1);
		while(xunji_Read_X(0));//当走出黑线的时候退出循环
		McWheel_SetOF(0);
		Delay_ms(200);
		McWheel_set_pulse_num(2,700);
		McWheel_set_pulse_num(3,700);
		McWheel_set_pulse_num(4,700);
		McWheel_set_pulse_num(5,700);
		McWheel_SetOF(1);//从退出黑线开始再向前走600步
		while(!McWheel_pulse_is_end(2));
		return ;
	}
	else if(NUM==1)
	{
		McWheel_SetHL(0,0,0,0);
		McWheel_set_pulse_freq(2,1500);
		McWheel_set_pulse_freq(3,1500);
		McWheel_set_pulse_freq(4,1500);
		McWheel_set_pulse_freq(5,1500);
		McWheel_SetOF(1);
		while(xunji_Read_X(3));//当走出黑线的时候退出循环
		McWheel_SetOF(0);
		McWheel_set_pulse_num(2,200);
		McWheel_set_pulse_num(3,200);
		McWheel_set_pulse_num(4,200);
		McWheel_set_pulse_num(5,200);
		McWheel_SetOF(1);//从退出黑线开始再向后走350步
		while(!McWheel_pulse_is_end(2));
		return ;
	}
	else if(NUM==2)
	{
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_freq(2,1500);
		McWheel_set_pulse_freq(3,1500);
		McWheel_set_pulse_freq(4,1500);
		McWheel_set_pulse_freq(5,1500);
		McWheel_SetOF(1);
		while(xunji_Read_X(4));//当走出黑线的时候退出循环
		McWheel_SetOF(0);
		return ;
	}
	else if(NUM==3)
	{
//		printf("1\r\n");
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_freq(2,1500);
		McWheel_set_pulse_freq(3,1500);
		McWheel_set_pulse_freq(4,1500);
		McWheel_set_pulse_freq(5,1500);
		McWheel_SetOF(1);
		while(xunji_Read_X(5));//当走出黑线的时候退出循环
		McWheel_SetOF(0);
		McWheel_set_pulse_num(2,800);
		McWheel_set_pulse_num(3,800);
		McWheel_set_pulse_num(4,800);
		McWheel_set_pulse_num(5,800);
		McWheel_SetOF(1);//从退出黑线开始再向前走800步
		while(!McWheel_pulse_is_end(2));
		printf("6\r\n");
		return ;
	}
	if(NUM==4)
	{
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_freq(2,1500);
		McWheel_set_pulse_freq(3,1500);
		McWheel_set_pulse_freq(4,1500);
		McWheel_set_pulse_freq(5,1500);
		McWheel_SetOF(1);
		while(xunji_Read_X(0));//当走出黑线的时候退出循环
		McWheel_SetOF(0);
		McWheel_set_pulse_num(2,100);
		McWheel_set_pulse_num(3,100);
		McWheel_set_pulse_num(4,100);
		McWheel_set_pulse_num(5,100);
		McWheel_SetOF(1);//从退出黑线开始再向前走600步
		while(!McWheel_pulse_is_end(2));
		return ;
	}
}
void MCARM_MOV(u8 MOD,u16 Num,u16 fm)
{
	McWheel_set_pulse_freq(2,fm);
	McWheel_set_pulse_freq(3,fm);
	McWheel_set_pulse_freq(4,fm);
	McWheel_set_pulse_freq(5,fm);
	while(MOD==0)
	{
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_num(2,Num);
		McWheel_set_pulse_num(3,Num);
		McWheel_set_pulse_num(4,Num);
		McWheel_set_pulse_num(5,Num);
		McWheel_SetOF(1);
		while(!McWheel_pulse_is_end(2));
		McWheel_set_pulse_freq(2,5000);
		McWheel_set_pulse_freq(3,5000);
		McWheel_set_pulse_freq(4,5000);
		McWheel_set_pulse_freq(5,5000);
		break;
	}
	while(MOD==1)
	{
		McWheel_SetHL(0,0,0,0);
		McWheel_set_pulse_num(2,Num);
		McWheel_set_pulse_num(3,Num);
		McWheel_set_pulse_num(4,Num);
		McWheel_set_pulse_num(5,Num);
		McWheel_SetOF(1);
		while(!McWheel_pulse_is_end(2));
		McWheel_set_pulse_freq(2,5000);
		McWheel_set_pulse_freq(3,5000);
		McWheel_set_pulse_freq(4,5000);
		McWheel_set_pulse_freq(5,5000);
		break;
	}
	while(MOD==2)
	{
		McWheel_SetHL(1,1,1,1);
		McWheel_set_pulse_num(2,Num);
		McWheel_set_pulse_num(3,Num);
		McWheel_set_pulse_num(4,Num);
		McWheel_set_pulse_num(5,Num);
		McWheel_SetOF(1);
		break;
	}
	while(MOD==3)
	{
		McWheel_SetHL(0,0,0,0);
		McWheel_set_pulse_num(2,Num);
		McWheel_set_pulse_num(3,Num);
		McWheel_set_pulse_num(4,Num);
		McWheel_set_pulse_num(5,Num);
		McWheel_SetOF(1);
		break;
	}
	
}
void MCARM_ACT(u8 NUM)
{
	static u8 count;
	count%=3;
	switch(count)
	{
		case 0:ServoMotor_set_ccr (3,350 );count++;break;
		case 1:ServoMotor_set_ccr (3,800 );count++;break;
		case 2:ServoMotor_set_ccr (3,1240);count++;break;
	}
	if(NUM==1)
	{
		ServoMotor_set_ccr (1,630);
		Delay_ms(800);
		StepMotor_move_pos (2,1900);
		StepMotor_move_pos (1,9700);
		while(!StepMotor_pulse_is_end(2));
		Delay_ms(100);
		ServoMotor_set_ccr (2,301);
		Delay_ms(400);
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(2));
		ServoMotor_set_ccr (1,1110);
		Delay_ms(800);
		StepMotor_move_pos (1,8000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,10000);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==2)
	{
		ServoMotor_set_ccr (1,510);
		Delay_ms(800);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		StepMotor_move_pos (1,10500);
		if(count==3);
		else MCARM_MOV(1,300,1500);
		Delay_ms(200);
		ServoMotor_set_ccr (1,1110);
		Delay_ms(1050);
		StepMotor_move_pos (1,8000);
		if(count==3);
		else MCARM_MOV(2,300,1500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,10000);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==3)
	{
		ServoMotor_set_ccr (1,380);
		Delay_s(1);
		Delay_ms(250);
		StepMotor_move_pos (2,1900);
		StepMotor_move_pos (1,10000);
		while(!StepMotor_pulse_is_end(2));
		ServoMotor_set_ccr (2,301);
		Delay_ms(300);
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(2));
		if(count==3);
		else MCARM_MOV(1,600,1500);
		ServoMotor_set_ccr (1,1110);
		Delay_ms(1200);
		if(count==3);
		else MCARM_MOV(2,600,1500);
		StepMotor_move_pos (1,8000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==4)
	{
		ServoMotor_set_ccr (1,620);
		Delay_ms(300);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (1,1500);
		Delay_ms(500);
		MCARM_MOV(0,2250,1500);
		while(!McWheel_pulse_is_end(2));
		Delay_ms(50);
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		MCARM_MOV(3,2250,1500);
		Delay_ms(700);
		ServoMotor_set_ccr (1,700);
		StepMotor_reset(2);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(2));
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (1,1100);
		Delay_ms(800);
		StepMotor_move_pos (1,4000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,10500);
		return ;
	}
	else if(NUM==5)
	{
		ServoMotor_set_ccr (1,800);
		Delay_ms(300);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (1,1000);
		ServoMotor_set_ccr (1,510);
		Delay_ms(700);
		MCARM_MOV(0,1800,1500);
		while(!McWheel_pulse_is_end(2));
		Delay_ms(50);
		ServoMotor_set_ccr (2,301);
		Delay_ms(400);
		MCARM_MOV(3,1800,1500);
		Delay_ms(700);
		ServoMotor_set_ccr (1,800);
		StepMotor_reset(2);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(2));
		ServoMotor_set_ccr (1,1100);
		Delay_ms(750);
		StepMotor_move_pos (1,4000);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(50);
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,11000);
		return ;
	}
	else if(NUM==6)
	{
		ServoMotor_set_ccr (1,800);
		Delay_ms(300);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (2,5400);
		StepMotor_move_pos (1,1000);
		ServoMotor_set_ccr (1,380);
		Delay_ms(800);
		MCARM_MOV(0,2250,1500);
		while(!McWheel_pulse_is_end(2));
		Delay_ms(50);
		ServoMotor_set_ccr (2,301);
		Delay_ms(300);
		MCARM_MOV(3,2250,1500);
		Delay_ms(1000);
		ServoMotor_set_ccr (1,800);
		StepMotor_reset(2);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(2));
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (1,1100);
		Delay_ms(650);
		StepMotor_move_pos (1,4000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,10500);
		return ;
	}
	else if(NUM==8)
	{	
		StepMotor_move_pos (2,300);
		MCARM_MOV(1,500,1000);
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_move_pos (2,300);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,6500);
		MCARM_MOV(0,500,1000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		MCARM_MOV(1,500,1000);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,490);	
		Delay_ms(600);
		StepMotor_move_pos (2,7500);
		StepMotor_move_pos (1,6300);
		while(!StepMotor_pulse_is_end(2));
		MCARM_MOV(2,500,1500);
		StepMotor_move_pos (1,5300);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		
		if(count==3);
		else
		{
			StepMotor_move_pos (2,3000);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==7)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			StepMotor_move_pos (2,300);
			ServoMotor_set_ccr (1,1100);
			Delay_s(1);
		}
		StepMotor_move_pos (2,300);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(2));
		Delay_ms(200);
		StepMotor_move_pos (1,6000);
		while(!StepMotor_pulse_is_end(1));
		
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,360);	
		Delay_ms(800);
		StepMotor_move_pos (2,5400);
//		StepMotor_move_pos (1,5500);
//		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,1000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,4500);
		while(!StepMotor_pulse_is_end(1));
		StepMotor_move_pos (1,9500);
		
		if(count==3)
		{
			while(!StepMotor_pulse_is_end(1));
		}
		else
		{
			StepMotor_move_pos (2,3000);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==9)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1100);
		}
		StepMotor_move_pos (2,300);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(2));
		Delay_ms(200);
		StepMotor_move_pos (1,6500);
		MCARM_MOV(2,300,1000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,640);	
		Delay_ms(300);
		StepMotor_move_pos (2,5800);
		StepMotor_move_pos (2,5800);
//		StepMotor_move_pos (1,5500);
//		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,2000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		MCARM_MOV(3,300,1000);
		StepMotor_move_pos (1,4500);
		while(!StepMotor_pulse_is_end(1));
		StepMotor_move_pos (1,9500);
		
		if(count==3)
		{
			while(!StepMotor_pulse_is_end(1));
			Delay_ms(200);
		}
		else
		{
			StepMotor_move_pos (2,3000);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==11)
	{
		MCARM_MOV(3,500,1000);
		ServoMotor_set_ccr (1,505);	
		if (count==1)Delay_ms(400);
		else Delay_s(1);
		StepMotor_move_pos (2,7500);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(1));
		MCARM_MOV(0,500,1000);
		ServoMotor_set_ccr (2,301);
		Delay_ms(400);
		MCARM_MOV(1,300,1000);
		StepMotor_move_pos (1,7500);
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(1));
		StepMotor_move_pos (1,10000);
		ServoMotor_set_ccr (1,800);
		while(!StepMotor_pulse_is_end(2));
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (1,1100);
		Delay_ms(800);
		MCARM_MOV(2,300,1000);
		StepMotor_move_pos(1,8000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==10)
	{
		ServoMotor_set_ccr (1,360);	
		if (count==1)Delay_ms(400);
		else Delay_s(1);
		StepMotor_move_pos (2,5600);
		StepMotor_move_pos (1,4600);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,2000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(400);
//		MCARM_MOV(3,300,1000);
		StepMotor_move_pos (1,7500);
		StepMotor_reset(2);
		while(!StepMotor_pulse_is_end(1));
		StepMotor_move_pos (1,10500);
		ServoMotor_set_ccr (1,800);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (1,1100);
		Delay_ms(500);
//		MCARM_MOV(2,300,1000);
		StepMotor_move_pos (1,8000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==12)
	{
		ServoMotor_set_ccr (1,655);	
		if (count==1)Delay_ms(400);
		else Delay_s(1);
		StepMotor_move_pos (2,5800);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,2200);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(400);
//		MCARM_MOV(3,300,1000);
		StepMotor_move_pos (1,10500);
		StepMotor_move_pos (2,1500);
		while(!StepMotor_pulse_is_end(1));
		while(!StepMotor_pulse_is_end(2));
		StepMotor_reset(2);
		ServoMotor_set_ccr (1,1100);
		Delay_ms(550);
//		MCARM_MOV(2,300,1000);
		StepMotor_move_pos (1,8000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(1));
		}
		return ;
	}
	else if(NUM==14)
	{
		MCARM_MOV(3,300,1000);
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_reset(2);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,200);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,485);	
		Delay_ms(850);
		StepMotor_move_pos (2,6500);
		StepMotor_move_pos (2,6500);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,8500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		MCARM_MOV(3,300,1500);
		StepMotor_move_pos (1,10500);
		while(!StepMotor_pulse_is_end(1));
		MCARM_MOV(2,600,1000);
		StepMotor_move_pos (2,3000);
		if(count==3);
		else
		{
			StepMotor_move_pos (1,9500);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==13)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_reset(2);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,200);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,350);	
		Delay_s(1);
		StepMotor_move_pos (2,4800);
		StepMotor_move_pos (2,4800);
		StepMotor_move_pos (1,4300);
		while(!StepMotor_pulse_is_end(2));
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		if(count==3);
		else
		{
			StepMotor_move_pos (1,9500);
			StepMotor_move_pos (2,3000);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==15)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_reset(2);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,301);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,630);	
		Delay_ms(700);
		StepMotor_move_pos (2,5000);
		StepMotor_move_pos (2,5000);
		StepMotor_move_pos (1,5500);
		while(!StepMotor_pulse_is_end(2));
		while(!StepMotor_pulse_is_end(1));
		StepMotor_move_pos (1,5000);
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,7000);
		while(!StepMotor_pulse_is_end(1));
		if(count==3);
		else
		{
			StepMotor_move_pos (1,9500);
			StepMotor_move_pos (2,3000);
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==17)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_reset(2);
		
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,200);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,495);	
		Delay_s(1);
		StepMotor_move_pos (1,1000);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		if(count==3);
		else
		{
			Delay_ms(600);
		}
		return ;
	}
	else if(NUM==16)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			Delay_ms(1300);
			StepMotor_move_pos (2,300);
			Delay_ms(150);
		}
		StepMotor_reset(2);
		Delay_ms(400);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,200);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,350);	
		Delay_s(1);
		StepMotor_move_pos (2,2000);
		StepMotor_move_pos (2,2000);
		while(!StepMotor_pulse_is_end(2));
		StepMotor_move_pos (1,5100);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		Delay_ms(400);
		StepMotor_reset(2);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
	else if(NUM==18)
	{
		if (count==1)Delay_ms(10);
		else 
		{
			ServoMotor_set_ccr (1,1100);
			StepMotor_move_pos (2,300);
			Delay_ms(1200);
		}
		StepMotor_reset(2);
		StepMotor_move_pos (1,6500);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,200);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		while(!StepMotor_pulse_is_end(1));
		Delay_ms(200);
		ServoMotor_set_ccr (1,635);	
		Delay_s(1);
		StepMotor_move_pos (2,2000);
		StepMotor_move_pos (2,2000);
		StepMotor_move_pos (1,5100);
		while(!StepMotor_pulse_is_end(1));
		ServoMotor_set_ccr (2,630);
		Delay_ms(200);
		StepMotor_move_pos (1,9500);
		Delay_ms(400);
		StepMotor_reset(2);
		if(count==3);
		else
		{
			while(!StepMotor_pulse_is_end(2));
		}
		return ;
	}
}
