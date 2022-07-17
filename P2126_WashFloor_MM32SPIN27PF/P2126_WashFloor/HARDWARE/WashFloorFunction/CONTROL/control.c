#include "control.h"
#include "gpio.h"
#include "tilt.h"
#include "timer.h"
#include "brush.h"
#include "dry.h"
#include "bldc.h"
#include "waterelec.h"
#include "waterpump.h"
#include "key.h"
#include "keyio.h"
#include "keyad.h"
#include "uart.h"
#include "sewage.h"
#include "cleanwater.h"
#include "waterelec.h"
#include "voice.h"
#include "adc.h"
#include "commbms.h"
#include "commbldc.h"
#include "commscreen.h"
#include "commfct.h"
#include "charge.h"
#include "voicelist.h"
#include "error.h"
#include "eeprom.h"
#include "improve.h"

extern FCTtoMCUData     FCTtoMData;
extern FCT_UART1Comm    FctUART1COMMpara;
extern MCUtoFCTData     MtoFCTData;
extern Eeprom_TypeDef E2prom;

TILTPara       TiltVar;
CHGPara        CHGVar;
PumpPara       PumpVar;
BrushPara      BrushVar;
BLDCCtl        BLDCVar;
DryPara        DryVar;
WaterElecPara  WaterElecVar;

VoicePara      Voice;
KeyPara        KeyVar;
AutoCleanValue ACleanVar;

MCUtoLCDData   MtoLCDData;
MCUtoBMSData   MtoBMSData;
MCUtoBLDCData  MtoBLDCData;
extern UART1Comm    UART1COMMpara;
extern UART2Comm    UART2COMMpara;

SewagePara     SewageVar;
WaterBoxStatus WaterBSta;

WorkError      ErrorVar;
WorkPara       WorkVar;
TimePara       TimeVar;
BMSPara        BMSVar;


ADCPara        ADCVar; 
BMStoMCUData   BMStoMData;
BLDCtoMCUData  BLDCtoMData;

VoiceList      VoiceListpara;

VoicePara      Voice;

RccReadPara RccTestPara;

//****************地刷水泵占空比处理函数*******************
///*功能：10ms执行一次	分别控制水泵/地刷的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************
void Elimination_BatVolt(void)
{
	static uint32_t uiVoltTemp=0;
	static uint8_t m=0;

    if(WorkVar.ucWorkOrderStatus==OPEN_STA)
    	{
		if(KeyVar.ucKeyGearValue==2)
//			else if(KeyVar.ucKeyGearValue==2)
			{ 
			BrushVar.ulBRUSH_REF_VOLT=BRUSH_REF_VOLT2;
			}
		else
			{
			BrushVar.ulBRUSH_REF_VOLT=BRUSH_REF_VOLT1;
			}
    	}
	else
		{
		BrushVar.ulBRUSH_REF_VOLT=BRUSH_REF_VOLT1;
		}
	uiVoltTemp += BMSVar.uiBatVoltage;
	if(++m>=8)
	{
		m=0;
		uiVoltTemp>>=3;
		Deal_PumpDuty(uiVoltTemp,&PumpVar);
		Deal_BrushDuty(uiVoltTemp,&BrushVar);
		uiVoltTemp = 0;			
	}
	
//	if(WorkVar.ucWorkOrderStatus!=AUTO_STA)
//		{
//		ADCVar.uiNOBrushAdcValue=NO_BRUSH_VREF;
//		ADCVar.uiStallBrushAdcValue=STALL_BRUSH_VREF;
//		ADCVar.uiLimitBrushAdcValue=LIMIT_BRUSH_VREF;		
//		}
//	else
//		{
//        if(BrushVar.bEnalbeSlowBrushFlag==0)
//        	{
			ADCVar.uiNOBrushAdcValue=NO_BRUSH_VREF;
			ADCVar.uiStallBrushAdcValue=STALL_BRUSH_VREF;
			ADCVar.uiOC1BrushAdcValue=OC1_BRUSH_VREF;
			ADCVar.uiOC2BrushAdcValue=OC2_BRUSH_VREF;
			ADCVar.uiLimitBrushAdcValue=LIMIT_BRUSH_VREF;
 //       	}
//		else
//			{
//			ADCVar.uiNOBrushAdcValue=NO_BRUSH_VREF;
//			ADCVar.uiStallBrushAdcValue=STALL_BRUSH_VREF;///2;
//			ADCVar.uiOC1BrushAdcValue=OC1_BRUSH_VREF;///2;
//			ADCVar.uiOC2BrushAdcValue=OC2_BRUSH_VREF;///2;
//			ADCVar.uiLimitBrushAdcValue=LIMIT_BRUSH_VREF;///2;
//			}
//		}
}

