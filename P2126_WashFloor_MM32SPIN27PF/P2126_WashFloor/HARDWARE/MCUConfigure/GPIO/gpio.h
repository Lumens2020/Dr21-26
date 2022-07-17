#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define pbVOICE_OPEN		 GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define pbVOICE_CLOSE	     GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define pbSpk_VOICE			 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)


#define pbLED_Tr_OPEN		 GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define pbLED_Tr_CLOSE	     GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define pbWater_Level	     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
//#define pbWater_Level	     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define pbTilt_Key		     GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

//#define pbCleanW_Key	     GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)

#define CHG_PIN	 	 	     GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define DC_5V_EN	 	 	 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define pbKey_Spk		 	 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)


#define pbSpk_BUSY_ON	     GPIO_SetBits(GPIOB,GPIO_Pin_8)	
#define pbSpk_BUSY_OFF	     GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define pbSpk_BUSY	 	     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

#define pbSpk_SCL_ON	     GPIO_SetBits(GPIOB,GPIO_Pin_6)	
#define pbSpk_SCL_OFF	     GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define pbSpk_SCL			 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)

#define pbSpk_SDA_ON	     GPIO_SetBits(GPIOB,GPIO_Pin_7)	
#define pbSpk_SDA_OFF	     GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define pbSpk_SDA			 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

#define pbSpk_SHUT_ON	     GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define pbSpk_SHUT_OFF       GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define pbSpk_SHUT	 	     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#define pbFull_Test1_ON	     GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define pbFull_Test1_OFF     GPIO_ResetBits(GPIOC,GPIO_Pin_14)

#define pbFull_Test2_ON	     GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define pbFull_Test2_OFF     GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define pbPWM1	 	         GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
#define pbPWM1_ON			 GPIO_SetBits(GPIOD,GPIO_Pin_7)	
#define pbPWM1_OFF			 GPIO_ResetBits(GPIOD,GPIO_Pin_7)
#define pbPWM1_TOGGLE		 (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7))?(GPIO_ResetBits(GPIOD,GPIO_Pin_7)):(GPIO_SetBits(GPIOD,GPIO_Pin_7))
#define pbPWM2	 	         GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define pbPWM2_ON			 GPIO_SetBits(GPIOC,GPIO_Pin_13)	
#define pbPWM2_OFF			 GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define pbPWM2_TOGGLE		 (GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))?(GPIO_ResetBits(GPIOC,GPIO_Pin_13)):(GPIO_SetBits(GPIOC,GPIO_Pin_13))


#define pbPtc_COMM	 	     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define pbPtc_COMM_ON	     GPIO_SetBits(GPIOB,GPIO_Pin_10)	
#define pbPtc_COMM_OFF	     GPIO_ResetBits(GPIOB,GPIO_Pin_10)// 烘干通讯
#define pbPtc_COMM_TOGGLE	 (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10))?(GPIO_ResetBits(GPIOB,GPIO_Pin_10)):(GPIO_SetBits(GPIOB,GPIO_Pin_10))

#define pbTEST_COMM	 	     GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
#define pbTEST_COMM_ON	     GPIO_SetBits(GPIOD,GPIO_Pin_0)	
#define pbTEST_COMM_OFF	     GPIO_ResetBits(GPIOD,GPIO_Pin_0)// 烘干通讯
#define pbTEST_COMM_TOGGLE	 (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_0))?(GPIO_ResetBits(GPIOD,GPIO_Pin_0)):(GPIO_SetBits(GPIOD,GPIO_Pin_0))

#define pbW_EN_ON	     GPIO_SetBits(GPIOA,GPIO_Pin_7)	
#define pbW_EN_OFF	     GPIO_ResetBits(GPIOA,GPIO_Pin_7)// 电解水DCDC使能
#define pbFAULT	 	     GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)// 电解水故障反馈

void IO_Init(void);

#endif 

