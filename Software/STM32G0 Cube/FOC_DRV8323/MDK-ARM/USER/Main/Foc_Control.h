#ifndef __FOC_CONTROL_H
#define __FOC_CONTROL_H

#include "stm32g4xx_hal.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "tim.h"
#include "adc.h"
#include "adc_util.h"
#include "spi_util.h"
#include "pid_util.h"
#include "configure.h"

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define SQRT3 1.73205080756887729352
#define SQRT3_DIV2 0.86602540378443864626
#define SQRT3_DIV3 0.57735026918962576451
#define TWO_THIRD 0.66666666666666666666
#define TWO_DIV_SQRT3 1.1547005383792517

#define PWM_MAX 1000
#define PWM_MIN -1000

#define VOLTAGE_MAX 24
#define CURRENT_MAX 10

typedef struct
{
    double Ia;//A相电流_反馈
    double Ib;//B相电流_反馈
    double Ic;//C相电流_反馈
    double Ialpha;
    double Ibeta;
    double Id;//D轴电流_反馈
    double Iq;//Q轴电流_反馈
    double Vd;//D轴电压_输出
    double Vq;//Q轴电压_输出
    double Valpha;
    double Vbeta;
    double Va;//A相电压_电机期望
    double Vb;//B相电压_电机期望
    double Vc;//C相电压_电机期望
    double Theta;//电角度_反馈
} FOC_Struct;


typedef struct 
{
    /* data */
    float Valtage_Current_A;
    float Valtage_Current_B;
    float Valtage_Current_C;
    float Valtage_VCC;
    float Temperature;
    float Internal_Vref;
} ADC_Struct;

extern FOC_Struct Motor_FOC_Expect;
extern FOC_Struct Motor_FOC_FeedBack;
extern ADC_Struct Motor_ADC;

#endif