void Deal_CommStatus(void)
{
 static uint16_t key_onoff_xdcnt=0;
/////////////////////////////20220322//////////////////////////////////////
	  if(BMStoMData.xpKeyOnOffStatus==0x01)
		{
			error_clear();
		}
	
	
	if(WorkVar.ucWorkOrderStatus==AUTO_STA)
//		if((WorkVar.ucWorkOrderStatus==AUTO_STA)&&(CHGVar.bChargingFlag||(BMStoMData.xpWorkStatus!=CLOSE_STA)))
	   {// 主控板发出自清洁模式处理
	   /////////////////////20210923/////////////////////
	  if(BMStoMData.xpKeyOnOffStatus==0x01)
	  	{//自清洁时按下关机键停止自清洁响应
	  	   if(key_onoff_xdcnt==0)
	  	   	{
	  	   	key_onoff_xdcnt=1;
			
			if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpRunErrorFlag==0&&MtoLCDData.xpQWaterBoxStatus==QBULET_FULL_STA)// 20220224
//				if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpRunErrorFlag==0&&MtoLCDData.xpQWaterBoxStatus==QBULET_FULL_STA&&(MtoLCDData.xpWWaterBoxStatus==0x01))// 20211018
				{
			   if(ACleanVar.xpAutoCleanStep==STEP0&&MtoLCDData.xpWWaterBoxStatus==0x01)// 电解水过程中
				  {
				   ACleanVar.xpAutoDrySoloFlag=0;// 20210924
				   ACleanVar.xpAutoCleanStep_back=STEPELEC;
				   ACleanVar.xpAutoCleanCount_back=ACleanVar.xpAutoCleanCount;
				   /*
				   if(ACleanVar.xpAutoCleanStep_back1>0)
					   {
					   ACleanVar.xpAutoCleanCount=ACleanVar.xpAutoCleanCount_back1;
					   ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back1;
					   }
				   else
					   {
					   ACleanVar.xpAutoCleanCount	 = 0;
					   ACleanVar.xpAutoCleanStep=STEP1;
					   }
					*/
				   ACleanVar.xpAutoCleanCount= 0;
				   ACleanVar.xpAutoCleanStep= STEP9;  
				   KeyVar.bVoiceFlag = 1;			
				   Clear_VoiceFlag(&VoiceListpara);
				   VoiceListpara.VoicePromptSelfCleanPausedFlag=1;//暂停自清洁
				   KeyVar.bFastVoiceFlag = 1;
				   Voice.ucSendVoiceOVcnt=0;
				  }
			   else if(ACleanVar.xpAutoCleanStep<STEP6&&MtoLCDData.xpWWaterBoxStatus==0x01)// 自清洁过程中
				  {
				   ACleanVar.xpAutoCleanStep_back1=ACleanVar.xpAutoCleanStep;
				   ACleanVar.xpAutoCleanCount_back1=ACleanVar.xpAutoCleanCount;
				   ACleanVar.xpAutoDrySoloFlag=0;// 20210924
				   ACleanVar.bAutoCleanFlag=0;
				   ACleanVar.xpAutoCleanCount= 0;
				   ACleanVar.xpAutoCleanStep= STEP9;  
				   Clear_VoiceFlag(&VoiceListpara);
				   VoiceListpara.VoicePromptSelfCleanPausedFlag=1;	   //暂停自清洁
				   KeyVar.bFastVoiceFlag = 1;
				   Voice.ucSendVoiceOVcnt=0;
				  }
			   else if(ACleanVar.xpAutoCleanStep==STEP7)// 烘干中
				  {
					ACleanVar.xpAutoDrySoloFlag=0;// 20210924
					ACleanVar.bAutoCleanFlag=0;
					ACleanVar.xpAutoCleanCount= AUTO_STEP8_TIME;// 20210913
					ACleanVar.xpAutoCleanStep= STEP9;  
	//							ACleanVar.xpAutoCleanStep_back=0;
	//							ACleanVar.xpAutoCleanCount_back=0;
	                Clear_VoiceFlag(&VoiceListpara);
					VoiceListpara.VoicePromptBrushDryOffFlag=1; 	//烘干功能已关闭  
					KeyVar.bFastVoiceFlag = 1;
					Voice.ucSendVoiceOVcnt=0;
				  }
			   else if(MtoLCDData.xpWWaterBoxStatus!=0x01)
			   	{
				   Clear_VoiceFlag(&VoiceListpara);
				   VoiceListpara.VoicePromptCleanCloseFlag=1;	  //自清洁已关闭  
				   if(ACleanVar.xpAutoCleanStayCnt<29990)ACleanVar.xpAutoCleanStayCnt=29990;
			   	}
			   else //if(ACleanVar.xpAutoCleanStep<STEP9)// 关机中
				  {
				   //ACleanVar.bAutoCleanFlag=0;
				   ACleanVar.xpAutoDrySoloFlag=0;// 20210924
				   WaterElecVar.waterelec_OK_flag=0;// 20220107
				   ACleanVar.xpAutoCleanStep_back1=0;
				   ACleanVar.xpAutoCleanStep_back=0;
				   ACleanVar.xpAutoCleanCount_back=0;
				   ACleanVar.xpAutoCleanCount_back1=0;
				   ACleanVar.xpAutoCleanCount	 = AUTO_STEP8_TIME;
				   ACleanVar.xpAutoCleanStep	 = STEP9;  
				   Clear_VoiceFlag(&VoiceListpara);
				   VoiceListpara.VoicePromptCleanCloseFlag=1;	   //自清洁已关闭  
				   KeyVar.bFastVoiceFlag = 1;
				   Voice.ucSendVoiceOVcnt=0;
				  }		 
				}
			else
				{
				//ACleanVar.bAutoCleanFlag=0;
//				if(ACleanVar.xpAutoCleanStayCnt<AUTO_STAY_TIME)ACleanVar.xpAutoCleanStayCnt=
				Clear_VoiceFlag(&VoiceListpara);
				VoiceListpara.VoicePromptCleanCloseFlag=1;	   //自清洁已关闭 				
				if(ACleanVar.xpAutoCleanStayCnt<29990)ACleanVar.xpAutoCleanStayCnt=29990;
				}
	  	   	}
	  	}
	  else
	  	{
	  	key_onoff_xdcnt=0;
	  	}
	  /////////////////////////////////////////
      if(BMStoMData.xpWorkStatus==CLOSE_STA||CHGVar.bChargingFlag==0)
      	{
		  if(++WorkVar.ucChangeStateCnt>OFF_MACHINE_CNT)// 20210922
			 {
			  WorkVar.ucWorkOrderStatus=CLOSE_STA;
			  WorkVar.ucWorkStatus=CLOSE_STA;
			  WorkVar.ucChangeStateCnt=0;
			 }
		  DryVar.bDryPowerFlag=0;
		  WorkVar.ucChangeStateCnt3=0;
      	}
	  else
	  	{
	  	WorkVar.ucChangeStateCnt=0;
	  	if(DryVar.bDryPowerFlag==0)
	  		{
			if(++WorkVar.ucChangeStateCnt3>40)// 20210922
			   {
				DryVar.bDryPowerFlag=1;
				WorkVar.ucChangeStateCnt3=0;
			   }
	  		}
		  WorkVar.ucWorkStatus=AUTO_STA;
	  	}
	    WorkVar.ucChangeStateCnt2=0;
	   }
	else
	   {
        ////////////////息屏开关机按键唤醒/////////////////20220111
		if(BMStoMData.xpKeyOnOffStatus==0x01)
		  {
			 if(key_onoff_xdcnt==0)
			  {
			  key_onoff_xdcnt=1;
			  WorkVar.ucCloseScreencnt=0;
			  }
		  }
		else
		  {
			key_onoff_xdcnt=0;
		  }
	    ///////////////////////////////////////////////////
		if(WorkVar.xpWorkOrderCLOSEFlag==1&&(WorkVar.ucWorkOrderStatus==IDLE_STA||WorkVar.ucWorkOrderStatus==OPEN_STA))// 20220110
//			if(WorkVar.xpWorkOrderCLOSEFlag==1)
		  {//主控板发出关机指令处理
		   if(BMStoMData.xpWorkStatus==CLOSE_STA)
			  {
			   WorkVar.xpWorkOrderCLOSEFlag=0;
			  }
		   WorkVar.ucWorkStatus=CLOSE_STA;
		   WorkVar.ucChangeStateCnt2=0;
		  } 
	   else
		  {// 主控板正常回传BMS工作状态
		   WorkVar.xpWorkOrderCLOSEFlag=0;// 20220110
			  if((BMStoMData.xpWorkStatus==AUTO_STA)||(WorkVar.ucWorkStatus==AUTO_STA))
			  	{
			  	 if(CHGVar.bChargingFlag)WorkVar.ucWorkStatus=CHARGE_STA;
				 else
				 	{
				 	 //WorkVar.xpWorkOrderCLOSEFlag=1;
					 WorkVar.ucWorkStatus=CLOSE_STA;// 20220110
					 WorkVar.ucWorkOrderStatus=CLOSE_STA;// 20220110
				 	}
				 WorkVar.ucChangeStateCnt2=0;
			  	}
			  else
			  	{
			  	 if(BMStoMData.xpWorkStatus==CHARGE_STA&&CHGVar.bChargingFlag)
			  	 	{
					 WorkVar.ucWorkStatus=CHARGE_STA;
					 WorkVar.ucChangeStateCnt2=0;
			  	 	}
				 else if(BMStoMData.xpWorkStatus!=CLOSE_STA)
				 	{
					 WorkVar.ucWorkStatus=BMStoMData.xpWorkStatus;
					 
					 WorkVar.ucChangeStateCnt2=0;
				 	}
				 else
				 	{
				 	 if(WorkVar.ucWorkStatus==CHARGE_STA)
				 	 	{
						 if(++WorkVar.ucChangeStateCnt2>OFF_MACHINE_CNT)// 20210922
							{
							 WorkVar.ucChangeStateCnt2=0;
//						     WorkVar.xpWorkOrderCLOSEFlag=1;
						     WorkVar.ucWorkStatus=CLOSE_STA;// 20220110
						     WorkVar.ucWorkOrderStatus=CLOSE_STA;
						 	}
				 	 	}
					 else
					 	{
						 WorkVar.ucChangeStateCnt2=0;
						 WorkVar.ucWorkStatus=CLOSE_STA;
					 	}
				 	}
			  	}
		  }
	   }
	BMSVar.uiBatVoltage=(uint16_t)((BMStoMData.xpBatVolH<<8)+BMStoMData.xpBatVolL);
}

void Deal_LocalStatus(void)
{
 if(TimeVar.bPowerOnmsFlag==1)
 	{
	 //本地工作状态处理
	 if(WorkVar.ucWorkStatus!=WorkVar.ucWorkOrderStatus)
	 	{
	 	 if((WorkVar.ucWorkStatus==OPEN_STA)&&(WorkVar.ucWorkOrderStatus==IDLE_STA))
	 	 	{
			 WorkVar.ucSynchroCommCnt=0;
			 WorkVar.ucChangeStateCnt4=0;
	 	 	}
		 else//
		 	{
			 if(++WorkVar.ucSynchroCommCnt>2)
				 {
				  WorkVar.ucChangeStateCnt5=0;// 20210929
				  if(WorkVar.ucWorkStatus==OPEN_STA)
				  	{
				  	if(WorkVar.xpWorkOrderCLOSEFlag==0)
						{
						 KeyVar.bFastVoiceFlag=1;
						 Voice.ucSendVoiceOVcnt=0;
						 KeyVar.ucKeyGearValue=0;
						 KeyVar.ucKeyGearValue_back=0;
//						 WorkVar.ucChangeStateCnt5=10;//250;// 20210929
//						 WorkVar.ucChangeStateCnt5=250;// 20211215
						 WorkVar.ucChangeStateCnt5=11;// 20220210
						 //VoiceListpara.VoicePromptAutoModeFlag=1; // 自动模式
						 if(TiltVar.bTiltEnableFlag==0)
	    	                {               
	    	                 WorkVar.ucWorkOrderStatus=OPEN_STA;
						 	}
						 else
						 	{
	    	                 WorkVar.ucWorkOrderStatus=IDLE_STA;
						 	}
				  		}
					WorkVar.ucChangeStateCnt4=0;
				  	}
				  else if(WorkVar.ucWorkStatus==CLOSE_STA)
				  	{
				  	if(WorkVar.ucWorkOrderStatus!=CHARGE_STA)
				  		{
				  		 if(CHGVar.bChargingFlag==0)// 20220111
				  		   {
				  		    Clear_VoiceFlag(&VoiceListpara);// 20220106
					        VoiceListpara.VoicePromptPowerOffFlag=1; // 已关机
				  		   }
					     KeyVar.ucKeyGearValue=0;
						 KeyVar.ucKeyGearValue_back=0;
					     KeyVar.bFastVoiceFlag=1;
						 Voice.ucSendVoiceOVcnt=0;		
				  		}
				  	WorkVar.ucWorkOrderStatus=CLOSE_STA;

					WorkVar.ucChangeStateCnt4=0;
				  	}
//				  else if(WorkVar.ucWorkStatus==CHARGE_STA)
				  else if((WorkVar.ucWorkStatus==CHARGE_STA)&&(CHGVar.bChargingFlag==1))
				  	{
				  	if(WorkVar.ucWorkOrderStatus!=AUTO_STA)
						{
						 if(++WorkVar.ucChangeStateCnt4>10)// 20210929
						 	{
						 	 Clear_VoiceFlag(&VoiceListpara);// 20220106
							 VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
							 if((SewageVar.xpSewageWarnFlag==0)&&\
							 	(ADCVar.uiSewageLVAdcValue<SewageVar.uiSewagepromptValue0))
							 	{
							 	 VoiceListpara.VoicePromptDirtyStartCLeanFlag=1;
							 	}
							 WorkVar.ucWorkOrderStatus=CHARGE_STA;
							 WorkVar.ucChangeStateCnt4=0;
						 	}
				  		}
				  	}
				  WorkVar.ucSynchroCommCnt=0;
				 }	 	
		 	}
	 	}
	 else
	 	{
		 WorkVar.ucChangeStateCnt4=0;
		 WorkVar.ucSynchroCommCnt=0;
	 	}
	 ///////////////20210929
	 if(WorkVar.ucChangeStateCnt5>0)
	 	{
	 	--WorkVar.ucChangeStateCnt5;
	 	if(WorkVar.ucChangeStateCnt5==10)VoiceListpara.VoicePromptAutoModeFlag=1; // 自动模式
	 	}
		
		
	 //////////////////////////////////////待机5分钟关机
//	 if((BrushVar.bEnalbeBrushFlag==0)&&(DryVar.bEnalbeDryFlag==0)&&(WaterElecVar.bEnalbeWaterElecFlag==0)\
//	 	&&(BLDCVar.bBldcForceStopFlag==1)&&((WorkVar.ucWorkOrderStatus==OPEN_STA)||(WorkVar.ucWorkOrderStatus==IDLE_STA)))// 20211119
////		 if((BrushVar.bEnalbeBrushFlag==0)&&(DryVar.bEnalbeDryFlag==0)&&(WaterElecVar.bEnalbeWaterElecFlag==0)\
////			&&(BLDCVar.bBldcForceStopFlag==1)&&((WorkVar.ucWorkOrderStatus==OPEN_STA)||\
////			 ((WorkVar.ucWorkOrderStatus==IDLE_STA)&&(ErrorVar.xpRunErrorFlag||ErrorVar.xpServiceErrorFlag||ErrorVar.xpWBoxErrorFlag
////			 (BMStoMData.xpBatSocVal==0)||(BMStoMData.xpErrorStatus==BMS_LV_ERROR)))))// 20211119
//	 	{
//		 if(++WorkVar.ucCloseMachinecnt>CLOSE_MACHINE_TIME)
//			 {
////			 WorkVar.ucWorkOrderStatus=CLOSE_STA;
//			 WorkVar.xpWorkOrderCLOSEFlag=1;
//			 WorkVar.ucCloseMachinecnt=0;
//			 }
//	 	}
//	 else
//	 	{
//		 WorkVar.ucCloseMachinecnt=0;
//	 	}
		
		
	 //////////////////////////增加电池包和主控板状态不同步报错 20220111
     if(ErrorVar.xpBmsSleepErrorFlag==0)
     	{
     	 if(WorkVar.ucWorkStatus!=BMStoMData.xpWorkStatus)
     	 	{
     	 	 if(++ErrorVar.xpErrorCnt1>UART1_COMM_TIMEOUT)
     	 	 	{
/////////////////////////////20220322//////////////////////////////////////
//				 ErrorVar.xpErrorCnt1=UART1_COMM_TIMEOUT;
//				 UART1COMMpara.bBmsCommErrorFlag = 1;
     	 	 	}
     	 	}
		 else
		 	{
		 	ErrorVar.xpErrorCnt1=0;
		 	}
     	}
	 else
	 	{
	 	ErrorVar.xpErrorCnt1=0;
	 	}
	 //////////////////////////////////
 }
}

