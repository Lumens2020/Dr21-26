#ifndef _KEY_H_
#define _KEY_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"


#define LONG_PRESS_TIME_1S							500      //unit:2ms 长按时间1s
#define LONG_PRESS_TIME_2S							1000      //unit:2ms 长按时间2s
#define LONG_PRESS_TIME_3S							1500      //unit:2ms 长按时间3s
#define LONG_PRESS_TIME_4S							2000      //unit:2ms 长按时间4s
#define LONG_PRESS_TIME_5S							2500      //unit:2ms 长按时间5s
#define LONG_PRESS_TIME_6S							3000      //unit:2ms 长按时间6s
#define LONG_PRESS_TIME_7S							3500      //unit:2ms 长按时间7s
#define LONG_PRESS_TIME_8S							4000      //unit:2ms 长按时间8s
#define LONG_PRESS_TIME_9S							4500      //unit:2ms 长按时间9s
#define LONG_PRESS_TIME_10S							5000      //unit:2ms 长按时间10s
#define LONG_PRESS_TIME_11S							5500      //unit:2ms 长按时间11s
#define LONG_PRESS_TIME_12S							6000      //unit:2ms 长按时间12s

#define KEY_LESS_NOT    							100					//无效状态赋值参数

//长按的十二种状态
#define LONG_PRESS_STATE1							     1             //对应长按1状态
#define LONG_PRESS_STATE2							     2             //对应长按2状态
#define LONG_PRESS_STATE3							     3             //对应长按3状态
#define LONG_PRESS_STATE4							     4             //对应长按4状态
#define LONG_PRESS_STATE5								 5             //对应长按5状态
#define LONG_PRESS_STATE6								 6             //对应长按6状态
#define LONG_PRESS_STATE7								 7             //对应长按7状态
#define LONG_PRESS_STATE8								 8             //对应长按8状态
#define LONG_PRESS_STATE9								 9             //对应长按9状态
#define LONG_PRESS_STATE10								10             //对应长按10状态
#define LONG_PRESS_STATE11								11            //对应长按11状态
#define LONG_PRESS_STATE12								12            //对应长按12状态

#define KEY_CLOSE_TIME			  25			//unit:100ms    2.5S
#define KEY_AUTO_TIME					50			//unit:100ms    5S
#define KEY_DRY_TIME					300			//unit:100ms    10S

typedef void (*BtnCallback)(void*);


typedef enum{
	PRESS_DOWN=0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS1_START,
	LONG_PRESS2_START,
	LONG_PRESS_HOLD,
	RELESE_PRESS_10S,
	number_of_event,
	NONE_PRESS
} PressEvent;

typedef struct{
	uint8_t bFastVoiceFlag:1;
	uint8_t bVoiceFlag:1;
	uint8_t bGearKeyPressFlag:1;// AD挡位按键按下标志
	uint8_t bCleanKeyPressFlag:1;// AD自清洁按键按下标志
	uint8_t bADFirstFlag:1;// 
	uint8_t ucMainKeyStatusValue;
	uint8_t	ucKeyGearValue; // 挡位
	uint8_t	ucKeyGearValue_back; // 挡位 0 自动 1 杀菌 2吸水
	uint16_t ucKeyGearStayCnt; // 挡位切换缓存时间
	uint8_t	ucKeyStatus;
	uint8_t	ucKeyWakeUpcnt;
	uint8_t	ucKeyWakeUpcnt1;
}KeyPara;

void KEY_VOICE_PRESS_UP(void);
void KEY_VOICE_LONG_PRESS(void);
void KEY_VOICE_RELESE_10S(void);
void ADKEY_CLEAN_PRESS_UP(void);
void ADKEY_CLEAN_LONG_PRESS(void);
void ADKEY_MODE_PRESS_UP(void);
void ADKEY_RELESE(void);
void KEY_Init(void);

#endif

