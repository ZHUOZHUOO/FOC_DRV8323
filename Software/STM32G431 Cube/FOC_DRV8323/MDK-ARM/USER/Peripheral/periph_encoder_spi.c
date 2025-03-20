/**
 *
 * @file periph_encoder_spi.c
 * @author Foundary
 * @date 2025-03-09    17:21:10
 *
 */

#include "periph_encoder_spi.h"
#include "string.h"

void Encoder_SPI_Init(Encoder_SPI_HandleTypeDef *encoder,
                      SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
                      uint16_t cs_pin, float radius) {
  encoder->hspi = hspi;
  encoder->cs_port = cs_port;
  encoder->cs_pin = cs_pin;
  encoder->radius = radius;
  encoder->multi_turn = 0;
  encoder->last_multi_turn = 0;
  encoder->last_angle = 0;
  encoder->angle_diff = 0;
  encoder->turns = 0;
  encoder->angular_speed = 0;
  encoder->linear_speed = 0;
  encoder->last_update_time = HAL_GetTick();

  memset(encoder->rx_buffer, 0, 4);

  uint8_t tx_buffer[4] = {0,0,0,0};
  //  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  //  HAL_Delay(1);
  //  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0xea;
  tx_buffer[1] = 0x54;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x1c;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  tx_buffer[2] = 0x00;
  tx_buffer[3] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 4, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0xea;
  tx_buffer[1] = 0x54;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x0D;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  tx_buffer[0] = 0x00;
  tx_buffer[1] = 0x00;
  tx_buffer[2] = 0x00;
  tx_buffer[3] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 4, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  HAL_Delay(1);

  // tx_buffer[0] = 0b11010010;
  // tx_buffer[1] = 0x1c;
  // HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  // HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2,
  // 100); HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  // HAL_Delay(1);

  // tx_buffer[0] = 0b11010010;
  // tx_buffer[1] = 0x1c;
  // HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  // HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2,
  // 100); HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  // HAL_Delay(1);

  // tx_buffer[0] = 0x00;
  // tx_buffer[1] = 0x00;
  // HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  // HAL_SPI_TransmitReceive(encoder->hspi, tx_buffer, encoder->rx_buffer, 2,
  // 100); HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
  // HAL_Delay(1);
}

void Encoder_SPI_Data_Process(Encoder_SPI_HandleTypeDef *encoder,
                              uint8_t *buffer) {
  uint64_t now_time = DWT_GetTimeline_us();
  // 读取编码器角度值（0-65535范围）
  int16_t encoderRawData = (int16_t)(buffer[0] << 8 | buffer[1]);
  // 将编码器值转换为-180°~180°的角度
  float rawAngle = (float)(encoderRawData) * 360.0f / 65536.0f - 180.0f;
  // 读取多圈信息
  int16_t turns = (int16_t)((buffer[2] << 8) | buffer[3]);
  // 计算角度的差值，考虑过零点情况
  float angle_diff = rawAngle - encoder->last_angle;
  if (angle_diff > 180.0f) {
    // 说明角度从负值跳到正值（即过零点）
    angle_diff -= 360.0f;
  } else if (angle_diff < -180.0f) {
    // 说明角度从正值跳到负值（即过零点）
    angle_diff += 360.0f;
  }

  encoder->angle_diff = angle_diff;

  // 根据角度差值判断圈数变化
  if (angle_diff > 0 && encoder->last_angle < -90 && rawAngle > 90) {
    // 过零点（从负数跨到正数），圈数加一
    encoder->multi_turn++;
  } else if (angle_diff < 0 && encoder->last_angle > 90 && rawAngle < -90) {
    // 过零点（从正数跨到负数），圈数减一
    encoder->multi_turn--;
  }

  // 将角度差转换为角速度 (rad/s)
  float sampling_period =
      (float)(now_time - encoder->last_update_time) / 1000000.0f; // us->s
  encoder->last_update_time = now_time;
  encoder->angular_speed =
      (angle_diff / 180.0f * 3.1415926f) / sampling_period; // rad/s

  // 根据角速度计算线速度 (m/s)
  encoder->linear_speed = encoder->angular_speed * encoder->radius;

  // 保存上次的角度和多圈数
  encoder->last_angle = rawAngle;
  encoder->last_multi_turn = encoder->multi_turn;
}

void Encoder_SPI_Reset(Encoder_SPI_HandleTypeDef *encoder) {
  encoder->multi_turn = 0;
  encoder->last_multi_turn = 0;
  encoder->last_angle = 0;
  encoder->angle_diff = 0;
  encoder->turns = 0;
  encoder->angular_speed = 0;
  encoder->linear_speed = 0;
}

/**
 * @brief get the angle 角度
 *
 * @param encoder
 * @return float
 */
float Encoder_SPI_Get_Angle(Encoder_SPI_HandleTypeDef *encoder) {
  return encoder->last_angle + encoder->multi_turn * 360.0f;
}

/**
 * @brief get the angular speed 角速度
 *
 * @param encoder
 * @return float
 */
float Encoder_SPI_Get_Angular_Speed(Encoder_SPI_HandleTypeDef *encoder) {
  return encoder->angular_speed;
}

void Encoder_Read_Reg(Encoder_SPI_HandleTypeDef *encoder) {
  uint8_t txbuffer[4];
  txbuffer[0] = 0x00;
  txbuffer[1] = 0x00;
  txbuffer[2] = 0x00;
  txbuffer[3] = 0x00;
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(encoder->hspi, txbuffer, encoder->rx_buffer, 4, 100);
  HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);

  Encoder_SPI_Data_Process(encoder, encoder->rx_buffer);
}
