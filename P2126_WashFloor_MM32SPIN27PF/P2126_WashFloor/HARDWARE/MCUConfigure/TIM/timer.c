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
**������Ϣ ��void TIM3_PWM_Init(u16 Prescaler,u16 Period)
**�������� �����ö�ʱ��3���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
**PB1--ˮ��
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
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR�Ĵ���ֵ
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //Ԥ��Ƶֵ
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //������Ƶֵ
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //����ģʽ
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                                          //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx		
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);                           //CH1Ԥװ��ʹ��	 	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);	
}
/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
**PB11--��ˢ���
********************************************************************************************************/
void TIM2_PWM_Init(u16 Prescaler,u32 Period)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_StructInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;						//TIM1_CH2N,TIM1_CH3N���
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_2);		//tim2_ch4
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR�Ĵ���ֵ
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //Ԥ��Ƶֵ
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //������Ƶֵ
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //����ģʽ
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 60;                                         //���ô�װ�벶��ȽϼĴ���������ֵ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = (Period/2-60);                              //60:Period/2��50US����ȥ60��Ϊ�˸��õĲɼ���ˢ���������2022-06-09
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);                                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_OCInitStructure.TIM_Pulse = 0;    
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);		
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);                           //CH4Ԥװ��ʹ��	 

	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Enable);//TIM_TRGOSource_Update
    //Master-slave mode MSM
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void TIM1_PWM_Init(u16 Prescaler,u32 Period)
{
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_StructInit.TIM_Period=Period;                                                                      //ARR�Ĵ���ֵ
	TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //Ԥ��Ƶֵ
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //������Ƶֵ
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //����ģʽ
	TIM_StructInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_StructInit);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 120;                                          //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = 600;    
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);                                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OCInitStructure.TIM_Pulse = 1200;    
//	TIM_OC3Init(TIM1, &TIM_OCInitStructure);                                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	
//	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);                           //CH4Ԥװ��ʹ��	 

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
**������Ϣ ��void SysTick_init(uint16_t uiHz)
**�������� ��systick��ʱ������ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void SysTick_init(uint16_t uiHz)
{
    if (SysTick_Config(SystemCoreClock / uiHz))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick�ж����ȼ�����
}


/********************************************************************************************************
**������Ϣ ��SysTick_Handler(void)
**�������� ��������жϺ�����Systick���еݼ� 1ms�ж�һ��
**������� ����
**������� ����
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

