#ifndef _BLDC_H_
#define _BLDC_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include"commbldc.h"

typedef struct{
	uint8_t bBldcForceStopFlag:1;  
	uint8_t bBldcForceStrongFlag:1;
	uint8_t bBldcForceLowFlag:1;
}BLDCCtl;


void Set_Bldc_OFF(BLDCCtl* bldcctrlpara);        // ��ˢͣ��
void Set_Bldc_Strong(BLDCCtl* bldcctrlpara);   // ��ˢǿ��
void Set_Bldc_Low(BLDCCtl* bldcctrlpara);       // ��ˢ�͵�
void Control_MainMotor(BLDCCtl* bldcctrlpara,MCUtoBLDCData* MToBldcD);
#endif

