#include "uart.h"
#include "timer.h"
#include "tilt.h"
#include "charge.h"
#include "commbms.h"
#include "commbldc.h"
#include "commscreen.h"
#include "commtest.h"
#include "commfct.h"
#include "control.h"
#include "key.h"
#include "waterelec.h"
#include "sewage.h"
#include "adc.h"
#include "cleanwater.h"
#include "brush.h"
#include "waterelec.h"
#include "dry.h"
#include "waterpump.h"
#include "voice.h"
#include "error.h"
#include "improve.h"


FCTtoMCUData     FCTtoMData;
FCT_UART1Comm    FctUART1COMMpara;
MCUtoFCTData     MtoFCTData;

extern BMStoMCUData BMStoMData;
UART1Comm    UART1COMMpara;

extern BLDCtoMCUData BLDCtoMData;
UART2Comm     UART2COMMpara;

extern MCUtoBLDCData MtoBLDCData;
extern MCUtoLCDData MtoLCDData;
extern MCUtoBMSData MtoBMSData;
       MCUtoTESTData MtoTESTData;

extern TimePara       TimeVar;
extern TILTPara       TiltVar;
extern CHGPara        CHGVar;

extern WorkPara       WorkVar;
extern KeyPara        KeyVar;

extern WaterElecPara  WaterElecVar;

extern BMSPara        BMSVar;

extern SewagePara     SewageVar;
extern ADCPara        ADCVar; 
extern WaterBoxStatus WaterBSta;
extern BrushPara      BrushVar;
extern AutoCleanValue ACleanVar;
extern DryPara        DryVar;
extern WaterElecPara  WaterElecVar;
extern PumpPara       PumpVar;
extern VoiceList      VoiceListpara;
extern WorkError      ErrorVar;

TesttoMCUData TestToMData;

void Deal_FCT_SendData(void)
{
 if(FctUART1COMMpara.bTestModeFlag==0)
 	{
 	MtoFCTData.xpTesStep=STEP0;
	MtoFCTData.xpTesProgress=TEST_STAY;
	FctUART1COMMpara.uiFctTestCnt=0;
	FctUART1COMMpara.bClearMemoryCnt=0;
 	}
 else
 	{
	 MtoFCTData.xpWashFloorMODE=P2126;
//	 MtoFCTData.xpSoftwareVer=Softvare_V1_2;//Softvare_V1_0;
//	 MtoFCTData.xpSoftwareVer=Softvare_V1_3;//Softvare_V1_0;
     MtoFCTData.xpSoftwareVer=Softvare_V2_1;//Softvare_V1_0;
//	 MtoFCTData.xpTesProgress=;
//	 MtoFCTData.xpTesStep=;
	 if(MtoFCTData.xpVoiceKeyStatus==1)
	   {
		if(++KeyVar.ucKeyWakeUpcnt1>100)
		   {
			MtoFCTData.xpVoiceKeyStatus=0x02;
		   }
	   }
	 else
	   {
	   KeyVar.ucKeyWakeUpcnt1=0;
	   }
	 MtoFCTData.xpCHGADH=(uint8_t)((ADCVar.uiCHGAdcValue&0xff00)>>8);
	 MtoFCTData.xpCHGADL=(uint8_t)(ADCVar.uiCHGAdcValue&0x00ff);
	 MtoFCTData.xpKeyADH=(uint8_t)((ADCVar.uiADC_ResultBuff[Key_CHN]&0xff00)>>8);
	 MtoFCTData.xpKeyADL=(uint8_t)(ADCVar.uiADC_ResultBuff[Key_CHN]&0x00ff);
	 MtoFCTData.xpBrushADH=(uint8_t)((ADCVar.uiBrushAdcValue&0xff00)>>8);
	 MtoFCTData.xpBrushADL=(uint8_t)(ADCVar.uiBrushAdcValue&0x00ff);
	 MtoFCTData.xpWaterpumpADH=(uint8_t)((ADCVar.uiADC_ResultBuff[WaterPumpCHN]&0xff00)>>8);
	 MtoFCTData.xpWaterpumpADL=(uint8_t)(ADCVar.uiADC_ResultBuff[WaterPumpCHN]&0x00ff);
	 MtoFCTData.xpQWaterBoxADH=(uint8_t)((ADCVar.uiCleanWaterAdcValue&0xff00)>>8);
	 MtoFCTData.xpQWaterBoxADL=(uint8_t)(ADCVar.uiCleanWaterAdcValue&0x00ff);
	 MtoFCTData.xpWWaterBoxADH=(uint8_t)((ADCVar.uiSewageStageAdcValue&0xff00)>>8);
	 MtoFCTData.xpWWaterBoxADL=(uint8_t)(ADCVar.uiSewageStageAdcValue&0x00ff);
	 MtoFCTData.xpSewageLVADH=(uint8_t)((ADCVar.uiSewageLVAdcValue&0xff00)>>8);
	 MtoFCTData.xpSewageLVADL=(uint8_t)(ADCVar.uiSewageLVAdcValue&0x00ff);
	 MtoFCTData.xpBldcSpeedH=BLDCtoMData.xpBldcSpeedH;
	 MtoFCTData.xpBldcSpeedM=BLDCtoMData.xpBldcSpeedM;
	 MtoFCTData.xpBldcSpeedL=BLDCtoMData.xpBldcSpeedL;
	 if(TiltVar.bTiltEnableFlag)
	 	{
		 MtoFCTData.TileStates=0x02;
	 	}
	 else
	 	{
		 MtoFCTData.TileStates=0x01;
	 	}
	 MtoFCTData.reserve1=0;
	 MtoFCTData.reserve2=0;
 	}
}

