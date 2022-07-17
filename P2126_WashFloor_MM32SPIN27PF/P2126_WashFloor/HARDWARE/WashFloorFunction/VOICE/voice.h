#ifndef _VOICE_H
#define _VOICE_H

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "eeprom.h"
#include "key.h"

#define OFF_VOICE_DEF  5000//


#define VOICE_TICKS_INTERVAL  1000

///////////////////////////////////////////////////////////////////////////////////
//-------------------提示音---------------------------------
typedef enum
{
    PROMPT_MUTE_MP3									= 0x0000,/* 静音 */
    PROMPT_CHINESE_MP3								= 0x0001,/* 中文  */
    PROMPT_ENGLISH_MP3								= 0x0002,/* 英语  */
    PROMPT_GERMANY_MP3								= 0x0003,/* 德语  */
    PROMPT_FRANCE_MP3							    = 0x0004,/* 法语  */
    PROMPT_ESPANISH_MP3								= 0x0005,/* 西班牙语  */
    PROMPT_ITALIA_MP3								= 0x0006,/* 意大利语  */
    PROMPT_RUSSION_MP3								= 0x0007,/* 俄语  */
    PROMPT_JAPANESE_MP3								= 0x0008,/* 日语  */
    PROMPT_KOREA_MP3								= 0x0009,/* 韩语  */
    PROMPT_ARABIC_MP3								= 0x000A,/* 阿拉伯语 */
    PROMPT_HEBREW_MP3								= 0x000B,/* 希伯来语 */
    PROMPT_RESV1_MP3								= 0x000C,/* 保留1 */
    PROMPT_RESV2_MP3								= 0x000D,/* 保留2 */
    PROMPT_RESV3_MP3								= 0x000E,/* 保留3 */
    PROMPT_RESV4_MP3								= 0x000F,/* 保留4 */
//---------------------------汉语播报-------------------------------	
    PROMPT_AUTO_MODE_MP3							= 0x10,/* 自动模式 */
    PROMPT_TURBO_MODE_MP3							= 0x11,/* 吸水模式 */
	PROMPT_SLEEP_REQ_MP3							= 0x12,/* 休眠中，请重新开机 */
    PROMPT_POWER_OFF_MP3							= 0x13,/* 已关机 */
    PROMPT_REQ_CLEAN_MP3							= 0x14,/* 请放回底座自清洁 */
	PROMPT_LOW_POWER_MP3							= 0x15,/* 电池电量低 */
    PROMPT_LOW_POWER_CHARGE_MP3				        = 0x16,/* 请及时充电 */
    PROMPT_POWER_OVER_MP3							= 0x17,/* 电量已耗尽 */
    PROMPT_START_CHARGE_MP3					     	= 0x18,/* 开始充电 */
    PROMPT_CLEANESS_DET_MP3					    	= 0x19,/* 请清洁管道和传感器 */
    PROMPT_LOW_POWER_CLEAN_MP3				        = 0x1A,/* 请充电后自清洁 */
    PROMPT_START_CLEAN_MP3					    	= 0x1B,/* 开始自清洁 */
    PROMPT_BRUSH_CLEANING_MP3				    	= 0x1C,/* 滚刷清洁中 */
    PROMPT_TUBE_CLEANING_MP3				    	= 0x1D,/* 管道清洁中 */
    PROMPT_INTELLIGENT_DET_MP3			        	= 0x1E,/* 智能检测中 */		
    PROMPT_DEEP_CLEANING_MP3			    		= 0x1F,/* 深度清洁中 */
    PROMPT_CLEAN_OVER_MP3							= 0x20,/* 自清洁完毕 */
    PROMPT_CLEAN_CLOSE_MP3				    		= 0x21,/* 自清洁已关闭 */
    PROMPT_DIRTY_CLEAN_MP3					    	= 0x22,/* 请清理污水箱 */
    PROMPT_CLEAN_DIRTY_TUBE_MP3						= 0x23,/* 请清理污水箱和底部管道 */
    PROMPT_VOLUME_LOW_MP3							= 0x24,/* 低音量 */
    PROMPT_VOLUME_HIGH_MP3				    		= 0x25,/* 高音量*/
    PROMPT_CLOSE_VOLUME_MP3				    		= 0x26,/* 关闭语音*/
    PROMPT_OPEN_VOLUME_MP3				    		= 0x27,/* 开启语音*/
    PROMPT_SETTING_LANGUAGE_MP3			        	= 0x28,/* 开始设置语言，短按切换，长按确认*/
    PROMPT_LANGUAGE_SUCCESS_MP3                     = 0x29,/* 语言设置成功*/
    PROMPT_CBOX_NO_WATER_MP3                        = 0x2A,/* 清水箱缺水，请及时加水*/
    PROMPT_NO_SEWAGE_BOX_MP3                        = 0x2B,/* 污水箱未安装，请安装污水箱*/
    PROMPT_SEWAGE_BOX_FULL_MP3                      = 0x2C,/* 污水箱已满*/
    PROMPT_NO_BRUSH_MP3            		            = 0x2D,/* 滚刷未安装，请安装滚刷,并重启机器*/
    PROMPT_CHG_BEFORE_DRY_MP3                       = 0x2E,/* 请充电后烘干*/
	PROMPT_BAT_TEMP_OVER_MP3                        = 0x2F,/* 电池温度过高，请等待降温后使用*/
	PROMPT_MACHINE_ERROR_MP3						= 0x30,/* 机器故障，请联系售后维修*/
	PROMPT_SELF_CLEAN_PAUSED_MP3 					= 0x31,/* 暂停自清洁*/
	PROMPT_CONTINUE_CLEAN_MP3						= 0x32,/* 继续自清洁*/
	PROMPT_CLEAN_TUBE_MP3					    	= 0x33,/* 请清理底部管道*/
	PROMPT_DISINFECT_MODE_MP3						= 0x34,/* 杀菌模式*/
	PROMPT_CHECK_FULL_CLEAR_BOX_MP3 				= 0x35,/* 除菌液制备中，请确保清水箱满水*/
	PROMPT_SANITIZER_STAY_MP3 			        	= 0x36,/* 除菌液制备中，请稍后*/
	PROMPT_SANITIZER_HALF_MP3 				        = 0x37,/* 除菌液已制备50%*/
	PROMPT_SANITIZER_COMPLETED_MP3					= 0x38,/* 除菌液已制备完成*/
	PROMPT_BRUSH_DRY_MP3                            = 0x39,/* 滚刷烘干中*/
	PROMPT_BRUSH_DRY_OFF_MP3						= 0x3A,/* 烘干功能已关闭*/
	PROMPT_BRUSH_DRY_OVER_MP3                       = 0x3B,/* 烘干完毕*/
	PROMPT_DIRTY_START_CLEAN_MP3					= 0x3C,/* 滚刷及管道脏污，请开始自清洁*/
	PROMPT_DEEP_CLEAN_MP3						    = 0x3D,/* 深度自清洁*/
	PROMPT_SANITIZER_START_CLAEN_MP3				= 0x3E,/* 除菌液制备已取消，开始清洗*/
} PROMPT_TONE_TYPE_E;

