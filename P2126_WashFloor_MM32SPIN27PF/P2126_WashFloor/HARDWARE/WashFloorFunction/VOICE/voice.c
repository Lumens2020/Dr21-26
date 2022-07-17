#include "key.h"
#include "gpio.h"
#include "voice.h"
#include "voicelist.h"
#include "eeprom.h"

//****************语音运放控制脚使能函数*******************
///*功能：      10ms执行一次 语音运放控制脚使能控制		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void Enable_OP_PIN(uint8_t enableoppin)
{
 if(enableoppin>0)
 	{
	 pbSpk_SHUT_ON;
 	}
 else
 	{
	 pbSpk_SHUT_OFF;
 	}
}

//****************运放控制脚电平读取函数*******************
///*功能：      10ms执行一次 读取运放脚电平		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************
uint8_t Read_OP_PIN(void)
{
 return (uint8_t)pbSpk_SHUT;
}

//****************语音模块SDA脚电平控制函数*******************
///*功能：      10ms执行一次 控制语音模块通讯SDA高低电平		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void SET_SDA_PIN(uint8_t enablesdapin)
{
 if(enablesdapin>0)
 	{
	 pbSpk_SDA_ON;
 	}
 else
 	{
	 pbSpk_SDA_OFF;
 	}
}

//****************语音模块SCL脚电平控制函数*******************
///*功能：      10ms执行一次 控制语音模块通讯SCL高低电平		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************

