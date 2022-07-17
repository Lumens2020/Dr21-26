#ifndef _CHARGE_H_
#define _CHARGE_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "adc.h"
#include "config.h"

//#define CHG_AD_ENABLE 1

#define CHG_TICKS_INTERVAL      10
#define CHARGE_TIME				(200/CHG_TICKS_INTERVAL)			// 100ms			//≥‰µÁºÏ≤‚ ±º‰


typedef struct{
	uint8_t    bChargingFlag:1;
	uint8_t    uiCHGCnt;
}CHGPara;
void Check_Charge(CHGPara* chgparactl,ADCPara* advalue);

#endif