void Deal_OPEN_BMS(void)
{
	ErrorVar.xpWBoxErrorcnt=0;// 20210925
	BrushVar.bEnalbeNGBrushFlag=0;// 20211105
	ACleanVar.xpAutoDryStayFlag=0;// 20211230
	ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
	ACleanVar.xpAutoCleanStep_back=0;
	ACleanVar.xpAutoCleanCount_back=0;
//	WaterElecVar.waterelec_OK_flag=0;// 20220107
	ACleanVar.xpAutoCleanStep_back1=0;
	ACleanVar.xpAutoCleanCount_back1=0;
	ACleanVar.xpAutoCleanStep=0;
	ACleanVar.xpAutoCleanCount=0;
	ACleanVar.bAutoCleanFlag	  = 0;
	ACleanVar.bAutoDeepCleanFlag  = 0;
    if(TiltVar.bTiltEnableFlag==0)
    	{    	
    	 if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpRunErrorFlag||ErrorVar.xpWBoxErrorFlag||ErrorVar.xpCommErrorFlag)
    	 	{
			 SewageVar.xpSewageChangeFlag=0;//20211012
			 SewageVar.xpSewageChangecnt=0;
			 Set_Bldc_OFF(&BLDCVar);
			 Set_Brush_OFF(&BrushVar);
			 Set_WaterPump_OFF(&PumpVar);
			 Set_Dry_OFF(&DryVar);
			 Set_WaterElec_OFF(&WaterElecVar);
    	 	}
		 else if((BMStoMData.xpBatSocVal>0)&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR))
    	 	{
			 if(KeyVar.ucKeyGearValue==0)
			   {// 自动模式
				 if((WaterBSta.bWaterLVEnableFlag==0)&&(PumpVar.uiEnableQWaterLCnt<500))// 20211230
	// 				if(WaterBSta.bWaterLVEnableFlag==0)
				    {
					PumpVar.ucPumpGearValue=3;//200cc  20211108 0;
				 	}
				 else
				 	{
					if(SewageVar.xpsewagelv==M_LOW_STA)
					   {
					    if(PumpVar.ucPumpGearValue!=0)
							{
							 if(PumpVar.ucPumpGearCHGFlag1)PumpVar.ucPumpGearCHGFlag1=0;
							 else PumpVar.ucPumpGearCHGFlag1=1;
					    	}
						PumpVar.ucPumpGearValue=0;//30cc 
					   }
					else if(SewageVar.xpsewagelv==M_MID_STA)
					   {
					   if(PumpVar.ucPumpGearValue!=1)
						   {
							if(PumpVar.ucPumpGearCHGFlag2)PumpVar.ucPumpGearCHGFlag2=0;
							else PumpVar.ucPumpGearCHGFlag2=1;
						   }
						PumpVar.ucPumpGearValue=1;//50cc 
					   }
					else
					   {
						if(PumpVar.ucPumpGearValue!=2)
							{
							 if(PumpVar.ucPumpGearCHGFlag3)PumpVar.ucPumpGearCHGFlag3=0;
							 else PumpVar.ucPumpGearCHGFlag3=1;
							}
						PumpVar.ucPumpGearValue=2;//100cc  
					   }
//			        PumpVar.ucPumpGearValue=0;//30cc  20211104 0;
				 	}
				 
				 Set_Bldc_Low(&BLDCVar);
				 Set_Brush_Normal(&BrushVar);
				 Set_WaterPump_ON(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
				 
				 WorkVar.ucFirstONFlag=1;// 20211108
				 WorkVar.ucWorkOrderStatusback=1;
				 SewageVar.xpSewageLVEnable=1;// 20220106
				 if(SewageVar.xpSewageChangeFlag==0)
				 	{
                    if(++SewageVar.xpSewageChangecnt>600)
                    	{
						SewageVar.xpSewageChangeFlag=1;//20211012
						SewageVar.xpSewageChangecnt=0;
                    	}
				 	}
				 ACleanVar.xpAutoCleanPromptCount_back=1;// 20211011
				 if(WaterElecVar.waterelec_OK_flag_back)// 20210928
				 	{
				 	WaterElecVar.waterelec_OK_flag=0;
					WaterElecVar.waterelec_OK_flag_back=0;
				 	}
			   }
		     else if(KeyVar.ucKeyGearValue==2)
			   {// 吸水模式
			     PumpVar.ucPumpGearValue=0;
				 Set_Bldc_Strong(&BLDCVar);
				 Set_Brush_Normal(&BrushVar);
				 Set_WaterPump_OFF(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
				 
				 WorkVar.ucWorkOrderStatusback=1;
				 WorkVar.ucFirstONFlag=1;// 20211108
				 SewageVar.xpSewageLVEnable=1;// 20220106
				 if(SewageVar.xpSewageChangeFlag==0)
				 	{
                    if(++SewageVar.xpSewageChangecnt>600)
                    	{
						SewageVar.xpSewageChangeFlag=1;//20211012
						SewageVar.xpSewageChangecnt=0;
                    	}
				 	}
				 ACleanVar.xpAutoCleanPromptCount_back=1;// 20211011
				 if(WaterElecVar.waterelec_OK_flag_back)// 20210928
				 	{
				 	WaterElecVar.waterelec_OK_flag=0;
					WaterElecVar.waterelec_OK_flag_back=0;
				 	}
			   }
			 else//杀菌模式
			   {	
				 if(WaterElecVar.waterelec_OK_flag==1)
					{
//					 PumpVar.ucPumpGearValue=0;
					 if((WaterBSta.bWaterLVEnableFlag==0)&&(PumpVar.uiEnableQWaterLCnt<500))// 20211230
//						 if(WaterBSta.bWaterLVEnableFlag==0)
						{
						PumpVar.ucPumpGearValue=3;//200cc  20211108 0;
						}
					 else
						{
						if(SewageVar.xpsewagelv==M_LOW_STA)
						   {
							if(PumpVar.ucPumpGearValue!=0)
								{
								 if(PumpVar.ucPumpGearCHGFlag1)PumpVar.ucPumpGearCHGFlag1=0;
								 else PumpVar.ucPumpGearCHGFlag1=1;
								}
							PumpVar.ucPumpGearValue=0;//30cc 
						   }
						else if(SewageVar.xpsewagelv==M_MID_STA)
						   {
						   if(PumpVar.ucPumpGearValue!=1)
							   {
								if(PumpVar.ucPumpGearCHGFlag2)PumpVar.ucPumpGearCHGFlag2=0;
								else PumpVar.ucPumpGearCHGFlag2=1;
							   }
							PumpVar.ucPumpGearValue=1;//50cc 
						   }
						else
						   {
							if(PumpVar.ucPumpGearValue!=2)
								{
								 if(PumpVar.ucPumpGearCHGFlag3)PumpVar.ucPumpGearCHGFlag3=0;
								 else PumpVar.ucPumpGearCHGFlag3=1;
								}
							PumpVar.ucPumpGearValue=2;//100cc  
						   }
					 	}
					 Set_Bldc_Low(&BLDCVar);
					 Set_Brush_Normal(&BrushVar);
					 Set_WaterPump_ON(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
					 
					 SewageVar.xpSewageLVEnable=1;// 20220106
					 if(SewageVar.xpSewageChangeFlag==0)
						{
						if(++SewageVar.xpSewageChangecnt>600)
							{
							SewageVar.xpSewageChangeFlag=1;//20211012
							SewageVar.xpSewageChangecnt=0;
							}
						}
					 ACleanVar.xpAutoCleanPromptCount_back=1;// 20211011
					 WorkVar.ucWorkOrderStatusback=1;
					 WorkVar.ucFirstONFlag=1;// 20211108
					}
				 else
					{
					SewageVar.xpSewageLVEnable=0;// 20220106
					SewageVar.xpSewageChangeFlag=0;//20211012
					SewageVar.xpSewageChangecnt=0;
					Set_Bldc_OFF(&BLDCVar);
					Set_Brush_OFF(&BrushVar);
					Set_WaterPump_OFF(&PumpVar);
					Set_Dry_OFF(&DryVar);
					if(WaterElecVar.bEnalbeWaterElecFlag==0)
						{
					    Set_WaterElec_ON(&WaterElecVar);
						}
					}
/*			   
			     PumpVar.ucPumpGearValue=0;
				 Set_Bldc_OFF(&BLDCVar);
				 Set_Brush_OFF(&BrushVar);
				 Set_WaterPump_OFF(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
*/
			   }
			 WorkVar.ucWorkOrderoffcnt=0;
    	 	}
    	}
	else
		{
		 SewageVar.xpSewageChangeFlag=0;//20211012
		 SewageVar.xpSewageChangecnt=0;
		 if(WorkVar.ucWorkOrderStatusback)
		 	{
		 	 if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpRunErrorFlag||ErrorVar.xpWBoxErrorFlag||ErrorVar.xpCommErrorFlag)
		 	 	{
				 WorkVar.ucWorkOrderStatusback=0;
		 	 	}
			 else if((WorkVar.ucWorkOrderoffcnt<OFF_MACHINE_TIME)&&(BMStoMData.xpBatSocVal>0)\
				&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR)&&BLDCVar.bBldcForceStopFlag!=1)// 20220111
//				 else if((WorkVar.ucWorkOrderoffcnt<OFF_MACHINE_TIME)&&(KeyVar.ucKeyGearValue!=1)\
//					&&((BMStoMData.xpBatSocVal>0)&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR)))
			 	{
			 	 WorkVar.ucWorkOrderoffcnt++;
//			     PumpVar.ucPumpGearValue=0;
//				 Set_Bldc_Strong(&BLDCVar);// 20211119
//				 Set_Bldc_OFF(&BLDCVar);// 20210927
                 Set_Bldc_Low(&BLDCVar);// 20210927
//			     Set_Brush_Normal(&BrushVar);// 20211119
				 if(BrushVar.bEnalbeBrushFlag&&BrushVar.ulBrushONCnt<BRUSH_OK_TIME)
					{
					  Set_WaterPump_OFF(&PumpVar);
					}
 	             Set_Brush_OFF(&BrushVar);
//				 Set_WaterPump_ON(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
//				 if(WorkVar.ucWorkOrderoffcnt==OFF_MACHINE_TIME)
//				 	{
//				 	 VoiceListpara.VoicePromptReqCleanFlag=1;// 请放回底座自清洁
//				 	}
			 	}
			 else
			 	{
				SewageVar.xpSewageLVEnable=0;// 20220106
				WorkVar.ucWorkOrderoffcnt=0;
				WorkVar.ucWorkOrderStatus=IDLE_STA; 		   
			 	WorkVar.ucWorkOrderStatusback=0;// 20211119
			 	}
		 	}
		 else
		 	{
			 WorkVar.ucWorkOrderoffcnt=0;
			 WorkVar.ucWorkOrderStatus=IDLE_STA;		 	
		 	}
		}
}

