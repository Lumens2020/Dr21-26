#ifndef __CLEAN_IWDG_H__
#define __CLEAN_IWDG_H__

#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

void PVU_CheckStatus(void);
void RVU_CheckStatus(void);
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload);
void Write_Iwdg_RL(void);
static void DelayLoop(u32 uldelay);
#endif //__CLEAN_IWDG_H__


