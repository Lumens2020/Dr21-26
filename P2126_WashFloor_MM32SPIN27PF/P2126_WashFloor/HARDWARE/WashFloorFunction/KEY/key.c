#include "key.h"
#include "keyio.h"
#include "keyad.h"
#include "gpio.h"
#include "init.h"
#include "tilt.h"
#include "charge.h"

#include "voice.h"
#include "voicelist.h"

#include "control.h"
#include "eeprom.h"
#include "commbms.h"
#include "commfct.h"
#include "commbldc.h"
#include "commscreen.h"
#include "brush.h"
#include "cleanwater.h"
#include "waterelec.h"

#include <string.h>


ADButton ADbutton1;
ADButton ADbutton2;
ADButton ADbutton3;

Button button1;


extern VoicePara      Voice;
extern WorkPara       WorkVar;
extern KeyPara        KeyVar;
extern TILTPara       TiltVar;
extern CHGPara        CHGVar;
extern AutoCleanValue ACleanVar;
extern WorkError      ErrorVar;

extern VoiceList      VoiceListpara;

extern Eeprom_TypeDef E2prom;


extern MCUtoBMSData   MtoBMSData;
extern BrushPara      BrushVar;
extern BLDCtoMCUData  BLDCtoMData;
extern WaterBoxStatus WaterBSta;
extern BMStoMCUData   BMStoMData;
extern MCUtoLCDData   MtoLCDData;

extern MCUtoFCTData   MtoFCTData;
extern FCT_UART1Comm  FctUART1COMMpara;
extern WaterElecPara  WaterElecVar;

//****************����������������*******************
///*���ܣ�      ��������������	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void KEY_VOICE_PRESS_UP(void)
{
	MtoBMSData.xpKeyPushStatus=1;
	MtoFCTData.xpVoiceKeyStatus=1;// 20211214
	KeyVar.ucKeyWakeUpcnt=0;
	KeyVar.ucKeyWakeUpcnt1=0;
	if(ErrorVar.xpBmsSleepErrorFlag==1)// 20220105
		{
		KeyVar.bFastVoiceFlag= 1;
		Voice.ucSendVoiceOVcnt=0;
		VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���	
		MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
		}
	else
		{
		WorkVar.ucCloseScreencnt=0;
		WorkVar.ucCloseMachinecnt=0;
        if(MtoLCDData.xpLCDStatus==CLOSE_LCD&&WorkVar.ucWorkOrderStatus!=CLOSE_STA)// 20220111
        	{
        	}
		else
			{
		    if(ErrorVar.xpServiceErrorFlag==0||FctUART1COMMpara.bTestModeFlag)//20220104
		//		if(ErrorVar.xpServiceErrorFlag==0)//20210910
		    	{
			   if(Voice.ucLongKeyMenuValue>0)
			   	{// ��������
				   if(Voice.ucLanguageCount_back<0x01) Voice.ucLanguageCount_back++;
				   else Voice.ucLanguageCount_back = 0;			   
				   if(Voice.ucLanguageCount_back==0)VoiceListpara.VoicePromptChineseFlag=1;
				   else if(Voice.ucLanguageCount_back==1)VoiceListpara.VoicePromptEnglishFlag=1;
				   else if(Voice.ucLanguageCount_back==2)VoiceListpara.VoicePromptGermanyFlag=1;
				   else if(Voice.ucLanguageCount_back==3)VoiceListpara.VoicePromptFranceFlag=1;
				   else if(Voice.ucLanguageCount_back==4)VoiceListpara.VoicePromptEspanishFlag=1;
				   else if(Voice.ucLanguageCount_back==5)VoiceListpara.VoicePromptItaliaFlag=1;
				   else if(Voice.ucLanguageCount_back==6)VoiceListpara.VoicePromptRussionFlag=1;
				   else if(Voice.ucLanguageCount_back==7)VoiceListpara.VoicePromptJapaneseFlag=1;
				   else if(Voice.ucLanguageCount_back==8)VoiceListpara.VoicePromptKoreaFlag=1;
				   else if(Voice.ucLanguageCount_back==9)VoiceListpara.VoicePromptArabicFlag=1;
				   else if(Voice.ucLanguageCount_back==10)VoiceListpara.VoicePromptHebrewFlag=1;
			   	}
			   else
			   	{// ��������
				   if(Voice.ucFunctionValue<3)Voice.ucFunctionValue++;
				   else Voice.ucFunctionValue = 0;							   
				   if(Voice.ucFunctionValue==0)VoiceListpara.VoicePromptVolumeHighFlag=1; //������
				   else if(Voice.ucFunctionValue==1)VoiceListpara.VoicePromptCloseVolumeFlag=1; //�ر�����
				   else if(Voice.ucFunctionValue==2)VoiceListpara.VoicePromptOpenVolumeFlag=1; //��������
				   else if(Voice.ucFunctionValue==3)VoiceListpara.VoicePromptVolumeLowFlag=1; //Voice.uiTonePromptValue = PROMPT_VOLUME_LOW_MP3;//������  
		           
				   if(Voice.ucFunctionValue==3)
					   {
					   Voice.uiVolumePromptValue=VLOUME_LOW_VALUE;
					   Voice.uiPreVolumePromptValue=0;
					   Voice.bVolumeChangeFlag=1;
					   Voice.bVolumePlayChangeFlag=1;// 20220113
					   }
				   else if(Voice.ucFunctionValue==0)
					   {
					   Voice.uiVolumePromptValue=VLOUME_HIGH_VALUE;//VOLUME_ADJUSTMENT_7;
					   Voice.uiPreVolumePromptValue=0;
					   Voice.bVolumeChangeFlag=1;
					   Voice.bVolumePlayChangeFlag=1;// 20220113
					   }
				   else 
					   {
//					   Voice.bVolumeChangeFlag=0;
					   Voice.bVolumePlayChangeFlag=1;// 20220113
					   }
			   }
			   
			   E2prom.uiWriteDataFlash[EE_VOICEFUN]	= Voice.ucFunctionValue; 
			   E2prom.bEnableEEpromFlag =1;
			   KeyVar.bFastVoiceFlag = 1;
			   Voice.ucSendVoiceOVcnt=0;
		    	}
        	}
		}
}


