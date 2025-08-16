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
	#define DRV8323_ENABLE_PORT GPIOA
	#define DRV8323_ENABLE GPIO_PIN_5
	#define DRV8323_nFault_PORT GPIOA
	#define DRV8323_nFault GPIO_PIN_6
	#define LED_PORT GPIOB
	#define LED_Pin GPIO_PIN_6
	#define SPI_CS_PORT GPIOA
	#define SPI_CS GPIO_PIN_15
#elif HARDWARE_VERSION == VERSION_2
	#define DRV8323_CAL_PORT GPIOA
	#define DRV8323_CAL GPIO_PIN_4
	#define DRV8323_ENABLE_PORT GPIOA
	#define DRV8323_ENABLE GPIO_PIN_5
	#define DRV8323_nFault_PORT GPIOA
	#define DRV8323_nFault GPIO_PIN_6
	#define LED_PORT GPIOF
	#define LED_Pin GPIO_PIN_1
	#define SPI_CS_PORT GPIOA
	#define SPI_CS GPIO_PIN_15
#endif

#define DRV8323_GAIN 20 //放大器增益
#define CURRENT_DETECTION_RES 0.1f//检流电阻
#define DRV8323_ADC_GAIN (DRV8323_GAIN * CURRENT_DETECTION_RES)
#define DRV8323_VREF 3.32f
#define DRV8323_VREF_DIV_TWO 1.66f


void DRV8323_GPIO_Init(void);						//初始化GPIO
void DRV8323_Init(void);						//运放校准, 保持高电平
void DRV8323_Enable(void);
void DRV8323_Disable(void);
void DRV8323_CAL_Align(void);

#endif
