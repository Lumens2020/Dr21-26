#ifndef _KEY_H_
#define _KEY_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"


#define LONG_PRESS_TIME_1S							500      //unit:2ms ����ʱ��1s
#define LONG_PRESS_TIME_2S							1000      //unit:2ms ����ʱ��2s
#define LONG_PRESS_TIME_3S							1500      //unit:2ms ����ʱ��3s
#define LONG_PRESS_TIME_4S							2000      //unit:2ms ����ʱ��4s
#define LONG_PRESS_TIME_5S							2500      //unit:2ms ����ʱ��5s
#define LONG_PRESS_TIME_6S							3000      //unit:2ms ����ʱ��6s
#define LONG_PRESS_TIME_7S							3500      //unit:2ms ����ʱ��7s
#define LONG_PRESS_TIME_8S							4000      //unit:2ms ����ʱ��8s
#define LONG_PRESS_TIME_9S							4500      //unit:2ms ����ʱ��9s
#define LONG_PRESS_TIME_10S							5000      //unit:2ms ����ʱ��10s
#define LONG_PRESS_TIME_11S							5500      //unit:2ms ����ʱ��11s
#define LONG_PRESS_TIME_12S							6000      //unit:2ms ����ʱ��12s

#define KEY_LESS_NOT    							100					//��Ч״̬��ֵ����

//������ʮ����״̬
#define LONG_PRESS_STATE1							     1             //��Ӧ����1״̬
#define LONG_PRESS_STATE2							     2             //��Ӧ����2״̬
#define LONG_PRESS_STATE3							     3             //��Ӧ����3״̬
#define LONG_PRESS_STATE4							     4             //��Ӧ����4״̬
#define LONG_PRESS_STATE5								 5             //��Ӧ����5״̬
#define LONG_PRESS_STATE6								 6             //��Ӧ����6״̬
#define LONG_PRESS_STATE7								 7             //��Ӧ����7״̬
#define LONG_PRESS_STATE8								 8             //��Ӧ����8״̬
#define LONG_PRESS_STATE9								 9             //��Ӧ����9״̬
#define LONG_PRESS_STATE10								10             //��Ӧ����10״̬
#define LONG_PRESS_STATE11								11            //��Ӧ����11״̬
#define LONG_PRESS_STATE12								12            //��Ӧ����12״̬

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
	uint8_t bGearKeyPressFlag:1;// AD��λ�������±�־
	uint8_t bCleanKeyPressFlag:1;// AD����ఴ�����±�־
	uint8_t bADFirstFlag:1;// 
	uint8_t ucMainKeyStatusValue;
	uint8_t	ucKeyGearValue; // ��λ
	uint8_t	ucKeyGearValue_back; // ��λ 0 �Զ� 1 ɱ�� 2��ˮ
	uint16_t ucKeyGearStayCnt; // ��λ�л�����ʱ��
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

