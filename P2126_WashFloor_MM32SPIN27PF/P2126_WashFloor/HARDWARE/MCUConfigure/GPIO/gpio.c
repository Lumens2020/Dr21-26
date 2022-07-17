#include "adc.h"
#include "key.h"
#include "dry.h"
#include "gpio.h"
#include "uart.h"
#include "init.h"
#include "timer.h"
#include "voice.h"
#include "brush.h"
#include "eeprom.h"
#include "sewage.h"
#include "control.h"
#include "waterpump.h"
#include "waterelec.h"
#include "cleanwater.h"

void IO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE);
	
	//--------------------------GPIO Output Setting----------------------------
		
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
//	    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_7;
	    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	    GPIO_Init(GPIOD, &GPIO_InitStructure);
	//--------------------------GPIO Input Setting------------------------------
	
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12;//|GPIO_Pin_11|;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	  GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
				
			
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6,GPIO_AF_4); 	
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,GPIO_AF_4); 		
//		Enable_OP_PIN(1);
		Enable_OP_PIN(0);
		Enable_IRLED_PIN(1);
		pbVOICE_OPEN;
//		pbVOICE_CLOSE;
}

