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
//-------------------��ʾ��---------------------------------
typedef enum
{
    PROMPT_MUTE_MP3									= 0x0000,/* ���� */
    PROMPT_CHINESE_MP3								= 0x0001,/* ����  */
    PROMPT_ENGLISH_MP3								= 0x0002,/* Ӣ��  */
    PROMPT_GERMANY_MP3								= 0x0003,/* ����  */
    PROMPT_FRANCE_MP3							    = 0x0004,/* ����  */
    PROMPT_ESPANISH_MP3								= 0x0005,/* ��������  */
    PROMPT_ITALIA_MP3								= 0x0006,/* �������  */
    PROMPT_RUSSION_MP3								= 0x0007,/* ����  */
    PROMPT_JAPANESE_MP3								= 0x0008,/* ����  */
    PROMPT_KOREA_MP3								= 0x0009,/* ����  */
    PROMPT_ARABIC_MP3								= 0x000A,/* �������� */
    PROMPT_HEBREW_MP3								= 0x000B,/* ϣ������ */
    PROMPT_RESV1_MP3								= 0x000C,/* ����1 */
    PROMPT_RESV2_MP3								= 0x000D,/* ����2 */
    PROMPT_RESV3_MP3								= 0x000E,/* ����3 */
    PROMPT_RESV4_MP3								= 0x000F,/* ����4 */
//---------------------------���ﲥ��-------------------------------	
    PROMPT_AUTO_MODE_MP3							= 0x10,/* �Զ�ģʽ */
    PROMPT_TURBO_MODE_MP3							= 0x11,/* ��ˮģʽ */
	PROMPT_SLEEP_REQ_MP3							= 0x12,/* �����У������¿��� */
    PROMPT_POWER_OFF_MP3							= 0x13,/* �ѹػ� */
    PROMPT_REQ_CLEAN_MP3							= 0x14,/* ��Żص�������� */
	PROMPT_LOW_POWER_MP3							= 0x15,/* ��ص����� */
    PROMPT_LOW_POWER_CHARGE_MP3				        = 0x16,/* �뼰ʱ��� */
    PROMPT_POWER_OVER_MP3							= 0x17,/* �����Ѻľ� */
    PROMPT_START_CHARGE_MP3					     	= 0x18,/* ��ʼ��� */
    PROMPT_CLEANESS_DET_MP3					    	= 0x19,/* �����ܵ��ʹ����� */
    PROMPT_LOW_POWER_CLEAN_MP3				        = 0x1A,/* ���������� */
    PROMPT_START_CLEAN_MP3					    	= 0x1B,/* ��ʼ����� */
    PROMPT_BRUSH_CLEANING_MP3				    	= 0x1C,/* ��ˢ����� */
    PROMPT_TUBE_CLEANING_MP3				    	= 0x1D,/* �ܵ������ */
    PROMPT_INTELLIGENT_DET_MP3			        	= 0x1E,/* ���ܼ���� */		
    PROMPT_DEEP_CLEANING_MP3			    		= 0x1F,/* �������� */
    PROMPT_CLEAN_OVER_MP3							= 0x20,/* �������� */
    PROMPT_CLEAN_CLOSE_MP3				    		= 0x21,/* ������ѹر� */
    PROMPT_DIRTY_CLEAN_MP3					    	= 0x22,/* ��������ˮ�� */
    PROMPT_CLEAN_DIRTY_TUBE_MP3						= 0x23,/* ��������ˮ��͵ײ��ܵ� */
    PROMPT_VOLUME_LOW_MP3							= 0x24,/* ������ */
    PROMPT_VOLUME_HIGH_MP3				    		= 0x25,/* ������*/
    PROMPT_CLOSE_VOLUME_MP3				    		= 0x26,/* �ر�����*/
    PROMPT_OPEN_VOLUME_MP3				    		= 0x27,/* ��������*/
    PROMPT_SETTING_LANGUAGE_MP3			        	= 0x28,/* ��ʼ�������ԣ��̰��л�������ȷ��*/
    PROMPT_LANGUAGE_SUCCESS_MP3                     = 0x29,/* �������óɹ�*/
    PROMPT_CBOX_NO_WATER_MP3                        = 0x2A,/* ��ˮ��ȱˮ���뼰ʱ��ˮ*/
    PROMPT_NO_SEWAGE_BOX_MP3                        = 0x2B,/* ��ˮ��δ��װ���밲װ��ˮ��*/
    PROMPT_SEWAGE_BOX_FULL_MP3                      = 0x2C,/* ��ˮ������*/
    PROMPT_NO_BRUSH_MP3            		            = 0x2D,/* ��ˢδ��װ���밲װ��ˢ,����������*/
    PROMPT_CHG_BEFORE_DRY_MP3                       = 0x2E,/* �������*/
	PROMPT_BAT_TEMP_OVER_MP3                        = 0x2F,/* ����¶ȹ��ߣ���ȴ����º�ʹ��*/
	PROMPT_MACHINE_ERROR_MP3						= 0x30,/* �������ϣ�����ϵ�ۺ�ά��*/
	PROMPT_SELF_CLEAN_PAUSED_MP3 					= 0x31,/* ��ͣ�����*/
	PROMPT_CONTINUE_CLEAN_MP3						= 0x32,/* ���������*/
	PROMPT_CLEAN_TUBE_MP3					    	= 0x33,/* ������ײ��ܵ�*/
	PROMPT_DISINFECT_MODE_MP3						= 0x34,/* ɱ��ģʽ*/
	PROMPT_CHECK_FULL_CLEAR_BOX_MP3 				= 0x35,/* ����Һ�Ʊ��У���ȷ����ˮ����ˮ*/
	PROMPT_SANITIZER_STAY_MP3 			        	= 0x36,/* ����Һ�Ʊ��У����Ժ�*/
	PROMPT_SANITIZER_HALF_MP3 				        = 0x37,/* ����Һ���Ʊ�50%*/
	PROMPT_SANITIZER_COMPLETED_MP3					= 0x38,/* ����Һ���Ʊ����*/
	PROMPT_BRUSH_DRY_MP3                            = 0x39,/* ��ˢ�����*/
	PROMPT_BRUSH_DRY_OFF_MP3						= 0x3A,/* ��ɹ����ѹر�*/
	PROMPT_BRUSH_DRY_OVER_MP3                       = 0x3B,/* ������*/
	PROMPT_DIRTY_START_CLEAN_MP3					= 0x3C,/* ��ˢ���ܵ����ۣ��뿪ʼ�����*/
	PROMPT_DEEP_CLEAN_MP3						    = 0x3D,/* ��������*/
	PROMPT_SANITIZER_START_CLAEN_MP3				= 0x3E,/* ����Һ�Ʊ���ȡ������ʼ��ϴ*/
} PROMPT_TONE_TYPE_E;