void Deal_CLOSE_BMS(void)
{
	WorkVar.ucFirstONFlag=0;// 20211108
	SewageVar.xpSewageChangeFlag=0;//20211012
	SewageVar.xpSewageChangecnt=0;
	ErrorVar.xpWBoxErrorcnt=0;// 20210925
	WaterElecVar.waterelec_work_cnt=0;// 20210927
	if(WaterElecVar.waterelec_OK_flag_back)// 20210928
	   {
	   WaterElecVar.waterelec_OK_flag=0;
	   WaterElecVar.waterelec_OK_flag_back=0;
	   }
//	PumpVar.ucPumpGearValue 	  = 0;		
	if(ACleanVar.xpAutoCleanPromptCount_back)
		{
		ACleanVar.xpAutoCleanPromptCount_back=0;
		if(ACleanVar.xpAutoCleanPromptCount<2)ACleanVar.xpAutoCleanPromptCount++;
		}
	ACleanVar.xpAutoDryStayFlag=0;// 20211230
	ACleanVar.xpAutoCleanCount	      = 0;
	ACleanVar.xpAutoCleanCount_back	  = 0;
	ACleanVar.xpAutoCleanCount_back1	  = 0;
	ACleanVar.xpAutoCleanStep_back= STEP0;	
	ACleanVar.xpAutoCleanStep_back1= STEP0;	
	WaterElecVar.waterelec_OK_flag=0;// 20220107
	ACleanVar.xpAutoCleanStep	  = STEP0;	
	ACleanVar.xpAutoCleanStatus    = NON_AUTO_CLEAN_MODE;	
	ACleanVar.bAutoCleanStartFlag = 0;
	ACleanVar.bAutoCleanOverFlag  = 1;	
	ACleanVar.bAutoCleanFlag	  = 0;
	ACleanVar.bAutoDeepCleanFlag  = 0;

 if(WorkVar.ucWorkOrderStatusback)
 	{
	 if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpRunErrorFlag||ErrorVar.xpWBoxErrorFlag||ErrorVar.xpCommErrorFlag)
		{
		 WorkVar.ucWorkOrderStatusback=0;
		}
	 else if((WorkVar.ucWorkOrderoffcnt<OFF_MACHINE_TIME)&&(BMStoMData.xpBatSocVal>0)\
	 	&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR)&&BLDCVar.bBldcForceStopFlag!=1)
		{
		 WorkVar.ucWorkOrderoffcnt++;
//		 PumpVar.ucPumpGearValue=0;
//		 Set_Bldc_Strong(&BLDCVar);// 20211119
 		 Set_Bldc_Low(&BLDCVar);// 20210927
//		 Set_Brush_Normal(&BrushVar);// 20211119
         if(BrushVar.bEnalbeBrushFlag&&BrushVar.ulBrushONCnt<BRUSH_OK_TIME)
            {
              Set_WaterPump_OFF(&PumpVar);
            }
		 Set_Brush_OFF(&BrushVar);
		 Set_Dry_OFF(&DryVar);
		 Set_WaterElec_OFF(&WaterElecVar);
		}
	 else
		{
		VoiceListpara.VoicePromptReqCleanFlag=1;// 请放回底座自清洁
		SewageVar.xpSewageLVEnable=0;// 20220106
		WorkVar.ucWorkOrderoffcnt=0;
		WorkVar.ucWorkOrderStatusback=0;
		}
 	}
 else
 	{
	SewageVar.xpSewageLVEnable=0;// 20220106
	PumpVar.ucPumpGearValue=0;       // 20211117
	WaterElecVar.waterelec_OK_flag=0;// 20210924
	WorkVar.ucWorkOrderoffcnt=0;
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
 	}
}

void Deal_IDLE_BMS(void)
{
	ACleanVar.xpAutoDryStayFlag=0;// 20211230
	ACleanVar.xpAutoCleanCount	      = 0;
	ACleanVar.xpAutoCleanCount_back	  = 0;
	ACleanVar.xpAutoCleanCount_back1  = 0;
	ACleanVar.xpAutoCleanStep_back    = STEP0;	
	ACleanVar.xpAutoCleanStep_back1   = STEP0;	
//	WaterElecVar.waterelec_OK_flag=0;// 20220107
	ACleanVar.xpAutoCleanStep	      = STEP0;	
	ACleanVar.xpAutoCleanStatus       = NON_AUTO_CLEAN_MODE;	
	ACleanVar.bAutoCleanStartFlag     = 0;
	ACleanVar.bAutoCleanOverFlag      = 1;	
	ACleanVar.bAutoCleanFlag	      = 0;
	ACleanVar.bAutoDeepCleanFlag      = 0;
	SewageVar.xpSewageChangeFlag=0;//20211012
	SewageVar.xpSewageChangecnt=0;
	SewageVar.xpSewageLVEnable=0;// 20220106

 ErrorVar.xpWBoxErrorcnt=0;// 20210925
 if(TiltVar.bTiltEnableFlag)
 	{
	if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpRunErrorFlag||ErrorVar.xpWBoxErrorFlag||ErrorVar.xpCommErrorFlag)// 20211021
	   {
	   Set_Bldc_OFF(&BLDCVar);
	   Set_Brush_OFF(&BrushVar);
	   Set_WaterPump_OFF(&PumpVar);
	   Set_Dry_OFF(&DryVar);
	   Set_WaterElec_OFF(&WaterElecVar);
	   }
	else
		{
		if((KeyVar.ucKeyGearValue==1)&&\
			(BMStoMData.xpBatSocVal>0)&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR))
			{
			 if(WaterElecVar.waterelec_OK_flag==1)
			 	{
			 	 //if(WorkVar.ucFirstONFlag)// 20211108
			 	 //	{
				 //   Set_Bldc_Low(&BLDCVar);
			 	 //	}
				// else
				// 	{
					 Set_Bldc_OFF(&BLDCVar);
				// 	}
				 Set_Brush_OFF(&BrushVar);
				 Set_WaterPump_OFF(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
			 	}
			 else
			 	{
			    Set_Bldc_OFF(&BLDCVar);
			    Set_Brush_OFF(&BrushVar);
			    Set_WaterPump_OFF(&PumpVar);
			    Set_Dry_OFF(&DryVar);
				if(WaterElecVar.bEnalbeWaterElecFlag==0)
					{
					Set_WaterElec_ON(&WaterElecVar);
					}
			 	}
			}
		else
			{
			if(WaterElecVar.waterelec_OK_flag_back)// 20210928
			   {
			   WaterElecVar.waterelec_OK_flag=0;
			   WaterElecVar.waterelec_OK_flag_back=0;
			   }
//			WaterElecVar.waterelec_OK_flag=0;
//			Set_Bldc_OFF(&BLDCVar);
            if((BMStoMData.xpBatSocVal>0)&&(BMStoMData.xpErrorStatus!=BMS_LV_ERROR))
            	{// 20211008
				//if(WorkVar.ucFirstONFlag)// 20211108
				//   {
				//   Set_Bldc_Low(&BLDCVar);
				//   }
				//else
				//  {
					Set_Bldc_OFF(&BLDCVar);
				//  }
            	}
			else
				{
				Set_Bldc_OFF(&BLDCVar);
				}
			Set_Brush_OFF(&BrushVar);
			Set_WaterPump_OFF(&PumpVar);
			Set_Dry_OFF(&DryVar);
			Set_WaterElec_OFF(&WaterElecVar);
			}
		}
 	}
 else
 	{
	 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;// 20220106
	 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20220106
	 ErrorVar.xpErrorPlayCnt2=0;// 20220106
	 
	 WorkVar.ucWorkOrderStatus=OPEN_STA;
 	}

	PumpVar.ucPumpGearValue 	  = 0;					
}

