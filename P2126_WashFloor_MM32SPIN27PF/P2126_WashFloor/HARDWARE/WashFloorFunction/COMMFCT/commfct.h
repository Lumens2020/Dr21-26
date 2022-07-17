#ifndef _COMMFCT_H_
#define _COMMFCT_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define FCT_UART1_TICKS_INTERVA      10
#define FCT_UART1_COMM_TIMEOUT		(3000/FCT_UART1_TICKS_INTERVA)// 2s 串口一帧数据接收结束计时
#define FCT_UART1_OVER_TIMEOUT		(20000/FCT_UART1_TICKS_INTERVA)// 2s 串口一帧数据接收结束计时


#define M_FCT_LENTH				(uint16_t)(29)//(uint16_t)(26)				//主板--->测试的一帧数据长度
#define FCT_M_LENTH			    (uint16_t)(10)			    //测试--->主板的一帧数据长度

#define FM_CHECKSUM_H	8					
#define FM_CHECKSUM_L	9					

#define MF_CHECKSUM_H	27//24
#define MF_CHECKSUM_L	28//25

#define SYNC_CODE		0xAC

#define FCT_MCU_ID		0x07// 0x08
#define FCT_MCU_LENTH	0x05

#define MCU_FCT_ID		0x02
#define MCU_FCT_LENTH	0x18//0x15//0x03

#define TEST_STAY       0x01
#define TEST_RUNNING    0x02
#define TEST_OVER       0x04

#define P2124   0x01
#define P2106   0x02
#define P2126   0x03

typedef enum{
	Softvare_V1_0=0x10,
	Softvare_V1_1,
	Softvare_V1_2,
	Softvare_V1_3,
	Softvare_V2_0=0x20,
	Softvare_V2_1,
	Softvare_V2_2,
	Softvare_V2_3,
	Softvare_V3_0=0x30,
	Softvare_V3_1,
	Softvare_V3_2,
	Softvare_V3_3,
	Softvare_V4_0=0x40,
	Softvare_V4_1,
	Softvare_V4_2,
	Softvare_V4_3
}SoftwareVer;

typedef struct{
 uint8_t xpMcuToFCTBuff[M_FCT_LENTH];
// uint8_t xpWashFloorMODE:4; // 洗地机型号
// uint8_t xpSoftwareVer:4;   // 软件版本号
 uint8_t xpWashFloorMODE; // 洗地机型号
 uint8_t xpSoftwareVer;   // 软件版本号
 uint8_t xpTesProgress:4;   // 测试进度
 uint8_t xpTesStep:4;       // 测试步骤
 uint8_t xpVoiceKeyStatus;  // 语音按键状态
 uint8_t xpCHGADH;          // 适配器AD高位
 uint8_t xpCHGADL;          // 适配器AD低位
 uint8_t xpKeyADH;          // 按键AD高位
 uint8_t xpKeyADL;  	    // 按键AD高位
 uint8_t xpBrushADH;        // 地刷AD高位
 uint8_t xpBrushADL;        // 地刷AD高位
 uint8_t xpWaterpumpADH;    // 水泵AD高位
 uint8_t xpWaterpumpADL;    // 水泵AD高位
 uint8_t xpQWaterBoxADH;    // 清水箱AD高位
 uint8_t xpQWaterBoxADL;    // 清水箱AD高位
 uint8_t xpWWaterBoxADH;    // 污水箱AD高位
 uint8_t xpWWaterBoxADL;    // 污水箱AD高位
 uint8_t xpSewageLVADH;     // 脏污AD高位
 uint8_t xpSewageLVADL;     // 脏污AD高位
 uint8_t xpBldcSpeedH;      // BLDC转速数据
 uint8_t xpBldcSpeedM;      // BLDC转速数据
 uint8_t xpBldcSpeedL;      // BLDC转速数据
 uint8_t TileStates;	    // 倾倒按键状态
 uint8_t reserve1;	        // 预留
 uint8_t reserve2;	        // 预留
}MCUtoFCTData;

typedef struct{
	uint8_t FCT_u8Uart1RecData[FCT_M_LENTH];
//	uint8_t xpWashFloorMODE:4; // 洗地机型号
	uint8_t xpWashFloorMODE; // 洗地机型号
//	uint8_t xpSoftwareVer:4;   // 软件版本号
	uint8_t xpSoftwareVer;   // 软件版本号
	uint8_t xpTesProgress:4;   // 测试进度
	uint8_t xpTesStep:4;	   // 测试步骤
    uint8_t reserve0;          // 预留
    uint8_t reserve1;          // 预留
    uint8_t reserve2;          // 预留
}FCTtoMCUData;

typedef struct{
	uint8_t bFCTCommErrorFlag:1;
	uint8_t bSendFCTBuffFlag:1;
	uint8_t bTestModeFlag:1;
	uint8_t bClearMemoryCnt;
	uint16_t uiUart1RxTimeOutCnt;
	uint16_t uiUart1RxCnt;
	uint32_t uiFctTestCnt;	
	uint8_t	ucSendByteCnt;
	uint8_t ucFCTTxGapTmr;
	uint8_t	ucTxdGapCnt;
}FCT_UART1Comm;

typedef struct{
	uint8_t		ucFctSocVal;
	uint8_t		ucFctWorkStatus;
	uint8_t		ucFctOldWorkStatus;	
	uint8_t		ucFctTemperature;
	uint8_t		ucFcttWarning;
	uint8_t		ucFctReserve1;
	uint8_t		ucFctReserve2;
	uint8_t 	ucToFctOrder;
	uint16_t	uiFctVoltage;
}FCTPara;

void Updata_SendFCTUartData(MCUtoFCTData* MToFCTD);
void Updata_ReceFCTUartData(FCTtoMCUData* FCTToMD);
void Deal_ReceFCTUartData(FCTtoMCUData* FCTToMD,FCT_UART1Comm* comm1para);
void OverTime_CommFCT(FCT_UART1Comm* comm1para);

#endif

