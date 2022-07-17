#ifndef _COMMTEST_H_
#define _COMMTEST_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include"commbldc.h"


#define UART2_TICKS_Test_INTERVA      10
#define UART2_COMM_TEST_TIMEOUT		  (3000/UART2_TICKS_Test_INTERVA)// 2s ����һ֡���ݽ��ս�����ʱ

#define M_TEST_LENTH				(uint16_t)(30)			//����--->��ʾ����һ֡���ݳ���
#define T_MCU_LENTH			        (uint16_t)(15)//(uint16_t)(14)			//����--->��ʾ����һ֡���ݳ���

#define TEST_CHECKSUM_H	  13
#define TEST_CHECKSUM_L	  14

#define MCU_CHECKSUM_H	  28
#define MCU_CHECKSUM_L	  29

#define SYNC_CODE		0xAC
#define MCU_TEST_ID		0x06
#define MCU_TEST_LENTH	0x19//0x14

#define TEST_MCU_ID		0x07
#define TEST_MCU_LENTH	0x09

typedef struct{
 uint8_t xpMcuToTestBuff[M_TEST_LENTH];
 uint8_t xpSoftwarever:4;
 uint8_t xpMachinemode:4;
 uint8_t xpWorkmode:4;
 uint8_t xpRunmode:4;
 uint8_t xpBatVolH; 	  // ��ص�ѹ��λ
 uint8_t xpBatVolL; 	  // ��ص�ѹ��λ
 uint8_t xpBatSocVal;
 uint8_t xpErrorStatus;
 uint8_t xpWWaterBoxStatus:4;
 uint8_t xpQWaterBoxStatus:4;
 uint8_t xpBrushStatus;
 uint8_t xpACleanStatus;
 uint8_t xpWaterelecStatus:4;
 uint8_t xpDryStatus:4;
 uint8_t xpKeyADH;
 uint8_t xpKeyADL;
 uint8_t xpBrushADH;
 uint8_t xpBrushADL;
 uint8_t xpWaterpumpADH;
 uint8_t xpWaterpumpADL;
 uint8_t xpWWaterBoxADH;
 uint8_t xpWWaterBoxADL;
 uint8_t xpSewageLVADH;
 uint8_t xpSewageLVADL;
 uint8_t xpBldcSpeedH; // BLDCת������
 uint8_t xpBldcSpeedM; // BLDCת������
 uint8_t xpBldcSpeedL; // BLDCת������
 uint8_t xpBldcTempVal;// BLDC�¶�
 uint8_t xpSewageLV;// ���۳̶�
}MCUtoTESTData;


typedef struct{
	uint8_t TEST_u8Uart2RecData[T_MCU_LENTH];
	uint8_t xpSoftwarever:4;
    uint8_t reserve0;     // Ԥ��
    uint8_t reserve1;     // Ԥ��
    uint8_t reserve2;     // Ԥ��
    uint8_t reserve3;     // Ԥ��
    uint8_t reserve4;     // Ԥ��
    uint8_t reserve5;     // Ԥ��
    uint8_t reserve6;     // Ԥ��
    uint8_t reserve7;     // Ԥ��
    uint8_t reserve8;     // Ԥ��
}TesttoMCUData;



void Deal_ReceTestUartData(TesttoMCUData* TESTToMD,BLDCtoMCUData* BLDCToMD,UART2Comm* comm2para);
void Updata_SendTESTUartData(MCUtoTESTData* MToTESTD);
void OverTime_CommTEST(UART2Comm* comm2para);
#endif