//����оƬ����ָ��


#define    STOP_NOW_VOICE                    0xFFFE /*ֹͣ���ŵ�ǰ����ָ��*/
#define    CYCLE_NOW_VOICE                   0xFFF2 /*ѭ�����ŵ�ǰָ��*/
#define    CONTINUOUS_NOW_VOICE              0xFFF3 /*���벥��ָ��*/

#define    VOLUME_ADJUSTMENT_0               0xFFE0/*��������0*/
#define    VOLUME_ADJUSTMENT_1               0xFFE1/*��������1*/
#define    VOLUME_ADJUSTMENT_2               0xFFE2/*��������2*/
#define    VOLUME_ADJUSTMENT_3               0xFFE3/*��������3*/
#define    VOLUME_ADJUSTMENT_4               0xFFE4/*��������4*/
#define    VOLUME_ADJUSTMENT_5               0xFFE5/*��������5*/
#define    VOLUME_ADJUSTMENT_6               0xFFE6/*��������6*/
#define    VOLUME_ADJUSTMENT_7               0xFFE7/*��������7*/
#define    VOLUME_ADJUSTMENT_8               0xFFE8/*��������8*/
#define    VOLUME_ADJUSTMENT_9               0xFFE9/*��������9*/
#define    VOLUME_ADJUSTMENT_A               0xFFEA/*��������10*/
#define    VOLUME_ADJUSTMENT_B               0xFFEB/*��������11*/
#define    VOLUME_ADJUSTMENT_C               0xFFEC/*��������12*/
#define    VOLUME_ADJUSTMENT_D               0xFFED/*��������13*/
#define    VOLUME_ADJUSTMENT_E               0xFFEE/*��������14*/
#define    VOLUME_ADJUSTMENT_F               0xFFEF/*��������15*/

