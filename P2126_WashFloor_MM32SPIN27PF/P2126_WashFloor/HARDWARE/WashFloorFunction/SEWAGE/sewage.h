#ifndef _SEWAGE_H_
#define _SEWAGE_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "adc.h"
#include "cleanwater.h"
#include "timer.h"
#include "key.h"// 20220106

#include "config.h"

// 0:自动模式 吸水模式倾倒工作前3S污水箱0.5S消抖检测，后,5S消抖检测
// 1:自动模式 吸水模式倾倒工作前3S污水箱0.5S消抖检测，后不检测
// 2:自动模式 吸水模式倾倒工作污水箱0.5S消抖检测
//#define SEWAGE_CHECK_MODE  1

#define SEWAGE_TICKS_INTERVAL      1//2//10

#define SEWAGE_TIME0				(5000/SEWAGE_TICKS_INTERVAL)//(5000/SEWAGE_TICKS_INTERVAL)			// 5S
#define SEWAGE_TIME00				(12000/SEWAGE_TICKS_INTERVAL)			// 10S
#define SEWAGE_TIME1				(3000/SEWAGE_TICKS_INTERVAL)			// 3S
#define SEWAGE_TIME3				(2000/SEWAGE_TICKS_INTERVAL)			// 3S
//#define SEWAGE_TIME2				(16/SEWAGE_TICKS_INTERVAL)//(1000/SEWAGE_TICKS_INTERVAL)			// 1S
//#define SEWAGE_TIME2				(50/SEWAGE_TICKS_INTERVAL)//(1000/SEWAGE_TICKS_INTERVAL)			// 1S
#define SEWAGE_TIME2				(10/SEWAGE_TICKS_INTERVAL)//(50/SEWAGE_TICKS_INTERVAL)//(80/SEWAGE_TICKS_INTERVAL)//(1000/SEWAGE_TICKS_INTERVAL)			// 1S
#define SEWAGE_TIME4				(5/SEWAGE_TICKS_INTERVAL)//(50/SEWAGE_TICKS_INTERVAL)//(1000/SEWAGE_TICKS_INTERVAL)			// 1S

#define SEWAGE_STAGE_VREF				800//200	出现一台未安装AD是270的		//污水桶未安装判断值
#define SEWAGE_STAGE_FULL_VREF			3000 //2900//3500//20211104 //2500	    //污水水位满判断值
#define SEWAGE_STAGE_COUNT		        20//10			//污水水位连续检测次数
#define SEWAGE_STAGE_COUNT2		        20			//污水水位连续检测次数
#define SEWAGE_STAGE_COUNT1		        50			//污水水位连续检测次数

// 2021.0630.1318: 未满   1365   满 3600~4016
//

#define SEWAGE_COMPREF_VREF	    3400		//初始校准判断值
#define SEWAGE_REF_VREF		    3600		//初始校准值

#define SEWAGE_HIGH_VREF		800			//脏污程度判断值/严重
#define SEWAGE_MID_VREF			2500		//脏污程度判断值/中等
#define SEWAGE_LOW_VREF			3850		//脏污程度判断值/低等   默认值
#define SEWAGE_MIN_VREF			((SEWAGE_LOW_VREF*22)/100)		//脏污程度最低参考值

#define SEWAGE_FREQ_TIME	    40			//unit:10ms  40 means 400ms			//脏污程度检测时间
#define SEWAGE_200MS			20			//unit:10ms  20 means 200ms			//脏污程度开通检测时间
#define SEWAGE_STAGE_FREQ_TIME	10			//unit:10ms  10 means 100ms			//污水桶检测时间
#define SEWAGE_LEVEL_TIME	    10			//unit:100us 10 means 1ms				//污水水位检测时间
#define SEWAGE_TIME				150			//unit:100us 150 means 15ms			//脏污程度判断时间
#define SEWAGE_COUNT			2			//脏污程度连续检测次数

#define SEWAGE_HIGH_PER   (0.78)//(0.95)//(0.88)//20211104(0.78)
#define SEWAGE_MID_PER    (0.4)//20211104(0.4)
#define SEWAGE_WARN_PER   (0.8)


typedef enum{
	M_LOW_STA=0x00,//脏污程度/轻度
	M_MID_STA=0x01,//脏污程度/中度
	M_HIGH_STA=0x02,//脏污程度/严重
	M_SOHIGH_STA=0x04//脏污程度/非常严重
}SewageSta;

typedef enum{
	WBULET_NORMAL_STA=0x00,
	WBULET_BLANK_STA,
	WBULET_FULL_STA
}SewageBoxSta;

typedef struct{
	uint8_t	 xpSewageWarnFlag:1;
	uint8_t	 xpSewageChangeFlag:1;// 污水箱检测延时变更标志
	uint8_t	 xpSewageLVEnable:1;//脏污检测使能标志
	uint8_t  xpsewagelv;
	uint8_t	 xpSewageValue;
	uint8_t  xpSewageStateCount;
	uint16_t xpSewageLVcnt0;
	uint16_t xpSewageLVcnt1;
	uint16_t xpSewageLVcnt2;
	uint16_t xpSewageLVcnt3;
	uint16_t xpSewageLVcnt4;
	uint16_t xpSewageLVcnt5;
	uint16_t xpSewageLVcnt6;
	uint16_t xpSewageLVcnt7;
	uint16_t xpSewageLVcnt8;// 20220110
	uint16_t xpSewageLVcnt9;// 20220110
	uint16_t xpSewageLVSens;// 20220106
	uint16_t xpSewageChangecnt;
	uint16_t uiSewagepromptValue0;// 20% 提示用户自清洁
	uint16_t uiSewagepromptValue1;// 80% 放在底座上提示用户自清洁
	uint16_t uiSewageHighValue;// 40%
	uint16_t uiSewageMiddValue;// 78% 
	uint16_t uiSewageUpValue;// 上限
	uint16_t uiSewageDownValue;// 下限
	uint16_t xpSewageLvADValue;
	uint16_t xpSewageAlignValue;
	uint16_t xpSewageDefaultRefValue;
}SewagePara;

void Check_SewageStage0(WaterBoxStatus* waterboxctl,SewagePara* sewagectl,ADCPara* adcparactl);
void Check_SewageStage1(WaterBoxStatus* waterboxctl,SewagePara* sewagectl,ADCPara* adcparactl);
void Enable_IRLED_PIN(uint8_t irpinctl);
//void Check_SewageLV(ADCPara* adcparactl,TimePara* timesetvar,SewagePara* sewagectl);
void Check_SewageLV(ADCPara* adcparactl,TimePara* timesetvar,SewagePara* sewagectl,KeyPara* keyvar);

#endif

