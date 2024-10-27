#ifndef __FOC_CONTROL_H
#define __FOC_CONTROL_H

#include "stm32g4xx_hal.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "main.h"
#include "tim.h"

#define PI 3.14159265358979323846
#define SQRT3 1.73205080756887729352

#define PWM_MAX 1000
#define PWM_MIN -1000

#define VOLTAGE_MAX 24
#define CURRENT_MAX 10

uint8_t FOC_Control(float Iq_ref, float Id_ref, float Iq_mes, float Id_mes, float *Vd, float);

#endif