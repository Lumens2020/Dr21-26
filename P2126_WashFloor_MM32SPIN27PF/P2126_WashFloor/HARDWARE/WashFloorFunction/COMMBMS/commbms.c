#include"commbms.h"
#include"uart.h"
#include"control.h"
#include "improve.h"

//****************�ϴ�BMS���ݸ���*******************
///*���ܣ�      10msִ��һ�� �ϴ���BMS���ݸ���			 						*///
///*��ڲ�����	MCUtoBMSData																	*///
///*���ڲ�����	��																				*///
//************************************************************
void Updata_SendBMSUartData(MCUtoBMSData* MToBMSD)
{
	uint16_t uiTempCheckSum;
	uint8_t  uclenth,i;
	
	MToBMSD->xpMcuToBmsBuff[HEAD]    = SYNC_CODE;
	MToBMSD->xpMcuToBmsBuff[ID]	     = MCU_BMS_ID;
	MToBMSD->xpMcuToBmsBuff[LENTH]   = MCU_BMS_LENTH;
	MToBMSD->xpMcuToBmsBuff[TxDATA1] = MToBMSD->xpWorkStatus;
	MToBMSD->xpMcuToBmsBuff[TxDATA2] = MToBMSD->xpKeyOnOffStatus;
	MToBMSD->xpMcuToBmsBuff[TxDATA3] = MToBMSD->xpKeyPushStatus;
	uiTempCheckSum = 0;
	uclenth = sizeof(MToBMSD->xpMcuToBmsBuff)-2;
	for(i=1;i<uclenth;i++)
	{
		uiTempCheckSum += MToBMSD->xpMcuToBmsBuff[i];
	}
	MToBMSD->xpMcuToBmsBuff[B_CHECKSUM_H] = (uint8_t)((uiTempCheckSum&0xFF00)>>8);
	MToBMSD->xpMcuToBmsBuff[B_CHECKSUM_L] = (uint8_t)(uiTempCheckSum&0x00FF);	
}

//****************����BMS���ݸ���*******************
///*���ܣ�      10msִ��һ�� ����BMS���ݸ���			 						*///
///*��ڲ�����	BMStoMCUData																	*///
///*���ڲ�����	��																				*///
//************************************************************
void Updata_ReceBMSUartData(BMStoMCUData* BMSToMD)
{
	BMSToMD->xpWorkStatus     = BMSToMD->BMS_u8Uart1RecData[RxDATA1];     //����BMS���͸����ذ����Ϣ��CHARGE_STA
	BMSToMD->xpBatSocVal      = BMSToMD->BMS_u8Uart1RecData[RxDATA2];
	BMSToMD->xpBatTempVal     = BMSToMD->BMS_u8Uart1RecData[RxDATA3];
	BMSToMD->xpErrorStatus    = BMSToMD->BMS_u8Uart1RecData[RxDATA4];
	BMSToMD->xpWarnStatus     = BMSToMD->BMS_u8Uart1RecData[RxDATA5];
	BMSToMD->xpKeyOnOffStatus = BMSToMD->BMS_u8Uart1RecData[RxDATA6];
	BMSToMD->xpBatVolH        = BMSToMD->BMS_u8Uart1RecData[RxDATA7];
	BMSToMD->xpBatVolL        = BMSToMD->BMS_u8Uart1RecData[RxDATA8];
	BMSToMD->reserve0         = BMSToMD->BMS_u8Uart1RecData[RxDATA9];
	BMSToMD->reserve1         = BMSToMD->BMS_u8Uart1RecData[RxDATA10];
	Deal_CommStatus();
}

//****************��ذ��������ݽ��պ���*******************
///*���ܣ�      10msִ��һ�� ����Э����յ�ذ�����		 						*///
///*��ڲ�����	BMStoMCUData      UART1Comm																	*///
///*���ڲ�����	��																				*///
//************************************************************

//void Deal_ReceBMSUartData(BMStoMCUData* BMSToMD,UART1Comm* comm1para)
void Deal_ReceBMSUartData(BMStoMCUData* BMSToMD,UART1Comm* comm1para,FCTtoMCUData* FCTToMD,FCT_UART1Comm* fctcomm1para)
{
	uint8_t k;
	uint16_t uiU1Checksum,uiU1Temp;

	BMSToMD->BMS_u8Uart1RecData[comm1para->uiUart1RxCnt] = UART_ReceiveData(UART1);
	FCTToMD->FCT_u8Uart1RecData[fctcomm1para->uiUart1RxCnt] = BMSToMD->BMS_u8Uart1RecData[comm1para->uiUart1RxCnt];// 20211207

	if(comm1para->uiUart1RxCnt<BAT_LENTH)comm1para->uiUart1RxCnt++;
	if(BMSToMD->BMS_u8Uart1RecData[HEAD]!=SYNC_CODE)
	{
		comm1para->uiUart1RxCnt = 0;
		BMSToMD->BMS_u8Uart1RecData[HEAD] =0x00;
	}
	else if(comm1para->uiUart1RxCnt==2)
	{
		if(BMSToMD->BMS_u8Uart1RecData[ID]!=BMS_MCU_ID)
		{
			comm1para->uiUart1RxCnt = 0;
			BMSToMD->BMS_u8Uart1RecData[HEAD] =0x00;
			BMSToMD->BMS_u8Uart1RecData[ID]=0x00;
		}
	}
	else if(comm1para->uiUart1RxCnt==3)
	{
		if(BMSToMD->BMS_u8Uart1RecData[LENTH]!=BMS_MCU_LENTH)
		{
			comm1para->uiUart1RxCnt = 0;
			BMSToMD->BMS_u8Uart1RecData[HEAD] =0x00;
			BMSToMD->BMS_u8Uart1RecData[ID]=0x00;
		}
	}					
	else if(comm1para->uiUart1RxCnt==BAT_LENTH)
	{
		uiU1Checksum = 0;
		for(k=1;k<(sizeof(BMSToMD->BMS_u8Uart1RecData)-2);k++)
		{
			uiU1Checksum += BMSToMD->BMS_u8Uart1RecData[k];
		}
		uiU1Temp = (uint16_t)((BMSToMD->BMS_u8Uart1RecData[RB_CHECKSUM_H]<<8) + BMSToMD->BMS_u8Uart1RecData[RB_CHECKSUM_L]);
		if(uiU1Checksum == uiU1Temp)
		{
			Updata_ReceBMSUartData(BMSToMD);
			comm1para->uiUart1RxTimeOutCnt 	 = 0;
			comm1para->bBmsCommErrorFlag = 0;
			if(UART1SendRespondCnt >0)
			{
				UART1SendRespondCnt--;
			}
			bms_comm_succeed_clear_error();
		}							
		comm1para->uiUart1RxCnt = 0;
	}	
}

//****************��ذ��������ݽ��ճ�ʱ��⺯��*******************
///*���ܣ�      10msִ��һ�� ���յ�ذ����ݳ�ʱ���		 						*///
///*��ڲ�����	UART1Comm																	*///
///*���ڲ�����	��																				*///
//************************************************************

void OverTime_CommBMS(UART1Comm* comm1para)
{
	if(++comm1para->uiUart1RxTimeOutCnt>=UART1_COMM_TIMEOUT)
		{
			BmsCommSucClearErrCnt = 0;
			comm1para->uiUart1RxTimeOutCnt = 0;
			comm1para->uiUart1RxCnt= 0;
			comm1para->bBmsCommErrorFlag = 1;
		} 
}

