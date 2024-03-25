#include "usmart.h"
#include "usmart_str.h" 
#include "StepMotor.h"
#include "ServoMotor.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"		
#include "sys.h"
#include "led.h"
												 
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
 
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",	
//	(void*)LCD_Clear,"void LCD_Clear(u16 Color)",
//	(void*)LCD_Fill,"void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)",
//	(void*)LCD_DrawLine,"void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)",
//	(void*)LCD_DrawRectangle,"void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)",
//	(void*)LCD_Draw_Circle,"void Draw_Circle(u16 x0,u16 y0,u8 r)",
//	(void*)LCD_ShowNum,"void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)",
//	(void*)LCD_ShowString,"void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)",			  	    
//	(void*)LCD_ReadPoint,"u16 LCD_ReadPoint(u16 x,u16 y)",
//	(void*)LCD_ShowChar,"void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)",
	(void*)LED1_CTRL,"void LED1_CTRL(u8 on_or_off)",
	(void*)StepMotor_generate_freq_pulse,"StepMotor_generate_freq_pulse(uint8_t Step_motor_num,uint8_t direction,uint32_t freq,uint16_t pulse_num)",
	(void*)StepMotor_generate_pulse,"StepMotor_generate_pulse(uint8_t Step_motor_num,uint8_t direction,uint16_t pulse_num)",
	(uint8_t*)StepMotor_pulse_is_end,"StepMotor_pulse_is_end(uint8_t step_motor_num)",
	(void*)StepMotor_stop,"StepMotor_stop(uint8_t step_motor_num)",
	(void*) StepMotor_reset,"StepMotor_reset(uint8_t Step_motor_num)",
	(uint32_t*) StepMotor_move_pos,"StepMotor_move_pos(uint8_t Step_motor_num,uint32_t position)",
	(void*) ServoMotor_set_ccr,"ServoMotor_set_ccr(uint8_t servo_motor_num,uint16_t ccr)",

};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















