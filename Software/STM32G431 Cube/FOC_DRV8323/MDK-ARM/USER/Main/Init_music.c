/*
 * @Date: 2025-04-08 15:00:13
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-04-08 18:03:43
 * @FilePath: \FOC_DRV8323\MDK-ARM\USER\Main\Init_music.c
 * @Description: Do not edit
 */

#include "Init_music.h"

#define MUSIC_TYPE INIT_MUSIC

#define INIT_MUSIC 0
#define CHUN_RI_YING 1 //春日影

float score[] = {0,262,294,330,349,392,415,440,494,0,0,523,587,659,698,784,880,988,0,0,1046,1175,1318,1397,1558,1760,1976};


#if MUSIC_TYPE == INIT_MUSIC
float time_p = 1.0f;

int melody[] = {7,11,15};
int duration[] = {1,1,1};

#elif MUSIC_TYPE == CHUN_RI_YING
float time_p = 6 / 8.0f;

int melody[] = {
	13,12,11,12,13,14,13,12,
	13,12,11,12,13,14,13,12,
	13,12,11,12,13,14,13,12,
	13,12,11,12,13,14,13,12,11,11,
	13,13,12,14,13,12,12,12,11,11,14,13,12,
	12,11,12,13,0,13,15,21,
	17,11,17,11,17,16,15,15,12,14,
	14,13,13,5,14,13,12,13,15,
	11,0
};

int duration[] = {
	4,2,4,2,3,1,2,6,
	4,2,4,2,3,1,2,6,
	4,2,4,2,3,1,2,6,
	4,2,4,2,3,1,2,4,1,1,
	2,2,2,2,2,2,2,2,1,1,2,2,2,
	4,1,1,6,6,2,2,2,
	4,2,4,2,1,1,4,2,2,2,
	4,2,4,2,2,2,2,4,2,
	6,4
};

#endif

uint8_t beep_volume;
int noteIndex = 0;

void Ms_Delay(uint32_t ms) {
//	for(uint32_t i = 0; i < BASE_FREQ / 50 * ms; i++){}
	DWT_Delayms(ms);
}

uint32_t Hz2PSC(float freq) {
	uint32_t TIMPSC = 0;
	TIMPSC = BASE_FREQ / (freq * 4 + 600); // 升高12个大调
	return TIMPSC;
}

void setVolume(uint8_t volume) {
	if (volume > 40) {
		volume = 40;
	}
	if (volume < 0) {
		volume = 0;
	}
	beep_volume = volume * 2;           // volume variable from 0 - 11 equates to CCR value of 0-22
}

void setCaptureCompare() {
	TIM1->CCR1 = beep_volume; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = beep_volume;
	TIM1->CCR3 = beep_volume;
}

void allOff() {
    TIM1->CCR1 = 0; // volume of the beep, (duty cycle) don't go above 25 out of 2000蜂鸣声的音量(占空比)在2000中不超过25
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;
}

void comStep(uint8_t step) {
    switch (step) {
    case 1:
        TIM1->CCR1 = beep_volume;
        TIM1->CCR2 = 0;
        TIM1->CCR3 = 0;
        break;
    case 2:
        TIM1->CCR1 = beep_volume;
        TIM1->CCR2 = beep_volume;
        TIM1->CCR3 = 0;
        break;
    case 3:
        TIM1->CCR1 = 0;
        TIM1->CCR2 = beep_volume;
        TIM1->CCR3 = 0; 
    case 4:
        TIM1->CCR1 = 0;
        TIM1->CCR2 = beep_volume;
        TIM1->CCR3 = beep_volume;
        break;
    case 5:
        TIM1->CCR1 = 0;
        TIM1->CCR2 = 0;
        TIM1->CCR3 = beep_volume;
        break;
    case 6:
        TIM1->CCR1 = beep_volume;
        TIM1->CCR2 = 0;
        TIM1->CCR3 = beep_volume;
        break;
    default:
        break;
    }
}

uint16_t getBlueJayNoteFrequency(uint8_t bjarrayfreq) {
	return 10000000 / (bjarrayfreq * 247 + 4000);
}

void playStartupTune() {
	__disable_irq();
		uint32_t SYS_ARR = TIM1->ARR;
		uint32_t SYS_PSC = TIM1->PSC;
		TIM1->ARR = TIM1_AUTORELOAD;
		setCaptureCompare();

		for(;;)
		{
//			comStep(noteIndex % 6 + 1); // activate a pwm channel
			comStep(1);
			if(melody[noteIndex] == 0){
				TIM1->PSC = 0;
			}
			else{
				TIM1->PSC = Hz2PSC(score[melody[noteIndex]]);// frequency of beep
			}				
			Ms_Delay((uint32_t)(duration[noteIndex] * time_p * 250)); // duration of beep
			noteIndex++;
			if (noteIndex >= sizeof(melody) / sizeof(melody[0])) {  //循环播放
				noteIndex = 0;
				break;
			}
		}

		allOff();                // turn all channels low again
		TIM1->PSC = SYS_PSC;           // set prescaler back to 0.
		TIM1->ARR = SYS_ARR;
	__enable_irq();
}
