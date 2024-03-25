#include "function.h"


// 打靶PID结构体
PID ShotPid;
float Shot_Pid[][5] = { 	{50,0.1,0.05,100},       // 最后一项为积分限幅 
							{7,0.1,0,100}
                      };
u8 Shot_Sequence = 0;


// ********************位置式动态PID控制（舵机PID）************************************
/*
函数：int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
功能：位置式动态PID控制
参数：
PID *sprt：      结构体指针
float *PID：     PID数组  （通过数组定义PID值）
int32 NowPiont： 当前值  （可使用结构体定义变量）
int32 SetPoint： 设定目标值   转向控制中设定值为0。

说明：  该函数参考其他程序。动态控制一般用于转向控制
返回值： int32 Realize
eg：Radius = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);// 动态PID控制转向
***************************************************************************/
// 位置式动态PID控制
int PlacePID_Control(PID *sprt, float *PID, int error)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int iError,								//当前误差
		  Actual;							//最后得出的实际输出值
	float Kp;								//动态P
	iError = error;							//计算当前误差
	sprt->SumError += iError*0.01;			//
	
	// 积分限幅
	if (sprt->SumError >= PID[KT])
		sprt->SumError = PID[KT];

	else if (sprt->SumError <= -PID[KT])
		sprt->SumError = -PID[KT];
	
    // 动态KP
	Kp = 1.0 * (iError*iError) / PID[KP] + PID[KI];										//P值与差值成二次函数关系，此处P和I不是PID参数，而是动态PID参数，要注意！！！
	
	Actual = Kp * iError 
		   + PID[KD] * ((0.8*iError + 0.2 * sprt->LastError) - sprt->LastError);			//只用PD
	sprt->LastError = iError;		//更新上次误差

	//Actual += sprt->SumError*0.1;
	//Actual = limit(Actual, 300); //限幅
	return Actual;
}

 /******** 限幅保护 *********/
int range_protect(int Stp, int min, int max)//限幅保护
{
	if (Stp >= max)
	{
		return max;
	}
	if (Stp <= min)
	{
		return min;
	}
	else
	{
		return Stp;
	}
}


int my_abs(int x) 
{
    if (x < 0) 
        return -x;
    else 
        return x;
}


// PID参数初始化
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0;
}
