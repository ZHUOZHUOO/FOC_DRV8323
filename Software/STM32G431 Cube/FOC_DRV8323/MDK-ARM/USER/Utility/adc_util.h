#ifndef __ADC_UTIL_H
#define __ADC_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "adc.h"
#include "Foc_Control.h"
#define ADC1_CHANNEL_NUM  6

//ADC Rank define
#define CURRENT_A_ADC_CHANNEL  1
#define CURRENT_B_ADC_CHANNEL  2
#define CURRENT_C_ADC_CHANNEL  3
#define VCC_ADC_CHANNEL  0
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
    float A_Offset;
    float B_Offset;
    float C_Offset;
    float Vref_Offset;
} ADC_Struct;

extern ADC_Struct Motor_ADC;
extern float Vref_Offset;

extern uint16_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
extern uint16_t Adc_flag;

void Adc_Init(void);
void ADC_Vrefint_Init(void);
void Get_ADC_Value(void);

#endif
