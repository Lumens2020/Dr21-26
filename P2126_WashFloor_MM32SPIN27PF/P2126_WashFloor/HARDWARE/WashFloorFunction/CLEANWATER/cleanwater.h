#ifndef _CLEANWATER_H_
#define _CLEANWATER_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "adc.h"


#define WATERLV_TICKS_INTERVAL  10

#define WATERLEVEL_TIME		    (2000/WATERLV_TICKS_INTERVAL)	//  6 means 600ms			//ˮλ���ʱ��

//#define WATERLV_CHECKAD         2000// 1.6V ���̽��飬0��8~1.2Vˮ����ˮ�� ���ô���0.6V�ж���ˮ
#define WATERLV_CHECKAD           744// 1.6V ���̽��飬0��8~1.2Vˮ����ˮ�� ���ô���0.6V�ж���ˮ

typedef enum{
  QBULET_FULL_STA=0x01,
  QBULET_BLANK_STA	
}CleanBoxSta;

typedef struct{
	uint8_t bSewageStageEnableFlag:1;
	uint8_t bWaterLVEnableFlag:1;// �����ˮ��ʱ����ˮ�ñ�־�����ǰ��־��1����ˮ�ã�ˮ�ÿ���ʱ�䵽�˱�־��0��ʼ�����ˮ��
	uint8_t ucQBucketStatus;
	uint8_t ucbackQBucketStatus;
	uint8_t	ucWBucketStatus;
	uint8_t	ucbackWBucketStatus;
	uint8_t uiWaterLevelCnt;
	uint8_t uiWaterLevelRecCnt;
	uint8_t uiSewageStageCnt;
	uint8_t uiSewageStageyUsCnt;
	uint8_t uiSewageStageDetCnt0;
	uint8_t uiSewageStageDetCnt1;
}WaterBoxStatus;

void Check_WaterBox(WaterBoxStatus* waterboxctl,ADCPara* advalue);

#endif

