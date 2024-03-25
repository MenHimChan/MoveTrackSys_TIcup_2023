#include "function.h"


// ���PID�ṹ��
PID ShotPid;
float Shot_Pid[][5] = { 	{50,0.1,0.05,100},       // ���һ��Ϊ�����޷� 
							{7,0.1,0,100}
                      };
u8 Shot_Sequence = 0;


// ********************λ��ʽ��̬PID���ƣ����PID��************************************
/*
������int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
���ܣ�λ��ʽ��̬PID����
������
PID *sprt��      �ṹ��ָ��
float *PID��     PID����  ��ͨ�����鶨��PIDֵ��
int32 NowPiont�� ��ǰֵ  ����ʹ�ýṹ�嶨�������
int32 SetPoint�� �趨Ŀ��ֵ   ת��������趨ֵΪ0��

˵����  �ú����ο��������򡣶�̬����һ������ת�����
����ֵ�� int32 Realize
eg��Radius = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);// ��̬PID����ת��
***************************************************************************/
// λ��ʽ��̬PID����
int PlacePID_Control(PID *sprt, float *PID, int error)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int iError,								//��ǰ���
		  Actual;							//���ó���ʵ�����ֵ
	float Kp;								//��̬P
	iError = error;							//���㵱ǰ���
	sprt->SumError += iError*0.01;			//
	
	// �����޷�
	if (sprt->SumError >= PID[KT])
		sprt->SumError = PID[KT];

	else if (sprt->SumError <= -PID[KT])
		sprt->SumError = -PID[KT];
	
    // ��̬KP
	Kp = 1.0 * (iError*iError) / PID[KP] + PID[KI];										//Pֵ���ֵ�ɶ��κ�����ϵ���˴�P��I����PID���������Ƕ�̬PID������Ҫע�⣡����
	
	Actual = Kp * iError 
		   + PID[KD] * ((0.8*iError + 0.2 * sprt->LastError) - sprt->LastError);			//ֻ��PD
	sprt->LastError = iError;		//�����ϴ����

	//Actual += sprt->SumError*0.1;
	//Actual = limit(Actual, 300); //�޷�
	return Actual;
}

 /******** �޷����� *********/
int range_protect(int Stp, int min, int max)//�޷�����
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


// PID������ʼ��
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0;
}
