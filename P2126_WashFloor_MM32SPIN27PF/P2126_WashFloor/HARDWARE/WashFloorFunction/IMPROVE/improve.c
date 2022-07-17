#include "improve.h"
extern BLDCtoMCUData  BLDCtoMData;
extern BMStoMCUData   BMStoMData;
extern MCUtoLCDData   MtoLCDData;
extern MCUtoBMSData   MtoBMSData;

extern DryPara        DryVar;
extern WaterElecPara  WaterElecVar;
extern BrushPara      BrushVar;
extern PumpPara       PumpVar;
extern WorkError      ErrorVar;
extern WorkPara       WorkVar;
extern VoiceList      VoiceListpara;

extern WaterBoxStatus WaterBSta;
extern TimePara       TimeVar;
extern TILTPara       TiltVar;

extern AutoCleanValue ACleanVar;

extern UART1Comm    UART1COMMpara;
extern UART2Comm    UART2COMMpara;

extern CHGPara      CHGVar;

extern KeyPara        KeyVar;

extern SewagePara     SewageVar;
extern ADCPara        ADCVar; 
extern Eeprom_TypeDef E2prom;

BaudRateCha BaudRateChangeF;

uint32_t BmsBaudRateNum = 0;							//bms要变化的波特率
uint32_t MonobusBaudRateNum = 0;					//单总线要变化的波特率
uint32_t UART1SendRespondCnt = 0;					//发送电池包数据计次
uint32_t UART2SendRespondCnt = 0;					//发送单总线数据计次
uint16_t BaudRateChangeCnt = 0;						//波特率改变计次
uint32_t BmsCommSucClearErrCnt = 0;					//与bms成功5s通讯错误清除
uint32_t MonobusCommSucClearErrCnt = 0;			//与单总线成功通讯5s错误清除

void error_clear(void)
{
	MtoLCDData.xpErrorStatus = 0;		
	MtoLCDData.xpBrushStatus=BRUSH_YES_STA;//20210909
 	BrushVar.bBrushNGErrFlag=0;
	BrushVar.bBrushShortErrFlag=0;
	BrushVar.bBrushStallErrFlag=0;
	
	PumpVar.bPumpShortErrFlag=0;
  DryVar.bDryStallErrFlag=0;
	WaterElecVar.bWaterElecStallErrFlag=0;
	
	ErrorVar.xpRunErrorFlag=0;
	ErrorVar.xpServiceErrorFlag=0;
	ErrorVar.xpCommErrorFlag=0;

	MtoLCDData.xpErrorStatus=0;
	MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;
	MtoLCDData.xpBrushStatus=BRUSH_YES_STA;

	UART1COMMpara.bBmsCommErrorFlag=0;
	UART1COMMpara.uiUart1RxTimeOutCnt=0;
	UART2COMMpara.bBldcCommErrorFlag=0;
	UART2COMMpara.uiUart2RxTimeOutCnt=0;

	ErrorVar.xpErrorPlayCnt=0;
	ErrorVar.xpErrorPlayCnt1=0;
	ErrorVar.xpErrorPlayCnt2=0;
	ErrorVar.xpErrorPlayCnt3=0;
	ErrorVar.xpErrorPlayCnt4=0;
	ErrorVar.xpErrorPlayCnt5=0;
	ErrorVar.xpErrorPlayCnt6=0;
	ErrorVar.xpErrorPlayCnt7=0;
	ErrorVar.xpErrorPlayCnt8=0;
	ErrorVar.xpErrorPlayCnt9=0;	


}


//已于供应商确认初始化
void baudrate_bms_init(uint32_t BmsBandRate)
{
	
		 UART1_Init(BmsBandRate);
//		 UART2_Init(BmsBandRate);
}


void baudrate_monobus_init(uint32_t MonobusBandRate)
{
//		 UART1_Init(MonobusBandRate);
		 UART2_Init(MonobusBandRate);
}


