#include "sewage.h"
#include "cleanwater.h"
#include "gpio.h"
#include "adc.h"


void Enable_IRLED_PIN(uint8_t irpinctl)
{
  if(irpinctl>0)
  	{
	  pbLED_Tr_OPEN;// 3180
  	}
  else
  	{
	  pbLED_Tr_CLOSE;// 3180
  	}
}
void Enable_TEST1_PIN(uint8_t test1pinctl)
{
  if(test1pinctl>0)
  	{
	  pbFull_Test1_ON;
  	}
  else
  	{
	  pbFull_Test1_OFF;
  	}
}
void Enable_TEST2_PIN(uint8_t test2pinctl)
{
  if(test2pinctl>0)
  	{
	  pbFull_Test2_ON;
  	}
  else
  	{
	  pbFull_Test2_OFF;
  	}
}


void Check_SewageStage1(WaterBoxStatus* waterboxctl,SewagePara* sewagectl,ADCPara* adcparactl)
{
  if(waterboxctl->bSewageStageEnableFlag == DISABLE)
  	{
		 if(++waterboxctl->uiSewageStageyUsCnt>2)
			{
				 if(waterboxctl->uiSewageStageyUsCnt<5)		//2~4
					{
						 Enable_TEST1_PIN(0);                 //检测1MS
						 Enable_TEST2_PIN(1);	 
					}
				 else			//4~6
					{
					 Enable_TEST1_PIN(0);
					 Enable_TEST2_PIN(0);	 
					 waterboxctl->uiSewageStageyUsCnt=6;
					}
			}
     else		//0~1
 	    {
	    Enable_TEST1_PIN(1);                      //检测1MS
	    Enable_TEST2_PIN(0);  

		#if SEWAGE_CHECK_MODE==0
	 	if(waterboxctl->uiSewageStageyUsCnt>1)
	 	 	{
	 	 	 adcparactl->uiSewageStageAdcValue=adcparactl->uiADC_ResultBuff[SewageStage_CHN];
			 if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]<SEWAGE_STAGE_VREF)
				 {
				 if(++waterboxctl->uiSewageStageDetCnt0> sewagectl->xpSewageStateCount)//SEWAGE_STAGE_COUNT)
					 {
					 waterboxctl->ucWBucketStatus = WBULET_BLANK_STA;
					 waterboxctl->uiSewageStageDetCnt0= sewagectl->xpSewageStateCount;//SEWAGE_STAGE_COUNT;
					 }
				 waterboxctl->uiSewageStageDetCnt1=0;
				 }
			 else if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]>SEWAGE_STAGE_FULL_VREF)
				 {
				 if(++waterboxctl->uiSewageStageDetCnt1>SEWAGE_STAGE_COUNT2)
					 {
					 waterboxctl->ucWBucketStatus = WBULET_FULL_STA;
					 waterboxctl->uiSewageStageDetCnt1= sewagectl->xpSewageStateCount;//SEWAGE_STAGE_COUNT;
					 }
				 waterboxctl->uiSewageStageDetCnt0=0;
				 }
			 else
				 {
				 waterboxctl->uiSewageStageDetCnt1=0;
				 waterboxctl->uiSewageStageDetCnt0=0;
				 if(waterboxctl->ucWBucketStatus!=WBULET_FULL_STA)waterboxctl->ucWBucketStatus = WBULET_NORMAL_STA;// 20211021
				 }
		 	}
		#elif SEWAGE_CHECK_MODE==1
		 	if(waterboxctl->uiSewageStageyUsCnt>1)    //这里应该进不来，2022-06-19，验证下
		 	 	{
		 	 	 adcparactl->uiSewageStageAdcValue=adcparactl->uiADC_ResultBuff[SewageStage_CHN];   //获得污水箱AD值
				 if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]<SEWAGE_STAGE_VREF)                //污水箱未安装
					 {
					 if(++waterboxctl->uiSewageStageDetCnt0> sewagectl->xpSewageStateCount)//SEWAGE_STAGE_COUNT)
						 {// 20211021
						 if(sewagectl->xpSewageChangeFlag)
						 	{
							 waterboxctl->uiSewageStageDetCnt0=0;
						 	}
						 else
						 	{
						    waterboxctl->ucWBucketStatus = WBULET_BLANK_STA;
						 	}
						 waterboxctl->uiSewageStageDetCnt0= sewagectl->xpSewageStateCount;//SEWAGE_STAGE_COUNT;
						 }
					 waterboxctl->uiSewageStageDetCnt1=0;
					 }
				 else if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]>SEWAGE_STAGE_FULL_VREF)
					 {
					 if(++waterboxctl->uiSewageStageDetCnt1> SEWAGE_STAGE_COUNT2)
						 {
						 waterboxctl->ucWBucketStatus = WBULET_FULL_STA;
						 waterboxctl->uiSewageStageDetCnt1= sewagectl->xpSewageStateCount;//SEWAGE_STAGE_COUNT;
						 }
					 waterboxctl->uiSewageStageDetCnt0=0;
					 }
				 else
					 {
					 waterboxctl->uiSewageStageDetCnt1=0;
					 waterboxctl->uiSewageStageDetCnt0=0;
					 if(waterboxctl->ucWBucketStatus!=WBULET_FULL_STA)waterboxctl->ucWBucketStatus = WBULET_NORMAL_STA;// 20211021
					 }
			 	}
		#else 
	 	if(waterboxctl->uiSewageStageyUsCnt>1)
	 	 	{
	 	 	 adcparactl->uiSewageStageAdcValue=adcparactl->uiADC_ResultBuff[SewageStage_CHN];
			 if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]<SEWAGE_STAGE_VREF)
				 {
				 if(++waterboxctl->uiSewageStageDetCnt0>SEWAGE_STAGE_COUNT)
					 {
					 waterboxctl->ucWBucketStatus = WBULET_BLANK_STA;
					 waterboxctl->uiSewageStageDetCnt0=SEWAGE_STAGE_COUNT;
					 }
				 waterboxctl->uiSewageStageDetCnt1=0;
				 }
			 else if(adcparactl->uiADC_ResultBuff[SewageStage_CHN]>SEWAGE_STAGE_FULL_VREF)
				 {
				 if(++waterboxctl->uiSewageStageDetCnt1>SEWAGE_STAGE_COUNT2)
					 {
					 waterboxctl->ucWBucketStatus = WBULET_FULL_STA;
					 waterboxctl->uiSewageStageDetCnt1=SEWAGE_STAGE_COUNT;
					 }
				 waterboxctl->uiSewageStageDetCnt0=0;
				 }
			 else
				 {
				 waterboxctl->uiSewageStageDetCnt1=0;
				 waterboxctl->uiSewageStageDetCnt0=0;
				 if(waterboxctl->ucWBucketStatus!=WBULET_FULL_STA)waterboxctl->ucWBucketStatus = WBULET_NORMAL_STA;// 20211021
				 }
		 	}
		#endif
	 	}
  	}
  else
  	{
      waterboxctl->uiSewageStageyUsCnt=0;
	  Enable_TEST1_PIN(0);
	  Enable_TEST2_PIN(0);	  
  	}
}


