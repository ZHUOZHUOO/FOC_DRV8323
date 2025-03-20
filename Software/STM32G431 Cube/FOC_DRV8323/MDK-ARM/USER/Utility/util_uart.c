/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:31:27
 */

#include "util_uart.h"

/**
* @brief         Init UART
* @param         huart
* @return        NULL
*/
void UART_Init(UART_HandleTypeDef *huart)
{
    uint32_t ret;
    if (HAL_UART_Init(huart) != HAL_OK)
    {
        UART_ErrorHandler(ret);
    }
}

/**
 * @brief      Sending information to UART (blocking mode)
 * @param      huart: UART handle
 * @param      txdata: The message to send
 * @param      size: The message length
 * @param      timeout: Timeout duration
 * @retval     NULL
 */
void UART_Send(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size, uint32_t timeout)
{
    if ((huart == NULL) || (txdata == NULL))
    {
        UART_ErrorHandler(HAL_ERROR);
    }
        
    uint32_t ret = HAL_UART_Transmit(huart, txdata, size, 10);
    if (ret != HAL_OK) 
    {
        UART_ErrorHandler(ret);
    }
}

/**
 * @brief      Sending information to UART (Non blocking mode)
 * @param      huart: UART handle
 * @param      txdata: The message to send
 * @param      size: The message length
 * @retval     NULL
 */
void UART_SendIT(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size)
{
    if ((huart == NULL) || (txdata == NULL))
    {
        UART_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_UART_Transmit_IT(huart, txdata, size);
    if (ret != HAL_OK) 
    {
        UART_ErrorHandler(ret);
    }
}

/**
 * @brief      Sending information to UART (Non blocking mode)��force waiting��may cause delay
 * @param      huart: UART handle
 * @param      txdata: The message to send
 * @param      size: The message length
 * @param      timeout: Timeout duration
 * @retval     NULL
 */
void UART_SendITForce(UART_HandleTypeDef *huart, uint8_t txdata[], uint16_t size)
{
    if ((huart == NULL) || (txdata == NULL))
    {
        UART_ErrorHandler(HAL_ERROR);
    }

    __HAL_UNLOCK(huart);
    uint32_t ret = HAL_UART_Transmit_IT(huart, txdata, size);
    if (ret != HAL_OK) 
    {
        UART_ErrorHandler(ret);
    }
}

/**
 * @brief      initialization UART DMA
 * @param      huart: UART handle
 * @retval     NULL
 */
void UART_InitDMA(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}

/**
  * @brief      Sending information to UART (blocking mode)
  * @param      huart: UART handle
  * @param      txdata: The message to send
  * @param      size: The message length
  * @param      timeout: Timeout duration
  * @retval     NULL
  */
void UART_SendDMA(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size) 
{
    if ((huart == NULL) || (txdata == NULL))
    {
        UART_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_UART_Transmit_DMA(huart, txdata, size);
    if (ret != HAL_OK) 
    {
        UART_ErrorHandler(ret);
    }
}

/**
 * @brief Receive an amount of data in DMA mode.
 * @note   When the UART parity is enabled (PCE = 1), the received data contain
 *         the parity bit (MSB position).
 * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
 *         the received data is handled as a set of u16. In this case, Size must indicate the number
 *         of u16 available through pData.
 * @param huart UART handle.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size  Amount of data elements (u8 or u16) to be received.
 * @retval HAL status
 */
HAL_StatusTypeDef UART_ReceiveDMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U)) 
        {
            return HAL_ERROR;
        }
            
        __HAL_LOCK(huart);
        huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
        if (!(IS_LPUART_INSTANCE(huart->Instance)))
        {
            if (READ_BIT(huart->Instance->CR2, USART_CR2_RTOEN) != 0U)
            {
                ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_RTOIE);
            }
        }
        return (UART_Start_Receive_DMA(huart, pData, Size));
    }
    else 
    {
        return HAL_BUSY;
    }
}

/**
 * @brief      returns the number of remaining data units in the current DMAy Streamx transfer.
 * @param      dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
 *             to 7 to select the DMA Stream.
 * @retval     The number of remaining data units in the current DMAy Streamx transfer.
 */
uint16_t UART_DMACurrentDataCounter(DMA_HandleTypeDef *dma_handle)
{
    return ((uint16_t)__HAL_DMA_GET_COUNTER(dma_handle));
}

/**
 * @brief      UART error handler
 * @param      ret: error data
 * @retval     NULL
 */
void UART_ErrorHandler(uint32_t ret)
{
    while (1) 
    {;}
}
