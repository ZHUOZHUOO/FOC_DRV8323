/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 20:56:13
 */
 
#include "util_pwm.h"

/**
 * @brief      Init PWM
 * @param      pwm: The pointer points to the PWM object to initialize
 * @param      htim:Timer object for PWM
 * @param      ch:  The PWM channel
 * @retval     NULL
 */
void PWM_Init(PWM_HandleTypeDef *pwm, TIM_HandleTypeDef *htim, uint32_t ch, uint32_t clk)
{
    pwm->state = PWM_OFF;
    pwm->htim = htim;
    pwm->ch = ch;
    pwm->clk = clk; 
    pwm->conf.OCMode = TIM_OCMODE_PWM1;
    pwm->conf.OCPolarity = TIM_OCPOLARITY_HIGH;
    pwm->conf.OCFastMode = TIM_OCFAST_DISABLE;
    pwm->duty = 0.0;
}

/**
 * @brief      Start PWM output
 * @param      pwm: The pointer points to the PWM object to initialize
 * @retval     NULL
 */
void PWM_Start(PWM_HandleTypeDef *pwm)
{
    if (pwm->state == PWM_OFF)
    {
        HAL_TIM_PWM_Start(pwm->htim, pwm->ch);
        pwm->state = PWM_ON;
    }
}

/**
 * @brief      Stop PWM output
 * @param      pwm: The pointer points to the PWM object to initialize
 * @retval     NULL
 */
void PWM_Stop(PWM_HandleTypeDef *pwm)
{
    if (pwm->state == PWM_ON)
    {
        HAL_TIM_PWM_Stop(pwm->htim, pwm->ch);
        pwm->state = PWM_OFF;
    }
}

/**
 * @brief      Setting PWM duty
 * @param      pwm: The pointer points to the PWM object to initialize
 * @param      duty: PWM duty
 * @retval     NULL
 */
void PWM_SetDuty(PWM_HandleTypeDef *pwm, float duty)
{
    PWM_StateEnum last_state = pwm->state;
    PWM_Stop(pwm);
    pwm->duty = duty;
    pwm->conf.Pulse = pwm->duty * (pwm->htim->Init.Period + 1);
    HAL_TIM_PWM_ConfigChannel(pwm->htim, &(pwm->conf), pwm->ch);
    if (last_state == PWM_ON) 
    {
        PWM_Start(pwm);
    }
}

/**
 * @brief      Setting PWM frequency
 * @param      pwm: The pointer points to the PWM object to initialize
 * @param      freq: PWM frequency
 * @retval     NULL
 */
void PWM_SetFreq(PWM_HandleTypeDef *pwm, uint32_t freq)
{
    PWM_StateEnum last_state = pwm->state;
    PWM_Stop(pwm);
    pwm->freq = freq;
    pwm->htim->Init.Prescaler = pwm->clk / (pwm->htim->Init.Period + 1) / freq - 1;
    HAL_TIM_PWM_Init(pwm->htim);
    PWM_SetDuty(pwm, pwm->duty);
    if (last_state == PWM_ON) 
    {
        PWM_Start(pwm);
    }
}
