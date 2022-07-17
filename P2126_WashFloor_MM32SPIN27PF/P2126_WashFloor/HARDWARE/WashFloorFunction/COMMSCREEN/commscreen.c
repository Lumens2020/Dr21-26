#include"commscreen.h"
#include"uart.h"

//****************上传屏幕数据更新*******************
///*功能：      10ms执行一次 上传给屏幕数据更新			 						*///
///*入口参数：	MCUtoLCDData																	*///
///*出口参数：	无																				*///
//************************************************************
void Updata_SendLCDUartData(MCUtoLCDData* MToLCDD)
{
	uint16_t uiTempCheckSum;
	uint8_t  uclenth,i;
	
	MToLCDD->xpMcuToLcdBuff[HEAD]    = SYNC_CODE;
	MToLCDD->xpMcuToLcdBuff[ID]	     = MCU_LCD_ID;
	MToLCDD->xpMcuToLcdBuff[LENTH]   = MCU_LCD_LENTH;
	#if LCDScreenEnableE==1
	MToLCDD->xpMcuToLcdBuff[TxDATA1] = MToLCDD->xpworkgear;
	MToLCDD->xpMcuToLcdBuff[TxDATA1]|= (((uint8_t)MToLCDD->xpworkgeartime)<<5);
	MToLCDD->xpMcuToLcdBuff[TxDATA2] = MToLCDD->xpBatSocVal;
	MToLCDD->xpMcuToLcdBuff[TxDATA3] = MToLCDD->xpErrorStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA4] = MToLCDD->xpWorkStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA4]|= (((uint8_t)MToLCDD->xpWorkErrorDisEnable)<<3);
	MToLCDD->xpMcuToLcdBuff[TxDATA4]|= (((uint8_t)MToLCDD->xpLCDStatus)<<4);
	MToLCDD->xpMcuToLcdBuff[TxDATA4]|= (((uint8_t)MToLCDD->xpMachineStatus)<<6);
	MToLCDD->xpMcuToLcdBuff[TxDATA5] = MToLCDD->xpQWaterBoxStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA6] = MToLCDD->xpWWaterBoxStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA7] = MToLCDD->xpBrushStatus;	
	MToLCDD->xpMcuToLcdBuff[TxDATA8] = MToLCDD->xpACleanStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA8]|= (((uint8_t)MToLCDD->xpACleanWarnStatus)<<4);
	MToLCDD->xpMcuToLcdBuff[TxDATA9] = MToLCDD->xpDryStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA9]|= (((uint8_t)MToLCDD->xpWaterEStatus)<<4);
	MToLCDD->xpMcuToLcdBuff[TxDATA10]= MToLCDD->xpDryTimeS_H;
	MToLCDD->xpMcuToLcdBuff[TxDATA11]= MToLCDD->xpDryTimeS_L;
	MToLCDD->xpMcuToLcdBuff[TxDATA12]= MToLCDD->xpWaterETimeS_H;
	MToLCDD->xpMcuToLcdBuff[TxDATA13]= MToLCDD->xpWaterETimeS_L;
	MToLCDD->xpMcuToLcdBuff[TxDATA14]= MToLCDD->xpACleanTimeS_H;
	MToLCDD->xpMcuToLcdBuff[TxDATA15]= MToLCDD->xpACleanTimeS_L;
	MToLCDD->xpMcuToLcdBuff[TxDATA16]= MToLCDD->xpSewagelv;
	MToLCDD->xpMcuToLcdBuff[TxDATA16]|= (((uint8_t)MToLCDD->xpSewageStatus)<<4);
	#else
	MToLCDD->xpMcuToLcdBuff[TxDATA1] = MToLCDD->xpworkgear;
	MToLCDD->xpMcuToLcdBuff[TxDATA1]|= (((uint8_t)MToLCDD->xpsewagelv)<<4);
	MToLCDD->xpMcuToLcdBuff[TxDATA2] = MToLCDD->xpBatSocVal;
	MToLCDD->xpMcuToLcdBuff[TxDATA3] = MToLCDD->xpErrorStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA4] = MToLCDD->xpWorkStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA5] = MToLCDD->xpQWaterBoxStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA6] = MToLCDD->xpWWaterBoxStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA7] = MToLCDD->xpBrushStatus;	
	MToLCDD->xpMcuToLcdBuff[TxDATA8] = MToLCDD->xpACleanStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA9] = MToLCDD->xpLCDStatus;
	MToLCDD->xpMcuToLcdBuff[TxDATA10]= MToLCDD->reserve;
	#endif
	uiTempCheckSum = 0;
	uclenth = sizeof(MToLCDD->xpMcuToLcdBuff)-2;
	for(i=1;i<uclenth;i++)
	{
		uiTempCheckSum += MToLCDD->xpMcuToLcdBuff[i];
	}
	MToLCDD->xpMcuToLcdBuff[L_CHECKSUM_H] = (uint8_t)((uiTempCheckSum&0xFF00)>>8);
	MToLCDD->xpMcuToLcdBuff[L_CHECKSUM_L] = (uint8_t)(uiTempCheckSum&0x00FF);	
}