void Deal_SendData(void)
{
	static uint16_t clear_cnt=0;
  //-------------------上电1S数据更新-------------------------   
   if(TimeVar.bPowerOnmsFlag==0)
   	{
/////////////////////////////20220322//////////////////////////////////////
//	   if(CHGVar.bChargingFlag==1)
//	   	{
//			 MtoBMSData.xpWorkStatus=CHARGE_STA;
//			 WorkVar.ucWorkStatus=CHARGE_STA;
//	   	}
//	   else
//	   	{
//		   MtoBMSData.xpWorkStatus=IDLE_STA;//OPEN_STA;
//		   WorkVar.ucWorkStatus=IDLE_STA;//OPEN_STA;
////		   MtoBMSData.xpWorkStatus=OPEN_STA;
////		   WorkVar.ucWorkStatus=OPEN_STA;
//	   	}
	   if((CHGVar.bChargingFlag==1)&&(BMStoMData.xpWorkStatus == CHARGE_STA))
	   	{
			 MtoBMSData.xpWorkStatus=CHARGE_STA;
			 WorkVar.ucWorkStatus=CHARGE_STA;
	   	}		
			else
			{
				if(CHGVar.bChargingFlag == 0)
				{
					MtoBMSData.xpWorkStatus=IDLE_STA;//OPEN_STA;
					WorkVar.ucWorkStatus=IDLE_STA;//OPEN_STA;	
				}
				else
				{
					MtoBMSData.xpWorkStatus=CLOSE_STA;//OPEN_STA;
					WorkVar.ucWorkStatus=CLOSE_STA;//OPEN_STA;					
				}
			}
   	}
  //-------------------Send BLDC DEAL-------------------------	
  if(FctUART1COMMpara.bTestModeFlag)
  	{
	  MtoBLDCData.xpsewagelv=M_MID_STA;
  	}
  else
  	{
     MtoBLDCData.xpWorkStatus=WorkVar.ucWorkStatus;
  
#if TEST_LIFE_ENABLE==1
     MtoBLDCData.xpsewagelv=M_HIGH_STA;//M_SOHIGH_STA;
#else
     if(TimeVar.bFLASH500msFlag)  // 20211009
     	{
   		TimeVar.bFLASH500msFlag=0;
		if(WorkVar.ucWorkOrderoffcnt>0)
			{// 20211119		
			if(MtoBLDCData.xpsewagelv<=M_MID_STA)
				{
				MtoBLDCData.xpsewagelv=M_MID_STA;
				}
			else 
			    {
			    MtoBLDCData.xpsewagelv=M_HIGH_STA;
			    }
			}
		else
			{
		    if(WorkVar.ucWorkStatus==OPEN_STA)
		   	    {
			   	 if(WorkVar.ucWorkOrderStatus==OPEN_STA)
			   	 	{
				   	 if(WorkVar.ucWorkOrderoffcnt>0)
				   	 	{
			             if(MtoBLDCData.xpsewagelv>M_LOW_STA)
							{
							 MtoBLDCData.xpsewagelv>>=1;
							}
				   	 	}
					 else
					 	{
						 if(KeyVar.ucKeyGearValue==2)// 吸水模式 
//							 if(KeyVar.ucKeyGearValue!=0)// 吸水模式 杀菌模式
							{
							if(SewageVar.xpsewagelv!=M_SOHIGH_STA)
								{
								if(SewageVar.xpsewagelv>0&&MtoBLDCData.xpsewagelv==0)
							 	 {
							 	  MtoBLDCData.xpsewagelv=M_MID_STA;
								 }
								if(MtoBLDCData.xpsewagelv<SewageVar.xpsewagelv)
								 {
								  MtoBLDCData.xpsewagelv<<=1;
								 }
								else if(MtoBLDCData.xpsewagelv>SewageVar.xpsewagelv)
								 {
								  MtoBLDCData.xpsewagelv>>=1;
								 }
								}
							else
								{
								if(SewageVar.xpsewagelv>0&&MtoBLDCData.xpsewagelv==0)
								  {
								   MtoBLDCData.xpsewagelv=M_MID_STA;
								  }
								if(MtoBLDCData.xpsewagelv<M_HIGH_STA)
								 {
								  MtoBLDCData.xpsewagelv<<=1;
								 }
								else if(MtoBLDCData.xpsewagelv>M_HIGH_STA)
								 {
								  MtoBLDCData.xpsewagelv>>=1;
								 }
								}
							}
					  	 else// 自动模式 杀菌模式
							{
							if(SewageVar.xpsewagelv>0&&MtoBLDCData.xpsewagelv==0)
							   {
								MtoBLDCData.xpsewagelv=M_MID_STA;
							   }
							if(MtoBLDCData.xpsewagelv<SewageVar.xpsewagelv)
							   {
						 	    MtoBLDCData.xpsewagelv<<=1;
							   }
							else if(MtoBLDCData.xpsewagelv>SewageVar.xpsewagelv)
							   {
								MtoBLDCData.xpsewagelv>>=1;
							   }
							}
				 		}
		   	 		 }
				 else
			 		{
					 if(MtoBLDCData.xpsewagelv>M_LOW_STA)
						{
						 MtoBLDCData.xpsewagelv>>=1;
						}
			 		}
	   	   		 }
	  	  else if(WorkVar.ucWorkStatus==AUTO_STA)
	   		    {
			    MtoBLDCData.xpsewagelv=M_HIGH_STA;// 150W  M_MID_STA;// 自清洁 135W
	  	 	    }
	 	  else
	     		{
		 	   MtoBLDCData.xpsewagelv=M_LOW_STA;
	    		}
			}
   		}
   #if BLDC_UART_MODE==0
   #else
   if(MtoBLDCData.xpsewagelv==M_SOHIGH_STA)MtoBLDCData.xpsewagelv=M_HIGH_STA;
   #endif
#endif
  	}
  //-------------------Send BMS DEAL------------------------
  MtoBMSData.xpWorkStatus= WorkVar.ucWorkStatus;
  MtoBMSData.xpKeyOnOffStatus=BMStoMData.xpKeyOnOffStatus;
  
  if(MtoBMSData.xpKeyPushStatus==1)
  	{
  	 if(++KeyVar.ucKeyWakeUpcnt>100)
  	 	{
  	 	 MtoBMSData.xpKeyPushStatus=2;
  	 	}
  	}
  else
  	{
  	KeyVar.ucKeyWakeUpcnt=0;
  	}
  //-------------------Send LCD DEAL-------------------------	
#if  LCDScreenEnableE==1
if(KeyVar.ucKeyGearValue_back==0)MtoLCDData.xpworkgear=K_AUTO_MODE;
else if(KeyVar.ucKeyGearValue_back==1)MtoLCDData.xpworkgear=K_DISIN_MODE;
else
  {
  MtoLCDData.xpworkgear=K_STRONG_MODE;
  }
//if((KeyVar.ucKeyGearStayCnt>0)&&(KeyVar.ucKeyGearStayCnt<3000))
//	{
//	//MtoLCDData.xpworkgeartime=30-KeyVar.ucKeyGearStayCnt/100;
//	MtoLCDData.xpworkgeartime=1;
//    }
//else
//{
// MtoLCDData.xpworkgeartime=0;
//}
MtoLCDData.xpBatSocVal=BMStoMData.xpBatSocVal;
//MtoLCDData.xpErrorStatus
//MtoLCDData.xpWorkErrorDisEnable
if(FctUART1COMMpara.bTestModeFlag)
{
	MtoLCDData.xpLCDStatus=OPEN_LCD;
}
else
{
if(WorkVar.ucWorkStatus==CLOSE_STA)
 {
	WorkVar.ucWorkLEDOnFlag=0;// 20220105
  if((WorkVar.ucWorkOrderoffcnt==0)&&(WorkVar.ucWorkOrderStatusback==0))
	 {
	  if(WorkVar.ucplaylcd_cnt>0)
	  	{
	  	  WorkVar.ucplaylcd_cnt--;
		  MtoLCDData.xpLCDStatus=OPEN_LCD;
		  MtoLCDData.xpWorkStatus=OPEN_STA;
	  	}
	  else
	  	{
		  MtoLCDData.xpLCDStatus=CLOSE_LCD;
		  MtoLCDData.xpWorkStatus=CLOSE_STA;
		  WorkVar.ucplaylcd_cnt=0;
	  	}
	 }
  else
	 {
//    WorkVar.ucplaylcd_cnt=800;
	  MtoLCDData.xpLCDStatus=OPEN_LCD;
//	 MtoLCDData.xpWorkStatus=OPEN_LCD;
	 MtoLCDData.xpWorkStatus=CLOSE_STA;// 20211215
	 }
  WorkVar.ucCloseScreencnt=0;
 }
else
 {	
    WorkVar.ucplaylcd_cnt=0;
    MtoLCDData.xpWorkStatus=WorkVar.ucWorkOrderStatus;
	if(WorkVar.ucWorkStatus==CHARGE_STA)
	 {
/*
		if(++WorkVar.ucCloseScreencnt>30000)
		   {
			WorkVar.ucCloseScreencnt=30000;
			MtoLCDData.xpLCDStatus=CLOSE_LCD;
			if(MtoLCDData.xpBatSocVal==100&&WorkVar.ucWorkLEDOnFlag==0)
				{
				 WorkVar.ucCloseScreencnt=29000;
				 WorkVar.ucWorkLEDOnFlag=1;
				}
		   }
		else
		   {
		    WorkVar.ucWorkLEDOnFlag=1;
			MtoLCDData.xpLCDStatus=OPEN_LCD;
		   }
*/
		
	  if(MtoLCDData.xpBatSocVal==100)
		 {
		  if(++WorkVar.ucCloseScreencnt>1000)// 20220111
//			  if(++WorkVar.ucCloseScreencnt>6000)
			 {
			  WorkVar.ucCloseScreencnt=1000;// 20220111
//			  WorkVar.ucCloseScreencnt=6000;
			  MtoLCDData.xpLCDStatus=CLOSE_LCD;
			 }
		  else
			 {
			  MtoLCDData.xpLCDStatus=OPEN_LCD;
			 }
		 }
	  else
		 {
		  WorkVar.ucCloseScreencnt=0;
		  MtoLCDData.xpLCDStatus=OPEN_LCD;
		 }

	 }
	else if(WorkVar.ucWorkOrderStatus==IDLE_STA&&WaterElecVar.bEnalbeWaterElecFlag==0)// 20220110
//		else if(WorkVar.ucWorkOrderStatus==IDLE_STA)// 20211231
//		else if(WorkVar.ucWorkStatus==IDLE_STA)// 20211209
	 {
	    WorkVar.ucWorkLEDOnFlag=0;// 20220105
		if(++WorkVar.ucCloseScreencnt>6000)
		   {
			WorkVar.ucCloseScreencnt=6000;
			MtoLCDData.xpLCDStatus=CLOSE_LCD;
		   }
		else
		   {
			MtoLCDData.xpLCDStatus=OPEN_LCD;
		   }
	 }
	else // 20211209
		{
		WorkVar.ucWorkLEDOnFlag=0;// 20220105
		WorkVar.ucCloseScreencnt=0;
		MtoLCDData.xpLCDStatus=OPEN_LCD;
		}
 }
}
//MtoLCDData.xpWorkStatus=TEST_STA;// 20211228

if(TiltVar.bTiltEnableFlag==0)
   {
	MtoLCDData.xpMachineStatus=2;
   }
else
   {
	MtoLCDData.xpMachineStatus=1;
   }
//MtoLCDData.xpQWaterBoxStatus;
//MtoLCDData.xpWWaterBoxStatus;
//MtoLCDData.xpBrushStatus;
if(WorkVar.ucWorkOrderStatus==AUTO_STA)
{
 if(ACleanVar.xpAutoCleanStep>STEP0&&ACleanVar.xpAutoCleanStep<STEP7)
 	{
	 MtoLCDData.xpACleanStatus=AUTO_CLEAN_MODE;
 	}
 else
 	{
	 MtoLCDData.xpACleanStatus=NON_AUTO_CLEAN_MODE;
 	}
}
else
{
	MtoLCDData.xpACleanStatus=NON_AUTO_CLEAN_MODE;
}
/*
if(VoiceListpara.VoicePromptLowPowerLowPowerCleanFlag)
	{
	MtoLCDData.xpACleanWarnStatus=1;//提示自清洁电量不足
	}
else if(VoiceListpara.VoicePromptSleepREQFlag)
	{
	MtoLCDData.xpACleanWarnStatus=2; // 提示按开关键唤醒
	}
else if(VoiceListpara.VoicePromptReqCleanFlag)
	{
	MtoLCDData.xpACleanWarnStatus=4;//提示放回底座自清洁
	}
else
	{
	MtoLCDData.xpACleanWarnStatus=0;
	}
if(VoiceListpara.VoicePromptLowPowerLowPowerCleanFlag==0&&(Read_VoiceBusy_PIN()==1)&&\
	VoiceListpara.VoicePromptSleepREQFlag==0&&VoiceListpara.VoicePromptReqCleanFlag==0)
{
//	MtoLCDData.xpACleanWarnStatus=0;
}
*/

if(MtoLCDData.xpACleanWarnStatus>0)
  {
   if(++clear_cnt>500)
   	{
   	 MtoLCDData.xpACleanWarnStatus=0;
	 clear_cnt=0;
   	}
  }
else
{
 clear_cnt=0;
}

if(DryVar.bEnalbeDryFlag||ACleanVar.xpAutoCleanStep==STEP7)
	{
	MtoLCDData.xpDryStatus&=0xfc;
	MtoLCDData.xpDryStatus|=0x01;
	}
else
	{
	MtoLCDData.xpDryStatus&=0xfc;
	MtoLCDData.xpDryStatus|=0x02;
	}

if(ACleanVar.xpAutoDrySoloFlag==0)// 20210924
{
	MtoLCDData.xpDryStatus&=0xf3;
}
else
{
	MtoLCDData.xpDryStatus&=0xf3;
	MtoLCDData.xpDryStatus|=0x04;
}


if(WaterElecVar.bEnalbeWaterElecFlag||\
	(ACleanVar.xpAutoCleanStep==STEP0&&(ACleanVar.xpAutoCleanStatus==AUTO_CLEAN_MODE)))// 20211230
//	(ACleanVar.xpAutoCleanStep==STEP0&&(WorkVar.ucWorkOrderStatus==AUTO_STA)))
	{
	MtoLCDData.xpWaterEStatus=1;
	}
else
	{
	MtoLCDData.xpWaterEStatus=2;
	}
//#if YKLLCD==0
//MtoLCDData.xpDryTimeS_L=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)/256);
//MtoLCDData.xpDryTimeS_H=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)%256);
//MtoLCDData.xpWaterETimeS_L=0;
//MtoLCDData.xpWaterETimeS_H=(uint8_t)((WaterElecVar.waterelec_work_cnt_cntdn/100)%256);
//MtoLCDData.xpACleanTimeS_L=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)/256);
//MtoLCDData.xpACleanTimeS_H=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)%256);
//#else
MtoLCDData.xpDryTimeS_H=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)/256);
MtoLCDData.xpDryTimeS_L=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)%256);

