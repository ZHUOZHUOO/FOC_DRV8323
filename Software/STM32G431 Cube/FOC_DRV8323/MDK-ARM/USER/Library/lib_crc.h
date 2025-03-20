/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:46:03
 */

#ifndef LIB_CRC_H
#define LIB_CRC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32g4xx_hal.h"

    extern unsigned char CRC8;
    extern unsigned char CRC8_INIT;
    extern uint16_t CRC16;
    extern uint16_t CRC16_INIT;

    typedef enum
    {
        NOT_MATCH = 0,
        MATCH = 1
    } CRC_MatchEnum;

    unsigned char CRC_GetCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength, char ucCRC8);
    unsigned int CRC_VerifyCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
    void CRC_AppendCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
    uint16_t CRC_GetCRC16CheckSum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
    unsigned int CRC_VerifyCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);
    void CRC_AppendCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);

#ifdef __cplusplus
}
#endif

#endif