#define    VOLUME_ADJUSTMENT_ON              0x5A5A/*��������*/
#define    VOLUME_ADJUSTMENT_OFF             0x5AA5/*�ر�����*/
#define    VOLUME_INIT                       0xFFFF/*ϵͳ����������ʼ��*/

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
	uint16_t  uiTonePromptValue;         //��ʾ��
	uint16_t  uiSecondTonePromptValue;
	uint16_t  uiThirdTonePromptValue;
	uint16_t  uiPreTonePromptValue;
	uint16_t  uiVolumePromptValue;
	uint16_t  uiPreVolumePromptValue;
	uint16_t  uiOffVoiceCnt;// �ر������ȴ�����������ر��˷ż�ʱ
	uint8_t   ucAutoCleanSuccess;
	uint8_t   ucChargingSewageHint;  //��ˮ��ʾ
	uint8_t   ucShortKeyMenuValue;
	uint8_t   ucLongKeyMenuValue;	
	uint8_t   ucLanguageValue;	
	uint8_t   ucKeyLanguageValue;
	uint8_t	  ucLanguageCount;
	uint8_t	  ucLanguageCount_back;
	uint8_t   ucFunctionValue;	
	uint16_t  ucPlayVoiceValue;	
	uint16_t  ucSendVoiceOVcnt;	// �������ű�־�л���ʱ
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
 uint8_t VoicePromptAutoModeFlag:1; // �Զ�ģʽ
 uint8_t VoicePromptTurboModeFlag:1;//��ˮģʽ
 uint8_t VoicePromptSleepREQFlag:1;// �����У������·Żص���
 uint8_t VoicePromptPowerOffFlag:1;// �ѹػ�
 uint8_t VoicePromptReqCleanFlag:1; //��Żص��������
 uint8_t VoicePromptLowPowerFlag:1;//��ص�����
 uint8_t VoicePromptLowPowerChargeFlag:1;//�뼰ʱ��� 
 uint8_t VoicePromptPowerOverFlag:1;//�����Ѻľ�
 uint8_t VoicePromptStartChargeFlag:1;// ��ʼ���
 uint8_t VoicePromptCleanessDetFlag:1;// �����ܵ��ʹ�����
 uint8_t VoicePromptLowPowerCleanFlag:1; // ����������	
 uint8_t VoicePromptStartCleanFlag:1; //��ʼ�����
 uint8_t VoicePromptBrushCleaningFlag:1;//��ˢ�����
 uint8_t VoicePromptTubeCleaningFlag:1;//�ܵ������
 uint8_t VoicePromptIntelligentDetFlag:1;//���ܼ����
 uint8_t VoicePromptDeepCleaningFlag:1;// ��������
 uint8_t VoicePromptCleanOverFlag:1;// ��������
 uint8_t VoicePromptCleanCloseFlag:1;//������ѹر�
 uint8_t VoicePromptDirtyCleanFlag:1;// ��������ˮ��
 uint8_t VoicePromptCleanDirtyTubeFlag:1;// ��������ˮ��͵ײ��ܵ�
 uint8_t VoicePromptVolumeLowFlag:1;//������
 uint8_t VoicePromptVolumeHighFlag:1;//������
 uint8_t VoicePromptCloseVolumeFlag:1;//�ر�����
 uint8_t VoicePromptOpenVolumeFlag:1;//��������
 uint8_t VoicePromptSettingLanguageFlag:1;//��ʼ�������ԣ��̰��л�������ȷ��
 uint8_t VoicePromptLanguageSuccessFlag:1; //�������óɹ�
 uint8_t VoicePromptCboxNoWaterFlag:1;//��ˮ��ȱˮ���뼰ʱ��ˮ
 uint8_t VoicePromptNoSewageBoxFlag:1;//��ˮ��δ��װ���밲װ��ˮ��
 uint8_t VoicePromptSewageBoxFullFlag:1;//��ˮ������
 uint8_t VoicePromptNoBrushFlag:1;//��ˢδ��װ���밲װ��ˢ ,����������
 uint8_t VoicePromptChgBeforeDryFlag:1;//�������
 uint8_t VoicePromptBatTempOverFlag:1;//����¶ȹ��ߣ���ȴ����º�ʹ��
 uint8_t VoicePromptMachineErrorFlag:1;//�������ϣ�����ϵ�ۺ�ά��
 uint8_t VoicePromptSelfCleanPausedFlag:1;//��ͣ�����
 uint8_t VoicePromptContinueCleanFlag:1;//���������
 uint8_t VoicePromptCleanTubeFlag:1;//�������ˢ�͵ײ��ܵ�
 uint8_t VoicePromptDisinfectModeFlag:1;// ɱ��ģʽ
 uint8_t VoicePromptCheckFullClearFlag:1;// ����Һ�Ʊ��У���ȷ�����������ˮ��
 uint8_t VoicePromptSanitizerStayFlag:1;// ����Һ�Ʊ��У����Ժ�
 uint8_t VoicePromptSanitizerHalfFlag:1;// ����Һ���Ʊ�50%
 uint8_t VoicePromptSanitizerCompletedFlag:1;// ����Һ���Ʊ����
 uint8_t VoicePromptBrushDryFlag:1;//��ˢ�����
 uint8_t VoicePromptBrushDryOffFlag:1;// ��ɹ����ѹر�
 uint8_t VoicePromptBrushDryOverFlag:1;// ������
 uint8_t VoicePromptDirtyStartCLeanFlag:1;// ��ˢ���ܵ����ۣ��뿪ʼ�����
 
 uint8_t VoicePromptStartCleanBrushCleaningFlag:1;// ��ʼ������ˢ�����
 uint8_t VoicePromptStartCleanSolutionProdectFlag:1;// ��ʼ���������ˮ������
 uint8_t VoicePromptBrushDryOverCleanOverFlag:1;// ������ ��������
 uint8_t VoicePromptLowPowerLowPowerCleanFlag:1;// ��ص����ͣ�����������
 uint8_t VoicePromptSewageBoxFullDirtyCleanFlag:1;// ��ˮ��������������ˮ��
}VoiceList;

void Enable_OP_PIN(uint8_t enableoppin);
void Play_WT588F(KeyPara* keyparactl,VoicePara* voiceparactl,Eeprom_TypeDef* eepromparactl,VoiceList* voicelistflag);
uint8_t Read_VoiceBusy_PIN(void);
void SET_SCL_PIN(uint8_t enablesclpin);
void SET_SDA_PIN(uint8_t enablesclpin);

#endif

