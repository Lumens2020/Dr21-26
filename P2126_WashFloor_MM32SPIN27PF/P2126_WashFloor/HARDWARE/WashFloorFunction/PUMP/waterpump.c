#include "waterpump.h"
#include "pumpvolt.h"
#include "cleanwater.h"
#include "timer.h"

//****************水泵功能打开函数*******************
///*功能：      10ms执行一次 打开水泵功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_WaterPump_ON(PumpPara* pumpparactl)
{
	pumpparactl->bEnalbePumpFlag=1;
}

//****************水泵功能关闭函数*******************
///*功能：      10ms执行一次 关闭水泵功能	 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Set_WaterPump_OFF(PumpPara* pumpparactl)
{
	pumpparactl->bEnalbePumpFlag=0;
}

//*****************水泵占空比处理函数*******************
///*功能：10ms执行一次	根据电池包电压控制水泵的频率对应水量	 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************
void Deal_PumpDuty(uint32_t batVolt,PumpPara* pumpparactl)
{
	if(batVolt<16500)	   pumpparactl->uiPumpVolt = 0;
	else if(batVolt<17000) pumpparactl->uiPumpVolt = 1;
	else if(batVolt<17500) pumpparactl->uiPumpVolt = 2;
	else if(batVolt<18000) pumpparactl->uiPumpVolt = 3;
	else if(batVolt<18500) pumpparactl->uiPumpVolt = 4;
	else if(batVolt<19000) pumpparactl->uiPumpVolt = 5;
	else if(batVolt<18500) pumpparactl->uiPumpVolt = 6;
	else if(batVolt<20000) pumpparactl->uiPumpVolt = 7;
	else if(batVolt<20500) pumpparactl->uiPumpVolt = 8;
	else if(batVolt<21000) pumpparactl->uiPumpVolt = 9;
	else if(batVolt<21500) pumpparactl->uiPumpVolt = 10;
	else if(batVolt<22000) pumpparactl->uiPumpVolt = 11;
	else if(batVolt<22500) pumpparactl->uiPumpVolt = 12;
	else if(batVolt<23000) pumpparactl->uiPumpVolt = 13;
	else if(batVolt<23500) pumpparactl->uiPumpVolt = 14;	
	else if(batVolt<24000) pumpparactl->uiPumpVolt = 15;
	else if(batVolt<24500) pumpparactl->uiPumpVolt = 16;			
	else if(batVolt<25000) pumpparactl->uiPumpVolt = 17;	
	else pumpparactl->uiPumpVolt = 18;
}




//*****************水泵启停控制函数*******************
///*功能：      10ms执行一次	控制水泵的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************

void Test_Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl,WorkPara* workstate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if((pumpparactl->bPumpShortErrFlag==0)&&(pumpparactl->bPumpResetFlag==0)&&(pumpparactl->bEnalbePumpFlag==1))// 20211012
     	{		 
		 switch(pumpparactl->ucPumpGearValue)		 //根据档位和电压，给定不同的占空比和频率
			{
			 case 0:				 
			 	if(pumpparactl->ucPumpGearCHGFlag1)
			 		{
				    pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0];
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;
					}
				 break;
			 case 1:
			 	if(pumpparactl->ucPumpGearCHGFlag2)
			 		{
				    pumpparactl->ulPumpDuty = uc50ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc50ccPumpVolt[pumpparactl->uiPumpVolt][0];		
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc50ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc50ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;		
					}
				 break;
			 case 2:
			 	if(pumpparactl->ucPumpGearCHGFlag3)
			 		{
				    pumpparactl->ulPumpDuty = uc100ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc100ccPumpVolt[pumpparactl->uiPumpVolt][0];		
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc100ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc100ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;		
					}
				 break;
			 case 3:
				 pumpparactl->ulPumpDuty = uc200ccPumpVolt[pumpparactl->uiPumpVolt][1];
				 pumpparactl->ulPumpFreq = uc200ccPumpVolt[pumpparactl->uiPumpVolt][0];						 
				 break; 			 
			 default:
				 pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1];
				 pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0];				 
				 break; 		 
			}
     	}
	 else
	 	{
		 pumpparactl->ulPumpDuty = 0;
		 pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0]; 
		 pumpparactl->uiEnableQWaterLCnt= 0;
		 if(pumpparactl->bEnalbePumpFlag==0)
		 	{		 	
			 waterboxctl->bWaterLVEnableFlag=0;
		 	}
	 	}	 
	 ///////////////////////////////////////////////////
		 if(++pumpparactl->ulPumpshortperiod>200)
			 {
			 pumpparactl->ulPumpshortCnt=0;
			 pumpparactl->ulPumpshorttime=0;
			 pumpparactl->ulPumpshortperiod=0;
			 }
		 if(pumpparactl->bPumpResetFlag)
			 {
			  if(++pumpparactl->ulPumpshortCnt>10)
				 {
				  pumpparactl->bPumpResetFlag=0;
				  pumpparactl->ulPumpshortCnt=0;
				  GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_1; 				  
				  GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;		  
				  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
				  GPIO_Init(GPIOB, &GPIO_InitStructure); 
				  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_1);	  
				  if(++pumpparactl->ulPumpshorttime>5)
					 {
					   pumpparactl->ulPumpshorttime=0;
					   pumpparactl->bPumpShortErrFlag=1;
					 }
				 }
			 }
		 else
			 {
			 pumpparactl->ulPumpshortCnt=0;
			 }
	 /////////////////////////////////////////////////////	 

 if(pumpparactl->ulPumpFreq<5)
 	{
 	 WaterPump_PERIOD=(uint16_t)(TIM3_FREQFACT/5);
 	}
 else
 	{
	 WaterPump_PERIOD=(uint16_t)(TIM3_FREQFACT/pumpparactl->ulPumpFreq);
 	}

 WaterPump_PWM=(uint16_t)(pumpparactl->ulPumpDuty*WaterPump_PERIOD/100);
}