void baudrate_handle_bms(void)
{
	if(UART1COMMpara.uiUart1RxTimeOutCnt > UART1_COMM_2S_CNT)	
	{
		if(UART1SendRespondCnt<=(UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag1 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag1 = 1;
			}
		}
		else	if(UART1SendRespondCnt<=(2*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+2*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag2 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag2 = 1;
			}			
		}
		else	if(UART1SendRespondCnt<=(3*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+3*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag3 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag3 = 1;
			}
		}		
		else	if(UART1SendRespondCnt<=(4*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+4*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag4 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag4 = 1;
			}
		}		
		else	if(UART1SendRespondCnt<=(5*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+5*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag5 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag5 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(6*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE+6*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag6 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag6 = 1;
			}
		}
		else	if(UART1SendRespondCnt<=(7*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag7 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag7 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(8*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-2*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag8 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag8 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(9*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-3*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag9 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag9 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(10*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-4*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag10 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag10 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(11*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-5*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag11 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag11 = 1;
			}
		}	
		else	if(UART1SendRespondCnt<=(12*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum	= DEFAULT_BAUDRATE-6*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.BmsBaudRateChangeFlag12 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag12 = 1;
			}
		}		
		else if(UART1SendRespondCnt<=(13*UART_BAUDRATE_CHANGE_CNT))
		{
			BmsBaudRateNum = DEFAULT_BAUDRATE;
			if(BaudRateChangeF.BmsBaudRateChangeFlag13 == 0)
			{
				baudrate_bms_init(BmsBaudRateNum);
				BaudRateChangeF.BmsBaudRateChangeFlag13 = 1;
			}			
		}
		else
		{
			
			UART1SendRespondCnt = 0;
			bms_change_baudrate_clear();
		}

		
	}
	else
	{
		UART1SendRespondCnt = 0;
		bms_change_baudrate_clear();
	}


}



void bms_change_baudrate_clear(void)
{
	BaudRateChangeF.BmsBaudRateChangeFlag1=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag2=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag3=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag4=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag5=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag6=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag7=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag8=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag9=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag10=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag11=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag12=0;	
	BaudRateChangeF.BmsBaudRateChangeFlag13=0;	
}



void baudrate_handle_monobus(void)
{
	if(UART2COMMpara.uiUart2RxTimeOutCnt > UART2_COMM_2S_CNT)	
	{
		if(UART2SendRespondCnt<=(UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag1 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag1 = 1;
			}
		}
		else	if(UART2SendRespondCnt<=(2*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+2*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag2 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag2 = 1;
			}			
		}
		else	if(UART2SendRespondCnt<=(3*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+3*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag3 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag3 = 1;
			}
		}		
		else	if(UART2SendRespondCnt<=(4*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+4*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag4 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag4 = 1;
			}
		}		
		else	if(UART2SendRespondCnt<=(5*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+5*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag5 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag5 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(6*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE+6*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag6 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag6 = 1;
			}
		}
		else	if(UART2SendRespondCnt<=(7*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag7 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag7 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(8*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-2*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag8 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag8 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(9*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-3*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag9 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag9 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(10*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-4*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag10 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag10 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(11*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-5*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag11 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag11 = 1;
			}
		}	
		else	if(UART2SendRespondCnt<=(12*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum	= DEFAULT_BAUDRATE-6*BAUDRATE_ADD_NUM;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag12 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag12 = 1;
			}
		}		
		else if(UART2SendRespondCnt<=(13*UART_BAUDRATE_CHANGE_CNT))
		{
			MonobusBaudRateNum = DEFAULT_BAUDRATE;
			if(BaudRateChangeF.MonobusBaudRateChangeFlag13 == 0)
			{
				baudrate_monobus_init(MonobusBaudRateNum);
				BaudRateChangeF.MonobusBaudRateChangeFlag13 = 1;
			}			
		}
		else
		{
			
			UART2SendRespondCnt = 0;
			monobus_change_baudrate_clear();
		}
		

	}
	else
	{
		UART2SendRespondCnt = 0;
		monobus_change_baudrate_clear();
	}


}





void monobus_change_baudrate_clear(void)
{
	BaudRateChangeF.MonobusBaudRateChangeFlag1=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag2=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag3=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag4=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag5=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag6=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag7=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag8=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag9=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag10=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag11=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag12=0;	
	BaudRateChangeF.MonobusBaudRateChangeFlag13=0;	
}


void bms_comm_succeed_clear_error(void)
{
	if(BmsCommSucClearErrCnt<60000)
	{
		BmsCommSucClearErrCnt++;
	}
	
	if(MtoLCDData.xpErrorStatus==BMS_COMM_ERROR)
	{
		if(BmsCommSucClearErrCnt>10)
		{
			error_clear();
//			ErrorVar.xpServiceErrorFlag=0;
			BmsCommSucClearErrCnt = 0;
//			ErrorVar.xpCommErrorFlag = 0;
			MtoLCDData.xpErrorStatus = NO_ERROR;
		}
	}

}

void monobus_comm_succeed_clear_error(void)
{
	if(MonobusCommSucClearErrCnt<60000)
	{
		MonobusCommSucClearErrCnt++;
	}
	
	if(MtoLCDData.xpErrorStatus==MCU_COMM_ERROR)
	{
		if(MonobusCommSucClearErrCnt>10)
		{
			error_clear();
//			ErrorVar.xpServiceErrorFlag=0;
			MonobusCommSucClearErrCnt = 0;
//			ErrorVar.xpCommErrorFlag = 0;
			MtoLCDData.xpErrorStatus = NO_ERROR;
		}
	}

}