//语音芯片特殊指令


#define    STOP_NOW_VOICE                    0xFFFE /*停止播放当前语音指令*/
#define    CYCLE_NOW_VOICE                   0xFFF2 /*循环播放当前指令*/
#define    CONTINUOUS_NOW_VOICE              0xFFF3 /*连码播放指令*/

#define    VOLUME_ADJUSTMENT_0               0xFFE0/*音量调节0*/
#define    VOLUME_ADJUSTMENT_1               0xFFE1/*音量调节1*/
#define    VOLUME_ADJUSTMENT_2               0xFFE2/*音量调节2*/
#define    VOLUME_ADJUSTMENT_3               0xFFE3/*音量调节3*/
#define    VOLUME_ADJUSTMENT_4               0xFFE4/*音量调节4*/
#define    VOLUME_ADJUSTMENT_5               0xFFE5/*音量调节5*/
#define    VOLUME_ADJUSTMENT_6               0xFFE6/*音量调节6*/
#define    VOLUME_ADJUSTMENT_7               0xFFE7/*音量调节7*/
#define    VOLUME_ADJUSTMENT_8               0xFFE8/*音量调节8*/
#define    VOLUME_ADJUSTMENT_9               0xFFE9/*音量调节9*/
#define    VOLUME_ADJUSTMENT_A               0xFFEA/*音量调节10*/
#define    VOLUME_ADJUSTMENT_B               0xFFEB/*音量调节11*/
#define    VOLUME_ADJUSTMENT_C               0xFFEC/*音量调节12*/
#define    VOLUME_ADJUSTMENT_D               0xFFED/*音量调节13*/
#define    VOLUME_ADJUSTMENT_E               0xFFEE/*音量调节14*/
#define    VOLUME_ADJUSTMENT_F               0xFFEF/*音量调节15*/

