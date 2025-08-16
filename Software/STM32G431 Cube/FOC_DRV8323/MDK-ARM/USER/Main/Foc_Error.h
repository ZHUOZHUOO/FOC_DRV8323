/*
 * @Date: 2025-02-27 19:16:35
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-02-28 00:49:15
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Main\Foc_Error.h
 * @Description: Do not edit
 */
#ifndef __FOC_ERROR_H
#define __FOC_ERROR_H

#include "stm32g4xx_hal.h"
#include "Foc_Control.h"

#define VOLTAGE_MAX MOTOR_VOLTAGE + 3//V
#define VOLTAGE_MIN MOTOR_VOLTAGE - 3 //V
#define CURRENT_MAX 15 //A
#define TEMPERATURE_MAX 300 //°C
#define OVER_SPEED 1000 //rpm
#define OVER_LOAD 100 

typedef struct
{
    uint8_t SAFETY_STATE_WINDOW;
    uint8_t OVER_VOLTAGE_STATE_WINDOW;
    uint8_t UNDER_VOLTAGE_STATE_WINDOW;
    uint8_t OVER_CURRENT_STATE_WINDOW;
    uint8_t OVER_SPEED_STATE_WINDOW;
    uint8_t OVER_TEMPERATURE_STATE_WINDOW;
    uint8_t OVER_LOAD_STATE_WINDOW;
    uint8_t DRV8323_Error_State_Window;
}Error_State_Window;


typedef struct
{
    uint8_t SAFETY_STATE;           //0:Safety expired  1:Normal
    uint8_t OVER_VOLTAGE_STATE;     //0:OverVoltage     1:Normal
    uint8_t UNDER_VOLTAGE_STATE;    //0:UnderVoltage    1:Normal
    uint8_t OVER_CURRENT_STATE;     //0:OverCurrent     1:Normal
    uint8_t OVER_SPEED_STATE;       //0:OverSpeed       1:Normal
    uint8_t OVER_TEMPERATURE_STATE; //0:OverTemperature 1:Normal
    uint8_t DRV8323_ERROR_STATE;    //0:DRV8323 Error   1:Normal
    Error_State_Window STATE_WINDOW;
} ERROR_Struct;

extern ERROR_Struct Motor_Error;

void Error_Struct_Init(ERROR_Struct *error);
void Error_Main_Loop(void);

#endif
