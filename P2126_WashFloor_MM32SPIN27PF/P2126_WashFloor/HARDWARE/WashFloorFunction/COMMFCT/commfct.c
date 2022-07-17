#include"commfct.h"
#include"uart.h"
#include"control.h"

//****************上传FCT数据更新*******************
///*功能：      10ms执行一次 上传给FCT数据更新			 						*///
///*入口参数：	MCUtoFCTData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_SendFCTUartData(MCUtoFCTData* MToFCTD)
{
	uint16_t uiTempCheckSum;
	uint8_t  uclenth,i;
	
	MToFCTD->xpMcuToFCTBuff[HEAD]    = SYNC_CODE;
	MToFCTD->xpMcuToFCTBuff[ID]	     = MCU_FCT_ID;
	MToFCTD->xpMcuToFCTBuff[LENTH]   = MCU_FCT_LENTH;	
	MToFCTD->xpMcuToFCTBuff[TxDATA1] = MToFCTD->xpWashFloorMODE;
//	MToFCTD->xpMcuToFCTBuff[TxDATA1]|= (((uint8_t)MToFCTD->xpSoftwareVer)<<4);
	MToFCTD->xpMcuToFCTBuff[TxDATA2] = MToFCTD->xpTesProgress;
	MToFCTD->xpMcuToFCTBuff[TxDATA2]|= (((uint8_t)MToFCTD->xpTesStep)<<4);
	MToFCTD->xpMcuToFCTBuff[TxDATA3] = MToFCTD->xpVoiceKeyStatus;
	MToFCTD->xpMcuToFCTBuff[TxDATA4] = MToFCTD->xpCHGADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA5] = MToFCTD->xpCHGADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA6] = MToFCTD->xpKeyADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA7] = MToFCTD->xpKeyADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA8] = MToFCTD->xpBrushADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA9] = MToFCTD->xpBrushADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA10] = MToFCTD->xpWaterpumpADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA11] = MToFCTD->xpWaterpumpADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA12] = MToFCTD->xpQWaterBoxADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA13] = MToFCTD->xpQWaterBoxADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA14] = MToFCTD->xpWWaterBoxADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA15] = MToFCTD->xpWWaterBoxADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA16] = MToFCTD->xpSewageLVADH;
	MToFCTD->xpMcuToFCTBuff[TxDATA17] = MToFCTD->xpSewageLVADL;
	MToFCTD->xpMcuToFCTBuff[TxDATA18] = MToFCTD->xpBldcSpeedH;
	MToFCTD->xpMcuToFCTBuff[TxDATA19] = MToFCTD->xpBldcSpeedM;
	MToFCTD->xpMcuToFCTBuff[TxDATA20] = MToFCTD->xpBldcSpeedL;
	MToFCTD->xpMcuToFCTBuff[TxDATA21] = MToFCTD->TileStates;
//	MToFCTD->xpMcuToFCTBuff[TxDATA22] = MToFCTD->reserve1;
	MToFCTD->xpMcuToFCTBuff[TxDATA22] = MToFCTD->xpSoftwareVer;
	MToFCTD->xpMcuToFCTBuff[TxDATA23] = MToFCTD->reserve2;
	uiTempCheckSum = 0;
	uclenth = sizeof(MToFCTD->xpMcuToFCTBuff)-2;
	for(i=1;i<uclenth;i++)
	{
		uiTempCheckSum += MToFCTD->xpMcuToFCTBuff[i];
	}
	MToFCTD->xpMcuToFCTBuff[MF_CHECKSUM_H] = (uint8_t)((uiTempCheckSum&0xFF00)>>8);
	MToFCTD->xpMcuToFCTBuff[MF_CHECKSUM_L] = (uint8_t)(uiTempCheckSum&0x00FF);	
}

