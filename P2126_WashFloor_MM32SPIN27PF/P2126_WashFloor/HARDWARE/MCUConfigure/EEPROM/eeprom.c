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
#include "error.h"
#include "sewage.h"
#include "control.h"
#include "waterpump.h"
#include "waterelec.h"
#include "cleanwater.h"
#include "voice.h"

Eeprom_TypeDef E2prom;
extern VoicePara Voice;
extern KeyPara        KeyVar;
extern WorkError      ErrorVar;

void Read_EEPROM(void)                                
{
    uint16_t num=256;
    uint16_t *data=&E2prom.uiReadDataFlash[0];
	uint16_t *temp_addr = (uint16_t *)(0x08000000 + Page * 1024);
	
	while(num --)
	{
		*data ++ = *temp_addr ++;
	}
	if(E2prom.uiReadDataFlash[EE_HEADER]==EEPROM_HEADADD)
	{
		if(E2prom.uiReadDataFlash[EE_VOICEFUN]>0)Voice.ucFunctionValue= E2prom.uiReadDataFlash[EE_VOICEFUN]; 
		else Voice.ucFunctionValue= E2prom.uiReadDataFlash[EE_VOICEFUN]; 
		
		Voice.ucLanguageCount 		             = E2prom.uiReadDataFlash[EE_LANGUAGE];
		E2prom.uiWriteDataFlash[EE_VOICEFUN] 	 = E2prom.uiReadDataFlash[EE_VOICEFUN]; 
		E2prom.uiWriteDataFlash[EE_LANGUAGE] 	 = E2prom.uiReadDataFlash[EE_LANGUAGE];		
		
		/*认为下面两行多余，2022-06-19*/
		E2prom.uiWriteDataFlash[EE_VOICEFUN]	 = E2prom.uiWriteDataFlash[EE_VOICEFUN]; 
		E2prom.uiWriteDataFlash[EE_LANGUAGE]	 = E2prom.uiWriteDataFlash[EE_LANGUAGE];	
		
#if TEST_AD_ENABLE==1
        E2prom.uiWriteDataFlash[WARN_CNT]	 = E2prom.uiReadDataFlash[WARN_CNT]; 
        E2prom.uiWriteDataFlash[WARN_TYPE0]	 = E2prom.uiReadDataFlash[WARN_TYPE0]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterPump0]	 = E2prom.uiReadDataFlash[WARN_AD_WaterPump0]; 	
        E2prom.uiWriteDataFlash[WARN_AD_Brush0]	 = E2prom.uiReadDataFlash[WARN_AD_Brush0]; 
        E2prom.uiWriteDataFlash[WARN_AD_Key0]	 = E2prom.uiReadDataFlash[WARN_AD_Key0]; 	
        E2prom.uiWriteDataFlash[WARN_AD_SewageLV0]	 = E2prom.uiReadDataFlash[WARN_AD_SewageLV0];    //污水
        E2prom.uiWriteDataFlash[WARN_AD_SewageStage0]	 = E2prom.uiReadDataFlash[WARN_AD_SewageStage0]; 	
        E2prom.uiWriteDataFlash[WARN_AD_CHGVol0]	 = E2prom.uiReadDataFlash[WARN_AD_CHGVol0]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterLV0]	 = E2prom.uiReadDataFlash[WARN_AD_WaterLV0]; 	
        E2prom.uiWriteDataFlash[WARN_TYPE1]	 = E2prom.uiReadDataFlash[WARN_TYPE1]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterPump1]	 = E2prom.uiReadDataFlash[WARN_AD_WaterPump1]; 	
        E2prom.uiWriteDataFlash[WARN_AD_Brush1]	 = E2prom.uiReadDataFlash[WARN_AD_Brush1]; 
        E2prom.uiWriteDataFlash[WARN_AD_Key1]	 = E2prom.uiReadDataFlash[WARN_AD_Key1]; 	
        E2prom.uiWriteDataFlash[WARN_AD_SewageLV1]	 = E2prom.uiReadDataFlash[WARN_AD_SewageLV1]; 
        E2prom.uiWriteDataFlash[WARN_AD_SewageStage1]	 = E2prom.uiReadDataFlash[WARN_AD_SewageStage1]; 	
        E2prom.uiWriteDataFlash[WARN_AD_CHGVol1]	 = E2prom.uiReadDataFlash[WARN_AD_CHGVol1]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterLV1]	 = E2prom.uiReadDataFlash[WARN_AD_WaterLV1]; 	
        E2prom.uiWriteDataFlash[WARN_TYPE2]	 = E2prom.uiReadDataFlash[WARN_TYPE2]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterPump2]	 = E2prom.uiReadDataFlash[WARN_AD_WaterPump2]; 	
        E2prom.uiWriteDataFlash[WARN_AD_Brush2]	 = E2prom.uiReadDataFlash[WARN_AD_Brush2]; 
        E2prom.uiWriteDataFlash[WARN_AD_Key2]	 = E2prom.uiReadDataFlash[WARN_AD_Key2]; 	
        E2prom.uiWriteDataFlash[WARN_AD_SewageLV2]	 = E2prom.uiReadDataFlash[WARN_AD_SewageLV2]; 
        E2prom.uiWriteDataFlash[WARN_AD_SewageStage2]	 = E2prom.uiReadDataFlash[WARN_AD_SewageStage2]; 	
        E2prom.uiWriteDataFlash[WARN_AD_CHGVol2]	 = E2prom.uiReadDataFlash[WARN_AD_CHGVol2]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterLV2]	 = E2prom.uiReadDataFlash[WARN_AD_WaterLV2]; 	
        E2prom.uiWriteDataFlash[WARN_TYPE3]	 = E2prom.uiReadDataFlash[WARN_TYPE3]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterPump3]	 = E2prom.uiReadDataFlash[WARN_AD_WaterPump3]; 	
        E2prom.uiWriteDataFlash[WARN_AD_Brush3]	 = E2prom.uiReadDataFlash[WARN_AD_Brush3]; 
        E2prom.uiWriteDataFlash[WARN_AD_Key3]	 = E2prom.uiReadDataFlash[WARN_AD_Key3]; 	
        E2prom.uiWriteDataFlash[WARN_AD_SewageLV3]	 = E2prom.uiReadDataFlash[WARN_AD_SewageLV3]; 
        E2prom.uiWriteDataFlash[WARN_AD_SewageStage3]	 = E2prom.uiReadDataFlash[WARN_AD_SewageStage3]; 	
        E2prom.uiWriteDataFlash[WARN_AD_CHGVol3]	 = E2prom.uiReadDataFlash[WARN_AD_CHGVol3]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterLV3]	 = E2prom.uiReadDataFlash[WARN_AD_WaterLV3]; 	
        E2prom.uiWriteDataFlash[WARN_TYPE4]	 = E2prom.uiReadDataFlash[WARN_TYPE4]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterPump4]	 = E2prom.uiReadDataFlash[WARN_AD_WaterPump4]; 	
        E2prom.uiWriteDataFlash[WARN_AD_Brush4]	 = E2prom.uiReadDataFlash[WARN_AD_Brush4]; 
        E2prom.uiWriteDataFlash[WARN_AD_Key4]	 = E2prom.uiReadDataFlash[WARN_AD_Key4]; 	
        E2prom.uiWriteDataFlash[WARN_AD_SewageLV4]	 = E2prom.uiReadDataFlash[WARN_AD_SewageLV4]; 
        E2prom.uiWriteDataFlash[WARN_AD_SewageStage4]	 = E2prom.uiReadDataFlash[WARN_AD_SewageStage4]; 	
        E2prom.uiWriteDataFlash[WARN_AD_CHGVol4]	 = E2prom.uiReadDataFlash[WARN_AD_CHGVol4]; 
        E2prom.uiWriteDataFlash[WARN_AD_WaterLV4]	 = E2prom.uiReadDataFlash[WARN_AD_WaterLV4]; 	
		ErrorVar.xpErrorMemoryCnt=(uint8_t)E2prom.uiWriteDataFlash[WARN_CNT];