//****************������������������*******************
///*���ܣ�      ����������������	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************


void KEY_VOICE_LONG_PRESS(void)
{
	MtoBMSData.xpKeyPushStatus=1;
	KeyVar.ucKeyWakeUpcnt=0;
	if(ErrorVar.xpBmsSleepErrorFlag==1)// 20220105
		{
		KeyVar.bFastVoiceFlag		= 1;
		Voice.ucSendVoiceOVcnt=0;
		VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���	
		MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
		}
	else
		{
		WorkVar.ucCloseScreencnt=0;
		WorkVar.ucCloseMachinecnt=0;
	//    if(ErrorVar.xpServiceErrorFlag==0)//20210910
	if(MtoLCDData.xpLCDStatus==CLOSE_LCD&&WorkVar.ucWorkOrderStatus!=CLOSE_STA)// 20220111
		{
		}
	else
		{
		if(ErrorVar.xpServiceErrorFlag==0&&FctUART1COMMpara.bTestModeFlag==0)//20220104
	    	{
				if((WorkVar.ucWorkOrderStatus==IDLE_STA)||(WorkVar.ucWorkOrderStatus==CHARGE_STA)\
					||(WorkVar.ucWorkOrderStatus==CLOSE_STA)
				)
				{// ���кͳ��״̬
					if(Voice.ucLongKeyMenuValue>0) Voice.ucLongKeyMenuValue=0;
					else Voice.ucLongKeyMenuValue=1;
					if(Voice.ucLongKeyMenuValue==1) 
						{
						VoiceListpara.VoicePromptSettingLanguageFlag=1;
						Voice.ucLanguageCount_back=Voice.ucLanguageCount;
						}
					else
					   {
						VoiceListpara.VoicePromptLanguageSuccessFlag=1;
						Voice.ucLanguageCount=Voice.ucLanguageCount_back;
						E2prom.uiWriteDataFlash[EE_LANGUAGE]	 = Voice.ucLanguageCount;
						E2prom.bEnableEEpromFlag = 1;				
					   }	
					KeyVar.bFastVoiceFlag = 1;
					Voice.ucSendVoiceOVcnt=0;
				}
	    	}
		}
	}
}
//****************���������ͷź�����*******************
///*���ܣ�      ���������ͷź���	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void KEY_VOICE_RELESE_10S(void)
{
	if(Voice.ucLongKeyMenuValue==1)
		{
		 Voice.ucLongKeyMenuValue=0;
		 Voice.ucLanguageCount_back=Voice.ucLanguageCount;
		}
}
//****************����ఴ���ͷź�����*******************
///*���ܣ�      ����ఴ���ͷź���	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void ADKEY_CLEAN_PRESS_UP(void)
{
 if(ErrorVar.xpBmsSleepErrorFlag==0)// 20220105
	{
	WorkVar.ucCloseScreencnt=0;
	WorkVar.ucCloseMachinecnt=0;
	
	if(MtoLCDData.xpLCDStatus==CLOSE_LCD&&WorkVar.ucWorkOrderStatus!=CLOSE_STA)// 20220111
		{
		}
	else
		{
		if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpRunErrorFlag==0)// 20210909
		// if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&\
		//	ErrorVar.xpWBoxErrorFlag==0&&ErrorVar.xpRunErrorFlag==0)// 20210915
			{
			 if(KeyVar.bADFirstFlag==1)
		//		 if((WorkVar.ucWorkOrderStatus!=CLOSE_STA)&&(KeyVar.bADFirstFlag==1))
				{
				KeyVar.bCleanKeyPressFlag = 1;
				KeyVar.bGearKeyPressFlag   = 0;
				}
			else
				{
				KeyVar.bCleanKeyPressFlag = 0;
				KeyVar.bGearKeyPressFlag   = 0;
				}
			}
		}
	}
 else
 	{
	KeyVar.bFastVoiceFlag		= 1;
	Voice.ucSendVoiceOVcnt=0;
	VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���	
	MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
	KeyVar.bGearKeyPressFlag   = 0;
	KeyVar.bCleanKeyPressFlag  = 0;
 	}
 MtoBMSData.xpKeyPushStatus=1;
 KeyVar.ucKeyWakeUpcnt=0;
}
//****************����ఴ������������*******************
///*���ܣ�      ����ఴ����������	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************
void ADKEY_CLEAN_LONG_PRESS(void)
{
	WorkVar.ucCloseMachinecnt=0;

	if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpRunErrorFlag==0)
		{
		MtoBMSData.xpKeyPushStatus=1;
		KeyVar.ucKeyWakeUpcnt=0;
		
		if(WorkVar.ucWorkOrderStatus!=CLOSE_STA)
			{			   
			KeyVar.bGearKeyPressFlag	= 0;
			KeyVar.bCleanKeyPressFlag	= 0;
			KeyVar.ucMainKeyStatusValue = 1;
//			if((CHGVar.bChargingFlag==0)||(TiltVar.bTiltEnableFlag==0))
			if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))
				{//�ǳ��״̬���߷�ֱ��״̬
				VoiceListpara.VoicePromptReqCleanFlag=1;//��Żص��������
//				WorkVar.ucplaylcd_cnt=1200;// 20210925
//				MtoLCDData.xpACleanWarnStatus=4;//��ʾ�Żص�������� 20210925
				KeyVar.bFastVoiceFlag		= 1;
				Voice.ucSendVoiceOVcnt=0;
				ACleanVar.bAutoCleanFlag=0;
				ACleanVar.bAutoDeepCleanFlag=0;
			   }
			else
			   {//���״̬��ֱ��״̬		   
			    if(ErrorVar.xpBmsSleepErrorFlag)
			    	{
					KeyVar.bFastVoiceFlag		= 1;
					Voice.ucSendVoiceOVcnt=0;
					MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
					VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���		  
			    	}
				else if((BMStoMData.xpBatSocVal<SOC_15_VALUE)&&\
					(ACleanVar.xpAutoCleanStatus!=AUTO_CLEAN_MODE))// 20210910
//					else if((BMStoMData.xpBatSocVal<SOC_20_VALUE)&&ACleanVar.bAutoDeepCleanFlag==0)
				   {
					VoiceListpara.VoicePromptChgBeforeDryFlag=1; //�������  
					MtoLCDData.xpACleanWarnStatus=1;//��ʾ������������ 20210925
					ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
					ACleanVar.bAutoDeepCleanFlag=0;
					KeyVar.bFastVoiceFlag		= 1;
					Voice.ucSendVoiceOVcnt=0;
				   }
				else //if(ACleanVar.bAutoCleanFlag==0)
				   {// δ������ͨ��������������
					ErrorVar.xpErrorPlayCnt2=0;
					ErrorVar.xpErrorPlayCnt3=0;
					ErrorVar.xpErrorPlayCnt4=0;
				    WaterBSta.ucQBucketStatus = QBULET_FULL_STA;
				    ACleanVar.xpAutoCleanStatusCnt=0;
					if(ACleanVar.xpAutoCleanStatus!=AUTO_CLEAN_MODE)
//					if((ACleanVar.bAutoCleanFlag==0)||(ACleanVar.xpAutoCleanStep==STEP9))
				   	    {						
						    ACleanVar.bAutoCleanFlag=1;
						    ACleanVar.xpAutoDrySoloFlag=1;// 20210924
							ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //�����  
						    ACleanVar.xpAutoCleanCount	 = 0;
						    ACleanVar.xpAutoCleanStep	 = STEP7;  
							KeyVar.bFastVoiceFlag = 1;
							Voice.ucSendVoiceOVcnt=0;
				     	}
					}
			   }
		   }
		}
}
//****************���������̰�������*******************
///*���ܣ�      ���������̰�����	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************
void ADKEY_MODE_PRESS_UP(void)
{
	
	if(ErrorVar.xpBmsSleepErrorFlag==0)// 20220105
		{
		WorkVar.ucCloseScreencnt=0;
		WorkVar.ucCloseMachinecnt=0;
		
        if(MtoLCDData.xpLCDStatus==CLOSE_LCD&&WorkVar.ucWorkOrderStatus!=CLOSE_STA)// 20220111
        	{
        	}
		else
			{
			if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpBmsSleepErrorFlag==0&&\
				ErrorVar.xpWBoxErrorFlag==0&&ErrorVar.xpRunErrorFlag==0)// 20210915
		//		if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0&&ErrorVar.xpBmsSleepErrorFlag==0\
		//			&&ErrorVar.xpRunErrorFlag==0)// 20210909
				{
				if(WorkVar.ucWorkOrderStatus!=CLOSE_STA)
					{
					KeyVar.bGearKeyPressFlag = 1;
					KeyVar.bCleanKeyPressFlag = 0;
					}
				else
					{
					KeyVar.bGearKeyPressFlag   = 0;
					KeyVar.bCleanKeyPressFlag  = 0;
					}
				}
			}
		}
	else
		{
		KeyVar.bFastVoiceFlag		= 1;
		Voice.ucSendVoiceOVcnt=0;
		VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���	
		MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
		KeyVar.bGearKeyPressFlag   = 0;
		KeyVar.bCleanKeyPressFlag  = 0;
		}
	MtoBMSData.xpKeyPushStatus=1;
	KeyVar.ucKeyWakeUpcnt=0;
}
//****************��������������ఴ���ɿ�������*******************
///*���ܣ�      ��������������ఴ���ɿ�����	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************
void ADKEY_RELESE(void)
{
	KeyVar.bADFirstFlag=1;
	if(ErrorVar.xpServiceErrorFlag==0&&ErrorVar.xpCommErrorFlag==0)
		{// �޹��������
		if(KeyVar.bCleanKeyPressFlag==1)
			{// ����ఴ������
			/*
			if(ErrorVar.xpBmsSleepErrorFlag==0)// 20220105
				{
			    WorkVar.ucCloseScreencnt=0;
			    WorkVar.ucCloseMachinecnt=0;
				}
			MtoBMSData.xpKeyPushStatus=1;
			KeyVar.ucKeyWakeUpcnt=0;
			*/
//			if(CHGVar.bChargingFlag==0)
			if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))	
//				if((CHGVar.bChargingFlag==0)||(TiltVar.bTiltEnableFlag==0))
				{//�ǳ��״̬���߷�ֱ��״̬
				ACleanVar.bAutoCleanFlag=0;
				ACleanVar.bAutoDeepCleanFlag=0;
				ACleanVar.xpAutoCleanCount	  = 0;
				ACleanVar.xpAutoCleanStep	  = STEP0;	
				ACleanVar.xpAutoCleanStatus = NON_AUTO_CLEAN_MODE; //�������
				VoiceListpara.VoicePromptReqCleanFlag=1;	//��Żص��������	
//				WorkVar.ucplaylcd_cnt=1200;// 20210925
				if(WorkVar.ucWorkOrderStatus!=CLOSE_STA)MtoLCDData.xpACleanWarnStatus=4;//��ʾ�Żص�������� 20210925
				KeyVar.bFastVoiceFlag = 1;
				Voice.ucSendVoiceOVcnt=0;
				}
			else
				{//���״̬��ֱ��״̬
				if(ErrorVar.xpBmsSleepErrorFlag)
			    	{
					KeyVar.bFastVoiceFlag		= 1;
					Voice.ucSendVoiceOVcnt=0;
					VoiceListpara.VoicePromptSleepREQFlag=1;//�����У������·Żص���	
					MtoLCDData.xpACleanWarnStatus=2; // ��ʾ�����ؼ����� 20210925
			    	}
//				else if((BMStoMData.xpBatSocVal<SOC_20_VALUE)&&(WorkVar.ucWorkOrderStatus==CHARGE_STA))// 20220226
				else if((BMStoMData.xpBatSocVal<SOC_20_VALUE)&&(ACleanVar.xpAutoCleanStatus!=AUTO_CLEAN_MODE))// 20210910
//			        else if((BMStoMData.xpBatSocVal<SOC_20_VALUE)&&ACleanVar.bAutoCleanFlag==0)
				   {
					VoiceListpara.VoicePromptLowPowerLowPowerCleanFlag=1; // ��ص����ͣ�����������    
                    MtoLCDData.xpACleanWarnStatus=1;//��ʾ������������ 20210925
//					ACleanVar.xpAutoCleanStatus=NON_AUTO_CLEAN_MODE;
//				    ACleanVar.bAutoCleanFlag=0;
// 20220226
                    if(WorkVar.ucWorkOrderStatus!=CHARGE_STA)
                    	{// �������״̬�ȴ��������̣������������20%����������ఴ������������ص����״̬
                    	 WorkVar.ucWorkOrderStatus=CHARGE_STA;
						 VoiceListpara.VoicePromptStartChargeFlag=1;// ��ʼ���
                    	}
// 20220226
					KeyVar.bFastVoiceFlag= 1;
					Voice.ucSendVoiceOVcnt=0;
				   }
				else 
				   {
					ErrorVar.xpErrorPlayCnt2=0;
					ErrorVar.xpErrorPlayCnt3=0;
					ErrorVar.xpErrorPlayCnt4=0;
					WaterBSta.ucQBucketStatus = QBULET_FULL_STA;
					if(ACleanVar.bAutoCleanFlag==0&&ACleanVar.xpAutoCleanStep_back==0&&ACleanVar.xpAutoCleanStep_back1==0)
	//					if((ACleanVar.bAutoCleanFlag==0)||(ACleanVar.xpAutoCleanStep==STEP9))
						{
						ACleanVar.bAutoCleanFlag    =1;
						ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //�����  
//                      if(ErrorVar.xpWBoxErrorFlag==0)VoiceListpara.VoicePromptStartCleanFlag=1;//��ʼ�����
//						VoiceListpara.VoicePromptStartCleanFlag=1;//��ʼ�����
						ACleanVar.xpAutoCleanStep_back=0;
						ACleanVar.xpAutoCleanStep_back1=0;
						WaterElecVar.waterelec_OK_flag=0;// 20220107
						ACleanVar.xpAutoCleanCount_back=0;
						ACleanVar.xpAutoCleanCount_back1=0;
						ACleanVar.xpAutoCleanCount	  = 0;
						ACleanVar.xpAutoCleanStep	  = STEP0;	
						ACleanVar.xpAutoDrySoloFlag=0;// 20210924
						KeyVar.bFastVoiceFlag = 1;
						Voice.ucSendVoiceOVcnt=0;
						}
					else
						{
						 ACleanVar.xpAutoCleanStatusCnt=0;// �ȴ�������ʱ
						 if((ACleanVar.xpAutoCleanStatus==WAIT_AUTO_CLEAN_MODE)||\
						 	((ACleanVar.xpAutoCleanStep_back||ACleanVar.xpAutoCleanStep_back1)&&(ACleanVar.xpAutoCleanStep==STEP9)))
						 	{
						 	//20210913
						 	 if(ACleanVar.xpAutoCleanStep==STEP7)
						 	 	{
								 Clear_VoiceFlag(&VoiceListpara);
								 VoiceListpara.VoicePromptBrushDryFlag=1;//��ˢ�����
						 	 	}
							 else
							 	{
								 MtoLCDData.xpQWaterBoxStatus=QBULET_FULL_STA;//20210929
								 WaterBSta.ucQBucketStatus = QBULET_FULL_STA;// 20210929
								 ACleanVar.xpAutoDrySoloFlag=0;// 20210924	
								 /////20210930
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
								 /////20210930
								 Clear_VoiceFlag(&VoiceListpara);
								 VoiceListpara.VoicePromptContinueCleanFlag=1;//���������
							 	}
							 //20210913
							 ACleanVar.bAutoCleanFlag	 =1;// 20220225
							 ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //�����  
							 KeyVar.bFastVoiceFlag = 1;
							 Voice.ucSendVoiceOVcnt=0;
						    }
						 else if(ACleanVar.xpAutoCleanStep==STEP0)// ���ˮ������
						 	{
							ACleanVar.xpAutoDrySoloFlag=0;// 20210924
							ACleanVar.xpAutoCleanStep_back=STEPELEC;
							ACleanVar.xpAutoCleanCount_back=ACleanVar.xpAutoCleanCount;
							if(ACleanVar.xpAutoCleanStep_back1>0)
								{
								ACleanVar.xpAutoCleanCount=ACleanVar.xpAutoCleanCount_back1;
								ACleanVar.xpAutoCleanStep=ACleanVar.xpAutoCleanStep_back1;
								}
							else
								{
								ACleanVar.xpAutoCleanCount	  = 0;
								ACleanVar.xpAutoCleanStep=STEP1;
								}
							KeyVar.bVoiceFlag = 1;			
							// 20220225
							ACleanVar.bAutoCleanFlag	=1;// 
							ACleanVar.xpAutoCleanStatus = AUTO_CLEAN_MODE; //�����  
							//////////
							Clear_VoiceFlag(&VoiceListpara);
							VoiceListpara.VoicePromptStartCleanFlag=1;//��ʼ�����
							KeyVar.bFastVoiceFlag = 1;
							Voice.ucSendVoiceOVcnt=0;
						 	}
						 else if(ACleanVar.xpAutoCleanStep<STEP6)// ����������
						 	{
							 ACleanVar.xpAutoCleanStep_back1=ACleanVar.xpAutoCleanStep;
							 ACleanVar.xpAutoCleanCount_back1=ACleanVar.xpAutoCleanCount;
							 ACleanVar.xpAutoDrySoloFlag=0;// 20210924
						 	 ACleanVar.bAutoCleanFlag=0;
							 ACleanVar.xpAutoCleanCount= 0;
							 ACleanVar.xpAutoCleanStep= STEP9;  
							 Clear_VoiceFlag(&VoiceListpara);
							 VoiceListpara.VoicePromptSelfCleanPausedFlag=1; 	 //��ͣ�����
							 KeyVar.bFastVoiceFlag = 1;
							 Voice.ucSendVoiceOVcnt=0;
						 	}
						 else if(ACleanVar.xpAutoCleanStep==STEP7)// �����
						 	{
							 ACleanVar.xpAutoDrySoloFlag=0;// 20210924
						 	 ACleanVar.bAutoCleanFlag=0;
							 ACleanVar.xpAutoCleanCount= AUTO_STEP8_TIME;// 20210913
							 ACleanVar.xpAutoCleanStep= STEP9;  
//          				 ACleanVar.xpAutoCleanStep_back=0;
//                           ACleanVar.xpAutoCleanCount_back=0;
							 Clear_VoiceFlag(&VoiceListpara);
							 VoiceListpara.VoicePromptBrushDryOffFlag=1; 	 //��ɹ����ѹر�  
							 KeyVar.bFastVoiceFlag = 1;
							 Voice.ucSendVoiceOVcnt=0;
						 	}
						 else //if(ACleanVar.xpAutoCleanStep<STEP9)// �ػ���
						 	{
						 	 //ACleanVar.bAutoCleanFlag=0;
							 ACleanVar.xpAutoDrySoloFlag=0;// 20210924
							 ACleanVar.xpAutoCleanStep_back1=0;
							 WaterElecVar.waterelec_OK_flag=0;// 20220107
							 ACleanVar.xpAutoCleanStep_back=0;
							 ACleanVar.xpAutoCleanCount_back=0;
							 ACleanVar.xpAutoCleanCount_back1=0;
							 ACleanVar.xpAutoCleanCount    = AUTO_STEP8_TIME;
							 ACleanVar.xpAutoCleanStep	   = STEP9;  
							 Clear_VoiceFlag(&VoiceListpara);
							 VoiceListpara.VoicePromptCleanCloseFlag=1; 	 //������ѹر�  
							 KeyVar.bFastVoiceFlag = 1;
							 Voice.ucSendVoiceOVcnt=0;
						 	}
						}
					}
				}
			}
		else if(KeyVar.bGearKeyPressFlag==1)
			{// ��λ��������	
			/*
			if(ErrorVar.xpBmsSleepErrorFlag==0)// 20220105
				{
			    WorkVar.ucCloseScreencnt=0;
			    WorkVar.ucCloseMachinecnt=0;
				}
			MtoBMSData.xpKeyPushStatus=1;
			KeyVar.ucKeyWakeUpcnt=0;
			*/
			if((WorkVar.ucWorkOrderStatus!=CHARGE_STA)&&(WorkVar.ucWorkOrderStatus!=AUTO_STA))	
//				if(CHGVar.bChargingFlag==0)
				{
				/*
				 if(TiltVar.bTiltEnableFlag==0)
					{// ��ֱ��״̬
					 if(KeyVar.ucKeyGearValue) KeyVar.ucKeyGearValue = 0;
					 else KeyVar.ucKeyGearValue = 1;
					}
				 else
					{// ֱ��״̬
 				  if(KeyVar.ucKeyGearValue<2) KeyVar.ucKeyGearValue ++;
//                if(KeyVar.ucKeyGearValue==0) KeyVar.ucKeyGearValue = 1;
				  else KeyVar.ucKeyGearValue = 0;
				//	}
				*/
				 if(KeyVar.ucKeyGearValue_back<2) KeyVar.ucKeyGearValue_back ++;
				 else KeyVar.ucKeyGearValue_back = 0;
				 /////////202220210
				 if(KeyVar.ucKeyGearValue_back==0)
					{// �Զ�ģʽ
					VoiceListpara.VoicePromptAutoModeFlag=1;				
					}
				  else if(KeyVar.ucKeyGearValue_back==1)
					{ //����ģʽ
					 VoiceListpara.VoicePromptDisinfectModeFlag=1; 
					}
				  else
					{ // ��ˮģʽ
					 VoiceListpara.VoicePromptTurboModeFlag=1;	
					}
				  ///////
				 KeyVar.ucKeyGearStayCnt=0;
				 #if LCDScreenEnableE==1
				 MtoLCDData.xpworkgeartime=1;
				 #else
				 #endif
				}
			KeyVar.bFastVoiceFlag = 1;
			Voice.ucSendVoiceOVcnt=0;
			}
	}
	