///*
MtoLCDData.xpWaterETimeS_H=0;
MtoLCDData.xpWaterETimeS_L=(uint8_t)((WaterElecVar.waterelec_work_cnt_cntdn/100)%256);
//*/
/*
if(MtoLCDData.xpWaterEStatus==1)
{
MtoLCDData.xpWaterETimeS_H=0;
MtoLCDData.xpWaterETimeS_L=(uint8_t)((WaterElecVar.waterelec_work_cnt_cntdn/100)%256);
}
else
{
	MtoLCDData.xpWaterETimeS_H=0;
	MtoLCDData.xpWaterETimeS_L=0;
}
*/
MtoLCDData.xpACleanTimeS_H=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)/256);
MtoLCDData.xpACleanTimeS_L=(uint8_t)((ACleanVar.xpAutoCleanNorCount/100)%256);
//#endif

#if SEWAGE_3LED_ENABLE==0
if(SewageVar.xpsewagelv==M_LOW_STA)
	{
	 MtoLCDData.xpSewagelv=1;
	}
else
	{
	 MtoLCDData.xpSewagelv=2;
	}
#else
if(SewageVar.xpsewagelv==M_LOW_STA)
	{
	 MtoLCDData.xpSewagelv=1;
	}
else if(SewageVar.xpsewagelv==M_MID_STA)
	{
	 MtoLCDData.xpSewagelv=2;
	}