void Check_SewageStage0(WaterBoxStatus* waterboxctl,SewagePara* sewagectl,ADCPara* adcparactl)
{// 10ms
    if(sewagectl->xpSewageChangeFlag==0)
      {
	  sewagectl->xpSewageStateCount=SEWAGE_STAGE_COUNT;
      }
    else
      {
	  sewagectl->xpSewageStateCount=SEWAGE_STAGE_COUNT1;
      }

	if(++waterboxctl->uiSewageStageCnt>5)
     	{
        if(waterboxctl->bSewageStageEnableFlag>0) waterboxctl->bSewageStageEnableFlag=0;
		    else waterboxctl->bSewageStageEnableFlag=1;
		    waterboxctl->uiSewageStageCnt=0;
     	}

}

void Check_SewageLV(ADCPara* adcparactl,TimePara* timesetvar,SewagePara* sewagectl,KeyPara* keyvar)
//	void Check_SewageLV(ADCPara* adcparactl,TimePara* timesetvar,SewagePara* sewagectl)
{
////////////////////////////// 20220106
 if(keyvar->ucKeyGearValue==2)// 吸水模式
	{
	 sewagectl->xpSewageLVSens=SEWAGE_TIME4;
	}
 else// 杀菌和自动模式
	{
	 sewagectl->xpSewageLVSens=SEWAGE_TIME2;
	}
 ////////////////////////////////
 if(timesetvar->bPowerOnmsFlag==0||sewagectl->xpSewageLVEnable==0)// 20220106
//   if(timesetvar->bPowerOnmsFlag==0)
 	{
	 Enable_IRLED_PIN(1);
	 if(adcparactl->uiSewageLVAdcValue<SEWAGE_COMPREF_VREF)
	 	{
//		 if(sewagectl->xpSewageLVcnt0<100)sewagectl->xpSewageLVcnt0++;
//		 if(sewagectl->xpSewageLVcnt1>0)sewagectl->xpSewageLVcnt1--;
		 if(sewagectl->xpSewageLVcnt8<100)sewagectl->xpSewageLVcnt0++;
		 if(sewagectl->xpSewageLVcnt9>0)sewagectl->xpSewageLVcnt1--;
	 	}
	 else
	 	{
//		 if(sewagectl->xpSewageLVcnt1<100)sewagectl->xpSewageLVcnt1++;
//		 if(sewagectl->xpSewageLVcnt0>0)sewagectl->xpSewageLVcnt0--;
		 if(sewagectl->xpSewageLVcnt9<100)sewagectl->xpSewageLVcnt9++;
		 if(sewagectl->xpSewageLVcnt8>0)sewagectl->xpSewageLVcnt8--;
	 	}
	 timesetvar->bPowerOnmsFlagBack=0;
//默认最干净状态
	sewagectl->xpsewagelv=M_LOW_STA;
	sewagectl->xpSewageLVcnt6=0;
	sewagectl->xpSewageLVcnt7=0;
	sewagectl->xpSewageLVcnt0=0;
	sewagectl->xpSewageLVcnt1=0;
	sewagectl->xpSewageLVcnt2=0;
	sewagectl->xpSewageLVcnt3=0;
	sewagectl->xpSewageLVcnt4=0;
	sewagectl->xpSewageWarnFlag=0;
 	}
 else
 	{
 	 if(timesetvar->bPowerOnmsFlagBack==0)
 	 	{
		 Enable_IRLED_PIN(1);
 //	 	 if(sewagectl->xpSewageLVcnt0>sewagectl->xpSewageLVcnt1)
			 if(sewagectl->xpSewageLVcnt8>sewagectl->xpSewageLVcnt9)// 20220110
 	 	 	{
 	 	 	sewagectl->xpSewageAlignValue=SEWAGE_REF_VREF;
 	 	 	}
		 else
		 	{
			 sewagectl->xpSewageAlignValue=adcparactl->uiSewageLVAdcValue;		 	
		 	}
//		 sewagectl->xpSewageLVcnt0=0;
//		 sewagectl->xpSewageLVcnt1=0;
		 sewagectl->xpSewageLVcnt8=0;// 20220110
		 sewagectl->xpSewageLVcnt9=0;// 20220110
 	 	}
	 else
	 	{
	 	 sewagectl->uiSewageHighValue=sewagectl->xpSewageAlignValue*SEWAGE_HIGH_PER;
		 sewagectl->uiSewagepromptValue0=sewagectl->xpSewageAlignValue*SEWAGE_WARN_PER;
		 sewagectl->xpSewageDefaultRefValue= sewagectl->uiSewageHighValue;
		 sewagectl->uiSewageMiddValue=sewagectl->xpSewageAlignValue*SEWAGE_MID_PER;
		 if(sewagectl->xpSewageAlignValue<3895)sewagectl->uiSewageUpValue=sewagectl->xpSewageAlignValue+200;
		 else
		 	{
		 	sewagectl->uiSewageUpValue=4095;
		 	}
		 if(sewagectl->xpSewageAlignValue>200)sewagectl->uiSewageDownValue=sewagectl->xpSewageAlignValue-200;
		 else
		 	{
		 	sewagectl->uiSewageDownValue=0;
		 	}
		 // 污染程度判断
	 	 if(adcparactl->uiSewageLVAdcValue>sewagectl->uiSewageHighValue)
	 	 	{// 污染轻度
	 	 	 sewagectl->xpSewageLVcnt7=0;
		 	 if(++sewagectl->xpSewageLVcnt2>sewagectl->xpSewageLVSens)
//			 if(++sewagectl->xpSewageLVcnt2>SEWAGE_TIME2)
		 	 	{
		 	 	 if(sewagectl->xpsewagelv>M_LOW_STA)
				 	{
					if(++sewagectl->xpSewageLVcnt6>SEWAGE_TIME3)
						{
						 sewagectl->xpsewagelv=M_LOW_STA;
						 sewagectl->xpSewageLVcnt6=0;
						 
						 sewagectl->xpSewageWarnFlag=0;// 20211229
						 sewagectl->xpSewageLVcnt3=0;//   20211229
						}
				 	}
				 else
		 	 	 	{
					sewagectl->xpsewagelv=M_LOW_STA;
		 	 	 	}
//				 sewagectl->xpSewageLVcnt2=SEWAGE_TIME2;
				 sewagectl->xpSewageLVcnt2=sewagectl->xpSewageLVSens;
		 	 	}
			 sewagectl->xpSewageLVcnt1=0;
			 sewagectl->xpSewageLVcnt0=0;
	 	 	}
		 else if(adcparactl->uiSewageLVAdcValue>sewagectl->uiSewageMiddValue)
	 	 	{// 污染中度
			 sewagectl->xpSewageLVcnt6=0;

	 	 	 if(++sewagectl->xpSewageLVcnt1>sewagectl->xpSewageLVSens)
//			 if(++sewagectl->xpSewageLVcnt1>SEWAGE_TIME2)
	 	 	 	{
				//if(sewagectl->xpsewagelv>M_MID_STA)// M_MID_STA
				//   {
				//   if(++sewagectl->xpSewageLVcnt7>SEWAGE_TIME3)
				//	   {
				//		sewagectl->xpsewagelv=M_MID_STA;
				//		sewagectl->xpSewageLVcnt7=0;
				//	   }
				//   }
				//else
				//   {
				   sewagectl->xpsewagelv=M_MID_STA;
				//   }
				sewagectl->xpSewageLVcnt1=sewagectl->xpSewageLVSens;
//				sewagectl->xpSewageLVcnt1=SEWAGE_TIME2;
	 	 	 	}
			 sewagectl->xpSewageLVcnt2=0;
			 sewagectl->xpSewageLVcnt0=0;
	 	 	}
		 else 
		 	{ // 污染重度
		 	 sewagectl->xpSewageLVcnt6=0;
			 sewagectl->xpSewageLVcnt7=0;
	 	 	 if(++sewagectl->xpSewageLVcnt0>sewagectl->xpSewageLVSens)
//			 if(++sewagectl->xpSewageLVcnt0>SEWAGE_TIME2)
	 	 	 	{
				if(sewagectl->xpsewagelv!=M_SOHIGH_STA)sewagectl->xpsewagelv=M_HIGH_STA;
//				sewagectl->xpSewageLVcnt0=SEWAGE_TIME2;
				sewagectl->xpSewageLVcnt0=sewagectl->xpSewageLVSens;
	 	 	 	}
			 sewagectl->xpSewageLVcnt2=0;
			 sewagectl->xpSewageLVcnt1=0;
		 	}
	    // 脏污清洗提醒
	    if(sewagectl->xpSewageWarnFlag==0)
	    	{
	        if(adcparactl->uiSewageLVAdcValue<SEWAGE_HIGH_VREF)
	        	{
				 ++sewagectl->xpSewageLVcnt3;
	        	 if(sewagectl->xpSewageLVcnt3>SEWAGE_TIME00)
	        	 	{
	        	 	 sewagectl->xpsewagelv=M_HIGH_STA;
	        	 	 sewagectl->xpSewageWarnFlag=1;
					 sewagectl->xpSewageLVcnt3=0;
	        	 	}
				 else if(++sewagectl->xpSewageLVcnt3>SEWAGE_TIME0)
				 	{
					 sewagectl->xpsewagelv=M_SOHIGH_STA;
				 	}
				 else
				 	{
				 	sewagectl->xpsewagelv=M_HIGH_STA;
				 	}
	        	}
			else
				{
				 sewagectl->xpSewageLVcnt3=0;
				}
	    	}
		// 连续3S监测，更新校准值
		 if((adcparactl->uiSewageLVAdcValue<sewagectl->uiSewageUpValue)&&(adcparactl->uiSewageLVAdcValue>sewagectl->uiSewageDownValue))
		 	{
		 	 if(++sewagectl->xpSewageLVcnt4>SEWAGE_TIME1)
		 	 	{
		 	 	sewagectl->xpSewageAlignValue=adcparactl->uiSewageLVAdcValue;
		 	 	sewagectl->xpSewageLVcnt4=SEWAGE_TIME1;
		 	 	}
		 	}
		 else
		 	{
		 	sewagectl->xpSewageLVcnt4=0;
		 	}
		 //
	 	}
	 timesetvar->bPowerOnmsFlagBack=1;
 	}
}

