#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"
#include "dma.h"
#include "alg_swf.h"
#include "Foc_Control.h"

	
#define ADC1_CHANNEL_NUM  6

#define ADC1_REGULAR_MODE_CH 3
#define ADC1_INJECTED_MODE_CH 3

//ADC REGULAR_MODE_CH
#if 	THREE_PHASE_LINE_SEQUENCCE == A_B_C
	#define CURRENT_A_ADC_CHANNEL  0
	#define CURRENT_B_ADC_CHANNEL  1
	#define CURRENT_C_ADC_CHANNEL  2
#elif THREE_PHASE_LINE_SEQUENCCE == A_C_B
	#define CURRENT_A_ADC_CHANNEL  0
	#define CURRENT_B_ADC_CHANNEL  2
	#define CURRENT_C_ADC_CHANNEL  1
#endif

//ADC INJECTED_MODE_CH
#define VCC_ADC_CHANNEL  3
#define TEMP_ADC_CHANNEL  4
#define VREF_ADC_CHANNEL  5

typedef struct 
{
    /* data */
    float Valtage_Current_A;
    float Valtage_Current_B;
    float Valtage_Current_C;
    float Valtage_VCC;
    float Temperature;
    float Internal_Vref;
    float Vref_Offset;
} ADC_Struct;

extern ADC_Struct Motor_ADC;

void Adc_Init(void);
void ADC_Vrefint_Init(void);
void Get_ADC_Value(void);

#endif