#if LCDScreenEnableE==1
	if((WorkVar.ucWorkStatus==OPEN_STA)&&MtoLCDData.xpworkgeartime==1)
#else
    if(WorkVar.ucWorkStatus==OPEN_STA)
#endif
		
//		if((KeyVar.ucKeyGearValue_back!=KeyVar.ucKeyGearValue)&&(WorkVar.ucWorkStatus==OPEN_STA))
		{
		WorkVar.ucChangeStateCnt5=0;// 20220105
	  if(++KeyVar.ucKeyGearStayCnt>(2000/AD_TICKS_INTERVAL))
//		if(++KeyVar.ucKeyGearStayCnt>(4000/AD_TICKS_INTERVAL))
		 	{
			 KeyVar.ucKeyGearStayCnt=0;
#if LCDScreenEnableE==1
			 MtoLCDData.xpworkgeartime=0;
#else
#endif
			 KeyVar.ucKeyGearValue=KeyVar.ucKeyGearValue_back;
/*
			 if(KeyVar.ucKeyGearValue_back==0)
				{// �Զ�ģʽ
				VoiceListpara.VoicePromptAutoModeFlag=1;				
				}
			  else if(KeyVar.ucKeyGearValue_back==1)
				{ //����ģʽ
				 VoiceListpara.VoicePromptDisinfectModeFlag=1; 
				}
			  else
				{ // ��ˮģʽ
				 VoiceListpara.VoicePromptTurboModeFlag=1;	
				}
*/
		 	}
		}
	else
		{
		if(WorkVar.ucWorkStatus!=OPEN_STA)
			{
//		    KeyVar.ucKeyGearValue_back=0;
#if LCDScreenEnableE==1
			MtoLCDData.xpworkgeartime=0;
#else
#endif
//		    KeyVar.ucKeyGearValue=KeyVar.ucKeyGearValue_back;
			}
		KeyVar.ucKeyGearStayCnt=0;
		}
	KeyVar.ucMainKeyStatusValue = 1;
	KeyVar.bGearKeyPressFlag = 0;
	KeyVar.bCleanKeyPressFlag= 0;
}
//****************������ʼ������*******************
///*���ܣ�      ������ʼ��	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************
void KEY_Init(void)
{
	button_init(&button1,read_button1_GPIO,0);// ������ʼ��
	
	button_attach(&button1,PRESS_UP,button1_callback);
	button_attach(&button1,LONG_PRESS2_START,button1_callback);
	button_attach(&button1,RELESE_PRESS_10S,button1_callback);// �����¼�ע��
	
	button_start(&button1);// �������� 

	ADbutton_init(&ADbutton1,read_button1_AD,0,CLEAN_KEY_AD);
	ADbutton_init(&ADbutton2,read_button1_AD,CHANGE_GEARS_AD,KEY_NOT_DOWN_AD);
	ADbutton_init(&ADbutton3,read_button1_AD,KEY_NOT_DOWN_AD,4099);// ������ʼ�� 
	
	ADbutton_attach(&ADbutton1,PRESS_UP,ADbutton1_callback);
	ADbutton_attach(&ADbutton1,LONG_PRESS1_START,ADbutton1_callback);
	ADbutton_attach(&ADbutton2,PRESS_UP,ADbutton2_callback);
	ADbutton_attach(&ADbutton3,LONG_PRESS_HOLD,ADbutton3_callback);// �����¼�ע��
	
	ADbutton_start(&ADbutton3);// �������� 
	ADbutton_start(&ADbutton2);// �������� 
	ADbutton_start(&ADbutton1);// �������� 
}




