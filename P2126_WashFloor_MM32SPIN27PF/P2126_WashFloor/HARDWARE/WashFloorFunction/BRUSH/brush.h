#ifndef _BRUSH_H_
#define _BRUSH_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "timer.h"
#include "adc.h"

#include "config.h"

//#define Brush_R       50//100

#define BRUSH_PWM               TIM2->CCR4

#define BRUSH_FULL_PWM          BRUSH_PERIOD

#define BRUSH_SLOW_PWM          (BRUSH_FULL_PWM/4)//750//1000 //1800//1500//1000 // 500
#define BRUSH_SLOW_PWM1         (BRUSH_FULL_PWM*3/20)//(BRUSH_FULL_PWM*9/50)//(BRUSH_FULL_PWM/10)//(BRUSH_FULL_PWM/5)//(BRUSH_FULL_PWM*3/20)//750//1000 //1800//1500//1000 // 500
#define BRUSH_SLOW_PWM2         (BRUSH_FULL_PWM/10)//750//1000 //1800//1500//1000 // 500

#define	BRUSH_REF_VOLT1			18140//18520//18140
#define	BRUSH_REF_VOLT2			18140//24140//18520//18140


#define	BRUSH_REF_SOFT_VOLT1			3000//3000//2500//3000//2500//3000//18520//18140
#define	BRUSH_REF_SOFT_VOLT2			3500//18520//18140
#define	BRUSH_REF_SOFT_VOLT3			4000//18520//18140


// t0机器 未安装30 安装悬空 70
#define NO_BRUSH_I              700//800//450// 未安装AD 11(11*25==275ma) 24.6V 自清洁安装AD18~20
#define STALL_BRUSH_I           4000//2500
#define OC1_BRUSH_I             5000//2500
#define OC2_BRUSH_I             8000//2500
#define LIMIT_BRUSH_I           5000//7000//5000
#define SHORT_BRUSH_I           60000 

// ma
#if Brush_R==100

#define NO_BRUSH_VREF			(NO_BRUSH_I*31/250)//(NO_BRUSH_I*2/25)//(NO_Brush_I*0.05*82/100)//(NO_Brush_I*0.05*4096/(5*1000)) //35//60			//缺地刷判断值
#define STALL_BRUSH_VREF		(STALL_BRUSH_I*31/250)//(STALL_BRUSH_I*2/25)//(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define OC1_BRUSH_VREF		    (OC1_BRUSH_I*31/250)//(OC1_BRUSH_I*2/25)//(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define OC2_BRUSH_VREF		    (OC2_BRUSH_I*31/250)//(OC2_BRUSH_I*2/25)//(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define LIMIT_BRUSH_VREF		(LIMIT_BRUSH_I*31/250)//(LIMIT_BRUSH_I*2/25)//(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define SHORT_BRUSH_VREF		(SHORT_BRUSH_I*31/250)//(SHORT_BRUSH_I*2/25)//(SHORT_BRUSH_I*0.05*82/100)//(SHORT_BRUSH_I*0.05*4096/(5*1000))//2458//地刷短路判断值

#else


#define NO_BRUSH_VREF			(NO_BRUSH_I*31/500)////(NO_Brush_I*0.05*82/100)//(NO_Brush_I*0.05*4096/(5*1000)) //35//60			//缺地刷判断值
#define STALL_BRUSH_VREF		(STALL_BRUSH_I*31/500)////(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define OC1_BRUSH_VREF		    (OC1_BRUSH_I*31/500)////(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define OC2_BRUSH_VREF		    (OC2_BRUSH_I*31/500)////(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define LIMIT_BRUSH_VREF		(LIMIT_BRUSH_I*31/500)////(STALL_BRUSH_I*0.05*82/100)//(STALL_BRUSH_I*0.05*4096/(5*1000))//185//205//200//100//200			//地刷堵转判断值
#define SHORT_BRUSH_VREF		(SHORT_BRUSH_I*31/500)////(SHORT_BRUSH_I*0.05*82/100)//(SHORT_BRUSH_I*0.05*4096/(5*1000))//2458//地刷短路判断值
#endif

#define BRUSH_TICKS_INTERVAL     10

#define NO_BRUSH_TIME			(1500/BRUSH_TICKS_INTERVAL)		//缺地刷检测时间
#define STALL_BRUSH_TIME		(2000/BRUSH_TICKS_INTERVAL)		//地刷堵转检测时间
#define OC1_BRUSH_TIME		    (500/BRUSH_TICKS_INTERVAL)		//地刷堵转检测时间
#define OC2_BRUSH_TIME		    (50/BRUSH_TICKS_INTERVAL)		//地刷堵转检测时间
#define BRUSH_OK_TIME0	    	(1000/BRUSH_TICKS_INTERVAL)		//地刷正常工作时间
#define BRUSH_OK_TIME	    	(4000/BRUSH_TICKS_INTERVAL)		//地刷正常工作时间
#define BRUSH_CHECK_TIME	    (5000/BRUSH_TICKS_INTERVAL)		//地刷正常工作时间


typedef enum{
	BRUSH_YES_STA=0x01,
	BRUSH_NO_STA,
	BRUSH_STALL_STA
}BrushSta;

typedef struct{
	uint8_t  bEnalbeBrushFlag:1;
	uint8_t  bEnalbeSlowBrushFlag:1;
	uint8_t  bEnalbeNGBrushFlag:1;// 20211105 自清洁不检测滚刷未安装标志
	uint8_t  bBrushStallErrFlag:1;
	uint8_t  bBrushShortErrFlag:1;
	uint8_t  bBrushNGErrFlag:1;
	uint8_t  bBrushResetFlag:1;
	uint16_t uiBrushSlowMode;
	uint16_t uiBrushNGCnt;
	uint16_t uiBrushStallCnt;
	uint16_t uiBrushOC1Cnt;
	uint16_t uiBrushOC2Cnt;
	uint32_t ulBrushVolt;
	uint32_t ulBrushDuty;
	uint32_t ulBrushVoltDuty;
	uint32_t ulBRUSH_REF_VOLT;
	uint32_t ulBrushSetVoltDuty;
	uint32_t ulBrushONCnt;
	uint32_t ulBrushAdSUM;// 20211123
	uint8_t ulBrushshortCnt;
	uint8_t ulBrushshortperiod;
	uint8_t ulBrushshorttime;
}BrushPara;

void Set_Brush_Normal(BrushPara* brushparactl);
void Set_Brush_Slow(BrushPara* brushparactl);
void Set_Brush_OFF(BrushPara* brushparactl);
void Deal_BrushDuty(uint32_t batVolt,BrushPara* brushparactl);
void Control_Brush(BrushPara* brushparactl,ADCPara* adcparactl);
void Test_Control_Brush(BrushPara* brushparactl,ADCPara* adcparactl);

#endif

