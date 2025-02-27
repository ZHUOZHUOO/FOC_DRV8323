#include "Foc_Error.h"

ERROR_Struct Motor_Error;
void Error_Struct_Init(ERROR_Struct *error)
{
    error->SAFETY_STATE = 1;
    error->OVER_VOLTAGE_STATE = 1;
    error->UNDER_VOLTAGE_STATE = 1;
    error->OVER_CURRENT_STATE = 1;
    error->OVER_SPEED_STATE = 1;
    error->OVER_TEMPERATURE_STATE = 1;
    error->OVER_LOAD_STATE = 1;
}

void FOC_Error_Handler(void)
{
    if(Motor_Error.SAFETY_STATE){return;}

    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;

    while (1)
    {

    }
}

void Error_Main_Loop(void)
{
    float32_t temp = Motor_FOC.Ialpha * Motor_FOC.Ialpha + Motor_FOC.Ibeta * Motor_FOC.Ibeta;
    arm_sqrt_f32(temp, Motor_FOC.Iamp);
    if(Motor_FOC.Iamp > CURRENT_MAX)
    {
        Motor_Error.OVER_CURRENT_STATE = 0;
        Motor_Error.SAFETY_STATE = 0;
        FOC_Error_Handler();
    }
}
