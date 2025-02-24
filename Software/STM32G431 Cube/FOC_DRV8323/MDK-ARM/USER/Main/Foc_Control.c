#include "Foc_Control.h"

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Max(a, b, c) Max(Max(a, b), c)

FOC_Struct Motor_FOC;
ADC_Struct Motor_ADC;
PID_Struct Current_Id_PID;
PID_Struct Current_Iq_PID;
PID_Struct Speed_PID;
PID_Struct Position_PID;

float Vref_Offset = 1.0f;

//--------------------函数声明--------------------
void FOC_Calc_Electrical_Angle(void);
void Park_transform(double Ialpha, double Ibeta, double *Id, double *Iq, double Theta);
void Clarke_transform(double Ia, double Ib, double Ic, double *Ialpha, double *Ibeta);
void Inv_Park_transform(double Id, double Iq, double *Ialpha, double *Ibeta, double Theta);
void Inv_Clarke_transform(double Ialpha, double Ibeta, double *Ia, double *Ib, double *Ic);
void CALC_SVPWM(double Valpha, double Vbeta);
void ADC_Struct_Init(ADC_Struct *adc);

//------------------------------------------------

void FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    ADC_Struct_Init(&Motor_ADC);

    Adc_Init();
    SPI_Init();
    ADC_Vrefint_Init();

    // PID初始化
    PID_Init(&Current_Id_PID, 0.001, 0.001, 0.0, 1);
    PID_Init(&Current_Iq_PID, 0.001, 0.001, 0.0, 1);
    PID_Init(&Speed_PID, 0.001, 0.001, 0.0, 1);
    PID_Init(&Position_PID, 0.001, 0.001, 0.0, 1);

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
    // // 计数上限: 500
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250);
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 300);
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
}

uint16_t state_led_flag = 0;
double x = 0, y = 1;
void FOC_Main_Loop(void)
{

    MT6816_SPI_Get_AngleData();
    Motor_FOC.Theta = mt6816_spi.angle / 16384.0 * TWO_PI;
    Motor_FOC.Ia = Motor_ADC.Valtage_Current_A;
    Motor_FOC.Ib = Motor_ADC.Valtage_Current_B;
    Motor_FOC.Ic = Motor_ADC.Valtage_Current_C;

    // 克拉克变换
    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
    // 帕克变换
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);

    // 电流环PID计算
    PID_Calc(&Current_Id_PID, 0, Motor_FOC.Id);
    PID_Calc(&Current_Iq_PID, 1, Motor_FOC.Iq);
    // 逆帕克变换
    // Inv_Park_transform(Current_Id_PID.Output, Current_Iq_PID.Output, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
    
    Inv_Park_transform(x, y, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
    // SVPWM计算
    CALC_SVPWM(Motor_FOC.Valpha, Motor_FOC.Vbeta);

    if (state_led_flag == 1000)
    {
        state_led_flag = 0;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
    state_led_flag++;
}

// 电角度计算
void FOC_Calc_Electrical_Angle(void)
{
    Motor_FOC.Theta = Motor_FOC.Theta + Motor_FOC.Speed * 0.001;
    if (Motor_FOC.Theta > TWO_PI)
    {
        Motor_FOC.Theta -= TWO_PI;
    }
}

// 帕克变换，将αβ坐标系下的电流转换为dq坐标系下的电流
void Park_transform(double Ialpha, double Ibeta, double *Id, double *Iq, double Theta)
{
    *Id = Ialpha * cos(Theta) + Ibeta * sin(Theta);
    *Iq = -Ialpha * sin(Theta) + Ibeta * cos(Theta);
}
// 克拉克变换，将Ia,Ib,Ic转换为Ialpha和Ibeta
void Clarke_transform(double Ia, double Ib, double Ic, double *Ialpha, double *Ibeta)
{
    *Ialpha = TWO_THIRD * Ia;
    *Ibeta = SQRT3_DIV3 * (Ib - Ic);
}
// 逆帕克变换，将dq坐标系下的电流转换为αβ坐标系下的电流
void Inv_Park_transform(double Id, double Iq, double *Ialpha, double *Ibeta, double Theta)
{
    *Ialpha = Id * cos(Theta) - Iq * sin(Theta);
    *Ibeta = Id * sin(Theta) + Iq * cos(Theta);
}
// 逆克拉克变换，将Ialpha和Ibeta转换为Ia,Ib,Ic
void Inv_Clarke_transform(double Ialpha, double Ibeta, double *Ia, double *Ib, double *Ic)
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
}


#define SQRT_3		1.732051           //根号3
#define T		    (PWM_PERIOD * 4)   //TIM1 ARR值的4倍
#define T_SQRT3     (uint16_t)(T * SQRT_3)
#define SECTOR_1	(uint32_t)1
#define SECTOR_2	(uint32_t)2
#define SECTOR_3	(uint32_t)3
#define SECTOR_4	(uint32_t)4
#define SECTOR_5	(uint32_t)5
#define SECTOR_6	(uint32_t)6

int32_t wX, wY, wZ, wUAlpha, wUBeta;
uint16_t  hTimePhA=0, hTimePhB=0, hTimePhC=0;		
uint8_t bSector;

void CALC_SVPWM(double Valpha, double Vbeta)
{
    // uint8_t bSector;
    // uint32_t wX, wY, wZ, wUAlpha, wUBeta;
    // uint16_t  hTimePhA=0, hTimePhB=0, hTimePhC=0;		
 
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
        hTimePhA = (T/8) + ((((T + wX) - wZ)/2)/24);
        hTimePhB = hTimePhA + wZ/24;
        hTimePhC = hTimePhB - wX/24;
        break;
    case SECTOR_2:
    case SECTOR_5:
        hTimePhA = (T/8) + ((((T + wY) - wZ)/2)/24);
        hTimePhB = hTimePhA + wZ/24;
        hTimePhC = hTimePhA - wY/24;
        break;
 
    case SECTOR_3:
    case SECTOR_6:
        hTimePhA = (T/8) + ((((T - wX) + wY)/2)/131072);
        hTimePhC = hTimePhA - wY/131072;
        hTimePhB = hTimePhC + wX/131072;
        break;
    default:
        break;
    }
 
    // TIM1->CCR1 = hTimePhA;
    // TIM1->CCR2 = hTimePhB;
    // TIM1->CCR3 = hTimePhC;
    // 计数上限: 500
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, hTimePhA);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, hTimePhB);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, hTimePhC);
}

