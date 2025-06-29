#ifndef __FOC_CONTROL_H
#define __FOC_CONTROL_H

#include "stm32g4xx_hal.h"
#include "stdint.h"
#include "tim.h"
#include "gpio.h"
#include "spi.h"
#include "alg_pid.h"
#include "drv8323_util.h"
#include "configure.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "periph_encoder_spi.h"
#include "Foc_Error.h"
#include "Foc_Comm.h"
#include "Init_music.h"
#include "util_adc.h"


#define MY_PI           (float)3.1415926535898
#define TWO_PI          (float)6.2831853071796
#define SQRT3           (float)1.7320508075689
#define SQRT3_DIV2      (float)0.8660254037844
#define SQRT3_DIV3      (float)0.5773502691896
#define TWO_THIRD       (float)0.6666666666667
#define TWO_DIV_SQRT3   (float)1.1547005383793

//SVPWM_SETTING
#define T_2		    (PWM_PERIOD * 4)   //TIM1 ARR值的4倍
#define T		    (PWM_PERIOD * 2)   //TIM1 ARR值的2倍
#define T_SQRT3   	(uint16_t)(T * SQRT3)
#define SECTOR_1	(uint8_t)1
#define SECTOR_2	(uint8_t)2
#define SECTOR_3	(uint8_t)3
#define SECTOR_4	(uint8_t)4
#define SECTOR_5	(uint8_t)5
#define SECTOR_6	(uint8_t)6

typedef enum
{
    Speed_Open_Loop = 0,
    Speed_Mode = 2,
    Position_Mode = 3,
    Force_Mode = 4
}Close_Loop_Mode_t;
 
typedef struct
{
    float Ia;//A相电流_反馈
    float Ib;//B相电流_反馈
    float Ic;//C相电流_反馈
    float Ialpha;
    float Ibeta;
    float Iamp;//电流幅值
    float Id;
    float Id_ref;
    float Iq;
	float Iq_ref;
    float Vd;//D轴电压_期望
    float Vq;//Q轴电压_期望
    float Valpha;
    float Vbeta;
    uint16_t hTimePhA;
    uint16_t hTimePhB;
    uint16_t hTimePhC;
    float PWM_A_DutyCycle;//占空比A相
    float PWM_B_DutyCycle;//占空比B相
    float PWM_C_DutyCycle;//占空比C相

    float Theta;//机械角度_反馈
    float Theta_Ref;//机械角度_期望
    float ElecTheta;//电角度_反馈
		float ElecTheta_Offset;
    float Open_Loop_Theta;//开环角度_ref
    
    float Speed_Rpm;//速度
    float Speed_Rpm_Ref;//期望速度

    Close_Loop_Mode_t Motor_Close_Loop_Mode;
} FOC_Struct;

typedef struct 
{
    uint32_t run_flag;
    uint32_t run_Hz;
    uint16_t state_led_flag;
    uint16_t Adc_flag;
	uint16_t Adc_Hz;
	uint16_t spi_flag;
	uint16_t spi_Hz;
}FOC_Running_Struct;

extern FOC_Struct Motor_FOC;
extern FOC_Running_Struct Motor_Run;

extern PID_TypeDef Current_Id_PID;
extern PID_TypeDef Current_Iq_PID;
extern PID_TypeDef Speed_PID;
extern PID_TypeDef Position_PID;

void FOC_Struct_Init(FOC_Struct *foc);
void FOC_Main_Init(void);
void FOC_Main_Loop_H_Freq(void);
void FOC_Main_Loop_L_Freq(void);


#endif
