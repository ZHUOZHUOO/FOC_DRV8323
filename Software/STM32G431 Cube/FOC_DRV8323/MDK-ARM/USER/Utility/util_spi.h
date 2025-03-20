/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2023-12-28 00:37:49
 */

#ifndef UTIL_SPI_H
#define UTIL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "spi.h"
#include "dma.h"

void SPI_Init(SPI_HandleTypeDef *hspi);
void SPI_Send(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len, uint32_t timeout);
void SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len, uint32_t timeout);
void SPI_SendDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len);
void SPI_ReceiveDMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t len);
void SPI_Swap(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len, uint32_t timeout);
void SPI_SwapDMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t len);
uint8_t SPI_SwapAbyte(SPI_HandleTypeDef *hspi, uint8_t txdata);
void SPI_ReadMuliReg(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint8_t len);
void SPI_ErrorHandler(uint32_t ret);


#endif

#ifdef __cplusplus
}
#endif
