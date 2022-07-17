#include"commbldc.h"
#include"uart.h"
#include"bldc.h"
#include "improve.h"

//****************上传主电机数据更新*******************
///*功能：      10ms执行一次 上传给BLDC数据更新			 						*///
///*入口参数：	BLDCCtl     MCUtoBLDCData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_SendBLDCUartData(MCUtoBLDCData* MToBldcD)
{
	uint16_t uiTempCheckSum;
	uint8_t  uclenth,i;

	MToBldcD->xpMcuToBldcBuff[HEAD]    = SYNC_CODE;
	MToBldcD->xpMcuToBldcBuff[ID]      = MCU_BLDC_ID;
	MToBldcD->xpMcuToBldcBuff[LENTH]   = MCU_BLDC_LENTH;
	MToBldcD->xpMcuToBldcBuff[TxDATA1] =MToBldcD->xpworkgear;
	MToBldcD->xpMcuToBldcBuff[TxDATA1]|=(((uint8_t)MToBldcD->xpsewagelv)<<4);
	MToBldcD->xpMcuToBldcBuff[TxDATA2] =MToBldcD->xpWorkStatus;
	MToBldcD->xpMcuToBldcBuff[TxDATA3] =MToBldcD->reserve0;
	MToBldcD->xpMcuToBldcBuff[TxDATA4] =MToBldcD->reserve1;
	MToBldcD->xpMcuToBldcBuff[TxDATA5] =MToBldcD->reserve2;
	uclenth = sizeof(MToBldcD->xpMcuToBldcBuff)-2;
	uiTempCheckSum = 0;
	for(i=1;i<uclenth;i++)
	{
		uiTempCheckSum += MToBldcD->xpMcuToBldcBuff[i];
	}
	MToBldcD->xpMcuToBldcBuff[M_CHECKSUM_H] = (uint8_t)((uiTempCheckSum&0xFF00)>>8);
	MToBldcD->xpMcuToBldcBuff[M_CHECKSUM_L] = (uint8_t)(uiTempCheckSum&0x00FF);
}
//***************接收主电机数据更新*******************
///*功能：      10ms执行一次 接收BLDC数据更新			 						*///
///*入口参数：	BLDCtoMCUData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_ReceBLDCUartData(BLDCtoMCUData* BldcToMD)
{
	BldcToMD->xpBldcSpeedH =BldcToMD->BLDC_u8Uart2RecData[RxDATA1]; 
	BldcToMD->xpBldcSpeedM =BldcToMD->BLDC_u8Uart2RecData[RxDATA2];
	BldcToMD->xpBldcSpeedL =BldcToMD->BLDC_u8Uart2RecData[RxDATA3];
	BldcToMD->xpBldcTempVal=BldcToMD->BLDC_u8Uart2RecData[RxDATA4];
	BldcToMD->xpErrorStatus=BldcToMD->BLDC_u8Uart2RecData[RxDATA5]; 
	BldcToMD->reserve0     =BldcToMD->BLDC_u8Uart2RecData[RxDATA6]; 
	BldcToMD->reserve1     =BldcToMD->BLDC_u8Uart2RecData[RxDATA7];
	BldcToMD->reserve2     =BldcToMD->BLDC_u8Uart2RecData[RxDATA8];
	BldcToMD->reserve3     =BldcToMD->BLDC_u8Uart2RecData[RxDATA9];
	BldcToMD->reserve4     =BldcToMD->BLDC_u8Uart2RecData[RxDATA10];
}

//****************主电机串口数据接收函数*******************
///*功能：      10ms执行一次 按照协议接收主电机数据		 						*///
///*入口参数：	BLDCtoMCUData      UART2Comm																	*///
///*出口参数：	无																				*///
//************************************************************

void Deal_ReceBldcUartData(BLDCtoMCUData* BLDCToMD,UART2Comm* comm2para)
{
	uint8_t k;
	uint16_t uiU2RxChecksum,uiU2Temp;

	BLDCToMD->BLDC_u8Uart2RecData[comm2para->uiUart2RxCnt] = UART_ReceiveData(UART2);
	
	if(comm2para->uiUart2RxCnt<MOTO_LENTH)comm2para->uiUart2RxCnt++;
	if(BLDCToMD->BLDC_u8Uart2RecData[HEAD]!=SYNC_CODE)
	{
		comm2para->uiUart2RxCnt = 0;
		BLDCToMD->BLDC_u8Uart2RecData[HEAD] =0x00;
	}
	else if(comm2para->uiUart2RxCnt==2)
	{
		if(BLDCToMD->BLDC_u8Uart2RecData[ID]!=BLDC_MCU_ID)
		{
			comm2para->uiUart2RxCnt = 0;
			BLDCToMD->BLDC_u8Uart2RecData[HEAD] =0x00;
			BLDCToMD->BLDC_u8Uart2RecData[ID]  =0x00;
		}
	}
	else if(comm2para->uiUart2RxCnt==3)
	{
		if(BLDCToMD->BLDC_u8Uart2RecData[LENTH]!=0x0A)
		{
			comm2para->uiUart2RxCnt = 0;
			BLDCToMD->BLDC_u8Uart2RecData[HEAD] =0x00;
			BLDCToMD->BLDC_u8Uart2RecData[ID]  =0x00;
		}
	}					
	else if(comm2para->uiUart2RxCnt==MOTO_LENTH)
	{
		uiU2RxChecksum = 0;
		for(k=1;k<(sizeof(BLDCToMD->BLDC_u8Uart2RecData)-2);k++)
		{
			uiU2RxChecksum += BLDCToMD->BLDC_u8Uart2RecData[k];
		}
		uiU2Temp = (uint16_t)((BLDCToMD->BLDC_u8Uart2RecData[RM_CHECKSUM_H]<<8) + BLDCToMD->BLDC_u8Uart2RecData[RM_CHECKSUM_L]);
		if(uiU2RxChecksum == uiU2Temp)
		{
			Updata_ReceBLDCUartData(BLDCToMD);
			comm2para->bBldcCommErrorFlag = 0;
			comm2para->uiUart2RxTimeOutCnt = 0;
			/////////////////////////////20220322//////////////////////////////////////
			if(UART2SendRespondCnt >0)
			{
				UART2SendRespondCnt--;
			}
			monobus_comm_succeed_clear_error();
		}							
		comm2para->uiUart2RxCnt = 0;
	}

}

//****************主电机串口数据接收超时检测函数*******************
///*功能：      10ms执行一次 接收主电机数据超时检测		 						*///
///*入口参数：	UART2Comm																	*///
///*出口参数：	无																				*///
//************************************************************
void OverTime_CommBLDC(UART2Comm* comm2para)
{
	if(++comm2para->uiUart2RxTimeOutCnt>=UART2_COMM_TIMEOUT)
		{
			/////////////////////////////20220322//////////////////////////////////////
			MonobusCommSucClearErrCnt = 0;
			comm2para->uiUart2RxTimeOutCnt = 0;
			comm2para->uiUart2RxCnt= 0;
			comm2para->bBldcCommErrorFlag = 1;
		} 
}

