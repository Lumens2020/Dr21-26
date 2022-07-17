#ifndef _VOICELIST_H
#define _VOICELIST_H



#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

#include "voice.h"



#define Voice_Change_Time       (600000/VOICE_TICKS_INTERVAL)
//#define Voice_Change_Time       (500*5/2)//(250*5/2)//625/
//#define Voice_Change_Time     (3000*5/2)//(250*5/2)//625/



void Play_VoiceList(VoiceList* voicelistflag,VoicePara* voicesetpara,KeyPara* keyparactl);

void Clear_VoiceFlag(VoiceList* voicelistflag);


#endif

