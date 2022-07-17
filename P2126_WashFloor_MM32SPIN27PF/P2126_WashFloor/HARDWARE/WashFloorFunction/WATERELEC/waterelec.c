#include "waterelec.h"
#include "gpio.h"

//****************���ˮ��Դ���ƽ�ʹ�ܺ���*******************
///*���ܣ�      10msִ��һ�� ���ˮ��Դ���ƽ�ʹ�ܿ���		 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Enable_WaterElecDC(uint8_t pinctl)
{
	if(pinctl>0)
		{
	    pbW_EN_ON;
		}
	else
		{
	    pbW_EN_OFF;
		}
}
//****************���ˮ���ƽ�1��λ����*******************
///*���ܣ�      10msִ��һ�� ���ˮ���ƽ�1��λ		 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************
void Set_WaterElecPIN1(uint8_t pinctl)
{
	if(pinctl>0)
	   {
		pbPWM1_ON;
	   }
	else
	   {
		pbPWM1_OFF;
	   }
}
//****************���ˮ���ƽ�2��λ����*******************
///*���ܣ�      10msִ��һ�� ���ˮ���ƽ�2��λ		 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_WaterElecPIN2(uint8_t pinctl)
{
	if(pinctl>0)
	   {
		pbPWM2_ON;
	   }
	else
	   {
		pbPWM2_OFF;
	   }
}

//****************���ˮ���Ͻż�⺯��*******************
///*���ܣ�      10msִ��һ�� �����ˮģ����Ͻŵ�ƽ	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

uint8_t Read_WaterE_FaultPin(void)
{
  return (uint8_t)pbFAULT;
}
//****************���ˮ���ƽ�1��⺯��*******************
///*���ܣ�      10msִ��һ�� �����ˮ���ƽ�1��ƽ	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

uint8_t Read_WaterElecPIN1(void)
{
  return (uint8_t)pbPWM1;
}
//****************���ˮ���ƽ�2��⺯��*******************
///*���ܣ�      10msִ��һ�� �����ˮ���ƽ�2��ƽ	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

uint8_t Read_WaterElecPIN2(void)
{
  return (uint8_t)pbPWM2;
}

//****************���ˮ���ܴ򿪺���*******************
///*���ܣ�      10msִ��һ�� �򿪵��ˮ����	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_WaterElec_ON(WaterElecPara* Wateeelecctlpara)
{
	Wateeelecctlpara->bEnalbeWaterElecFlag=1;
}
//****************���ˮ���ܹرպ���*******************
///*���ܣ�      10msִ��һ�� �رյ��ˮ����	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_WaterElec_OFF(WaterElecPara* Wateeelecctlpara)
{
	Wateeelecctlpara->bEnalbeWaterElecFlag=0;
}

//*****************���ˮ���Ƶĺ���*******************
///*���ܣ�10msִ��һ��	���Ƶ��ˮ����ͣ�͹��ϼ��			 						*///
///*��ڲ�����	��																				*///
///*���ڲ�����	��																				*///
//************************************************************
void Control_WaterElec_Test(WaterElecPara* Wateeelecctlpara,VoiceList* voicelistpara,WorkPara* workstate)
{
	if(Wateeelecctlpara->bEnalbeWaterElecFlag&&(Wateeelecctlpara->bWaterElecStallErrFlag==0))
	   {
	   Enable_WaterElecDC(1);
	   Wateeelecctlpara->waterelec_work_open_cnt=10;
		 if(Wateeelecctlpara->waterelec_work_close_cnt>0)
		 	{
		 	Wateeelecctlpara->waterelec_work_close_cnt--;
			Set_WaterElecPIN1(0);
			Set_WaterElecPIN2(0);
		 	}
		 else
		 	{
			 if(Wateeelecctlpara->waterelec_work_cnt<=100)
			 	{
				Set_WaterElecPIN1(1);
				Set_WaterElecPIN2(0);
			 	}
			 else if(Wateeelecctlpara->waterelec_work_cnt<=102)
			 	{
				 Set_WaterElecPIN1(0);
				 Set_WaterElecPIN2(0);
			 	}
			 else
			 	{
				 Set_WaterElecPIN1(0);
				 Set_WaterElecPIN2(1);
			 	}
		 	}
		 /////////////////////////
	   if(Read_WaterE_FaultPin()==0)
		  {
		   if(++Wateeelecctlpara->waterelec_low_cnt>WaterElec_ERRORTime)
			   {
			   Wateeelecctlpara->bWaterElecStallErrFlag=1;
			   Wateeelecctlpara->bEnalbeWaterElecFlag=0;
			   Wateeelecctlpara->waterelec_low_cnt=WaterElec_ERRORTime;
			   }
		   }
	   else
		   {
		   if(Wateeelecctlpara->waterelec_low_cnt>0)Wateeelecctlpara->waterelec_low_cnt>>=1;
		   }
	   if(++Wateeelecctlpara->waterelec_work_cnt>220)
	   	{
		   Wateeelecctlpara->waterelec_work_cnt=0;
	   	}
	   }
	else
	   {
	   Wateeelecctlpara->waterelec_work_close_cnt=10;
	   Set_WaterElecPIN1(0);
	   Set_WaterElecPIN2(0);
	   if(Wateeelecctlpara->waterelec_work_open_cnt>0)
		  {
		  Wateeelecctlpara->waterelec_work_open_cnt--;
		  Enable_WaterElecDC(0);
		  }
	   Wateeelecctlpara->waterelec_low_cnt=0;
	   }
}

