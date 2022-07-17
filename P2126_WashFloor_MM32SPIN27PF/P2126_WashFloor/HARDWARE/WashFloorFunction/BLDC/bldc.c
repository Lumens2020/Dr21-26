#include "bldc.h"

//****************�������ˢ�رտ��ƺ���*******************
///*���ܣ�10msִ��һ��	�����������ͣ��		 						*///
///*��ڲ�����	BLDCCtl																				*///
///*���ڲ�����	��																				*///
//************************************************************
void Set_Bldc_OFF(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=1;
	bldcctrlpara->bBldcForceStrongFlag=0;
	bldcctrlpara->bBldcForceLowFlag=0;
}

//****************�������ˢǿ�����ƺ���*******************
///*���ܣ�10msִ��һ��	�������������ǿ��			 						*///
///*��ڲ�����	BLDCCtl																				*///
///*���ڲ�����	��																				*///
//************************************************************
void Set_Bldc_Strong(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=0;
	bldcctrlpara->bBldcForceStrongFlag=1;
	bldcctrlpara->bBldcForceLowFlag=0;
}

//****************�������ˢ�͵����ƺ���*******************
///*���ܣ�10msִ��һ��	��������������͵�			 						*///
///*��ڲ�����	BLDCCtl																				*///
///*���ڲ�����	��																				*///
//************************************************************
void Set_Bldc_Low(BLDCCtl* bldcctrlpara)
{
	bldcctrlpara->bBldcForceStopFlag=0;
	bldcctrlpara->bBldcForceStrongFlag=0;
	bldcctrlpara->bBldcForceLowFlag=1;
}

//****************�������ˢ���ƺ���*******************
///*���ܣ�10msִ��һ��	�����������������ֹͣ			 						*///
///*��ڲ�����	BLDCCtl     MCUtoBLDCData																	*///
///*���ڲ�����	��																				*///
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

