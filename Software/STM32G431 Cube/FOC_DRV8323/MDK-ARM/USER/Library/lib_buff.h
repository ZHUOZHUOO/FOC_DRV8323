/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:45:36
 */

#ifndef LIB_BUFF_H
#define LIB_BUFF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "string.h"

    float buff2float(uint8_t *buff);
    void float2buff(float f, uint8_t *buff);
    int16_t buff2i16(uint8_t *buff);
    uint16_t buff2ui16(uint8_t *buff);
    void i162buff(int16_t u, uint8_t *buff);
    void ui162buff(uint16_t u, uint8_t *buff);
    uint32_t buff2ui32(uint8_t *buff);
    void ui322buff(uint32_t u, uint8_t *buff);

#ifdef __cplusplus
}
#endif

#endif
