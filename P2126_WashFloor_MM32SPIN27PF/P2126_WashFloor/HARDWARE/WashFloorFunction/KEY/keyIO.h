#ifndef _KEYIO_H_
#define _KEYIO_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include <string.h>

#include "key.h"


#define TICKS_INTERVAL         1	                // 按键处理函数扫描时间设置
#define DEBOUNCE_TICKS         15	                // 按键值扫描次数
#define SHORT_TICKS            (300/TICKS_INTERVAL) // 短按按键时间设置(连续按键时统计按键次数使用)
#define LONG1_TICKS            (1000/TICKS_INTERVAL)// 长按按键时间设置
#define LONG2_TICKS            (3000/TICKS_INTERVAL)// 长按按键时间设置
#define HOLD_RELESE_TICKS      (10000/TICKS_INTERVAL)//(20000/TICKS_INTERVAL)// 松开按键时间设置

typedef struct Button{
	u16 ticks;
	u16 relese_ticks;
	u8  repeat: 4;
	u8  event : 4;
	u8  state : 3;
	u8  debounce_cnt; 
	u8  active_level : 1;
	u8  button_level : 1;
	u8  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
void button_handler(struct Button* handle);
void button_init(struct Button* handle,uint8_t(*pin_level)(),uint8_t active_level);
PressEvent get_button_event(struct Button* handle);
int button_start(struct Button* handle);
void button_stop(struct Button* handle);
uint8_t read_button1_GPIO(void);
void button1_callback(void* button);
void button_ticks(void);


#endif

