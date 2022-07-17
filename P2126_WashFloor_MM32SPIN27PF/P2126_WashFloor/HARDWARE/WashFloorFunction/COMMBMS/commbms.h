#ifndef _COMMBMS_H_
#define _COMMBMS_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "commfct.h"

#define UART1_TICKS_INTERVA      10
#define UART1_COMM_TIMEOUT		(10000/UART1_TICKS_INTERVA)// 2s ����һ֡���ݽ��ս�����ʱ


#define BMS_LENTH				(uint16_t)(8)				//����--->��ص�һ֡���ݳ���
#define BAT_LENTH			    (uint16_t)(15)			    //���--->�����һ֡���ݳ���

#define RB_CHECKSUM_H	13					//BMS����У��MSB
#define RB_CHECKSUM_L	14					//BMS����У��LSB

#define B_CHECKSUM_H	6
#define B_CHECKSUM_L	7

#define SYNC_CODE		0xAC

#define BMS_MCU_ID		0x05
#define BMS_MCU_LENTH	0x0A

#define MCU_BMS_ID		0x02
#define MCU_BMS_LENTH	0x03


typedef struct{
 uint8_t xpMcuToBmsBuff[BMS_LENTH];
 uint8_t xpWorkStatus;      // ���ذ幤��״̬
 uint8_t xpKeyOnOffStatus;  // ���ذ�������״̬�ش�
 uint8_t xpKeyPushStatus;   // ���ذ��ϰ���״̬
}MCUtoBMSData;

typedef struct{
	uint8_t BMS_u8Uart1RecData[BAT_LENTH];
    uint8_t xpWorkStatus;    // BMS����״̬
	uint8_t xpBatSocVal;     // ���SOC
	uint8_t xpBatTempVal;    // ����¶�
	uint8_t xpErrorStatus;   // ��ع���״ֵ̬
	uint8_t xpWarnStatus;    // ����״̬������
	uint8_t xpKeyOnOffStatus;// ���ذ�������״̬
    uint8_t xpBatVolH;       // ��ص�ѹ��λ
    uint8_t xpBatVolL;       // ��ص�ѹ��λ
    uint8_t reserve0;        // Ԥ��
    uint8_t reserve1;        // Ԥ��
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