void Deal_CHG_BMS(void)
{
	SewageVar.xpSewageLVEnable=0;// 20220106
	SewageVar.xpSewageChangeFlag=0;//20211012
	SewageVar.xpSewageChangecnt=0;
	ErrorVar.xpWBoxErrorcnt=0;// 20210925
	WaterElecVar.waterelec_work_cnt=0;// 20210927
	if(WaterElecVar.waterelec_OK_flag_back)// 20210928
	   {
	   WaterElecVar.waterelec_OK_flag=0;
	   WaterElecVar.waterelec_OK_flag_back=0;
	   }
	WorkVar.ucWorkOrderoffcnt=0;
	ACleanVar.xpAutoCleanStayCnt=0;
//	if(WorkVar.ucWorkOrderStatusback)
//		{
//		WorkVar.ucWorkOrderStatusback=0;
//		VoiceListpara.VoicePromptReqCleanFlag=1;// 请放回底座自清洁
//		MtoLCDData.xpACleanWarnStatus=4;//提示放回底座自清洁 20210925
//		}
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
	if(ACleanVar.xpAutoCleanStatus==WAIT_AUTO_CLEAN_MODE)
		{
		 if(++ACleanVar.xpAutoCleanStatusCnt>CLEAN_STATUS_TIME)
		 	{
		 	 ACleanVar.xpAutoCleanStatusCnt=0;
			 ACleanVar.xpAutoCleanCount    = 0;
			 ACleanVar.xpAutoCleanStep	   = STEP0;  
			 ACleanVar.xpAutoCleanCount_back= 0;
			 ACleanVar.xpAutoCleanStep_back= STEP0;  
			 ACleanVar.xpAutoCleanCount_back1= 0;
			 ACleanVar.xpAutoCleanStep_back1= STEP0;  
			 WaterElecVar.waterelec_OK_flag=0;// 20220107
			 ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	 
			 ACleanVar.bAutoCleanStartFlag = 0;
			 ACleanVar.bAutoCleanOverFlag  = 1;  
			 ACleanVar.bAutoCleanFlag	   = 0;
			 ACleanVar.bAutoDeepCleanFlag  = 0;
		 	}
		}
	else
		{
		 ACleanVar.xpAutoCleanStatusCnt=0;
		}
	if((ACleanVar.bAutoCleanFlag||ACleanVar.bAutoDeepCleanFlag)&&(ACleanVar.xpAutoCleanStatus==AUTO_CLEAN_MODE))
		{
		 WorkVar.ucWorkOrderStatus=AUTO_STA;
		}
	if(ACleanVar.xpAutoDryStayFlag&&BMStoMData.xpBatSocVal>=SOC_15_VALUE)// 20211230
		{
		WorkVar.ucWorkOrderStatus=AUTO_STA;
		ACleanVar.bAutoCleanFlag	=1;
		ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //自清洁  
		ACleanVar.xpAutoCleanCount	  = 0;
		ACleanVar.xpAutoCleanStep	  = STEP7;	
		ACleanVar.xpAutoDrySoloFlag=0;// 20210924
		KeyVar.bFastVoiceFlag = 1;
		Voice.ucSendVoiceOVcnt=0;
		ACleanVar.xpAutoDryStayFlag=0;
		}
}

void Deal_AUTO_BMS(void)
{ 
//    uint8_t i=0;
	BrushVar.bEnalbeNGBrushFlag=1;// 20211105
	SewageVar.xpSewageLVEnable=1;// 20220106
	SewageVar.xpSewageChangeFlag=0;//20211012
	SewageVar.xpSewageChangecnt=0;
	WorkVar.ucWorkOrderoffcnt=0;
	WorkVar.ucWorkOrderStatusback=0;
//    if((ACleanVar.xpAutoCleanStatus==AUTO_CLEAN_MODE)&&(ErrorVar.xpServiceErrorFlag==0)&&(ErrorVar.xpRunErrorFlag==0)\
//		&&((ACleanVar.xpAutoCleanStep==STEP7)||((ErrorVar.xpWBoxErrorFlag==0)&&(WaterBSta.ucQBucketStatus == QBULET_FULL_STA)))&&(ErrorVar.xpCommErrorFlag==0))
		if((ACleanVar.xpAutoCleanStatus==AUTO_CLEAN_MODE)&&(ErrorVar.xpServiceErrorFlag==0)&&(ErrorVar.xpRunErrorFlag==0)\
			&&(ErrorVar.xpWBoxErrorFlag==0)&&(WaterBSta.ucQBucketStatus == QBULET_FULL_STA)&&(ErrorVar.xpCommErrorFlag==0))
    	{
			ACleanVar.xpAutoCleanStayCnt=0;
			ACleanVar.xpAutoCleanPromptCount=0;// 20211011
			/*
            if(ACleanVar.xpAutoCleanStep==STEP7)
            	{
				if(ACleanVar.xpAutoCleanCount<AUTO_MAX_TIME)ACleanVar.xpAutoCleanCount+=100;	
            	}
			else
				{
				if(ACleanVar.xpAutoCleanCount<AUTO_MAX_TIME)ACleanVar.xpAutoCleanCount++;	
				}	
			*/
			if(ACleanVar.xpAutoCleanCount<AUTO_MAX_TIME)ACleanVar.xpAutoCleanCount++;	
		    switch(ACleanVar.xpAutoCleanStep){
				case STEP0:// 消毒水生成
		            //if(ACleanVar.bAutoDeepCleanFlag==1)
					//	{
					ACleanVar.xpAutoCleanPromptFlag=1;// 20211011
					if(WaterElecVar.waterelec_OK_flag)
						{
						if(ACleanVar.xpAutoCleanStep_back1>0)
						   {
							ACleanVar.xpAutoCleanCount = ACleanVar.xpAutoCleanCount_back1;
							ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back1;
						   }
						else
						   {
						   ACleanVar.xpAutoCleanCount = 0;
						   ACleanVar.xpAutoCleanStep=STEP1;
						   }
						ACleanVar.xpAutoCleanCount_back= 0;
						ACleanVar.xpAutoCleanStep_back=STEP0;							   
						}
					else
						{
							if(ACleanVar.xpAutoCleanCount<AUTO_STEP0_TIME)
							{		
								PumpVar.ucPumpGearValue=0;
								Set_Bldc_OFF(&BLDCVar);
								Set_Brush_OFF(&BrushVar);
								Set_WaterPump_OFF(&PumpVar);
								Set_Dry_OFF(&DryVar);
								Set_WaterElec_ON(&WaterElecVar);
								if(WaterElecVar.waterelec_work_cnt!=ACleanVar.xpAutoCleanCount)
//									if(WaterElecVar.waterelec_work_cnt==0)
									{
									 WaterElecVar.waterelec_work_cnt=ACleanVar.xpAutoCleanCount;
									}
							}
							else
							{
							 if(ACleanVar.xpAutoCleanStep_back1>0)
							 	{
								 ACleanVar.xpAutoCleanCount = ACleanVar.xpAutoCleanCount_back1;
								 ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back1;
							 	}
							 else
							 	{
								ACleanVar.xpAutoCleanCount = 0;
								ACleanVar.xpAutoCleanStep=STEP1;
							 	}
							 ACleanVar.xpAutoCleanCount_back= 0;
							 ACleanVar.xpAutoCleanStep_back=STEP0;								
							}
						}
					//	}
					//else
					//	{
					//	ACleanVar.xpAutoCleanCount = 0;
					//	ACleanVar.xpAutoCleanStep=STEP1;								
					//	}
					ErrorVar.xpWBoxErrorcnt=0;// 20210925					
				    break;
				case STEP1:// 地刷清洁
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP1_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==10)
						 	{
						 	 if(ACleanVar.bAutoDeepCleanFlag==0)
						 	 	{
								 ACleanVar.bAutoCleanStartFlag = 1;//开始自清洁
								 ACleanVar.bAutoCleanOverFlag= 0;
						 	 	}
							 if(KeyVar.bVoiceFlag==0)
							 	{
							 	 VoiceListpara.VoicePromptStartCleanFlag=1;
							 	}
							 else
							 	{
							 	KeyVar.bVoiceFlag=0;
							 	}
							 VoiceListpara.VoicePromptBrushCleaningFlag=1;//地刷清洁中
						 	}
						 PumpVar.ucPumpGearValue=3;//水泵流量值
						 Set_Bldc_OFF(&BLDCVar);
						 Set_Brush_Normal(&BrushVar);
						 Set_WaterPump_ON(&PumpVar);
						 Set_Dry_OFF(&DryVar);
						 Set_WaterElec_OFF(&WaterElecVar);
						}
					else
						{
						ACleanVar.xpAutoCleanCount = 0;
						ACleanVar.xpAutoCleanStep=STEP2;								
						}
//					ACleanVar.xpAutoCleanNorCount=10500-ACleanVar.xpAutoCleanCount;
//					ACleanVar.xpAutoCleanNorCount=11000-ACleanVar.xpAutoCleanCount;
					ACleanVar.xpAutoCleanNorCount=14000-ACleanVar.xpAutoCleanCount;// 20211230
					ErrorVar.xpWBoxErrorcnt=0;// 20210925
					break;
				case STEP2:// 管道清洁中
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP2_TIME)
						{	
						    if(ACleanVar.xpAutoCleanCount==1)
						    	{
								VoiceListpara.VoicePromptTubeCleaningFlag=1;//管道清洁中
						    	}
							PumpVar.ucPumpGearValue = 0;
//							Set_Bldc_Strong(&BLDCVar);
							Set_Bldc_Low(&BLDCVar);
							Set_Brush_Normal(&BrushVar);
							Set_WaterPump_OFF(&PumpVar);
							Set_Dry_OFF(&DryVar);
							Set_WaterElec_OFF(&WaterElecVar);
						}
						else
						{
							ACleanVar.xpAutoCleanCount = 0;
							ACleanVar.xpAutoCleanStep=STEP3;								
						}	

						ACleanVar.xpAutoCleanNorCount=12000-ACleanVar.xpAutoCleanCount;// 20211230
