/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-10 16:17:57
 */

#include "util_spi.h"

/**
 * @brief          Initnation SPI
 * @param          hspi: The spi handle
 * @retval         NULL
 */
void SPI_Init(SPI_HandleTypeDef *hspi)
{
    uint32_t ret;
    if (HAL_SPI_Init(hspi) != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Send data or command to spi address
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_Send(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len, uint32_t timeout)
{
    if ((hspi == NULL) || (pData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_Transmit(hspi, pData, len, timeout);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Receive data or command to spi address
 * @param          hspi: The spi handle
 * @param          pData: To be received data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len, uint32_t timeout)
{
    if ((hspi == NULL) || (pData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_Receive(hspi, pData, len, timeout);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }   
}

/**
 * @brief          Send data or command to spi address(For DMA)
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_SendDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len)
{
    if ((hspi == NULL) || (pData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_Transmit_DMA(hspi, pData, len);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Receive data or command to spi address(For DMA)
 * @param          hspi: The spi handle
 * @param          pData: To be received data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_ReceiveDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len)
{
    if ((hspi == NULL) || (pData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_Receive_DMA(hspi, pData, len);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Swap data or command to spi address
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_Swap(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len, uint32_t timeout)
{
    if ((hspi == NULL) || (pTxData == NULL) || (pRxData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, len, timeout);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Swap data or command to spi address(For DMA)
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_SwapDMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len)
{
    if ((hspi == NULL) || (pTxData == NULL) || (pRxData == NULL))
    {
        SPI_ErrorHandler(HAL_ERROR);
    }

    uint32_t ret = HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, len);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
}

/**
 * @brief          Swap a data or command to spi address
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
uint8_t SPI_SwapAbyte(SPI_HandleTypeDef *hspi, uint8_t txdata)
{
    uint8_t rx_data;
    uint32_t ret = HAL_SPI_TransmitReceive(hspi, &txdata, &rx_data, 1, 100);
    if (ret != HAL_OK)
    {
        SPI_ErrorHandler(ret);
    }
    return rx_data;
}

/**
 * @brief          Swap muli data or command to spi address
 * @param          hspi: The spi handle
 * @param          pData: To be transmit data
 * @param          len: The data length
 * @retval         NULL
 */
void SPI_ReadMuliReg(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint8_t len)
{
    while (len != 0)
    {
        *rx_data = SPI_SwapAbyte(hspi, 0x55);
        rx_data++;
        len--;
    }
}

/**
 * @brief      SPI error handler
 * @param      ret: error data
 * @retval     NULL
 */
void SPI_ErrorHandler(uint32_t ret)
{
    while (1) 
    {;}
}
