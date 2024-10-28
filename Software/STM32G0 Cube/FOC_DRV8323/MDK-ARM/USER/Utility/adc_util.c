#include "adc_util.h"

uint32_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
double   Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data

void Adc_Init(void) { 
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_Delay(10);
    Adc_GetValue_DMA(&hadc1, &Adc_Val, ADC1_CHANNEL_NUM);
    HAL_Delay(10);
}