#ifndef _ERROR_H_
#define _ERROR_H_


#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#define Recover_WBLANK_WARING_TIME  (120*2)

#define Work_WBLANK_WARING_TIME     (900*2)
#define Work_WBLANK_WARING_TIME1    (600*2)
#define Work_WBLANK_WARING_TIME2    (300*2)
#define Work_WBLANK_WARING_TIME3    (20*2)

void Play_Service_ERROR(void);
void Play_Running_ERROR(void);
void Play_SOC_ERROR(void);
#endif

