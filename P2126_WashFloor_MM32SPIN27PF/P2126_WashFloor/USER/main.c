/*****************************************************************************************************
׷�ٿƼ�

��Ʒ����: �ֳ�ϴ�ػ�
��Ʒ�ͺ�: P2126
��Ʒ�ͻ�: MOVA
MCU     : MM32SPIN27PF
Ƶ��    : ����96MHz
��λ    : �ڲ��ϵ縴λ�͵͵�ѹ��λ
����    : LOCK

��ǰ�汾: 2.4
����    : ������
��ʼ����: 2021��08��04��
�������: 

ȡ���汾: 2.3
ԭ����  : ������
��ʼ����: 2021��07��22��
�������: ��


����ʱ�䣺20220322
�����ˣ�κ��ͨ
���İ汾:V2.2
���ı�ǣ�/////////////////////////////20220322//////////////////////////////////////
***************************************************************************************************/
#include "main.h"

extern KeyPara        KeyVar;
extern VoicePara      Voice;
extern Eeprom_TypeDef E2prom;
extern ADCPara        ADCVar; 
extern TimePara       TimeVar;
extern VoiceList      VoiceListpara;
extern WorkPara       WorkVar;
extern SewagePara     SewageVar;
extern WaterBoxStatus WaterBSta;
extern FCT_UART1Comm  FctUART1COMMpara;

extern RccReadPara RccTestPara;

extern MCUtoBLDCData  MtoBLDCData;
extern MCUtoBMSData   MtoBMSData;

///*
void HardFault_Handler(void)
{  
	NVIC_SystemReset();//��λ //software reset 
} 
//*/

int main(void)
{
	PWR_PVDLevelConfig(PWR_PVDLevel_2V4);
	PWR_PVDCmd(ENABLE);
	MCU_Init();

	while(1)
	{	
///////////////////////////20220227///////////////////////////////

//	  Write_Iwdg_RL();
///*
	  if(TimeVar.bTmrSysFlag&&TimeVar.bTmrAdcFlag)
	  	{
		  Write_Iwdg_RL();
		  TimeVar.bTmrSysFlag=0;
		  TimeVar.bTmrAdcFlag=0;
	  	}
//*/	  	
//////////////////////////////////////////////////////////////////	  
	  if(TimeVar.bTmr200usFlag)
	  	{// 
	     Eimination_AdValue(&ADCVar);						// AD��������
		   TimeVar.bTmr200usFlag=0;
//		 pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr500usFlag)
		{// 
		 Check_SewageStage1(&WaterBSta,&SewageVar,&ADCVar);   // ��ˮ��ˮλ���
		 TimeVar.bTmr500usFlag=0;
//		 pbPtc_COMM_TOGGLE;
		}
	  if(TimeVar.bTmr01msFlag)
	  	{	
		  if(TimeVar.bPowerOnmsFlag)
			  {// �ϵ�1S��ʼ����			  
			  button_ticks();                                      // IO�ڰ�������
			  ADbutton_ticks();                                    // AD��������
			  Play_WT588F(&KeyVar,&Voice,&E2prom,&VoiceListpara);  // ��������
			  }
		    Check_SewageLV(&ADCVar,&TimeVar,&SewageVar,&KeyVar);	   // ���ۼ��
	  	 TimeVar.bTmr01msFlag=0;
//		 pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr02msFlag)
	  	{ 
		 ControlAndCheck_Load();                    // ���ؼ����ƴ���
	  	 TimeVar.bTmr02msFlag=0;
//       pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr10msFlag)
	  	{
		 Deal_UartComm();                          // ����ͨѶ���ƴ���	 
	  	 TimeVar.bTmr10msFlag=0;
//		 pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr100msFlag)
	    {
		  /////////////////20220226/////////////////////
		  RccTestPara.Rcc_CR=RCC->CR;
		  RccTestPara.Rcc_CFGR=RCC->CFGR;
		  RccTestPara.Rcc_CR_PLLMUL=(uint8_t)((RccTestPara.Rcc_CR&0xFC000000)>>26);
		  RccTestPara.Rcc_CR_PLLON=(uint8_t)((RccTestPara.Rcc_CR&0x01000000)>>24);
		  RccTestPara.Rcc_CR_PLLDIV=(uint8_t)((RccTestPara.Rcc_CR&0x00700000)>>20);
		  RccTestPara.Rcc_CFGR_SWSSW=(uint8_t)(RccTestPara.Rcc_CFGR&0x0000000F);
///*
		  if((RccTestPara.Rcc_CR_PLLMUL!=0x03)||(RccTestPara.Rcc_CR_PLLON!=0x01)||(RccTestPara.Rcc_CFGR_SWSSW!=0x0A))
			  {
			  __disable_irq();
			  SystemInit();
			  IO_Init();
			  TIM2_PWM_Init(TIM2_PRESCAL,BRUSH_PERIOD);    
			  TIM3_PWM_Init(TIM3_PRESCAL,PUMP_PERIOD);	
			  ADC1_Init();
			  ADC2_Init();
			  SysTick_init(1000);
			  UART1_Init(9600);
			  UART2_Init(9600);
			  MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoBLDCData.xpMcuToBldcBuff,BLDC_LENTH);//DMA1ͨ��2,����Ϊ����2,�洢��ΪucMcuToBldcBuff,��BLDC_LENTH
			  MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)MtoBMSData.xpMcuToBmsBuff,BMS_LENTH);  
			  Comp_Config();
			  Write_Iwdg_ON(IWDG_Prescaler_128,0x1f);//100ms 0xf);;
			  __enable_irq();
			  }
		  //*/
	     TimeVar.bTmr100msFlag=0;
//	   pbPtc_COMM_TOGGLE;
	    }
	  
	  if(TimeVar.bTmr500msFlag)
	  	{
		if(TimeVar.bPowerOn1sCount>=1000)
			{
			if(WorkVar.ucChangeStateCnt5==0&&FctUART1COMMpara.bTestModeFlag==0)
				{
		        Play_Service_ERROR();
		        Play_Running_ERROR();
		        Play_SOC_ERROR();
				}
			Write_EEPROM();
			}
	  	TimeVar.bTmr500msFlag=0;
//		pbPtc_COMM_TOGGLE;
	  	}
//	  pbTEST_COMM_TOGGLE;
	}
}

void PVD_IRQHandler(void)
{

    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* Clear the EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
        NVIC_SystemReset();
    }
		
}


