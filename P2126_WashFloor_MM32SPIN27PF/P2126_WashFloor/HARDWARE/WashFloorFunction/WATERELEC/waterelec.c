#include "waterelec.h"
#include "gpio.h"

//****************电解水电源控制脚使能函数*******************
///*功能：      10ms执行一次 电解水电源控制脚使能控制		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
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
//****************电解水控制脚1置位函数*******************
///*功能：      10ms执行一次 电解水控制脚1置位		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
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
//****************电解水控制脚2置位函数*******************
///*功能：      10ms执行一次 电解水控制脚2置位		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
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

//****************电解水故障脚检测函数*******************
///*功能：      10ms执行一次 检测电解水模块故障脚电平	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

uint8_t Read_WaterE_FaultPin(void)
{
  return (uint8_t)pbFAULT;
}
//****************电解水控制脚1检测函数*******************
///*功能：      10ms执行一次 检测电解水控制脚1电平	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

uint8_t Read_WaterElecPIN1(void)
{
  return (uint8_t)pbPWM1;
}
//****************电解水控制脚2检测函数*******************
///*功能：      10ms执行一次 检测电解水控制脚2电平	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

uint8_t Read_WaterElecPIN2(void)
{
  return (uint8_t)pbPWM2;
}

//****************电解水功能打开函数*******************
///*功能：      10ms执行一次 打开电解水功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_WaterElec_ON(WaterElecPara* Wateeelecctlpara)
{
	Wateeelecctlpara->bEnalbeWaterElecFlag=1;
}
//****************电解水功能关闭函数*******************
///*功能：      10ms执行一次 关闭电解水功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_WaterElec_OFF(WaterElecPara* Wateeelecctlpara)
{
	Wateeelecctlpara->bEnalbeWaterElecFlag=0;
}

//*****************电解水控制的函数*******************
///*功能：10ms执行一次	控制电解水的启停和故障检测			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
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

//*****************电解水控制的函数*******************
///*功能：10ms执行一次	控制电解水的启停和故障检测			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
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

