/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-25 12:55:09
 */

#ifndef ALG_FILTER_H
#define ALG_FILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32g4xx_hal.h"
#include "lib_math.h"

#define MAX_LENGTH 30

    typedef struct
    {
        float cut_off_frq;
        float filt_para;
        float last_tick;
        float calc_frq;

        float filted_val;
        float filted_last_val;
    } Filter_Lowpass_TypeDef;

    typedef struct
    {
        uint8_t length;
        float val[MAX_LENGTH];
        float sum;
    } Filter_Window_TypeDef;

    typedef struct
    {
        double ybuf[4];
        double xbuf[4];
        float filted_val;
    } Filter_Bessel_TypeDef;

    void Filter_Lowpass_Init(float param, Filter_Lowpass_TypeDef *lpf);
    float Filter_Lowpass(float val, Filter_Lowpass_TypeDef *lpf);
    void Filter_Aver_Init(Filter_Window_TypeDef *filt, uint8_t length);
    float Filter_Aver(float val, Filter_Window_TypeDef *filt);
    float Filter_Bessel(float val, Filter_Bessel_TypeDef *filt);

#ifdef __cplusplus
}
#endif

#endif