void SET_SCL_PIN(uint8_t enablesclpin)
{
 if(enablesclpin>0)
 	{
	 pbSpk_SCL_ON;
 	}
 else
 	{
	 pbSpk_SCL_OFF;
 	}
}
//****************语音模块BUSY脚电平读取函数*******************
///*功能：      10ms执行一次 读取语音模块通讯BUSY脚电平		 						*///
///*入口参数：	无																*///
///*出口参数：	无																				*///
//************************************************************
uint8_t Read_VoiceBusy_PIN(void)
{
 return (uint8_t)pbSpk_BUSY;
}
//****************语音模块通讯传输函数*******************
///*功能：      200us执行一次 语音模块的数据传输逻辑处理		 						*///
///*入口参数：	KeyPara		VoicePara														*///
///*出口参数：	无																				*///
//************************************************************
/*
void Line_2A_WT588F(KeyPara* keyparactl,VoicePara* voiceparactl)
{// 200us
	static uint8_t j;
	static uint8_t num_temp;
	static uint8_t voice_cnt1,voice_step,voice_num,voice_first;
	static uint16_t ddata_temp,pdata_temp;
	if(voice_step==0)
		{
		ddata_temp=voiceparactl->WT588F_data[voice_num];
		pdata_temp = ddata_temp& 0x00FF;
		ddata_temp >>= 8;
		pdata_temp <<= 8;
		ddata_temp |= pdata_temp;
		num_temp =16;
		j=0;
		voice_first=0;
		SET_SCL_PIN(0);
		if(++voice_cnt1>24)
			{
			voice_cnt1=0;
			voice_step=1;
			}
		}
	if(voice_step==1)
		{
			if((j==8)&&(voice_first==0))
				{
				SET_SCL_PIN(1);
				SET_SDA_PIN(1);
				if(++voice_cnt1>9)
					{
					voice_step=4;
					}
				}
			else
				{
				SET_SCL_PIN(0);
				if(ddata_temp&0x0001)SET_SDA_PIN(1);
				else SET_SDA_PIN(0);
				voice_step=2;
				}
		}
	else if(voice_step==2)
		{
		    SET_SCL_PIN(1);
			voice_step=3;
		}
	else if(voice_step==3)
		{
			ddata_temp=ddata_temp>>1;
			if(j<num_temp)
				{
				j++;
				voice_step=1;
				}
			else
				{
				voice_step=5;
				}
		}
	else if(voice_step==4)
		{
		SET_SCL_PIN(0);
		if(++voice_cnt1>23)
			{//延时 5ms	 
			voice_cnt1=0;
			voice_step=1;
			voice_first=1;
			}
		}
	if(voice_step==5)
		{
		SET_SCL_PIN(1);
		SET_SDA_PIN(1);
		voice_step=0;
		if(voice_num<voiceparactl->WT588F_step)
			{
			voice_num++;
			}
		else
			{
			keyparactl->bFastVoiceFlag = 0;
			
			if(voiceparactl->uiVolumePromptValue==voiceparactl->uiPreVolumePromptValue)
				{
				if(voiceparactl->uiTonePromptValue==PROMPT_VOLUME_LOW_MP3)
					{
					voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_1;
					voiceparactl->uiPreVolumePromptValue=0;
					}
				else if(voiceparactl->uiTonePromptValue==PROMPT_VOLUME_HIGH_MP3)
					{
					voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_7;
					voiceparactl->uiPreVolumePromptValue=0;
					}
				else if(voiceparactl->uiTonePromptValue==PROMPT_CLOSE_VOLUME_MP3)
					{
					voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_OFF;
					voiceparactl->uiPreVolumePromptValue=0;
					}
				else if(voiceparactl->uiTonePromptValue==PROMPT_LANGUAGE_SUCCESS_MP3)
					{
					 if(voiceparactl->bVolumeOFFFlagback==1)
					 	{
						 voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_OFF;
						 voiceparactl->uiPreVolumePromptValue=0;
					 	}
					}
				}
			else
				{
				voiceparactl->uiVolumePromptValue=0;
				voiceparactl->uiPreVolumePromptValue=0;
				}
			voiceparactl->uiTonePromptValue=0;
			voiceparactl->uiPreTonePromptValue=voiceparactl->uiTonePromptValue;
			voiceparactl->uiSecondTonePromptValue = 0;
			voiceparactl->uiThirdTonePromptValue  = 0;
			voiceparactl->ucLanguageValue = voiceparactl->ucKeyLanguageValue;
			voice_num=0;
			}
		}
}
*/
void Line_2A_WT588F(KeyPara* keyparactl,VoicePara* voiceparactl)
{// 200us
	static uint8_t j;
	static uint8_t num_temp;
	static uint8_t voice_cnt1,voice_step,voice_num,voice_first;
	static uint16_t ddata_temp,pdata_temp;
	static uint16_t xpVoicetemp1,xpVoicetemp2;
	if(voice_step==0)
		{
		ddata_temp=voiceparactl->WT588F_data[voice_num];
		pdata_temp = ddata_temp& 0x00FF;
		ddata_temp >>= 8;
		pdata_temp <<= 8;
		ddata_temp |= pdata_temp;
		num_temp =15;
		j=0;
		voice_first=0;

		if(voiceparactl->WT588F_step>1&&(voice_num==3||voice_num==5))
			{// 连码播报，FFF3+地址间隔下一条要延时大于10MS
			if(voice_cnt1<(6000/VOICE_TICKS_INTERVAL))
//				if(voice_cnt1<30)
				{
				SET_SCL_PIN(1);
				++voice_cnt1;
				}
			else if(voice_cnt1<(12000/VOICE_TICKS_INTERVAL))
//				else if(voice_cnt1<60)
				{
				SET_SCL_PIN(0);
				++voice_cnt1;
				}
			else
				{
				voice_cnt1=0;
				voice_step=1;
				}
			}
		else
			{
			if(voice_cnt1<(1000/VOICE_TICKS_INTERVAL))
//				if(voice_cnt1<5)
				{
				SET_SCL_PIN(1);
				++voice_cnt1;
				}
			else if(voice_cnt1<(7000/VOICE_TICKS_INTERVAL))
//				else if(voice_cnt1<35)
				{
				SET_SCL_PIN(0);
				++voice_cnt1;
				}
			else
				{
				voice_cnt1=0;
				voice_step=1;
				}
			}
		
		if((voiceparactl->bVolumeChangeFlag==0)&&(voiceparactl->uiPreTonePromptValue!=voiceparactl->uiTonePromptValue))
			{
		    if(voiceparactl->off_op_cnt==0)voiceparactl->off_op_cnt=1;
//		    if(voiceparactl->on_op_cnt==0)voiceparactl->on_op_cnt=1;
		    voiceparactl->on_op_cnt=1;// 20220114
			}
		}
	else if(voice_step==1)
		{
		  if((j==8)&&(voice_first==0))
				{
				SET_SCL_PIN(1);
				SET_SDA_PIN(1);
				if(++voice_cnt1>(1000/VOICE_TICKS_INTERVAL))
//					if(++voice_cnt1>5)
					{
					voice_step=4;
					voice_cnt1=0;
					}
				}
			else
				{
				SET_SCL_PIN(0);
				if(ddata_temp&0x0001)SET_SDA_PIN(1);
				else SET_SDA_PIN(0);
				ddata_temp=ddata_temp>>1;
				if(j<num_temp)
					{
					j++;
					voice_step=2;
					}
				else
					{
					voice_step=5;
					}
				}
		}
	else if((voice_step==2)||(voice_step==3))
		{
		    SET_SCL_PIN(1);
			voice_step=1;
		}
	else if(voice_step==4)
		{
		SET_SCL_PIN(0);
		if(++voice_cnt1>(6000/VOICE_TICKS_INTERVAL))
//			if(++voice_cnt1>30)
			{//延时 5ms	 
			voice_cnt1=0;
			voice_step=1;
			voice_first=1;
			}
		}
   else if(voice_step==5)
		{
	    SET_SCL_PIN(1);
		if(voice_cnt1>0)
			{// 不加SDA最后一位会丢
			SET_SDA_PIN(1);
			}
		if(++voice_cnt1>(1000/VOICE_TICKS_INTERVAL))
//			if(++voice_cnt1>5)
//			if(++voice_cnt1>12)
			{//延时 1ms	 
			voice_cnt1=0;
			voice_step=0;
			if(voice_num<voiceparactl->WT588F_step)
				{
				voice_num++;
				}
			else
				{
				voiceparactl->bVolumeSendFlag=0;// 20220113
//////////////////////////////////////////////////////////////////				
				if(voiceparactl->uiTonePromptValue>=0x10)xpVoicetemp1 = voiceparactl->uiTonePromptValue+(voiceparactl->ucLanguageCount*0x34);
				else xpVoicetemp1 =voiceparactl->uiTonePromptValue;
				xpVoicetemp2 = voiceparactl->uiSecondTonePromptValue+(voiceparactl->ucLanguageCount*0x34);
/////////////////////////////////////////////////////////////////
// 在发送非音量设置语音时，按语音按键，音量高低控制有大概率不起作用
				if(voiceparactl->uiVolumePromptValue==voiceparactl->uiPreVolumePromptValue)
					{// 语音播报发送完成
					if(voiceparactl->uiTonePromptValue==PROMPT_CLOSE_VOLUME_MP3)
						{// 关闭语音播报完成
//						voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_OFF;
//						voiceparactl->uiPreVolumePromptValue=0;
						voiceparactl->bVolumeOFFFlag=1;// 加入关闭语音标志，防止关闭语音播报时其他语音插入打断
						voiceparactl->bVolumePlayChangeFlag=0;// 20220113
						voiceparactl->uiPreVolumePromptValue =0;
						voiceparactl->uiVolumePromptValue=0;
						}
					else if(voiceparactl->uiTonePromptValue==PROMPT_LANGUAGE_SUCCESS_MP3)
						{// 设置语言完成后还原音量开关设置
						 if(voiceparactl->bVolumeOFFFlagback==1)
						 	{
							 voiceparactl->uiVolumePromptValue=VOLUME_ADJUSTMENT_OFF;
							 voiceparactl->uiPreVolumePromptValue=0;
						 	}
						}
////////////////////////////////////////////////////////////////
					if(voiceparactl->uiSecondTonePromptValue==0)
						{// 一条语音
						if((voiceparactl->WT588F_step==1)&&(xpVoicetemp1==voiceparactl->WT588F_data[1]))
							{// 命令发送完判断现在语音控制命令和发送是否一样，防止发送完毕清除其他语音
							voiceparactl->uiTonePromptValue=0;
							voiceparactl->uiPreTonePromptValue=voiceparactl->uiTonePromptValue;
							voiceparactl->uiSecondTonePromptValue = 0;
							voiceparactl->ucLanguageValue = voiceparactl->ucKeyLanguageValue;
							keyparactl->bFastVoiceFlag = 0;
							voiceparactl->bVolumePlayChangeFlag=0;// 20220113
							}
						}
					else 
						{// 两条语音
						if((voiceparactl->WT588F_step==4)&&(xpVoicetemp1==voiceparactl->WT588F_data[2])&&(xpVoicetemp2==voiceparactl->WT588F_data[4]))
							{// 命令发送完判断现在语音控制命令和发送是否一样，防止发送完毕清除其他语音
							voiceparactl->uiTonePromptValue=0;
							voiceparactl->uiPreTonePromptValue=voiceparactl->uiTonePromptValue;
							voiceparactl->uiSecondTonePromptValue = 0;
							voiceparactl->ucLanguageValue = voiceparactl->ucKeyLanguageValue;
							keyparactl->bFastVoiceFlag = 0;
							voiceparactl->bVolumePlayChangeFlag=0;// 20220113
							}
						}
////////////////////////////////////////////////////////////					
			    	}
				else
					{// 音量设置完成
					if(voiceparactl->WT588F_step==0)
						{
						voiceparactl->bVolumeChangeFlag=0;
						voiceparactl->uiVolumePromptValue=0;
						voiceparactl->uiPreVolumePromptValue=0;
						}
					}
				voice_num=0;
				}
			}
		}
}
//****************语音模块通讯数据处理函数*******************
///*功能：      200us执行一次 语音模块的通讯数据处理		 						*///
///*入口参数：	KeyPara		VoicePara	 Eeprom_TypeDef													*///
///*出口参数：	无																				*///
//************************************************************

