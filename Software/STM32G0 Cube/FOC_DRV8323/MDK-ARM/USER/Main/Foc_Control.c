#include "Foc_Control.h"

FOC_Struct Motor_FOC_Expect={0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
FOC_Struct Motor_FOC_FeedBack={0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
ADC_Struct Motor_ADC={0 , 0 , 0 , 0 , 0 , 0, 0, 0};
PID_Struct Current_PID;
PID_Struct Speed_PID;
PID_Struct Position_PID;

float Vref_Offset = 1.0f;

FOC_Main_Init(void)
{
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
    Motor_FOC_FeedBack.Theta = mt6816_spi.angle / 16384.0 * TWO_PI;
    Motor_FOC_FeedBack.Ia = Motor_ADC.Valtage_Current_A;
    Motor_FOC_FeedBack.Ib = Motor_ADC.Valtage_Current_B;
    Motor_FOC_FeedBack.Ic = Motor_ADC.Valtage_Current_C;

    //位置环

    //克拉克变换
    Clarke_transform(Motor_FOC_FeedBack.Ia, Motor_FOC_FeedBack.Ib, Motor_FOC_FeedBack.Ic, &Motor_FOC_FeedBack.Ialpha, &Motor_FOC_FeedBack.Ibeta);
    //帕克变换
    Park_transform(Motor_FOC_FeedBack.Ialpha, Motor_FOC_FeedBack.Ibeta, &Motor_FOC_FeedBack.Id, &Motor_FOC_FeedBack.Iq, Motor_FOC_FeedBack.Theta);
    

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

//ADC DMA中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        //读取ADC1的值
        for(int i = 0; i < ADC1_CHANNEL_NUM; i++)
        {
            Adc_Sum_Val[i] = Adc_Val[i]/4095.0*3.3;
        }//将ADC1的值转换为电压值
        Motor_ADC.Valtage_Current_A = Adc_Val[0] * Vref_Offset;
        Motor_ADC.Valtage_Current_B = Adc_Val[1] * Vref_Offset;
        Motor_ADC.Valtage_Current_C = Adc_Val[2] * Vref_Offset;
        Motor_ADC.Valtage_VCC = Adc_Val[3] * 11.0 * Vref_Offset;
        Motor_ADC.Temperature = (Adc_Val[4] * Vref_Offset - 0.76) / 0.0025 + 25;
        Motor_ADC.Internal_Vref = Adc_Val[5];
    }
}


void ADC_Vrefint_Init(void)
{
	__IO uint16_t* VREFINT_CAL = (__IO uint16_t *)(0x1FFF75AA);

	float VREFINT_CAL_DATA = 0;
	float VREFINT_CAL_VAL  = 0;
	float Vref_Offset_Sum  = 0.0f;
	
	VREFINT_CAL_DATA = (float)*VREFINT_CAL;
	VREFINT_CAL_VAL = (VREFINT_CAL_DATA/4095.0f * 3.0f);
	for(int flag=0;flag<1000;flag++)
	{
			Vref_Offset_Sum += VREFINT_CAL_VAL / Motor_ADC.Internal_Vref;
			osDelay(1);
	}
	Vref_Offset = Vref_Offset_Sum / 1000;

	HAL_Delay(20);
}
 


