#ifndef __FOC_CONTROL_H
#define __FOC_CONTROL_H

#include "stm32g4xx_hal.h"
#include "stdbool.h"
#include "stdint.h"
#include "tim.h"
#include "adc.h"
#include "adc_util.h"
#include "spi_util.h"
#include "pid_util.h"
#include "configure.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "Foc_Error.h"


#define PI              (float)3.1415926535898
#define TWO_PI          (float)6.2831853071796
#define SQRT3           (float)1.7320508075689
#define SQRT3_DIV2      (float)0.8660254037844
#define SQRT3_DIV3      (float)0.5773502691896
#define TWO_THIRD       (float)0.6666666666667
#define TWO_DIV_SQRT3   (float)1.1547005383793

//SVPWM_SETTING
#define T		    (PWM_PERIOD * 4)   //TIM1 ARR值的4倍
#define T_SQRT3     (uint16_t)(T * SQRT3)
#define SECTOR_1	(uint8_t)1
#define SECTOR_2	(uint8_t)2
#define SECTOR_3	(uint8_t)3
#define SECTOR_4	(uint8_t)4
#define SECTOR_5	(uint8_t)5
#define SECTOR_6	(uint8_t)6
#define zeta		(uint16_t)540 //SVPWM力矩线性灵敏度

#define DRV8323_PORT GPIOA
#define DRV8323_CAL 4

typedef struct
{
    float Ia;//A相电流_反馈
    float Ib;//B相电流_反馈
    float Ic;//C相电流_反馈
    float Ialpha;
    float Ibeta;
    float Iamp;//电流幅值
    float Id;
    float Iq;
    float Vd;//D轴电压_期望
    float Vq;//Q轴电压_期望
    float Valpha;
    float Vbeta;
    float Va;//A相电压_期望
    float Vb;//B相电压_期望
    float Vc;//C相电压_期望
    float Theta;//电角度_反馈
    float Speed;//速度
    float PWM_A_DutyCycle;//占空比A相
    float PWM_B_DutyCycle;//占空比B相
    float PWM_C_DutyCycle;//占空比C相
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

extern FOC_Struct Motor_FOC;
extern ADC_Struct Motor_ADC;
extern float Vref_Offset;

extern uint32_t run_flag;

void FOC_Struct_Init(FOC_Struct *foc);
void FOC_Main_Init(void);
void FOC_Main_Loop(void);


#endif
