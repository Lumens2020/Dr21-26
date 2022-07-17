#include "adc.h"
#include "timer.h"
#include "gpio.h"
#include "brush.h"
#include "waterpump.h"
#include "cleanwater.h"
#include "sewage.h"

#include "iwdg.h"
#include "voice.h"

extern TimePara TimeVar;
extern ADCPara  ADCVar; 
extern PumpPara PumpVar;
extern BrushPara BrushVar;

extern SewagePara     SewageVar;
extern WaterBoxStatus WaterBSta;

extern VoiceList      VoiceListpara;
extern VoicePara      Voice;
extern Eeprom_TypeDef E2prom;
extern KeyPara        KeyVar;


void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    
    GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_0|GPIO_Pin_6);//(GPIO_Pin_0);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_14|GPIO_Pin_15);//
	GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);//
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);		
}
void ADC1_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
	/* Initialize the ADC IO  */
    ADC_GPIO_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);   
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_8;//ADC_PCLK2_PRESCARE_4;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
//    ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T2_CC1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    /* Initialize the ADC1 */
    ADC_Init(ADC1, &ADC_InitStructure);
    /*使能ADC1选中通道,后面参数保留*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  0, ADC_SampleTime_13_5Cycles);   // AIN0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 0, ADC_SampleTime_13_5Cycles);   // AIN10
	  /* Enable ADC Intterupt */
//		ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);			
    ADC_Cmd(ADC1, ENABLE);
//	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
}

void ADC2_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_8;//ADC_PCLK2_PRESCARE_4;
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC1;
 //   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    /* Initialize the ADC2 */
    ADC_Init(ADC2, &ADC_InitStructure);
	
	  ADC2->ADCR |= 0x00010000;	//高通道号优先采样  ，即从高到低采样   

    /*使能ADC2选中通道,后面参数保留*/
		
	/*
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1,  0,ADC_SampleTime_28_5Cycles);// ADC_SampleTime_13_5Cycles);   // AIN1
    ADC_RegularChannelConfig(ADC2, ADC_Channel_2,  0,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3,  0,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN3
    ADC_RegularChannelConfig(ADC2, ADC_Channel_6,	0,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
    ADC_RegularChannelConfig(ADC2, ADC_Channel_10,	0,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
   */
	 
	 /*设置了rank号，2022-06-09*/
	  ADC_RegularChannelConfig(ADC2, ADC_Channel_1,  1,ADC_SampleTime_28_5Cycles);// ADC_SampleTime_13_5Cycles);   // AIN1
    ADC_RegularChannelConfig(ADC2, ADC_Channel_2,  2,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3,  3,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN3
    ADC_RegularChannelConfig(ADC2, ADC_Channel_6,	 4,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
    ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 5,ADC_SampleTime_28_5Cycles);//  ADC_SampleTime_13_5Cycles);   // AIN2
		
	  /* Enable ADC Intterupt */
	 NVIC_InitStructure.NVIC_IRQChannel = ADC2_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 ADC_ITConfig(ADC2,ADC_IT_EOC,ENABLE);	
   ADC_Cmd(ADC2, ENABLE); 
	 ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	 ADC_ExternalTrigConvCmd(ADC2,ENABLE);		 
}

void LOAD_SHORT_CHECK(ADCPara* advalue,PumpPara* pumpparactl,BrushPara* brushparactl)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//短路判断
	if((advalue->uiADC_ResultBuff[WaterPumpCHN]>SHORT_PUMP_VREF)&&(WaterPump_PWM>0))
		{
		WaterPump_PWM= 0;	
		if(pumpparactl->bPumpShortErrFlag==0)
			{
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_7);
			GPIO_InitStructure.GPIO_Pin  	=  GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
			}
		pumpparactl->bPumpShortErrFlag=1;
		}
	if((advalue->uiADC_ResultBuff[Brush_CHN]>SHORT_BRUSH_VREF)&&(BRUSH_PWM>0))
		{
		BRUSH_PWM= 0;	
		if(brushparactl->bBrushShortErrFlag==0)
			{
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_0);
			GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_11);			
			}
		brushparactl->bBrushShortErrFlag=1;
		}
}