//						ACleanVar.xpAutoCleanNorCount=9000-ACleanVar.xpAutoCleanCount;
//						ACleanVar.xpAutoCleanNorCount=8500-ACleanVar.xpAutoCleanCount;
					ErrorVar.xpWBoxErrorcnt=0;// 20210925
					break;
		         case STEP3:// 智能检测
 /////////////////////////////20220322//////////////////////////////////////
					 if(ACleanVar.xpAutoCleanCount < AUTO_STEP3_1_TIME)
					 {				 
					     if(ACleanVar.xpAutoCleanCount==10)
						     {
							 VoiceListpara.VoicePromptIntelligentDetFlag=1;//智能检测中
						     }
						 PumpVar.ucPumpGearValue = 0;//水泵流量值
						 Set_Bldc_OFF(&BLDCVar);
						 Set_Brush_Normal(&BrushVar);
						 Set_WaterPump_ON(&PumpVar);
						 Set_Dry_OFF(&DryVar);
						 Set_WaterElec_OFF(&WaterElecVar);
					 }
					 else if(ACleanVar.xpAutoCleanCount < AUTO_STEP3_TIME)
					 {				 
//					     if(ACleanVar.xpAutoCleanCount==10)
//						     {
//							 VoiceListpara.VoicePromptIntelligentDetFlag=1;//智能检测中
//						     }
						 PumpVar.ucPumpGearValue = 3;//水泵流量值
						 Set_Bldc_OFF(&BLDCVar);
						 Set_Brush_Normal(&BrushVar);
						 Set_WaterPump_ON(&PumpVar);
						 Set_Dry_OFF(&DryVar);
						 Set_WaterElec_OFF(&WaterElecVar);
					 }
					 else
					 {
						 ACleanVar.xpAutoCleanCount = 0;					 
//						 if(ADCVar.uiSewageLVAdcValue>SewageVar.xpSewageDefaultRefValue)ACleanVar.xpAutoCleanStep=STEP5;  //低污染  //执行第五步
//						 else ACleanVar.xpAutoCleanStep=STEP4;								 //污染比较大，执行第四步
						 ACleanVar.xpAutoCleanStep=STEP5;// 20220227
					 }	 
//					 ACleanVar.xpAutoCleanNorCount=7500-ACleanVar.xpAutoCleanCount;
//					 ACleanVar.xpAutoCleanNorCount=8000-ACleanVar.xpAutoCleanCount;
/////////////////////////////20220322//////////////////////////////////////
					 ACleanVar.xpAutoCleanNorCount=10000-ACleanVar.xpAutoCleanCount;// 20211230
					 ErrorVar.xpWBoxErrorcnt=0;// 20210925
		            break;
				case STEP4:// 自清洁强
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP4_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==10)
						    {
							VoiceListpara.VoicePromptDeepCleaningFlag=1;//深度清洁中
						    }
						PumpVar.ucPumpGearValue    = 0;
						Set_WaterPump_OFF(&PumpVar);
//							Set_Bldc_Strong(&BLDCVar);
						Set_Bldc_Low(&BLDCVar);
						Set_Brush_Normal(&BrushVar);
						Set_Dry_OFF(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						/*
						VoiceListpara.VoicePromptCleanOverFlag=1;// 自清洁完毕	
						ACleanVar.xpAutoCleanPromptFlag=0;// 20211011
						ACleanVar.xpAutoCleanStep=STEP6;
						*/
						///////20220227
						VoiceListpara.VoicePromptCleanOverFlag=1;// 自清洁完毕	
						ACleanVar.xpAutoCleanPromptFlag=0;// 20211011
						ACleanVar.xpAutoCleanStep=STEP6;
						ACleanVar.xpAutoCleanCount = 0; 	
						ACleanVar.xpAutoCleanCount_back1= 0;
						WaterElecVar.waterelec_OK_flag=0;// 20220107
						ACleanVar.xpAutoCleanStep_back1=STEP0;
						//////////
						}
					ACleanVar.xpAutoCleanNorCount=8000-ACleanVar.xpAutoCleanCount;// 20211230
