#include "util_adc.h"

uint16_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
float   Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data

#if ADC_FILTER_MODE == MODE_ON
float Adc_Sum_Temp[ADC1_CHANNEL_NUM][SLIDING_WINDOW_SIZE];
uint8_t filter_flag = 0;
#endif

ADC_Struct Motor_ADC;

void ADC_Struct_Init(ADC_Struct *adc);

void Adc_Init(void) { 
	ADC_Struct_Init(&Motor_ADC);
	
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  	HAL_Delay(1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Adc_Val, ADC1_CHANNEL_NUM);//开启ADC DMA
  	HAL_ADC_Start(&hadc1);//开启ADC1

	ADC_Vrefint_Init();
}

void Get_ADC_Value(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Adc_Val, ADC1_CHANNEL_NUM);
}

void Adc_Val_Decode(void)
{
#if ADC_FILTER_MODE == MODE_OFF
	for(int i = 0; i < ADC1_CHANNEL_NUM; i++)
	{
		Adc_Sum_Val[i] = Adc_Val[i] * (3.3f / 4095.0f);
	}//将ADC1的值转换为电压值
#elif ADC_FILTER_MODE == MODE_ON
	for(int i = 0; i < ADC1_CHANNEL_NUM; i++)
	{
		Adc_Sum_Temp[i][filter_flag] = Adc_Val[i] * (3.3f / 4095.0f / (float)SLIDING_WINDOW_SIZE);
		float sum_temp = 0.0f;
		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
		{
			sum_temp += Adc_Sum_Temp[i][j];
		}
		Adc_Sum_Val[i] = sum_temp;
	}
	filter_flag = (filter_flag + 1) % SLIDING_WINDOW_SIZE;
#endif

    Adc_Sum_Val[CURRENT_A_ADC_CHANNEL] -= Motor_ADC.A_Offset;
    Adc_Sum_Val[CURRENT_B_ADC_CHANNEL] -= Motor_ADC.B_Offset;
    Adc_Sum_Val[CURRENT_C_ADC_CHANNEL] -= Motor_ADC.C_Offset;

#if ADC_VREF_MODE == MODE_ON
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[CURRENT_A_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[CURRENT_B_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[CURRENT_C_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
	Motor_ADC.Valtage_VCC = Adc_Sum_Val[VCC_ADC_CHANNEL] * 11 * Motor_ADC.Vref_Offset;
	Motor_ADC.Temperature = (Adc_Sum_Val[TEMP_ADC_CHANNEL] * Motor_ADC.Vref_Offset - 0.76f) / 0.0025f + 2;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[VREF_ADC_CHANNEL];
#elif ADC_VREF_MODE == MODE_OFF
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[CURRENT_A_ADC_CHANNEL];
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[CURRENT_B_ADC_CHANNEL];
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[CURRENT_C_ADC_CHANNEL];
	Motor_ADC.Valtage_VCC = Adc_Sum_Val[VCC_ADC_CHANNEL] * 11;
	Motor_ADC.Temperature = (Adc_Sum_Val[TEMP_ADC_CHANNEL] - 0.76f) / 0.0025f + 2;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[VREF_ADC_CHANNEL];
#endif
}

//ADC 中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
		Adc_Val_Decode();
		Motor_Run.Adc_flag++;
    }
}

void ADC_Vrefint_Init(void)
{
	HAL_Delay(100);
	#if ADC_VREF_MODE == MODE_ON
	__IO uint16_t* VREFINT_CAL = (__IO uint16_t *)(0x1FFF75AA);

	float VREFINT_CAL_DATA = 0;
	float VREFINT_CAL_VAL  = 0;
	float Vref_Offset_Sum  = 0.0f;

	VREFINT_CAL_DATA = (float)*VREFINT_CAL;
	VREFINT_CAL_VAL = (VREFINT_CAL_DATA / 4095.0f * 3.0f);
	for(int flag = 0; flag < 300; flag++)
	{
		Get_ADC_Value();
		HAL_Delay(1);
	}
	for(int flag = 0; flag < 1200; flag++)
	{
		Get_ADC_Value();
		Vref_Offset_Sum += VREFINT_CAL_VAL / Motor_ADC.Internal_Vref;
		HAL_Delay(1);
	}
	Motor_ADC.Vref_Offset = Vref_Offset_Sum / 1200.0f;
	HAL_Delay(20);
	#endif
}

void ADC_Struct_Init(ADC_Struct *adc)
{
    adc->Valtage_Current_A = 0;
    adc->Valtage_Current_B = 0;
    adc->Valtage_Current_C = 0;
    adc->Valtage_VCC = 0;
    adc->Temperature = 0;
    adc->Internal_Vref = 0;
    adc->A_Offset = 0;
    adc->B_Offset = 0;
    adc->C_Offset = 0;
		adc->Vref_Offset = 1.0f;
}

