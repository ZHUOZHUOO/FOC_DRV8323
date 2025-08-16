/*
 * @Date: 2025-03-01 15:00:33
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-01 22:11:45
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Utility\drv8323_util.c
 * @Description: Do not edit
 */
#include "drv8323_util.h"


void DRV8323_Enable(void)
{
		HAL_GPIO_WritePin(DRV8323_ENABLE_PORT, DRV8323_ENABLE, GPIO_PIN_SET);
		HAL_Delay(10);
}

void DRV8323_Disable(void)
{
		HAL_GPIO_WritePin(DRV8323_ENABLE_PORT, DRV8323_ENABLE, GPIO_PIN_RESET);
		HAL_Delay(10);
}

void DRV8323_CAL_Align(void)
{
		HAL_Delay(100);
		HAL_GPIO_WritePin(DRV8323_CAL_PORT, DRV8323_CAL, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(DRV8323_CAL_PORT, DRV8323_CAL, GPIO_PIN_RESET);
}

void DRV8323_Init(void)
{
		DRV8323_GPIO_Init();
		DRV8323_Enable();
		DRV8323_CAL_Align();
}

void DRV8323_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_DeInit(DRV8323_CAL_PORT, DRV8323_CAL);
    HAL_GPIO_DeInit(DRV8323_nFault_PORT, DRV8323_nFault);
    HAL_GPIO_DeInit(LED_PORT, LED_Pin);

    HAL_GPIO_WritePin(DRV8323_CAL_PORT, DRV8323_CAL, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DRV8323_nFault_PORT, DRV8323_nFault, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED_Pin, GPIO_PIN_RESET);

#ifdef DRV8323_ENABLE
    HAL_GPIO_DeInit(DRV8323_ENABLE_PORT, DRV8323_ENABLE);
    HAL_GPIO_WritePin(DRV8323_ENABLE_PORT, DRV8323_ENABLE, GPIO_PIN_SET);
#endif

    /*Configure GPIO pins : DRV8323_CAL*/
    GPIO_InitStruct.Pin = DRV8323_CAL;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DRV8323_CAL_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : LED_Pin */
    GPIO_InitStruct.Pin = LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : DRV8323_ENABLE */
#ifdef DRV8323_ENABLE
    GPIO_InitStruct.Pin = DRV8323_ENABLE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DRV8323_ENABLE_PORT, &GPIO_InitStruct);
#endif

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
		
//		/*Configure GPIO pins : SPI_CS */
//#ifdef DRV8323_ENABLE
//    GPIO_InitStruct.Pin = SPI_CS;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(SPI_CS_PORT, &GPIO_InitStruct);
//#endif
}