else// if(SewageVar.xpsewagelv==M_HIGH_STA)
	{
	 MtoLCDData.xpSewagelv=4;
	}
#endif

//if(SewageVar.xpSewageWarnFlag||BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)// 20220228
	if(SewageVar.xpSewageWarnFlag)// 20211230
//	if(SewageVar.xpSewageWarnFlag&&(ErrorVar.xpErrorPlayCnt10<1))
	{
	 MtoLCDData.xpSewageStatus=2;
	}
else
	{
	 MtoLCDData.xpSewageStatus=1;
	}
#else
  if(KeyVar.ucKeyGearValue_back==0)MtoLCDData.xpworkgear=K_AUTO_MODE;
  else if(KeyVar.ucKeyGearValue_back==1)MtoLCDData.xpworkgear=K_STRONG_MODE;
  else
  	{
  	MtoLCDData.xpworkgear=K_AUTO_MODE;//K_DISIN_MODE;
  	}
  MtoLCDData.xpBatSocVal=BMStoMData.xpBatSocVal;
  MtoLCDData.xpsewagelv =SewageVar.xpsewagelv;
  if(WorkVar.ucWorkStatus==OPEN_STA)
  	{
  	 if(KeyVar.ucKeyGearValue==1)
  	 	{
		 if(WaterElecVar.waterelec_OK_flag==0)MtoLCDData.xpWorkStatus=OPEN_STA; 
		 else MtoLCDData.xpWorkStatus=IDLE_STA;
  	 	}
	 else
	 	{
	  	 if(TiltVar.bTiltEnableFlag==0)
	  	 	{
			 MtoLCDData.xpWorkStatus=OPEN_STA;
	  	 	}
		 else
		 	{
				 if((WorkVar.ucWorkOrderoffcnt==0)&&(WorkVar.ucWorkOrderStatusback==0))
					{
					 MtoLCDData.xpWorkStatus=IDLE_STA;
					}
				 else
					{
					 MtoLCDData.xpWorkStatus=OPEN_STA;
					}
		 	}
	 	}
  	}
  else 
  	{
	  MtoLCDData.xpWorkStatus=WorkVar.ucWorkStatus;
  	}
  
   if(WorkVar.ucWorkStatus==CLOSE_STA)
   	{
   	 if((WorkVar.ucWorkOrderoffcnt==0)&&(WorkVar.ucWorkOrderStatusback==0))
   	 	{
	     MtoLCDData.xpLCDStatus=CLOSE_LCD;
   	 	}
	 else
	 	{
		 MtoLCDData.xpLCDStatus=OPEN_LCD;
	 	}
	 WorkVar.ucCloseScreencnt=0;
   	}
   else
   	{  
   	   if(WorkVar.ucWorkStatus==CHARGE_STA)
   	   	{
   	   	 if(MtoLCDData.xpBatSocVal==100)
   	   	 	{
   	   	 	 if(++WorkVar.ucCloseScreencnt>1000)
   	   	 	 	{
   	   	 	 	 WorkVar.ucCloseScreencnt=1000;
				 MtoLCDData.xpLCDStatus=CLOSE_LCD;
   	   	 	 	}
			 else
			 	{
				 MtoLCDData.xpLCDStatus=OPEN_LCD;
			 	}
   	   	 	}
		 else
		 	{
			 WorkVar.ucCloseScreencnt=0;
			 MtoLCDData.xpLCDStatus=OPEN_LCD;
		 	}
   	   	}
	   else
	   	{
	       WorkVar.ucCloseScreencnt=0;
		   MtoLCDData.xpLCDStatus=OPEN_LCD;
	   	}
   	}
