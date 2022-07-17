#include "brush.h"
#include "timer.h"
#include "adc.h"

void Set_Brush_Normal(BrushPara* brushparactl)
{
	brushparactl->bEnalbeBrushFlag=1;
	brushparactl->bEnalbeSlowBrushFlag=0;
}

void Set_Brush_Slow(BrushPara* brushparactl)
{
	brushparactl->bEnalbeBrushFlag=1;
	brushparactl->bEnalbeSlowBrushFlag=1;
}
void Set_Brush_OFF(BrushPara* brushparactl)
{
	brushparactl->bEnalbeBrushFlag=0;
	brushparactl->bEnalbeSlowBrushFlag=0;
}

//*****************地刷占空比处理函数*******************
///*功能：10ms执行一次	控制地刷的启动与停止			 						*///
///*入口参数：	无																				*///
///*出口参数：	无																				*///
//************************************************************
void Deal_BrushDuty(uint32_t batVolt,BrushPara* brushparactl)
{
 if(brushparactl->bEnalbeSlowBrushFlag==0)
 	{
	if(batVolt<=brushparactl->ulBRUSH_REF_VOLT) brushparactl->ulBrushVoltDuty = BRUSH_PERIOD;
	else brushparactl->ulBrushVoltDuty = brushparactl->ulBRUSH_REF_VOLT*BRUSH_PERIOD/batVolt;//BRUSH_PERIOD-(((batVolt-BRUSH_REF_VOLT)*BRUSH_PERIOD)/batVolt);	
    if(brushparactl->uiBrushSlowMode==0)
    	{
	    if(brushparactl->ulBrushVoltDuty<1300)brushparactl->ulBrushVoltDuty=1300;
    	}
 	}
 else
 	{
 	 if(brushparactl->uiBrushSlowMode==1)
 	 	{
		 if(batVolt<=BRUSH_REF_SOFT_VOLT1) brushparactl->ulBrushVoltDuty = BRUSH_PERIOD;
		 else brushparactl->ulBrushVoltDuty = BRUSH_REF_SOFT_VOLT1*BRUSH_PERIOD/batVolt;//BRUSH_PERIOD-(((batVolt-BRUSH_REF_VOLT)*BRUSH_PERIOD)/batVolt);  
 	 	}
	 else if(brushparactl->uiBrushSlowMode==2)
	 	{
		 if(batVolt<=BRUSH_REF_SOFT_VOLT2) brushparactl->ulBrushVoltDuty = BRUSH_PERIOD;
		 else brushparactl->ulBrushVoltDuty = BRUSH_REF_SOFT_VOLT2*BRUSH_PERIOD/batVolt;//BRUSH_PERIOD-(((batVolt-BRUSH_REF_VOLT)*BRUSH_PERIOD)/batVolt);  
	 	}
	 else
	 	{
		 if(batVolt<=BRUSH_REF_SOFT_VOLT3) brushparactl->ulBrushVoltDuty = BRUSH_PERIOD;
		 else brushparactl->ulBrushVoltDuty = BRUSH_REF_SOFT_VOLT3*BRUSH_PERIOD/batVolt;//BRUSH_PERIOD-(((batVolt-BRUSH_REF_VOLT)*BRUSH_PERIOD)/batVolt);  
	 	}
 	}
}

