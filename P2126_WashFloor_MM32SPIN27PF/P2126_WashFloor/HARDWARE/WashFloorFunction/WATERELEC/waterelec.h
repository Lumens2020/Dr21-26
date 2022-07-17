#ifndef _WATERELEC_H_
#define _WATERELEC_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "voicelist.h"
#include "control.h"
#include "config.h"

//#define WaterElecTestEnable 0


#define WATERELEC_TICKS_INTERVAL  10
#define WaterElec_WorkTime       (30000/WATERELEC_TICKS_INTERVAL)
#define WaterElec_ERRORTime      (1000/WATERELEC_TICKS_INTERVAL)

typedef struct{
 uint8_t  bEnalbeWaterElecFlag:1;
 uint8_t  bWaterElecStallErrFlag:1;
 uint8_t  waterelec_toggle_flag:1;
 uint8_t  waterelec_OK_flag:1;
 uint8_t  waterelec_OK_flag_back:1;
 uint8_t  waterelec_low_cnt;
 uint16_t waterelec_work_cnt;
 uint16_t waterelec_work_cnt_cntdn;
 uint16_t waterelec_work_open_cnt;// 20211202
 uint16_t waterelec_work_close_cnt;// 20211202
}WaterElecPara;

void Set_WaterElec_ON(WaterElecPara* Wateeelecctlpara);
void Set_WaterElec_OFF(WaterElecPara* Wateeelecctlpara);
void Control_WaterElec(WaterElecPara* Wateeelecctlpara,VoiceList* voicelistpara,WorkPara* workstate);
void Control_WaterElec_Test(WaterElecPara* Wateeelecctlpara,VoiceList* voicelistpara,WorkPara* workstate);
#endif