#define    VOLUME_ADJUSTMENT_ON              0x5A5A/*开启语音*/
#define    VOLUME_ADJUSTMENT_OFF             0x5AA5/*关闭语音*/
#define    VOLUME_INIT                       0xFFFF/*系统语音参数初始化*/

#define    VLOUME_LOW_VALUE                 VOLUME_ADJUSTMENT_6 //VOLUME_ADJUSTMENT_A//VOLUME_ADJUSTMENT_3//VOLUME_ADJUSTMENT_A
#define    VLOUME_HIGH_VALUE                VOLUME_ADJUSTMENT_D //VOLUME_ADJUSTMENT_F//VOLUME_ADJUSTMENT_F

//#define    VLOUME_LOW_VALUE                 VOLUME_ADJUSTMENT_A//VOLUME_ADJUSTMENT_3//VOLUME_ADJUSTMENT_A
//#define    VLOUME_HIGH_VALUE                VOLUME_ADJUSTMENT_F//VOLUME_ADJUSTMENT_F

typedef struct
{
	uint8_t   bVolumeOFFFlag:1;
	uint8_t   bVolumeOFFFlagback:1;
	uint8_t   bVolumeChangeFlag:1;
	uint8_t   bVolumePlayChangeFlag:1;// 20220113
	uint8_t   bVolumeSendFlag:1;// 20220113
	uint16_t  uiTonePromptValue;         //提示音
	uint16_t  uiSecondTonePromptValue;
	uint16_t  uiThirdTonePromptValue;
	uint16_t  uiPreTonePromptValue;
	uint16_t  uiVolumePromptValue;
	uint16_t  uiPreVolumePromptValue;
	uint16_t  uiOffVoiceCnt;// 关闭语音等待语音播报完关闭运放计时
	uint8_t   ucAutoCleanSuccess;
	uint8_t   ucChargingSewageHint;  //污水提示
	uint8_t   ucShortKeyMenuValue;
	uint8_t   ucLongKeyMenuValue;	
	uint8_t   ucLanguageValue;	
	uint8_t   ucKeyLanguageValue;
	uint8_t	  ucLanguageCount;
	uint8_t	  ucLanguageCount_back;
	uint8_t   ucFunctionValue;	
	uint16_t  ucPlayVoiceValue;	
	uint16_t  ucSendVoiceOVcnt;	// 语音播放标志切换延时
	uint16_t  WT588F_data[7];
	uint16_t  WT588F_step;
	uint16_t  off_op_cnt;
	uint16_t  on_op_cnt;
	uint32_t  close_voice_cnt;
	uint32_t  open_voice_cnt;
//////////////////////////////////////
//    uint16_t voice_cnt1;
//    uint8_t voice_step;
//	uint8_t voice_num;
//	uint8_t voice_first;	
}VoicePara;

