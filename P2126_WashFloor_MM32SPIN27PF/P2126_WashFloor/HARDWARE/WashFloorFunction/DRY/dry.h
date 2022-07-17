#ifndef _DRY_H_
#define _DRY_H_

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"
#include "config.h"

// 1: ���º��ͨѶЭ�� 20211101
// 0: ��ͨѶЭ�飬�������͵�ƽ
//#define DRY_BUS_MODE  1

#define DRY_TICKS_INTERVAL  10

#define DRY_ERROR_TIME     (400/DRY_TICKS_INTERVAL)

typedef struct{
	uint8_t bEnalbeDryFlag:1;      // ���ʹ��
	uint8_t bDryPowerFlag:1;       // ��ɵ�Դ״̬
	uint8_t bDryStallErrFlag:1;    // ��ɹ���
	uint8_t dry_comm_toggle_flag:1;// ���ͨѶ�ŷ�ת
	uint16_t dry_low_cnt;           // ��ɹ��ϼ�ʱ
	uint16_t dry_low_ERROR_cnt;        
	uint16_t dry_low_cnt_H;           
}DryPara;

void Set_Dry_ON(DryPara* dryctlpara);
void Set_Dry_OFF(DryPara* dryctlpara);
void Control_Dry(DryPara* dryctlpara);
void Test_Control_Dry(DryPara* dryctlpara);
#endif

