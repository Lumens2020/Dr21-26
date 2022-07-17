#ifndef _COMMSCREEN_H_
#define _COMMSCREEN_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "config.h"

// 1: LCD屏幕
// 0: LED屏幕
//#define LCDScreenEnableE   1
//#define YKLLCD             1

#if LCDScreenEnableE==1

#define LCD_LENTH		(uint16_t)(21)			//主板--->显示屏的一帧数据长度
#define L_CHECKSUM_H	19
#define L_CHECKSUM_L	20

#define SYNC_CODE		0xAC
#define MCU_LCD_ID		0x04
#define MCU_LCD_LENTH	0x10

typedef struct{
 uint8_t xpMcuToLcdBuff[LCD_LENTH];
 uint8_t xpworkgear:3;
 uint8_t xpworkgeartime:5;
 uint8_t xpBatSocVal;
 uint8_t xpErrorStatus;
 uint8_t xpWorkStatus:3;
 uint8_t xpWorkErrorDisEnable:1;
 uint8_t xpLCDStatus:2;
 uint8_t xpMachineStatus:2;
 uint8_t xpQWaterBoxStatus;
 uint8_t xpWWaterBoxStatus;
 uint8_t xpBrushStatus;
 uint8_t xpACleanStatus:4;
 uint8_t xpACleanWarnStatus:4;
 uint8_t xpDryStatus:4;
 uint8_t xpWaterEStatus:4;
 uint8_t xpDryTimeS_H;
 uint8_t xpDryTimeS_L;
 uint8_t xpWaterETimeS_H;
 uint8_t xpWaterETimeS_L;
 uint8_t xpACleanTimeS_H;
 uint8_t xpACleanTimeS_L;
 uint8_t xpSewagelv:4;
 uint8_t xpSewageStatus:4;
}MCUtoLCDData;

typedef struct{
	uint8_t		ucLcdOrderStatus;
	uint8_t		ucLcdPowerStatus;
}LCDPara;

typedef enum{
	OPEN_LCD=1,//亮屏
	CLOSE_LCD//关屏
}LCDSta;

typedef enum{
	K_AUTO_MODE=0x01,
	K_STRONG_MODE=0x02,
	K_DISIN_MODE=0x04
}DCHGSta;

#else
#define LCD_LENTH		(uint16_t)(15)			//主板--->显示屏的一帧数据长度

#define L_CHECKSUM_H	13
#define L_CHECKSUM_L	14

#define SYNC_CODE		0xAC
#define MCU_LCD_ID		0x04
#define MCU_LCD_LENTH	0x0A

typedef struct{
 uint8_t xpMcuToLcdBuff[LCD_LENTH];
 uint8_t xpworkgear:4;
 uint8_t xpsewagelv:4;
 uint8_t xpBatSocVal;
 uint8_t xpErrorStatus;
 uint8_t xpWorkStatus;
 uint8_t xpQWaterBoxStatus;
 uint8_t xpWWaterBoxStatus;
 uint8_t xpBrushStatus;
 uint8_t xpACleanStatus;
 uint8_t xpLCDStatus;
 uint8_t reserve;
}MCUtoLCDData;

typedef struct{
	uint8_t		ucLcdOrderStatus;
	uint8_t		ucLcdPowerStatus;
}LCDPara;

typedef enum{
	OPEN_LCD=1,//亮屏
	CLOSE_LCD//关屏
}LCDSta;

typedef enum{
	K_AUTO_MODE=0x01,
	K_STRONG_MODE=0x02,
	K_DISIN_MODE=0x04
}DCHGSta;

#endif

void Updata_SendLCDUartData(MCUtoLCDData* MToLCDD);

#endif

