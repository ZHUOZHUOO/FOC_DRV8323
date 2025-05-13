/**
 * @file periph_encoder_spi.h
 * @author Star_Plucking
 * @brief SPI编码器
 * @version 0.1
 * @date 2025-03-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __PERIPH_ENCODER_SPI_H
#define __PERIPH_ENCODER_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "sys_dwt.h"
#include "util_gpio.h"
#include "util_spi.h"
#include "Foc_Control.h"
#include "alg_swf.h"

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *cs_port;
  uint16_t cs_pin;

  uint32_t last_update_time; // 上次更新时间
  int16_t multi_turn;        // 当前多圈数
  int16_t last_multi_turn;   // 上一次的多圈数
  float rawAngle;
  float last_rawAngle;
  float rawAngle_diff;

  float last_angle;        	 // 上一次的角度
  SlidingWindowFilter *angle_filter;
  float *angle_buffer;

  float angle_diff;          // 角度差值
	SlidingWindowFilter *angle_diff_Filter;
	float *angle_diff_buffer;

  int32_t turns;             // 累计圈数
  float angular_speed;       // 角速度 (rad/s)
  float linear_speed;        // 线速度 (m/s)
  float radius;              // 半径 (m)
  uint8_t rx_buffer[4];

} Encoder_SPI_HandleTypeDef;

extern Encoder_SPI_HandleTypeDef MA600_spi;
extern SlidingWindowFilter MA600_diff_Filter;
extern float MA600_diff_buffer[DIFF_SLIDING_WINDOW_SIZE];
extern SlidingWindowFilter MA600_angle_Filter;
extern float MA600_angle_buffer[ANGLE_SLIDING_WINDOW_SIZE];

void Encoder_SPI_Init(Encoder_SPI_HandleTypeDef *encoder,
											SlidingWindowFilter *diff_filter,float *diff_buffer,
                      SlidingWindowFilter *angle_filter,float *angle_buffer,
                      SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
                      uint16_t cs_pin, float radius);

void Encoder_SPI_Data_Process(Encoder_SPI_HandleTypeDef *encoder,
                              uint8_t *buffer);

void Encoder_SPI_Reset(Encoder_SPI_HandleTypeDef *encoder);

float Encoder_SPI_Get_Angle(Encoder_SPI_HandleTypeDef *encoder);

float Encoder_SPI_Get_Angular_Speed(Encoder_SPI_HandleTypeDef *encoder);

void Encoder_Read_Reg(Encoder_SPI_HandleTypeDef *encoder);

#ifdef __cplusplus
}
#endif

#endif