typedef struct{
 uint8_t VoicePromptMuteFlag:1;
 uint8_t VoicePromptChineseFlag:1;
 uint8_t VoicePromptEnglishFlag:1;
 uint8_t VoicePromptGermanyFlag:1;
 uint8_t VoicePromptFranceFlag:1;
 uint8_t VoicePromptEspanishFlag:1;
 uint8_t VoicePromptItaliaFlag:1;
 uint8_t VoicePromptRussionFlag:1;
 uint8_t VoicePromptJapaneseFlag:1;
 uint8_t VoicePromptKoreaFlag:1;
 uint8_t VoicePromptArabicFlag:1;//
 uint8_t VoicePromptHebrewFlag:1;
 uint8_t VoicePromptAutoModeFlag:1; // 自动模式
 uint8_t VoicePromptTurboModeFlag:1;//吸水模式
 uint8_t VoicePromptSleepREQFlag:1;// 休眠中，请重新放回底座
 uint8_t VoicePromptPowerOffFlag:1;// 已关机
 uint8_t VoicePromptReqCleanFlag:1; //请放回底座自清洁
 uint8_t VoicePromptLowPowerFlag:1;//电池电量低
 uint8_t VoicePromptLowPowerChargeFlag:1;//请及时充电 
 uint8_t VoicePromptPowerOverFlag:1;//电量已耗尽
 uint8_t VoicePromptStartChargeFlag:1;// 开始充电
 uint8_t VoicePromptCleanessDetFlag:1;// 请清洁管道和传感器
 uint8_t VoicePromptLowPowerCleanFlag:1; // 请充电后自清洁	
 uint8_t VoicePromptStartCleanFlag:1; //开始自清洁
 uint8_t VoicePromptBrushCleaningFlag:1;//滚刷清洁中
 uint8_t VoicePromptTubeCleaningFlag:1;//管道清洁中
 uint8_t VoicePromptIntelligentDetFlag:1;//智能检测中
 uint8_t VoicePromptDeepCleaningFlag:1;// 深度清洁中
 uint8_t VoicePromptCleanOverFlag:1;// 自清洁完毕
 uint8_t VoicePromptCleanCloseFlag:1;//自清洁已关闭
 uint8_t VoicePromptDirtyCleanFlag:1;// 请清理污水箱
 uint8_t VoicePromptCleanDirtyTubeFlag:1;// 请清理污水箱和底部管道
 uint8_t VoicePromptVolumeLowFlag:1;//音量低
 uint8_t VoicePromptVolumeHighFlag:1;//音量高
 uint8_t VoicePromptCloseVolumeFlag:1;//关闭语音
 uint8_t VoicePromptOpenVolumeFlag:1;//开启语音
 uint8_t VoicePromptSettingLanguageFlag:1;//开始设置语言，短按切换，长按确认
 uint8_t VoicePromptLanguageSuccessFlag:1; //语言设置成功
 uint8_t VoicePromptCboxNoWaterFlag:1;//清水箱缺水，请及时加水
 uint8_t VoicePromptNoSewageBoxFlag:1;//污水箱未安装，请安装污水箱
 uint8_t VoicePromptSewageBoxFullFlag:1;//污水箱已满
 uint8_t VoicePromptNoBrushFlag:1;//滚刷未安装，请安装滚刷 ,并重启机器
 uint8_t VoicePromptChgBeforeDryFlag:1;//请充电后烘干
 uint8_t VoicePromptBatTempOverFlag:1;//电池温度过高，请等待降温后使用
 uint8_t VoicePromptMachineErrorFlag:1;//机器故障，请联系售后维修
 uint8_t VoicePromptSelfCleanPausedFlag:1;//暂停自清洁
 uint8_t VoicePromptContinueCleanFlag:1;//继续自清洁
 uint8_t VoicePromptCleanTubeFlag:1;//请清理滚刷和底部管道
 uint8_t VoicePromptDisinfectModeFlag:1;// 杀菌模式
 uint8_t VoicePromptCheckFullClearFlag:1;// 除菌液制备中，请确保满足自清洁水量
 uint8_t VoicePromptSanitizerStayFlag:1;// 除菌液制备中，请稍后
 uint8_t VoicePromptSanitizerHalfFlag:1;// 除菌液已制备50%
 uint8_t VoicePromptSanitizerCompletedFlag:1;// 除菌液已制备完成
 uint8_t VoicePromptBrushDryFlag:1;//滚刷烘干中
 uint8_t VoicePromptBrushDryOffFlag:1;// 烘干功能已关闭
 uint8_t VoicePromptBrushDryOverFlag:1;// 烘干完毕
 uint8_t VoicePromptDirtyStartCLeanFlag:1;// 滚刷及管道脏污，请开始自清洁
 
 uint8_t VoicePromptStartCleanBrushCleaningFlag:1;// 开始自清洁地刷清洁中
 uint8_t VoicePromptStartCleanSolutionProdectFlag:1;// 开始自清洁消毒水生成中
 uint8_t VoicePromptBrushDryOverCleanOverFlag:1;// 烘干完毕 自清洁完毕
 uint8_t VoicePromptLowPowerLowPowerCleanFlag:1;// 电池电量低，请充电后自清洁
 uint8_t VoicePromptSewageBoxFullDirtyCleanFlag:1;// 污水箱已满请清理污水箱
}VoiceList;

void Enable_OP_PIN(uint8_t enableoppin);
void Play_WT588F(KeyPara* keyparactl,VoicePara* voiceparactl,Eeprom_TypeDef* eepromparactl,VoiceList* voicelistflag);
uint8_t Read_VoiceBusy_PIN(void);
void SET_SCL_PIN(uint8_t enablesclpin);
void SET_SDA_PIN(uint8_t enablesclpin);

#endif

