/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix 3113624526@qq.com
 * @LastEditTime: 2024-02-12 15:44:40
 */

#ifndef LIB_QUATERNION_H
#define LIB_QUATERNION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "alg_kf.h"

    typedef struct
    {
        float roll;
        float pitch;
        float yaw;

        float q[4];
        float accel[3];
        float gyro[3];
        float gyro_bias[3];
        float orientation[3];

        KF_DataTypeDef ekf;
        uint8_t init_flag;
        uint8_t converge_flag;
        uint8_t stable_flag;
        uint64_t error_count;
        uint64_t update_count;

        float Q1;
        float Q2;
        float R;

        float dt;
        float lambda;
        float accLPFcoef;
        float gyro_norm;
        float accl_norm;
        float gain_scale;

        int16_t yaw_round_count;
        float yaw_consequent;
        float yaw_last;

        mat ChiSquare;
        float chisquare_data[1];
        float chisquare_test_threshold;
    } Quaternion_DataTypeDef;

    void QEKF_Update(Quaternion_DataTypeDef *quaternion, float gx, float gy, float gz, float ax, float ay, float az, float dt);
    static void QEKF_F_Linearization_P_Fading(Quaternion_DataTypeDef *quaternion);
    static void QEKF_SetH(Quaternion_DataTypeDef *quaternion);
    static void QEKF_xhatUpdate(Quaternion_DataTypeDef *quaternion);

    void QEKF_Init(Quaternion_DataTypeDef *quaternion, float process_noise1, float process_noise2, float measure_noise, float lambda, float lpf);

#endif

#ifdef __cplusplus
}
#endif