//*****************控制地刷启停的函数*******************
//*功能：10ms执行一次	分别控制水泵/地刷的启动与停止			 						*///
//*入口参数：	无																				*///
//*出口参数：	无																				*///
//************************************************************
void Test_Control_Brush(BrushPara* brushparactl,ADCPara* adcparactl)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//-----------------------地刷---------------------------	
	if(brushparactl->bEnalbeBrushFlag&&(brushparactl->bBrushResetFlag==0)&&(brushparactl->bBrushStallErrFlag==0)&&(brushparactl->bBrushShortErrFlag==0))
	{
	if(adcparactl->uiBrushAdcValue>adcparactl->uiStallBrushAdcValue)
	 	{
	 	 if(++brushparactl->uiBrushStallCnt>STALL_BRUSH_TIME)
		 	{
			 brushparactl->uiBrushStallCnt = 0;
			 brushparactl->bBrushStallErrFlag = 1;
	 	 	}
		 if(adcparactl->uiBrushAdcValue>adcparactl->uiOC1BrushAdcValue)
	 	    {
	 	    if(++brushparactl->uiBrushOC1Cnt>OC1_BRUSH_TIME)
		     	{
		    	 brushparactl->uiBrushOC1Cnt = 0;
			     brushparactl->bBrushStallErrFlag = 1;
	 	     	}
			if(adcparactl->uiBrushAdcValue>adcparactl->uiOC2BrushAdcValue)
	 	        {
	 	         if(++brushparactl->uiBrushOC2Cnt>OC2_BRUSH_TIME)
		         	{
					 brushparactl->uiBrushOC2Cnt = 0;
					 brushparactl->bBrushStallErrFlag = 1;
	 	 			}
	 	        }
			else
				{
				brushparactl->uiBrushOC2Cnt = 0;
				}
	 	    }
		 else
		 	{
			 brushparactl->uiBrushOC1Cnt = 0;
			 brushparactl->uiBrushOC2Cnt = 0;
		 	}
	 	}
	 else
	 	{
		if(brushparactl->uiBrushStallCnt>0)brushparactl->uiBrushStallCnt--;
		brushparactl->uiBrushOC1Cnt = 0;
		brushparactl->uiBrushOC2Cnt = 0;
	 	}
	 
			 if((brushparactl->ulBrushDuty+50)<brushparactl->ulBrushVoltDuty)brushparactl->ulBrushDuty+=50;
			 else if(brushparactl->ulBrushDuty<brushparactl->ulBrushVoltDuty)brushparactl->ulBrushDuty++;
			 else
			 	{
			 	brushparactl->ulBrushDuty=brushparactl->ulBrushVoltDuty;
			 	}
	 if(brushparactl->ulBrushONCnt<BRUSH_OK_TIME)brushparactl->ulBrushONCnt++;
	}		
	else 
	{
	 brushparactl->ulBrushAdSUM=0;// 20211123
	 brushparactl->ulBrushONCnt=0;
	 brushparactl->uiBrushStallCnt = 0;
	 brushparactl->uiBrushNGCnt = 0;
	 brushparactl->ulBrushDuty=0;
	}
///////////////////////////////////////////////////
	if(++brushparactl->ulBrushshortperiod>200)
		{
		brushparactl->ulBrushshortCnt=0;
		brushparactl->ulBrushshorttime=0;
		brushparactl->ulBrushshortperiod=0;
		}
    if(brushparactl->bBrushResetFlag)
    	{
    	 if(++brushparactl->ulBrushshortCnt>10)
    	 	{
			 brushparactl->bBrushResetFlag=0;
			 brushparactl->ulBrushshortCnt=0;
			 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;					  //TIM1_CH2N,TIM1_CH3N输出
			 GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;		  //复用推挽输出
			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			 GPIO_Init(GPIOB, &GPIO_InitStructure); 
			 GPIO_ResetBits(GPIOB,GPIO_Pin_11);			 
			 GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_2);		  //tim2_ch4			 
			 if(++brushparactl->ulBrushshorttime>5)
			 	{
			 	  brushparactl->ulBrushshorttime=0;
				  brushparactl->bBrushShortErrFlag=1;
			 	}
		 }
    	}
	else
		{
		brushparactl->ulBrushshortCnt=0;
		}
/////////////////////////////////////////////////////	
	if(brushparactl->ulBrushDuty>=BRUSH_PERIOD)
		{
		brushparactl->ulBrushDuty=BRUSH_PERIOD;
		BRUSH_PWM=BRUSH_PERIOD;
		}
	else
		{
		BRUSH_PWM=brushparactl->ulBrushDuty;
		}
}

