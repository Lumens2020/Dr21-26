#ifndef _COMMBLDC_H_
#define _COMMBLDC_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define UART2_TICKS_INTERVA      10
#define UART2_COMM_TIMEOUT		(10000/UART2_TICKS_INTERVA)// 2s 串口一帧数据接收结束计时


#define BLDC_LENTH			    (uint16_t)(10)			//主板--->电机的一帧数据长度
#define MOTO_LENTH			    (uint16_t)(15)			//电机--->主板的一帧数据长度

#define RM_CHECKSUM_H	13					//MOTO接收校验MSB
#define RM_CHECKSUM_L	14					//MOTO接收校验MSB

#define M_CHECKSUM_H	8
#define M_CHECKSUM_L	9

#define SYNC_CODE		0xAC
#define BLDC_MCU_ID		0x01
#define BLDC_MCU_LENTH	0x0A
#define MCU_BLDC_ID		0x03
#define MCU_BLDC_LENTH	0x05

typedef enum{
	STOP_BLDC=0xf0,//关机
	LOW_BLDC,      //低档
	HIGH_BLDC      //强力
}BLDCSta;

typedef struct{
 uint8_t xpMcuToBldcBuff[BLDC_LENTH];
 uint8_t xpworkgear:4;// BLDC工作挡位
 uint8_t xpsewagelv:4;// 脏污程度
 uint8_t xpWorkStatus;// 主控板工作状态
 uint8_t reserve0;    // 预留   
 uint8_t reserve1;    // 预留  
 uint8_t reserve2;    // 预留  
}MCUtoBLDCData;

typedef struct{
	uint8_t BLDC_u8Uart2RecData[MOTO_LENTH];
    uint8_t xpBldcSpeedH; // BLDC转速数据
    uint8_t xpBldcSpeedM; // BLDC转速数据
    uint8_t xpBldcSpeedL; // BLDC转速数据
    uint8_t xpBldcTempVal;// BLDC温度
	uint8_t xpErrorStatus;// BLDC错误状态
    uint8_t reserve0;     // 预留
    uint8_t reserve1;     // 预留
    uint8_t reserve2;     // 预留
    uint8_t reserve3;     // 预留
    uint8_t reserve4;     // 预留
}BLDCtoMCUData;

typedef struct{
	uint8_t		ucBldcReserve1;
	uint8_t		ucBldcReserve2;
	uint8_t		ucBldcReserve3;
	uint8_t		ucBldcReserve4;
	uint8_t		ucBldcReserve5;
	uint8_t		ucBldcTemperature;
	uint32_t	ucSpeedValue;
}BLDCPara;

typedef struct{
	uint8_t bBldcCommErrorFlag:1;
	uint8_t bSendBldcBuffFlag:1;
	uint8_t bSendTestBuffFlag:1;
	uint8_t bSendTestEnable:1;
	uint8_t bRxClearFlag:1;
	uint16_t uiUart2RxTimeOutCnt;
	uint16_t uiUart2RxTestTimeOutCnt;
	uint16_t uiUart2RxCnt;
	uint8_t ucBldcTxGapTmr;
	uint8_t	ucTxdGapCnt;
	uint8_t	ucSendByteCnt;
}UART2Comm;

void Updata_SendBLDCUartData(MCUtoBLDCData* MToBldcD);
void Updata_ReceBLDCUartData(BLDCtoMCUData* BldcToMD);
void Deal_ReceBldcUartData(BLDCtoMCUData* BLDCToMD,UART2Comm* comm2para);
void OverTime_CommBLDC(UART2Comm* comm2para);

#endif