#endif
   //-------------------Send TEST DEAL-------------------------	 
/*
MtoTESTData.xpSoftwarever=1;
MtoTESTData.xpMachinemode=3;
MtoTESTData.xpWorkmode=WorkVar.ucWorkStatus;
if(KeyVar.ucKeyGearValue==0)MtoTESTData.xpRunmode=K_AUTO_MODE;
else if(KeyVar.ucKeyGearValue==1)MtoTESTData.xpRunmode=K_STRONG_MODE;
else
  {
  MtoTESTData.xpRunmode=K_DISIN_MODE;
  }
MtoTESTData.xpBatVolH=BMStoMData.xpBatVolH;
MtoTESTData.xpBatVolL=BMStoMData.xpBatVolL;
MtoTESTData.xpBatSocVal=BMStoMData.xpBatSocVal;
if(BMStoMData.xpErrorStatus>0)
{
	MtoTESTData.xpErrorStatus=BMStoMData.xpErrorStatus;
}
else if(BLDCtoMData.xpErrorStatus>0)
{
	MtoTESTData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
}
else if(BrushVar.bBrushShortErrFlag)
{
 MtoTESTData.xpErrorStatus=8;
}
else if(PumpVar.bPumpShortErrFlag)
{
 MtoTESTData.xpErrorStatus=9;
}
else if(UART1COMMpara.bBmsCommErrorFlag)
{
	MtoTESTData.xpErrorStatus=34;
}
else if(UART2COMMpara.bBldcCommErrorFlag)
{
	MtoTESTData.xpErrorStatus=13;
}
else
{
MtoTESTData.xpErrorStatus=0;
}

if(WaterBSta.ucWBucketStatus==WBULET_NORMAL_STA)
{
	MtoTESTData.xpWWaterBoxStatus=1;
}
else if(WaterBSta.ucWBucketStatus==WBULET_FULL_STA)
{
	MtoTESTData.xpWWaterBoxStatus=2;
}
else if(WaterBSta.ucWBucketStatus==WBULET_BLANK_STA)
{
	MtoTESTData.xpWWaterBoxStatus=3;
}

MtoTESTData.xpQWaterBoxStatus=WaterBSta.ucQBucketStatus;
if(BrushVar.bBrushNGErrFlag)
{
	MtoTESTData.xpBrushStatus=2;
}
else if(BrushVar.bBrushStallErrFlag)
{
	MtoTESTData.xpBrushStatus=3;
}
else
{
	MtoTESTData.xpBrushStatus=1;
}
if(WorkVar.ucWorkOrderStatus==AUTO_STA)
{
 if(ACleanVar.bAutoDeepCleanFlag)
 	{
     MtoTESTData.xpACleanStatus=DEEP_AUTO_CLEAN_MODE;
 	}
 else
 	{
     MtoTESTData.xpACleanStatus=AUTO_CLEAN_MODE;
 	}
}
else
{
   MtoTESTData.xpACleanStatus=NON_AUTO_CLEAN_MODE;
}
if(DryVar.bEnalbeDryFlag)
{
    MtoTESTData.xpDryStatus=1;
}
else
{
    MtoTESTData.xpDryStatus=2;
}
if(WaterElecVar.bEnalbeWaterElecFlag)
{
	MtoTESTData.xpWaterelecStatus=1;
}
else
{
	MtoTESTData.xpWaterelecStatus=2;
}
MtoTESTData.xpKeyADH=(uint8_t)((ADCVar.uiADC_ResultBuff[Key_CHN]&0xff00)>>8);
MtoTESTData.xpKeyADL=(uint8_t)(ADCVar.uiADC_ResultBuff[Key_CHN]&0x00ff);
MtoTESTData.xpBrushADH=(uint8_t)((ADCVar.uiBrushAdcValue&0xff00)>>8);
MtoTESTData.xpBrushADL=(uint8_t)(ADCVar.uiBrushAdcValue&0x00ff);
MtoTESTData.xpWaterpumpADH=(uint8_t)((ADCVar.uiADC_ResultBuff[WaterPumpCHN]&0xff00)>>8);
MtoTESTData.xpWaterpumpADL=(uint8_t)(ADCVar.uiADC_ResultBuff[WaterPumpCHN]&0x00ff);
MtoTESTData.xpWWaterBoxADH=(uint8_t)((ADCVar.uiSewageStageAdcValue&0xff00)>>8);
MtoTESTData.xpWWaterBoxADL=(uint8_t)(ADCVar.uiSewageStageAdcValue&0x00ff);
MtoTESTData.xpSewageLVADH=(uint8_t)((ADCVar.uiSewageLVAdcValue&0xff00)>>8);
MtoTESTData.xpSewageLVADL=(uint8_t)(ADCVar.uiSewageLVAdcValue&0x00ff);
MtoTESTData.xpBldcSpeedH=BLDCtoMData.xpBldcSpeedH;
MtoTESTData.xpBldcSpeedM=BLDCtoMData.xpBldcSpeedM;
MtoTESTData.xpBldcSpeedL=BLDCtoMData.xpBldcSpeedL;
MtoTESTData.xpBldcTempVal=BLDCtoMData.xpBldcTempVal;
*/
}

