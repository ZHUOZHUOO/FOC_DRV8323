/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-10 11:22:17
 */

#include "lib_buff.h"

/**
 * @brief      Extract float from byte buffer
 * @param      buff: Pointer to byte buffer
 * @retval     Extracted float
 */
float buff2float(uint8_t *buff)
{
    uint8_t tmpbuff[4];
    memcpy(tmpbuff, buff, 4);
    return *((float *)tmpbuff);
}

/**
 * @brief      Push float into byte buffer
 * @param      f: Float to be pressed in
 * @param      buff: Pointer to byte buffer
 * @retval     NULL
 */
void float2buff(float f, uint8_t *buff)
{
    uint8_t tmpbuff[4];
    *((float *)tmpbuff) = f;
    memcpy(buff, tmpbuff, 4);
}

/**
 * @brief      Extract int16 from byte buffer
 * @param      buff: Pointer to byte buffer
 * @retval     The extracted int16
 */
int16_t buff2i16(uint8_t *buff)
{
    uint8_t tmpbuff[2];
    memcpy(tmpbuff, buff, 2);
    return *((int16_t *)tmpbuff);
}

/**
 * @brief      Extract uint16 from byte buffer
 * @param      buff: Pointer to byte buffer
 * @retval     The extracted uint16
 */
uint16_t buff2ui16(uint8_t *buff)
{
    uint8_t tmpbuff[2];
    memcpy(tmpbuff, buff, 2);
    return *((uint16_t *)tmpbuff);
}

/**
 * @brief      Turn int16 push into byte buffer
 * @param      u: Uint16 T to be pressed in
 * @param      buff: Pointer to byte buffer
 * @retval     NULL
 */
void i162buff(int16_t u, uint8_t *buff)
{
    uint8_t tmpbuff[2];
    *((int16_t *)tmpbuff) = u;
    memcpy(buff, tmpbuff, 2);
}

/**
 * @brief      Turn uint16_ T push into byte buffer
 * @param      u: Uint16 T to be pressed in
 * @param      buff: Pointer to byte buffer
 * @retval     NULL
 */
void ui162buff(uint16_t u, uint8_t *buff)
{
    uint8_t tmpbuff[2];
    *((uint16_t *)tmpbuff) = u;
    memcpy(buff, tmpbuff, 2);
}

/**
 * @brief      Extract uint32 from byte buffer
 * @param      buff: Pointer to byte buffer
 * @retval     The extracted uint32
 */
uint32_t buff2ui32(uint8_t *buff)
{
    uint8_t tmpbuff[4];
    memcpy(tmpbuff, buff, 4);
    return *((uint32_t *)tmpbuff);
}

/**
 * @brief      uint32_ T push into byte buffer
 * @param      u: Uint32 to be pressed in
 * @param      buff: Pointer to byte buffer
 * @retval     NULL
 */
void ui322buff(uint32_t u, uint8_t *buff)
{
    uint8_t tmpbuff[4];
    *((uint32_t *)tmpbuff) = u;
    memcpy(buff, tmpbuff, 4);
}
