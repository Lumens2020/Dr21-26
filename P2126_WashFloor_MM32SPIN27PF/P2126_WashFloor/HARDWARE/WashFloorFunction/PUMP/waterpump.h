#ifndef _WATERPUMP_H_
#define _WATERPUMP_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "cleanwater.h"
#include "brush.h"
#include "control.h"

#define WaterPump_PWM     TIM3->CCR4
#define WaterPump_PERIOD  TIM3->ARR


// ma
#define SHORT_PUMP_I           60000 
#define SHORT_PUMP_VREF		   (SHORT_PUMP_I/25)		//水泵短路判断值

#define WATER_TICKS_INTERVAL     10
#define WATER_DELAY_TIME	    (8000/WATER_TICKS_INTERVAL)// 20211012//(3000/WATER_TICKS_INTERVAL)		

typedef struct{
	uint8_t     bEnalbePumpFlag:1;
	uint8_t     bPumpShortErrFlag:1;
	uint8_t     bPumpResetFlag:1;
	uint8_t     ucPumpGearValue;// 水泵流量挡位
	uint8_t     ucPumpGearCHGFlag1;// 水泵流量调档标志
	uint8_t     ucPumpGearCHGFlag2;// 水泵流量调档标志
	uint8_t     ucPumpGearCHGFlag3;// 水泵流量调档标志
	uint16_t    uiPumpVolt;
	uint32_t	ulPumpDuty;
	uint32_t	ulPumpFreq;
	uint16_t    uiEnableQWaterLCnt;
	uint8_t ulPumpshortCnt;
	uint8_t ulPumpshortperiod;
	uint8_t ulPumpshorttime;
}PumpPara;


void Set_WaterPump_ON(PumpPara* pumpparactl);
void Set_WaterPump_OFF(PumpPara* pumpparactl);
void Deal_PumpDuty(uint32_t batVolt,PumpPara* pumpparactl);
//void Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl);
void Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl,WorkPara* workstate);
void Test_Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl,WorkPara* workstate);

#endif