#else

#endif

		if((Voice.ucFunctionValue==0)||(Voice.ucFunctionValue==2))
			{
//			Enable_OP_PIN(1);
			Voice.bVolumeOFFFlag=0;
			Voice.uiVolumePromptValue=VLOUME_HIGH_VALUE;;//VOLUME_ADJUSTMENT_7;   //音量提示
			Voice.uiPreVolumePromptValue=0;
			Voice.bVolumeChangeFlag=1;
			Voice.bVolumePlayChangeFlag=0;// 20220113
			}
		else if(Voice.ucFunctionValue==1)
			{
//			Enable_OP_PIN(0);
			Voice.bVolumePlayChangeFlag=0;// 20220113
			Voice.bVolumeOFFFlag=1;
			Voice.uiVolumePromptValue=VOLUME_ADJUSTMENT_OFF;  //关闭语音
			Voice.uiPreVolumePromptValue=0;
			}
		else if(Voice.ucFunctionValue==3)
			{
//			Enable_OP_PIN(1);
			Voice.bVolumeOFFFlag=0;
			Voice.uiVolumePromptValue=VLOUME_LOW_VALUE;
			Voice.uiPreVolumePromptValue=0;                  //前期音量提示值
			Voice.bVolumeChangeFlag=1;
			Voice.bVolumePlayChangeFlag=0;// 20220113
			}
		else
			{
			Voice.ucFunctionValue=3;
			Voice.ucLanguageCount=0;
			Voice.bVolumeOFFFlag=0;
			Voice.uiVolumePromptValue=VLOUME_LOW_VALUE;
			Voice.uiPreVolumePromptValue=0;
			Voice.bVolumeChangeFlag=1;
			Voice.bVolumePlayChangeFlag=0;// 20220113
			}
	}
	else
		{		
		Voice.ucFunctionValue=3;
		Voice.ucLanguageCount=0;
		Voice.bVolumeOFFFlag=0;
		Voice.uiVolumePromptValue=VLOUME_LOW_VALUE;
		Voice.uiPreVolumePromptValue=0;
		Voice.bVolumeChangeFlag=1;
		Voice.bVolumePlayChangeFlag=0;// 20220113
		}
		
	KeyVar.bFastVoiceFlag = 1;
	Voice.ucSendVoiceOVcnt=0;
	E2prom.bEnableEEpromFlag=1;
}