//					ACleanVar.xpAutoCleanNorCount=6000-ACleanVar.xpAutoCleanCount;
					ErrorVar.xpWBoxErrorcnt=0;// 20210925
				    break;
				case STEP5:// 自清洁弱
					if(ACleanVar.xpAutoCleanCount < AUTO_STEP5_TIME)				//运行30s
					    { 
						if(ACleanVar.xpAutoCleanCount==10)
						   {
						     VoiceListpara.VoicePromptDeepCleaningFlag=1;//深度清洁中
						   }
						PumpVar.ucPumpGearValue = 0;			//水泵流量值=0;
						Set_WaterPump_OFF(&PumpVar);
//							Set_Bldc_Strong(&BLDCVar);
						Set_Bldc_Low(&BLDCVar);
						Set_Brush_Normal(&BrushVar);
						Set_Dry_OFF(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else  if(ACleanVar.xpAutoCleanCount<AUTO_STEP4_TIME)				//运行30s
						{
						PumpVar.ucPumpGearValue = 0;			//水泵流量值=0;
						Set_WaterPump_OFF(&PumpVar);
						Set_Bldc_Low(&BLDCVar);
						Set_Brush_Normal(&BrushVar);
						Set_Dry_OFF(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
						}
					else
					    {
						VoiceListpara.VoicePromptCleanOverFlag=1;// 自清洁完毕	
						ACleanVar.xpAutoCleanPromptFlag=0;// 20211011
						ACleanVar.xpAutoCleanStep=STEP6;
						ACleanVar.xpAutoCleanCount = 0; 	
						ACleanVar.xpAutoCleanCount_back1= 0;
						WaterElecVar.waterelec_OK_flag=0;// 20220107
						ACleanVar.xpAutoCleanStep_back1=STEP0;								
						}
/////////////////////////////20220322//////////////////////////////////////
					ACleanVar.xpAutoCleanNorCount=8000-ACleanVar.xpAutoCleanCount;// 20211230
//					ACleanVar.xpAutoCleanNorCount=6000-ACleanVar.xpAutoCleanCount;
					ErrorVar.xpWBoxErrorcnt=0;// 20210925
					break;
				case STEP6:// 提醒清理污水箱
					PumpVar.ucPumpGearValue = 0;			//水泵流量值=0;
					Set_WaterPump_OFF(&PumpVar);
					Set_Bldc_OFF(&BLDCVar);
					Set_Brush_OFF(&BrushVar);
					Set_Dry_OFF(&DryVar);
					Set_WaterElec_OFF(&WaterElecVar);
					ErrorVar.xpWBoxErrorcnt=Recover_WBLANK_WARING_TIME;
					if(ACleanVar.xpAutoCleanCount < AUTO_WARN_TIME)	
					    { 
						if(ACleanVar.xpAutoCleanCount==200)
						   {
						     VoiceListpara.VoicePromptDirtyCleanFlag=1;//请清理污水箱
							 MtoLCDData.xpWWaterBoxStatus=WBULET_FULL_STA;
						   }
						}
					else
						{
						//MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA+1;// 20220111
						if(BMStoMData.xpBatSocVal<SOC_15_VALUE)// 20211229
							{
							ACleanVar.xpAutoDryStayFlag=1;// 20211230
							ACleanVar.xpAutoCleanStep  = STEP9;
							ACleanVar.xpAutoCleanCount = AUTO_STEP8_TIME; 				
							}
						else
							{
							ACleanVar.xpAutoCleanStep  = STEP7;
							ACleanVar.xpAutoCleanCount = 0; 				
							}
						}
					ACleanVar.xpAutoCleanNorCount=0;
				    break;
				case STEP7:// 烘干
				if(ACleanVar.xpAutoDrySoloFlag==0)
					{
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP6_TIME)				//运行1h
					   { 
//						if(ACleanVar.xpAutoCleanCount==1000)
							if(ACleanVar.xpAutoCleanCount==10)
							{
						     VoiceListpara.VoicePromptBrushDryFlag=1;//滚刷烘干中
							}
//						if(ACleanVar.xpAutoCleanCount<30000)
							if(ACleanVar.xpAutoCleanCount<300)
							{
							BrushVar.uiBrushSlowMode=0;
							}
						else
							{
							BrushVar.uiBrushSlowMode=1;
							}
						Set_Brush_Slow(&BrushVar);

						Set_Dry_ON(&DryVar);
						Set_Bldc_OFF(&BLDCVar);
						Set_WaterPump_OFF(&PumpVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						ACleanVar.xpAutoCleanStep=STEP8;
						ACleanVar.xpAutoCleanCount = 0; 				
						}
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP6_TIME)
						{
						ACleanVar.xpAutoCleanNorCount=AUTO_STEP6_TIME-ACleanVar.xpAutoCleanCount;
						}
					else
						{
						ACleanVar.xpAutoCleanNorCount=0;
						}
					}
				else
					{
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP6_TIME/2)				//运行1h
					   { 
//						if(ACleanVar.xpAutoCleanCount==10000)
							if(ACleanVar.xpAutoCleanCount==10)
							{
						     VoiceListpara.VoicePromptBrushDryFlag=1;//滚刷烘干中
							}
//						if(ACleanVar.xpAutoCleanCount<30000)
							if(ACleanVar.xpAutoCleanCount<300)
							{
							BrushVar.uiBrushSlowMode=0;
							}
						else
							{
							BrushVar.uiBrushSlowMode=1;
							}
						Set_Brush_Slow(&BrushVar);
						Set_Dry_ON(&DryVar);
						Set_Bldc_OFF(&BLDCVar);
						Set_WaterPump_OFF(&PumpVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						ACleanVar.xpAutoCleanStep=STEP8;
						ACleanVar.xpAutoCleanCount = 0; 				
						}
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP6_TIME/2)
						{
						ACleanVar.xpAutoCleanNorCount=AUTO_STEP6_TIME/2-ACleanVar.xpAutoCleanCount;
						}
					else
						{
						ACleanVar.xpAutoCleanNorCount=0;
						}
					}
				
				    ErrorVar.xpWBoxErrorcnt=0;// 20210925
					break;
			    case STEP8:// 自清洁结束
//					VoiceListpara.VoicePromptBrushDryOverFlag=1;//烘干完毕 
					Set_Bldc_OFF(&BLDCVar);
					Set_Brush_OFF(&BrushVar);
					Set_WaterPump_OFF(&PumpVar);
					Set_Dry_OFF(&DryVar);
					Set_WaterElec_OFF(&WaterElecVar);
					if(ACleanVar.xpAutoCleanStep_back>0)
					  {
					   if(ACleanVar.xpAutoCleanStep_back==STEPELEC)ACleanVar.xpAutoCleanStep=STEP0;
					   else
						  {
						  ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back;
						  }
			            //ACleanVar.xpAutoCleanCount=ACleanVar.xpAutoCleanCount_back;
						//ACleanVar.xpAutoCleanCount_back=0;
						//ACleanVar.xpAutoCleanStep_back=0;
						ACleanVar.bAutoCleanFlag=1;
					    ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
					   }
					else
					   {
					    ACleanVar.xpAutoCleanCount	  = 0;
					    ACleanVar.xpAutoCleanStep	  = STEP0;	
					    ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;
						ACleanVar.bAutoCleanStartFlag = 0;
					    ACleanVar.bAutoCleanOverFlag  = 1;	
						ACleanVar.bAutoCleanFlag	  = 0;
					   }
					PumpVar.ucPumpGearValue 	  = 0;					
					ACleanVar.bAutoCleanStartFlag = 0;
					ACleanVar.bAutoCleanOverFlag  = 1;	
					WorkVar.ucWorkOrderStatus     = CHARGE_STA;
//					Clear_VoiceFlag(&VoiceListpara);// 20220106
//					VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
					break;
			case STEP9:// 按键中断自清洁
				ErrorVar.xpWBoxErrorcnt=0;// 20210925
				if(ACleanVar.xpAutoCleanCount<AUTO_STEP8_TIME)
					{
					PumpVar.ucPumpGearValue 	  = 0;					
//					Set_WaterPump_ON(&PumpVar);
					Set_WaterPump_OFF(&PumpVar);// 20211119
					Set_Bldc_Low(&BLDCVar);
//					Set_Bldc_Strong(&BLDCVar);
					Set_Brush_Normal(&BrushVar);
					}
				else
					{
					if(ACleanVar.xpAutoCleanStep_back||ACleanVar.xpAutoCleanStep_back1)
					  {
					  /*
					   if(ACleanVar.xpAutoCleanStep_back==STEPELEC)
					   	{
						   ACleanVar.xpAutoCleanStep=STEP0;
						   ACleanVar.xpAutoCleanCount=ACleanVar.xpAutoCleanCount_back;
					   	}
					   else
						  {
						  ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back1;
						  ACleanVar.xpAutoCleanCount=ACleanVar.xpAutoCleanCount_back1;
						  }
					 */
						ACleanVar.bAutoCleanFlag=1;
					    ACleanVar.xpAutoCleanStatus=WAIT_AUTO_CLEAN_MODE;
					   }
					else
						{
					    ACleanVar.xpAutoCleanCount	  = 0;
					    ACleanVar.xpAutoCleanStep	  = STEP0;	
					    ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;
						ACleanVar.bAutoCleanStartFlag = 0;
					    ACleanVar.bAutoCleanOverFlag  = 1;	
						ACleanVar.bAutoCleanFlag	  = 0;
						}
					ACleanVar.bAutoDeepCleanFlag  = 0;
					WorkVar.ucWorkOrderStatus	  = CHARGE_STA;
//					Clear_VoiceFlag(&VoiceListpara);// 20220106
					VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
					}
//				PumpVar.ucPumpGearValue 	  = 0;					
//				Set_WaterPump_OFF(&PumpVar);
				Set_Dry_OFF(&DryVar);
				Set_WaterElec_OFF(&WaterElecVar);
				break;					
				default:
				break;
		    	}
    	}
	else
		{
		 if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpCommErrorFlag)// 20211119
//			 if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpCommErrorFlag||ErrorVar.xpRunErrorFlag)
			{
			ACleanVar.xpAutoCleanStayCnt  = 0;
			ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	 
			ACleanVar.xpAutoCleanCount	  = 0;
			ACleanVar.xpAutoCleanStep	  = STEP0;	
			ACleanVar.bAutoCleanStartFlag = 0;
			ACleanVar.bAutoCleanOverFlag  = 1;	
			ACleanVar.bAutoCleanFlag	   = 0;
			ACleanVar.bAutoDeepCleanFlag  = 0;
			}
		 else if(ACleanVar.xpAutoCleanStatus==WAIT_AUTO_CLEAN_MODE)
			{
				 if(++ACleanVar.xpAutoCleanStayCnt>AUTO_STAY_TIME)
				 	{
					 WorkVar.ucWorkOrderStatus= CHARGE_STA;
					 Clear_VoiceFlag(&VoiceListpara);// 20220106
					 VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
					 ACleanVar.xpAutoCleanStayCnt=0;
					 ACleanVar.xpAutoCleanStatusCnt=0;
					 ACleanVar.xpAutoCleanCount    = 0;
					 ACleanVar.xpAutoCleanStep	   = STEP0;  
					 ACleanVar.xpAutoCleanCount_back= 0;
					 ACleanVar.xpAutoCleanStep_back= STEP0;  
					 ACleanVar.xpAutoCleanCount_back1= 0;
					 ACleanVar.xpAutoCleanStep_back1= STEP0;  
					 WaterElecVar.waterelec_OK_flag=0;// 20220107
					 ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	 
					 ACleanVar.bAutoCleanStartFlag = 0;
					 ACleanVar.bAutoCleanOverFlag  = 1;  
					 ACleanVar.bAutoCleanFlag	   = 0;
					 ACleanVar.bAutoDeepCleanFlag  = 0;
				 	}
			}
		 else if(ACleanVar.xpAutoCleanStatus!=AUTO_CLEAN_MODE)
		 	{
			 WorkVar.ucWorkOrderStatus= CHARGE_STA;
//			 Clear_VoiceFlag(&VoiceListpara);// 20220106
//			 VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电
			 ACleanVar.xpAutoCleanStayCnt=0;
			 ACleanVar.xpAutoCleanStatusCnt=0;
			 ACleanVar.xpAutoCleanCount    = 0;
			 ACleanVar.xpAutoCleanStep	   = STEP0;  
			 ACleanVar.xpAutoCleanCount_back= 0;
			 ACleanVar.xpAutoCleanStep_back= STEP0;  
			 ACleanVar.xpAutoCleanCount_back1= 0;
			 ACleanVar.xpAutoCleanStep_back1= STEP0;  
			 WaterElecVar.waterelec_OK_flag=0;// 20220107
			 ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	 
			 ACleanVar.bAutoCleanStartFlag = 0;
			 ACleanVar.bAutoCleanOverFlag  = 1;  
			 ACleanVar.bAutoCleanFlag	   = 0;
			 ACleanVar.bAutoDeepCleanFlag  = 0;
		 	}
		PumpVar.ucPumpGearValue 	  = 0;					//水泵流量值
		Set_Bldc_OFF(&BLDCVar);
		Set_Brush_OFF(&BrushVar);
		Set_WaterPump_OFF(&PumpVar);
		Set_Dry_OFF(&DryVar);
		Set_WaterElec_OFF(&WaterElecVar);
		}
}

