#include "pid_util.h"

void PID_Init(PID_Struct *pid, float Kp, float Ki, float Kd, float OutputMax)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->OutputMax = OutputMax;
    pid->Integral = 0;
    pid->PreError = 0;
    pid->PrePreError = 0;
    pid->Output = 0;
}

void PID_Calc(PID_Struct *pid, float Expect, float FeedBack)
{
    float Error = Expect - FeedBack;
    pid->Integral += Error;
    pid->Output = pid->Kp * Error + pid->Ki * pid->Integral + pid->Kd * (Error - pid->PreError);
    pid->PrePreError = pid->PreError;
    pid->PreError = Error;
    if(pid->Output > pid->OutputMax)
        pid->Output = pid->OutputMax;
    else if(pid->Output < -pid->OutputMax)
        pid->Output = -pid->OutputMax;
}
