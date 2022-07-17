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
#include "charge.h"
#include "voicelist.h"


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
BMStoMCUData   BMStoMData;
BLDCtoMCUData  BLDCtoMData;

VoiceList      VoiceListpara;
//****************地刷水泵占空比处理函数*******************
///*功能：10ms执行一次	分别控制水泵/地刷的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************
void Elimination_BatVolt(void)
{
	static uint32_t uiVoltTemp=0;
	static uint8_t m=0;
	
	uiVoltTemp += BMSVar.uiBatVoltage;
	if(++m>=8)
	{
		m=0;
		uiVoltTemp>>=3;
		Deal_PumpDuty(uiVoltTemp,&PumpVar);
		Deal_BrushDuty(uiVoltTemp,&BrushVar);
		uiVoltTemp = 0;			
	}
}

void Deal_OPEN_BMS(void)
{
	if(TiltVar.bTiltEnableFlag==0)
	   {
	    if(WorkVar.ucWorkOrderStatus==OPEN_STA)
	    	{
			 if((WaterBSta.ucWBucketStatus==WBULET_NORMAL_STA)&&(BrushVar.bBrushNGErrFlag==0)&&\
			 	(BrushVar.bBrushStallErrFlag==0)&&(ErrorVar.bBmsErrFlag==0)) 
			 	{
				 if(KeyVar.ucKeyGearValue==0)
				   {// 自动模式
					 Set_Bldc_Low(&BLDCVar);
					 Set_Brush_Normal(&BrushVar);
					 Set_WaterPump_ON(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
				   }
				 else if(KeyVar.ucKeyGearValue==1)
				   {// 吸水模式
					 Set_Bldc_Strong(&BLDCVar);
					 Set_Brush_Normal(&BrushVar);
					 Set_WaterPump_OFF(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
				   }
				 else //杀菌模式
				   {
					 Set_Bldc_OFF(&BLDCVar);
					 Set_Brush_OFF(&BrushVar);
					 Set_WaterPump_OFF(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
				   }
			 	}
			 else
			 	{
				 Set_Bldc_OFF(&BLDCVar);
				 Set_Brush_OFF(&BrushVar);
				 Set_WaterPump_OFF(&PumpVar);
				 Set_Dry_OFF(&DryVar);
				 Set_WaterElec_OFF(&WaterElecVar);
			 	}
	    	}
		else
			{
			if(TiltVar.bTiltEnablebackFlag==1)WorkVar.ucWorkOrderStatus=OPEN_STA;
			Set_WaterElec_OFF(&WaterElecVar);
			Set_Bldc_OFF(&BLDCVar);
			Set_Brush_OFF(&BrushVar);
			Set_WaterPump_OFF(&PumpVar);
			Set_Dry_OFF(&DryVar);
			}
		TiltVar.bTiltEnablebackFlag=0;
	   }
	else
		{
		WorkVar.ucWorkOrderStatus=IDLE_STA;
		if(KeyVar.ucKeyGearValue==2)
		  {//杀菌模式
		    if((WaterElecVar.bEnalbeWaterElecFlag==0)&&(WaterElecVar.waterelec_OK_flag==0))
		    	{
				Voice.uiTonePromptValue = PROMPT_DISINFECT_MODE_MP3; //杀菌模式
				Voice.uiSecondTonePromptValue = PROMPT_CHECK_FULL_CLEAR_BOX_MP3; //消毒水生成中，请确保清水箱满水
				Voice.uiPreTonePromptValue = 0;
				Set_WaterElec_ON(&WaterElecVar);
		    	}
		  }
		else
		  {
		    WaterElecVar.waterelec_OK_flag=0;
			Set_WaterElec_OFF(&WaterElecVar);
		  }
		Set_Bldc_OFF(&BLDCVar);
		Set_Brush_OFF(&BrushVar);
		Set_WaterPump_OFF(&PumpVar);
		Set_Dry_OFF(&DryVar);
		TiltVar.bTiltEnablebackFlag=1;
		}
}

void Deal_CLOSE_BMS(void)
{
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
}

void Deal_IDLE_BMS(void)
{
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
}

void Deal_CHG_BMS(void)
{
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
}

void Deal_AUTO_BMS(void)
{ 
    if(ACleanVar.xpAutoCleanStatus==AUTO_CLEAN_MODE)
    	{
			if(ACleanVar.xpAutoCleanCount<AUTO_MAX_TIME)ACleanVar.xpAutoCleanCount++;	
		    switch(ACleanVar.xpAutoCleanStep){
				case STEP0:// 消毒水生成
		            if(ACleanVar.bAutoDeepCleanFlag==1)
						{
							if(ACleanVar.xpAutoCleanCount<AUTO_STEP0_TIME)
							{		
								if(ACleanVar.xpAutoCleanCount==1){					 
//									Voice.uiTonePromptValue = PROMPT_START_CLEAN_MP3;
//									Voice.uiSecondTonePromptValue = PROMPT_SOLUTION_PRODECT_MP3;	 
//									Voice.uiPreTonePromptValue = 0;

									VoiceListpara.VoicePromptStartCleanSolutionProdectFlag=1;//开始自清洁,消毒水生成中
									
									ACleanVar.bAutoCleanStartFlag = 1;//开始自清洁
									ACleanVar.bAutoCleanOverFlag= 0;
								}
								PumpVar.ucPumpGearValue=0;
								Set_Bldc_OFF(&BLDCVar);
								Set_Brush_OFF(&BrushVar);
								Set_WaterPump_OFF(&PumpVar);
								Set_Dry_OFF(&DryVar);
								Set_WaterElec_ON(&WaterElecVar);
							}
							else
							{
								ACleanVar.xpAutoCleanCount = 0;
								ACleanVar.xpAutoCleanStep=STEP1;								
							}
						}
					else
						{
						ACleanVar.xpAutoCleanCount = 0;
						ACleanVar.xpAutoCleanStep=STEP1;								
						}
				    break;
				case STEP1:// 地刷清洁
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP1_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==1)
						 	{
						 	 if(ACleanVar.bAutoDeepCleanFlag==0)
						 	 	{
//						         Voice.uiTonePromptValue = PROMPT_START_CLEAN_MP3;
//								 Voice.uiSecondTonePromptValue = PROMPT_BRUSH_CLEANING_MP3; 		
//								 Voice.uiPreTonePromptValue = 0;

								 VoiceListpara.VoicePromptStartCleanBrushCleaningFlag=1;//开始自清洁,地刷清洁中
								 ACleanVar.bAutoCleanStartFlag = 1;//开始自清洁
								 ACleanVar.bAutoCleanOverFlag= 0;
						 	 	}
							 else
							 	{
//								 Voice.uiTonePromptValue = PROMPT_BRUSH_CLEANING_MP3;
//								 Voice.uiPreTonePromptValue = 0;
								VoiceListpara.VoicePromptBrushCleaningFlag=1;//地刷清洁中
							 	}
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
					break;
				case STEP2:// 管道清洁中
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP2_TIME)
						{	
						    if(ACleanVar.xpAutoCleanCount==1)
						    	{
//								Voice.uiTonePromptValue = PROMPT_TUBE_CLEANING_MP3;
//								Voice.uiPreTonePromptValue = 0;

								VoiceListpara.VoicePromptTubeCleaningFlag=1;//管道清洁中
						    	}
							PumpVar.ucPumpGearValue = 0;
							Set_Bldc_Strong(&BLDCVar);
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
					break;
		         case STEP3:// 智能检测
					 if(ACleanVar.xpAutoCleanCount < AUTO_STEP3_TIME)
					 {				 
					     if(ACleanVar.xpAutoCleanCount==1)
						     {
//						      Voice.uiTonePromptValue = PROMPT_INTELLIGENT_DET_MP3;
//						      Voice.uiPreTonePromptValue = 0;
							 VoiceListpara.VoicePromptIntelligentDetFlag=1;//智能检测中
						     }
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
						 if(ADCVar.uiSewageLVAdcValue>SewageVar.xpSewageDefaultRefValue)ACleanVar.xpAutoCleanStep=STEP5;  //低污染  //执行第五步
						 else ACleanVar.xpAutoCleanStep=STEP4;								 //污染比较大，执行第四步
					 }	 
		            break;
				case STEP4:// 自清洁强
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP4_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==1)
						    {
//							  Voice.uiTonePromptValue = PROMPT_DEEP_CLEANING_MP3;
//						      Voice.uiPreTonePromptValue = 0;
							VoiceListpara.VoicePromptDeepCleaningFlag=1;//深度清洁中
						    }
						if(ACleanVar.xpAutoCleanCount<AUTO_PUMP15_TIME) 
						    {
							PumpVar.ucPumpGearValue    = 3;
							Set_WaterPump_ON(&PumpVar);
						    }
						else																				
						    {
							PumpVar.ucPumpGearValue    = 0;
							Set_WaterPump_OFF(&PumpVar);
						    }				
						Set_Bldc_Strong(&BLDCVar);
						Set_Brush_Normal(&BrushVar);
						Set_Dry_OFF(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						if(ACleanVar.bAutoDeepCleanFlag==1)
							{
							ACleanVar.xpAutoCleanStep=STEP6;
							ACleanVar.xpAutoCleanCount = 0;					
							}
						else
							{
							ACleanVar.xpAutoCleanStep=STEP7;
							ACleanVar.xpAutoCleanCount = 0;					
							}
						}
				    break;
				case STEP5:// 自清洁弱
					if(ACleanVar.xpAutoCleanCount < AUTO_STEP5_TIME)				//运行30s
					    { 
						if(ACleanVar.xpAutoCleanCount==1)
						   {
//							 Voice.uiTonePromptValue = PROMPT_DEEP_CLEANING_MP3;
//							 Voice.uiPreTonePromptValue = 0;
						     VoiceListpara.VoicePromptDeepCleaningFlag=1;//深度清洁中
						   }
						if(ACleanVar.xpAutoCleanCount < AUTO_PUMP5_TIME)			//前5s
						   {
							PumpVar.ucPumpGearValue = 3;				//水泵流量值
							Set_WaterPump_ON(&PumpVar);
						   }
						else																				
						   {
							PumpVar.ucPumpGearValue = 0;			//水泵流量值=0;
							Set_WaterPump_OFF(&PumpVar);
						   }				
						Set_Bldc_Strong(&BLDCVar);
						Set_Brush_Normal(&BrushVar);
						Set_Dry_OFF(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						if(ACleanVar.bAutoDeepCleanFlag==1)
							{
							ACleanVar.xpAutoCleanStep=STEP6;
							ACleanVar.xpAutoCleanCount = 0;					
							}
						else
							{
							ACleanVar.xpAutoCleanStep=STEP7;
							ACleanVar.xpAutoCleanCount = 0;					
							}
						}
					break;
				case STEP6:// 烘干
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP6_TIME)				//运行1h
					   { 
						if(ACleanVar.xpAutoCleanCount==1)
							{
//							 Voice.uiTonePromptValue = PROMPT_BRUSH_DRY_MP3;
//							 Voice.uiPreTonePromptValue = 0;

						     VoiceListpara.VoicePromptBrushDryFlag=1;//滚刷烘干中
							}
						Set_Bldc_OFF(&BLDCVar);
						Set_Brush_Slow(&BrushVar);
						Set_WaterPump_OFF(&PumpVar);
						Set_Dry_ON(&DryVar);
						Set_WaterElec_OFF(&WaterElecVar);
					    }
					else 
						{
						ACleanVar.xpAutoCleanStep=STEP7;
						ACleanVar.xpAutoCleanCount = 0; 				
						}
					break;
			    case STEP7:// 自清洁结束
					if(ACleanVar.bAutoDeepCleanFlag==1)
						{
						if(ACleanVar.xpAutoCleanStatus == AUTO_CLEAN_MODE) 
							{
//							Voice.uiTonePromptValue = PROMPT_BRUSH_DRY_OVER_MP3;
//							Voice.uiSecondTonePromptValue = PROMPT_CLEAN_OVER_MP3;
//							Voice.uiPreTonePromptValue = 0;
							
							VoiceListpara.VoicePromptBrushDryOverCleanOverFlag=1;//烘干完毕 自清洁完毕
							
							}
						}
					else
						{
						if(ACleanVar.xpAutoCleanStatus == AUTO_CLEAN_MODE) 
							{
//							 Voice.uiTonePromptValue = PROMPT_CLEAN_OVER_MP3;
//							 Voice.uiPreTonePromptValue = 0;
							VoiceListpara.VoicePromptCleanOverFlag=1;//自清洁完毕
							}
						}
					Set_Bldc_OFF(&BLDCVar);
					Set_Brush_OFF(&BrushVar);
					Set_WaterPump_OFF(&PumpVar);
					Set_Dry_OFF(&DryVar);
					Set_WaterElec_OFF(&WaterElecVar);
					PumpVar.ucPumpGearValue 	  = 0;					
					ACleanVar.xpAutoCleanCount	  = 0;
					ACleanVar.xpAutoCleanStep	  = STEP0;	
					ACleanVar.xpAutoCleanStatus    = NON_AUTO_CLEAN_MODE;	
					ACleanVar.bAutoCleanStartFlag = 0;
					ACleanVar.bAutoCleanOverFlag  = 1;	
					ACleanVar.bAutoCleanFlag	  = 0;
					ACleanVar.bAutoDeepCleanFlag  = 0;
					WorkVar.ucWorkStatus=CHARGE_STA;
					break;
				default:
					break;
		    	}
    	}
	else
		{
		ACleanVar.xpAutoCleanCount 	  = 0;
		ACleanVar.xpAutoCleanStep 	  = STEP0;	
		ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	
		ACleanVar.bAutoCleanStartFlag = 0;
		ACleanVar.bAutoCleanOverFlag  = 1;	
		ACleanVar.bAutoCleanFlag 	  = 0;
		ACleanVar.bAutoDeepCleanFlag  = 0;
		WorkVar.ucWorkStatus=CHARGE_STA;
		PumpVar.ucPumpGearValue 	  = 0;					//水泵流量值
		Set_Bldc_OFF(&BLDCVar);
		Set_Brush_OFF(&BrushVar);
		Set_WaterPump_OFF(&PumpVar);
		Set_Dry_OFF(&DryVar);
		Set_WaterElec_OFF(&WaterElecVar);
		}
}

void Deal_WorkProcess(void)
{
 switch(WorkVar.ucWorkStatus){
 	case OPEN_STA:
        Deal_OPEN_BMS();
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



void VoicePlay_LcdDisplay_OPEN_BMS(void)
{
  switch(ErrorVar.xpErrorPlayStep){
		 case STEP0:
			 if(((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))||\
			  ((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			  ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR))||\
			   BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)
				 {// 充电故障 电池包故障 主电机故障 短路故障
				   if(Voice.ucPlayVoiceValue==1)
					  {
//					 Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //机器故障，请联系售后维修
//					 Voice.uiPreTonePromptValue = 0;
				       VoiceListpara.VoicePromptMachineErrorFlag=1;//机器故障，请联系售后维修

					   ErrorVar.xpErrorPlayFlag=1;
					   ErrorVar.bBldcErrFlag=1;
					   if(BMStoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BMStoMData.xpErrorStatus;
					   if(BLDCtoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
					   if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)MtoLCDData.xpErrorStatus=BRUSH_SHORT_ERROR;
					  }
				   if(Voice.ucPlayVoiceValue==200)
					{
					   ErrorVar.xpErrorPlayStep=STEP9;
					   Voice.ucPlayVoiceValue=0;
					}
				 }
			 else
				{
				ErrorVar.xpErrorPlayStep=STEP1;
				Voice.ucPlayVoiceValue=0;
				}
			break;
		 case STEP1:
			 if(BMStoMData.xpErrorStatus==BMS_OT_ERROR)
				 {
				  if(Voice.ucPlayVoiceValue==1)
					  {
					   Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //电池温度过高，请等待降温后使用
					   Voice.uiPreTonePromptValue = 0;
					   ErrorVar.xpErrorPlayFlag=1;
					   ErrorVar.bBmsErrFlag=1;
					   MtoLCDData.xpErrorStatus=BMS_OT_ERROR;
					  }
				  if(Voice.ucPlayVoiceValue==200)
				   {
					  ErrorVar.xpErrorPlayStep=STEP9;
					  Voice.ucPlayVoiceValue=0;
				   }
				 }
			 else
				{
				 ErrorVar.xpErrorPlayStep=STEP2;
				 Voice.ucPlayVoiceValue=0;
				}
			break;
		 case STEP2:
			 if(WaterBSta.ucQBucketStatus == QBULET_BLANK_STA) 
			  {
				 if(Voice.ucPlayVoiceValue==1)
					 {
					  Voice.uiTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;  //清水箱缺水，请及时加水
					  Voice.uiPreTonePromptValue = 0;
					   ErrorVar.xpErrorPlayFlag=1;
					   ErrorVar.bRunErrFlag=1;
					   MtoLCDData.xpQWaterBoxStatus=QBULET_BLANK_STA;
					  }
				 if(Voice.ucPlayVoiceValue==200)
				  {
					 ErrorVar.xpErrorPlayStep=STEP3;
					 Voice.ucPlayVoiceValue=0;
				  }
			  }
			 else
				{
				 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;
				 ErrorVar.xpErrorPlayStep=STEP3;
				 Voice.ucPlayVoiceValue=0;
				}
			break;
		 case STEP3:
			 if(WaterBSta.ucWBucketStatus == WBULET_FULL_STA) 
			  {
				 if(Voice.ucPlayVoiceValue==1)
					 {
					 Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //污水箱已满，请及时清理
					 Voice.uiPreTonePromptValue = 0;
					 ErrorVar.bRunErrFlag=1;
					  ErrorVar.xpErrorPlayFlag=1;
					  MtoLCDData.xpWWaterBoxStatus=0x02;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				   {
					 ErrorVar.xpErrorPlayStep=STEP4;
					 Voice.ucPlayVoiceValue=0;
				   }
			  }
			 else if(WaterBSta.ucWBucketStatus == WBULET_BLANK_STA)
			  {
				 if(Voice.ucPlayVoiceValue==1)
					 {
					 Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //污水箱未安装，请安装污水箱
					 Voice.uiPreTonePromptValue = 0;
					  ErrorVar.xpErrorPlayFlag=1;
					  ErrorVar.bRunErrFlag=1;
					  MtoLCDData.xpWWaterBoxStatus=0x02;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				   {
					 ErrorVar.xpErrorPlayStep=STEP4;
					 Voice.ucPlayVoiceValue=0;
				   }
			  }
			 else
				{
				 ErrorVar.xpErrorPlayStep=STEP4;
				 Voice.ucPlayVoiceValue=0;
				 MtoLCDData.xpWWaterBoxStatus=0x01;
				}
			break;
		 case STEP4:
			 if(BrushVar.bBrushNGErrFlag) 
				 { 
				 if(Voice.ucPlayVoiceValue==1)
					 {
					 Voice.uiTonePromptValue = PROMPT_NO_BRUSH_MP3; //滚刷未安装，请安装滚刷 
					 Voice.uiPreTonePromptValue = 0;
					  ErrorVar.xpErrorPlayFlag=1;
					  ErrorVar.bRunErrFlag=1;
					  MtoLCDData.xpBrushStatus=BRUSH_NO_STA;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				   {
					 ErrorVar.xpErrorPlayStep=STEP5;
					 Voice.ucPlayVoiceValue=0;
				   }
				 }
			 else
				{
				 ErrorVar.xpErrorPlayStep=STEP5;
				 Voice.ucPlayVoiceValue=0;
				}
			break;
		case STEP5:
			if((BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)||BrushVar.bBrushStallErrFlag)
				{
				if(Voice.ucPlayVoiceValue==1)
					{
					Voice.uiTonePromptValue = PROMPT_CHECK_BRUSH_TUBE_MP3;//请检查滚刷或底部管道
					Voice.uiPreTonePromptValue = 0;
					 ErrorVar.xpErrorPlayFlag=1;
					 ErrorVar.bRunErrFlag=1;
					 if(BrushVar.bBrushStallErrFlag)MtoLCDData.xpBrushStatus=BRUSH_STALL_STA;
					}
				if(Voice.ucPlayVoiceValue==200)
				  {
				   ErrorVar.xpErrorPlayStep=STEP6;
				   Voice.ucPlayVoiceValue=0;
				  }
				}
			else
				{
				if(MtoLCDData.xpBrushStatus!=BRUSH_NO_STA)MtoLCDData.xpBrushStatus=BRUSH_YES_STA;
				ErrorVar.xpErrorPlayStep=STEP6;
				Voice.ucPlayVoiceValue=0;
				}
			break;
		case STEP6:
			if(BMStoMData.xpBatSocVal==0)//SOC_0_VALUE
				{
				if(Voice.ucPlayVoiceValue==1)
					{
					Voice.uiTonePromptValue=PROMPT_POWER_OVER_MP3;//电量已耗尽
					Voice.uiPreTonePromptValue = 0; 
					ErrorVar.bRunErrFlag=1;
					 ErrorVar.xpErrorPlayFlag=1;
					}
				if(Voice.ucPlayVoiceValue==200)
				  {
				   ErrorVar.xpErrorPlayStep=STEP7;
				   Voice.ucPlayVoiceValue=0;
				  }
				}
			else
				{
				 ErrorVar.xpErrorPlayStep=STEP7;
				 Voice.ucPlayVoiceValue=0;
				}
			break;
		 case STEP7:
			 if(BMStoMData.xpBatSocVal<SOC_10_VALUE)
				 {
				 if(Voice.ucPlayVoiceValue==1)
					 {
					 Voice.uiTonePromptValue=PROMPT_LOW_POWER_CHARGE_MP3;//电量低，请及时充电
					 Voice.uiPreTonePromptValue = 0;	 
//					  ErrorVar.xpErrorPlayFlag=1;
//					  ErrorVar.bRunErrFlag=1;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				   {
					ErrorVar.xpErrorPlayStep=STEP8;
					Voice.ucPlayVoiceValue=0;
				   }
				 }
			 else
				 {
				  ErrorVar.xpErrorPlayStep=STEP8;
				  Voice.ucPlayVoiceValue=0;
				 }
		 case STEP8:
			 if(BMStoMData.xpBatSocVal<SOC_20_VALUE)
				 {
				 if(Voice.ucPlayVoiceValue==1)
					 {
					 Voice.uiTonePromptValue=PROMPT_LOW_POWER_MP3;//电池电量低
					 Voice.uiPreTonePromptValue = 0;	 
//					  ErrorVar.xpErrorPlayFlag=1;
//					  ErrorVar.bRunErrFlag=1;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				   {
					ErrorVar.xpErrorPlayStep=STEP9;
					Voice.ucPlayVoiceValue=0;
				   }
				 }
			 else
				 {
				  ErrorVar.xpErrorPlayStep=STEP9;
				  Voice.ucPlayVoiceValue=0;
				 }
			break;
		case STEP9:
			if(ErrorVar.xpErrorPlayFlag==1)
				{
				 if(++ErrorVar.xpErrorPlayCnt>=ERROR_PLAY_CNT)
				 	{ 
				 	 ErrorVar.xpErrorPlayCnt=0;
				 	 WorkVar.ucWorkOrderStatus=IDLE_STA;
				 	}
				 if(ErrorVar.bBldcErrFlag||ErrorVar.bBmsErrFlag)
					 {
					 ErrorVar.xpErrorRecoverFlag=1;
					 }
				 else
					 {
					 ErrorVar.xpErrorRecoverFlag=0;
					 }
				WaterBSta.ucQBucketStatus=QBULET_FULL_STA;
				ErrorVar.bRunErrFlag=0;
				ErrorVar.bBldcErrFlag=0;
				ErrorVar.bBmsErrFlag=0;
				ErrorVar.xpErrorPlayFlag=0;
				BrushVar.bBrushNGErrFlag=0;
				BrushVar.bBrushStallErrFlag=0;
				}
			WorkVar.ucbackWorkOrderStatus=WorkVar.ucWorkOrderStatus;
			ErrorVar.xpErrorPlayStep=STEP0;
			Voice.ucPlayVoiceValue=0;
			break;
		default:
			break;
		}
}
void VoicePlay_LcdDisplay_IDLE_STA(void)
{
   switch(ErrorVar.xpErrorPlayStep){
			case STEP0:
				if(((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))||\
				 ((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
				 ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR))||\
				  BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)
					{// 充电故障 电池包故障 主电机故障 短路故障
					  if(Voice.ucPlayVoiceValue==1)
						 {
						  Voice.uiTonePromptValue    = PROMPT_MACHINE_ERROR_MP3; //机器故障，请联系售后维修
						  Voice.uiPreTonePromptValue = 0;
						  ErrorVar.xpErrorPlayFlag=1;
						  ErrorVar.bBldcErrFlag=1;
						  if(BMStoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BMStoMData.xpErrorStatus;
						  if(BLDCtoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
						  if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)MtoLCDData.xpErrorStatus=BRUSH_SHORT_ERROR;
						 }
					  if(Voice.ucPlayVoiceValue==200)
					   {
						  ErrorVar.xpErrorPlayStep=STEP9;
						  Voice.ucPlayVoiceValue=0;
					   }
					}
				else
				   {
				   ErrorVar.xpErrorPlayStep=STEP1;
				   Voice.ucPlayVoiceValue=0;
				   }
			   break;
			case STEP1:
				if(BMStoMData.xpErrorStatus==BMS_OT_ERROR)
					{
					 if(Voice.ucPlayVoiceValue==1)
						 {
//						  Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //电池温度过高，请等待降温后使用
//						  Voice.uiPreTonePromptValue = 0;
						  VoiceListpara.VoicePromptBatTempOverFlag=1;//电池温度过高，请等待降温后使用

						  ErrorVar.xpErrorPlayFlag=1;
						  ErrorVar.bBmsErrFlag=1;
						  MtoLCDData.xpErrorStatus=BMS_OT_ERROR;
						 }
					 if(Voice.ucPlayVoiceValue==200)
					  {
						 ErrorVar.xpErrorPlayStep=STEP9;
						 Voice.ucPlayVoiceValue=0;
					  }
				   }
				else
				   {
					ErrorVar.bBmsErrFlag=0;
					ErrorVar.xpErrorPlayStep=STEP2;
					Voice.ucPlayVoiceValue=0;
				   }
			   break;
			case STEP2:
				if(WaterBSta.ucQBucketStatus == QBULET_BLANK_STA) 
				 {
					if(Voice.ucPlayVoiceValue==1)
						{
						  ErrorVar.xpErrorPlayFlag=1;
						  ErrorVar.bRunErrFlag=1;
						  MtoLCDData.xpQWaterBoxStatus=QBULET_BLANK_STA;
						  if(TiltVar.bTiltEnableFlag==1)
						  	{
						  	WaterBSta.ucQBucketStatus=QBULET_FULL_STA;
						  	}
						 }
					if(Voice.ucPlayVoiceValue==200)
					 {
						ErrorVar.xpErrorPlayStep=STEP3;
						Voice.ucPlayVoiceValue=0;
					 }
				 }
				else
				   {
					MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;
					ErrorVar.xpErrorPlayStep=STEP3;
					Voice.ucPlayVoiceValue=0;
				   }
			   break;
			case STEP3:
				if(WaterBSta.ucWBucketStatus == WBULET_FULL_STA) 
				 {
					if(Voice.ucPlayVoiceValue==1)
						{
						 ErrorVar.xpErrorPlayFlag=1;
						 ErrorVar.bRunErrFlag=1;
						 MtoLCDData.xpWWaterBoxStatus=0x02;
						}
					if(Voice.ucPlayVoiceValue==200)
					  {
						ErrorVar.xpErrorPlayStep=STEP4;
						Voice.ucPlayVoiceValue=0;
					  }
				 }
				else if(WaterBSta.ucWBucketStatus == WBULET_BLANK_STA)
				 {
					if(Voice.ucPlayVoiceValue==1)
						{
						 ErrorVar.xpErrorPlayFlag=1;
						 ErrorVar.bRunErrFlag=1;
						 MtoLCDData.xpWWaterBoxStatus=0x02;
						}
					if(Voice.ucPlayVoiceValue==200)
					  {
						ErrorVar.xpErrorPlayStep=STEP4;
						Voice.ucPlayVoiceValue=0;
					  }
				 }
				else
				   {
					ErrorVar.xpErrorPlayStep=STEP4;
					Voice.ucPlayVoiceValue=0;
					MtoLCDData.xpWWaterBoxStatus=0x01;
				   }
			   break;
			case STEP4:
				if(BrushVar.bBrushNGErrFlag) 
					{ 
					if(Voice.ucPlayVoiceValue==1)
						{
						 ErrorVar.xpErrorPlayFlag=1;
						 ErrorVar.bRunErrFlag=1;
						 MtoLCDData.xpBrushStatus=BRUSH_NO_STA;
						 if(TiltVar.bTiltEnableFlag==1)BrushVar.bBrushNGErrFlag=0;
						}
					if(Voice.ucPlayVoiceValue==200)
					  {
						ErrorVar.xpErrorPlayStep=STEP5;
						Voice.ucPlayVoiceValue=0;
					  }
					}
				else
				   {
					ErrorVar.xpErrorPlayStep=STEP5;
					Voice.ucPlayVoiceValue=0;
				   }
			   break;
		   case STEP5:
			   if((BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)||BrushVar.bBrushStallErrFlag)
				   {
				   if(Voice.ucPlayVoiceValue==1)
					   {
						ErrorVar.xpErrorPlayFlag=1;
						ErrorVar.bRunErrFlag=1;
						if(BrushVar.bBrushStallErrFlag)
							{
							 MtoLCDData.xpBrushStatus=BRUSH_STALL_STA;
							 if(TiltVar.bTiltEnableFlag==1)BrushVar.bBrushStallErrFlag=0;
							}
					   }
				   if(Voice.ucPlayVoiceValue==200)
					 {
					  ErrorVar.xpErrorPlayStep=STEP6;
					  Voice.ucPlayVoiceValue=0;
					 }
				   }
			   else
				   {
				   if(MtoLCDData.xpBrushStatus!=BRUSH_NO_STA)MtoLCDData.xpBrushStatus=BRUSH_YES_STA;
				   ErrorVar.xpErrorPlayStep=STEP6;
				   Voice.ucPlayVoiceValue=0;
				   }
			   break;
		   case STEP6:
			   if(BMStoMData.xpBatSocVal==0)//SOC_0_VALUE
				   {
				   if(Voice.ucPlayVoiceValue==1)
					   {
//					   Voice.uiTonePromptValue=PROMPT_POWER_OVER_MP3;//电量已耗尽
//					   Voice.uiPreTonePromptValue = 0;	   
					   VoiceListpara.VoicePromptPowerOverFlag=1;//电量已耗尽

					   ErrorVar.bRunErrFlag=1;
						ErrorVar.xpErrorPlayFlag=1;
					   }
				   if(Voice.ucPlayVoiceValue==200)
					 {
					  ErrorVar.xpErrorPlayStep=STEP7;
					  Voice.ucPlayVoiceValue=0;
					 }
				   }
			   else
				   {
					ErrorVar.xpErrorPlayStep=STEP7;
					Voice.ucPlayVoiceValue=0;
				   }
			   break;
			case STEP7:
				if(BMStoMData.xpBatSocVal<SOC_10_VALUE)
					{
					if(Voice.ucPlayVoiceValue==1)
						{
//						Voice.uiTonePromptValue=PROMPT_LOW_POWER_CHARGE_MP3;//电量低，请及时充电
//						Voice.uiPreTonePromptValue = 0; 
                         VoiceListpara.VoicePromptLowPowerChargeFlag=1;//电量低，请及时充电
						
//						ErrorVar.bRunErrFlag=1;
//						 ErrorVar.xpErrorPlayFlag=1;
						}
					if(Voice.ucPlayVoiceValue==200)
					  {
					   ErrorVar.xpErrorPlayStep=STEP8;
					   Voice.ucPlayVoiceValue=0;
					  }
					}
				else
					{
					 ErrorVar.xpErrorPlayStep=STEP8;
					 Voice.ucPlayVoiceValue=0;
					}
			case STEP8:
				if(BMStoMData.xpBatSocVal<SOC_20_VALUE)
					{
					if(Voice.ucPlayVoiceValue==1)
						{
//						Voice.uiTonePromptValue=PROMPT_LOW_POWER_MP3;//电池电量低
//						Voice.uiPreTonePromptValue = 0; 	
						VoiceListpara.VoicePromptLowPowerFlag=1;//电池电量低
						
//						ErrorVar.bRunErrFlag=1;
//						 ErrorVar.xpErrorPlayFlag=1;
						}
					if(Voice.ucPlayVoiceValue==200)
					  {
					   ErrorVar.xpErrorPlayStep=STEP9;
					   Voice.ucPlayVoiceValue=0;
					  }
					}
				else
					{
					 ErrorVar.xpErrorPlayStep=STEP9;
					 Voice.ucPlayVoiceValue=0;
					}
			   break;
		   case STEP9:
			   if(ErrorVar.xpErrorPlayFlag==1)
				   {
				    if(ErrorVar.bBldcErrFlag)
				    	{
						WorkVar.ucWorkOrderStatus=IDLE_STA;
				    	}
					else if((WaterBSta.ucWBucketStatus==WBULET_NORMAL_STA)&&(WaterBSta.ucQBucketStatus==QBULET_FULL_STA)&&\
					(BrushVar.bBrushNGErrFlag==0)&&(BrushVar.bBrushStallErrFlag==0)&&(ErrorVar.bBmsErrFlag==0))	
						{
						if(TiltVar.bTiltEnableFlag==0)WorkVar.ucWorkOrderStatus=OPEN_STA;
						}
					ErrorVar.xpErrorPlayFlag=0;
				   }
			   WorkVar.ucbackWorkOrderStatus=WorkVar.ucWorkOrderStatus;
			   ErrorVar.xpErrorPlayStep=STEP0;
			   Voice.ucPlayVoiceValue=0;
			   break;
		   default:
			   break;
		   }
}

void VoicePlay_LcdDisplay_CLOSE_BMS(void)
{
	ErrorVar.xpErrorPlayStep=STEP0;
	Voice.ucPlayVoiceValue=0;
	UART2COMMpara.bBldcCommErrorFlag=0;
	UART1COMMpara.bBmsCommErrorFlag=0;
	BrushVar.bBrushNGErrFlag=0;
	BrushVar.bBrushShortErrFlag=0;
	BrushVar.bBrushStallErrFlag=0;
	PumpVar.bPumpShortErrFlag=0;
	DryVar.bDryStallErrFlag=0;
	WaterElecVar.bWaterElecStallErrFlag=0;
}
void VoicePlay_LcdDisplay_CHARGE_STA(void)
{
// 	MtoLCDData.xpBrushStatus=BRUSH_YES_STA;
//	MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;
//	MtoLCDData.xpWWaterBoxStatus=WBULET_NORMAL_STA;
    switch(ErrorVar.xpErrorPlayStep){
		case STEP0:
			if(((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))||\
			 ((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			 ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR)))
				{// 充电故障 电池包故障 主电机故障 
				  if(Voice.ucPlayVoiceValue==1)
					 {
//					Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //机器故障，请联系售后维修
//					Voice.uiPreTonePromptValue = 0;
				 	  VoiceListpara.VoicePromptMachineErrorFlag=1;//机器故障，请联系售后维修
					 						
					  ErrorVar.xpErrorPlayFlag=1;
					  if(BMStoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BMStoMData.xpErrorStatus;
					  if(BLDCtoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
					 }
				  if(Voice.ucPlayVoiceValue==200)
				   {
					  ErrorVar.xpErrorPlayStep=STEP2;
					  Voice.ucPlayVoiceValue=0;
				   }
				}
			else
			   {
			   ErrorVar.xpErrorPlayStep=STEP1;
			   Voice.ucPlayVoiceValue=0;
			   }
		   break;
		case STEP1:
			if(BMStoMData.xpErrorStatus==BMS_OT_ERROR)
				{
				 if(Voice.ucPlayVoiceValue==1)
					 {
// 				   Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //电池温度过高，请等待降温后使用
// 				   Voice.uiPreTonePromptValue = 0;
				      VoiceListpara.VoicePromptBatTempOverFlag=1;//电池温度过高，请等待降温后使用
					  ErrorVar.xpErrorPlayFlag=1;
					  MtoLCDData.xpErrorStatus=BMS_OT_ERROR;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				  {
					 ErrorVar.xpErrorPlayStep=STEP2;
					 Voice.ucPlayVoiceValue=0;
				  }
				}
			else
			   {
				ErrorVar.xpErrorPlayStep=STEP2;
				Voice.ucPlayVoiceValue=0;
			   }
		   break;
		case STEP2:
		   if(ErrorVar.xpErrorPlayFlag==1)
			   {
//				WorkVar.ucWorkStatus=IDLE_STA;
			   }
		   WorkVar.ucbackWorkOrderStatus=WorkVar.ucWorkOrderStatus;
		   ErrorVar.xpErrorPlayStep=STEP0;
		   Voice.ucPlayVoiceValue=0;
		   break;
	   default:
		   break;
	   }
}

void VoicePlay_LcdDisplay_AUTO_STA(void)
{
  switch(ErrorVar.xpErrorPlayStep){
		case STEP0:
			if(((BLDCtoMData.xpErrorStatus>NO_ERROR)&&(BLDCtoMData.xpErrorStatus<SMD_BLDC_ERROR))||\
			 ((BMStoMData.xpErrorStatus>CHG_OV_VMS_ERROR)&&(BMStoMData.xpErrorStatus<=CHG_OT_ERROR))||\
			 ((BMStoMData.xpErrorStatus>=BMS_BAT_ERROR)&&(BMStoMData.xpErrorStatus<=BMS_COMM_ERROR))||\
			  BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)
				{// 充电故障 电池包故障 主电机故障 短路故障
				  if(Voice.ucPlayVoiceValue==1)
					 {
//					  Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //机器故障，请联系售后维修
//					  Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptMachineErrorFlag=1;//机器故障，请联系售后维修
					  
					  ErrorVar.xpErrorPlayFlag=1;
					  ErrorVar.bBldcErrFlag=1;
					  if(BMStoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BMStoMData.xpErrorStatus;
					  if(BLDCtoMData.xpErrorStatus>0)MtoLCDData.xpErrorStatus=BLDCtoMData.xpErrorStatus;
					  if(BrushVar.bBrushShortErrFlag||PumpVar.bPumpShortErrFlag)MtoLCDData.xpErrorStatus=BRUSH_SHORT_ERROR;
					 }
				  if(Voice.ucPlayVoiceValue==200)
				   {
					  ErrorVar.xpErrorPlayStep=STEP1;
					  Voice.ucPlayVoiceValue=0;
				   }
				}
			else
			   {
			   ErrorVar.xpErrorPlayStep=STEP1;
			   Voice.ucPlayVoiceValue=0;
			   }
		   break;
		case STEP1:
			if(BMStoMData.xpErrorStatus==BMS_OT_ERROR)
				{
				 if(Voice.ucPlayVoiceValue==1)
					 {
//					  Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //电池温度过高，请等待降温后使用
//					  Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptBatTempOverFlag=1;//电池温度过高，请等待降温后使用

					  ErrorVar.xpErrorPlayFlag=1;
					  ErrorVar.bBmsErrFlag=1;
					  MtoLCDData.xpErrorStatus=BMS_OT_ERROR;
					 }
				 if(Voice.ucPlayVoiceValue==200)
				  {
					 ErrorVar.xpErrorPlayStep=STEP2;
					 Voice.ucPlayVoiceValue=0;
				  }
				}
			else
			   {
				ErrorVar.xpErrorPlayStep=STEP2;
				Voice.ucPlayVoiceValue=0;
			   }
		   break;
		case STEP2:
			if(WaterBSta.ucQBucketStatus == QBULET_BLANK_STA) 
			 {
				if(Voice.ucPlayVoiceValue==1)
					{
//					 Voice.uiTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;  //清水箱缺水，请及时加水
//					 Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptCboxNoWaterFlag=1;//清水箱缺水，请及时加水
					 
					  ErrorVar.xpErrorPlayFlag=1;
					  ErrorVar.bRunErrFlag=1;
					  MtoLCDData.xpQWaterBoxStatus=QBULET_BLANK_STA;
					 }
				if(Voice.ucPlayVoiceValue==200)
				 {
					ErrorVar.xpErrorPlayStep=STEP3;
					Voice.ucPlayVoiceValue=0;
				 }
			 }
			else
			   {
				MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;
				ErrorVar.xpErrorPlayStep=STEP3;
				Voice.ucPlayVoiceValue=0;
			   }
		   break;
		case STEP3:
			if(WaterBSta.ucWBucketStatus == WBULET_FULL_STA) 
			 {
				if(Voice.ucPlayVoiceValue==1)
					{
//					Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //污水箱已满，请及时清理
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptSewageBoxFullFlag=1;//污水箱已满，请及时清理
					 
					 ErrorVar.xpErrorPlayFlag=1;
					 ErrorVar.bRunErrFlag=1;
					 MtoLCDData.xpWWaterBoxStatus=0x02;
					}
				if(Voice.ucPlayVoiceValue==200)
				  {
					ErrorVar.xpErrorPlayStep=STEP4;
					Voice.ucPlayVoiceValue=0;
				  }
			 }
			else if(WaterBSta.ucWBucketStatus == WBULET_BLANK_STA)
			 {
				if(Voice.ucPlayVoiceValue==1)
					{
//					Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //污水箱未安装，请安装污水箱
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoSewageBoxFlag=1;//污水箱未安装，请安装污水箱

					 ErrorVar.xpErrorPlayFlag=1;
					 ErrorVar.bRunErrFlag=1;
					 MtoLCDData.xpWWaterBoxStatus=0x02;
					}
				if(Voice.ucPlayVoiceValue==200)
				  {
					ErrorVar.xpErrorPlayStep=STEP4;
					Voice.ucPlayVoiceValue=0;
				  }
			 }
			else
			   {
				ErrorVar.xpErrorPlayStep=STEP4;
				Voice.ucPlayVoiceValue=0;
				MtoLCDData.xpWWaterBoxStatus=0x01;
			   }
		   break;
		case STEP4:
			if(BrushVar.bBrushNGErrFlag) 
				{ 
				if(Voice.ucPlayVoiceValue==1)
					{
//					Voice.uiTonePromptValue = PROMPT_NO_BRUSH_MP3; //滚刷未安装，请安装滚刷 
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoBrushFlag=1;//滚刷未安装，请安装滚刷 

					 ErrorVar.xpErrorPlayFlag=1;
					 ErrorVar.bRunErrFlag=1;
					 MtoLCDData.xpBrushStatus=BRUSH_NO_STA;
					}
				if(Voice.ucPlayVoiceValue==200)
				  {
					ErrorVar.xpErrorPlayStep=STEP5;
					Voice.ucPlayVoiceValue=0;
				  }
				}
			else
			   {
				ErrorVar.xpErrorPlayStep=STEP5;
				Voice.ucPlayVoiceValue=0;
			   }
		   break;
	   case STEP5:
		   if((BLDCtoMData.xpErrorStatus==SMD_BLDC_ERROR)||BrushVar.bBrushStallErrFlag)
			   {
			   if(Voice.ucPlayVoiceValue==1)
				   {
//				   Voice.uiTonePromptValue = PROMPT_CHECK_BRUSH_TUBE_MP3;//请检查滚刷或底部管道
//				   Voice.uiPreTonePromptValue = 0;
					VoiceListpara.VoicePromptCheckBrushTubeFlag=1; //请检查滚刷或底部管道
				   
					ErrorVar.xpErrorPlayFlag=1;
					ErrorVar.bRunErrFlag=1;
					if(BrushVar.bBrushStallErrFlag)MtoLCDData.xpBrushStatus=BRUSH_STALL_STA;
				   }
			   if(Voice.ucPlayVoiceValue==200)
				 {
				  ErrorVar.xpErrorPlayStep=STEP6;
				  Voice.ucPlayVoiceValue=0;
				 }
			   }
		   else
			   {
			   if(MtoLCDData.xpBrushStatus!=BRUSH_NO_STA)MtoLCDData.xpBrushStatus=BRUSH_YES_STA;
			   ErrorVar.xpErrorPlayStep=STEP6;
			   Voice.ucPlayVoiceValue=0;
			   }
		   break;
	   case STEP6:
		   if(ErrorVar.xpErrorPlayFlag==1)
			   {
			   if(++ErrorVar.xpErrorPlayCnt>=ERROR_PLAY_CNT)
				  { 
				   ErrorVar.xpErrorPlayCnt=0;
				   WorkVar.ucWorkOrderStatus=CHARGE_STA;
				   WorkVar.ucWorkStatus=CHARGE_STA;
				  }
				ACleanVar.bAutoCleanStartFlag=0;
				ACleanVar.bAutoCleanOverFlag=0;
	 		 	ACleanVar.bAutoCleanFlag=0;
				ACleanVar.bAutoDeepCleanFlag=0;
				WaterBSta.ucQBucketStatus =QBULET_FULL_STA;
				ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
				if(ErrorVar.bBldcErrFlag||ErrorVar.bBmsErrFlag)
					{
					ErrorVar.xpErrorRecoverFlag=1;
					}
				else
					{
					ErrorVar.xpErrorRecoverFlag=0;
					}
				ErrorVar.bRunErrFlag=0;
				ErrorVar.bBldcErrFlag=0;
				ErrorVar.bBmsErrFlag=0;
				ErrorVar.xpErrorPlayFlag=0;
				BrushVar.bBrushNGErrFlag=0;
				BrushVar.bBrushStallErrFlag=0;
			   }
		   WorkVar.ucbackWorkOrderStatus=WorkVar.ucWorkOrderStatus;
		   ErrorVar.xpErrorPlayStep=STEP0;
		   Voice.ucPlayVoiceValue=0;
		   break;
	   default:
		   break;
	   }
}

void VoicePlay_LcdDisplay_ChangeWorkStatus(void)
{// 工作状态改变语音播报
	if(++WorkVar.ucSynchroCommCnt>2)
		{
		WorkVar.ucSynchroCommCnt=0;
		switch(WorkVar.ucWorkStatus){
		   case OPEN_STA:
		   	if(WorkVar.ucbackWorkStatus==AUTO_STA)
		   		{
				ACleanVar.xpAutoCleanCount	  = 0;
				ACleanVar.xpAutoCleanStep	  = STEP0;	
				ACleanVar.xpAutoCleanStatus   = NON_AUTO_CLEAN_MODE;	
				ACleanVar.bAutoCleanStartFlag = 0;
				ACleanVar.bAutoCleanOverFlag  = 0;	
				ACleanVar.bAutoCleanFlag	  = 0;
				ACleanVar.bAutoDeepCleanFlag  = 0;
		   		}
			   KeyVar.ucKeyGearValue=0;
			   KeyVar.bFastVoiceFlag=1;
			   VoiceListpara.VoicePromptAutoModeFlag=1; // 自动模式
			   break;
		   case CLOSE_STA:
			   if(WorkVar.ucbackWorkStatus==AUTO_STA)
				   {
				   ACleanVar.xpAutoCleanCount	 = 0;
				   ACleanVar.xpAutoCleanStep	 = STEP0;  
				   ACleanVar.xpAutoCleanStatus	 = NON_AUTO_CLEAN_MODE;    
				   ACleanVar.bAutoCleanStartFlag = 0;
				   ACleanVar.bAutoCleanOverFlag  = 0;  
				   ACleanVar.bAutoCleanFlag 	 = 0;
				   ACleanVar.bAutoDeepCleanFlag  = 0;
				   }
			   if((WorkVar.ucbackWorkStatus!=CHARGE_STA)&&(WorkVar.ucbackWorkStatus!=AUTO_STA))
				  {
				   VoiceListpara.VoicePromptPowerOffFlag=1; // 已关机
				   KeyVar.bFastVoiceFlag=1;
				  }
			   break;
		   case CHARGE_STA:
			if(WorkVar.ucbackWorkStatus!=AUTO_STA)
				{
				VoiceListpara.VoicePromptStartChargeFlag=1;// 开始充电 	
				}
			   break;
		   case AUTO_STA:			  
			   break;
		   default:
			   break;
		   }
		ErrorVar.xpErrorPlayStep=STEP0;
		Voice.ucPlayVoiceValue=0;
		ErrorVar.xpErrorPlayFlag=0;
		WorkVar.ucWorkOrderStatus=WorkVar.ucWorkStatus;
		WorkVar.ucbackWorkStatus=WorkVar.ucWorkStatus;
		}
}

void VoicePlay_LcdDisplay_WorkStatus(void)
{
	if((WorkVar.ucWorkStatus!=WorkVar.ucbackWorkStatus)&&(BMStoMData.xpWorkStatus==WorkVar.ucWorkStatus))
	  {// 工作状态切换时播放语音
		VoicePlay_LcdDisplay_ChangeWorkStatus();
	  }
	else if((WorkVar.ucWorkStatus==WorkVar.ucbackWorkStatus)&&(BMStoMData.xpWorkStatus==WorkVar.ucWorkStatus))
		{
		WorkVar.ucSynchroCommCnt=0;
		if(Voice.ucPlayVoiceValue<200)Voice.ucPlayVoiceValue++;
			switch(WorkVar.ucWorkOrderStatus){
			   case OPEN_STA:
			   	   VoicePlay_LcdDisplay_OPEN_BMS();
				   break;
			   case CLOSE_STA:
				   break;
			   case IDLE_STA:
		           VoicePlay_LcdDisplay_IDLE_STA();
				   break;
			   case CHARGE_STA:
		           VoicePlay_LcdDisplay_CHARGE_STA();
				   break;
			   case AUTO_STA:
				   VoicePlay_LcdDisplay_AUTO_STA();
				   break;
			   default:
				   break;
			   }
		}
	else
		{
		WorkVar.ucSynchroCommCnt=0;
		}
}

void Check_WorkStatus(void)
{
  VoicePlay_LcdDisplay_WorkStatus();
  switch(WorkVar.ucWorkOrderStatus)
  	{
  	 case OPEN_STA:
	 	break;
	 case CLOSE_STA:
	 	BrushVar.bBrushNGErrFlag=0;
		BrushVar.bBrushStallErrFlag=0;
		BrushVar.bBrushShortErrFlag=0;
		PumpVar.bPumpShortErrFlag=0;
		break;
	 case IDLE_STA:
		break;
	 case CHARGE_STA:
	 	if(ACleanVar.bAutoCleanFlag||ACleanVar.bAutoDeepCleanFlag||(ACleanVar.xpAutoCleanStatus==WAIT_AUTO_CLEAN_MODE))
	 		{
	 		 if(BMStoMData.xpBatSocVal<SOC_MIN_VALUE)
	 		 	{
//				Voice.uiTonePromptValue = PROMPT_LOW_POWER_CLEAN_MP3;	 // 电量低，请充电后自清洁															//电量低,请充电后自清洁
//				Voice.uiPreTonePromptValue = 0;
				VoiceListpara.VoicePromptLowPowerCleanFlag=1; // 电量低，请充电后自清洁	

	 		 	ACleanVar.bAutoCleanFlag=0;
				ACleanVar.bAutoDeepCleanFlag=0;
				ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
	 		 	}
			 else if((BrushVar.bBrushNGErrFlag==0)&&(BrushVar.bBrushStallErrFlag==0))
			 	{
			 	 if(WaterBSta.ucWBucketStatus==WBULET_BLANK_STA)
			 	 	{
//					 Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //污水箱未安装，请安装污水箱
//					 Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoSewageBoxFlag=1;//污水箱未安装，请安装污水箱

					 ACleanVar.bAutoCleanFlag=0;
					 ACleanVar.bAutoDeepCleanFlag=0;
					 ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
			 	 	}
				 else if(WaterBSta.ucWBucketStatus==WBULET_FULL_STA)
				 	{
//					 Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //污水箱已满，请及时清理
//					 Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptSewageBoxFullFlag=1; //污水箱已满，请及时清理

					 ACleanVar.bAutoCleanFlag=0;
					 ACleanVar.bAutoDeepCleanFlag=0;
					 ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
				 	}
				 else if(CHGVar.bChargingFlag==1)
				 	{
				     WaterBSta.ucQBucketStatus=QBULET_FULL_STA;
			 	     WorkVar.ucWorkOrderStatus=AUTO_STA;
			 	     WorkVar.ucWorkStatus=AUTO_STA;
				     ACleanVar.xpAutoCleanStatus=AUTO_CLEAN_MODE;
				 	}
			 	}
			 else
			 	{
				 ACleanVar.bAutoCleanFlag=0;
				 ACleanVar.bAutoDeepCleanFlag=0;
				 ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
			 	}
	 		}
	    break;
	 case AUTO_STA:
		WorkVar.ucWorkOrderStatus=AUTO_STA;
		WorkVar.ucWorkStatus=AUTO_STA;
		break;
	 default:
		break;
  	}
}



//****************负载检测控制处理函数*******************
///*功能：2ms执行一次	控制主电机的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************
void ControlAndCheck_Load(void)
{// 2ms

 switch(TimeVar.bTmrRun02msCount){
    case STEP0:// 10ms
		Check_Tilt(&TiltVar);                               // 倾倒开关检测
		Check_Charge(&CHGVar);                              // 充电检测
		break;
	case STEP1:// 10ms
		Deal_WorkProcess();                                 // 各种工作模式 负载控制处理函数
		break;
	case STEP2:// 10ms
		Elimination_BatVolt();                              // 地刷水泵占空比处理函数 (根据电池电量 )
		Control_MainMotor(&BLDCVar,&MtoBLDCData);           // 主电机控制函数	
		Control_Brush(&BrushVar,&ADCVar);                   // 地刷控制函数	
		Control_Dry(&DryVar);                               // 烘干控制函数	
		Control_Pump(&PumpVar,&WaterBSta,&BrushVar);        // 水泵控制函数	
		Control_WaterElec(&WaterElecVar);                   // 电解水控制函数	
		break;
	case STEP3:// 10ms
		Check_SewageStage0(&WaterBSta,&SewageVar,&ADCVar);  // 污水箱水位检测
		Check_SewageLV(&ADCVar,&TimeVar,&SewageVar);        // 脏污检测
		Check_WaterBox(&WaterBSta);                         // 清水箱有水检测
		break;
	case STEP4:// 10ms
		break;
    default:
		break;
 	}
 if(++TimeVar.bTmrRun02msCount>4)TimeVar.bTmrRun02msCount=0;
}





