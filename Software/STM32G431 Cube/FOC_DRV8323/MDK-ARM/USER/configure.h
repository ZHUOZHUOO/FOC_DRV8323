/*
 * @Date: 2025-03-09 20:13:14
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-03-10 11:26:13
 * @FilePath: \MDK-ARM\USER\configure.h
 * @Description: Do not edit
 */
#ifndef __CONFIGURE_H
#define __CONFIGURE_H

//-----------Version Setting------------//
//Hardware Version
#define HARDWARE_VERSION VERSION_2
//Motor Type
#define MOTOR_TYPE HT4315
//Encoder Type
#define ENCODER_TYPE MA600

//-----------FDCAN ID Setting------------//
#define WHO_AM_I Slave1_Arm_ID

#define Tx_Master_ID 	WHO_AM_I | 0x100
#define Rx_Master_ID 	WHO_AM_I

#define Master_ID 		0x410
#define Slave0_Arm_ID 	0x420
#define Slave1_Arm_ID 	0x430
#define Slave2_Arm_ID 	0x440
#define Slave0_End_ID 	0x450
#define Slave1_End_ID 	0x460
#define Slave2_End_ID 	0x470

//-----------Mode Setting--------------//

//闭环模式
#define FOC_CLOSE_LOOP_MODE MODE_POSITION
//零点校准模式
#define ZERO_POINT_MODE MODE_OFF
//ADC电压校准模式
#define ADC_VREF_MODE MODE_OFF
//滤波模式, Sliding Window Filter
#define ADC_FILTER_MODE MODE_ON
#define SLIDING_WINDOW_SIZE 8
//错误处理模式
#define ERROR_MODE MODE_OFF
//nFAULT中断处理模式 MODE_ON:中断 MODE_OFF:轮询
#define N_FAULT_MODE MODE_OFF
//MA600差分滤波窗口宽度
#define DIFF_SLIDING_WINDOW_SIZE 64
//MA600角度滤波窗口宽度
#define ANGLE_SLIDING_WINDOW_SIZE 1

#define MODE_OFF 0
#define MODE_ON 1
#define MODE_SPEED 2
#define MODE_POSITION 3
#define MODE_FORCE 4

//------------PWM Setting-------------//
#define CKTIM 170000000//定时器时钟频率
#define PWM_PRSC 1-1//PWM预分频
#define PWM_FREQ 20000//PWM频率, T=1000us
#define PWM_PERIOD CKTIM/(2*PWM_FREQ*(PWM_PRSC+1))  //计数器计数上限 ARR=3400
#define REP_RATE 1 //电流环刷新频率为(REP_RATE+1)/(2*PWM_FREQ)=40us, f=25kHz
#define DEADTIME_NS 1000//死区时间ns
#define DEADTIME CKTIM/1000000/2*DEADTIME_NS/1000  //死区时间计数值 85us

#define SPEED_STEP (float)(MOTOR_POLE_PAIRS / (float)PWM_FREQ) //速度步进

#define ALIGNMENT_ANGLE 300
#define COUNTER_RESET (ALIGNMENT_ANGLE*4*ENCODER_PPR/360-1)/POLE_PAIR_NUM
#define ICx_FILTER 8

//-----------Version Define------------//
#define VERSION_1 1 //M3固定,双板设计,选取Qgd较小的MOS管
#define VERSION_2 2 //ZQ_FOC DREAM_CHASER

#define HT4315 0
#define DJI_SNAIL_2305 1
#define HT2806 2

#define MA600 0

//---------Motor Parameter Define---------//
#if MOTOR_TYPE == HT4315
	#define MOTOR_POLE_PAIRS 14 //电机极对数
	#define MOTOR_VOLTAGE 24 //电机电压
	#define MOTOR_RESISTANCE 16.5 //电机电阻
	#define MOTOR_INDUCTANCE 0.0042 //电机电感
	#define MOTOR_SPEED_MAX 8.4 //电机最大速度(空载)
	#define MOTOR_CURRENT_MAX 0.92 //电机最大电流
	#define SPEED_CONSTANT 20 //转速常数Kn
	#define TORQUE_CONSTANT 0.45 //转矩常数KT
	
	#define MAX_Iq
#elif MOTOR_TYPE == DJI_SNAIL_2305
	#define MOTOR_POLE_PAIRS 7 //电机极对数
	#define MOTOR_VOLTAGE 14.8 //电机电压
	#define MOTOR_RESISTANCE 0.061 //电机电阻
	#define MOTOR_INDUCTANCE 0.000014 //电机电感
	#define MOTOR_SPEED_MAX  200//电机最大速度(空载)
	#define MOTOR_CURRENT_MAX 2 //电机最大电流
	#define SPEED_CONSTANT 2400 //转速常数Kn
	#define TORQUE_CONSTANT 0.0054267 //转矩常数KT
	#define	BACK_ELEC_FORCE_CONSTANT	0.056364//反电动势常数Ke
#elif MOTOR_TYPE == HT2806
	#define MOTOR_POLE_PAIRS 7 //电机极对数
	#define MOTOR_VOLTAGE 12 //电机电压
	#define MOTOR_RESISTANCE 5.1 //电机电阻
	#define MOTOR_INDUCTANCE 0.0023 //电机电感
	#define MOTOR_SPEED_MAX  20//电机最大速度(空载)
	#define MOTOR_CURRENT_MAX 0.8 //电机最大电流
	#define SPEED_CONSTANT 183 //转速常数Kn
	#define TORQUE_CONSTANT 0.06 //转矩常数KT
#endif

//---------Encoder Parameter Define---------//
#if ENCODER_TYPE == MA600 && MOTOR_TYPE == HT4315 && HARDWARE_VERSION == VERSION_1
	#define MOTOR_ENCODER_DIR 	-1 //电机编码器方向
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MA600
	#define THREE_PHASE_LINE_SEQUENCCE A_B_C
	#define MAX_IQ 2.0f
	#define MAX_VQ 12.0f
	#define MAX_VD 2.3f
	
#elif ENCODER_TYPE == MA600 && MOTOR_TYPE == HT4315 && HARDWARE_VERSION == VERSION_2
	#define MOTOR_ENCODER_DIR 	-1 //电机编码器方向
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MA600
	#define THREE_PHASE_LINE_SEQUENCCE A_C_B
	#define MAX_IQ 2.0f
	#define MAX_VQ 12.0f
	#define MAX_VD 10.0f
	
#elif ENCODER_TYPE == MA600 && MOTOR_TYPE == DJI_SNAIL_2305
	#define MOTOR_ENCODER_DIR 	1 //电机编码器方向
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MA600
	#define THREE_PHASE_LINE_SEQUENCCE A_B_C
	#define MAX_IQ 1.7f
	#define MAX_VQ 6.8f
	#define MAX_VD 2.3f
	
#elif ENCODER_TYPE == MA600 && MOTOR_TYPE == HT2806
	#define MOTOR_ENCODER_DIR 	-1 //电机编码器方向
	#define MOTOR_ENCODER_LINES 8192 //电机编码器线数_MA600
	#define THREE_PHASE_LINE_SEQUENCCE A_C_B
	#define MAX_IQ 1.7f
	#define MAX_VQ 6.8f
	#define MAX_VD 2.3f
#endif

#define A_B_C  1
#define A_C_B -1

#endif
