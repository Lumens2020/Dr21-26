#ifndef _KEYAD_H_
#define _KEYAD_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include <string.h>

#include "key.h"

#define AD_TICKS_INTERVAL         1	                      // ����ɨ��ʱ��
#define AD_DEBOUNCE_TICKS         15                      // ����ֵ������	
#define AD_SHORT_TICKS            (300/AD_TICKS_INTERVAL) // �̰�ʱ������
#define AD_LONG1_TICKS            (3000/AD_TICKS_INTERVAL)// ����ʱ��1����
#define AD_LONG2_TICKS            (4000/AD_TICKS_INTERVAL)// ����ʱ��2����
#define AD_HOLD_TICKS             (10/AD_TICKS_INTERVAL)  // �����ɿ�ʱ������

/////////////////////////AD����///////////////////////////////////////////
// ���������궨��ģ��     unit:1ms
// ��λ����AD   3244
// ����ఴ��AD 117
#define KEY_NOT_DOWN_AD							   3800//3200			//����δ�����жϵ�ADֵ
#define CLEAN_KEY_AD							   1000         //����ఴ�������ж�ADֵ 
#define CHANGE_GEARS_AD							   2500			//�������������ж�ADֵ  
#define LONG_PRESS_BUTTON_COUNT_TIME_AD			   65000		//����������ʱʱ��65s
#define LONG_PRESS_COUNT_TIME_LIMIT_AD			   60000		//����������ʱ������60s


typedef struct ADButton{
	u16 ticks;
	u8  repeat: 4;
	u8  event : 4;
	u8  state : 3;
	u8  debounce_cnt; 
	u16  active_level_upper;
	u16  active_level_lower;
	u16  button_level;
	u16  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct ADButton* next;
} ADButton;



void ADbutton_attach(struct ADButton* handle, PressEvent event, BtnCallback cb);
void ADbutton_handler(struct ADButton* handle);
void ADbutton_init(struct ADButton* handle,uint16_t(*pin_level)(),uint16_t active_level_lower,uint16_t active_level_upper);
PressEvent get_adbutton_event(struct ADButton* handle);
int ADbutton_start(struct ADButton* handle);
void ADbutton_stop(struct ADButton* handle);
uint16_t read_button1_AD(void);
void ADbutton1_callback(void* adbutton);
void ADbutton2_callback(void* adbutton);
void ADbutton3_callback(void* adbutton);
void ADbutton_ticks(void);
#endif

