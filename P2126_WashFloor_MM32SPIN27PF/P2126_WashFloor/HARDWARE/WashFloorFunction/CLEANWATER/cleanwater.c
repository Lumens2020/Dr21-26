#include "cleanwater.h"
#include "gpio.h"

uint8_t Read_WaterBox_PIN(void)
{
 return (uint8_t)pbWater_Level;
}

void Check_WaterBox(WaterBoxStatus* waterboxctl,ADCPara* advalue)
{
  if(waterboxctl->bWaterLVEnableFlag)	//ˮ������һ��ʱ��󣬲�ʹ�ܼ�⣬��ֹˮ����ˮ
 	{
	  if(advalue->uiCleanWaterAdcValue>WATERLV_CHECKAD)
//		  if(Read_WaterBox_PIN()>0)
		{
			if(waterboxctl->uiWaterLevelCnt<(WATERLEVEL_TIME+10))
			{
			  waterboxctl->uiWaterLevelCnt++;
			  if(waterboxctl->uiWaterLevelCnt==WATERLEVEL_TIME)
			  	{
				 waterboxctl->ucQBucketStatus = QBULET_BLANK_STA;					
			  	}
			}
		    waterboxctl->uiWaterLevelRecCnt=0;
		}
		else
		{
		    waterboxctl->uiWaterLevelCnt=0;
			if(waterboxctl->uiWaterLevelRecCnt<(WATERLEVEL_TIME+10))
			{
			  waterboxctl->uiWaterLevelRecCnt++;
			  if(waterboxctl->uiWaterLevelRecCnt==WATERLEVEL_TIME)
			  	{
				 waterboxctl->ucQBucketStatus = QBULET_FULL_STA;					
			  	}
			}
		}	
 	}
  else
  	{
  	waterboxctl->uiWaterLevelCnt=0;
  	}
}

