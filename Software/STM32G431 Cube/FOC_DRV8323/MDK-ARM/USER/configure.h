/*
 * @Date: 2025-03-09 20:13:14
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-10 11:26:13
 * @FilePath: \MDK-ARM\USER\configure.h
 * @Description: Do not edit
 */
#ifndef __CONFIGURE_H
#define __CONFIGURE_H

//Hardware Version
#define HARDWARE_VERSION VERSION_3
//Motor Type
#define MOTOR_TYPE HT4315

#define VERSION_1 1 //三相电路错误
#define VERSION_2 2 //M2固定
#define VERSION_3 3 //M3固定,双板设计,选取Qgd较小的MOS管

#define HT4315 0
#define DJI_SNAIL_2305 1

//闭环模式
#define FOC_CLOSE_LOOP_MODE MODE_OFF
//PWM
//SVPWM模式，可变占空比模式
#define SVPWM_MODE MODE_ON
//ADC电压校准模式
#define ADC_VREF_MODE MODE_OFF
//滤波模式, Sliding Window Filter
#define ADC_FILTER_MODE MODE_ON
#define SLIDING_WINDOW_SIZE 8
//错误处理模式
#define ERROR_MODE MODE_ON
//nFAULT中断处理模式 MODE_ON:中断 MODE_OFF:轮询
#define N_FAULT_MODE MODE_OFF

#define MODE_ON 1
#define MODE_OFF 0

#define CKTIM 170000000//定时器时钟频率
#define PWM_PRSC 1-1//PWM预分频
#define PWM_FREQ 25000//PWM频率, T=1000us
#define PWM_PERIOD CKTIM/(2*PWM_FREQ*(PWM_PRSC+1))  //计数器计数上限 ARR=3400
#define REP_RATE 1 //电流环刷新频率为(REP_RATE+1)/(2*PWM_FREQ)=40us, f=25kHz
#define DEADTIME_NS 1000//死区时间ns
#define DEADTIME CKTIM/1000000/2*DEADTIME_NS/1000  //死区时间计数值 85us

#define SPEED_STEP (float)(MOTOR_POLE_PAIRS / (float)PWM_FREQ) //速度步进

#define ALIGNMENT_ANGLE 300
#define COUNTER_RESET (ALIGNMENT_ANGLE*4*ENCODER_PPR/360-1)/POLE_PAIR_NUM
#define ICx_FILTER 8


#if MOTOR_TYPE == HT4315
	#define MOTOR_POLE_PAIRS 14 //电机极对数
	#define MOTOR_VOLTAGE 24 //电机电压
	#define MOTOR_RESISTANCE 16.5 //电机电阻
	#define MOTOR_INDUCTANCE 0.0042 //电机电感
	#define MOTOR_SPEED_MAX 8.4 //电机最大速度(空载)
	#define MOTOR_CURRENT_MAX 0.92 //电机最大电流
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MT6816
	#define MOTOR_ENCODER_DIR 1 //电机编码器方向

	#define SPEED_CONSTANT 20 //转速常数Kn
	#define TORQUE_CONSTANT 0.45 //转矩常数KT
#elif MOTOR_TYPE == DJI_SNAIL_2305
	#define MOTOR_POLE_PAIRS 7 //电机极对数
	#define MOTOR_VOLTAGE 14.8 //电机电压
	#define MOTOR_RESISTANCE 0.061 //电机电阻
	#define MOTOR_INDUCTANCE 0.000014 //电机电感
	#define MOTOR_SPEED_MAX  200//电机最大速度(空载)
	#define MOTOR_CURRENT_MAX 2 //电机最大电流
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MT6816
	#define MOTOR_ENCODER_DIR 1 //电机编码器方向
	
	#define SPEED_CONSTANT 2400 //转速常数Kn
	#define TORQUE_CONSTANT 0.0054267 //转矩常数KT
	#define	BACK_ELEC_FORCE_CONSTANT	0.056364//反电动势常数Ke
#endif

#endif