void Write_EEPROM(void)                                
{
    uint16_t num=256;
	uint16_t *data=&E2prom.uiWriteDataFlash[0];
	FLASH_Status temp_stat;
	uint32_t temp_addr = 0x08000000 + Page * 1024; //第31	页注意：所有STfoc系统要求program size中必须Code+Rodata<31744否则将导致不确定后果


    if(E2prom.bEnableEEpromFlag)
    	{
    	E2prom.bEEpromRunFlag=1;
		
		__disable_irq();// 20220228
		FLASH_Unlock();								 // Flash解锁，允许操作相关的寄存器
		temp_stat = FLASH_ErasePage(temp_addr);     // 擦除指定的页
		if(temp_stat != FLASH_COMPLETE)
		{
	    FLASH_Lock();
		}
		while(num --)
		{
			temp_stat = FLASH_ProgramHalfWord(temp_addr,*data);	//FLASH_ProgramHalfWord(temp_addr,*data);
			if(temp_stat != FLASH_COMPLETE)
			{
					FLASH_Lock();
			}
			temp_addr += 2;
			data++;
		}
	//	for(num=256;num>0;num--){	uiWriteDataFlash[num-1]=0;}
	    FLASH_Lock();        	
	    __enable_irq();// 20220228
		E2prom.bEnableEEpromFlag = 0;		
    	E2prom.bEEpromRunFlag=0;
    	}
}





