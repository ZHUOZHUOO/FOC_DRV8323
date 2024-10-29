#ifndef __CONFIGURE_H
#define __CONFIGURE_H

#define FOC_MODE OPEN_LOOP

#define OPEN_LOOP 0
#define CLOSE_LOOP 1

#define MOTOR_POLE_PAIRS 4 //电机极对数
#define MOTOR_RESISTANCE 0.1 //电机电阻
#define MOTOR_VOLTAGE 24 //电机电压
#define MOTOR_INDUCTANCE 0.0001 //电机电感
#define MOTOR_FLUX_LINKAGE 0.001 //电机磁链
#define MOTOR_SPEED_MAX 1000 //电机最大速度
#define MOTOR_SPEED_MIN -1000 //电机最小速度
#define MOTOR_CURRENT_MAX 10 //电机最大电流
#define MOTOR_ENCODER_LINES 8192 //电机编码器线数
#define MOTOR_ENCODER_DIR 1 //电机编码器方向

#endif