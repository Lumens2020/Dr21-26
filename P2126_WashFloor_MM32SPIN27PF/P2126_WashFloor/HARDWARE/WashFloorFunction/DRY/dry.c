#include "dry.h"
#include "gpio.h"

//****************烘干控制脚置高函数*******************
///*功能：      10ms执行一次 烘干控制脚置高		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
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

//****************烘干控制脚浮空输入函数*******************
///*功能：      10ms执行一次 烘干控制脚设置为浮空输入	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_DryPin_INFloat(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//****************烘干控制脚置低函数*******************
///*功能：      10ms执行一次 烘干控制脚置低		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
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

//****************读取烘干控制脚电平函数*******************
///*功能：      10ms执行一次 读取烘干控制脚电平	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

uint8_t Read_DryPin(void)
{
  return (uint8_t)pbPtc_COMM;
}

//****************烘干功能打开函数*******************
///*功能：      10ms执行一次 打开烘干功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_Dry_ON(DryPara* dryctlpara)
{
   dryctlpara->bEnalbeDryFlag=1;
}

//****************烘干功能关闭函数*******************
///*功能：      10ms执行一次 关闭烘干功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_Dry_OFF(DryPara* dryctlpara)
{
   dryctlpara->bEnalbeDryFlag=0;
}

//*****************烘干控制函数*******************
///*功能：10ms执行一次	控制烘干的启动停止和故障检测			 						*///
///*入口参数：	DryPara															*///
///*出口参数：	无															    *///
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

//*****************烘干控制函数*******************
///*功能：10ms执行一次	控制烘干的启动停止和故障检测			 						*///
///*入口参数：	DryPara															*///
///*出口参数：	无															    *///
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
            		dryctlpara->dry_low_cnt_H=300;// 触点未接触
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