//*****************控制地刷启停的函数*******************
//*功能：10ms执行一次	分别控制水泵/地刷的启动与停止			 						*///
//*入口参数：	无																				*///
//*出口参数：	无																				*///
//************************************************************
void Control_Brush(BrushPara* brushparactl,ADCPara* adcparactl)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//-----------------------地刷---------------------------	
	if(brushparactl->bEnalbeBrushFlag&&(brushparactl->bBrushResetFlag==0)&&\
		(brushparactl->bBrushNGErrFlag==0)&&(brushparactl->bBrushStallErrFlag==0)&&(brushparactl->bBrushShortErrFlag==0))
	{
	if(brushparactl->ulBrushONCnt<BRUSH_OK_TIME&&brushparactl->ulBrushONCnt>BRUSH_OK_TIME0)
		{
		brushparactl->ulBrushAdSUM+=adcparactl->uiBrushAdcValue;
		}
	else if(brushparactl->ulBrushONCnt==BRUSH_OK_TIME)
//		else if(brushparactl->ulBrushONCnt>=BRUSH_OK_TIME)
		{
		if(brushparactl->ulBrushAdSUM<adcparactl->uiNOBrushAdcValue*300)// 202220107
//			if(brushparactl->ulBrushAdSUM>0&&brushparactl->ulBrushAdSUM<adcparactl->uiNOBrushAdcValue*300)
		 	{
            if((brushparactl->bEnalbeSlowBrushFlag==0)&&(brushparactl->bEnalbeNGBrushFlag==0))
            	{
		    	 brushparactl->bBrushNGErrFlag= 1;
		        }    
		 	}
		 brushparactl->ulBrushAdSUM=0;
		}
	/*
	if((adcparactl->uiBrushAdcValue<adcparactl->uiNOBrushAdcValue)&&(brushparactl->bEnalbeSlowBrushFlag==0)&&(brushparactl->bEnalbeNGBrushFlag==0))// 20211105
//			if((adcparactl->uiBrushAdcValue<adcparactl->uiNOBrushAdcValue)&&(brushparactl->bEnalbeSlowBrushFlag==0))
     	{
     	 if(brushparactl->ulBrushONCnt<BRUSH_OK_TIME)// 20210917 妥协
     	 	{
			 if(++brushparactl->uiBrushNGCnt > NO_BRUSH_TIME)
			    {
				 brushparactl->uiBrushNGCnt   = 0;
				 brushparactl->bBrushNGErrFlag= 1;
			    }
     	 	}
		 brushparactl->uiBrushOC1Cnt = 0;
		 brushparactl->uiBrushOC2Cnt = 0;
		 if(brushparactl->uiBrushStallCnt>0)brushparactl->uiBrushStallCnt--;
     	}
     */
	if(adcparactl->uiBrushAdcValue>adcparactl->uiStallBrushAdcValue)
//		else if(adcparactl->uiBrushAdcValue>adcparactl->uiStallBrushAdcValue)
	 	{
	 	 if(++brushparactl->uiBrushStallCnt>STALL_BRUSH_TIME)
		 	{
			 brushparactl->uiBrushStallCnt = 0;
			 brushparactl->bBrushStallErrFlag = 1;
	 	 	}
		 if(adcparactl->uiBrushAdcValue>adcparactl->uiOC1BrushAdcValue)
	 	    {
	 	    if(++brushparactl->uiBrushOC1Cnt>OC1_BRUSH_TIME)
		     	{
		    	 brushparactl->uiBrushOC1Cnt = 0;
			     brushparactl->bBrushStallErrFlag = 1;
	 	     	}
			if(adcparactl->uiBrushAdcValue>adcparactl->uiOC2BrushAdcValue)
	 	        {
	 	         if(++brushparactl->uiBrushOC2Cnt>OC2_BRUSH_TIME)
		         	{
					 brushparactl->uiBrushOC2Cnt = 0;
					 brushparactl->bBrushStallErrFlag = 1;
	 	 			}
	 	        }
			else
				{
				brushparactl->uiBrushOC2Cnt = 0;
				}
	 	    }
		 else
		 	{
			 brushparactl->uiBrushOC1Cnt = 0;
			 brushparactl->uiBrushOC2Cnt = 0;
		 	}
		// brushparactl->uiBrushNGCnt = 0;
	 	}
	 else
	 	{
//	 	brushparactl->uiBrushStallCnt = 0;
		if(brushparactl->uiBrushStallCnt>0)brushparactl->uiBrushStallCnt--;
	 	//brushparactl->uiBrushNGCnt = 0;
		brushparactl->uiBrushOC1Cnt = 0;
		brushparactl->uiBrushOC2Cnt = 0;
	 	}
	 
//	 if(brushparactl->bEnalbeSlowBrushFlag==0)
//	 	{
			 if((brushparactl->ulBrushDuty+50)<brushparactl->ulBrushVoltDuty)brushparactl->ulBrushDuty+=50;
			 else if(brushparactl->ulBrushDuty<brushparactl->ulBrushVoltDuty)brushparactl->ulBrushDuty++;
			 else
			 	{
			 	brushparactl->ulBrushDuty=brushparactl->ulBrushVoltDuty;
			 	}
/*	 	}
	 else
	 	{
			 if(brushparactl->uiBrushSlowMode==1)
			 	{
				 if((brushparactl->ulBrushDuty+50)<BRUSH_SLOW_PWM1)brushparactl->ulBrushDuty+=50;
				 else if(brushparactl->ulBrushDuty<BRUSH_SLOW_PWM1)brushparactl->ulBrushDuty++;
				 else
				 	{
					 brushparactl->ulBrushDuty=BRUSH_SLOW_PWM1;
				 	}
			 	}
			 else if(brushparactl->uiBrushSlowMode==2)
			 	{
				 if((brushparactl->ulBrushDuty+50)<BRUSH_SLOW_PWM2)brushparactl->ulBrushDuty+=50;
				 else if(brushparactl->ulBrushDuty<BRUSH_SLOW_PWM2)brushparactl->ulBrushDuty++;
				 else
				 	{
					 brushparactl->ulBrushDuty=BRUSH_SLOW_PWM2;
				 	}
			 	}
			 else
			 	{
				 if((brushparactl->ulBrushDuty+50)<BRUSH_SLOW_PWM)brushparactl->ulBrushDuty+=50;
				 else if(brushparactl->ulBrushDuty<BRUSH_SLOW_PWM)brushparactl->ulBrushDuty++;
				 else
				 	{
					 brushparactl->ulBrushDuty=BRUSH_SLOW_PWM;
				 	}
			 	}
	 	}
	 	*/
	if(brushparactl->ulBrushONCnt<BRUSH_CHECK_TIME)brushparactl->ulBrushONCnt++;
//	if(brushparactl->ulBrushONCnt<BRUSH_OK_TIME)brushparactl->ulBrushONCnt++;
	}		
	else 
	{
	 brushparactl->ulBrushAdSUM=0;// 20211123
	 brushparactl->ulBrushONCnt=0;
	 brushparactl->uiBrushStallCnt = 0;
	 brushparactl->uiBrushNGCnt = 0;
	 brushparactl->ulBrushDuty=0;
	}
