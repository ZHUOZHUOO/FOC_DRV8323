#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"
#define ADC1_CHANNEL_NUM  6

extern uint32_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
extern float    Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hdac);
float Adc_GetValue(ADC_HandleTypeDef *hadc);
void Adc_GetValue_DMA(ADC_HandleTypeDef *hadc, uint32_t *pdata, uint32_t Length);
void Adc_Init(void);
void Adc_Decoder(ADC_HandleTypeDef *hadc) ;

#endif
