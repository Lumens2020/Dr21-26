#include "adc.h"
#include "key.h"
#include "dry.h"
#include "gpio.h"
#include "uart.h"
#include "init.h"
#include "timer.h"
#include "voice.h"
#include "brush.h"
#include "bldc.h"
#include "eeprom.h"
#include "sewage.h"
#include "control.h"
#include "waterpump.h"
#include "waterelec.h"
#include "cleanwater.h"
#include "commbms.h"
#include "commbldc.h"
#include "commscreen.h"
#include "iwdg.h"
#include "compare.h"
#include "voice.h"

extern MCUtoBMSData  MtoBMSData;
extern MCUtoBLDCData  MtoBLDCData;
extern TimePara      TimeVar;
extern BMStoMCUData  BMStoMData;
extern ADCPara       ADCVar; 
extern KeyPara       KeyVar;

extern SewagePara     SewageVar;

extern Eeprom_TypeDef E2prom;
extern WaterBoxStatus WaterBSta;

extern BrushPara      BrushVar;
extern BLDCCtl        BLDCVar;
extern DryPara        DryVar;
extern WaterElecPara  WaterElecVar;
extern PumpPara       PumpVar;
extern WorkPara       WorkVar;
extern VoicePara      Voice;

void Delay_ms(uint32_t ms)
{
	uint32_t i;
	while(ms)
	{
		i=7800;
		while(i){i--;}
		ms--;
	}
}


void Init_Pamameter(void)
{
  ADCVar.V_adc_trans_count=0x83;   //0x83减少到0x80,消抖

	TimeVar.bTmr200usFlag=0;
	TimeVar.bTmr01msFlag=0;
	TimeVar.bTmr02msFlag=0;
	TimeVar.bTmr10msFlag=0;
	TimeVar.bPowerOnmsFlag=0;
	TimeVar.bTmr01msCount=0;
	TimeVar.bTmr02msCount=0;
	TimeVar.bTmr10msCount=0;
	TimeVar.bTmrRun02msCount=0;
	TimeVar.bTmrRun10msCount=0;
	TimeVar.bPowerOn1sCount=0;	

	BMStoMData.xpWorkStatus=0;
	BMStoMData.xpWarnStatus=0;
	BMStoMData.xpKeyOnOffStatus=0;
	BMStoMData.xpErrorStatus=0;
	BMStoMData.xpBatVolL=0;
	BMStoMData.xpBatVolH=0;
	BMStoMData.xpBatTempVal=0;
	BMStoMData.xpBatSocVal=0;
	BMStoMData.reserve0=0;
	BMStoMData.reserve1=0;

	KeyVar.bCleanKeyPressFlag = 0;
	KeyVar.bGearKeyPressFlag = 0;
	ADCVar.uiKeyAdcValue=4095;
	
	SewageVar.xpSewageLVcnt0=0;
	SewageVar.xpSewageLVcnt1=0;
	SewageVar.xpSewageChangeFlag=0;//20211012
	SewageVar.xpSewageChangecnt=0;

	WaterBSta.ucQBucketStatus = QBULET_FULL_STA;
	E2prom.uiWriteDataFlash[EE_HEADER]=EEPROM_HEADADD;
	WorkVar.ucFirstONFlag=0;// 20211108

	Voice.off_op_cnt=0;// 2021115
	Voice.on_op_cnt=0;// 2021115
		
	Set_Bldc_OFF(&BLDCVar);
	Set_Brush_OFF(&BrushVar);
	Set_WaterPump_OFF(&PumpVar);
	Set_Dry_OFF(&DryVar);
	Set_WaterElec_OFF(&WaterElecVar);
}

void MCU_Init(void)
{
 Delay_ms(30);	
 IO_Init();
 TIM2_PWM_Init(TIM2_PRESCAL,BRUSH_PERIOD); 	 
// TIM1_PWM_Init(TIM1_PRESCAL,TMR1_PERIOD); 
 TIM3_PWM_Init(TIM3_PRESCAL,PUMP_PERIOD);  
 ADC1_Init();
 ADC2_Init();
 SysTick_init(1000);
 UART1_Init(9600);
 UART2_Init(9600);
 MYDMA_Config(DMA1_Channel4,(u32)&UART2->TDR,(u32)MtoBLDCData.xpMcuToBldcBuff,BLDC_LENTH);//DMA1通道2,外设为串口2,存储器为ucMcuToBldcBuff,长BLDC_LENTH
 MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)MtoBMSData.xpMcuToBmsBuff,BMS_LENTH);	 
 Comp_Config();
 KEY_Init();
 Init_Pamameter();
 Read_EEPROM();
	
// Write_Iwdg_ON(IWDG_Prescaler_128,0x1f);//100ms 0xf);;
}

