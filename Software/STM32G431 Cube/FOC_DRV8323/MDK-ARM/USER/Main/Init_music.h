/*
 * @Date: 2025-04-08 15:00:13
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-04-08 15:56:18
 * @FilePath: \FOC_DRV8323\MDK-ARM\USER\Main\Init_music.h
 * @Description: Do not edit
 */

#ifndef __INIT_MUSIC_H
#define __INIT_MUSIC_H

#include "configure.h"
#include "tim.h"
#include "sys_dwt.h"

#define TIM1_AUTORELOAD (uint32_t)170 //定时器自动重装载值
#define TIMER1_MAX_ARR  (uint32_t)PWM_PERIOD //定时器最大重装载值
#define BASE_FREQ       (uint32_t)CKTIM/TIM1_AUTORELOAD //音乐基频 1MHz

void setVolume(uint8_t volume);
void playStartupTune(void);

#endif /* __INIT_MUSIC_H */
