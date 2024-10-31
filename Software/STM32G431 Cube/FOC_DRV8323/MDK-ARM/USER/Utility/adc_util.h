#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"
#include "Foc_Control.h"
#define ADC1_CHANNEL_NUM  6

extern uint32_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    

void Adc_Init(void);
void ADC_Vrefint_Init(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hdac);

#endif
