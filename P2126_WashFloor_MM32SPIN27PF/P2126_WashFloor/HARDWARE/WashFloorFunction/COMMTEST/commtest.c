#include"commtest.h"
#include"commbldc.h"
#include"uart.h"


//****************上传屏幕数据更新*******************
///*功能：      10ms执行一次 上传给屏幕数据更新			 						*///
///*入口参数：	MCUtoLCDData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_SendTESTUartData(MCUtoTESTData* MToTESTD)
{
	uint16_t uiTempCheckSum;
	uint8_t  uclenth,i;
//	if(MToTESTD->xpSendTestFlag==0)
//		{
		MToTESTD->xpMcuToTestBuff[HEAD]    = SYNC_CODE;
		MToTESTD->xpMcuToTestBuff[ID]	   = MCU_TEST_ID;
		MToTESTD->xpMcuToTestBuff[LENTH]   = MCU_TEST_LENTH;
		MToTESTD->xpMcuToTestBuff[TxDATA1] = MToTESTD->xpMachinemode;
		MToTESTD->xpMcuToTestBuff[TxDATA1]|= (((uint8_t)MToTESTD->xpSoftwarever)<<4);
		MToTESTD->xpMcuToTestBuff[TxDATA2] = MToTESTD->xpRunmode;
		MToTESTD->xpMcuToTestBuff[TxDATA2]|= (((uint8_t)MToTESTD->xpWorkmode)<<4);
		MToTESTD->xpMcuToTestBuff[TxDATA3] = MToTESTD->xpBatVolH;
		MToTESTD->xpMcuToTestBuff[TxDATA4] = MToTESTD->xpBatVolL;
		MToTESTD->xpMcuToTestBuff[TxDATA5] = MToTESTD->xpBatSocVal;
		MToTESTD->xpMcuToTestBuff[TxDATA6] = MToTESTD->xpErrorStatus+1;
		MToTESTD->xpMcuToTestBuff[TxDATA7] = MToTESTD->xpQWaterBoxStatus;
		MToTESTD->xpMcuToTestBuff[TxDATA7]|= (((uint8_t)MToTESTD->xpWWaterBoxStatus)<<4);
		MToTESTD->xpMcuToTestBuff[TxDATA8] = MToTESTD->xpBrushStatus;
		MToTESTD->xpMcuToTestBuff[TxDATA9] = MToTESTD->xpACleanStatus;
		MToTESTD->xpMcuToTestBuff[TxDATA10] = MToTESTD->xpDryStatus;
		MToTESTD->xpMcuToTestBuff[TxDATA10]|= (((uint8_t)MToTESTD->xpWaterelecStatus)<<4);
		MToTESTD->xpMcuToTestBuff[TxDATA11]= MToTESTD->xpKeyADH;
		MToTESTD->xpMcuToTestBuff[TxDATA12]= MToTESTD->xpKeyADL;
		MToTESTD->xpMcuToTestBuff[TxDATA13]= MToTESTD->xpBrushADH;
		MToTESTD->xpMcuToTestBuff[TxDATA14]= MToTESTD->xpBrushADL;
		MToTESTD->xpMcuToTestBuff[TxDATA15]= MToTESTD->xpWaterpumpADH;
		MToTESTD->xpMcuToTestBuff[TxDATA16]= MToTESTD->xpWaterpumpADL;
		MToTESTD->xpMcuToTestBuff[TxDATA17]= MToTESTD->xpWWaterBoxADH;
		MToTESTD->xpMcuToTestBuff[TxDATA18]= MToTESTD->xpWWaterBoxADL;
		MToTESTD->xpMcuToTestBuff[TxDATA19]= MToTESTD->xpSewageLVADH;
		MToTESTD->xpMcuToTestBuff[TxDATA20]=MToTESTD->xpSewageLVADL;
		MToTESTD->xpMcuToTestBuff[TxDATA21]=MToTESTD->xpBldcSpeedH;
		MToTESTD->xpMcuToTestBuff[TxDATA22]=MToTESTD->xpBldcSpeedM;
		MToTESTD->xpMcuToTestBuff[TxDATA23]=MToTESTD->xpBldcSpeedL;
		MToTESTD->xpMcuToTestBuff[TxDATA24]=MToTESTD->xpBldcTempVal;
		MToTESTD->xpMcuToTestBuff[TxDATA25]=MToTESTD->xpSewageLV;
	uiTempCheckSum = 0;
	uclenth = sizeof(MToTESTD->xpMcuToTestBuff)-2;
	for(i=1;i<uclenth;i++)
		{
		uiTempCheckSum += MToTESTD->xpMcuToTestBuff[i];
		}
	MToTESTD->xpMcuToTestBuff[MCU_CHECKSUM_H] = (uint8_t)((uiTempCheckSum&0xFF00)>>8);
	MToTESTD->xpMcuToTestBuff[MCU_CHECKSUM_L] = (uint8_t)(uiTempCheckSum&0x00FF);	
}

