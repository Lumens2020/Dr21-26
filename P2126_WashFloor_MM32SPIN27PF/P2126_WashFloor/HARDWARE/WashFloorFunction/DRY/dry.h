#ifndef _DRY_H_
#define _DRY_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "config.h"

// 1: 最新烘干通讯协议 20211101
// 0: 老通讯协议，故障拉低电平
//#define DRY_BUS_MODE  1

#define DRY_TICKS_INTERVAL  10

#define DRY_ERROR_TIME     (400/DRY_TICKS_INTERVAL)

typedef struct{
	uint8_t bEnalbeDryFlag:1;      // 烘干使能
	uint8_t bDryPowerFlag:1;       // 烘干电源状态
	uint8_t bDryStallErrFlag:1;    // 烘干故障
	uint8_t dry_comm_toggle_flag:1;// 烘干通讯脚翻转
	uint16_t dry_low_cnt;           // 烘干故障计时
	uint16_t dry_low_ERROR_cnt;        
	uint16_t dry_low_cnt_H;           
}DryPara;

void Set_Dry_ON(DryPara* dryctlpara);
void Set_Dry_OFF(DryPara* dryctlpara);
void Control_Dry(DryPara* dryctlpara);
void Test_Control_Dry(DryPara* dryctlpara);
#endif

