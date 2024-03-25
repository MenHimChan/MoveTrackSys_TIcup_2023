#ifndef __FUNCTION_H__
#define __FUNCTION_H__


#include "sys.h"
#include "math.h"


//PID
//这几个类似于数组下标号，方便看
#define KP 0
#define KI 1
#define KD 2
#define KT 3   //积分限幅项


typedef struct PID   	// 用来PID参数计算变量
{
	float SumError;			// 误差累计	
	int LastError;		// 上次误差
	int PrevError;		// 预测误差	
	int LastData;			// 上次数据
} PID;




extern PID ShotPid;
extern float Shot_Pid[][5];
extern u8 Shot_Sequence;


int PlacePID_Control(PID *sprt, float *PID, int error);
void PID_Parameter_Init(PID *sptr);											
int range_protect(int Stp, int min, int max);								//限幅保护
int my_abs(int x);




#endif 
