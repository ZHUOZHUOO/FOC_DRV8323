/*
 * @Date: 2025-02-26 18:25:59
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-12 19:15:23
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Main\Foc_Control.c
 * @Description: Do not edit
 */

#include "Foc_Control.h"

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Max_3(a, b, c) Max(Max(a, b), c)

FOC_Struct Motor_FOC;
FOC_Running_Struct Motor_Run = {0, 0, 0, 0, 0};
PID_Struct Current_Id_PID;
PID_Struct Current_Iq_PID;
PID_Struct Speed_PID;
PID_Struct Position_PID;


//--------------------函数声明--------------------
void FOC_Calc_Electrical_Angle(void);
void Park_transform(float Ialpha, float Ibeta, float *Id, float *Iq, float Theta);
void Clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta);
void Inv_Park_transform(float Id, float Iq, float *Ialpha, float *Ibeta, float Theta);
void CALC_SVPWM(float Valpha, float Vbeta);
void FOC_Struct_Init(FOC_Struct *foc);

//------------------------------------------------

void FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    Error_Struct_Init(&Motor_Error);

    DRV8323_GPIO_Init();
		DRV8323_Init();
    Adc_Init();
    SPI_Init();

    // PID初始化
    PID_Init(&Current_Id_PID, 0.001f, 0.001f, 0.0f, 1);
    PID_Init(&Current_Iq_PID, 0.001f, 0.001f, 0.0f, 1);
#if MOTOR_TYPE == HAITAI
    PID_Init(&Speed_PID, 0.1f, 0.0000f, 0.0f, 2.0f);//haitai
#elif MOTOR_TYPE == DJI_SNAIL_2305
		PID_Init(&Speed_PID, 0.012f, 0.0000f, 0.0f, 2.0f);//snail
#endif
    PID_Init(&Position_PID, 0.001f, 0.001f, 0.0f, 1);

    // 设置PWM
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
		
		HAL_TIM_Base_Start_IT(&htim3);
}

float x = 0, y = 1;
void FOC_Main_Loop_H_Freq(void)
{
    //位置环PID计算

    // 开环模式：生成模拟电角度信号
    #if FOC_CLOSE_LOOP_MODE == MODE_OFF
    static float t = 0.0;                           // 时间变量
    Motor_FOC.Theta = TWO_PI * t;                   // 生成 sin(πt) 信号             
    t += Motor_FOC.Speed_Rpm * SPEED_STEP;          // 时间步进
    if (t >= 1.0f) t -= 1.0f;                       // 周期1秒
    // 闭环模式：读取实际编码器角度
    #elif FOC_CLOSE_LOOP_MODE == MODE_ON
    MT6816_SPI_Get_AngleData();
    Motor_FOC.Theta = mt6816_spi.angle * ( TWO_PI / 16384.0f );
    #endif


    FOC_Calc_Electrical_Angle(); 
		
    Motor_FOC.Ia = (Motor_ADC.Valtage_Current_A - DRV8323_VREF_DIV_TWO) / DRV8323_ADC_GAIN;
    Motor_FOC.Ib = (Motor_ADC.Valtage_Current_B - DRV8323_VREF_DIV_TWO) / DRV8323_ADC_GAIN;
    Motor_FOC.Ic = (Motor_ADC.Valtage_Current_C - DRV8323_VREF_DIV_TWO) / DRV8323_ADC_GAIN;

    // 克拉克变换
    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
    // 帕克变换
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);

    // 开环模式：设置固定 Vd/Vq 或禁用 PID
    #if FOC_CLOSE_LOOP_MODE == MODE_OFF
    Motor_FOC.Vd = 0.01;  // 示例：设置固定 Vd
		#if MOTOR_TYPE == HAITAI
    Motor_FOC.Vq = 50 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 50;  // 示例：设置固定 Vq
		#elif MOTOR_TYPE == DJI_SNAIL_2305
		Motor_FOC.Vq = 80 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 20;  // 示例：设置固定 Vq
		#endif

    #elif FOC_CLOSE_LOOP_MODE == MODE_ON
    // 闭环模式：执行 PID 计算
    PID_Calc(&Current_Id_PID, 0, Motor_FOC.Id);
    PID_Calc(&Current_Iq_PID, 1, Motor_FOC.Iq);
    Motor_FOC.Vd = Current_Id_PID.Output;
    Motor_FOC.Vq = Current_Iq_PID.Output;
    #endif
	

    // 逆帕克变换
    Inv_Park_transform(Motor_FOC.Vd, Motor_FOC.Vq, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
    // SVPWM计算
    CALC_SVPWM(Motor_FOC.Valpha, Motor_FOC.Vbeta);

    if (Motor_Run.state_led_flag == 10000)
    {
        Motor_Run.state_led_flag = 0;
        HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
	    //HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
    }
    Motor_Run.state_led_flag++;
    Motor_Run.run_flag++;
}

