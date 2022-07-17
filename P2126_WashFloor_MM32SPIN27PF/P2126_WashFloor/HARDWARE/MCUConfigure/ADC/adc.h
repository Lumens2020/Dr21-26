#ifndef _ADC_H_
#define _ADC_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

typedef enum{
	WaterPumpCHN=0,
	Brush_CHN,
	Key_CHN,
	SewageLV_CHN,// A_DHL_CHN
	SewageStage_CHN,// SIGL_CHN
	CHGVol_CHN,
	WaterLV_CHN
}ADC_CHN;

#define ADC_CHANNAL_NUM 		8

typedef struct{
	uint16_t uiADC_ResultBuff[ADC_CHANNAL_NUM];
	uint16_t uiWaterPumpAdcValue;// 水泵电流AD
	uint16_t uiBrushAdcValue;// 地刷电流AD
	uint16_t uiKeyAdcValue;// 按键AD
	uint16_t uiSewageStageAdcValue;// 污水水位AD
	uint16_t uiSewageLVAdcValue;// 污水脏污AD
	uint16_t uiCHGAdcValue;// 充电口AD
	uint16_t uiCleanWaterAdcValue;// 清水箱检测AD
	uint16_t uiNOBrushAdcValue;
	uint16_t uiStallBrushAdcValue;
	uint16_t uiOC1BrushAdcValue;
	uint16_t uiOC2BrushAdcValue;
	uint16_t uiLimitBrushAdcValue;
	uint16_t uiWaterPumpAdcValue_sum;
	uint16_t uiWaterPumpAdcValue_max;
	uint16_t uiWaterPumpAdcValue_min;
	uint16_t uiBrushAdcValue_sum;
	uint16_t uiBrushAdcValue_max;
	uint16_t uiBrushAdcValue_min;
	uint16_t uiKeyAdcValue_sum;
	uint16_t uiKeyAdcValue_max;
	uint16_t uiKeyAdcValue_min;
	uint16_t uiSewageLVAdcValue_sum;
	uint16_t uiSewageLVAdcValue_max;
	uint16_t uiSewageLVAdcValue_min;
	uint16_t uiSewageStageAdcValue_sum;
	uint16_t uiSewageStageAdcValue_max;
	uint16_t uiSewageStageAdcValue_min;
	uint8_t  V_adc_trans_count;	
	uint8_t  uiBrushAdcValue_cnt;	
	
	uint16_t uiBrushAdcValueH;// 地刷电流AD
	uint16_t uiBrushAdcValueL;// 地刷电流AD
}ADCPara;


/* ------------ADC ---------------*/
///-----ADC1-----
#define SewageStage_ADDR			(ADC1->ADDR0 & 0xFFF)						//PA0-->AN0
//#define A_DHL_ADDR			(ADC1->ADDR1 & 0xFFF)						//PA1-->AN1
//#define W_bemf					(ADC1->ADDR2 & 0xFFF)						//PA2-->AN2
//#define NC							(ADC1->ADDR3 & 0xFFF)						//PA3-->AN3
//#define NC							(ADC1->ADDR4 & 0xFFF)						//PA4-->AN4
//#define ADCCOMMANDINPUT (ADC1->ADDR5 & 0xFFF)						//PA5-->AN5
//#define Tmosfet				  (ADC1->ADDR6 & 0xFFF)						//PA6-->AN6
//#define OVP         		(ADC1->ADDR7 & 0xFFF)						//PA7-->AN7
#define CHGV_ADDR			(ADC1->ADDR8 & 0xFFF)						//PB0-->AN8
//#define Tmotor         	(ADC1->ADDR9 & 0xFFF)						//PB1-->AN9
#define Key_ADDR    		(ADC2->ADDR2 & 0xFFF)//	(ADC1->ADDR10 & 0xFFF)					//PB2-->AN10
//#define A_DHC_ADDR      (ADC1->ADDR11 & 0xFFF)					//PB10-->AN11

//------ADC2-----
//#define NC							(ADC2->ADDR0 & 0xFFF)						//PA10-->AN0
#define SewageLV_ADDR				(ADC2->ADDR1 & 0xFFF)						//PB15-->AN1
#define WaterLV_ADDR				(ADC2->ADDR3 & 0xFFF)						//PB15-->AN1
#define Brush_ADDR					(ADC2->ADDR6 & 0xFFF)//(ADC2->ADDR2 & 0xFFF)						//PB14-->AN2
#define WaterPumpADDR				(ADC2->ADDR10 & 0xFFF)//(ADC2->ADDR3 & 0xFFF)						//PB13-->AN3
//#define NC							(ADC2->ADDR4 & 0xFFF)						//PA4-->AN4
//#define ADCCOMMANDINPUT (ADC2->ADDR5 & 0xFFF)						//PA5-->AN5
//#define Tmosfet				  (ADC2->ADDR6 & 0xFFF)						//PA6-->AN6
//#define BpowerADDR      (ADC2->ADDR7 & 0xFFF)						//PA7-->AN7
//#define ApowerADDR			(ADC2->ADDR8 & 0xFFF)						//PB0-->AN8
//#define BpowerCHN       (ADC2->ADDR9 & 0xFFF)						//PB1-->AN9
//#define BpowerCHN    		(ADC2->ADDR10 & 0xFFF)					//PB2-->AN10
//#define IPEAK         	(ADC2->ADDR11 & 0xFFF)					//PB10-->AN11

void ADC_GPIO_Init(void);
void ADC1_Init(void);
void ADC2_Init(void);
extern void Eimination_AdValue(ADCPara* advalue);

#endif

