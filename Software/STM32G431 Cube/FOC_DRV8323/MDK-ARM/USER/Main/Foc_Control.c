/*
 * @Date: 2025-02-26 18:25:59
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-04-08 17:44:50
 * @FilePath: \FOC_DRV8323\MDK-ARM\USER\Main\Foc_Control.c
 * @Description: Do not edit
 */

#include "Foc_Control.h"

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Max_3(a, b, c) Max(Max(a, b), c)

FOC_Struct Motor_FOC;
FOC_Running_Struct Motor_Run = {0, 0, 0, 0, 0};

PID_TypeDef Current_Id_PID;
PID_TypeDef Current_Iq_PID;
PID_TypeDef Speed_PID;
PID_TypeDef Position_PID;

#define MA600_CS_GPIO_Port GPIOA
#define MA600_CS_Pin GPIO_PIN_15

static float inv_motor_voltage;
static float sqrt3_inv_mv;
static float half_inv_mv;
static const uint8_t sector_map[] = {0, 2, 6, 1, 4, 3, 5, 0};
static const uint8_t time_order[7][3] = {
    {0}, {0,1,2}, {1,0,2}, {2,0,1}, {2,1,0}, {1,2,0}, {0,2,1}
};

void CALC_SVPWM_Init() {
    // 初始化时计算一次，假设MOTOR_VOLTAGE和T在运行时不变
    inv_motor_voltage = 1.0f / MOTOR_VOLTAGE;
    sqrt3_inv_mv = sqrtf(3.0f) * inv_motor_voltage;
    half_inv_mv = 1.5f * inv_motor_voltage; // 调整原计算式中的系数
}

void CALC_SVPWM(float Valpha, float Vbeta) {
    float X, Y, Z, t1, t2;
    uint16_t Sector;
    // 预计算公共项
    float sqrt3_Vbeta = SQRT3 * Vbeta;
    float term3Valpha = 3.0f * Valpha;

    // 计算X, Y, Z（合并常量）
    X = sqrt3_Vbeta * inv_motor_voltage * T;
    Y = (sqrt3_Vbeta + term3Valpha) * (0.5f * inv_motor_voltage) * T;
    Z = (sqrt3_Vbeta - term3Valpha) * (0.5f * inv_motor_voltage) * T;

    // 计算扇区
		uint16_t N = (Vbeta > 0) + 2 * (3*Valpha > sqrt3_Vbeta) + 4 * (-3*Valpha > sqrt3_Vbeta);
    Sector = sector_map[N];

    // 计算t1和t2
    switch (Sector) {
        case 1: t1 = -Z; t2 =  X; break;
        case 2: t1 =  Z; t2 =  Y; break;
        case 3: t1 =  X; t2 = -Y; break;
        case 4: t1 = -X; t2 =  Z; break;
        case 5: t1 = -Y; t2 = -Z; break;
        case 6: t1 =  Y; t2 = -X; break;
        default: t1 = t2 = 0; break;
    }

    // 过调制调整
    float sum = t1 + t2;
    if (sum > T) {
        float scale = T / sum;
        t1 *= scale;
        t2 *= scale;
    }

    // 计算作用时间
    float Ta = (T - t1 - t2) * 0.25f;
    float Tb = Ta + t1 * 0.5f;
    float Tc = Tb + t2 * 0.5f;
    float times[] = {Ta, Tb, Tc};

    // 查表获取时间分配顺序
    const uint8_t *order = time_order[Sector];
    uint16_t hTimePhA = (uint16_t)times[order[0]];
    uint16_t hTimePhB = (uint16_t)times[order[1]];
    uint16_t hTimePhC = (uint16_t)times[order[2]];

    // 更新寄存器
    TIM1->CCR1 = hTimePhA;
    TIM1->CCR2 = hTimePhB;
    TIM1->CCR3 = hTimePhC;

    // 保存到结构体
    Motor_FOC.hTimePhA = hTimePhA;
    Motor_FOC.hTimePhB = hTimePhB;
    Motor_FOC.hTimePhC = hTimePhC;
}

void Park_transform(float Ialpha, float Ibeta, float *Id, float *Iq, float Theta)
{
  // 帕克变换，将αβ坐标系下的电流转换为dq坐标系下的电流
    *Id = Ialpha * arm_cos_f32(Theta) + Ibeta * arm_sin_f32(Theta);
    *Iq = -Ialpha * arm_sin_f32(Theta) + Ibeta * arm_cos_f32(Theta);
}

void Clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta)
{
  // 克拉克变换，将Ia,Ib,Ic转换为Ialpha和Ibeta
    *Ialpha = Ia;
    *Ibeta = SQRT3_DIV3 * (Ib - Ic);
}

void Inv_Park_transform(float Id, float Iq, float *Ialpha, float *Ibeta, float Theta)
{
  // 逆帕克变换，将dq坐标系下的电流转换为αβ坐标系下的电流
    *Ialpha = Id * arm_cos_f32(Theta) - Iq * arm_sin_f32(Theta);
    *Ibeta = Id * arm_sin_f32(Theta) + Iq * arm_cos_f32(Theta);
}