///////////////////////////////////////////////////
	if(++brushparactl->ulBrushshortperiod>200)
		{
		brushparactl->ulBrushshortCnt=0;
		brushparactl->ulBrushshorttime=0;
		brushparactl->ulBrushshortperiod=0;
		}
    if(brushparactl->bBrushResetFlag)
    	{
    	 if(++brushparactl->ulBrushshortCnt>10)
    	 	{
			 brushparactl->bBrushResetFlag=0;
			 brushparactl->ulBrushshortCnt=0;
			 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;					  //TIM1_CH2N,TIM1_CH3N输出
			 GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;		  //复用推挽输出
			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			 GPIO_Init(GPIOB, &GPIO_InitStructure); 
			 GPIO_ResetBits(GPIOB,GPIO_Pin_11);			 
			 GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_2);		  //tim2_ch4			 
			 if(++brushparactl->ulBrushshorttime>5)
			 	{
			 	  brushparactl->ulBrushshorttime=0;
				  brushparactl->bBrushShortErrFlag=1;
			 	}
		   }
    	}
	else
		{
		brushparactl->ulBrushshortCnt=0;
		}
/////////////////////////////////////////////////////	
	if(brushparactl->ulBrushDuty>=BRUSH_PERIOD)
		{
		brushparactl->ulBrushDuty=BRUSH_PERIOD;
		BRUSH_PWM=BRUSH_PERIOD;
		}
	else
		{
		BRUSH_PWM=brushparactl->ulBrushDuty;
		}
}


