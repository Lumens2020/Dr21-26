#include"charge.h"
#include"gpio.h"


uint8_t Read_CHG_PIN(void)
{
 return (uint8_t)CHG_PIN;
}

//*********************����⺯��***********************
///*���ܣ�	ÿ10ms���һ�Σ�����ʱ����100ms								*///
///*��ڲ�����	��														*///
///*���ڲ�����	��														*///
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




