#ifndef _CONTROL_H_
#define _CONTROL_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "config.h"


//#define TEST_LIFE_ENABLE 0 // ��������


#define ERROR_PLAY_CNT          2


#define OFF_TICKS_INTERVAL               100
#define OFF_MACHINE_CNT                  (2000/OFF_TICKS_INTERVAL)

//********************************ϵͳ����********************************
#define STEP0		0
#define STEP1		1
#define STEP2		2
#define STEP3		3
#define STEP4		4
#define STEP5		5
#define STEP6		6
#define STEP7		7
#define STEP8		8
#define STEP9		9
#define STEP10	10
#define STEPELEC	11
//***********************************�Զ����******************************************
#define AUTO_TICKS_INTERVAL              10
#define AUTO_STEP0_TIME					(30000/AUTO_TICKS_INTERVAL)			//unit:10ms   20S
#define AUTO_STEP1_TIME					(20000/AUTO_TICKS_INTERVAL)			//unit:10ms   20S
/////////////////////////////20220322//////////////////////////////////////
#define AUTO_STEP2_TIME					(20000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 10S
#define AUTO_STEP3_TIME					(20000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 20S
#define AUTO_STEP3_1_TIME					(5000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 20S
#define AUTO_STEP4_TIME					(80000/AUTO_TICKS_INTERVAL)//(60000/AUTO_TICKS_INTERVAL)         // (60000/AUTO_TICKS_INTERVAL)			//unit:10ms	 60S
#define AUTO_STEP5_TIME					(30000/AUTO_TICKS_INTERVAL)			//unit:10ms	 30S
#define AUTO_STEP6_TIME					(7200000/AUTO_TICKS_INTERVAL)       //(3600000/AUTO_TICKS_INTERVAL)		//unit:10ms	 1h
#define AUTO_STEP8_TIME					(2500/AUTO_TICKS_INTERVAL)          //unit:10ms	 1h

#define AUTO_WARN_TIME                  (6000/AUTO_TICKS_INTERVAL)//(18000/AUTO_TICKS_INTERVAL)

#define AUTO_STAY_TIME                  (300000/AUTO_TICKS_INTERVAL)//(300000/AUTO_TICKS_INTERVAL)

#define AUTO_MAX_TIME					(AUTO_STEP6_TIME+10000)//(3700000/AUTO_TICKS_INTERVAL)		//unit:10ms	 
#define AUTO_BREAK_TIME					(2500/AUTO_TICKS_INTERVAL)			//unit:10ms 	 2.5S
#define AUTO_WAIT_TIME					(60000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 60S
#define AUTO_PUMP15_TIME				(15000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 15S
#define AUTO_PUMP5_TIME					(5000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 5S
//��ɲ���
#define AUTO_DRY_BRUSH_PERIOD           (240000/AUTO_TICKS_INTERVAL)           // ��ɵ�ˢ�����������
#define AUTO_DRY_BRUSH_TIME             (1000/AUTO_TICKS_INTERVAL)             // ��ɵ����ڹ���ʱ��
#define AUTO_DRY_BRUSH_CNT              (AUTO_STEP6_TIME/AUTO_DRY_BRUSH_PERIOD)// ��ɹ�������
//
#define CLEAN_STATUS_TIME				(300000/AUTO_TICKS_INTERVAL)		//unit:10ms 	 300S  ˮ����ϵȴ�������־�����ʱʱ��
//***********************************SOC******************************************
#define SOC_MIN_VALUE					    10			//�������͵���Ҫ��  10%
#define SOC_10_VALUE						10			//10%����ֵ
#define SOC_15_VALUE						15			//15%����ֵ
#define SOC_20_VALUE						20			//20%����ֵ
#define SOC_25_VALUE						25			//25%����ֵ
#define SOC_30_VALUE						30			//30%����ֵ
#define SOC_35_VALUE						35			//35%����ֵ
#define SOC_40_VALUE						40			//40%����ֵ
//*****************************************************************************
#define OFF_MACHINE_TIME					(3000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 5S
#define CLOSE_MACHINE_TIME					(300000/AUTO_TICKS_INTERVAL)			//unit:10ms 	 5S

typedef enum{
	AUTO_CLEAN_MODE=0x01,
	NON_AUTO_CLEAN_MODE=0x02,
	WAIT_AUTO_CLEAN_MODE=0x03,
	DEEP_AUTO_CLEAN_MODE=0x04
}AutoCleanStatus;

typedef enum{
	OPEN_STA=0x01,//����״̬
	CLOSE_STA=0x02,//�ػ�״̬
	IDLE_STA=0x03,//����״̬
	CHARGE_STA=0x04,//���״̬
	AUTO_STA=0x05,//�����״̬
	TEST_STA=0x06//����ȫ��״̬
}WorkSta;

