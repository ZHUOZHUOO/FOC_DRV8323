/*
 * @Date: 2025-02-27 19:16:35
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-01 23:06:25
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Main\Foc_Error.c
 * @Description: Do not edit
 */
#include "Foc_Error.h"

ERROR_Struct Motor_Error = {1, 1, 1, 1, 1, 1, 1, {0, 0, 0, 0, 0, 0, 0, 0}};

void Error_Struct_Init(ERROR_Struct *error)
{
    error->SAFETY_STATE = 1;
    error->OVER_VOLTAGE_STATE = 1;
    error->UNDER_VOLTAGE_STATE = 1;
    error->OVER_CURRENT_STATE = 1;
    error->OVER_SPEED_STATE = 1;
    error->OVER_TEMPERATURE_STATE = 1;
    error->DRV8323_ERROR_STATE = 1;
}

void FOC_Error_Handler(void)
{
#if ERROR_MODE == MODE_ON
    Motor_Error.SAFETY_STATE =  Motor_Error.OVER_VOLTAGE_STATE & Motor_Error.UNDER_VOLTAGE_STATE & \
                                Motor_Error.OVER_CURRENT_STATE & Motor_Error.OVER_SPEED_STATE & Motor_Error.OVER_TEMPERATURE_STATE & \
                                Motor_Error.OVER_LOAD_STATE & Motor_Error.DRV8323_ERROR_STATE;
    
    if(Motor_Error.SAFETY_STATE){return;}

    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;

    while (1)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
        HAL_Delay(50);
    }
#endif
}

void Error_Main_Loop(void)
{
    Motor_Error.STATE_WINDOW.OVER_CURRENT_STATE_WINDOW <<= 1;
    Motor_Error.STATE_WINDOW.OVER_VOLTAGE_STATE_WINDOW <<= 1;
    Motor_Error.STATE_WINDOW.UNDER_VOLTAGE_STATE_WINDOW <<= 1;
    Motor_Error.STATE_WINDOW.OVER_SPEED_STATE_WINDOW <<= 1;
    Motor_Error.STATE_WINDOW.OVER_TEMPERATURE_STATE_WINDOW <<= 1;
    Motor_Error.STATE_WINDOW.OVER_LOAD_STATE_WINDOW <<= 1;
    // Motor_Error.STATE_WINDOW.DRV8323_Error_State_Window <<= 1;

	// Check over current
    float32_t temp = Motor_FOC.Ialpha * Motor_FOC.Ialpha + Motor_FOC.Ibeta * Motor_FOC.Ibeta;
    arm_sqrt_f32(temp, &Motor_FOC.Iamp);
    
    if(Motor_FOC.Iamp > CURRENT_MAX)
    {
        Motor_Error.STATE_WINDOW.OVER_CURRENT_STATE_WINDOW |= 1;
        if(Motor_Error.STATE_WINDOW.OVER_CURRENT_STATE_WINDOW == 0xFF)
        {
            Motor_Error.OVER_CURRENT_STATE = 0;
        }
    }
	// Check over voltage
    else if(Motor_ADC.Valtage_VCC > VOLTAGE_MAX)
    {
        Motor_Error.STATE_WINDOW.OVER_VOLTAGE_STATE_WINDOW |= 1;
        if(Motor_Error.STATE_WINDOW.OVER_VOLTAGE_STATE_WINDOW == 0xFF)
        {
            Motor_Error.OVER_VOLTAGE_STATE = 0;
        }
    }
    // Check under voltage
    else if(Motor_ADC.Valtage_VCC < VOLTAGE_MIN)
    {
        Motor_Error.STATE_WINDOW.UNDER_VOLTAGE_STATE_WINDOW |= 1;
        if(Motor_Error.STATE_WINDOW.UNDER_VOLTAGE_STATE_WINDOW == 0xFF)
        {
            Motor_Error.UNDER_VOLTAGE_STATE = 0;
        }
    }
    // Check over temperature
    else if(Motor_ADC.Temperature > TEMPERATURE_MAX)
    {
        Motor_Error.STATE_WINDOW.OVER_TEMPERATURE_STATE_WINDOW |= 1;
        if(Motor_Error.STATE_WINDOW.OVER_TEMPERATURE_STATE_WINDOW == 0xFF)
        {
            Motor_Error.OVER_TEMPERATURE_STATE = 0;
        }
    }
#if N_FAULT_MODE == MODE_OFF
    //Check DRV8323 nFault Pin
    else if (HAL_GPIO_ReadPin(DRV8323_nFault_PORT, DRV8323_nFault) == GPIO_PIN_RESET)
    {
        // Motor_Error.STATE_WINDOW.DRV8323_Error_State_Window |= 1;
        // if(Motor_Error.STATE_WINDOW.DRV8323_Error_State_Window == 0xFF)
        // {
            Motor_Error.DRV8323_ERROR_STATE = 0;
        // }
    }
#endif
		
	FOC_Error_Handler();
}

// DRV8323_nFault_Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == DRV8323_nFault)
    {
    #if N_FAULT_MODE == MODE_ON
        Motor_Error.DRV8323_ERROR_STATE = 0;
        FOC_Error_Handler();
    #endif
    }
}