//*****************水泵启停控制函数*******************
///*功能：      10ms执行一次	控制水泵的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************

void Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl,WorkPara* workstate)
//	void Control_Pump(PumpPara* pumpparactl,WaterBoxStatus* waterboxctl,BrushPara* brushparactl)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if((pumpparactl->bPumpShortErrFlag==0)&&(pumpparactl->bPumpResetFlag==0)&&(pumpparactl->bEnalbePumpFlag==1)&&((brushparactl->ulBrushONCnt>=BRUSH_OK_TIME)||(workstate->ucWorkOrderoffcnt>0)))// 20211012
//		if((pumpparactl->bPumpShortErrFlag==0)&&(pumpparactl->bPumpResetFlag==0)&&(pumpparactl->bEnalbePumpFlag==1)&&(brushparactl->ulBrushONCnt>=BRUSH_OK_TIME))// 20211012
//		if((pumpparactl->bPumpShortErrFlag==0)&&(pumpparactl->bPumpResetFlag==0)&&(pumpparactl->bEnalbePumpFlag==1)&&(brushparactl->ulBrushONCnt>=BRUSH_OK_TIME)&&(waterboxctl->ucQBucketStatus==QBULET_FULL_STA))
     	{
     	 if(waterboxctl->bWaterLVEnableFlag==0)
     	 	{
			 if(++pumpparactl->uiEnableQWaterLCnt>WATER_DELAY_TIME)
				{//使用前水泵抽水判断是否有水
					 waterboxctl->bWaterLVEnableFlag=1;
					 pumpparactl->uiEnableQWaterLCnt= 0;
				}			 
     	 	}
		 if(waterboxctl->ucQBucketStatus==QBULET_BLANK_STA)pumpparactl->ucPumpGearValue=0;
		 
		 switch(pumpparactl->ucPumpGearValue)		 //根据档位和电压，给定不同的占空比和频率
			{
			 case 0:				 
			 	if(pumpparactl->ucPumpGearCHGFlag1)
			 		{
				    pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0];
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;
					}
				 break;
			 case 1:
			 	if(pumpparactl->ucPumpGearCHGFlag2)
			 		{
				    pumpparactl->ulPumpDuty = uc50ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc50ccPumpVolt[pumpparactl->uiPumpVolt][0];		
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc50ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc50ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;		
					}
				 break;
			 case 2:
			 	if(pumpparactl->ucPumpGearCHGFlag3)
			 		{
				    pumpparactl->ulPumpDuty = uc100ccPumpVolt[pumpparactl->uiPumpVolt][1];
				    pumpparactl->ulPumpFreq = uc100ccPumpVolt[pumpparactl->uiPumpVolt][0];		
			 		}
				else
					{
				    pumpparactl->ulPumpDuty = uc100ccPumpVolt[pumpparactl->uiPumpVolt][1]-1;
				    pumpparactl->ulPumpFreq = uc100ccPumpVolt[pumpparactl->uiPumpVolt][0]-1;		
					}
				 break;
			 case 3:
				 pumpparactl->ulPumpDuty = uc200ccPumpVolt[pumpparactl->uiPumpVolt][1];
				 pumpparactl->ulPumpFreq = uc200ccPumpVolt[pumpparactl->uiPumpVolt][0];						 
				 break; 			 
			 default:
				 pumpparactl->ulPumpDuty = uc30ccPumpVolt[pumpparactl->uiPumpVolt][1];
				 pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0];				 
				 break; 		 
			}
     	}
	 else
	 	{
		 pumpparactl->ulPumpDuty = 0;
		 pumpparactl->ulPumpFreq = uc30ccPumpVolt[pumpparactl->uiPumpVolt][0]; 
		 pumpparactl->uiEnableQWaterLCnt= 0;
		 if(pumpparactl->bEnalbePumpFlag==0)
		 	{		 	
			 waterboxctl->bWaterLVEnableFlag=0;
		 	}
	 	}	 
	 ///////////////////////////////////////////////////
		 if(++pumpparactl->ulPumpshortperiod>200)
			 {
			 pumpparactl->ulPumpshortCnt=0;
			 pumpparactl->ulPumpshorttime=0;
			 pumpparactl->ulPumpshortperiod=0;
			 }
		 if(pumpparactl->bPumpResetFlag)
			 {
			  if(++pumpparactl->ulPumpshortCnt>10)
				 {
				  pumpparactl->bPumpResetFlag=0;
				  pumpparactl->ulPumpshortCnt=0;
				  GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_1; 				  
				  GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;		  
				  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
				  GPIO_Init(GPIOB, &GPIO_InitStructure); 
				  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_1);	  
				  if(++pumpparactl->ulPumpshorttime>5)
					 {
					   pumpparactl->ulPumpshorttime=0;
					   pumpparactl->bPumpShortErrFlag=1;
					 }
				 }
			 }
		 else
			 {
			 pumpparactl->ulPumpshortCnt=0;
			 }
	 /////////////////////////////////////////////////////	 

 if(pumpparactl->ulPumpFreq<5)
 	{
 	 WaterPump_PERIOD=(uint16_t)(TIM3_FREQFACT/5);
 	}
 else
 	{
	 WaterPump_PERIOD=(uint16_t)(TIM3_FREQFACT/pumpparactl->ulPumpFreq);
 	}

 WaterPump_PWM=(uint16_t)(pumpparactl->ulPumpDuty*WaterPump_PERIOD/100);
}



