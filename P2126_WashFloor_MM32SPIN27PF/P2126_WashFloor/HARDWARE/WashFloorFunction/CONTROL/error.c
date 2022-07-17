#include "error.h"
#include "brush.h"
#include "waterpump.h"
#include "sewage.h"
#include "cleanwater.h"
#include "waterelec.h"
#include "commbms.h"
#include "commbldc.h"
#include "commscreen.h"
#include "voicelist.h"
#include "voice.h"
#include "control.h"
#include "timer.h"
#include "tilt.h"
#include "uart.h"
#include "charge.h"
#include "key.h"
#include "keyad.h"
#include "adc.h"
#include "gpio.h"
#include "dry.h"
#include "waterelec.h"
#include "sewage.h"
#include "eeprom.h"

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

#if TEST_AD_ENABLE==1
void Remember_WarnData(void)
{
	if(ErrorVar.xpErrorMemoryEnable)
	 {
		  ErrorVar.xpErrorMemoryEnable=0;
		  if(ErrorVar.xpErrorMemoryCnt<5)
			 {
			 ErrorVar.xpErrorMemoryCnt++;
			 }
		  else //if(ErrorVar.xpErrorMemoryCnt>5)
			 {
			 ErrorVar.xpErrorMemoryCnt=1;
			 }
		  E2prom.uiWriteDataFlash[WARN_CNT]=ErrorVar.xpErrorMemoryCnt;
		  E2prom.bEnableEEpromFlag=1;
		  if(ErrorVar.xpErrorMemoryCnt==1)
			 {
			  if((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE0]=BLDCtoMData.xpErrorStatus; 
				 }
			  else if(((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE0]=BMStoMData.xpErrorStatus; 
				 }
			  else if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
					||ErrorVar.xpCommErrorFlag)
				 {
				  if(BrushVar.bBrushShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE0]=BRUSH_ERROR; 
				  else if(PumpVar.bPumpShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE0]=PUMP_ERROR; 
				  else if(DryVar.bDryStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE0]=DRY_ERROR; 
				  else if(WaterElecVar.bWaterElecStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE0]=WATERELEC_ERROR; 
				  else if(ErrorVar.xpCommErrorFlag)
					 {
					  E2prom.uiWriteDataFlash[WARN_TYPE0]=MtoLCDData.xpErrorStatus; 
					 }
				 }
			  E2prom.uiWriteDataFlash[WARN_AD_WaterPump0]  = ADCVar.uiWaterPumpAdcValue;  
			  E2prom.uiWriteDataFlash[WARN_AD_Brush0]	   = ADCVar.uiBrushAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_Key0] 	   = ADCVar.uiKeyAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_SewageLV0]   = ADCVar.uiSewageLVAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_SewageStage0]= ADCVar.uiSewageStageAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_CHGVol0]	   = ADCVar.uiCHGAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_WaterLV0]    = ADCVar.uiCleanWaterAdcValue;	  
			 }
		  else if(ErrorVar.xpErrorMemoryCnt==2)
			 {
			  if((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE1]=BLDCtoMData.xpErrorStatus; 
				 }
			  else if(((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE1]=BMStoMData.xpErrorStatus; 
				 }
			  else if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
					||ErrorVar.xpCommErrorFlag)
				 {
				  if(BrushVar.bBrushShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE1]=BRUSH_ERROR; 
				  else if(PumpVar.bPumpShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE1]=PUMP_ERROR; 
				  else if(DryVar.bDryStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE1]=DRY_ERROR; 
				  else if(WaterElecVar.bWaterElecStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE1]=WATERELEC_ERROR; 
				  else if(ErrorVar.xpCommErrorFlag)
					 {
					  E2prom.uiWriteDataFlash[WARN_TYPE1]=MtoLCDData.xpErrorStatus; 
					 }
				 }
			  E2prom.uiWriteDataFlash[WARN_AD_WaterPump1]  = ADCVar.uiWaterPumpAdcValue;  
			  E2prom.uiWriteDataFlash[WARN_AD_Brush1]	   = ADCVar.uiBrushAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_Key1] 	   = ADCVar.uiKeyAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_SewageLV1]   = ADCVar.uiSewageLVAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_SewageStage1]= ADCVar.uiSewageStageAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_CHGVol1]	   = ADCVar.uiCHGAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_WaterLV1]    = ADCVar.uiCleanWaterAdcValue;	  
			 }
		  else if(ErrorVar.xpErrorMemoryCnt==3)
			 {
			  if((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE2]=BLDCtoMData.xpErrorStatus; 
				 }
			  else if(((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE2]=BMStoMData.xpErrorStatus; 
				 }
			  else if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
					||ErrorVar.xpCommErrorFlag)
				 {
				  if(BrushVar.bBrushShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE2]=BRUSH_ERROR; 
				  else if(PumpVar.bPumpShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE2]=PUMP_ERROR; 
				  else if(DryVar.bDryStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE2]=DRY_ERROR; 
				  else if(WaterElecVar.bWaterElecStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE2]=WATERELEC_ERROR; 
				  else if(ErrorVar.xpCommErrorFlag)
					 {
					  E2prom.uiWriteDataFlash[WARN_TYPE2]=MtoLCDData.xpErrorStatus; 
					 }
				 }
			  E2prom.uiWriteDataFlash[WARN_AD_WaterPump2]  = ADCVar.uiWaterPumpAdcValue;  
			  E2prom.uiWriteDataFlash[WARN_AD_Brush2]	   = ADCVar.uiBrushAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_Key2] 	   = ADCVar.uiKeyAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_SewageLV2]   = ADCVar.uiSewageLVAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_SewageStage2]= ADCVar.uiSewageStageAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_CHGVol2]	   = ADCVar.uiCHGAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_WaterLV2]    = ADCVar.uiCleanWaterAdcValue;	  
			 }
		  else if(ErrorVar.xpErrorMemoryCnt==4)
			 {
			  if((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE3]=BLDCtoMData.xpErrorStatus; 
				 }
			  else if(((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE3]=BMStoMData.xpErrorStatus; 
				 }
			  else if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
					||ErrorVar.xpCommErrorFlag)
				 {
				  if(BrushVar.bBrushShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE3]=BRUSH_ERROR; 
				  else if(PumpVar.bPumpShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE3]=PUMP_ERROR; 
				  else if(DryVar.bDryStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE3]=DRY_ERROR; 
				  else if(WaterElecVar.bWaterElecStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE3]=WATERELEC_ERROR; 
				  else if(ErrorVar.xpCommErrorFlag)
					 {
					  E2prom.uiWriteDataFlash[WARN_TYPE3]=MtoLCDData.xpErrorStatus; 
					 }
				 }
			  E2prom.uiWriteDataFlash[WARN_AD_WaterPump3]  = ADCVar.uiWaterPumpAdcValue;  
			  E2prom.uiWriteDataFlash[WARN_AD_Brush3]	   = ADCVar.uiBrushAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_Key3] 	   = ADCVar.uiKeyAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_SewageLV3]   = ADCVar.uiSewageLVAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_SewageStage3]= ADCVar.uiSewageStageAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_CHGVol3]	   = ADCVar.uiCHGAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_WaterLV3]    = ADCVar.uiCleanWaterAdcValue;	  
			 }
		  else if(ErrorVar.xpErrorMemoryCnt==5)
			 {
			  if((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE4]=BLDCtoMData.xpErrorStatus; 
				 }
			  else if(((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				 {
				 E2prom.uiWriteDataFlash[WARN_TYPE4]=BMStoMData.xpErrorStatus; 
				 }
			  else if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
					||ErrorVar.xpCommErrorFlag)
				 {
				  if(BrushVar.bBrushShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE4]=BRUSH_ERROR; 
				  else if(PumpVar.bPumpShortErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE4]=PUMP_ERROR; 
				  else if(DryVar.bDryStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE4]=DRY_ERROR; 
				  else if(WaterElecVar.bWaterElecStallErrFlag)E2prom.uiWriteDataFlash[WARN_TYPE4]=WATERELEC_ERROR; 
				  else if(ErrorVar.xpCommErrorFlag)
					 {
					  E2prom.uiWriteDataFlash[WARN_TYPE4]=MtoLCDData.xpErrorStatus; 
					 }
				 }
			  E2prom.uiWriteDataFlash[WARN_AD_WaterPump4]  = ADCVar.uiWaterPumpAdcValue;  
			  E2prom.uiWriteDataFlash[WARN_AD_Brush4]	   = ADCVar.uiBrushAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_Key4] 	   = ADCVar.uiKeyAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_SewageLV4]   = ADCVar.uiSewageLVAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_SewageStage4]= ADCVar.uiSewageStageAdcValue;	  
			  E2prom.uiWriteDataFlash[WARN_AD_CHGVol4]	   = ADCVar.uiCHGAdcValue; 
			  E2prom.uiWriteDataFlash[WARN_AD_WaterLV4]    = ADCVar.uiCleanWaterAdcValue;	  
			 }
		 }
}
#else
#endif

