/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:26:16
 */

#ifndef UTIL_PWM_H
#define UTIL_PWM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "tim.h"

    typedef enum
    {
        PWM_OFF = 0,
        PWM_ON = 1
    } PWM_StateEnum;

    typedef struct
    {
        PWM_StateEnum state;
        TIM_HandleTypeDef *htim;
        TIM_OC_InitTypeDef conf;
        uint32_t ch;
        uint32_t clk;
        uint32_t freq;
        float duty;
    } PWM_HandleTypeDef;

    void PWM_Init(PWM_HandleTypeDef *pwm, TIM_HandleTypeDef *htim, uint32_t ch, uint32_t clk);
    void PWM_Start(PWM_HandleTypeDef *pwm);
    void PWM_Stop(PWM_HandleTypeDef *pwm);
    void PWM_SetDuty(PWM_HandleTypeDef *pwm, float duty);
    void PWM_SetFreq(PWM_HandleTypeDef *pwm, uint32_t freq);


#ifdef __cplusplus
}
#endif

#endif
