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

FOC_Main_Init(void)
{
    FOC_Struct_Init(&Motor_FOC);
    ADC_Struct_Init(&Motor_ADC);

    Adc_Init();
    SPI_Init();
    ADC_Vrefint_Init();

    PID_Init(&Current_PID, 0.001, 0.001, 0.0, 1);
    PID_Init(&Speed_PID, 0.001, 0.001, 0.0, 1);
    PID_Init(&Position_PID, 0.001, 0.001, 0.0, 1);
}

FOC_Main_Loop(void)
{
    //获取电机反馈数据
    MT6816_SPI_Get_AngleData();
    Motor_FOC.Theta = mt6816_spi.angle / 16384.0 * TWO_PI;
    Motor_FOC.Ia = Motor_ADC.Valtage_Current_A;
    Motor_FOC.Ib = Motor_ADC.Valtage_Current_B;
    Motor_FOC.Ic = Motor_ADC.Valtage_Current_C;

    //克拉克变换
    Clarke_transform(Motor_FOC.Ia, Motor_FOC.Ib, Motor_FOC.Ic, &Motor_FOC.Ialpha, &Motor_FOC.Ibeta);
    //帕克变换
    Park_transform(Motor_FOC.Ialpha, Motor_FOC.Ibeta, &Motor_FOC.Id, &Motor_FOC.Iq, Motor_FOC.Theta);
    
    //电流环PID计算
    PID_Calc(&Current_Id_PID, Motor_FOC.Id, Motor_FOC.Id);
    PID_Calc(&Current_Iq_PID, Motor_FOC.Iq, Motor_FOC.Iq);
    //逆帕克变换
    Inv_Park_transform(Current_Id_PID.Output, Current_Iq_PID.Output, &Motor_FOC.Valpha, &Motor_FOC.Vbeta, Motor_FOC.Theta);
    //SVPWM计算
    SVPWM_Calc(Motor_FOC.Valpha, Motor_FOC.Vbeta);

}

//电角度计算
void FOC_Calc_Electrical_Angle(void)
{
    Motor_FOC.Theta = Motor_FOC.Theta + Motor_FOC.Speed * 0.001;
    if (Motor_FOC.Theta > TWO_PI)
    {
        Motor_FOC.Theta -= TWO_PI; 
    }
}

//帕克变换，将αβ坐标系下的电流转换为dq坐标系下的电流
void Park_transform(double Ialpha, double Ibeta, double *Id, double *Iq, double Theta)
{
    *Id = Ialpha * cos(Theta) + Ibeta * sin(Theta);
    *Iq = -Ialpha * sin(Theta) + Ibeta * cos(Theta);
}
//克拉克变换，将Ia,Ib,Ic转换为Ialpha和Ibeta
void Clarke_transform(double Ia, double Ib, double Ic, double *Ialpha, double *Ibeta)
{
    *Ialpha = TWO_THIRD * Ia;
    *Ibeta = SQRT3_DIV3 * (Ib - Ic);
}
//逆帕克变换，将dq坐标系下的电流转换为αβ坐标系下的电流
void Inv_Park_transform(double Id, double Iq, double *Ialpha, double *Ibeta, double Theta)
{
    *Ialpha = Id * cos(Theta) - Iq * sin(Theta);
    *Ibeta = Id * sin(Theta) + Iq * cos(Theta);
}
//逆克拉克变换，将Ialpha和Ibeta转换为Ia,Ib,Ic
void Inv_Clarke_transform(double Ialpha, double Ibeta, double *Ia, double *Ib, double *Ic)
{
    *Ia = Ialpha;
    *Ib = -0.5 * Ialpha + SQRT3_DIV2 * Ibeta;
    *Ic = -0.5 * Ialpha - SQRT3_DIV2 * Ibeta;
}
//快速SVPWM算法
void SVPWM_Calc(double Valpha, double Vbeta, double *Va, double *Vb, double *Vc)
{
    Inv_Clarke_transform(Valpha, Vbeta, Va, Vb, Vc);

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