void Deal_WorkProcess(void)
{//  本地工作状态负载控制处理
 uint8_t eeprom_i=0;
 if(TimeVar.bPowerOn1sCount>=1000)
 	{
 	#if TEST_LIFE_ENABLE==1
	ErrorVar.xpWBoxErrorcnt=0;// 20210925
	ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
	ACleanVar.xpAutoCleanStep_back=0;
	ACleanVar.xpAutoCleanCount_back=0;
	ACleanVar.xpAutoCleanStep_back1=0;
	WaterElecVar.waterelec_OK_flag=0;// 20220107
	ACleanVar.xpAutoCleanCount_back1=0;
	ACleanVar.xpAutoCleanStep=0;
	ACleanVar.xpAutoCleanCount=0;
	ACleanVar.bAutoCleanFlag	  = 0;
	ACleanVar.bAutoDeepCleanFlag  = 0;
	if(ErrorVar.xpServiceErrorFlag||ErrorVar.xpRunErrorFlag||ErrorVar.xpWBoxErrorFlag||ErrorVar.xpCommErrorFlag)
	   {
		SewageVar.xpSewageChangeFlag=0;//20211012
		SewageVar.xpSewageChangecnt=0;
		Set_Bldc_OFF(&BLDCVar);
		Set_Brush_OFF(&BrushVar);
		Set_WaterPump_OFF(&PumpVar);
		Set_Dry_OFF(&DryVar);
		Set_WaterElec_OFF(&WaterElecVar);
	   }
   else
       {
	   PumpVar.ucPumpGearValue=0;
	   Set_Bldc_Low(&BLDCVar);
	   Set_Brush_Normal(&BrushVar);
	   Set_WaterPump_ON(&PumpVar);
	   Set_Dry_OFF(&DryVar);
	   Set_WaterElec_OFF(&WaterElecVar);
       }
	#else
	if(FctUART1COMMpara.bTestModeFlag)
	   {
//	   if(++FctUART1COMMpara.uiFctTestCnt>FCT_UART1_OVER_TIMEOUT)// 
//	   	{
//	   	   FctUART1COMMpara.uiFctTestCnt=FCT_UART1_OVER_TIMEOUT;
//		   MtoFCTData.xpTesProgress=TEST_OVER;
//		   FctUART1COMMpara.bClearMemoryCnt=0;
//	   	}
//	   else
//	   	{
		   switch(FCTtoMData.xpTesStep){
			   case STEP1:
				   Set_Bldc_OFF(&BLDCVar);
				   Set_Brush_OFF(&BrushVar);
				   Set_WaterPump_OFF(&PumpVar);
				   Set_Dry_OFF(&DryVar);
				   Set_WaterElec_OFF(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=CLOSE_STA;
				   MtoLCDData.xpWorkStatus=CLOSE_STA;
			   	   FctUART1COMMpara.bClearMemoryCnt=0;
				   MtoFCTData.xpTesStep=STEP1;
				   MtoFCTData.xpTesProgress=TEST_RUNNING;
				   break;
			   case STEP2:
				   Set_Bldc_OFF(&BLDCVar);
				   Set_Brush_OFF(&BrushVar);
				   Set_WaterPump_OFF(&PumpVar);
				   Set_Dry_OFF(&DryVar);
				   Set_WaterElec_OFF(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=CLOSE_STA;
				   MtoLCDData.xpWorkStatus=CLOSE_STA;
			   	   FctUART1COMMpara.bClearMemoryCnt=0;
				   MtoFCTData.xpTesStep=STEP2;
				   MtoFCTData.xpTesProgress=TEST_RUNNING;
				   break;
			   case STEP3:
			       PumpVar.ucPumpGearValue=3;
				   Set_Bldc_Low(&BLDCVar);
				   Set_Brush_Normal(&BrushVar);
				   Set_WaterPump_ON(&PumpVar);
				   Set_Dry_ON(&DryVar);
				   Set_WaterElec_ON(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=OPEN_STA;
				   MtoLCDData.xpWorkStatus=CLOSE_STA;
			   	   FctUART1COMMpara.bClearMemoryCnt=0;
				   MtoFCTData.xpTesStep=STEP3;
				   MtoFCTData.xpTesProgress=TEST_RUNNING;
				   break;
			   case STEP4:
				   Set_Bldc_OFF(&BLDCVar);
				   Set_Brush_OFF(&BrushVar);
				   Set_WaterPump_OFF(&PumpVar);
				   Set_Dry_OFF(&DryVar);
				   Set_WaterElec_OFF(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=CLOSE_STA;
				   MtoLCDData.xpWorkStatus=TEST_STA;
				   MtoFCTData.xpTesStep=STEP4;
				   break;
			   case STEP5:
				   Set_Bldc_OFF(&BLDCVar);
				   Set_Brush_OFF(&BrushVar);
				   Set_WaterPump_OFF(&PumpVar);
				   Set_Dry_OFF(&DryVar);
				   Set_WaterElec_OFF(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=CLOSE_STA;
				   MtoLCDData.xpWorkStatus=CLOSE_STA;
			   	   if(FctUART1COMMpara.bClearMemoryCnt==0)
			   	   	{
				   	   E2prom.bEnableEEpromFlag=1;
					   for(eeprom_i=5;eeprom_i<100;eeprom_i++)
					   	{
					   	E2prom.uiWriteDataFlash[eeprom_i]=0;
					   	}
			   	   	}
				   if(++FctUART1COMMpara.bClearMemoryCnt>100)
				   	{
					   MtoFCTData.xpTesProgress=TEST_OVER;
					   FctUART1COMMpara.bClearMemoryCnt=100;
				   	}
				   else
				   	{
					   MtoFCTData.xpTesProgress=TEST_RUNNING;
				   	}
				   MtoFCTData.xpTesStep=STEP5;
				   break;
			   default:
				   Set_Bldc_OFF(&BLDCVar);
				   Set_Brush_OFF(&BrushVar);
				   Set_WaterPump_OFF(&PumpVar);
				   Set_Dry_OFF(&DryVar);
				   Set_WaterElec_OFF(&WaterElecVar);
				   MtoBLDCData.xpWorkStatus=CLOSE_STA;
				   MtoLCDData.xpWorkStatus=CLOSE_STA;
			   	   FctUART1COMMpara.bClearMemoryCnt=0;
				   MtoFCTData.xpTesProgress=TEST_STAY;
				   break;
//			   }
	   	}
	   }
	else
	   {
		 switch(WorkVar.ucWorkOrderStatus){
		 	case OPEN_STA:
		        Deal_OPEN_BMS();
				break;
			case IDLE_STA:
				Deal_IDLE_BMS();
				break;
			case CLOSE_STA:
		        Deal_CLOSE_BMS();
				break;
			case CHARGE_STA:
			    Deal_CHG_BMS();	
				break;
			case AUTO_STA:
			    Deal_AUTO_BMS();	
				break;
		    default:
				break;
		 	}
		}
	 #endif
 	}
}

//****************负载检测控制处理函数*******************
///*功能：2ms执行一次	控制主电机的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//*******************************************************
void ControlAndCheck_Load(void)
{// 2ms
	if(FctUART1COMMpara.bTestModeFlag)
		{
		 switch(TimeVar.bTmrRun02msCount){
		    case STEP0:// 10ms
			  Check_Tilt(&TiltVar);										      // 倾倒开关检测
				Test_Control_Brush(&BrushVar,&ADCVar);							  // 地刷控制函数	
				Test_Control_Pump(&PumpVar,&WaterBSta,&BrushVar,&WorkVar);		  // 水泵控制函数	
				Test_Control_Dry(&DryVar);										  // 烘干控制函数	
				Control_MainMotor(&BLDCVar,&MtoBLDCData);						  // 主电机控制函数 
				Control_WaterElec_Test(&WaterElecVar,&VoiceListpara,&WorkVar);	  // 电解水控制函数
				break;
			case STEP1:// 10ms
				Deal_WorkProcess(); 											  // 各种工作模式负载控制处理函数
				break;
			case STEP2:// 10ms
			    Check_SewageStage0(&WaterBSta,&SewageVar,&ADCVar);			      // 污水箱水位检测
				break;
			case STEP3:// 10ms
				break;
			case STEP4:// 10ms
				break;
		    default:
				break;
		 	}
		}
    else
    	{
		 switch(TimeVar.bTmrRun02msCount){
		    case STEP0:// 10ms
				Check_Tilt(&TiltVar);                                        // 倾倒开关检测
				Check_Charge(&CHGVar,&ADCVar);                               // 充电检测
				break;
			case STEP1:// 10ms
			    Deal_LocalStatus();                                          // 通讯工作状态和本地工作状态处理
				Deal_WorkProcess();                                          // 各种工作模式负载控制处理函数
				break;
			case STEP2:// 10ms
				Elimination_BatVolt();                                       // 地刷水泵占空比处理函数 (根据电池电量 )
				Control_MainMotor(&BLDCVar,&MtoBLDCData);                    // 主电机控制函数	
				Control_Brush(&BrushVar,&ADCVar);                            // 地刷控制函数	
				Control_Dry(&DryVar);                                        // 烘干控制函数	
				Control_Pump(&PumpVar,&WaterBSta,&BrushVar,&WorkVar);        // 水泵控制函数	
				Control_WaterElec(&WaterElecVar,&VoiceListpara,&WorkVar);    // 电解水控制函数	
				break;
			case STEP3:// 10ms
				Check_SewageStage0(&WaterBSta,&SewageVar,&ADCVar);           // 污水箱水位检测
				Check_WaterBox(&WaterBSta,&ADCVar);                          // 清水箱有水检测
				break;
			case STEP4:// 10ms
				break;
		    default:
				break;
		 	}
    	}
 if(++TimeVar.bTmrRun02msCount>4)TimeVar.bTmrRun02msCount=0;
}