//***************接收主电机数据更新*******************
///*功能：      10ms执行一次 接收BLDC数据更新			 						*///
///*入口参数：	BLDCtoMCUData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_ReceTestUartData(TesttoMCUData* TESTToMD)
{
	TESTToMD->xpSoftwarever =TESTToMD->TEST_u8Uart2RecData[RxDATA1]; 
	TESTToMD->reserve0 =TESTToMD->TEST_u8Uart2RecData[RxDATA2];
	TESTToMD->reserve1 =TESTToMD->TEST_u8Uart2RecData[RxDATA3];
	TESTToMD->reserve2=TESTToMD->TEST_u8Uart2RecData[RxDATA4];
	TESTToMD->reserve3=TESTToMD->TEST_u8Uart2RecData[RxDATA5]; 
	TESTToMD->reserve4=TESTToMD->TEST_u8Uart2RecData[RxDATA6]; 
	TESTToMD->reserve5=TESTToMD->TEST_u8Uart2RecData[RxDATA7];
	TESTToMD->reserve6=TESTToMD->TEST_u8Uart2RecData[RxDATA8];
	TESTToMD->reserve7=TESTToMD->TEST_u8Uart2RecData[RxDATA9];
	TESTToMD->reserve8=TESTToMD->TEST_u8Uart2RecData[RxDATA10];
}

void Deal_ReceTestUartData(TesttoMCUData* TESTToMD,BLDCtoMCUData* BLDCToMD,UART2Comm* comm2para)
{
	uint8_t k,i;
	uint16_t uiU2RxChecksum,uiU2Temp;

	TESTToMD->TEST_u8Uart2RecData[comm2para->uiUart2RxCnt] = UART_ReceiveData(UART2);
	if(comm2para->uiUart2RxCnt<MOTO_LENTH)comm2para->uiUart2RxCnt++;
	if(TESTToMD->TEST_u8Uart2RecData[HEAD]!=SYNC_CODE)
		{
		comm2para->uiUart2RxCnt = 0;
		TESTToMD->TEST_u8Uart2RecData[HEAD] =0x00;
		}
	else if(comm2para->uiUart2RxCnt==2)
		{
		if((TESTToMD->TEST_u8Uart2RecData[ID]!=TEST_MCU_ID)&&(TESTToMD->TEST_u8Uart2RecData[ID]!=BLDC_MCU_ID))
			{
			comm2para->uiUart2RxCnt = 0;
			TESTToMD->TEST_u8Uart2RecData[HEAD] =0x00;
			TESTToMD->TEST_u8Uart2RecData[ID]  =0x00;
			}
		}
	else if(comm2para->uiUart2RxCnt==3)
		{
		if(!(((TESTToMD->TEST_u8Uart2RecData[LENTH]==TEST_MCU_LENTH)&&(TESTToMD->TEST_u8Uart2RecData[ID]==TEST_MCU_ID))\
			||((TESTToMD->TEST_u8Uart2RecData[LENTH]==BLDC_MCU_LENTH)&&(TESTToMD->TEST_u8Uart2RecData[ID]==BLDC_MCU_ID))))
			{
			comm2para->uiUart2RxCnt = 0;
			TESTToMD->TEST_u8Uart2RecData[HEAD] =0x00;
			TESTToMD->TEST_u8Uart2RecData[ID]  =0x00;
			}
		}					
	else if((comm2para->uiUart2RxCnt==T_MCU_LENTH)&&(TESTToMD->TEST_u8Uart2RecData[ID]==TEST_MCU_ID))
		{
		uiU2RxChecksum = 0;
		for(k=1;k<(sizeof(TESTToMD->TEST_u8Uart2RecData)-2);k++)
			{
			uiU2RxChecksum += TESTToMD->TEST_u8Uart2RecData[k];
			}
		uiU2Temp = (uint16_t)((TESTToMD->TEST_u8Uart2RecData[TEST_CHECKSUM_H]<<8) + TESTToMD->TEST_u8Uart2RecData[TEST_CHECKSUM_L]);
		if(uiU2RxChecksum == uiU2Temp)
			{
			Updata_ReceTestUartData(TESTToMD);
			comm2para->bSendTestEnable = 1;
			comm2para->uiUart2RxTestTimeOutCnt = 0;
			}							
		comm2para->uiUart2RxCnt = 0;
		}
	else if((comm2para->uiUart2RxCnt==MOTO_LENTH)&&(TESTToMD->TEST_u8Uart2RecData[ID]==BLDC_MCU_ID))
		{
		uiU2RxChecksum = 0;
		for(k=1;k<(sizeof(TESTToMD->TEST_u8Uart2RecData)-2);k++)
			{
			uiU2RxChecksum += TESTToMD->TEST_u8Uart2RecData[k];
			}
		uiU2Temp = (uint16_t)((TESTToMD->TEST_u8Uart2RecData[RM_CHECKSUM_H]<<8) + TESTToMD->TEST_u8Uart2RecData[RM_CHECKSUM_L]);
		if(uiU2RxChecksum == uiU2Temp)
			{
		    for(i=0;i<MOTO_LENTH;i++)
		    	{
		    	 BLDCToMD->BLDC_u8Uart2RecData[i]=TESTToMD->TEST_u8Uart2RecData[i];
		    	}
			Updata_ReceBLDCUartData(BLDCToMD);
			comm2para->bBldcCommErrorFlag = 0;
			comm2para->uiUart2RxTimeOutCnt = 0;
			}							
		comm2para->uiUart2RxCnt = 0;
		}
}

void OverTime_CommTEST(UART2Comm* comm2para)
{
	if(++comm2para->uiUart2RxTestTimeOutCnt>=UART2_COMM_TEST_TIMEOUT)
		{
		comm2para->uiUart2RxTestTimeOutCnt = 0;
		comm2para->bSendTestEnable =0;
		} 
}