void Deal_UartComm(void)
{// 10ms
	baudrate_handle_bms();
	baudrate_handle_monobus();
	Deal_SendData();
	Deal_FCT_SendData();
	Updata_SendBMSUartData(&MtoBMSData);
	Updata_SendBLDCUartData(&MtoBLDCData); 
//    Updata_SendTESTUartData(&MtoTESTData); 
	Updata_SendLCDUartData(&MtoLCDData); 
	Updata_SendFCTUartData(&MtoFCTData);// 20211214
	OverTime_CommBMS(&UART1COMMpara);
	
	if(WorkVar.ucWorkStatus!=CHARGE_STA)
	{
		OverTime_CommBLDC(&UART2COMMpara);
	}
//	OverTime_CommTEST(&UART2COMMpara);
	OverTime_CommFCT(&FctUART1COMMpara);// 20211207
  //----------------------------------------------		  
  if(TimeVar.bPowerOnmsFlag==1)
  	{
	  if(++UART2COMMpara.ucTxdGapCnt>=UART_TX_50MS)
	  	{
	  	 UART2COMMpara.ucTxdGapCnt=0;
		 UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
		 UART2COMMpara.ucSendByteCnt = UART2->RDR;
		 UART_ITConfig(UART2,  UART_IT_RXIEN, DISABLE); 	 
		 UART2COMMpara.ucSendByteCnt = 0;
		 DMA_ClearFlag(DMA1_FLAG_TC4);	//清除通道2传输完成标志
         if(UART2COMMpara.bSendBldcBuffFlag==0)
         	{
		     MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoBLDCData.xpMcuToBldcBuff,BLDC_LENTH);
             UART2COMMpara.bSendBldcBuffFlag=1;
						
					if(UART2SendRespondCnt < 60000)
					{
							UART2SendRespondCnt++;
					}
//			 UART2COMMpara.bSendTestBuffFlag=1;
		    }
		 else
		 	{
//		 	 	if(UART2COMMpara.bSendTestBuffFlag)
//		 	 		{
//					 MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoTESTData.xpMcuToTestBuff,M_TEST_LENTH);
//					 UART2COMMpara.bSendTestBuffFlag=0;
//		 	 		}
//				else
//					{
					 MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoLCDData.xpMcuToLcdBuff,LCD_LENTH);
					 UART2COMMpara.bSendBldcBuffFlag=0;
//					 UART2COMMpara.bSendTestBuffFlag=1;
//					}
		 	}
		 UART_DMACmd(UART2,UART_DMAReq_EN,ENABLE);						//使能uart1 DMA
		 MYDMA_Enable(DMA1_Channel4);									//开始一次DMA传输！  
		 UART_ITConfig(UART2,  UART_IT_RXIEN, ENABLE);		 
	  	}
	  //---------------------send to BMS-------------------------
	  if(++UART1COMMpara.ucTxdGapCnt>=UART_TX_50MS)
	  	{
		  UART1COMMpara.ucTxdGapCnt = 0;				  
		  UART1COMMpara.ucSendByteCnt = 0;
		  DMA_ClearFlag(DMA1_FLAG_TC2); 									  //清除通道2传输完成标志
		  if(FctUART1COMMpara.bTestModeFlag==0)
		  	{
		     MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)MtoBMSData.xpMcuToBmsBuff,BMS_LENTH);
					if(UART1SendRespondCnt < 60000)
					{
							UART1SendRespondCnt++;
					}
		    }
		  else
            {
			 MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)MtoFCTData.xpMcuToFCTBuff,M_FCT_LENTH);
            }
		  UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE); 							  //使能uart1 DMA
		  MYDMA_Enable(DMA1_Channel2);											  //开始一次DMA传输！					  
	  	}
  	}
  else// 20210928开机屏幕显示慢
  	{
	  if(++UART2COMMpara.ucTxdGapCnt>=UART_TX_50MS)
	  	{
	  	 UART2COMMpara.ucTxdGapCnt=0;
		 UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
		 UART2COMMpara.ucSendByteCnt = UART2->RDR;
		 UART_ITConfig(UART2,  UART_IT_RXIEN, DISABLE); 	 
		 UART2COMMpara.ucSendByteCnt = 0;
		 DMA_ClearFlag(DMA1_FLAG_TC4);	//清除通道2传输完成标志
 //        if(UART2COMMpara.bSendBldcBuffFlag==0)
 //        	{
//		     MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoBLDCData.xpMcuToBldcBuff,BLDC_LENTH);
//           UART2COMMpara.bSendBldcBuffFlag=1;
//			 UART2COMMpara.bSendTestBuffFlag=1;
//		    }
//		 else
//		 	{
//		 	 	if(UART2COMMpara.bSendTestBuffFlag)
//		 	 		{
//					 MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoTESTData.xpMcuToTestBuff,M_TEST_LENTH);
//					 UART2COMMpara.bSendTestBuffFlag=0;
//		 	 		}
//				else
//					{
					 MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoLCDData.xpMcuToLcdBuff,LCD_LENTH);
					 UART2COMMpara.bSendBldcBuffFlag=0;
//					 UART2COMMpara.bSendTestBuffFlag=1;
//					}
//		 	}
		 UART_DMACmd(UART2,UART_DMAReq_EN,ENABLE);						//使能uart1 DMA
		 MYDMA_Enable(DMA1_Channel4);									//开始一次DMA传输！  
		 UART_ITConfig(UART2,  UART_IT_RXIEN, ENABLE);		 
	  	}
  	}
}

