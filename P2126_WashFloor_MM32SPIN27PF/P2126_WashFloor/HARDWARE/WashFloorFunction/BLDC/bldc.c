#include "bldc.h"

//****************主电机无刷关闭控制函数*******************
///*功能：10ms执行一次	控制主电机的停机		 						*///
///*入口参数：	BLDCCtl																				*///
///*出口参数：	无																				*///
//************************************************************
void Set_Bldc_OFF(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=1;
	bldcctrlpara->bBldcForceStrongFlag=0;
	bldcctrlpara->bBldcForceLowFlag=0;
}

//****************主电机无刷强力控制函数*******************
///*功能：10ms执行一次	控制主电机工作强力			 						*///
///*入口参数：	BLDCCtl																				*///
///*出口参数：	无																				*///
//************************************************************
void Set_Bldc_Strong(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=0;
	bldcctrlpara->bBldcForceStrongFlag=1;
	bldcctrlpara->bBldcForceLowFlag=0;
}

//****************主电机无刷低档控制函数*******************
///*功能：10ms执行一次	控制主电机工作低档			 						*///
///*入口参数：	BLDCCtl																				*///
///*出口参数：	无																				*///
//************************************************************
void Set_Bldc_Low(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=0;
	bldcctrlpara->bBldcForceStrongFlag=0;
	bldcctrlpara->bBldcForceLowFlag=1;
}

//****************主电机无刷控制函数*******************
///*功能：10ms执行一次	控制主电机的启动与停止			 						*///
///*入口参数：	BLDCCtl     MCUtoBLDCData																	*///
///*出口参数：	无																				*///
//************************************************************
void Control_MainMotor(BLDCCtl* bldcctrlpara,MCUtoBLDCData* MToBldcD)
{
  if(bldcctrlpara->bBldcForceStopFlag)
  	{
	  MToBldcD->xpworkgear=STOP_BLDC;
  	}
  else if(bldcctrlpara->bBldcForceStrongFlag)
  	{
	  MToBldcD->xpworkgear=HIGH_BLDC;
  	}
  else 
  	{
	  MToBldcD->xpworkgear=LOW_BLDC;
  	}
}

