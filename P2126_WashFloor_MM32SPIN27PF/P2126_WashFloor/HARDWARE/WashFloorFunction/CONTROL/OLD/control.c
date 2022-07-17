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
//****************��ˢˮ��ռ�ձȴ�����*******************
///*���ܣ�10msִ��һ��	�ֱ����ˮ��/��ˢ��������ֹͣ			 						*///
///*��ڲ�����	��																				*///
///*���ڲ�����	��																				*///
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
				   {// �Զ�ģʽ
					 Set_Bldc_Low(&BLDCVar);
					 Set_Brush_Normal(&BrushVar);
					 Set_WaterPump_ON(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
				   }
				 else if(KeyVar.ucKeyGearValue==1)
				   {// ��ˮģʽ
					 Set_Bldc_Strong(&BLDCVar);
					 Set_Brush_Normal(&BrushVar);
					 Set_WaterPump_OFF(&PumpVar);
					 Set_Dry_OFF(&DryVar);
					 Set_WaterElec_OFF(&WaterElecVar);
				   }
				 else //ɱ��ģʽ
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
		  {//ɱ��ģʽ
		    if((WaterElecVar.bEnalbeWaterElecFlag==0)&&(WaterElecVar.waterelec_OK_flag==0))
		    	{
				Voice.uiTonePromptValue = PROMPT_DISINFECT_MODE_MP3; //ɱ��ģʽ
				Voice.uiSecondTonePromptValue = PROMPT_CHECK_FULL_CLEAR_BOX_MP3; //����ˮ�����У���ȷ����ˮ����ˮ
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
				case STEP0:// ����ˮ����
		            if(ACleanVar.bAutoDeepCleanFlag==1)
						{
							if(ACleanVar.xpAutoCleanCount<AUTO_STEP0_TIME)
							{		
								if(ACleanVar.xpAutoCleanCount==1){					 
//									Voice.uiTonePromptValue = PROMPT_START_CLEAN_MP3;
//									Voice.uiSecondTonePromptValue = PROMPT_SOLUTION_PRODECT_MP3;	 
//									Voice.uiPreTonePromptValue = 0;

									VoiceListpara.VoicePromptStartCleanSolutionProdectFlag=1;//��ʼ�����,����ˮ������
									
									ACleanVar.bAutoCleanStartFlag = 1;//��ʼ�����
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
				case STEP1:// ��ˢ���
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP1_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==1)
						 	{
						 	 if(ACleanVar.bAutoDeepCleanFlag==0)
						 	 	{
//						         Voice.uiTonePromptValue = PROMPT_START_CLEAN_MP3;
//								 Voice.uiSecondTonePromptValue = PROMPT_BRUSH_CLEANING_MP3; 		
//								 Voice.uiPreTonePromptValue = 0;

								 VoiceListpara.VoicePromptStartCleanBrushCleaningFlag=1;//��ʼ�����,��ˢ�����
								 ACleanVar.bAutoCleanStartFlag = 1;//��ʼ�����
								 ACleanVar.bAutoCleanOverFlag= 0;
						 	 	}
							 else
							 	{
//								 Voice.uiTonePromptValue = PROMPT_BRUSH_CLEANING_MP3;
//								 Voice.uiPreTonePromptValue = 0;
								VoiceListpara.VoicePromptBrushCleaningFlag=1;//��ˢ�����
							 	}
						 	}
						 PumpVar.ucPumpGearValue=3;//ˮ������ֵ
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
				case STEP2:// �ܵ������
					if(ACleanVar.xpAutoCleanCount<AUTO_STEP2_TIME)
						{	
						    if(ACleanVar.xpAutoCleanCount==1)
						    	{
//								Voice.uiTonePromptValue = PROMPT_TUBE_CLEANING_MP3;
//								Voice.uiPreTonePromptValue = 0;

								VoiceListpara.VoicePromptTubeCleaningFlag=1;//�ܵ������
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
		         case STEP3:// ���ܼ��
					 if(ACleanVar.xpAutoCleanCount < AUTO_STEP3_TIME)
					 {				 
					     if(ACleanVar.xpAutoCleanCount==1)
						     {
//						      Voice.uiTonePromptValue = PROMPT_INTELLIGENT_DET_MP3;
//						      Voice.uiPreTonePromptValue = 0;
							 VoiceListpara.VoicePromptIntelligentDetFlag=1;//���ܼ����
						     }
						 PumpVar.ucPumpGearValue = 3;//ˮ������ֵ
						 Set_Bldc_OFF(&BLDCVar);
						 Set_Brush_Normal(&BrushVar);
						 Set_WaterPump_ON(&PumpVar);
						 Set_Dry_OFF(&DryVar);
						 Set_WaterElec_OFF(&WaterElecVar);
					 }
					 else
					 {
						 ACleanVar.xpAutoCleanCount = 0;					 
						 if(ADCVar.uiSewageLVAdcValue>SewageVar.xpSewageDefaultRefValue)ACleanVar.xpAutoCleanStep=STEP5;  //����Ⱦ  //ִ�е��岽
						 else ACleanVar.xpAutoCleanStep=STEP4;								 //��Ⱦ�Ƚϴ�ִ�е��Ĳ�
					 }	 
		            break;
				case STEP4:// �����ǿ
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP4_TIME)
						{
						 if(ACleanVar.xpAutoCleanCount==1)
						    {
//							  Voice.uiTonePromptValue = PROMPT_DEEP_CLEANING_MP3;
//						      Voice.uiPreTonePromptValue = 0;
							VoiceListpara.VoicePromptDeepCleaningFlag=1;//��������
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
				case STEP5:// �������
					if(ACleanVar.xpAutoCleanCount < AUTO_STEP5_TIME)				//����30s
					    { 
						if(ACleanVar.xpAutoCleanCount==1)
						   {
//							 Voice.uiTonePromptValue = PROMPT_DEEP_CLEANING_MP3;
//							 Voice.uiPreTonePromptValue = 0;
						     VoiceListpara.VoicePromptDeepCleaningFlag=1;//��������
						   }
						if(ACleanVar.xpAutoCleanCount < AUTO_PUMP5_TIME)			//ǰ5s
						   {
							PumpVar.ucPumpGearValue = 3;				//ˮ������ֵ
							Set_WaterPump_ON(&PumpVar);
						   }
						else																				
						   {
							PumpVar.ucPumpGearValue = 0;			//ˮ������ֵ=0;
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
				case STEP6:// ���
					if(ACleanVar.xpAutoCleanCount< AUTO_STEP6_TIME)				//����1h
					   { 
						if(ACleanVar.xpAutoCleanCount==1)
							{
//							 Voice.uiTonePromptValue = PROMPT_BRUSH_DRY_MP3;
//							 Voice.uiPreTonePromptValue = 0;

						     VoiceListpara.VoicePromptBrushDryFlag=1;//��ˢ�����
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
			    case STEP7:// ��������
					if(ACleanVar.bAutoDeepCleanFlag==1)
						{
						if(ACleanVar.xpAutoCleanStatus == AUTO_CLEAN_MODE) 
							{
//							Voice.uiTonePromptValue = PROMPT_BRUSH_DRY_OVER_MP3;
//							Voice.uiSecondTonePromptValue = PROMPT_CLEAN_OVER_MP3;
//							Voice.uiPreTonePromptValue = 0;
							
							VoiceListpara.VoicePromptBrushDryOverCleanOverFlag=1;//������ ��������
							
							}
						}
					else
						{
						if(ACleanVar.xpAutoCleanStatus == AUTO_CLEAN_MODE) 
							{
//							 Voice.uiTonePromptValue = PROMPT_CLEAN_OVER_MP3;
//							 Voice.uiPreTonePromptValue = 0;
							VoiceListpara.VoicePromptCleanOverFlag=1;//��������
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
		PumpVar.ucPumpGearValue 	  = 0;					//ˮ������ֵ
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
				 {// ������ ��ذ����� ��������� ��·����
				   if(Voice.ucPlayVoiceValue==1)
					  {
//					 Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //�������ϣ�����ϵ�ۺ�ά��
//					 Voice.uiPreTonePromptValue = 0;
				       VoiceListpara.VoicePromptMachineErrorFlag=1;//�������ϣ�����ϵ�ۺ�ά��

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
					   Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //����¶ȹ��ߣ���ȴ����º�ʹ��
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
					  Voice.uiTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;  //��ˮ��ȱˮ���뼰ʱ��ˮ
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
					 Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //��ˮ���������뼰ʱ����
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
					 Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //��ˮ��δ��װ���밲װ��ˮ��
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
					 Voice.uiTonePromptValue = PROMPT_NO_BRUSH_MP3; //��ˢδ��װ���밲װ��ˢ 
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
					Voice.uiTonePromptValue = PROMPT_CHECK_BRUSH_TUBE_MP3;//�����ˢ��ײ��ܵ�
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
					Voice.uiTonePromptValue=PROMPT_POWER_OVER_MP3;//�����Ѻľ�
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
					 Voice.uiTonePromptValue=PROMPT_LOW_POWER_CHARGE_MP3;//�����ͣ��뼰ʱ���
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
					 Voice.uiTonePromptValue=PROMPT_LOW_POWER_MP3;//��ص�����
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
					{// ������ ��ذ����� ��������� ��·����
					  if(Voice.ucPlayVoiceValue==1)
						 {
						  Voice.uiTonePromptValue    = PROMPT_MACHINE_ERROR_MP3; //�������ϣ�����ϵ�ۺ�ά��
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
//						  Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //����¶ȹ��ߣ���ȴ����º�ʹ��
//						  Voice.uiPreTonePromptValue = 0;
						  VoiceListpara.VoicePromptBatTempOverFlag=1;//����¶ȹ��ߣ���ȴ����º�ʹ��

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
//					   Voice.uiTonePromptValue=PROMPT_POWER_OVER_MP3;//�����Ѻľ�
//					   Voice.uiPreTonePromptValue = 0;	   
					   VoiceListpara.VoicePromptPowerOverFlag=1;//�����Ѻľ�

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
//						Voice.uiTonePromptValue=PROMPT_LOW_POWER_CHARGE_MP3;//�����ͣ��뼰ʱ���
//						Voice.uiPreTonePromptValue = 0; 
                         VoiceListpara.VoicePromptLowPowerChargeFlag=1;//�����ͣ��뼰ʱ���
						
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
//						Voice.uiTonePromptValue=PROMPT_LOW_POWER_MP3;//��ص�����
//						Voice.uiPreTonePromptValue = 0; 	
						VoiceListpara.VoicePromptLowPowerFlag=1;//��ص�����
						
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
				{// ������ ��ذ����� ��������� 
				  if(Voice.ucPlayVoiceValue==1)
					 {
//					Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //�������ϣ�����ϵ�ۺ�ά��
//					Voice.uiPreTonePromptValue = 0;
				 	  VoiceListpara.VoicePromptMachineErrorFlag=1;//�������ϣ�����ϵ�ۺ�ά��
					 						
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
// 				   Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //����¶ȹ��ߣ���ȴ����º�ʹ��
// 				   Voice.uiPreTonePromptValue = 0;
				      VoiceListpara.VoicePromptBatTempOverFlag=1;//����¶ȹ��ߣ���ȴ����º�ʹ��
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
				{// ������ ��ذ����� ��������� ��·����
				  if(Voice.ucPlayVoiceValue==1)
					 {
//					  Voice.uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3; //�������ϣ�����ϵ�ۺ�ά��
//					  Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptMachineErrorFlag=1;//�������ϣ�����ϵ�ۺ�ά��
					  
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
//					  Voice.uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3; //����¶ȹ��ߣ���ȴ����º�ʹ��
//					  Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptBatTempOverFlag=1;//����¶ȹ��ߣ���ȴ����º�ʹ��

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
//					 Voice.uiTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;  //��ˮ��ȱˮ���뼰ʱ��ˮ
//					 Voice.uiPreTonePromptValue = 0;
					  VoiceListpara.VoicePromptCboxNoWaterFlag=1;//��ˮ��ȱˮ���뼰ʱ��ˮ
					 
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
//					Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //��ˮ���������뼰ʱ����
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptSewageBoxFullFlag=1;//��ˮ���������뼰ʱ����
					 
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
//					Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //��ˮ��δ��װ���밲װ��ˮ��
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoSewageBoxFlag=1;//��ˮ��δ��װ���밲װ��ˮ��

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
//					Voice.uiTonePromptValue = PROMPT_NO_BRUSH_MP3; //��ˢδ��װ���밲װ��ˢ 
//					Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoBrushFlag=1;//��ˢδ��װ���밲װ��ˢ 

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
//				   Voice.uiTonePromptValue = PROMPT_CHECK_BRUSH_TUBE_MP3;//�����ˢ��ײ��ܵ�
//				   Voice.uiPreTonePromptValue = 0;
					VoiceListpara.VoicePromptCheckBrushTubeFlag=1; //�����ˢ��ײ��ܵ�
				   
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
{// ����״̬�ı���������
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
			   VoiceListpara.VoicePromptAutoModeFlag=1; // �Զ�ģʽ
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
				   VoiceListpara.VoicePromptPowerOffFlag=1; // �ѹػ�
				   KeyVar.bFastVoiceFlag=1;
				  }
			   break;
		   case CHARGE_STA:
			if(WorkVar.ucbackWorkStatus!=AUTO_STA)
				{
				VoiceListpara.VoicePromptStartChargeFlag=1;// ��ʼ��� 	
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
	  {// ����״̬�л�ʱ��������
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
//				Voice.uiTonePromptValue = PROMPT_LOW_POWER_CLEAN_MP3;	 // �����ͣ�����������															//������,����������
//				Voice.uiPreTonePromptValue = 0;
				VoiceListpara.VoicePromptLowPowerCleanFlag=1; // �����ͣ�����������	

	 		 	ACleanVar.bAutoCleanFlag=0;
				ACleanVar.bAutoDeepCleanFlag=0;
				ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
	 		 	}
			 else if((BrushVar.bBrushNGErrFlag==0)&&(BrushVar.bBrushStallErrFlag==0))
			 	{
			 	 if(WaterBSta.ucWBucketStatus==WBULET_BLANK_STA)
			 	 	{
//					 Voice.uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3; //��ˮ��δ��װ���밲װ��ˮ��
//					 Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptNoSewageBoxFlag=1;//��ˮ��δ��װ���밲װ��ˮ��

					 ACleanVar.bAutoCleanFlag=0;
					 ACleanVar.bAutoDeepCleanFlag=0;
					 ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
			 	 	}
				 else if(WaterBSta.ucWBucketStatus==WBULET_FULL_STA)
				 	{
//					 Voice.uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3; //��ˮ���������뼰ʱ����
//					 Voice.uiPreTonePromptValue = 0;
					 VoiceListpara.VoicePromptSewageBoxFullFlag=1; //��ˮ���������뼰ʱ����

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



//****************���ؼ����ƴ�����*******************
///*���ܣ�2msִ��һ��	�����������������ֹͣ			 						*///
///*��ڲ�����	��																				*///
///*���ڲ�����	��																				*///
//************************************************************
void ControlAndCheck_Load(void)
{// 2ms

 switch(TimeVar.bTmrRun02msCount){
    case STEP0:// 10ms
		Check_Tilt(&TiltVar);                               // �㵹���ؼ��
		Check_Charge(&CHGVar);                              // �����
		break;
	case STEP1:// 10ms
		Deal_WorkProcess();                                 // ���ֹ���ģʽ ���ؿ��ƴ�����
		break;
	case STEP2:// 10ms
		Elimination_BatVolt();                              // ��ˢˮ��ռ�ձȴ����� (���ݵ�ص��� )
		Control_MainMotor(&BLDCVar,&MtoBLDCData);           // ��������ƺ���	
		Control_Brush(&BrushVar,&ADCVar);                   // ��ˢ���ƺ���	
		Control_Dry(&DryVar);                               // ��ɿ��ƺ���	
		Control_Pump(&PumpVar,&WaterBSta,&BrushVar);        // ˮ�ÿ��ƺ���	
		Control_WaterElec(&WaterElecVar);                   // ���ˮ���ƺ���	
		break;
	case STEP3:// 10ms
		Check_SewageStage0(&WaterBSta,&SewageVar,&ADCVar);  // ��ˮ��ˮλ���
		Check_SewageLV(&ADCVar,&TimeVar,&SewageVar);        // ���ۼ��
		Check_WaterBox(&WaterBSta);                         // ��ˮ����ˮ���
		break;
	case STEP4:// 10ms
		break;
    default:
		break;
 	}
 if(++TimeVar.bTmrRun02msCount>4)TimeVar.bTmrRun02msCount=0;
}





