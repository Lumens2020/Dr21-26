#ifndef _TILT_H_
#define _TILT_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define TILT_TICKS_INTERVAL      10

#define TILT_TIME				(200/TILT_TICKS_INTERVAL)			// 100ms			//ÉAµπºÏ≤‚ ±º‰

typedef struct{
	uint8_t    bTiltEnableFlag:1;
	uint8_t    bTiltEnablebackFlag:1;
	uint8_t    uiTiltCnt;
}TILTPara;
void Check_Tilt(TILTPara* tiltcheck);

#endif

