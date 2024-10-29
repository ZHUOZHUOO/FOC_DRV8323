#include "adc_util.h"

uint32_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
double   Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data

void Adc_Init(void) { 
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_Delay(10);
    HAL_ADC_Start_DMA(&hadc1, Adc_Val, ADC1_CHANNEL_NUM);
    HAL_Delay(10);
}

//ADC DMA中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        //读取ADC1的值
        for(int i = 0; i < ADC1_CHANNEL_NUM; i++)
        {
            Adc_Sum_Val[i] = Adc_Val[i]/4095.0*3.3;
        }//将ADC1的值转换为电压值
        Motor_ADC.Valtage_Current_A = Adc_Val[0] * Vref_Offset;
        Motor_ADC.Valtage_Current_B = Adc_Val[1] * Vref_Offset;
        Motor_ADC.Valtage_Current_C = Adc_Val[2] * Vref_Offset;
        Motor_ADC.Valtage_VCC = Adc_Val[3] * 11.0 * Vref_Offset;
        Motor_ADC.Temperature = (Adc_Val[4] * Vref_Offset - 0.76) / 0.0025 + 25;
        Motor_ADC.Internal_Vref = Adc_Val[5];
    }
}


void ADC_Vrefint_Init(void)
{
	__IO uint16_t* VREFINT_CAL = (__IO uint16_t *)(0x1FFF75AA);

	float VREFINT_CAL_DATA = 0;
	float VREFINT_CAL_VAL  = 0;
	float Vref_Offset_Sum  = 0.0f;
	
	VREFINT_CAL_DATA = (float)*VREFINT_CAL;
	VREFINT_CAL_VAL = (VREFINT_CAL_DATA/4095.0f * 3.0f);
	for(int flag=0;flag<1000;flag++)
	{
			Vref_Offset_Sum += VREFINT_CAL_VAL / Motor_ADC.Internal_Vref;
			osDelay(1);
	}
	Vref_Offset = Vref_Offset_Sum / 1000;

	HAL_Delay(20);
}