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

PID_TypeDef Current_Id_PID;
PID_TypeDef Current_Iq_PID;
PID_TypeDef Speed_PID;
PID_TypeDef Position_PID;

Encoder_SPI_HandleTypeDef MA600_spi;

#define MA600_CS_GPIO_Port GPIOA
#define MA600_CS_Pin GPIO_PIN_15



void CALC_SVPWM(float Valpha, float Vbeta)
{
    uint8_t bSector;
    int32_t wX, wY, wZ, wUAlpha, wUBeta;
    uint16_t hTimePhA,hTimePhB,hTimePhC = 0;
 
    wUAlpha = Valpha * T_SQRT3;
    wUBeta = -(Vbeta * T_2);
 
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
        hTimePhA = (T_2/8) + ((((T_2 + wX) - wZ)/2)/zeta);
        hTimePhB = hTimePhA + wZ/zeta;
        hTimePhC = hTimePhB - wX/zeta;
        break;
    case SECTOR_2:
    case SECTOR_5:
        hTimePhA = (T_2/8) + ((((T_2 + wY) - wZ)/2)/zeta);
        hTimePhB = hTimePhA + wZ/zeta;
        hTimePhC = hTimePhA - wY/zeta;
        break;
 
    case SECTOR_3:
    case SECTOR_6:
        hTimePhA = (T_2/8) + ((((T_2 - wX) + wY)/2)/zeta);
        hTimePhC = hTimePhA - wY/zeta;
        hTimePhB = hTimePhC + wX/zeta;
        break;
    default:
        break;
    }

    TIM1->CCR1 = hTimePhA;
    TIM1->CCR2 = hTimePhB;
    TIM1->CCR3 = hTimePhC;

    Motor_FOC.hTimePhA = hTimePhA;
    Motor_FOC.hTimePhB = hTimePhB;
    Motor_FOC.hTimePhC = hTimePhC;

// 	 float temp;
// 	 float X,Y,Z, t1,t2;
// 	 uint16_t A,B,C,N,Sector;
//   uint16_t hTimePhA,hTimePhB,hTimePhC = 0;
// 	 float Ta, Tb, Tc;
// 	 //p->T=1.0;//Normalize the whole modulation period
// 	 X= SQRT3*Vbeta/MOTOR_VOLTAGE*T;
// 	 Y=(SQRT3*Vbeta+3*Valpha)/(2*MOTOR_VOLTAGE)*T;
// 	 Z=(SQRT3*Vbeta-3*Valpha)/(2*MOTOR_VOLTAGE)*T;
// 	//
// 	 if(Vbeta>0)
// 	   {A=1;}
// 	 else
// 	   {A=0;}

// 	 if( (SQRT3*Valpha - Vbeta)>0 )
// 	   {B=1;}
// 	 else
// 	   {B=0;}

// 	 if((-SQRT3*Valpha - Vbeta)>0)
// 	   {C=1;}
// 	 else
// 	   {C=0;}

// 	 N=A+2*B+4*C;
// //
// 	 switch(N)
// 	 {
// 		case 1:{Sector=2;break;}
// 		case 2:{Sector=6;break;}
// 		case 3:{Sector=1;break;}
// 		case 4:{Sector=4;break;}
// 		case 5:{Sector=3;break;}
// 		case 6:{Sector=5;break;}
// 		 default:{;}
// 	 }
//  //
// 	switch(Sector)
// 	{
// 		case 1: {t1=-Z; t2= X;break;}
// 		case 2: {t1= Z; t2= Y;break;}
// 		case 3: {t1= X; t2=-Y;break;}
// 		case 4: {t1=-X; t2= Z;break;}
// 		case 5: {t1=-Y; t2=-Z;break;}
// 		case 6: {t1= Y; t2=-X;break;}
// 		  default:{;}
// 	}

// 	if((t1+t2)>T)//对过调制情况进行调整
// 	 {
// 		  temp=t1+t2;
// 		  t1=t1*T/temp;
// 		  t2=t2*T/temp;
// 	 }

// 	//
// 	 Ta=(T-t1-t2)/4;//作用时间分配
// 	 Tb=Ta+t1/2;
// 	 Tc=Tb+t2/2;

// 	 switch(Sector)
// 	  {
// 		case 1: {hTimePhA=Ta; hTimePhB=Tb; hTimePhC=Tc; break;}
// 		case 2: {hTimePhA=Tb; hTimePhB=Ta; hTimePhC=Tc; break;}
// 		case 3: {hTimePhA=Tc; hTimePhB=Ta; hTimePhC=Tb; break;}
// 		case 4: {hTimePhA=Tc; hTimePhB=Tb; hTimePhC=Ta; break;}
// 		case 5: {hTimePhA=Tb; hTimePhB=Tc; hTimePhC=Ta; break;}
// 		case 6: {hTimePhA=Ta; hTimePhB=Tc; hTimePhC=Tb; break;}
// 		  default:{;}
// 	  }
	
