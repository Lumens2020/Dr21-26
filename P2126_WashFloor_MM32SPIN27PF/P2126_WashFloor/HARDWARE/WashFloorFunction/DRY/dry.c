#include "dry.h"
#include "gpio.h"

//****************��ɿ��ƽ��øߺ���*******************
///*���ܣ�      10msִ��һ�� ��ɿ��ƽ��ø�		 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_DryPin_H(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	pbPtc_COMM_ON;
}

//****************��ɿ��ƽŸ������뺯��*******************
///*���ܣ�      10msִ��һ�� ��ɿ��ƽ�����Ϊ��������	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_DryPin_INFloat(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//****************��ɿ��ƽ��õͺ���*******************
///*���ܣ�      10msִ��һ�� ��ɿ��ƽ��õ�		 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_DryPin_L(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	pbPtc_COMM_OFF;
}

//****************��ȡ��ɿ��ƽŵ�ƽ����*******************
///*���ܣ�      10msִ��һ�� ��ȡ��ɿ��ƽŵ�ƽ	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

uint8_t Read_DryPin(void)
{
  return (uint8_t)pbPtc_COMM;
}

//****************��ɹ��ܴ򿪺���*******************
///*���ܣ�      10msִ��һ�� �򿪺�ɹ���	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_Dry_ON(DryPara* dryctlpara)
{
   dryctlpara->bEnalbeDryFlag=1;
}

//****************��ɹ��ܹرպ���*******************
///*���ܣ�      10msִ��һ�� �رպ�ɹ���	 						*///
///*��ڲ�����	��																*///
///*���ڲ�����	��																				*///
//************************************************************

void Set_Dry_OFF(DryPara* dryctlpara)
{
   dryctlpara->bEnalbeDryFlag=0;
}

//*****************��ɿ��ƺ���*******************
///*���ܣ�10msִ��һ��	���ƺ�ɵ�����ֹͣ�͹��ϼ��			 						*///
///*��ڲ�����	DryPara															*///
///*���ڲ�����	��															    *///
//************************************************************
void Test_Control_Dry(DryPara* dryctlpara)
{
  if(dryctlpara->bEnalbeDryFlag)
  	{
        if(dryctlpara->dry_comm_toggle_flag>0)
        	{
			Set_DryPin_L();
			dryctlpara->dry_comm_toggle_flag=0;
        	}
		else
			{
			Set_DryPin_INFloat();
			if(Read_DryPin()==0)
				{
				 if(++dryctlpara->dry_low_cnt>DRY_ERROR_TIME)
					{
					dryctlpara->bDryStallErrFlag=1;
					dryctlpara->bEnalbeDryFlag=0;
					dryctlpara->dry_low_cnt=DRY_ERROR_TIME;
					}
				}
			else
				{
				dryctlpara->dry_low_cnt=0;
				}
			dryctlpara->dry_comm_toggle_flag=1;
			}
  	}
  else
  	{
	  dryctlpara->dry_low_cnt=0;
	  dryctlpara->dry_low_cnt_H=0;
	  dryctlpara->dry_low_ERROR_cnt=0;
 	  Set_DryPin_INFloat();
  	}
}

//*****************��ɿ��ƺ���*******************
///*���ܣ�10msִ��һ��	���ƺ�ɵ�����ֹͣ�͹��ϼ��			 						*///
///*��ڲ�����	DryPara															*///
///*���ڲ�����	��															    *///
//************************************************************
void Control_Dry(DryPara* dryctlpara)
{
  if(dryctlpara->bEnalbeDryFlag&&dryctlpara->bDryPowerFlag&&(dryctlpara->bDryStallErrFlag==0))
  	{
        if(dryctlpara->dry_comm_toggle_flag>0)
        	{
			Set_DryPin_L();
			dryctlpara->dry_comm_toggle_flag=0;
        	}
		else
			{
			Set_DryPin_INFloat();
			#if DRY_BUS_MODE==1
            if(Read_DryPin()==0)
            	{
            	if(dryctlpara->dry_low_cnt_H>0)
            		{
					if(dryctlpara->dry_low_cnt_H>3)
					   {// 100ms 200ms
					   dryctlpara->dry_low_ERROR_cnt=0;
					   }
					else if(dryctlpara->dry_low_cnt_H>1)
					   {// 50ms 100ms
						if(++dryctlpara->dry_low_ERROR_cnt>20)
							{
							dryctlpara->bDryStallErrFlag=1;
							dryctlpara->bEnalbeDryFlag=0;
							dryctlpara->dry_low_ERROR_cnt=20;
							}
					   }
					else
						{
						dryctlpara->dry_low_ERROR_cnt=0;
						}
            		}
				dryctlpara->dry_low_cnt_H=0;
            	}
			else
				{
            	if(++dryctlpara->dry_low_cnt_H>300)
            		{// 20211108
            		dryctlpara->dry_low_cnt_H=300;// ����δ�Ӵ�
//					dryctlpara->bDryStallErrFlag=1;
//					dryctlpara->bEnalbeDryFlag=0;
            		}
				}
			#else
			if(Read_DryPin()==0)
				{
				 if(++dryctlpara->dry_low_cnt>DRY_ERROR_TIME)
					{
					dryctlpara->bDryStallErrFlag=1;
					dryctlpara->bEnalbeDryFlag=0;
					dryctlpara->dry_low_cnt=DRY_ERROR_TIME;
					}
				}
			else
				{
				dryctlpara->dry_low_cnt=0;
				}
			#endif
			dryctlpara->dry_comm_toggle_flag=1;
			}
  	}
  else
  	{
	  dryctlpara->dry_low_cnt=0;
	  dryctlpara->dry_low_cnt_H=0;
	  dryctlpara->dry_low_ERROR_cnt=0;
 	  Set_DryPin_INFloat();
  	}
}


