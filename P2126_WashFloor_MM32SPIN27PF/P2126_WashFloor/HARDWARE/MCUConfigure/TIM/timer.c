#include "gpio.h"
#include "init.h"
#include "timer.h"
#include "eeprom.h"
#include "adc.h"
#include "key.h"
#include "sewage.h"
#include "voice.h"

extern TimePara       TimeVar;
extern ADCPara        ADCVar; 
extern SewagePara     SewageVar;
extern KeyPara        KeyVar;

extern VoiceList      VoiceListpara;
extern VoicePara      Voice;
extern Eeprom_TypeDef E2prom;
extern KeyPara        KeyVar;

/********************************************************************************************************
**函数信息 ：void TIM3_PWM_Init(u16 Prescaler,u16 Period)
**功能描述 ：配置定时器3向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
**PB1--水泵
********************************************************************************************************/
void TIM3_PWM_Init(u16 Prescaler,u16 Period)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_StructInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;					
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 			
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_1);		
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR寄存器值
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //预分频值
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //采样分频值
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                                          //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //根据TIM_OCInitStruct中指定的参数初始化外设TIMx		
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);                           //CH1预装载使能	 	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);	
}
/********************************************************************************************************
**函数信息 ：void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**功能描述 ：配置定时器2向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
**PB11--地刷电机
********************************************************************************************************/
void TIM2_PWM_Init(u16 Prescaler,u32 Period)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_StructInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;						//TIM1_CH2N,TIM1_CH3N输出
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_2);		//tim2_ch4
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR寄存器值
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //预分频值
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //采样分频值
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 60;                                         //设置待装入捕获比较寄存器的脉冲值，
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = (Period/2-60);                              //60:Period/2是50US，减去60是为了更好的采集地刷电机电流，2022-06-09
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);                                    //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_OCInitStructure.TIM_Pulse = 0;    
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);		
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);                           //CH4预装载使能	 

	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Enable);//TIM_TRGOSource_Update
    //Master-slave mode MSM
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

/********************************************************************************************************
**函数信息 ：void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**功能描述 ：配置定时器2向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void TIM1_PWM_Init(u16 Prescaler,u32 Period)
{
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR寄存器值
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //预分频值
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //采样分频值
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 120;                                          //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = 600;    
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);                                    //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OCInitStructure.TIM_Pulse = 1200;    
//	TIM_OC3Init(TIM1, &TIM_OCInitStructure);                                    //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	
//	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);                           //CH4预装载使能	 

	TIM_ARRPreloadConfig(TIM1, ENABLE);     
    //Trigger selection, ITR1 is TIM2
    TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);
    //Trigger mode only starts; door mode starts and stops are controlled
    TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Trigger);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

/********************************************************************************************************
**函数信息 ：void SysTick_init(uint16_t uiHz)
**功能描述 ：systick延时函数初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_init(uint16_t uiHz)
{
    if (SysTick_Config(SystemCoreClock / uiHz))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置
}


/********************************************************************************************************
**函数信息 ：SysTick_Handler(void)
**功能描述 ：进入该中断函数后，Systick进行递减 1ms中断一次
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_Handler(void)			
{
 TimeVar.bTmr01msFlag=1;
 TimeVar.bTmrSysFlag=1;// 20220227
 
 if(++TimeVar.bTmr02msCount>1)
 	{
    TimeVar.bTmr02msFlag=1;
	TimeVar.bTmr02msCount=0;
 	}
 if(++TimeVar.bTmr10msCount>9)
 	{
	 if(++TimeVar.bTmr500msCount>100)//50)
		 {
		 TimeVar.bTmr500msFlag=1;
		 TimeVar.bFLASH500msFlag=1;
		 TimeVar.bTmr500msCount=0;
		 }
    TimeVar.bTmr10msFlag=1;
	TimeVar.bTmr10msCount=0;
 	}
 if(++TimeVar.bTmr100msCount>99)//50)
	 {
	 TimeVar.bTmr100msFlag=1;
	 TimeVar.bTmr100msCount=0;
	 }
 if(TimeVar.bPowerOn1sCount<2000)
 	{
 	TimeVar.bPowerOn1sCount++;
	if(TimeVar.bPowerOn1sCount==500)
		{
		Read_EEPROM();
		TimeVar.bPowerOnmsFlag=1;
		}
 	}
// pbPtc_COMM_TOGGLE;
}

