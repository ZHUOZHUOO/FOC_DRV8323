/*
 * @Date: 2025-04-08 15:00:13
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-04-08 18:03:43
 * @FilePath: \FOC_DRV8323\MDK-ARM\USER\Main\Init_music.c
 * @Description: Do not edit
 */

#include "Init_music.h"


uint8_t beep_volume;

//uint8_t blueJayTuneBuffer[128] = {};

void Ms_Delay(uint32_t ms) {
	for(uint32_t i = 0; i<10000000; i++){}
}

void pause(uint16_t ms) {
	TIM1->CCR1 = 0; // volume of the beep, (duty cycle) don't go above 25
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;

	Ms_Delay(ms);
	TIM1->CCR1 = beep_volume; // volume of the beep, (duty cycle) don't go above 25 out of 2000蜂鸣声的音量(占空比)在2000中不超过25
	TIM1->CCR2 = beep_volume;
	TIM1->CCR3 = beep_volume;
}


void setVolume(uint8_t volume) {
	if (volume > 20) {
		volume = 20;
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

void playBJNote(uint16_t freq, uint16_t bduration) {       // hz and ms
	uint16_t timerOne_reload = TIM1_AUTORELOAD;

	TIM1->PSC = 10;
	timerOne_reload = CKTIM / TIM1->PSC / freq;

	TIM1->ARR = timerOne_reload;
	TIM1->CCR1 = beep_volume * timerOne_reload / TIM1_AUTORELOAD ; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = beep_volume * timerOne_reload / TIM1_AUTORELOAD;
	TIM1->CCR3 = beep_volume * timerOne_reload / TIM1_AUTORELOAD;

	Ms_Delay(bduration);
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
		TIM1->ARR = TIM1_AUTORELOAD;
		setCaptureCompare();
		comStep(3);       // activate a pwm channel
		TIM1->PSC = 105;        // frequency of beep
		Ms_Delay(200);         // duration of beep

		comStep(5);
		TIM1->PSC = 70;            // next beep is higher frequency
		Ms_Delay(200);

		comStep(6);
		TIM1->PSC = 55;         // higher again..
		Ms_Delay(200);

		allOff();                // turn all channels low again
		TIM1->PSC = 0;           // set prescaler back to 0.


		TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playBrushedStartupTune() {
	__disable_irq();
	TIM1->ARR = TIM1_AUTORELOAD;
	setCaptureCompare();
	comStep(1);       // activate a pwm channel
	TIM1->PSC = 40;        // frequency of beep
	Ms_Delay(300);         // duration of beep
	comStep(2);       // activate a pwm channel
	TIM1->PSC = 30;        // frequency of beep
	Ms_Delay(300);         // duration of beep
	comStep(3);       // activate a pwm channel
	TIM1->PSC = 25;        // frequency of beep
	Ms_Delay(300);         // duration of beep
	comStep(4);
	TIM1->PSC = 20;         // higher again..
	Ms_Delay(300);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
//	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playDuskingTune() {
	setCaptureCompare();
	TIM1->ARR = TIM1_AUTORELOAD;
	comStep(2);       // activate a pwm channel
	TIM1->PSC = 60;        // frequency of beep
	Ms_Delay(200);         // duration of beep
	TIM1->PSC = 55;            // next beep is higher frequency
	Ms_Delay(150);
	TIM1->PSC = 50;         // higher again..
	Ms_Delay(150);
	TIM1->PSC = 45;        // frequency of beep
	Ms_Delay(100);         // duration of beep
	TIM1->PSC = 50;            // next beep is higher frequency
	Ms_Delay(100);
	TIM1->PSC = 55;         // higher again..
	Ms_Delay(100);
	TIM1->PSC = 25;         // higher again..
	Ms_Delay(200);
	TIM1->PSC = 55;         // higher again..
	Ms_Delay(150);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	TIM1->ARR = TIMER1_MAX_ARR;
}

void playInputTune2() {
	TIM1->ARR = TIM1_AUTORELOAD;
	__disable_irq();
//	LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 60;
	setCaptureCompare();
	comStep(1);
	Ms_Delay(75);
	TIM1->PSC = 80;
	Ms_Delay(75);
	TIM1->PSC = 90;
//	LL_IWDG_ReloadCounter(IWDG);
	Ms_Delay(75);
	allOff();
	TIM1->PSC = 0;
//	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playInputTune() {
	__disable_irq();
	TIM1->ARR = TIM1_AUTORELOAD;
//	LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 80;
	setCaptureCompare();
	comStep(3);
	Ms_Delay(100);
	TIM1->PSC = 70;
	Ms_Delay(100);
	TIM1->PSC = 40;
	Ms_Delay(100);
	allOff();
	TIM1->PSC = 0;
//	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playDefaultTone() {
	TIM1->ARR = TIM1_AUTORELOAD;
	TIM1->PSC = 50;
	setCaptureCompare();
	comStep(2);
	Ms_Delay(150);
//	LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 30;
	Ms_Delay(150);
	allOff();
	TIM1->PSC = 0;
//	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;

}

void playChangedTone() {
	TIM1->ARR = TIM1_AUTORELOAD;
	TIM1->PSC = 40;
	setCaptureCompare();
	comStep(2);
	Ms_Delay(150);
//	LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 80;
	Ms_Delay(150);
	allOff();
	TIM1->PSC = 0;
//	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;

}

void playBeaconTune3() {
	TIM1->ARR = TIM1_AUTORELOAD;
	__disable_irq();
	setCaptureCompare();
	for (int i = 119 ; i > 0 ; i = i - 2) {
//		LL_IWDG_ReloadCounter(IWDG);
		comStep(i / 20);
		TIM1->PSC = 10 + (i / 2);
		Ms_Delay(10);
	}
	allOff();
	TIM1->PSC = 0;

	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}
