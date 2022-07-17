#include "voice.h"
#include "voicelist.h"
#include "key.h"

void Play_PROMPT_MUTE(VoicePara* voicesetpara)
{// 静音
	voicesetpara->uiTonePromptValue = PROMPT_CLOSE_VOLUME_MP3;//关闭语音
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CHINESE(VoicePara* voicesetpara)
{// 中文
	voicesetpara->uiTonePromptValue = PROMPT_CHINESE_MP3;//汉语
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_ENGLISH(VoicePara* voicesetpara)
{// 英文
	voicesetpara->uiTonePromptValue = PROMPT_ENGLISH_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_GERMANY(VoicePara* voicesetpara)
{//  德语
	voicesetpara->uiTonePromptValue = PROMPT_GERMANY_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_FRANCE(VoicePara* voicesetpara)
{//  法语
	voicesetpara->uiTonePromptValue = PROMPT_FRANCE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_ESPANISH(VoicePara* voicesetpara)
{// 西班牙语
	voicesetpara->uiTonePromptValue = PROMPT_ESPANISH_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_ITALIA(VoicePara* voicesetpara)
{// 意大利语
	voicesetpara->uiTonePromptValue = PROMPT_ITALIA_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;

}

void Play_PROMPT_RUSSION(VoicePara* voicesetpara)
{// 俄罗斯语
	voicesetpara->uiTonePromptValue = PROMPT_RUSSION_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_JAPANESE(VoicePara* voicesetpara)
{// 日语
	voicesetpara->uiTonePromptValue = PROMPT_JAPANESE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_KOREA(VoicePara* voicesetpara)
{// 韩语
	voicesetpara->uiTonePromptValue = PROMPT_KOREA_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_ARABIC(VoicePara* voicesetpara)
{// 阿拉伯语
	voicesetpara->uiTonePromptValue = PROMPT_ARABIC_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_HEBREW(VoicePara* voicesetpara)
{// 希伯来语
	voicesetpara->uiTonePromptValue = PROMPT_HEBREW_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_AUTO_MODE(VoicePara* voicesetpara)
{// 自动模式
	voicesetpara->uiTonePromptValue = PROMPT_AUTO_MODE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_TURBO_MODE(VoicePara* voicesetpara)
{// 吸水模式
	voicesetpara->uiTonePromptValue = PROMPT_TURBO_MODE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_SLEEP_REQ(VoicePara* voicesetpara)
{// 休眠中，请重新放回底座
	voicesetpara->uiTonePromptValue = PROMPT_SLEEP_REQ_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}


void Play_PROMPT_POWER_OFF(VoicePara* voicesetpara)
{// 已关机
	voicesetpara->uiTonePromptValue = PROMPT_POWER_OFF_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_REQ_CLEAN(VoicePara* voicesetpara)
{//  请放回底座自清洁
	voicesetpara->uiTonePromptValue = PROMPT_REQ_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_LOW_POWER(VoicePara* voicesetpara)
{//  电池电量低
	voicesetpara->uiTonePromptValue = PROMPT_LOW_POWER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_LOW_POWER_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_LOW_POWER_CHARGE(VoicePara* voicesetpara)
{//  请及时充电
	voicesetpara->uiTonePromptValue = PROMPT_LOW_POWER_CHARGE_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_LOW_POWER_CHARGE_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_POWER_OVER(VoicePara* voicesetpara)
{//  电量已耗尽
	voicesetpara->uiTonePromptValue = PROMPT_POWER_OVER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_POWER_OVER_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_START_CHARGE(VoicePara* voicesetpara)
{//  开始充电
	voicesetpara->uiTonePromptValue = PROMPT_START_CHARGE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CLEANESS_DET(VoicePara* voicesetpara)
{//  请清洁管道和传感器
	voicesetpara->uiTonePromptValue = PROMPT_CLEANESS_DET_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CLEANESS_DET_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_LOW_POWER_CLEAN(VoicePara* voicesetpara)
{//  请充电后自清洁
	voicesetpara->uiTonePromptValue = PROMPT_LOW_POWER_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_START_CLEAN(VoicePara* voicesetpara)
{//  开始自清洁
	voicesetpara->uiTonePromptValue = PROMPT_START_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_BRUSH_CLEANING(VoicePara* voicesetpara)
{//  滚刷清洁中 
	voicesetpara->uiTonePromptValue = PROMPT_BRUSH_CLEANING_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_TUBE_CLEANING(VoicePara* voicesetpara)
{//  管道清洁中
	voicesetpara->uiTonePromptValue = PROMPT_TUBE_CLEANING_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_INTELLIGENT_DET(VoicePara* voicesetpara)
{//  智能检测中
	voicesetpara->uiTonePromptValue = PROMPT_INTELLIGENT_DET_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_DEEP_CLEANING(VoicePara* voicesetpara)
{// 深度清洁中
	voicesetpara->uiTonePromptValue = PROMPT_DEEP_CLEANING_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CLEAN_OVER(VoicePara* voicesetpara)
{// 自清洁完毕
	voicesetpara->uiTonePromptValue = PROMPT_CLEAN_OVER_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CLEAN_CLOSE(VoicePara* voicesetpara)
{// 自清洁已关闭
	voicesetpara->uiTonePromptValue = PROMPT_CLEAN_CLOSE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_DIRTY_CLEAN(VoicePara* voicesetpara)
{// 请清理污水箱
	voicesetpara->uiTonePromptValue = PROMPT_DIRTY_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_DIRTY_CLEAN_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CLEAN_DIRTY_TUBE(VoicePara* voicesetpara)
{// 请清理污水箱和底部管道
	voicesetpara->uiTonePromptValue = PROMPT_CLEAN_DIRTY_TUBE_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CLEAN_DIRTY_TUBE_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;

}

void Play_PROMPT_VOLUME_LOW(VoicePara* voicesetpara)
{// 音量低
	voicesetpara->uiTonePromptValue = PROMPT_VOLUME_LOW_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_VOLUME_HIGH(VoicePara* voicesetpara)
{// 音量高
	voicesetpara->uiTonePromptValue = PROMPT_VOLUME_HIGH_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
	
void Play_PROMPT_CLOSE_VOLUME(VoicePara* voicesetpara)
{// 关闭语音*/
	voicesetpara->uiTonePromptValue = PROMPT_CLOSE_VOLUME_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_OPEN_VOLUME(VoicePara* voicesetpara)
{// 开启语音
	voicesetpara->uiTonePromptValue = PROMPT_OPEN_VOLUME_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_SETTING_LANGUAGE(VoicePara* voicesetpara)
{// 开始设置语言，短按切换，长按确认
	voicesetpara->uiTonePromptValue = PROMPT_SETTING_LANGUAGE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_LANGUAGE_SUCCESS(VoicePara* voicesetpara)
{// 语言设置成功
	voicesetpara->uiTonePromptValue = PROMPT_LANGUAGE_SUCCESS_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CBOX_NO_WATER(VoicePara* voicesetpara)
{// 清水箱缺水，请及时加水
	voicesetpara->uiTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CBOX_NO_WATER_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_NO_SEWAGE_BOX(VoicePara* voicesetpara)
{// 污水箱未安装，请安装污水箱
	voicesetpara->uiTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_NO_SEWAGE_BOX_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_SEWAGE_BOX_FULL(VoicePara* voicesetpara)
{// 污水箱已满
	voicesetpara->uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_NO_BRUSH(VoicePara* voicesetpara)
{// 滚刷未安装，请安装滚刷，并重启机器
	voicesetpara->uiTonePromptValue = PROMPT_NO_BRUSH_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_NO_BRUSH_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_CHG_BEFORE_DRY(VoicePara* voicesetpara)
{// 电池电量低，请充电后烘干
	voicesetpara->uiTonePromptValue = PROMPT_LOW_POWER_MP3;
//	voicesetpara->uiTonePromptValue = PROMPT_CHG_BEFORE_DRY_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CHG_BEFORE_DRY_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}


void Play_PROMPT_BAT_TEMP_OVER(VoicePara* voicesetpara)
{// 电池温度过高，请等待降温后使用
	voicesetpara->uiTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_BAT_TEMP_OVER_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_MACHINE_ERROR(VoicePara* voicesetpara)
{// 机器故障，请联系售后维修
	voicesetpara->uiTonePromptValue = PROMPT_MACHINE_ERROR_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_MACHINE_ERROR_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_SELF_CLEAN_PAUSED(VoicePara* voicesetpara)
{// 暂停自清洁
	voicesetpara->uiTonePromptValue = PROMPT_SELF_CLEAN_PAUSED_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_CONTINUE_CLEAN(VoicePara* voicesetpara)
{// 继续自清洁
	voicesetpara->uiTonePromptValue = PROMPT_CONTINUE_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_CLEAN_TUBE(VoicePara* voicesetpara)
{// 请清理底部管道
	voicesetpara->uiTonePromptValue = PROMPT_CLEAN_TUBE_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CLEAN_TUBE_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_DISINFECT_MODE(VoicePara* voicesetpara)
{// 杀菌模式
	voicesetpara->uiTonePromptValue = PROMPT_DISINFECT_MODE_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
//	voicesetpara->uiSecondTonePromptValue = PROMPT_CHECK_FULL_CLEAR_BOX_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}


void Play_PROMPT_CHECK_FULL_CLEAR_BOX(VoicePara* voicesetpara)
{// 除菌液制备中，请确保清水箱满水
	voicesetpara->uiTonePromptValue = PROMPT_CHECK_FULL_CLEAR_BOX_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_SANITIZER_STAY(VoicePara* voicesetpara)
{// 除菌液制备中，请稍后
	voicesetpara->uiTonePromptValue = PROMPT_SANITIZER_STAY_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_SANITIZER_HALF(VoicePara* voicesetpara)
{// 除菌液已制备50%
	voicesetpara->uiTonePromptValue = PROMPT_SANITIZER_HALF_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_SANITIZER_COMPLETED(VoicePara* voicesetpara)
{//除菌液已制备完成
	voicesetpara->uiTonePromptValue = PROMPT_SANITIZER_COMPLETED_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_BRUSH_DRY(VoicePara* voicesetpara)
{// 滚刷烘干中
	voicesetpara->uiTonePromptValue = PROMPT_BRUSH_DRY_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_BRUSH_DRY_OFF(VoicePara* voicesetpara)
{// 烘干功能已关闭
	voicesetpara->uiTonePromptValue = PROMPT_BRUSH_DRY_OFF_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_BRUSH_DRY_OVER(VoicePara* voicesetpara)
{// 烘干完毕
	voicesetpara->uiTonePromptValue = PROMPT_BRUSH_DRY_OVER_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_DIRTY_START_CLEAN(VoicePara* voicesetpara)
{// 滚刷及管道脏污，请开始自清洁
	voicesetpara->uiTonePromptValue = PROMPT_DIRTY_START_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = 0;
	voicesetpara->uiPreTonePromptValue	= 0;
}

////
void Play_PROMPT_START_CLEANAndSOLUTION_PRODECT(VoicePara* voicesetpara)
{// 开始自清洁，除菌液制备中，请确保清水箱满水
	voicesetpara->uiTonePromptValue = PROMPT_START_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CHECK_FULL_CLEAR_BOX_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}
void Play_PROMPT_START_CLEANAndBRUSH_CLEANING(VoicePara* voicesetpara)
{// 开始自清洁，滚刷清洁中
	voicesetpara->uiTonePromptValue = PROMPT_START_CLEAN_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_BRUSH_CLEANING_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_BRUSH_DRY_OVERAndCLEAN_OVER(VoicePara* voicesetpara)
{// 烘干完毕，自清洁完毕
	voicesetpara->uiTonePromptValue = PROMPT_BRUSH_DRY_OVER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_CLEAN_OVER_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}


void Play_PROMPT_LOW_POWERAndLOW_POWER_CLEAN_OVER(VoicePara* voicesetpara)
{// 电池电量低，请充电后自清洁
	voicesetpara->uiTonePromptValue = PROMPT_LOW_POWER_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_LOW_POWER_CLEAN_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}

void Play_PROMPT_SEWAGE_BOX_FULL_DIRTY_CLEAN_OVER(VoicePara* voicesetpara)
{// 污水箱已满，请清理污水箱
	voicesetpara->uiTonePromptValue = PROMPT_SEWAGE_BOX_FULL_MP3;
	voicesetpara->uiSecondTonePromptValue = PROMPT_DIRTY_CLEAN_MP3;
	voicesetpara->uiPreTonePromptValue	= 0;
}


void Play_VoiceList(VoiceList* voicelistflag,VoicePara* voicesetpara,KeyPara* keyparactl)
{
// if(keyparactl->bFastVoiceFlag==1)voicesetpara->ucSendVoiceOVcnt=0;
 if(voicesetpara->ucSendVoiceOVcnt>0)voicesetpara->ucSendVoiceOVcnt--;
 else
 	{
 if(voicelistflag->VoicePromptMuteFlag)
 	{
    Play_PROMPT_MUTE(voicesetpara);
//	keyparactl->bFastVoiceFlag=1;
 	voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	voicelistflag->VoicePromptMuteFlag=0;
 	}
 else if(voicelistflag->VoicePromptChineseFlag)
 	{
	Play_PROMPT_CHINESE(voicesetpara);
//	keyparactl->bFastVoiceFlag=1;
 	voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	voicelistflag->VoicePromptChineseFlag=0;
 	}
 else if(voicelistflag->VoicePromptEnglishFlag)
 	{
	Play_PROMPT_ENGLISH(voicesetpara);
//	keyparactl->bFastVoiceFlag=1;
 	voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	voicelistflag->VoicePromptEnglishFlag=0;
 	}
 else if(voicelistflag->VoicePromptGermanyFlag)
 	{
    Play_PROMPT_GERMANY(voicesetpara);
//	keyparactl->bFastVoiceFlag=1;
 	voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	voicelistflag->VoicePromptGermanyFlag=0;
 	}
 else if(voicelistflag->VoicePromptFranceFlag)
 	{
	 Play_PROMPT_FRANCE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptFranceFlag=0;
 	}
 else if(voicelistflag->VoicePromptEspanishFlag)
 	{
	 Play_PROMPT_ESPANISH(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptEspanishFlag=0;
 	}
 else if(voicelistflag->VoicePromptItaliaFlag)
 	{
	 Play_PROMPT_ITALIA(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptItaliaFlag=0;
 	}
 else if(voicelistflag->VoicePromptRussionFlag)
 	{
	 Play_PROMPT_RUSSION(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptRussionFlag=0;
 	}
 else if(voicelistflag->VoicePromptJapaneseFlag)
 	{
	 Play_PROMPT_JAPANESE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptJapaneseFlag=0;
 	}
 else if(voicelistflag->VoicePromptKoreaFlag)
 	{
	 Play_PROMPT_KOREA(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptKoreaFlag=0;
 	}
 else if(voicelistflag->VoicePromptArabicFlag)
 	{
	 Play_PROMPT_ARABIC(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptArabicFlag=0;
 	}
 else if(voicelistflag->VoicePromptHebrewFlag)
 	{
	 Play_PROMPT_HEBREW(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptHebrewFlag=0;
 	}
 else if(voicelistflag->VoicePromptAutoModeFlag)
 	{
	 Play_PROMPT_AUTO_MODE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptAutoModeFlag=0;
 	}
 else if(voicelistflag->VoicePromptTurboModeFlag)
 	{
	 Play_PROMPT_TURBO_MODE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptTurboModeFlag=0;
 	}
 else if(voicelistflag->VoicePromptSleepREQFlag)
 	{
	 Play_PROMPT_SLEEP_REQ(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSleepREQFlag=0;
 	}
 else if(voicelistflag->VoicePromptPowerOffFlag)
 	{
	 Play_PROMPT_POWER_OFF(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptPowerOffFlag=0;
 	}
 else if(voicelistflag->VoicePromptReqCleanFlag)
 	{
	 Play_PROMPT_REQ_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptReqCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptLowPowerFlag)
 	{
	 Play_PROMPT_LOW_POWER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptLowPowerFlag=0;
 	}
 else if(voicelistflag->VoicePromptLowPowerChargeFlag)
 	{
	 Play_PROMPT_LOW_POWER_CHARGE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptLowPowerChargeFlag=0;
 	}
 else if(voicelistflag->VoicePromptPowerOverFlag)
 	{
	 Play_PROMPT_POWER_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptPowerOverFlag=0;
 	}
 else if(voicelistflag->VoicePromptCleanessDetFlag)
 	{
	 Play_PROMPT_CLEANESS_DET(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCleanessDetFlag=0;
 	}
 else if(voicelistflag->VoicePromptLowPowerCleanFlag)
 	{
	 Play_PROMPT_LOW_POWER_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptLowPowerCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptStartCleanFlag)
 	{
	 Play_PROMPT_START_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptStartCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptBrushCleaningFlag)
 	{
	 Play_PROMPT_BRUSH_CLEANING(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBrushCleaningFlag=0;
 	}
 else if(voicelistflag->VoicePromptTubeCleaningFlag)
 	{
	 Play_PROMPT_TUBE_CLEANING(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptTubeCleaningFlag=0;
 	}
 else if(voicelistflag->VoicePromptIntelligentDetFlag)
 	{
	 Play_PROMPT_INTELLIGENT_DET(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptIntelligentDetFlag=0;
 	}
 else if(voicelistflag->VoicePromptDeepCleaningFlag)
 	{
	 Play_PROMPT_DEEP_CLEANING(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptDeepCleaningFlag=0;
 	}
 else if(voicelistflag->VoicePromptCleanOverFlag)
 	{
	 Play_PROMPT_CLEAN_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCleanOverFlag=0;
 	}
 else if(voicelistflag->VoicePromptCleanCloseFlag)
 	{
	 Play_PROMPT_CLEAN_CLOSE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCleanCloseFlag=0;
 	}
 else if(voicelistflag->VoicePromptDirtyCleanFlag)
 	{
	 Play_PROMPT_DIRTY_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptDirtyCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptDirtyCleanFlag)
 	{
	 Play_PROMPT_DIRTY_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptDirtyCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptCleanDirtyTubeFlag)
 	{
	 Play_PROMPT_CLEAN_DIRTY_TUBE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCleanDirtyTubeFlag=0;
 	}
 else if(voicelistflag->VoicePromptVolumeLowFlag)
 	{
	 Play_PROMPT_VOLUME_LOW(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptVolumeLowFlag=0;
 	}
 else if(voicelistflag->VoicePromptVolumeHighFlag)
 	{
	 Play_PROMPT_VOLUME_HIGH(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptVolumeHighFlag=0;
 	}
 else if(voicelistflag->VoicePromptCloseVolumeFlag)
 	{
	 Play_PROMPT_CLOSE_VOLUME(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCloseVolumeFlag=0;
 	}
 else if(voicelistflag->VoicePromptOpenVolumeFlag)
 	{
	 Play_PROMPT_OPEN_VOLUME(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptOpenVolumeFlag=0;
 	}
 else if(voicelistflag->VoicePromptSettingLanguageFlag)
 	{
	 Play_PROMPT_SETTING_LANGUAGE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSettingLanguageFlag=0;
 	}
 else if(voicelistflag->VoicePromptLanguageSuccessFlag)
 	{
	 Play_PROMPT_LANGUAGE_SUCCESS(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptLanguageSuccessFlag=0;
 	}
 else if(voicelistflag->VoicePromptCboxNoWaterFlag)
 	{
	 Play_PROMPT_CBOX_NO_WATER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCboxNoWaterFlag=0;
 	}
 else if(voicelistflag->VoicePromptNoSewageBoxFlag)
 	{
	 Play_PROMPT_NO_SEWAGE_BOX(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptNoSewageBoxFlag=0;
 	}
 else if(voicelistflag->VoicePromptSewageBoxFullFlag)
 	{
	 Play_PROMPT_SEWAGE_BOX_FULL(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSewageBoxFullFlag=0;
 	}
 else if(voicelistflag->VoicePromptNoBrushFlag)
 	{
	 Play_PROMPT_NO_BRUSH(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptNoBrushFlag=0;
 	}
 else if(voicelistflag->VoicePromptChgBeforeDryFlag)
 	{
	 Play_PROMPT_CHG_BEFORE_DRY(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptChgBeforeDryFlag=0;
 	}
 else if(voicelistflag->VoicePromptBatTempOverFlag)
 	{
	 Play_PROMPT_BAT_TEMP_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBatTempOverFlag=0;
 	}
 else if(voicelistflag->VoicePromptMachineErrorFlag)
 	{
	 Play_PROMPT_MACHINE_ERROR(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptMachineErrorFlag=0;
 	}
 else if(voicelistflag->VoicePromptSelfCleanPausedFlag)
 	{
	 Play_PROMPT_SELF_CLEAN_PAUSED(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSelfCleanPausedFlag=0;
 	}
 else if(voicelistflag->VoicePromptContinueCleanFlag)
 	{
	 Play_PROMPT_CONTINUE_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptContinueCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptCleanTubeFlag)
 	{
	 Play_PROMPT_CLEAN_TUBE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCleanTubeFlag=0;
 	}
 else if(voicelistflag->VoicePromptDisinfectModeFlag)
 	{
	 Play_PROMPT_DISINFECT_MODE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptDisinfectModeFlag=0;
 	}
 else if(voicelistflag->VoicePromptCheckFullClearFlag)
 	{
	 Play_PROMPT_CHECK_FULL_CLEAR_BOX(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptCheckFullClearFlag=0;
 	}
 else if(voicelistflag->VoicePromptSanitizerStayFlag)
 	{
	 Play_PROMPT_SANITIZER_STAY(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSanitizerStayFlag=0;
 	}
 else if(voicelistflag->VoicePromptSanitizerHalfFlag)
 	{
	 Play_PROMPT_SANITIZER_HALF(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSanitizerHalfFlag=0;
 	}
 else if(voicelistflag->VoicePromptSanitizerCompletedFlag)
 	{
	 Play_PROMPT_SANITIZER_COMPLETED(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSanitizerCompletedFlag=0;
 	}
 else if(voicelistflag->VoicePromptBrushDryFlag)
 	{
	 Play_PROMPT_BRUSH_DRY(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBrushDryFlag=0;
 	}
 else if(voicelistflag->VoicePromptBrushDryOffFlag)
 	{
	 Play_PROMPT_BRUSH_DRY_OFF(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBrushDryOffFlag=0;
 	}
 else if(voicelistflag->VoicePromptBrushDryOverFlag)
 	{
	 Play_PROMPT_BRUSH_DRY_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBrushDryOverFlag=0;
 	}
 else if(voicelistflag->VoicePromptStartCleanSolutionProdectFlag)
 	{
	 Play_PROMPT_START_CLEANAndSOLUTION_PRODECT(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptStartCleanSolutionProdectFlag=0;
 	}
 else if(voicelistflag->VoicePromptStartCleanBrushCleaningFlag)
 	{
	 Play_PROMPT_START_CLEANAndBRUSH_CLEANING(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptStartCleanBrushCleaningFlag=0;
 	}
 else if(voicelistflag->VoicePromptBrushDryOverCleanOverFlag)
 	{
	 Play_PROMPT_BRUSH_DRY_OVERAndCLEAN_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptBrushDryOverCleanOverFlag=0;
 	}
 else if(voicelistflag->VoicePromptLowPowerLowPowerCleanFlag)
 	{
	 Play_PROMPT_LOW_POWERAndLOW_POWER_CLEAN_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptLowPowerLowPowerCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptSewageBoxFullDirtyCleanFlag)
 	{
	 Play_PROMPT_SEWAGE_BOX_FULL_DIRTY_CLEAN_OVER(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptSewageBoxFullDirtyCleanFlag=0;
 	}
 else if(voicelistflag->VoicePromptStartChargeFlag)
 	{
	 Play_PROMPT_START_CHARGE(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptStartChargeFlag=0;
 	}
 else if(voicelistflag->VoicePromptDirtyStartCLeanFlag)
 	{
	 Play_PROMPT_DIRTY_START_CLEAN(voicesetpara);
//	 keyparactl->bFastVoiceFlag=1;
	 voicesetpara->ucSendVoiceOVcnt=Voice_Change_Time;
	 voicelistflag->VoicePromptDirtyStartCLeanFlag=0;
 	}
 	}
}


void Clear_VoiceFlag(VoiceList* voicelistflag)
{
	 voicelistflag->VoicePromptMuteFlag=0;
	 voicelistflag->VoicePromptChineseFlag=0;
	 voicelistflag->VoicePromptEnglishFlag=0;
	 voicelistflag->VoicePromptGermanyFlag=0;
	 voicelistflag->VoicePromptFranceFlag=0;
	 voicelistflag->VoicePromptEspanishFlag=0;
	 voicelistflag->VoicePromptItaliaFlag=0;
	 voicelistflag->VoicePromptRussionFlag=0;
	 voicelistflag->VoicePromptJapaneseFlag=0;
	 voicelistflag->VoicePromptKoreaFlag=0;
	 voicelistflag->VoicePromptArabicFlag=0;//
	 voicelistflag->VoicePromptHebrewFlag=0;
	 voicelistflag->VoicePromptAutoModeFlag=0; // 自动模式
	 voicelistflag->VoicePromptTurboModeFlag=0;//吸水模式
	 voicelistflag->VoicePromptSleepREQFlag=0;// 休眠中，请重新放回底座
	 voicelistflag->VoicePromptPowerOffFlag=0;// 已关机
	 voicelistflag->VoicePromptReqCleanFlag=0; //请放回底座自清洁
	 voicelistflag->VoicePromptLowPowerFlag=0;//电池电量低
	 voicelistflag->VoicePromptLowPowerChargeFlag=0;//请及时充电 
	 voicelistflag->VoicePromptPowerOverFlag=0;//电量已耗尽
	 voicelistflag->VoicePromptStartChargeFlag=0;// 开始充电
	 voicelistflag->VoicePromptCleanessDetFlag=0;// 请清洁管道和传感器
	 voicelistflag->VoicePromptLowPowerCleanFlag=0; // 请充电后自清洁  
	 voicelistflag->VoicePromptStartCleanFlag=0; //开始自清洁
	 voicelistflag->VoicePromptBrushCleaningFlag=0;//滚刷清洁中
	 voicelistflag->VoicePromptTubeCleaningFlag=0;//管道清洁中
	 voicelistflag->VoicePromptIntelligentDetFlag=0;//智能检测中
	 voicelistflag->VoicePromptDeepCleaningFlag=0;// 深度清洁中
	 voicelistflag->VoicePromptCleanOverFlag=0;// 自清洁完毕
	 voicelistflag->VoicePromptCleanCloseFlag=0;//自清洁已关闭
	 voicelistflag->VoicePromptDirtyCleanFlag=0;// 请清理污水箱
	 voicelistflag->VoicePromptCleanDirtyTubeFlag=0;// 请清理污水箱和底部管道
	 voicelistflag->VoicePromptVolumeLowFlag=0;//音量低
	 voicelistflag->VoicePromptVolumeHighFlag=0;//音量高
	 voicelistflag->VoicePromptCloseVolumeFlag=0;//关闭语音
	 voicelistflag->VoicePromptOpenVolumeFlag=0;//开启语音
	 voicelistflag->VoicePromptSettingLanguageFlag=0;//开始设置语言，短按切换，长按确认
	 voicelistflag->VoicePromptLanguageSuccessFlag=0; //语言设置成功
	 voicelistflag->VoicePromptCboxNoWaterFlag=0;//清水箱缺水，请及时加水
	 voicelistflag->VoicePromptNoSewageBoxFlag=0;//污水箱未安装，请安装污水箱
	 voicelistflag->VoicePromptSewageBoxFullFlag=0;//污水箱已满
	 voicelistflag->VoicePromptNoBrushFlag=0;//滚刷未安装，请安装滚刷 ,并重启机器
	 voicelistflag->VoicePromptChgBeforeDryFlag=0;//请充电后烘干
	 voicelistflag->VoicePromptBatTempOverFlag=0;//电池温度过高，请等待降温后使用
	 voicelistflag->VoicePromptMachineErrorFlag=0;//机器故障，请联系售后维修
	 voicelistflag->VoicePromptSelfCleanPausedFlag=0;//暂停自清洁
	 voicelistflag->VoicePromptContinueCleanFlag=0;//继续自清洁
	 voicelistflag->VoicePromptCleanTubeFlag=0;//请清理滚刷和底部管道
	 voicelistflag->VoicePromptDisinfectModeFlag=0;// 杀菌模式
	 voicelistflag->VoicePromptCheckFullClearFlag=0;// 除菌液制备中，请确保满足自清洁水量
	 voicelistflag->VoicePromptSanitizerStayFlag=0;// 除菌液制备中，请稍后
	 voicelistflag->VoicePromptSanitizerHalfFlag=0;// 除菌液已制备50%
	 voicelistflag->VoicePromptSanitizerCompletedFlag=0;// 除菌液已制备完成
	 voicelistflag->VoicePromptBrushDryFlag=0;//滚刷烘干中
	 voicelistflag->VoicePromptBrushDryOffFlag=0;// 烘干功能已关闭
	 voicelistflag->VoicePromptBrushDryOverFlag=0;// 烘干完毕
	 voicelistflag->VoicePromptDirtyStartCLeanFlag=0;// 滚刷及管道脏污，请开始自清洁
	
	 voicelistflag->VoicePromptStartCleanBrushCleaningFlag=0;// 开始自清洁地刷清洁中
	 voicelistflag->VoicePromptStartCleanSolutionProdectFlag=0;// 开始自清洁消毒水生成中
	 voicelistflag->VoicePromptBrushDryOverCleanOverFlag=0;// 烘干完毕 自清洁完毕
	 voicelistflag->VoicePromptLowPowerLowPowerCleanFlag=0;// 电池电量低，请充电后自清洁
	 voicelistflag->VoicePromptSewageBoxFullDirtyCleanFlag=0;// 污水箱已满请清理污水箱
}

