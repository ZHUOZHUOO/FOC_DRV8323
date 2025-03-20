/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-25 13:05:08
 */

#include "lib_filter.h"

/**
 * @brief      low_pass_filter_init
 * @param      param :Low pass filter param
 * @param      Filter_LowpassParamTypeDef: low pass filter param stuct
 * @retval     filtering result
 */
void Filter_Lowpass_Init(float param, Filter_Lowpass_TypeDef *lpf)
{
    lpf->filt_para = param;
    lpf->last_tick = 0;
}

/**
 * @brief      low_pass_filter
 * @param      val: inital value
 * @param      lpf: low pass filter sturct
 * @retval     filtering result
 */
float Filter_Lowpass(float val, Filter_Lowpass_TypeDef *lpf)
{
    // calculate cut off frequence
    uint32_t period = HAL_GetTick() - lpf->last_tick;
    lpf->last_tick = HAL_GetTick();
    if ((lpf->filt_para > 0) && (lpf->filt_para <= 1))
    {
        lpf->filted_val = lpf->filt_para * val + (1 - lpf->filt_para) * lpf->filted_last_val;
        lpf->filted_last_val = lpf->filted_val;
        if (period > 0)
        {
            lpf->cut_off_frq = lpf->filt_para / (2 * PI * (float)period * 0.001f);
        }
        lpf->calc_frq = 1000 / (float)period;
        return lpf->filted_val;
    }
    else
    {
        return val;
    }
}

/**
 * @brief      average_filter_init
 * @param      lpf :average_filter sturct
 * @param      length  :buff len
 * @retval     filtering result
 */
void Filter_Aver_Init(Filter_Window_TypeDef *lpf, uint8_t length)
{
    lpf->length = length;
    lpf->sum = 0;
}

/**
 * @brief      average_filter
 * @param      val  :inital value
 * @param      lpf :average_filter sturct
 * @retval     filtering result
 */
float Filter_Aver(float val, Filter_Window_TypeDef *lpf)
{
    lpf->sum = 0;
    for (int i = 0; i < lpf->length - 1; i++)
    {
        lpf->val[i] = lpf->val[i + 1];
    }
    lpf->val[lpf->length - 1] = val;
    for (int i = 0; i < lpf->length; i++)
    {
        lpf->sum += lpf->val[i];
    }
    return lpf->sum / lpf->length;
}

/**
 * @brief      bessel_filter
 * @param      val  :inital value
 * @param      lpf :bessel_filter sturct
 * @retval     filtering result
 */
float Filter_Bessel(float val, Filter_Bessel_TypeDef *lpf)
{
    for (int i = 3; i > 0; i--)
    {
        lpf->xbuf[i] = lpf->xbuf[i - 1];
        lpf->ybuf[i] = lpf->ybuf[i - 1];
    }
    lpf->xbuf[0] = val;
    lpf->ybuf[0] = 0.0001507 * lpf->xbuf[1] + 0.0005675 * lpf->xbuf[2] + 0.0001336 * lpf->xbuf[3] + 2.765 * lpf->ybuf[1] - 2.552 * lpf->ybuf[2] + 0.7866 * lpf->ybuf[3];
    lpf->filted_val = lpf->ybuf[0];
    return lpf->filted_val;
}
