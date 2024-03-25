#ifndef __FUNCTION_H__
#define __FUNCTION_H__


#include "sys.h"
#include "math.h"


//PID
//�⼸�������������±�ţ����㿴
#define KP 0
#define KI 1
#define KD 2
#define KT 3   //�����޷���


typedef struct PID   	// ����PID�����������
{
	float SumError;			// ����ۼ�	
	int LastError;		// �ϴ����
	int PrevError;		// Ԥ�����	
	int LastData;			// �ϴ�����
} PID;




extern PID ShotPid;
extern float Shot_Pid[][5];
extern u8 Shot_Sequence;


int PlacePID_Control(PID *sprt, float *PID, int error);
void PID_Parameter_Init(PID *sptr);											
int range_protect(int Stp, int min, int max);								//�޷�����
int my_abs(int x);




#endif 
