#include"tilt.h"
#include"gpio.h"

//*********************倾倒开关引脚检测函数***********************
///*功能：  	每10ms检测一次,读取倾倒开关引脚高低电平							*///
///*入口参数：	无														*///
///*出口参数：	无														*///
//************************************************************

uint8_t Read_TiltPIN(void)
{
 return (uint8_t)pbTilt_Key;
}

//*********************倾倒开关检测函数***********************
///*功能：	每10ms检测一次，消抖时间是200ms								*///
///*入口参数：	无														*///
///*出口参数：	无														*///
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