/********************************************************************************************************
**函数信息 ：MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)                      
**功能描述 ：可修改根据输入参数修改配置的DMA配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{    
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                        //使能DMA传输
    
    DMA_DeInit(DMA_CHx);                                                        //将DMA的通道1寄存器重设为缺省值
//    DMA1_MEM_LEN=cndtr;
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;                            //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;                                //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                          //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_BufferSize = cndtr;                                   //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //工作在正常缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA_CHx, &DMA_InitStructure);                                      //根据DMA_InitStruct中指定的参数初始化DMA的通道UART1_Tx_DMA_Channel所标识的寄存器
    
} 
/********************************************************************************************************
**函数信息 ：MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)                   
**功能描述 ：开始一次DMA传输
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    //	DMA_Cmd(DMA_CHx, DISABLE );  //关闭UART1 TX DMA1 所指示的通道      
    DMA_Cmd(DMA_CHx, ENABLE);  //使能UART1 TX DMA1 所指示的通道 
}	  
/********************************************************************************************************
**函数信息 ：MYDMA_Disable(DMA_Channel_TypeDef*DMA_CHx)                   
**功能描述 ：禁止DMA传输
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void MYDMA_Disable(DMA_Channel_TypeDef*DMA_CHx)
{ 
    	DMA_Cmd(DMA_CHx, DISABLE );  //关闭UART1 TX DMA1 所指示的通道      
//    DMA_Cmd(DMA_CHx, ENABLE);  //使能UART1 TX DMA1 所指示的通道 
}

/*******************************************************************************
* @name   : UART1_Init()
* @brief  : UART1 initial
* @param  : void
* @retval : void
*******************************************************************************/
void UART1_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_Init(& NVIC_InitStruct);
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStructure);

    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);

    UART_Cmd(UART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : UART2_Init()
* @brief  : UART2 initial
* @param  : void
* @retval : void
*******************************************************************************/
void UART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_Init(& NVIC_InitStruct);
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART2, &UART_InitStructure);

    UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);

    UART_Cmd(UART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}