//*****************���ˮ���Ƶĺ���*******************
///*���ܣ�10msִ��һ��	���Ƶ��ˮ����ͣ�͹��ϼ��			 						*///
///*��ڲ�����	��																				*///
///*���ڲ�����	��																				*///
//************************************************************
void Control_WaterElec(WaterElecPara* Wateeelecctlpara,VoiceList* voicelistpara,WorkPara* workstate)
{
	if(Wateeelecctlpara->bEnalbeWaterElecFlag&&(Wateeelecctlpara->bWaterElecStallErrFlag==0))
	   {
	   Enable_WaterElecDC(1);
	   Wateeelecctlpara->waterelec_work_open_cnt=10;
		 if(Wateeelecctlpara->waterelec_work_close_cnt>0)
		 	{
		 	Wateeelecctlpara->waterelec_work_close_cnt--;
			Set_WaterElecPIN1(0);
			Set_WaterElecPIN2(0);
		 	}
		 else
		 	{
			 if(Wateeelecctlpara->waterelec_work_cnt<=(WaterElec_WorkTime/2))
			 	{
				Set_WaterElecPIN1(1);
				Set_WaterElecPIN2(0);
			 	}
			 else if(Wateeelecctlpara->waterelec_work_cnt<=(WaterElec_WorkTime/2+2))
			 	{
				 Set_WaterElecPIN1(0);
				 Set_WaterElecPIN2(0);
			 	}
			 else
			 	{
				 Set_WaterElecPIN1(0);
				 Set_WaterElecPIN2(1);
			 	}
		 	}
		 /////////////////////////
	   if(Read_WaterE_FaultPin()==0)
		  {
		   if(++Wateeelecctlpara->waterelec_low_cnt>WaterElec_ERRORTime)
			   {
			   Wateeelecctlpara->bWaterElecStallErrFlag=1;
			   Wateeelecctlpara->bEnalbeWaterElecFlag=0;
			   Wateeelecctlpara->waterelec_low_cnt=WaterElec_ERRORTime;
			   }
		   }
	   else
		   {
		   if(Wateeelecctlpara->waterelec_low_cnt>0)Wateeelecctlpara->waterelec_low_cnt>>=1;
		   }
	   if(++Wateeelecctlpara->waterelec_work_cnt>WaterElec_WorkTime)
	   	{
	   	#if WaterElecTestEnable==1
		   Wateeelecctlpara->waterelec_work_cnt=WaterElec_WorkTime;
		#else
		   Wateeelecctlpara->waterelec_work_cnt=0;
		   Wateeelecctlpara->waterelec_OK_flag=1;
		   Wateeelecctlpara->bEnalbeWaterElecFlag=0;
        #endif
	   	}
	   if(Wateeelecctlpara->waterelec_work_cnt==(WaterElec_WorkTime/30))
	   	{
	   	 if(workstate->ucWorkOrderStatus!=AUTO_STA)
	   	 	{
	   	    voicelistpara->VoicePromptSanitizerStayFlag=1;
	   	 	}
		 else
		 	{
	   	    voicelistpara->VoicePromptCheckFullClearFlag=1;		 	
		 	}
		 
//		 voicelistpara->VoicePromptSanitizerStayFlag=1;
//		 voicelistpara->VoicePromptCheckFullClearFlag=1;		 
	   	}
	   else if(Wateeelecctlpara->waterelec_work_cnt==((WaterElec_WorkTime/2)-80))voicelistpara->VoicePromptSanitizerHalfFlag=1;
	   else if(Wateeelecctlpara->waterelec_work_cnt==(WaterElec_WorkTime-100))voicelistpara->VoicePromptSanitizerCompletedFlag=1;
       Wateeelecctlpara->waterelec_work_cnt_cntdn=WaterElec_WorkTime-Wateeelecctlpara->waterelec_work_cnt;
	   }
	else
	   {
	   Wateeelecctlpara->waterelec_work_close_cnt=10;
	   Set_WaterElecPIN1(0);
	   Set_WaterElecPIN2(0);
	   if(Wateeelecctlpara->waterelec_work_open_cnt>0)
		  {
		  Wateeelecctlpara->waterelec_work_open_cnt--;
		  Enable_WaterElecDC(0);
		  }
	   Wateeelecctlpara->waterelec_low_cnt=0;
	   }
}

