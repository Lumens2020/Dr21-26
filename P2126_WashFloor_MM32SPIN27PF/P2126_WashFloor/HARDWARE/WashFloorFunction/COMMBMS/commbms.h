#ifndef _COMMBMS_H_
#define _COMMBMS_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "commfct.h"

#define UART1_TICKS_INTERVA      10
#define UART1_COMM_TIMEOUT		(10000/UART1_TICKS_INTERVA)// 2s 串口一帧数据接收结束计时


#define BMS_LENTH				(uint16_t)(8)				//主板--->电池的一帧数据长度
#define BAT_LENTH			    (uint16_t)(15)			    //电池--->主板的一帧数据长度

#define RB_CHECKSUM_H	13					//BMS接收校验MSB
#define RB_CHECKSUM_L	14					//BMS接收校验LSB

#define B_CHECKSUM_H	6
#define B_CHECKSUM_L	7

#define SYNC_CODE		0xAC

#define BMS_MCU_ID		0x05
#define BMS_MCU_LENTH	0x0A

#define MCU_BMS_ID		0x02
#define MCU_BMS_LENTH	0x03


typedef struct{
 uint8_t xpMcuToBmsBuff[BMS_LENTH];
 uint8_t xpWorkStatus;      // 主控板工作状态
 uint8_t xpKeyOnOffStatus;  // 开关按键触发状态回传
 uint8_t xpKeyPushStatus;   // 主控板上按键状态
}MCUtoBMSData;

typedef struct{
	uint8_t BMS_u8Uart1RecData[BAT_LENTH];
    uint8_t xpWorkStatus;    // BMS工作状态
	uint8_t xpBatSocVal;     // 电池SOC
	uint8_t xpBatTempVal;    // 电池温度
	uint8_t xpErrorStatus;   // 电池故障状态值
	uint8_t xpWarnStatus;    // 警告状态：待定
	uint8_t xpKeyOnOffStatus;// 开关按键触发状态
    uint8_t xpBatVolH;       // 电池电压高位
    uint8_t xpBatVolL;       // 电池电压低位
    uint8_t reserve0;        // 预留
    uint8_t reserve1;        // 预留
}BMStoMCUData;

typedef struct{
	uint8_t bBmsCommErrorFlag:1;
	uint8_t bSendBldcBuffFlag:1;
	uint16_t uiUart1RxTimeOutCnt;
	uint16_t uiUart1RxCnt;
	uint8_t	ucSendByteCnt;
	uint8_t ucBmsTxGapTmr;
	uint8_t	ucTxdGapCnt;
}UART1Comm;

typedef struct{
	uint8_t		ucBatSocVal;
	uint8_t		ucBmsWorkStatus;
	uint8_t		ucBmsOldWorkStatus;	
	uint8_t		ucBatTemperature;
	uint8_t		ucBatWarning;
	uint8_t		ucBatReserve1;
	uint8_t		ucBatReserve2;
	uint8_t 	ucToBmsOrder;
	uint16_t	uiBatVoltage;
}BMSPara;

void Updata_SendBMSUartData(MCUtoBMSData* MToBMSD);
void Updata_ReceBMSUartData(BMStoMCUData* BMSToMD);
//void Deal_ReceBMSUartData(BMStoMCUData* BMSToMD,UART1Comm* comm1para);
void Deal_ReceBMSUartData(BMStoMCUData* BMSToMD,UART1Comm* comm1para,FCTtoMCUData* FCTToMD,FCT_UART1Comm* fctcomm1para);
void OverTime_CommBMS(UART1Comm* comm1para);

#endif