//****************接收FCT数据更新*******************
///*功能：      10ms执行一次 接收FCT数据更新			 						*///
///*入口参数：	FCTtoMCUData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_ReceFCTUartData(FCTtoMCUData* FCTToMD)
{
	FCTToMD->xpWashFloorMODE  = FCTToMD->FCT_u8Uart1RecData[RxDATA1];
//	FCTToMD->xpWashFloorMODE  = ((FCTToMD->FCT_u8Uart1RecData[RxDATA1])&0x0f);
//	FCTToMD->xpSoftwareVer    = ((FCTToMD->FCT_u8Uart1RecData[RxDATA1]>>4)&0x0f);
	FCTToMD->xpTesProgress    = ((FCTToMD->FCT_u8Uart1RecData[RxDATA2])&0x0f);
	FCTToMD->xpTesStep        = ((FCTToMD->FCT_u8Uart1RecData[RxDATA2]>>4)&0x0f);
	FCTToMD->xpSoftwareVer    = FCTToMD->FCT_u8Uart1RecData[RxDATA3];
	FCTToMD->reserve0         = FCTToMD->FCT_u8Uart1RecData[RxDATA4];
	FCTToMD->reserve1         = FCTToMD->FCT_u8Uart1RecData[RxDATA5];
}

//****************电池包串口数据接收函数*******************
///*功能：      10ms执行一次 按照协议接收电池包数据		 						*///
///*入口参数：	BMStoMCUData      UART1Comm																	*///
///*出口参数：	无																				*///
//************************************************************

void Deal_ReceFCTUartData(FCTtoMCUData* FCTToMD,FCT_UART1Comm* comm1para)
{
	uint8_t k;
	uint16_t uiU1Checksum,uiU1Temp;

//	FCTToMD->BMS_u8Uart1RecData[comm1para->uiUart1RxCnt] = UART_ReceiveData(UART1);
	
	if(comm1para->uiUart1RxCnt<FCT_M_LENTH)comm1para->uiUart1RxCnt++;
	if(FCTToMD->FCT_u8Uart1RecData[HEAD]!=SYNC_CODE)
	{
		comm1para->uiUart1RxCnt = 0;
		FCTToMD->FCT_u8Uart1RecData[HEAD] =0x00;
	}
	else if(comm1para->uiUart1RxCnt==2)
	{
		if(FCTToMD->FCT_u8Uart1RecData[ID]!=FCT_MCU_ID)
		{
			comm1para->uiUart1RxCnt = 0;
			FCTToMD->FCT_u8Uart1RecData[HEAD] =0x00;
			FCTToMD->FCT_u8Uart1RecData[ID]=0x00;
		}
	}
	else if(comm1para->uiUart1RxCnt==3)
	{
		if(FCTToMD->FCT_u8Uart1RecData[LENTH]!=FCT_MCU_LENTH)
		{
			comm1para->uiUart1RxCnt = 0;
			FCTToMD->FCT_u8Uart1RecData[HEAD] =0x00;
			FCTToMD->FCT_u8Uart1RecData[ID]=0x00;
		}
	}					
	else if(comm1para->uiUart1RxCnt==FCT_M_LENTH)
	{
		uiU1Checksum = 0;
		for(k=1;k<(sizeof(FCTToMD->FCT_u8Uart1RecData)-2);k++)
		{
			uiU1Checksum += FCTToMD->FCT_u8Uart1RecData[k];
		}
		uiU1Temp = (uint16_t)((FCTToMD->FCT_u8Uart1RecData[FM_CHECKSUM_H]<<8) + FCTToMD->FCT_u8Uart1RecData[FM_CHECKSUM_L]);
		if(uiU1Checksum == uiU1Temp)
		{
			Updata_ReceFCTUartData(FCTToMD);
			comm1para->bTestModeFlag 	   = 1;
			comm1para->uiUart1RxTimeOutCnt 	 = 0;
			comm1para->bFCTCommErrorFlag = 0;
		}							
		comm1para->uiUart1RxCnt = 0;
	}	
}

//****************电池包串口数据接收超时检测函数*******************
///*功能：      10ms执行一次 接收电池包数据超时检测		 						*///
///*入口参数：	UART1Comm																	*///
///*出口参数：	无																				*///
//************************************************************
void OverTime_CommFCT(FCT_UART1Comm* comm1para)
{
	if(++comm1para->uiUart1RxTimeOutCnt>=FCT_UART1_COMM_TIMEOUT)
		{
			comm1para->uiUart1RxTimeOutCnt = 0;
			comm1para->uiUart1RxCnt= 0;
			comm1para->bFCTCommErrorFlag = 1;
			comm1para->bTestModeFlag=0;
		} 
}

