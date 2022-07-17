#ifndef _CLEANWATER_H_
#define _CLEANWATER_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "adc.h"


#define WATERLV_TICKS_INTERVAL  10

#define WATERLEVEL_TIME		    (2000/WATERLV_TICKS_INTERVAL)	//  6 means 600ms			//水位检测时间

//#define WATERLV_CHECKAD         2000// 1.6V 厂商建议，0、8~1.2V水管有水珠 设置大于0.6V判断无水
#define WATERLV_CHECKAD           744// 1.6V 厂商建议，0、8~1.2V水管有水珠 设置大于0.6V判断无水

typedef enum{
  QBULET_FULL_STA=0x01,
  QBULET_BLANK_STA	
}CleanBoxSta;

typedef struct{
	uint8_t bSewageStageEnableFlag:1;
	uint8_t bWaterLVEnableFlag:1;// 检测清水箱时开启水泵标志，检测前标志置1开启水泵，水泵开启时间到了标志置0开始检测清水箱
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

