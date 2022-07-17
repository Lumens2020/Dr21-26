#ifndef _KEYAD_H_
#define _KEYAD_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include <string.h>

#include "key.h"

#define AD_TICKS_INTERVAL         1	                      // 函数扫描时间
#define AD_DEBOUNCE_TICKS         15                      // 按键值检测次数	
#define AD_SHORT_TICKS            (300/AD_TICKS_INTERVAL) // 短按时间设置
#define AD_LONG1_TICKS            (3000/AD_TICKS_INTERVAL)// 长按时间1设置
#define AD_LONG2_TICKS            (4000/AD_TICKS_INTERVAL)// 长按时间2设置
#define AD_HOLD_TICKS             (10/AD_TICKS_INTERVAL)  // 按键松开时间设置

/////////////////////////AD按键///////////////////////////////////////////
// 按键消抖宏定义模块     unit:1ms
// 挡位按键AD   3244
// 自清洁按键AD 117
#define KEY_NOT_DOWN_AD							   3800//3200			//按键未按下判断的AD值
#define CLEAN_KEY_AD							   1000         //自清洁按键按下判断AD值 
#define CHANGE_GEARS_AD							   2500			//调档按键按下判断AD值  
#define LONG_PRESS_BUTTON_COUNT_TIME_AD			   65000		//长按按键计时时间65s
#define LONG_PRESS_COUNT_TIME_LIMIT_AD			   60000		//长按按键计时间上限60s


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

