#ifndef __FOC_ERROR_H
#define __FOC_ERROR_H

#include "stm32g4xx_hal.h"
#include "Foc_Control.h"

#define VOLTAGE_MAX 24
#define CURRENT_MAX 10

typedef struct
{
    uint8_t SAFETY_STATE;//0:危险 1:安全
    uint8_t OVER_VOLTAGE_STATE;//0:过压 1:正常
    uint8_t UNDER_VOLTAGE_STATE;//0:欠压 1:正常
    uint8_t OVER_CURRENT_STATE;//0:过流 1:正常
    uint8_t OVER_SPEED_STATE;//0:过速 1:正常
    uint8_t OVER_TEMPERATURE_STATE;//0:过温 1:正常
    uint8_t OVER_LOAD_STATE;//0:过载 1:正常
} ERROR_Struct;

extern ERROR_Struct Motor_Error;

void Error_Struct_Init(ERROR_Struct *error);
void FOC_Error_Handler(void);

#endif