/*---------------------------------------------------------------------------------------------------------*/
/* UART0 and UART2 Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void UART1_Service(void)
{	
    if(UART_GetITStatus(UART1, UART_IT_RXIEN) == SET)
    {
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
//      /* Get the character from UART Buffer */			
        Deal_ReceBMSUartData(&BMStoMData,&UART1COMMpara,&FCTtoMData,&FctUART1COMMpara);
	    Deal_ReceFCTUartData(&FCTtoMData,&FctUART1COMMpara);
	} 
//-------------------------------------------------------------------------------------
    if(UART_GetITStatus(UART1, UART_IT_TXIEN) == SET)
    {
		if(UART1COMMpara.ucSendByteCnt<(BMS_LENTH-1))
		{
			UART1COMMpara.ucSendByteCnt++;
			while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
//			UART_SendData(UART1, MtoBMSData.xpMcuToBmsBuff[UART1COMMpara.ucSendByteCnt]);
			if(UART1COMMpara.ucSendByteCnt==(BMS_LENTH-1)) UART_ITConfig(UART1,  UART_IT_TXIEN, DISABLE);
		}						
    }		
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART0 and UART2 Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/

void UART2_Service(void)
{
    if(UART_GetITStatus(UART2, UART_IT_RXIEN) == SET)
    {
      UART_ClearITPendingBit(UART2, UART_IT_RXIEN);			
       /* Get all the input characters */
	  Deal_ReceBldcUartData(&BLDCtoMData,&UART2COMMpara);
//	  Deal_ReceTestUartData(&TestToMData,&BLDCtoMData,&UART2COMMpara);
    }
//-------------------------------------------------------------------------------------
    if(UART_GetITStatus(UART2, UART_IT_TXIEN) == SET)
    {
      UART_ClearITPendingBit(UART2, UART_IT_TXIEN);				
    }	
}


/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART2_IRQHandler(void)
{
    UART2_Service();
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 1 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
    UART1_Service();
}