void Eimination_AdValue(ADCPara* advalue)
{
 //   static uint16_t V1,V2,V3,V4,V5;
    static uint16_t V3,V4;

//AD值消抖
   if(advalue->V_adc_trans_count>0)
	   {
	   advalue->V_adc_trans_count--;
	   if(advalue->V_adc_trans_count==0x80)
		   {
		   advalue->V_adc_trans_count=10;
		   
		   advalue->uiKeyAdcValue_max=0;
		   advalue->uiSewageLVAdcValue_max=0;
		   advalue->uiKeyAdcValue_min=0x0FFF;
		   advalue->uiSewageLVAdcValue_min=0x0FFF;
		   advalue->uiKeyAdcValue_sum=0;
		   advalue->uiSewageLVAdcValue_sum=0;
		   }
	   else if((advalue->V_adc_trans_count&0x80)==0)
		   {
		   V3=advalue->uiADC_ResultBuff[Key_CHN];
		   V4=advalue->uiADC_ResultBuff[SewageLV_CHN];
		   advalue->uiKeyAdcValue_sum+=V3;
		   advalue->uiSewageLVAdcValue_sum+=V4;
		   if(V3>advalue->uiKeyAdcValue_max)
			   {
			   advalue->uiKeyAdcValue_max=V3;
			   }
		   if(V3<advalue->uiKeyAdcValue_min)
			   {
			   advalue->uiKeyAdcValue_min=V3;
			   }
		   if(V4>advalue->uiSewageLVAdcValue_max)
			   {
			   advalue->uiSewageLVAdcValue_max=V4;
			   }
		   if(V4<advalue->uiSewageLVAdcValue_min)
			   {
			   advalue->uiSewageLVAdcValue_min=V4;
			   }
		   }
	   }
   else
	   {
	   advalue->uiKeyAdcValue_sum-=advalue->uiKeyAdcValue_max;
	   advalue->uiKeyAdcValue_sum-=advalue->uiKeyAdcValue_min;
	   advalue->uiSewageLVAdcValue_sum-=advalue->uiSewageLVAdcValue_max;
	   advalue->uiSewageLVAdcValue_sum-=advalue->uiSewageLVAdcValue_min;
	   advalue->uiKeyAdcValue_sum>>=3;
	   advalue->uiSewageLVAdcValue_sum>>=3;
	   advalue->uiKeyAdcValue=advalue->uiKeyAdcValue_sum;                      //求出key_adc采集值
	   advalue->uiSewageLVAdcValue=advalue->uiSewageLVAdcValue_sum;            //求出污水采集值
	   advalue->V_adc_trans_count=0x83;
	   }
}

void CHECK_Brush_I_AD(void)
{
		 if(ADCVar.uiBrushAdcValue_cnt<32)//8)
			{
			 ADCVar.uiBrushAdcValue_cnt++;
			 ADCVar.uiBrushAdcValue_sum+=ADCVar.uiADC_ResultBuff[Brush_CHN];			
			}
		 else
			{
			ADCVar.uiBrushAdcValue=(ADCVar.uiBrushAdcValue_sum>>5);//3);
			ADCVar.uiBrushAdcValue_cnt=0;
			ADCVar.uiBrushAdcValue_sum=0;
			}
}

/********************************************************************************************************
**函数信息 ：ADC2_IRQHandler(void)     
**功能描述 : 中断函数
**输入参数 ：无
**输出参数 ：无
**50uS进入一次ADC中断
********************************************************************************************************/
void ADC2_IRQHandler(void)
{
	static uint8_t uiAdc100usCnt=0;
	static uint8_t uiAdc500usCnt=0;
	
	ADC_ClearFlag(ADC1, ADC_IT_EOC);
	ADC_ClearFlag(ADC2, ADC_IT_EOC);	
	
	TimeVar.bTmrAdcFlag=1;// 20220227

	if(ADC2->ADCR&0x00020000) ADC2->ADCR&=0xfffdffff;
	else ADC2->ADCR|=0x00020000;

	ADCVar.uiADC_ResultBuff[WaterPumpCHN]  = WaterPumpADDR;
	ADCVar.uiADC_ResultBuff[Brush_CHN] 	    = Brush_ADDR;
	ADCVar.uiADC_ResultBuff[SewageLV_CHN] 	= SewageLV_ADDR;
	if(++uiAdc100usCnt>3)
		{
		ADCVar.uiADC_ResultBuff[SewageStage_CHN]	= SewageStage_ADDR;
		ADCVar.uiADC_ResultBuff[Key_CHN]			= Key_ADDR;
		ADCVar.uiADC_ResultBuff[CHGVol_CHN]			= CHGV_ADDR;
		ADCVar.uiADC_ResultBuff[WaterLV_CHN]		= WaterLV_ADDR;
		ADCVar.uiCHGAdcValue=ADCVar.uiADC_ResultBuff[CHGVol_CHN];
		ADCVar.uiCleanWaterAdcValue=ADCVar.uiADC_ResultBuff[WaterLV_CHN];
//		Check_SewageStage1(&WaterBSta,&SewageVar,&ADCVar);	 // 污水箱水位检测
		TimeVar.bTmr200usFlag=1;
		uiAdc100usCnt=0;
	    }
	
	if(++uiAdc500usCnt>9)
		{
		TimeVar.bTmr500usFlag=1;
		uiAdc500usCnt=0;
		}
	
	if(BrushVar.bEnalbeSlowBrushFlag==0||uiAdc100usCnt%2)
		{
		CHECK_Brush_I_AD();
		}
	/*
    if(BrushVar.bEnalbeSlowBrushFlag==1)
    	{
        if(uiAdc100usCnt%2)
        	{
	    	CHECK_Brush_I_AD();
         	}
    	}
	else
		{
		CHECK_Brush_I_AD();
		}
    */
    /*
	if(uiAdc100usCnt%2)
		{
		ADCVar.uiBrushAdcValueL=ADCVar.uiADC_ResultBuff[Brush_CHN];
		}
	else 
		{
		ADCVar.uiBrushAdcValueH=ADCVar.uiADC_ResultBuff[Brush_CHN];
		}
   */
//   pbPtc_COMM_TOGGLE;
}



