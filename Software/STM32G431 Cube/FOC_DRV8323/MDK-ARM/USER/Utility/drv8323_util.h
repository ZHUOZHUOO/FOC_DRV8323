/*
 * @Date: 2025-03-01 15:00:33
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-01 16:59:45
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Utility\drv8323_util.h
 * @Description: Do not edit
 */
#ifndef __DRV8323_UTIL_H
#define __DRV8323_UTIL_H

#include "configure.h"
#include "stm32g4xx_hal.h"

//DRV8323_SETTING
#if HARDWARE_VERSION == VERSION_1
	#define DRV8323_CAL_PORT GPIOA
	#define DRV8323_CAL GPIO_PIN_4
	#define DRV8323_nFault_PORT GPIOA
	#define DRV8323_nFault GPIO_PIN_6
	#define LED_PORT GPIOA
	#define LED_Pin GPIO_PIN_5
#elif HARDWARE_VERSION == VERSION_2
	#define DRV8323_CAL_PORT GPIOA
	#define DRV8323_CAL GPIO_PIN_4
	#define DRV8323_nFault_PORT GPIOA
	#define DRV8323_nFault GPIO_PIN_5
	#define LED_PORT GPIOA
	#define LED_Pin GPIO_PIN_6
#elif HARDWARE_VERSION == VERSION_3
	#define DRV8323_CAL_PORT GPIOA
	#define DRV8323_CAL GPIO_PIN_4
	#define DRV8323_ENABLE_PORT GPIOA
	#define DRV8323_ENABLE GPIO_PIN_5
	#define DRV8323_nFault_PORT GPIOA
	#define DRV8323_nFault GPIO_PIN_6
	#define LED_PORT GPIOB
	#define LED_Pin GPIO_PIN_6
#endif

#define DRV8323_GAIN 40

void DRV8323_GPIO_Init(void);						//初始化GPIO
void DRV8323_CAL_Init(void);						//放大器初始化, 保持高电平

#endif
