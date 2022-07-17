#include"tilt.h"
#include"gpio.h"

//*********************�㵹�������ż�⺯��***********************
///*���ܣ�  	ÿ10ms���һ��,��ȡ�㵹�������Ÿߵ͵�ƽ							*///
///*��ڲ�����	��														*///
///*���ڲ�����	��														*///
//************************************************************

uint8_t Read_TiltPIN(void)
{
 return (uint8_t)pbTilt_Key;
}

//*********************�㵹���ؼ�⺯��***********************
///*���ܣ�	ÿ10ms���һ�Σ�����ʱ����200ms								*///
///*��ڲ�����	��														*///
///*���ڲ�����	��														*///
//************************************************************
void Check_Tilt(TILTPara* tiltcheck)
{
	if(Read_TiltPIN()==1)
	{
		if(++tiltcheck->uiTiltCnt>TILT_TIME)
		{
			tiltcheck->uiTiltCnt=TILT_TIME;
			tiltcheck->bTiltEnableFlag = 1;
		}
	}
	else
	{
		if(tiltcheck->uiTiltCnt>0)tiltcheck->uiTiltCnt>>=1;
		else tiltcheck->bTiltEnableFlag = 0;
	}	
}



