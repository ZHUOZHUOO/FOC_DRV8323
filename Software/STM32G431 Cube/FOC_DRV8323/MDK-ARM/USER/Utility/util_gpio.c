/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 16:47:29
 */

#include "util_gpio.h"

/**
 * @brief         Init GPIO    
 * @param         gpio
 * @return        NULL
 */
void GPIO_Init(GPIO_HandleTypeDef *gpio, GPIO_TypeDef *handle, uint16_t pin)
{
    gpio->state = GPIO_PIN_RESET;
    gpio->handle = handle;
    gpio->pin = pin;
}

/**
 * @brief         Reset GPIO    
 * @param         gpio
 * @return        NULL
 */
void GPIO_Reset(GPIO_HandleTypeDef *gpio)
{
    HAL_GPIO_WritePin(gpio->handle, gpio->pin, GPIO_PIN_RESET);
    gpio->state = GPIO_ReadPin(gpio);
}

/**
 * @brief         Set GPIO
 * @param         gpio
 * @return        NULL
 */
void GPIO_Set(GPIO_HandleTypeDef *gpio)
{
    HAL_GPIO_WritePin(gpio->handle, gpio->pin, GPIO_PIN_SET);
    gpio->state = GPIO_ReadPin(gpio);
}

/**
 * @brief         Get GPIO pin state
 * @param         gpio
 * @return        GPIO_PINState
 */
GPIO_PinState GPIO_ReadPin(GPIO_HandleTypeDef *gpio)
{
	gpio->tick = HAL_GetTick();
    gpio->state = HAL_GPIO_ReadPin(gpio->handle, gpio->pin);
    return gpio->state;
}

/**
 * @brief         Get the GPIO pin trigger tick
 * @param         gpio
 * @return        [uint32_t]tick
 */
uint32_t GPIO_GetTriggerTick(GPIO_HandleTypeDef *gpio)
{
    return gpio->tick;
}