//     TIM1->CCR1 = hTimePhA;
//     TIM1->CCR2 = hTimePhB;
//     TIM1->CCR3 = hTimePhC;

//     Motor_FOC.hTimePhA = hTimePhA;
//     Motor_FOC.hTimePhB = hTimePhB;
//     Motor_FOC.hTimePhC = hTimePhC;
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
    *Ialpha = TWO_THIRD * Ia;
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
  electrode_angle = MOTOR_POLE_PAIRS * Theta;
  electrode_angle = (uint32_t)electrode_angle % 360;
  return electrode_angle;
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
    foc->Speed_Rpm_Expect = MOTOR_SPEED_MAX - 2;
    foc->Speed_Rpm = 0;
    foc->Theta = 0;
}

void FOC_PID_Init(void)
{
    // PID初始化
    PID_Init(&Current_Id_PID, PID_DELTA, 0.001f, 0.001f, 0.0f, 0.0f, 0.0f, 50, 50, 0.1, 0.1, 0.1);
    PID_Init(&Current_Iq_PID, PID_DELTA, 0.001f, 0.001f, 0.0f, 0.0f, 0.0f, 100, 100, 0.1, 0.1, 0.1);
    #if MOTOR_TYPE == HAITAI
    PID_Init(&Speed_PID, PID_POSITION, 0.01f, 0.00001f, 0.0f, 0.0f, 0.0f, 10, 10, 0.1, 0.1, 0.1);//haitai
    #elif MOTOR_TYPE == DJI_SNAIL_2305
    PID_Init(&Speed_PID, PID_POSITION, 0.012f, 0.0000f, 0.0f, 0.0f, 0.0f, 300, 300, 0.1, 0.1, 0.1);//snail
    #endif
    PID_Init(&Position_PID, PID_POSITION, 0.001f, 0.001f, 0.0f, 0.0f, 0.0f, 200, 200, 0.1, 0.1, 0.1);
}

//======================FOC main=======================//

void FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    Error_Struct_Init(&Motor_Error);
    Adc_Init();
		DWT_Init(170);
    Encoder_SPI_Init(&MA600_spi, &hspi1, MA600_CS_GPIO_Port, MA600_CS_Pin, 0.01f);
		
		DRV8323_Init();

    FOC_PID_Init();

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

void FOC_Main_Loop_H_Freq(void)
{
#if FOC_CLOSE_LOOP_MODE == MODE_OFF
    static float t = 0.0;                           // 时间变量
    Motor_FOC.Theta = TWO_PI * t;                   // 生成 sin(πt) 信号             
    t += Motor_FOC.Speed_Rpm * SPEED_STEP;          // 时间步进
    if (t >= 1.0f) t -= 1.0f;                       // 周期1秒
#elif FOC_CLOSE_LOOP_MODE == MODE_ON
    Motor_FOC.Theta = FOC_Theta_Calc(Encoder_SPI_Get_Angle(&MA600_spi));
#endif
		
    // Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A) / DRV8323_ADC_GAIN;
    // Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B) / DRV8323_ADC_GAIN;
    // Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C) / DRV8323_ADC_GAIN;
    Motor_FOC.Ia = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_A);
    Motor_FOC.Ib = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_B);
    Motor_FOC.Ic = (DRV8323_VREF_DIV_TWO - Motor_ADC.Valtage_Current_C);

    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);

#if FOC_CLOSE_LOOP_MODE == MODE_OFF
    Motor_FOC.Vd = 0.00f;
		#if MOTOR_TYPE == HT4315
    Motor_FOC.Vq = (0.5 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 0.5)*13.5f; 
		#elif MOTOR_TYPE == DJI_SNAIL_2305
		Motor_FOC.Vq = (0.8 * Motor_FOC.Speed_Rpm / Motor_FOC.Speed_Rpm_Expect + 0.2)*10.0f;
		#endif

#elif FOC_CLOSE_LOOP_MODE == MODE_ON
    PID_SetFdb(&Current_Id_PID, Motor_FOC.Id);
    PID_SetRef(&Current_Id_PID, 0);
    Motor_FOC.Vd += PID_Calc(&Current_Id_PID);

    PID_SetFdb(&Current_Iq_PID, Motor_FOC.Iq);
    PID_SetRef(&Current_Iq_PID, 0.1f);
    Motor_FOC.Vq += PID_Calc(&Current_Iq_PID);
#endif
	
    Inv_Park_transform(Motor_FOC.Vd, Motor_FOC.Vq, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
    CALC_SVPWM(Motor_FOC.Valpha, Motor_FOC.Vbeta);

    if (Motor_Run.state_led_flag == 10000)
    {
        Motor_Run.state_led_flag = 0;
        HAL_GPIO_TogglePin(LED_PORT, LED_Pin);
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
		PID_SetFdb(&Speed_PID, 0);
    PID_SetRef(&Speed_PID, Motor_FOC.Speed_Rpm_Expect);//单位：rad/s
    PID_Calc(&Speed_PID);
		Motor_FOC.Speed_Rpm += PID_GetOutput(&Speed_PID);
#endif
}