void Play_Service_ERROR(void)
{// 售后故障显示播报处理
#if TEST_AD_ENABLE==1
    if(WorkVar.ucWorkOrderStatus==CLOSE_STA)ErrorVar.xpErrorMemoryEnable=1;
#else
#endif
 if(((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))||\
			  ((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR))||\
			   BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag\
			   ||ErrorVar.xpCommErrorFlag)
 	{
 	 ErrorVar.xpServiceErrorFlag=1;
#if TEST_AD_ENABLE==1
	 Remember_WarnData();
#else
#endif
	 ///////////////20210910
	 #if LCDScreenEnableE==1
	 /*
	 if(pbKey_Spk==0&&ADCVar.uiADC_ResultBuff[Key_CHN]<CLEAN_KEY_AD)
	 	{
	 	MtoLCDData.xpWorkErrorDisEnable=1;
	 	}
	 else
	 	{
		 MtoLCDData.xpWorkErrorDisEnable=0;
	 	}
	 */
	 MtoLCDData.xpWorkErrorDisEnable=1;
	 #else
	 #endif
	 //////////////20210910
	 if(WorkVar.ucWorkOrderStatus!=CLOSE_STA)
	 	{
		 if((VoiceListpara.VoicePromptMachineErrorFlag==0)&&(ErrorVar.xpErrorPlayCnt<1))
		 	{
/////////////////////////////20220322//////////////////////////////////////
//		 	 VoiceListpara.VoicePromptMachineErrorFlag=1;//机器故障，请联系售后维修
			 WorkVar.ucCloseScreencnt=0;// 20220110
			 WorkVar.ucCloseMachinecnt=0;// 20220110
			 MtoBMSData.xpKeyPushStatus=1;// 20220110
		 	 ++ErrorVar.xpErrorPlayCnt;
		 	}
		 if(BMStoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BMStoMData.xpErrorStatus;
		 else if(BLDCtoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
		 else if(BrushVar.bBrushShortErrFlag)
		 	{
		 	 MtoLCDData.xpErrorStatus=BRUSH_SHORT_ERROR;
		 	}
		 else if(PumpVar.bPumpShortErrFlag)
		 	{
			 MtoLCDData.xpErrorStatus=PUMP_SHORT_ERROR;
		 	}
		 else if(DryVar.bDryStallErrFlag)
		 	{
			 MtoLCDData.xpErrorStatus=DRY_ERROR;
		 	}
		 else if(WaterElecVar.bWaterElecStallErrFlag)
		 	{
			 MtoLCDData.xpErrorStatus=WATERELEC_ERROR;
		 	}
//		 if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag||DryVar.bDryStallErrFlag||WaterElecVar.bWaterElecStallErrFlag)MtoLCDData.xpErrorStatus=BRUSH_SHORT_ERROR;
	 	}
 	}
 else
 	{
	 ErrorVar.xpErrorPlayCnt=0;
 	}
}

void Play_Running_ERROR(void)
{// 运行故障显示播报处理
////////////////20211011
 static uint8_t autocleanwarnCnt=0;
 if(WorkVar.ucWorkStatus==OPEN_STA)
 	{
 	if(autocleanwarnCnt&&(ACleanVar.xpAutoCleanPromptCount==2||ACleanVar.xpAutoCleanPromptFlag))
 		{
 		 autocleanwarnCnt=0;
// 	     VoiceListpara.VoicePromptReqCleanFlag=1;// 20211101
//	     MtoLCDData.xpACleanWarnStatus=4;//提示放回底座自清洁//20211101
 		}
 	}
 else if(WorkVar.ucWorkStatus==CLOSE_STA)
 	{
 	autocleanwarnCnt=1;
 	}
//////////////////////////
 if(WorkVar.ucWorkOrderStatus!=CLOSE_STA)
 	{
	 if(SewageVar.xpSewageWarnFlag)
	 	{
	 	 if((ErrorVar.xpErrorPlayCnt10<1)&&(VoiceListpara.VoicePromptCleanessDetFlag==0))
		 	{
		 	 VoiceListpara.VoicePromptCleanessDetFlag=1;
			 ErrorVar.xpErrorPlayCnt10++;
	 	 	}
	 	}
	 else
	 	{
	 	ErrorVar.xpErrorPlayCnt10=0;
	 	}
 	}
 else
 	{
 	ErrorVar.xpErrorPlayCnt10=0;
 	SewageVar.xpSewageWarnFlag=0;
 	}
 
 if(ErrorVar.xpServiceErrorFlag==0)// 20210913
//	 if((ErrorVar.xpServiceErrorFlag==0)&&(WorkVar.ucWorkOrderStatus!=CLOSE_STA))
 	{
 	 if(WorkVar.ucWorkOrderStatus==CHARGE_STA)
 	   {
		UART2COMMpara.bBldcCommErrorFlag=0;
		UART2COMMpara.uiUart2RxTimeOutCnt=0;
		 if((BMStoMData.xpErrorStatus==CHG_OV_VMS_ERROR)&&CHGVar.bChargingFlag)
			{
			 if(UART1COMMpara.bBmsCommErrorFlag)ErrorVar.xpBmsSleepErrorFlag=1;
			 else// 20220105
			 	{
			 	ErrorVar.xpBmsSleepErrorFlag=0;
			 	}
			}
		 else
		 	{
		 	 ErrorVar.xpBmsSleepErrorFlag=0;
			 UART1COMMpara.bBmsCommErrorFlag=0;
			 UART1COMMpara.uiUart1RxTimeOutCnt=0;
		 	}
 	 	}
	 else
	 	{
	 	    ErrorVar.xpBmsSleepErrorFlag=0;
	 	 	if(UART2COMMpara.bBldcCommErrorFlag)
				{
				 MtoLCDData.xpErrorStatus=MCU_COMM_ERROR;
				 ErrorVar.xpCommErrorFlag=1;
	 	 		}
			else if(UART1COMMpara.bBmsCommErrorFlag)
				{
				 MtoLCDData.xpErrorStatus=BMS_COMM_ERROR;
				 ErrorVar.xpCommErrorFlag=1;
                }
		}
	 
	 if(BMStoMData.xpErrorStatus==BMS_OT_ERROR)
	 	{
		ErrorVar.xpServiceErrorFlag=1;
	 	if((VoiceListpara.VoicePromptBatTempOverFlag==0)&&(ErrorVar.xpErrorPlayCnt1<2))
	 		{
			VoiceListpara.VoicePromptBatTempOverFlag=1;//电池温度过高，请等待降温后使用
			++ErrorVar.xpErrorPlayCnt1;
	 		}
		MtoLCDData.xpErrorStatus=BMS_OT_ERROR;
	 	}
	 else
	 	{
		 ErrorVar.xpErrorPlayCnt1=0;	 	
	 	}
/////////////////////////////////////////////////////////////////////////
	 if(WaterBSta.ucQBucketStatus == QBULET_BLANK_STA)
	 	{
		 if((ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)||((WorkVar.ucWorkOrderStatus==OPEN_STA)&&(KeyVar.ucKeyGearValue!=2)))
		 	{
			 if((ACleanVar.xpAutoCleanStep>STEP0)&&(ACleanVar.xpAutoCleanStep<STEP6))// 自清洁过程中
			    {
				ACleanVar.xpAutoCleanStep_back1=ACleanVar.xpAutoCleanStep;
				ACleanVar.xpAutoCleanCount_back1=ACleanVar.xpAutoCleanCount;
			 	}
			 else
			 	{
				 ACleanVar.xpAutoCleanStep_back1=0;
//				 WaterElecVar.waterelec_OK_flag=0;// 20220107
				 ACleanVar.xpAutoCleanCount_back1=0;
			 	}
			 ACleanVar.xpAutoCleanStep_back=STEPELEC;//20210924
			 ACleanVar.xpAutoCleanCount_back=0;//20210924
			 ACleanVar.xpAutoCleanStep=0;//20210924
			 ACleanVar.xpAutoCleanCount=0;//20210924
		 	if(ErrorVar.xpErrorPlayCnt2==0)ErrorVar.xpErrorPlayCnt2=1;
			if(ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
		 	}
		 if(ErrorVar.xpErrorPlayCnt2>0)
		 	{
		 	if(ErrorVar.xpErrorPlayCnt2<2)
			 {
              if(VoiceListpara.VoicePromptCboxNoWaterFlag==0)
			  	{			  	
			  	 ErrorVar.xpErrorPlayCnt2++;
				 VoiceListpara.VoicePromptCboxNoWaterFlag= 1;  //清水箱缺水，请及时加水
				 MtoLCDData.xpQWaterBoxStatus=QBULET_BLANK_STA;// 20210908
			     ErrorVar.xpErrorWarnCnt=6;// 20210915
				 WaterElecVar.waterelec_work_cnt=0;// 20210927
				 if(WorkVar.ucWorkStatus==OPEN_STA&&KeyVar.ucKeyGearValue==1)
				 	{// 杀菌模式记录缺水状态，不立即制备除菌液
				 	WaterElecVar.waterelec_OK_flag_back=1;// 20210928
				 	}
				 else
				 	{
			        WaterElecVar.waterelec_OK_flag=0;//20210924
				 	}
			  	}
			 }
			else
				{
				if(ErrorVar.xpErrorWarnCnt>0)
				  {
					ErrorVar.xpErrorWarnCnt--;
				  }
				else
				  {
					if(WorkVar.ucWorkOrderStatus!=AUTO_STA)
					   {
						//MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210908
					   }
				  }
				
				if(WorkVar.ucWorkOrderStatus==CLOSE_STA||WorkVar.ucWorkOrderStatus==CHARGE_STA||(KeyVar.ucKeyGearValue==1&&WaterElecVar.waterelec_OK_flag==0))// 20220110
//					if(WorkVar.ucWorkOrderStatus==CLOSE_STA||WorkVar.ucWorkOrderStatus==CHARGE_STA||KeyVar.ucKeyGearValue==1)// 20220106
//						 if(WorkVar.ucWorkOrderStatus!=OPEN_STA&&WorkVar.ucWorkOrderStatus!=AUTO_STA)
					{// 关机 充电 除菌液制备中均清除缺水标志					
					 WaterBSta.uiWaterLevelCnt=0;                 //20220209
					 WaterBSta.uiWaterLevelRecCnt=0;              //20220209
					 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210908
					 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20210929
					}
				 /*
				 if(WorkVar.ucWorkOrderStatus==OPEN_STA&&(KeyVar.ucKeyGearValue==0||KeyVar.ucKeyGearValue==1))
				 	{
				 	}
				 else
				 	{
					 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20210929
					 if(WorkVar.ucWorkOrderStatus!=OPEN_STA&&WorkVar.ucWorkOrderStatus!=AUTO_STA)
					 	{
						 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210908
					 	}
				 	}
				 */
				}
		 	}
		 else
		 	{
			 if(ErrorVar.xpErrorWarnCnt>0)
			   {
				 ErrorVar.xpErrorWarnCnt--;
			   }
			 else
			   {
				 if(WorkVar.ucWorkOrderStatus!=AUTO_STA)
					{
					 MtoLCDData.xpQWaterBoxStatus= QBULET_FULL_STA;//20210908
					 WaterBSta.ucQBucketStatus   = QBULET_FULL_STA;// 20210929
					}
			   }
		 	}
	 	}
	 else
	 	{
		 if(ErrorVar.xpErrorWarnCnt>0)
		   {
			 ErrorVar.xpErrorWarnCnt--;
		   }
		 else
		   {
			 if(WorkVar.ucWorkOrderStatus!=AUTO_STA)
				{
				 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210908
				 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20210929
				}
		   }
		 ErrorVar.xpErrorPlayCnt2=0;
	 	}
	 
////////////////////////////////////////////////////////////////////////////////////////	
if(WorkVar.ucWorkOrderStatus!=CLOSE_STA&&WorkVar.ucWorkOrderStatus!=CHARGE_STA)
   {
    if(WorkVar.ucWorkOrderStatus==AUTO_STA&&ACleanVar.xpAutoCleanStep==STEP7)// 20210922
    	{
		ErrorVar.xpWBoxErrorcnt1=0;//20211109
		ErrorVar.xpWBoxErrorFlag=0;//20211109
		/////////////////////20220111
		if(WaterBSta.ucWBucketStatus == WBULET_BLANK_STA)
			{
			MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;//20211109
			}
		////////////////////
//		MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;//20211109
    	}
	else
		{
	      if(WaterBSta.ucWBucketStatus == WBULET_FULL_STA)
	      	{
			  ErrorVar.xpWBoxErrorFlag=1;		  
			  MtoLCDData.xpWWaterBoxStatus=WBULET_FULL_STA;
			  if((ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)||(WorkVar.ucWorkStatus==OPEN_STA))//20210923
//				  if((ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)||(WorkVar.ucWorkOrderStatus==OPEN_STA))
//				  if((WorkVar.ucWorkOrderStatus==AUTO_STA)||(WorkVar.ucWorkOrderStatus==OPEN_STA))
				 {
				  if(ErrorVar.xpErrorPlayCnt3==0)ErrorVar.xpErrorPlayCnt3=1;
				  if(ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
				 }
			  // 20211119
			  if(WorkVar.ucWorkOrderStatus==IDLE_STA)
			  	{
			  	WorkVar.ucFirstONFlag=0;
			  	}
	      	}
		  else if(WaterBSta.ucWBucketStatus == WBULET_BLANK_STA)
		  	{
		  	  if(ErrorVar.xpWBoxErrorcnt||ErrorVar.xpWBoxErrorcnt1)// 20210925
		  	  	{
				  if(ErrorVar.xpWBoxErrorcnt>0)ErrorVar.xpWBoxErrorcnt--;
				  if(ErrorVar.xpWBoxErrorcnt1>0)ErrorVar.xpWBoxErrorcnt1--;
		  	  	}
			  else
			  	{
				  ErrorVar.xpWBoxErrorFlag=1;
#if LCDScreenEnableE==0
				  MtoLCDData.xpWWaterBoxStatus=WBULET_FULL_STA;//WBULET_BLANK_STA;
#else
	              MtoLCDData.xpWWaterBoxStatus=WBULET_BLANK_STA+2;
#endif
				  if((ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)||(WorkVar.ucWorkStatus==OPEN_STA))// 20210923
//					  if((ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)||(WorkVar.ucWorkOrderStatus==OPEN_STA))
	//				  if((WorkVar.ucWorkOrderStatus==AUTO_STA)||(WorkVar.ucWorkOrderStatus==OPEN_STA))
					 {
					  if(ErrorVar.xpErrorPlayCnt4==0)ErrorVar.xpErrorPlayCnt4=1;
					  if(ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
					 }
			  	}
			  // 20211119
			  if(WorkVar.ucWorkOrderStatus==IDLE_STA)
			  	{
			  	WorkVar.ucFirstONFlag=0;
			  	}
		  	}
		  else
		  	{
		  	//////////////////20210929
		  	if(MtoLCDData.xpQWaterBoxStatus==QBULET_FULL_STA&&ErrorVar.xpServiceErrorFlag==0\
				&&ErrorVar.xpRunErrorFlag==0&&ErrorVar.xpCommErrorFlag==0)
		  		{
			  	if(ACleanVar.xpAutoCleanStatus==WAIT_AUTO_CLEAN_MODE)
			  		{
					 if(ACleanVar.xpAutoCleanStep==STEP7)
						{
						}
					 else
						{
						 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210929
						 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20210929
						 ACleanVar.xpAutoDrySoloFlag=0;// 20210924					 
						}
				    ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //自清洁  
			  		}
		  		}
			////////////////
		  	  if(ErrorVar.xpErrorPlayCnt3>=2)
		  	  	{
			    ErrorVar.xpErrorPlayCnt3=0;
		  	  	} 
		  	  if(ErrorVar.xpErrorPlayCnt4>=2)
		  	  	{
			    ErrorVar.xpErrorPlayCnt4=0;
		  	  	} 
			  if(ErrorVar.xpWBoxErrorcnt!=Recover_WBLANK_WARING_TIME)
			  	{// 防止自清洁提示清理污水箱冲突
				  ErrorVar.xpWBoxErrorcnt=0;
				  MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;
			  	}
		  	  ErrorVar.xpWBoxErrorcnt1=0;
			  ErrorVar.xpWBoxErrorFlag=0;
		  	}
    	}
}
else
{
	ErrorVar.xpWBoxErrorcnt1=0;// 20211021
    ErrorVar.xpWBoxErrorFlag=0;//20211109
	MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;
}
      if(ErrorVar.xpErrorPlayCnt3>0)
      	{      	 
	  	//20210908
//		  MtoLCDData.xpWWaterBoxStatus=WBULET_FULL_STA;// 20210908
		if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=CLOSE_STA))// 20210923
//			if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=IDLE_STA))
		  	{
		  	 if((VoiceListpara.VoicePromptSewageBoxFullFlag==0)&&(ErrorVar.xpErrorPlayCnt3<3))
		  	 	{
		  	 	 ErrorVar.xpErrorPlayCnt3++;
				 ErrorVar.xpWBoxErrorcnt1=Recover_WBLANK_WARING_TIME;// 20210925
				 VoiceListpara.VoicePromptSewageBoxFullDirtyCleanFlag= 1; //污水箱已满，请及时清理
				 if((WorkVar.ucWorkOrderStatus==AUTO_STA)&&(ErrorVar.xpErrorPlayCnt3==3))
				   {
				   ErrorVar.xpErrorPlayCnt3=0;
//				   ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
				   //WorkVar.ucWorkOrderStatus= CHARGE_STA;
				   //VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
				   }
		  	    }
     	    }
		  else
		  	{
			  ErrorVar.xpErrorPlayCnt3=0;
		  	}
      	}
	  else if(ErrorVar.xpErrorPlayCnt4>0)
	  	{
	  	//20210908
//#if LCDScreenEnableE==0
//						MtoLCDData.xpWWaterBoxStatus=WBULET_FULL_STA;//WBULET_BLANK_STA;
//#else
//						MtoLCDData.xpWWaterBoxStatus=WBULET_BLANK_STA+2;
//#endif
		if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=CLOSE_STA))// 20210923
//			if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=IDLE_STA))
		  	{
		  	 if((VoiceListpara.VoicePromptNoSewageBoxFlag==0)&&(ErrorVar.xpErrorPlayCnt4<2))
		  	 	{
		  	 	ErrorVar.xpErrorPlayCnt4++;
		  	 	VoiceListpara.VoicePromptNoSewageBoxFlag= 1; //污水箱未安装，请安装污水箱
			    if((WorkVar.ucWorkOrderStatus==AUTO_STA)&&(ErrorVar.xpErrorPlayCnt4==2))
				  {
				  ErrorVar.xpErrorPlayCnt4=0;				  
//				  ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
//                VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
//                WorkVar.ucWorkOrderStatus= CHARGE_STA;
				  }
		  	 	}
		  	}
		  else
		  	{
			  ErrorVar.xpErrorPlayCnt4=0;
		  	}
	  	}
	  else
	  	{
//		MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;	  	//20210908	
		}

	  /////////////////////////////////////////////////////////
	  if(BrushVar.bBrushNGErrFlag)
	  	{
		     ErrorVar.xpRunErrorFlag=1;
		   	 if((VoiceListpara.VoicePromptNoBrushFlag==0)&&(ErrorVar.xpErrorPlayCnt5<1))
		   	 	{
		   	 	 ErrorVar.xpErrorPlayCnt5++;
		   	 	 VoiceListpara.VoicePromptNoBrushFlag= 1; //滚刷未安装，请安装滚刷
		   	 	 MtoLCDData.xpBrushStatus=BRUSH_NO_STA;// 20210908
				if((WorkVar.ucWorkOrderStatus==AUTO_STA)&&(ErrorVar.xpErrorPlayCnt5>=1))
				   {
					if(ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
//				   ErrorVar.xpErrorPlayCnt5=0;
//				   WorkVar.ucWorkOrderStatus=CHARGE_STA;
//				   VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
				   }
		   	 	}
	  	}
			
	else  if((BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)||(BrushVar.bBrushStallErrFlag))
	  	{
		   ErrorVar.xpRunErrorFlag=1;
//		   if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=IDLE_STA))
//		   	{
		   	 if((VoiceListpara.VoicePromptCleanTubeFlag==0)&&(ErrorVar.xpErrorPlayCnt6<1))
//				 if((VoiceListpara.VoicePromptCheckBrushTubeFlag==0)&&(ErrorVar.xpErrorPlayCnt6<2)&&(Read_VoiceBusy_PIN()>0))
		   	 	{
				 ErrorVar.xpErrorPlayCnt6++;
/////////////////////////////20220322//////////////////////////////////////						
						if(BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)
						{
							MtoLCDData.xpErrorStatus = SMD_BLDC_ERROR;
						}
						else if(BrushVar.bBrushStallErrFlag)
						{
							MtoLCDData.xpBrushStatus=BRUSH_STALL_STA;//20210909						
						}

//////////////////20220228///////////////////////////
/*
                 if(BrushVar.bBrushStallErrFlag)
                  	{
					  MtoLCDData.xpBrushStatus=BRUSH_STALL_STA;//20220228
                  	}
				  else
				  	{
					  MtoLCDData.xpBrushStatus=BRUSH_YES_STA;//20220228
				  	}
*/				  	
//////////////////////////////////////////////////////
				 VoiceListpara.VoicePromptCleanTubeFlag= 1;//请清理滚刷和底部管道
				 if((WorkVar.ucWorkOrderStatus==AUTO_STA)&&(ErrorVar.xpErrorPlayCnt6>=1))
					 {
					 if(ACleanVar.xpAutoCleanStatus ==AUTO_CLEAN_MODE)ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
//					   ErrorVar.xpErrorPlayCnt6=0;
//					   WorkVar.ucWorkOrderStatus= CHARGE_STA;
//					   VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
					 }
				}
//		   	}
//		   else
//		   	{		   	
//		  	ErrorVar.xpErrorPlayCnt6=0;
//		   	}
	  	}
 	}
 /////
 if(WorkVar.ucWorkOrderStatus==CHARGE_STA)
 	{
	ErrorVar.xpRunErrorFlag=0; 	 
	BrushVar.bBrushNGErrFlag=0;
	BrushVar.bBrushStallErrFlag=0;
	ErrorVar.xpErrorPlayCnt5=0;
	ErrorVar.xpErrorPlayCnt6=0;
	MtoLCDData.xpBrushStatus=BRUSH_YES_STA;//20210909
/////////////////////////////20220322//////////////////////////////////////
	MtoLCDData.xpErrorStatus = 0;
 	}
 else if(WorkVar.ucWorkOrderStatus==CLOSE_STA)
 	{	
/////////////////////////////20220322//////////////////////////////////////		
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
///////////////////////////////////////////////////////// 
/*
 if(ErrorVar.xpWBoxErrorcnt1>0)
   { 
	ErrorVar.xpWBoxErrorcnt1--;
	if(ErrorVar.xpWBoxErrorcnt1==Work_WBLANK_WARING_TIME1)
	   {
		VoiceListpara.VoicePromptDirtyCleanFlag=1;
	   }
	else if(ErrorVar.xpWBoxErrorcnt1==Work_WBLANK_WARING_TIME2)
	   {
		VoiceListpara.VoicePromptDirtyCleanFlag=1;
	   }
	else if(ErrorVar.xpWBoxErrorcnt1==Work_WBLANK_WARING_TIME3)
	   {
		VoiceListpara.VoicePromptDirtyCleanFlag=1;
	   }
   }
*/   
}

void Play_SOC_ERROR(void)
{// 电量提示类显示播报处理
	
	if((ErrorVar.xpServiceErrorFlag==0)&&(UART1COMMpara.bBmsCommErrorFlag==0))
	   {
		 if((BMStoMData.xpBatSocVal==0)||(BMStoMData.xpErrorStatus==BMS_LV_ERROR))
		 	{
//			 if(WorkVar.ucWorkOrderStatus==OPEN_STA)
			 if((WorkVar.ucWorkOrderStatus!=CLOSE_STA)&&(WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))
			 	{
				 if((VoiceListpara.VoicePromptPowerOverFlag==0)&&(ErrorVar.xpErrorPlayCnt9<1))
	//				 if((VoiceListpara.VoicePromptPowerOverFlag==0)&&(ErrorVar.xpErrorPlayCnt9<1)&&(Read_VoiceBusy_PIN()>0))
					{
					 VoiceListpara.VoicePromptPowerOverFlag= 1;//电量已耗尽
					 ++ErrorVar.xpErrorPlayCnt9;
					 if(ErrorVar.xpErrorPlayCnt9==1)
						{
//						ErrorVar.xpErrorPlayCnt9=0;
//						WorkVar.xpWorkOrderCLOSEFlag=1;
						WorkVar.ucWorkOrderStatus=IDLE_STA;
//						WorkVar.ucWorkOrderStatus=CLOSE_STA;
    					}
					}
			 	}
			 else
			 	{
			 	 ErrorVar.xpErrorPlayCnt9=0;
			 	}
			 ErrorVar.xpErrorPlayCnt8=0;
			 ErrorVar.xpErrorPlayCnt7=0;
		 	}
		 else if(BMStoMData.xpBatSocVal<=SOC_10_VALUE)
		 	{
			 if((WorkVar.ucWorkOrderStatus!=CLOSE_STA)&&(WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))
			 	{
				 if((VoiceListpara.VoicePromptLowPowerChargeFlag==0)&&(ErrorVar.xpErrorPlayCnt8<1))
					{
					 VoiceListpara.VoicePromptLowPowerChargeFlag= 1;//电量低，请及时充电
					 ++ErrorVar.xpErrorPlayCnt8;
//				  	 WorkVar.ucWorkOrderStatus=IDLE_STA;
					}
			 	}
			 else
			 	{
			 	 ErrorVar.xpErrorPlayCnt8=0;
			 	}
			 ErrorVar.xpErrorPlayCnt9=0;
			 ErrorVar.xpErrorPlayCnt7=0;
		 	}
		 else if(BMStoMData.xpBatSocVal<=SOC_20_VALUE)
			 {
			  if((WorkVar.ucWorkOrderStatus!=CLOSE_STA)&&(WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))
				 {
				  if((VoiceListpara.VoicePromptLowPowerFlag==0)&&(ErrorVar.xpErrorPlayCnt7<1))
					 {
					  VoiceListpara.VoicePromptLowPowerFlag= 1;//电池电量低
					  ++ErrorVar.xpErrorPlayCnt7;
//					  WorkVar.ucWorkOrderStatus=IDLE_STA;
					 }
				 }
			  else
			  	{
			  	ErrorVar.xpErrorPlayCnt7=0;
			  	}
			  ErrorVar.xpErrorPlayCnt8=0;
			  ErrorVar.xpErrorPlayCnt9=0;
			 }
		 else
		 	{
			 ErrorVar.xpErrorPlayCnt9=0;
			 ErrorVar.xpErrorPlayCnt8=0;
			 ErrorVar.xpErrorPlayCnt7=0;
		 	}
		}
}