float FOC_Theta_Calc(float Theta)
{
  float electrode_angle;
  electrode_angle = ((uint32_t)(MOTOR_POLE_PAIRS * (Theta + 180))% 360) / 360.0f * TWO_PI;
  return electrode_angle;//rad
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
		foc->Iq_ref = 0;
    foc->Vd = 0;
    foc->Vq = 0;
    foc->Valpha = 0;
    foc->Vbeta = 0;
    foc->hTimePhA = 0;
    foc->hTimePhB = 0;
    foc->hTimePhC = 0;
    foc->Speed_Rpm_Expect = (MOTOR_SPEED_MAX - 105.5f);
    foc->Speed_Rpm = 0;
    foc->Theta = 0;
		foc->Open_Loop_Theta = 0;
}

void FOC_PID_Init(void)
{
    //===========HT4315==========//
		#if MOTOR_TYPE == HT4315
    PID_Init(&Current_Id_PID, PID_DELTA, 40.8f, 5.24f, 0.00f, 0.0f, 0.0f, 5.8f, 0.5f, 0.1f, 0.1f, 0.1f);
    PID_Init(&Current_Iq_PID, PID_DELTA, 40.8f, 5.24f, 0.00f, 0.0f, 0.0f, 5.8f, 0.5f, 0.1f, 0.1f, 0.1f);
		#if FOC_CLOSE_LOOP_MODE == MODE_ON
    PID_Init(&Speed_PID, PID_DELTA, 0.009f, 0.000011f, 0.0000003f, 0.0f, 0.0f, 500.0f, 1.5f, 0.1f, 0.1f, 0.1f);//HT4315
		#elif FOC_CLOSE_LOOP_MODE == MODE_OFF
		PID_Init(&Speed_PID, PID_DELTA, 0.00021f, 0.00000015f, 0.00f, 0.0f, 0.0f, 500.0f, 1.5f, 0.1f, 0.1f, 0.1f);//HT4315
		#endif
	  PID_Init(&Position_PID, PID_POSITION, 0.001f, 0.001f, 0.0f, 0.0f, 0.0f, 200, 200, 0.1f, 0.1f, 0.1f);
	
		//===========DJI_SNAIL_2305==========//
		#elif MOTOR_TYPE == DJI_SNAIL_2305
	  PID_Init(&Current_Id_PID, PID_DELTA, 2.5f, 0.54f, 0.00f, 0.0f, 0.0f, 5.8f, 0.5f, 0.1f, 0.1f, 0.1f);
    PID_Init(&Current_Iq_PID, PID_DELTA, 2.5f, 0.54f, 0.00f, 0.0f, 0.0f, 5.8f, 0.5f, 0.1f, 0.1f, 0.1f);
		#if FOC_CLOSE_LOOP_MODE == MODE_ON
		PID_Init(&Speed_PID, PID_DELTA, 0.0003f, 0.00000001f, 0.0000000f, 0.0f, 0.0f, 3000, 300, 0.1f, 0.000004f, 0.1f);//snail
		#elif FOC_CLOSE_LOOP_MODE == MODE_OFF
    PID_Init(&Speed_PID, PID_DELTA, 0.00003f, 0.00000003f, 0.0000000f, 0.0f, 0.0f, 3000, 300, 0.1f, 0.000004f, 0.1f);//snail
		#endif
		PID_Init(&Position_PID, PID_POSITION, 0.001f, 0.001f, 0.0f, 0.0f, 0.0f, 200, 200, 0.1f, 0.1f, 0.1f);
    #endif
}

//======================FOC main=======================//

void FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    Error_Struct_Init(&Motor_Error);

		DWT_Init(170);
    Encoder_SPI_Init(&MA600_spi, &MA600_diff_Filter , MA600_diff_buffer, &hspi1, MA600_CS_GPIO_Port, MA600_CS_Pin, 0.01f);
		Adc_Init();
		DRV8323_Init();
    FOC_PID_Init();
		CALC_SVPWM_Init();
		
		HAL_TIM_Base_Start (&htim1);
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
		
    setVolume(10); //音量设置
		playStartupTune();    //start music
		
		HAL_TIM_Base_Start_IT(&htim3);
}

