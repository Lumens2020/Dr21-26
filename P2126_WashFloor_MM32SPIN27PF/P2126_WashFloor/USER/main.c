/*****************************************************************************************************
追觅科技

产品名称: 手持洗地机
产品型号: P2126
产品客户: MOVA
MCU     : MM32SPIN27PF
频率    : 内振96MHz
复位    : 内部上电复位和低电压复位
保护    : LOCK

当前版本: 2.4
作者    : 潘星星
开始日期: 2021年08月04日
完成日期: 

取代版本: 2.3
原作者  : 潘星星
开始日期: 2021年07月22日
完成日期: 无


更改时间：20220322
更改人：魏衍通
更改版本:V2.2
更改标记：/////////////////////////////20220322//////////////////////////////////////
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
	NVIC_SystemReset();//复位 //software reset 
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
	     Eimination_AdValue(&ADCVar);						// AD采样消抖
		   TimeVar.bTmr200usFlag=0;
//		 pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr500usFlag)
		{// 
		 Check_SewageStage1(&WaterBSta,&SewageVar,&ADCVar);   // 污水箱水位检测
		 TimeVar.bTmr500usFlag=0;
//		 pbPtc_COMM_TOGGLE;
		}
	  if(TimeVar.bTmr01msFlag)
	  	{	
		  if(TimeVar.bPowerOnmsFlag)
			  {// 上电1S后开始处理			  
			  button_ticks();                                      // IO口按键处理
			  ADbutton_ticks();                                    // AD按键处理
			  Play_WT588F(&KeyVar,&Voice,&E2prom,&VoiceListpara);  // 语音处理
			  }
		    Check_SewageLV(&ADCVar,&TimeVar,&SewageVar,&KeyVar);	   // 脏污检测
	  	 TimeVar.bTmr01msFlag=0;
//		 pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr02msFlag)
	  	{ 
		 ControlAndCheck_Load();                    // 负载检测控制处理
	  	 TimeVar.bTmr02msFlag=0;
//       pbPtc_COMM_TOGGLE;
	  	}
	  if(TimeVar.bTmr10msFlag)
	  	{
		 Deal_UartComm();                          // 串口通讯控制处理	 
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
			  MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoBLDCData.xpMcuToBldcBuff,BLDC_LENTH);//DMA1通道2,外设为串口2,存储器为ucMcuToBldcBuff,长BLDC_LENTH
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


