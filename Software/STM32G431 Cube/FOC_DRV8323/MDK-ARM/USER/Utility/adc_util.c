#include "adc_util.h"

uint16_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
float   Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data
uint16_t Adc_flag;

#if ADC_VREF_MODE == MODE_ON
float Adc_Sum_Temp[ADC1_CHANNEL_NUM][SLIDING_WINDOW_SIZE];
uint8_t filter_flag = 0;
#endif

void Adc_Init(void) { 
		HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_Delay(10);

//    HAL_ADC_Start_IT(&hadc1);//开启ADC中断
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Adc_Val, ADC1_CHANNEL_NUM);//开启ADC DMA
    HAL_ADC_Start(&hadc1);//开启ADC1
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
		Adc_Sum_Val[i] = (float)Adc_Val[i] * (3.3f / 4095.0f);
	}//将ADC1的值转换为电压值
#elif ADC_FILTER_MODE == MODE_ON
	for(int i = 0; i < ADC1_CHANNEL_NUM; i++)
	{
		Adc_Sum_Temp[i][filter_flag] = (float)Adc_Val[i] * (3.3f / 4095.0f / SLIDING_WINDOW_SIZE);
		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
		{
			Adc_Sum_Val[i] += Adc_Sum_Temp[i][j];
		}
	}
	filter_flag = (filter_flag + 1) % SLIDING_WINDOW_SIZE;
#endif

#if ADC_VREF_MODE == MODE_ON
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[0] * Vref_Offset;
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[1] * Vref_Offset;
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[2] * Vref_Offset;
	Motor_ADC.Valtage_VCC = Adc_Sum_Val[3] * 11.0f * Vref_Offset;
	Motor_ADC.Temperature = (Adc_Sum_Val[4] * Vref_Offset - 0.76f) / 0.0025f + 25.0f;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[5];
#elif ADC_VREF_MODE == MODE_OFF
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[0];
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[1];
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[2];
	Motor_ADC.Valtage_VCC = Adc_Sum_Val[3] * 11.0f;
	Motor_ADC.Temperature = (Adc_Sum_Val[4] - 0.76f) / 0.0025f + 25.0f;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[5];
#endif
}

//ADC 中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
				Adc_Val_Decode();
				Adc_flag++;
    }
}

void ADC_Vrefint_Init(void)
{
	HAL_Delay(200);
	#if ADC_VREF_MODE == MODE_ON
	__IO uint16_t* VREFINT_CAL = (__IO uint16_t *)(0x1FFF75AA);

	float VREFINT_CAL_DATA = 0;
	float VREFINT_CAL_VAL  = 0;
	float Vref_Offset_Sum  = 0.0f;

	VREFINT_CAL_DATA = (float)*VREFINT_CAL;
	VREFINT_CAL_VAL = (VREFINT_CAL_DATA / 4095.0f * 3.0f);
	for(int flag = 0; flag < 1000; flag++)
	{
			Vref_Offset_Sum += VREFINT_CAL_VAL / Motor_ADC.Internal_Vref;
			HAL_Delay(1);
	}
	Vref_Offset = Vref_Offset_Sum / 1000.0f;
	HAL_Delay(20);
	#endif
}