void FOC_Main_Loop_H_Freq(void)
{
//----------Theta_Calc---------// 
#if FOC_CLOSE_LOOP_MODE == MODE_OFF
    static float t = 0.0;                           // 时间变量
    Motor_FOC.Open_Loop_Theta = TWO_PI * t;         // 生成 sin(πt) 信号             
    t += Motor_FOC.Speed_Rpm * SPEED_STEP;          // 时间步进
    if (t >= 1.0f) t -= 1.0f;                       // 周期1秒
		Motor_FOC.Theta = FOC_Theta_Calc(Encoder_SPI_Get_Angle(&MA600_spi));
#elif FOC_CLOSE_LOOP_MODE == MODE_ON
    Motor_FOC.Theta = FOC_Theta_Calc(Encoder_SPI_Get_Angle(&MA600_spi));
#endif
		
	
//----------Ia Ib Ic Calc----------//
//		static uint16_t k = 10;
//	
//			Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A) / DRV8323_ADC_GAIN;
//			Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B) / DRV8323_ADC_GAIN;
//			Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C) / DRV8323_ADC_GAIN;
//		if(Motor_FOC.hTimePhA > (Motor_FOC.hTimePhB + k) & Motor_FOC.hTimePhA > (Motor_FOC.hTimePhC + k))
//		{
//			Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B);
//			Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C);
//			Motor_FOC.Ia = - Motor_FOC.Ib - Motor_FOC.Ic;
//		}
//		else if(Motor_FOC.hTimePhB > (Motor_FOC.hTimePhA + k) & Motor_FOC.hTimePhB > (Motor_FOC.hTimePhC + k))
//		{
//			Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A);
//			Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C);
//			Motor_FOC.Ib = - Motor_FOC.Ia - Motor_FOC.Ic;
//		}
//		else if(Motor_FOC.hTimePhC > (Motor_FOC.hTimePhA + k) & Motor_FOC.hTimePhC > (Motor_FOC.hTimePhB + k))
//		{
//			Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A);
//			Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B);
//			Motor_FOC.Ic = - Motor_FOC.Ia - Motor_FOC.Ib;
//		}
//		else 
//		{
			Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A);
			Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B);
			Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C);
//		}
		
//----------Clarke_Park_transform----------//
    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
		
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);

//----------Vd_Vq_Calc----------//
#if FOC_CLOSE_LOOP_MODE == MODE_OFF
    Motor_FOC.Vd = 0.00f;
		#if MOTOR_TYPE == HT4315
    Motor_FOC.Vq = (0.5 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 0.5)*8.8f; 
		#elif MOTOR_TYPE == DJI_SNAIL_2305
		Motor_FOC.Vq = (0.8 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 0.2)*12.0f;
		#endif

#elif FOC_CLOSE_LOOP_MODE == MODE_ON
    PID_SetFdb(&Current_Id_PID, Motor_FOC.Id);
    PID_SetRef(&Current_Id_PID, 0.0f);
    Motor_FOC.Vd += PID_Calc(&Current_Id_PID);

    PID_SetFdb(&Current_Iq_PID, Motor_FOC.Iq);
    PID_SetRef(&Current_Iq_PID, Motor_FOC.Iq_ref);
//    PID_SetRef(&Current_Iq_PID, 0.42f);
    Motor_FOC.Vq += PID_Calc(&Current_Iq_PID);
#endif

		Max(Motor_FOC.Vq,12.0f);
		Min(Motor_FOC.Vq,12.0f);
		Max(Motor_FOC.Vd,12.0f);
		Min(Motor_FOC.Vd,12.0f);
		
//---------Inv_Park_transform----------//
#if FOC_CLOSE_LOOP_MODE == MODE_OFF
    Inv_Park_transform(Motor_FOC.Vd, Motor_FOC.Vq, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Open_Loop_Theta);
#elif FOC_CLOSE_LOOP_MODE == MODE_ON
		Inv_Park_transform(Motor_FOC.Vd, Motor_FOC.Vq, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
#endif
		
//---------SVPWM_transform----------//
    CALC_SVPWM(Motor_FOC.Valpha, Motor_FOC.Vbeta);
		
//---------Running_Frec_Calc----------//
    if (Motor_Run.state_led_flag == 10000)
    {
        Motor_Run.state_led_flag = 0;
//        HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
    }
		if ((Motor_Run.state_led_flag & 0x07) == 0)
    {
				Adc_Val_Decode();
			  Get_ADC_Value();
				Motor_Run.Adc_flag++;
		}
    Motor_Run.state_led_flag++;
    Motor_Run.run_flag++;
}

void FOC_Main_Loop_L_Freq(void)
{
		Encoder_Read_Reg(&MA600_spi);
#if FOC_CLOSE_LOOP_MODE == MODE_OFF
		//速度环PID计算
		PID_SetFdb(&Speed_PID, Motor_FOC.Speed_Rpm);
    PID_SetRef(&Speed_PID, Motor_FOC.Speed_Rpm_Expect);//单位：rad/s
    PID_Calc(&Speed_PID);
		Motor_FOC.Speed_Rpm += PID_GetOutput(&Speed_PID);
#elif FOC_CLOSE_LOOP_MODE == MODE_ON
		PID_SetFdb(&Speed_PID, MOTOR_ENCODER_DIR * Encoder_SPI_Get_Angular_Speed(&MA600_spi));
    PID_SetRef(&Speed_PID, Motor_FOC.Speed_Rpm_Expect * TWO_PI);//单位：rad/s
    PID_Calc(&Speed_PID);
		Motor_FOC.Iq_ref += PID_GetOutput(&Speed_PID);
#endif
}

