#include"charge.h"
#include"gpio.h"


uint8_t Read_CHG_PIN(void)
{
 return (uint8_t)CHG_PIN;
}

//*********************充电检测函数***********************
///*功能：	每10ms检测一次，消抖时间是100ms								*///
///*入口参数：	无														*///
///*出口参数：	无														*///
//************************************************************
void Check_Charge(CHGPara* chgparactl,ADCPara* advalue)
{
#if CHG_AD_ENABLE==1
	if(advalue->uiCHGAdcValue<1800)
#else
	if(Read_CHG_PIN()==0)
#endif
	{
        if(chgparactl->uiCHGCnt>0)chgparactl->uiCHGCnt--;
		else
		{
			chgparactl->bChargingFlag = 0;
		}	
	}
	else
	{
		if(++chgparactl->uiCHGCnt>CHARGE_TIME)
		{
			chgparactl->uiCHGCnt      = CHARGE_TIME;			
			chgparactl->bChargingFlag = 1;
		}				
	}
}