void FOC_Main_Loop_L_Freq(void)
{
    // 速度环PID计算
    PID_Calc(&Speed_PID, Motor_FOC.Speed_Rpm_Expect, Motor_FOC.Speed_Rpm);
		Motor_FOC.Speed_Rpm += Speed_PID.Output;
}

void CALC_SVPWM(float Valpha, float Vbeta)
{
    uint8_t bSector;
    int32_t wX, wY, wZ, wUAlpha, wUBeta;
    uint16_t hTimePhA,hTimePhB,hTimePhC = 0;
 
    wUAlpha = Valpha * T_SQRT3;
    wUBeta = -(Vbeta * T);
 
    wX = wUBeta;
    wY = (wUBeta + wUAlpha)/2;
    wZ = (wUBeta - wUAlpha)/2;
    
    //下面是查找定子电流的扇区号
    if (wY<0)
    {
        if (wZ<0)
        {
            bSector = SECTOR_5;
        }
        else // wZ >= 0
            if (wX<=0)
            {
                bSector = SECTOR_4;
            }
            else // wX > 0
            {
                bSector = SECTOR_3;
            }
    }
    else // wY > 0
    {
        if (wZ>=0)
        {
            bSector = SECTOR_2;
        }
        else // wZ < 0
            if (wX<=0)
            {
                bSector = SECTOR_6;
            }
            else // wX > 0
            {
                bSector = SECTOR_1;
            }
    }							   
 
    switch(bSector)		  //根据所在扇区号，计算三相占空比
    {
    case SECTOR_1:
    case SECTOR_4:
        hTimePhA = (T/8) + ((((T + wX) - wZ)/2)/zeta);
        hTimePhB = hTimePhA + wZ/zeta;
        hTimePhC = hTimePhB - wX/zeta;
        break;
    case SECTOR_2:
    case SECTOR_5:
        hTimePhA = (T/8) + ((((T + wY) - wZ)/2)/zeta);
        hTimePhB = hTimePhA + wZ/zeta;
        hTimePhC = hTimePhA - wY/zeta;
        break;
 
    case SECTOR_3:
    case SECTOR_6:
        hTimePhA = (T/8) + ((((T - wX) + wY)/2)/zeta);
        hTimePhC = hTimePhA - wY/zeta;
        hTimePhB = hTimePhC + wX/zeta;
        break;
    default:
        break;
    }

#if SVPWM_MODE == MODE_ON
    TIM1->CCR1 = hTimePhA;
    TIM1->CCR2 = hTimePhB;
    TIM1->CCR3 = hTimePhC;
#elif SVPWM_MODE == MODE_OFF
    TIM1->CCR1 = PWM_PERIOD * 0.5;
    TIM1->CCR2 = PWM_PERIOD * 0.5;
    TIM1->CCR3 = PWM_PERIOD * 0.5;
#endif

    Motor_FOC.hTimePhA = hTimePhA;
    Motor_FOC.hTimePhB = hTimePhB;
    Motor_FOC.hTimePhC = hTimePhC;
}


// 电角度计算
void FOC_Calc_Electrical_Angle(void)
{
    if (Motor_FOC.Theta > TWO_PI)
    {
        Motor_FOC.Theta -= TWO_PI;
    }
    else if (Motor_FOC.Theta < 0)
    {
        Motor_FOC.Theta += TWO_PI;
    }
}

// 帕克变换，将αβ坐标系下的电流转换为dq坐标系下的电流
void Park_transform(float Ialpha, float Ibeta, float *Id, float *Iq, float Theta)
{
    *Id = Ialpha * arm_cos_f32(Theta) + Ibeta * arm_sin_f32(Theta);
    *Iq = -Ialpha * arm_sin_f32(Theta) + Ibeta * arm_cos_f32(Theta);
}
// 克拉克变换，将Ia,Ib,Ic转换为Ialpha和Ibeta
void Clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta)
{
    *Ialpha = TWO_THIRD * Ia;
    *Ibeta = SQRT3_DIV3 * (Ib - Ic);
}
// 逆帕克变换，将dq坐标系下的电流转换为αβ坐标系下的电流
void Inv_Park_transform(float Id, float Iq, float *Ialpha, float *Ibeta, float Theta)
{
    *Ialpha = Id * arm_cos_f32(Theta) - Iq * arm_sin_f32(Theta);
    *Ibeta = Id * arm_sin_f32(Theta) + Iq * arm_cos_f32(Theta);
}


void FOC_Struct_Init(FOC_Struct *foc)
{
    foc->Ia = 0;
    foc->Ib = 0;
    foc->Ic = 0;
    foc->Ialpha = 0;
    foc->Ibeta = 0;
    foc->Id = 0;
    foc->Iq = 0;
    foc->Vd = 0;
    foc->Vq = 0;
    foc->Valpha = 0;
    foc->Vbeta = 0;
    foc->hTimePhA = 0;
    foc->hTimePhB = 0;
    foc->hTimePhC = 0;
    foc->Speed_Rpm_Expect = MOTOR_SPEED_MAX;
    foc->Speed_Rpm = 0;
    foc->Theta = 0;
}



