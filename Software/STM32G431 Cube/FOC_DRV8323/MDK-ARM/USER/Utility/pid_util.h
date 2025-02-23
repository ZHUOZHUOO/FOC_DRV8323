#ifndef __PID_UTIL_H
#define __PID_UTIL_H

#include "stm32g4xx_hal.h"
#include "math.h"

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float Integral;//积分项
    float PreError;//上一次误差
    float PrePreError;//上上次误差
    float Output;//输出
    float OutputMax;//输出限幅
} PID_Struct;

void PID_Init(PID_Struct *pid, float Kp, float Ki, float Kd, float OutputMax);
void PID_Calc(PID_Struct *pid, float Expect, float FeedBack);

#endif
