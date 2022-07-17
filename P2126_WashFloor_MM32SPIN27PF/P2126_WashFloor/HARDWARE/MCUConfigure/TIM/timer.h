#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define SYSYTEM_CLOCK    		48000000        //unit:Hz

//#define BRUSH_2K      11
//#define BRUSH_5K        3
//#define BRUSH_6_8K    3
#define BRUSH_10K       1
//#define BRUSH_20K         1
//#define BRUSH_15K       1

#define BRUSH_FREQ                 BRUSH_10K//BRUSH_20K//BRUSH_10K// BRUSH_5K

#define TIM1_PRESCAL				1  //47
#define TIM2_PRESCAL				BRUSH_FREQ//5//3//19 // 1
#define TIM3_PRESCAL				4799
#define TIM3_FREQFACT				10000//(SYSYTEM_CLOCK/(TIM3_PRESCAL+1))

#define TMR1_PERIOD					1200//2400//((SYSYTEM_CLOCK/(TIM1_PRESCAL+1))/10000)//24

#if   BRUSH_6_8K//BRUSH_FREQ==BRUSH_6_8K
#define	TMR2_PERIOD					1800//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#elif   BRUSH_5K//BRUSH_FREQ==BRUSH_5K
#define	TMR2_PERIOD					2400//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#elif BRUSH_10K//BRUSH_FREQ==BRUSH_10K
#define	TMR2_PERIOD					2400//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#elif BRUSH_15K//BRUSH_FREQ==BRUSH_10K
#define	TMR2_PERIOD					1600//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#elif BRUSH_20K//BRUSH_FREQ==BRUSH_10K
#define	TMR2_PERIOD					1200//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#elif BRUSH_2K//BRUSH_FREQ==BRUSH_2K
#define	TMR2_PERIOD					2000//2400//((SYSYTEM_CLOCK/(TIM2_PRESCAL+1))/10000)//10000HZ 100us中断一次 触发一次ADC采样
#endif


#define	TMR3_PERIOD					2000//(TIM3_FREQFACT/5)  //5HZ 200ms 
#define	BRUSH_PERIOD				(TMR2_PERIOD)
#define	PUMP_PERIOD					(TMR3_PERIOD)


typedef struct{
	uint8_t 	bTmrAdcFlag:1;// 20220227
	uint8_t 	bTmrSysFlag:1;// 20220227
	uint8_t 	bTmr200usFlag:1;
	uint8_t 	bTmr500usFlag:1;
	uint8_t 	bTmr01msFlag:1;
	uint8_t 	bTmr02msFlag:1;
	uint8_t 	bTmrRun02msFlag:1;
	uint8_t 	bTmr10msFlag:1;	
	uint8_t 	bTmr100msFlag:1;	
	uint8_t 	bTmrRun10msFlag:1;		
	uint8_t 	bTmr500msFlag:1;	
	uint8_t 	bFLASH500msFlag:1;	
	uint8_t 	bPowerOnmsFlag:1;	
	uint8_t 	bPowerOnmsFlagBack:1;	
	uint8_t 	bTmr01msCount;
	uint8_t 	bTmr02msCount;
	uint8_t 	bTmr10msCount;
	uint8_t 	bTmr100msCount;
	uint8_t 	bTmr500msCount;
	uint8_t 	bTmrRun02msCount;
	uint8_t 	bTmrRun10msCount;
	uint16_t 	bPowerOn1sCount;
}TimePara;

void TIM3_PWM_Init(u16 Prescaler,u16 Period);
void TIM2_PWM_Init(u16 Prescaler,u32 Period);
void TIM1_PWM_Init(u16 Prescaler,u32 Period);
void Tim17_UPCount(u16 Prescaler,u16 Period);
void SysTick_init(uint16_t uiHz);


#endif

