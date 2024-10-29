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
void Adc_Init(void);

#endif
