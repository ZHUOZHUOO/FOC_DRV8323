#include "util_adc.h"


uint16_t Adc_Val[ADC1_CHANNEL_NUM];                   //Adc data array    
float   Adc_Sum_Val[ADC1_CHANNEL_NUM];		            //Adc decode data
uint32_t ADC_INJECTED_RANK[4] = {ADC_INJECTED_RANK_1, ADC_INJECTED_RANK_2, ADC_INJECTED_RANK_3, ADC_INJECTED_RANK_4};

static float inv_adc_val = 3.3f / 4095.0f;

#if ADC_FILTER_MODE == MODE_ON
float Adc_Sum_Temp[ADC1_CHANNEL_NUM][SLIDING_WINDOW_SIZE];
uint8_t filter_flag = 0;
uint8_t inj_filter_flag = 0;
#endif

ADC_Struct Motor_ADC;

void ADC_Struct_Init(ADC_Struct *adc);

void Adc_Init(void) { 
	ADC_Struct_Init(&Motor_ADC);
	
	__HAL_ADC_ENABLE_IT(&hadc1, ADC_IT_JEOC);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_InjectedStart_IT(&hadc1);

	ADC_Vrefint_Init();
}

void Get_ADC_Value(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(&Adc_Val[ADC1_INJECTED_MODE_CH]), ADC1_REGULAR_MODE_CH);
}


void Adc_Val_Decode()
{
#if ADC_FILTER_MODE == MODE_OFF
	for(int i = ADC1_INJECTED_MODE_CH; i < ADC1_CHANNEL_NUM; i++)
	{
		Adc_Sum_Val[i] = Adc_Val[i] * (inv_adc_val);
	}
#elif ADC_FILTER_MODE == MODE_ON
	for(int i = ADC1_INJECTED_MODE_CH; i < ADC1_CHANNEL_NUM; i++)
	{
		Adc_Sum_Temp[i][filter_flag] = Adc_Val[i] * (inv_adc_val / (float)SLIDING_WINDOW_SIZE);
		float sum_temp = 0.0f;
		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
		{
			sum_temp += Adc_Sum_Temp[i][j];
		}
		Adc_Sum_Val[i] = sum_temp;
	}
	filter_flag = (filter_flag + 1) % SLIDING_WINDOW_SIZE;
#endif
	
#if ADC_VREF_MODE == MODE_ON
		Motor_ADC.Valtage_VCC = Adc_Sum_Val[VCC_ADC_CHANNEL] * 11 * Motor_ADC.Vref_Offset;
	Motor_ADC.Temperature = (Adc_Sum_Val[TEMP_ADC_CHANNEL] * Motor_ADC.Vref_Offset - 0.76f) / 0.0025f + 2;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[VREF_ADC_CHANNEL];
#elif ADC_VREF_MODE == MODE_OFF
		Motor_ADC.Valtage_VCC = Adc_Sum_Val[VCC_ADC_CHANNEL] * 11;
	Motor_ADC.Temperature = (Adc_Sum_Val[TEMP_ADC_CHANNEL] - 0.76f) / 0.0025f + 2;
	Motor_ADC.Internal_Vref = Adc_Sum_Val[VREF_ADC_CHANNEL];
#endif
}


void Adc_Injected_Val_Decode(ADC_HandleTypeDef *hadc)
{
#if ADC_FILTER_MODE == MODE_OFF
	for(int i = 0; i < ADC1_INJECTED_MODE_CH; i++)
	{
		uint32_t jdr_value = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK[i]);
    Adc_Sum_Val[i] = (float)jdr_value * (inv_adc_val);
	}

#elif ADC_FILTER_MODE == MODE_ON
	for(int i = 0; i < ADC1_INJECTED_MODE_CH; i++)
	{
		uint32_t jdr_value = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK[i]);
    Adc_Sum_Temp[i][inj_filter_flag] = (float)jdr_value * (inv_adc_val / (float)SLIDING_WINDOW_SIZE);
		float sum_temp = 0.0f;
		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
		{
			sum_temp += Adc_Sum_Temp[i][j];
		}
		Adc_Sum_Val[i] = sum_temp;
	}
	inj_filter_flag = (inj_filter_flag + 1) % SLIDING_WINDOW_SIZE;
#endif


#if ADC_VREF_MODE == MODE_ON
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[CURRENT_A_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[CURRENT_B_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[CURRENT_C_ADC_CHANNEL] * Motor_ADC.Vref_Offset;
#elif ADC_VREF_MODE == MODE_OFF
	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[CURRENT_A_ADC_CHANNEL];
	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[CURRENT_B_ADC_CHANNEL];
	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[CURRENT_C_ADC_CHANNEL];
#endif
	
}


//void Adc_Val_Decode(ADC_HandleTypeDef *hadc)
//{
//	for(int i = 0; i < ADC1_INJECTED_MODE_CH; i++)
//	{
//		uint32_t jdr_value = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK[i]);
//    Adc_Sum_Temp[i][filter_flag] = (float)jdr_value * (3.3f / 4095.0f / (float)SLIDING_WINDOW_SIZE);
//		float sum_temp = 0.0f;
//		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
//		{
//			sum_temp += Adc_Sum_Temp[i][j];
//		}
//		Adc_Sum_Val[i] = sum_temp;
//	}
//	for(int i = ADC1_INJECTED_MODE_CH ; i < ADC1_CHANNEL_NUM; i++)
//	{
//		Adc_Sum_Temp[i][filter_flag] = Adc_Val[i] * (3.3f / 4095.0f / (float)SLIDING_WINDOW_SIZE);
//		float sum_temp = 0.0f;
//		for(int j = 0; j < SLIDING_WINDOW_SIZE; j++)
//		{
//			sum_temp += Adc_Sum_Temp[i][j];
//		}
//		Adc_Sum_Val[i] = sum_temp;
//	}
//	filter_flag = (filter_flag + 1) % SLIDING_WINDOW_SIZE;

//	Motor_ADC.Valtage_Current_A = Adc_Sum_Val[CURRENT_A_ADC_CHANNEL];
//	Motor_ADC.Valtage_Current_B = Adc_Sum_Val[CURRENT_B_ADC_CHANNEL];
//	Motor_ADC.Valtage_Current_C = Adc_Sum_Val[CURRENT_C_ADC_CHANNEL];
//}


//ADC 中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {

    }
}


void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc == &hadc1)
	{
		if (TIM1->SR & TIM_SR_UIF) {
			if ((TIM1->CR1 & TIM_CR1_DIR) == 0) {
				// 向上溢出（Up-counting overflow）
				TIM1->SR &= ~TIM_SR_UIF; // 清除标志
			} 
			else {
				// 向下溢出（Down-counting overflow）
				TIM1->SR &= ~TIM_SR_UIF; // 清除标志
				HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
				Adc_Injected_Val_Decode(&hadc1);
				Motor_Run.Adc_flag++;
				FOC_Main_Loop_H_Freq();
				HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
			}
		}
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
		Adc_Val_Decode();
		Get_ADC_Value();
		HAL_Delay(1);
	}
	for(int flag = 0; flag < 1200; flag++)
	{
		Adc_Val_Decode();
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
		adc->Vref_Offset = 1.0f;
}

