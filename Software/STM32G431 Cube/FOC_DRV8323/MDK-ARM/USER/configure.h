#ifndef __CONFIGURE_H
#define __CONFIGURE_H
//闭环模式
#define FOC_CLOSE_LOOP_MODE MODE_OFF
//ADC电压校准模式
#define ADC_VREF_MODE MODE_ON
//滤波模式, Sliding Window Filter
#define ADC_FILTER_MODE MODE_ON
#define SLIDING_WINDOW_SIZE 8

#define MODE_ON 1
#define MODE_OFF 0

#define CURRENT_DETECTION_RES 0.002f//检流电阻
#define MOTOR_POLE_PAIRS 14 //电机极对数
#define MOTOR_RESISTANCE 16.5 //电机电阻
#define MOTOR_VOLTAGE 24 //电机电压
#define MOTOR_INDUCTANCE 0.0042 //电机电感
#define MOTOR_SPEED_MAX 250 //电机最大速度
#define MOTOR_CURRENT_MAX 2 //电机最大电流
#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MT6816
#define MOTOR_ENCODER_DIR 1 //电机编码器方向

#define CKTIM 170000000//定时器时钟频率
#define PWM_PRSC 0//PWM预分频
#define PWM_FREQ 25000//PWM频率, T=1000us
#define PWM_PERIOD CKTIM/(2*PWM_FREQ*(PWM_PRSC+1))  //计数器计数上限 ARR=3400
#define REP_RATE 1 //电流环刷新频率为(REP_RATE+1)/(2*PWM_FREQ)=40us, f=25kHz
#define DEADTIME_NS 1000//死区时间ns
#define DEADTIME CKTIM/1000000/2*DEADTIME_NS/1000  //死区时间计数值 85us

#define ALIGNMENT_ANGLE 300
#define COUNTER_RESET (ALIGNMENT_ANGLE*4*ENCODER_PPR/360-1)/POLE_PAIR_NUM
#define ICx_FILTER 8

#endif
