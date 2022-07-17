#ifndef  _EEPROM_H_
#define  _EEPROM_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define Page  (u8)(127)

#define EEPROM_HEADADD   0xAA55

typedef enum{
	EE_HEADER=0,
	EE_SEWAGE,
	EE_DEFAULT,
	EE_LANGUAGE,
#if TEST_AD_ENABLE==1
	EE_VOICEFUN,
	WARN_CNT,// 5
	WARN_TYPE0,// 6
	WARN_AD_WaterPump0,
	WARN_AD_Brush0,
	WARN_AD_Key0,
	WARN_AD_SewageLV0,
	WARN_AD_SewageStage0,
	WARN_AD_CHGVol0,
	WARN_AD_WaterLV0,
	WARN_TYPE1,	// 14
	WARN_AD_WaterPump1,
	WARN_AD_Brush1,
	WARN_AD_Key1,
	WARN_AD_SewageLV1,
	WARN_AD_SewageStage1,
	WARN_AD_CHGVol1,
	WARN_AD_WaterLV1,
	WARN_TYPE2, // 22
	WARN_AD_WaterPump2,
	WARN_AD_Brush2,
	WARN_AD_Key2,
	WARN_AD_SewageLV2,
	WARN_AD_SewageStage2,
	WARN_AD_CHGVol2,
	WARN_AD_WaterLV2,
	WARN_TYPE3,	// 30
	WARN_AD_WaterPump3,
	WARN_AD_Brush3,
	WARN_AD_Key3,
	WARN_AD_SewageLV3,
	WARN_AD_SewageStage3,
	WARN_AD_CHGVol3,
	WARN_AD_WaterLV3,
	WARN_TYPE4,// 38
	WARN_AD_WaterPump4,
	WARN_AD_Brush4,
	WARN_AD_Key4,
	WARN_AD_SewageLV4,
	WARN_AD_SewageStage4,
	WARN_AD_CHGVol4,
	WARN_AD_WaterLV4
#else
    EE_VOICEFUN      //4
#endif
} VoiceMode;


typedef struct{

uint16_t    uiWriteDataFlash[256];
uint16_t    uiReadDataFlash[256];
uint8_t 	bEnableEEpromFlag:1;
uint8_t 	bEEpromRunFlag:1;// 20220114
}Eeprom_TypeDef;

void Read_EEPROM(void);                                
void Write_EEPROM(void);                                

#endif

