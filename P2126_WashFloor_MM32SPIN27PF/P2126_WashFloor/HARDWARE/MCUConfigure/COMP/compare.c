#include"compare.h"
#include"brush.h"
#include"waterpump.h"

extern BrushPara      BrushVar;
extern PumpPara       PumpVar;

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTIX config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                      //Enable SYSCFGEN

//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling;                     //Falling Trigger
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line19;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling;                     //Falling Trigger
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = COMP1_2_3_4_5_IRQn;                          //Irq config
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //Priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //Enable Irq
    NVIC_Init(&NVIC_InitStructure);

}

void Comp_Config(void)//(uint32_t COMP_Selection_COMPx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	COMP_InitTypeDef COMP_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_CPT, ENABLE); 
//COMP4
	COMP_DeInit(COMP_Selection_COMP4);
	COMP_InitStructure.COMP_InvertingInput	  = COMP_InvertingInput_CRV;//COMP_InvertingInput_CRV;//COMP_InvertingInput_IO2;
	COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO2; //for PB2 waterpump
	COMP_InitStructure.COMP_Output			  = COMP_Output_None;//COMP_Output_TIM1BKIN;//COMP_Output_TIM1BKIN;//COMP_Output_None;
	COMP_InitStructure.COMP_OutputPol		  = COMP_OutputPol_NonInverted;//COMP_OutputPol_Inverted;//COMP_OutputPol_NonInverted;
	COMP_InitStructure.COMP_Hysteresis		  = COMP_Hysteresis_Medium;//COMP_Hysteresis_Low;//COMP_Hysteresis_No;
	COMP_InitStructure.COMP_Mode			  = COMP_Mode_MediumSpeed;//COMP_Mode_MediumSpeed;//COMP_Mode_UltraLowPower;
	COMP_InitStructure.COMP_Filter			  = COMP_Filter_4_Period;//COMP_Filter_64_Period;

	COMP_Init(COMP_Selection_COMP4, &COMP_InitStructure);
	
//COMP5
	COMP_DeInit(COMP_Selection_COMP5);
	COMP_InitStructure.COMP_InvertingInput	  = COMP_InvertingInput_CRV;//COMP_InvertingInput_CRV;//COMP_InvertingInput_IO2;
	COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO1; //for PA6 brush
	COMP_InitStructure.COMP_Output			  = COMP_Output_None;//COMP_Output_TIM1BKIN;//COMP_Output_TIM1BKIN;//COMP_Output_None;
	COMP_InitStructure.COMP_OutputPol		  = COMP_OutputPol_NonInverted;//COMP_OutputPol_Inverted;//COMP_OutputPol_NonInverted;
	COMP_InitStructure.COMP_Hysteresis		  = COMP_Hysteresis_Medium;//COMP_Hysteresis_Low;//COMP_Hysteresis_No;
	COMP_InitStructure.COMP_Mode			  = COMP_Mode_MediumSpeed;//COMP_Mode_MediumSpeed;//COMP_Mode_UltraLowPower;
	COMP_InitStructure.COMP_Filter			  = COMP_Filter_4_Period;//COMP_Filter_64_Period;

	COMP_Init(COMP_Selection_COMP5, &COMP_InitStructure);

//CRV
	SET_COMP_CRV(COMP_CRV_Sele_AVDD, COMP_CRV_Level_9_20);//(3_24) real is 1/20 of 5V = 0.25V,(6_24) real is 4/20 of 5V = 1V
//SET_COMP_CRV(COMP_CRV_Sele_AVDD, COMP_CRV_Level_6_20);//(3_24) real is 1/20 of 5V = 0.25V,(6_24) real is 4/20 of 5V = 1V

//ÖÐ¶Ï
	EXTI_ClearITPendingBit(EXTI_Line20);
	EXTI_ClearITPendingBit(EXTI_Line19);
	EXTIX_Init();
//Ê¹ÄÜ	
	COMP_Cmd(COMP_Selection_COMP4, ENABLE);
	COMP_Cmd(COMP_Selection_COMP5, ENABLE);
	
		
}


/*******************************************************************************
* @name   : Comp_StatusCheck
* @brief  : Check COMP status
* @param  : COMP_Selection_COMPx
* @retval : void
*******************************************************************************/
unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx)
{
    unsigned char chCapStatus = 2;
    if(COMP_GetOutputLevel(COMP_Selection_COMPx) == 0)
    {
        chCapStatus = 0;
    }
    else
    {
        chCapStatus = 1;
    }
    return chCapStatus;
}

void COMP1_2_3_4_5_IRQHandler(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(EXTI_GetITStatus(EXTI_Line20) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line20);
		if(Comp_StatusCheck(COMP_Selection_COMP5)==1)
			{
			BRUSH_PWM= 0;	
//			if(BrushVar.bBrushResetFlag==0)
//			if(BrushVar.bBrushShortErrFlag==0)
//				{
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_0);
				GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_11;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_ResetBits(GPIOB,GPIO_Pin_11);			
//				}
//			  BrushVar.bBrushShortErrFlag=1;
			 BrushVar.bBrushResetFlag=1;
	   	    }
       // Comp_StatusCheck(COMP_Selection_COMP5);
    }
	
	if(EXTI_GetITStatus(EXTI_Line19) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line19);
		if(Comp_StatusCheck(COMP_Selection_COMP4)==1)
			{
		    WaterPump_PWM= 0;	
//			if(PumpVar.bPumpResetFlag==0)
//			if(PumpVar.bPumpShortErrFlag==0)
//				{
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_7);
				GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
//				}
//			PumpVar.bPumpShortErrFlag=1;
			PumpVar.bPumpResetFlag=1;
			}
		//Comp_StatusCheck(COMP_Selection_COMP4);
    }
}







