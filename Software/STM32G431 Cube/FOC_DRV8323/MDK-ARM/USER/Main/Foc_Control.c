/*
 * @Date: 2025-02-26 18:25:59
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-01 16:57:17
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Main\Foc_Control.c
 * @Description: Do not edit
 */

#include "Foc_Control.h"

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Max_3(a, b, c) Max(Max(a, b), c)

FOC_Struct Motor_FOC;
ADC_Struct Motor_ADC;
PID_Struct Current_Id_PID;
PID_Struct Current_Iq_PID;
PID_Struct Speed_PID;
PID_Struct Position_PID;

float Vref_Offset = 1.0f;

//--------------------函数声明--------------------
void FOC_Calc_Electrical_Angle(void);
void Park_transform(float Ialpha, float Ibeta, float *Id, float *Iq, float Theta);
void Clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta);
void Inv_Park_transform(float Id, float Iq, float *Ialpha, float *Ibeta, float Theta);
void Inv_Clarke_transform(float Ialpha, float Ibeta, float *Ia, float *Ib, float *Ic);
void CALC_SVPWM(float Valpha, float Vbeta);
void FOC_Struct_Init(FOC_Struct *foc);
void ADC_Struct_Init(ADC_Struct *adc);
void DRV8323_CAL_Init(void);

//------------------------------------------------

void FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    ADC_Struct_Init(&Motor_ADC);
    Error_Struct_Init(&Motor_Error);

    DRV8323_GPIO_Init();
		DRV8323_CAL_Init();
    Adc_Init();
    SPI_Init();
    ADC_Vrefint_Init();

    // PID初始化
    PID_Init(&Current_Id_PID, 0.001f, 0.001f, 0.0f, 1);
    PID_Init(&Current_Iq_PID, 0.001f, 0.001f, 0.0f, 1);
    PID_Init(&Speed_PID, 0.001f, 0.001f, 0.0f, 1);
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
uint32_t run_flag = 0;
uint16_t state_led_flag = 0;
float x = 0, y = 1;
void FOC_Main_Loop(void)
{
    // 开环模式：生成模拟电角度信号
    #if FOC_CLOSE_LOOP_MODE == MODE_OFF
    static float t = 0.0;                         // 时间变量
    Motor_FOC.Theta = PI * t;                 // 生成 sin(πt) 信号             
    t += 0.0001;                                    // 时间步进（假设循环周期为1ms）
    if (t >= 2.0f) t -= 2.0f;                       // 周期2秒
    // 闭环模式：读取实际编码器角度
    #elif FOC_CLOSE_LOOP_MODE == MODE_ON
    MT6816_SPI_Get_AngleData();
    Motor_FOC.Theta = mt6816_spi.angle * ( TWO_PI / 16384.0f );
    #endif

    FOC_Calc_Electrical_Angle(); 
		
    Motor_FOC.Ia = Max(Motor_ADC.Valtage_Current_A / CURRENT_DETECTION_RES, 0);
    Motor_FOC.Ib = Max(Motor_ADC.Valtage_Current_B / CURRENT_DETECTION_RES, 0);
    Motor_FOC.Ic = Max(Motor_ADC.Valtage_Current_C / CURRENT_DETECTION_RES, 0);

    // 克拉克变换
    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
    // 帕克变换
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);

    // 开环模式：设置固定 Vd/Vq 或禁用 PID
    #if FOC_CLOSE_LOOP_MODE == MODE_OFF
    Motor_FOC.Vd = 0.01;  // 示例：设置固定 Vd
    Motor_FOC.Vq = 70.9;  // 示例：设置固定 Vq
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

    if (state_led_flag == 10000)
    {
        state_led_flag = 0;
        HAL_GPIO_TogglePin(DRV8323_PORT, LED_Pin);
	    //HAL_GPIO_TogglePin(DRV8323_PORT, LED_Pin);
    }
    state_led_flag++;
    run_flag++;
}

uint8_t bSector;
int32_t wX, wY, wZ, wUAlpha, wUBeta;
uint16_t  hTimePhA=0, hTimePhB=0, hTimePhC=0;		


void CALC_SVPWM(float Valpha, float Vbeta)
{
//    uint8_t bSector;
//    int32_t wX, wY, wZ, wUAlpha, wUBeta;
//    uint16_t  hTimePhA=0, hTimePhB=0, hTimePhC=0;		
 
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
    // 计数上限: 680
    TIM1->CCR1 = hTimePhA;
    TIM1->CCR2 = hTimePhB;
    TIM1->CCR3 = hTimePhC;
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
// 逆克拉克变换，将Ialpha和Ibeta转换为Ia,Ib,Ic
void Inv_Clarke_transform(float Ialpha, float Ibeta, float *Ia, float *Ib, float *Ic)
{
    *Ia = Ialpha;
    *Ib = -0.5 * Ialpha + SQRT3_DIV2 * Ibeta;
    *Ic = -0.5 * Ialpha - SQRT3_DIV2 * Ibeta;
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
    foc->Va = 0;
    foc->Vb = 0;
    foc->Vc = 0;
    foc->Theta = 0;
}

void ADC_Struct_Init(ADC_Struct *adc)
{
    adc->Valtage_Current_A = 0;
    adc->Valtage_Current_B = 0;
    adc->Valtage_Current_C = 0;
    adc->Valtage_VCC = 0;
    adc->Temperature = 0;
    adc->Internal_Vref = 0;
    adc->A_Offset = (0.005149f+0.1659f);
    adc->B_Offset = (0.000277f+0.0379f);
    adc->C_Offset = (0.000584f+0.00418f);
}