void Play_WT588F(KeyPara* keyparactl,VoicePara* voiceparactl,Eeprom_TypeDef* eepromparactl,VoiceList* voicelistflag)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	static uint16_t uiVoicetemp1,uiVoicetemp2;//,close_voice_cnt,open_voice_cnt;
	
if(pbSpk_VOICE==0)
{
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	pbSpk_BUSY_OFF;
	SET_SDA_PIN(0);
	SET_SCL_PIN(0);
	
	if(++voiceparactl->open_voice_cnt>(500000/VOICE_TICKS_INTERVAL))// 500ms
	   {
		pbVOICE_OPEN;
		voiceparactl->open_voice_cnt=0;
	   }
}
else
{
    GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	voiceparactl->open_voice_cnt=0;
	
	if((keyparactl->bFastVoiceFlag==1)||(Read_VoiceBusy_PIN()==1))
	  {	  
	   Play_VoiceList(voicelistflag,voiceparactl,keyparactl);
	   ///////////////////////////////开启语音控制
	   if(voiceparactl->uiTonePromptValue==PROMPT_OPEN_VOLUME_MP3)
		   {
		   voiceparactl->bVolumeOFFFlag=0;
		   }
	   //////////////////////////////关闭语音判断 设置语言时不关闭语音
	   if((voiceparactl->bVolumeOFFFlag==1)&&(((voiceparactl->uiTonePromptValue>PROMPT_RESV4_MP3)&&(voiceparactl->uiTonePromptValue<PROMPT_VOLUME_LOW_MP3))\
	   	||(voiceparactl->uiTonePromptValue>PROMPT_LANGUAGE_SUCCESS_MP3)))
	   	{
		voiceparactl->uiTonePromptValue       = 0;
		voiceparactl->uiPreTonePromptValue    = voiceparactl->uiTonePromptValue;
		voiceparactl->uiSecondTonePromptValue = 0;
		voiceparactl->ucLanguageValue         = voiceparactl->ucKeyLanguageValue;
	   	}
	   ///////////////////////////////
	   if((voiceparactl->bVolumeChangeFlag==0)&&(voiceparactl->uiPreTonePromptValue!=voiceparactl->uiTonePromptValue))
		{
			if(voiceparactl->uiTonePromptValue>=0x10)uiVoicetemp1 = voiceparactl->uiTonePromptValue+(voiceparactl->ucLanguageCount*0x34);
			else uiVoicetemp1 =voiceparactl->uiTonePromptValue;
			uiVoicetemp2 = voiceparactl->uiSecondTonePromptValue+(voiceparactl->ucLanguageCount*0x34);

			if(voiceparactl->bVolumeSendFlag==0)
				{
				voiceparactl->bVolumeSendFlag=1;
				if(voiceparactl->uiSecondTonePromptValue==0)
				    {
					voiceparactl->WT588F_step=1;
					voiceparactl->WT588F_data[0]=STOP_NOW_VOICE;
					voiceparactl->WT588F_data[1]=uiVoicetemp1;
				    }
				else 
				    {
					voiceparactl->WT588F_step=4;
					voiceparactl->WT588F_data[0]=STOP_NOW_VOICE;
					voiceparactl->WT588F_data[1]=CONTINUOUS_NOW_VOICE;
					voiceparactl->WT588F_data[2]=uiVoicetemp1;
					voiceparactl->WT588F_data[3]=CONTINUOUS_NOW_VOICE;
					voiceparactl->WT588F_data[4]=uiVoicetemp2;
				    }				
				uiVoicetemp1=0;
				uiVoicetemp2=0;
				}
		}
	   else if(voiceparactl->uiPreVolumePromptValue !=voiceparactl->uiVolumePromptValue)
	   	{
	   	/*
	   	   if(voiceparactl->uiVolumePromptValue==VOLUME_ADJUSTMENT_OFF)
	   	   	{
		      voiceparactl->bVolumeOFFFlag=1;// 加入关闭语音标志，防止关闭语音播报时其他语音插入打断
		      voiceparactl->bVolumeChangeFlag=0;
			  voiceparactl->bVolumePlayChangeFlag=0;// 20220113
			  voiceparactl->uiPreVolumePromptValue =0;
			  voiceparactl->uiVolumePromptValue=0;
	   	   	}
	   	*/
		 if(voiceparactl->bVolumeSendFlag==0)
//			else if(voiceparactl->bVolumeSendFlag==0)
		   	{
			  voiceparactl->WT588F_step=0;
			  voiceparactl->WT588F_data[0]=voiceparactl->uiVolumePromptValue;
		      voiceparactl->bVolumeSendFlag=1;
		   	}		
	   	}
	}
	
	if(voiceparactl->bVolumeSendFlag==1&&eepromparactl->bEEpromRunFlag==0)
	 {
		Line_2A_WT588F(keyparactl,voiceparactl);   
	 }
	else
		{
		 SET_SCL_PIN(1);
		 SET_SDA_PIN(1);
		 Write_EEPROM();
		}
}

	if(voiceparactl->off_op_cnt>0)
		{
		if(++voiceparactl->off_op_cnt>(150000/VOICE_TICKS_INTERVAL))//400)
			{
			Enable_OP_PIN(1);
			voiceparactl->off_op_cnt=(200000/VOICE_TICKS_INTERVAL);
			}
		else //if(voiceparactl->off_op_cnt>7500)
			{
			Enable_OP_PIN(0);
			}
		}
	
	if(voiceparactl->on_op_cnt>0&&pbSpk_VOICE==1)
		{
		if(++voiceparactl->on_op_cnt>(400000/VOICE_TICKS_INTERVAL))// 400ms
				{
				if(Read_VoiceBusy_PIN()==1)
					{
						if(++voiceparactl->close_voice_cnt>(300000/VOICE_TICKS_INTERVAL))// 300ms
						{
						pbVOICE_CLOSE;
						}
					}
				else
					{
					voiceparactl->close_voice_cnt=0;
					}
				if(voiceparactl->on_op_cnt>(1000000/VOICE_TICKS_INTERVAL))// 1000ms
					{
					voiceparactl->on_op_cnt=0;
					voiceparactl->close_voice_cnt=0;
					}
				}
		}
	else
		{
		voiceparactl->close_voice_cnt=0;
		voiceparactl->on_op_cnt=0;
		}
}