typedef enum{
    NO_ERROR=0x00,
	BLDC_LV_ERROR=0x01,// BLDCǷѹ
	BLDC_OV_ERROR=0x02,// BLDC��ѹ
	BLDC_OT_ERROR=0x03,// BLDC����
	BLDC_OC_ERROR=0x04,// BLDC����
	BLDC_OS_ERROR=0x05,// BLDC����
	BLDC_LP_ERROE=0x06,// BLDCȱ��
	SMD_BLDC_ERROR=0x07,// ȫ�·��
	BRUSH_SHORT_ERROR=0x08,// ��ˢ��·
	PUMP_SHORT_ERROR=0x09,// ˮ�ö�·
	BMS_LV_ERROR=0x0A,// ���ű���
	BMS_OT_ERROR=0x0B,// �ŵ���±���
	BMS_N0_LOAD_ERROR=0x0C,// �ŵ���ر���
	MCU_COMM_ERROR=0x0D,// ͨѶ��ʱ
	Reseve_ERROR=0x0E,
	//  �ŵ����/��·/BLDC�ض�ʧ�ܱ�������Ļ�޷���ʾ
    CHG_OV_VMS_ERROR=0x10,// ���䱣��
    CHG_V_ERROR=0x11,// �������ѹ�쳣
    CHG_OC_ERROR=0x12,// ������
    CHG_OT_ERROR=0x13,// ��糬ʱ
    BMS_BAT_ERROR=0x20,// ��о����
    BMS_NTC_ERROR=0x21,// �¶ȴ�����(NTC)����
	BMS_COMM_ERROR=0x22,// ǰ��ͨѶʧ��(BMSӲ���쳣)
	DRY_ERROR=0x23,// ��ɹ���
	WATERELEC_ERROR=0x24,//���ˮ����
	PUMP_ERROR=0x30,// 
	BRUSH_ERROR=0x31// 
}ErrorSta;


typedef struct{
uint8_t xpServiceErrorFlag:1;
uint8_t xpWBoxErrorFlag:1;
uint8_t xpQBoxErrorFlag:1;
uint8_t xpRunErrorFlag:1;
uint8_t xpCommErrorFlag:1;
uint8_t xpBmsSleepErrorFlag:1;
uint8_t xpErrorMemoryEnable:1;// 20211122
uint8_t xpWBoxErrorcnt;
uint16_t xpWBoxErrorcnt1;
uint8_t xpErrorWarnCnt;
uint8_t xpErrorPlayCnt;
uint8_t xpErrorPlayCnt1;
uint8_t xpErrorPlayCnt2;
uint8_t xpErrorPlayCnt3;
uint8_t xpErrorPlayCnt4;
uint8_t xpErrorPlayCnt5;
uint8_t xpErrorPlayCnt6;
uint8_t xpErrorPlayCnt7;
uint8_t xpErrorPlayCnt8;
uint8_t xpErrorPlayCnt9;
uint8_t xpErrorPlayCnt10;
uint16_t xpErrorCnt1;
uint8_t xpErrorMemoryCnt;// 20211122
}WorkError;


typedef struct{
 uint8_t  bAutoCleanPauseFlag:1;// �������ͣ
 uint8_t  bAutoCleanFlag:1;// ��ͨ�����
 uint8_t  bAutoDeepCleanFlag:1;// ��������
 uint8_t  bAutoCleanStartFlag:1;// ����࿪ʼ��־
 uint8_t  bAutoCleanOverFlag:1;// ����������־
 uint8_t  xpAutoCleanStatus;// �����״̬
 uint8_t  xpAutoCleanPromptFlag;// �������ʾ״̬
 
 uint8_t  xpAutoCleanPlayFlag:1;
 uint8_t  xpAutoDrySoloFlag:1;
 uint8_t  xpAutoDryStayFlag:1;// 20211230
 uint32_t xpAutoCleanCount;
 uint32_t xpAutoCleanPromptCount;// �������ʾ����
 uint32_t xpAutoCleanPromptCount_back;// �������ʾ����
 uint32_t xpAutoCleanNorCount;
 uint32_t xpAutoCleanCount_back;
 uint32_t xpAutoCleanCount_back1;
 uint8_t  xpAutoCleanStep;
 uint8_t  xpAutoCleanStep_back;
 uint8_t  xpAutoCleanStep_back1;
 uint16_t  xpAutoCleanStatusCnt;
 uint32_t  xpAutoCleanStayCnt;
}AutoCleanValue;


typedef struct{
	uint8_t	   xpWorkOrderCLOSEFlag:1;
	uint8_t	   ucWorkOrderStatusback:1;
	uint8_t	   ucWorkLEDOnFlag:1; // 20220105
	uint8_t	   ucFirstONFlag:1;// ֱ���ػ��ٿ����������������
	uint16_t   ucWorkOrderoffcnt;
	uint8_t	   ucWorkStatus;// �ϴ���BMS LCD ����״̬
//	uint8_t	   ucbackWorkStatus;
	uint8_t	   ucWorkOrderStatus;// ���ع���״̬
//	uint8_t	   ucbackWorkOrderStatus;
	uint8_t	   ucSynchroCommCnt;
	uint16_t	   ucChangeStateCnt;// 20210922
	uint16_t	   ucChangeStateCnt1;// 20210922
	uint16_t	   ucChangeStateCnt2;// 20210922
	uint16_t	   ucChangeStateCnt3;// 20210922
	uint16_t	   ucChangeStateCnt4;// 20210929
	uint16_t	   ucChangeStateCnt5;// 20210929
    uint32_t   ucCloseMachinecnt;
    uint32_t   ucCloseScreencnt;
	uint16_t   ucplaylcd_cnt;
}WorkPara;

typedef struct{
	uint32_t Rcc_CR;
	uint8_t Rcc_CR_PLLMUL;
	uint8_t Rcc_CR_PLLON;
	uint8_t Rcc_CR_PLLDIV;
	uint32_t Rcc_CFGR;
	uint8_t Rcc_CFGR_SWSSW;
}RccReadPara;

void ControlAndCheck_Load(void);
void Deal_WorkProcess(void);
void Check_WorkStatus(void);
void Deal_CommStatus(void);

#endif

