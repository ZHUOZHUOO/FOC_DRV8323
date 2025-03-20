/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:31:34
 */

#ifndef UTIL_UART_H
#define UTIL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "usart.h"

    void UART_Send(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
    void UART_SendIT(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size);
    void UART_SendIForce(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size);
    void UART_InitDMA(UART_HandleTypeDef *huart);
    void UART_SendDMA(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size);
    HAL_StatusTypeDef UART_ReceiveDMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
    uint16_t UART_DMACurrentDataCounter(DMA_HandleTypeDef *dma_handle);
    void UART_ErrorHandler(uint32_t ret);

#ifdef __cplusplus
}
#endif

#endif
