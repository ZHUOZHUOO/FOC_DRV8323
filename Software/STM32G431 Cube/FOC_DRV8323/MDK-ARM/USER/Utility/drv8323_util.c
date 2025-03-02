/*
 * @Date: 2025-03-01 15:00:33
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-01 22:11:45
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Utility\drv8323_util.c
 * @Description: Do not edit
 */
#include "drv8323_util.h"

void DRV8323_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_DeInit(DRV8323_PORT, DRV8323_CAL|DRV8323_nFault|LED_Pin);
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(DRV8323_PORT, DRV8323_CAL|DRV8323_nFault|LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : DRV8323_CAL|LED_Pin*/
    GPIO_InitStruct.Pin = DRV8323_CAL|LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : DRV8323_nFault */     
#if N_FAULT_MODE == MODE_ON                                                                                                                                                                                        
    GPIO_InitStruct.Pin = DRV8323_nFault;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#elif N_FAULT_MODE == MODE_OFF
    GPIO_InitStruct.Pin = DRV8323_nFault;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_Delay(1);
#endif
}

void DRV8323_CAL_Init(void)
{
    HAL_GPIO_WritePin(DRV8323_PORT, DRV8323_CAL, 1);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(DRV8323_PORT, DRV8323_CAL, 0);
}


