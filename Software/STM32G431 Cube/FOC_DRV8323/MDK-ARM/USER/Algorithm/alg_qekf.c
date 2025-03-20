/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-26 07:55:20
 */

#include "alg_qekf.h"

float QEKF_F[36] = {1, 0, 0, 0, 0, 0,
                             0, 1, 0, 0, 0, 0,
                             0, 0, 1, 0, 0, 0,
                             0, 0, 0, 1, 0, 0,
                             0, 0, 0, 0, 1, 0,
                             0, 0, 0, 0, 0, 1};

float QEKF_P[36] = {100000, 0.1, 0.1, 0.1, 0.1, 0.1,
                             0.1, 100000, 0.1, 0.1, 0.1, 0.1,
                             0.1, 0.1, 100000, 0.1, 0.1, 0.1,
                             0.1, 0.1, 0.1, 100000, 0.1, 0.1,
                             0.1, 0.1, 0.1, 0.1, 100, 0.1,
                             0.1, 0.1, 0.1, 0.1, 0.1, 100};

/**
 * @brief           Quaternion EKF update
 * @param[in]       gyro x y z in rad/s
 * @param[in]       accel x y z in m/s²
 * @param[in]       update period in s
 */
void QEKF_Update(Quaternion_DataTypeDef *quaternion, float gx, float gy, float gz, float ax, float ay, float az, float dt)
{
    // 0.5(Ohm-Ohm^bias)*deltaT
    static float halfgxdt, halfgydt, halfgzdt;
    if (!quaternion->init_flag)
    {
        QEKF_Init(quaternion, 10, 0.01f, 10000000, 1, 0.0085f);
    }

    /*   F, number with * represent vals to be set
     0      1*     2*     3*     4     5
     6*     7      8*     9*    10    11
    12*    13*    14     15*    16    17
    18*    19*    20*    21     22    23
    24     25     26     27     28    29
    30     31     32     33     34    35
    */
    quaternion->dt = dt;

    quaternion->gyro[0] = gx - quaternion->gyro_bias[0];
    quaternion->gyro[1] = gy - quaternion->gyro_bias[1];
    quaternion->gyro[2] = gz - quaternion->gyro_bias[2];

    // set F
    halfgxdt = 0.5f * quaternion->gyro[0] * dt;
    halfgydt = 0.5f * quaternion->gyro[1] * dt;
    halfgzdt = 0.5f * quaternion->gyro[2] * dt;

    memcpy(quaternion->ekf.F_data, QEKF_F, sizeof(QEKF_F));

    quaternion->ekf.F_data[1] = -halfgxdt;
    quaternion->ekf.F_data[2] = -halfgydt;
    quaternion->ekf.F_data[3] = -halfgzdt;

    quaternion->ekf.F_data[6] = halfgxdt;
    quaternion->ekf.F_data[8] = halfgzdt;
    quaternion->ekf.F_data[9] = -halfgydt;

    quaternion->ekf.F_data[12] = halfgydt;
    quaternion->ekf.F_data[13] = -halfgzdt;
    quaternion->ekf.F_data[15] = halfgxdt;

    quaternion->ekf.F_data[18] = halfgzdt;
    quaternion->ekf.F_data[19] = halfgydt;
    quaternion->ekf.F_data[20] = -halfgxdt;
    if (quaternion->update_count == 0)
    {
        quaternion->accel[0] = ax;
        quaternion->accel[1] = ay;
        quaternion->accel[2] = az;
    }
    quaternion->accel[0] = quaternion->accel[0] * quaternion->accLPFcoef / (quaternion->dt + quaternion->accLPFcoef) + ax * quaternion->dt / (quaternion->dt + quaternion->accLPFcoef);
    quaternion->accel[1] = quaternion->accel[1] * quaternion->accLPFcoef / (quaternion->dt + quaternion->accLPFcoef) + ay * quaternion->dt / (quaternion->dt + quaternion->accLPFcoef);
    quaternion->accel[2] = quaternion->accel[2] * quaternion->accLPFcoef / (quaternion->dt + quaternion->accLPFcoef) + az * quaternion->dt / (quaternion->dt + quaternion->accLPFcoef);

    quaternion->accl_norm = 1.0f / Math_InvSqrt(quaternion->accel[0] * quaternion->accel[0] +
                                                quaternion->accel[1] * quaternion->accel[1] +
                                                quaternion->accel[2] * quaternion->accel[2]);
    for (uint8_t i = 0; i < 3; i++)
    {
        quaternion->ekf.MeasureVector[i] = quaternion->accel[i] / quaternion->accl_norm;
    }

    // get body state
    quaternion->gyro_norm = 1.0f / Math_InvSqrt(quaternion->gyro[0] * quaternion->gyro[0] +
                                                quaternion->gyro[1] * quaternion->gyro[1] +
                                                quaternion->gyro[2] * quaternion->gyro[2]);

    if (quaternion->gyro_norm < 0.3f && quaternion->accl_norm > 9.8f - 0.5f && quaternion->accl_norm < 9.8f + 0.5f)
    {
        quaternion->stable_flag = 1;
    }
    else
    {
        quaternion->stable_flag = 0;
    }

    // set Q R
    quaternion->ekf.Q_data[0] = quaternion->Q1 * quaternion->dt;
    quaternion->ekf.Q_data[7] = quaternion->Q1 * quaternion->dt;
    quaternion->ekf.Q_data[14] = quaternion->Q1 * quaternion->dt;
    quaternion->ekf.Q_data[21] = quaternion->Q1 * quaternion->dt;
    quaternion->ekf.Q_data[28] = quaternion->Q2 * quaternion->dt;
    quaternion->ekf.Q_data[35] = quaternion->Q2 * quaternion->dt;
    quaternion->ekf.R_data[0] = quaternion->R;
    quaternion->ekf.R_data[4] = quaternion->R;
    quaternion->ekf.R_data[8] = quaternion->R;

    KF_Watch(&quaternion->ekf);

    KF_xhatminus_Update(&quaternion->ekf);
    QEKF_F_Linearization_P_Fading(quaternion);

    KF_Pminus_Update(&quaternion->ekf);
    QEKF_SetH(quaternion);

    KF_K_Update(&quaternion->ekf);
    QEKF_xhatUpdate(quaternion);

    KF_xhat_Update(&quaternion->ekf);
    
    KF_P_Update(&quaternion->ekf);
    
    memcpy(quaternion->ekf.FilteredValue, quaternion->ekf.xhat_data, sizeof(float) * quaternion->ekf.xhatSize);

    quaternion->q[0] = quaternion->ekf.FilteredValue[0];
    quaternion->q[1] = quaternion->ekf.FilteredValue[1];
    quaternion->q[2] = quaternion->ekf.FilteredValue[2];
    quaternion->q[3] = quaternion->ekf.FilteredValue[3];
    quaternion->gyro_bias[0] = quaternion->ekf.FilteredValue[4];
    quaternion->gyro_bias[1] = quaternion->ekf.FilteredValue[5];
    quaternion->gyro_bias[2] = 0;

    quaternion->yaw = atan2f(2.0f * (quaternion->q[0] * quaternion->q[3] + quaternion->q[1] * quaternion->q[2]), 2.0f * (quaternion->q[0] * quaternion->q[0] + quaternion->q[1] * quaternion->q[1]) - 1.0f) * 57.295779513f;
    quaternion->pitch = atan2f(2.0f * (quaternion->q[0] * quaternion->q[1] + quaternion->q[2] * quaternion->q[3]), 2.0f * (quaternion->q[0] * quaternion->q[0] + quaternion->q[3] * quaternion->q[3]) - 1.0f) * 57.295779513f;
    quaternion->roll = asinf(-2.0f * (quaternion->q[1] * quaternion->q[3] - quaternion->q[0] * quaternion->q[2])) * 57.295779513f;

    if (quaternion->yaw - quaternion->yaw_last > 180.0f)
    {
        quaternion->yaw_round_count--;
    }
    else if (quaternion->yaw - quaternion->yaw_last < -180.0f)
    {
        quaternion->yaw_round_count++;
    }
    quaternion->yaw_consequent = 360.0f * quaternion->yaw_round_count + quaternion->yaw;
    quaternion->yaw_last = quaternion->yaw;
    quaternion->update_count++;
}

/**
 * @brief It is used to update a 4x2 block matrix in the upper right corner of
 *        the linearized state transition matrix F. Later, it is used to update
 *        the covariance matrix P and limit the variance of the zero drift to prevent
 *        over-convergence and limit the amplitude to prevent divergence
 *
 * @param kf
 */
static void QEKF_F_Linearization_P_Fading(Quaternion_DataTypeDef *quaternion)
{
    static float q0, q1, q2, q3;
    static float qInvNorm;

    q0 = quaternion->ekf.xhatminus_data[0];
    q1 = quaternion->ekf.xhatminus_data[1];
    q2 = quaternion->ekf.xhatminus_data[2];
    q3 = quaternion->ekf.xhatminus_data[3];

    // quaternion normalize
    qInvNorm = Math_InvSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    for (uint8_t i = 0; i < 4; i++)
    {
        quaternion->ekf.xhatminus_data[i] *= qInvNorm;
    }
    /*  F, number with * represent vals to be set
     0     1     2     3     4*     5*
     6     7     8     9    10*    11*
    12    13    14    15    16*    17*
    18    19    20    21    22*    23*
    24    25    26    27    28     29
    30    31    32    33    34     35
    */
    // set F
    quaternion->ekf.F_data[4] = q1 * quaternion->dt / 2;
    quaternion->ekf.F_data[5] = q2 * quaternion->dt / 2;

    quaternion->ekf.F_data[10] = -q0 * quaternion->dt / 2;
    quaternion->ekf.F_data[11] = q3 * quaternion->dt / 2;

    quaternion->ekf.F_data[16] = -q3 * quaternion->dt / 2;
    quaternion->ekf.F_data[17] = -q0 * quaternion->dt / 2;

    quaternion->ekf.F_data[22] = q2 * quaternion->dt / 2;
    quaternion->ekf.F_data[23] = -q1 * quaternion->dt / 2;

    // fading filter
    quaternion->ekf.P_data[28] /= quaternion->lambda;
    quaternion->ekf.P_data[35] /= quaternion->lambda;

    LimitMax(quaternion->ekf.P_data[28], 10000);
    LimitMax(quaternion->ekf.P_data[35], 10000);
}

/**
 * @brief Calculate the Jacobi matrix H of the observation function h (x) at the working point
 *
 * @param kf
 */
static void QEKF_SetH(Quaternion_DataTypeDef *quaternion)
{
    static float doubleq0, doubleq1, doubleq2, doubleq3;
    uint8_t sizeof_float = sizeof(float);
    /* H
     0     1     2     3     4     5
     6     7     8     9    10    11
    12    13    14    15    16    17
    last two cols are zero
    */
    // set H
    doubleq0 = 2 * quaternion->ekf.xhatminus_data[0];
    doubleq1 = 2 * quaternion->ekf.xhatminus_data[1];
    doubleq2 = 2 * quaternion->ekf.xhatminus_data[2];
    doubleq3 = 2 * quaternion->ekf.xhatminus_data[3];

    memset(quaternion->ekf.H_data, 0, sizeof_float * quaternion->ekf.zSize * quaternion->ekf.xhatSize);

    quaternion->ekf.H_data[0] = -doubleq2;
    quaternion->ekf.H_data[1] = doubleq3;
    quaternion->ekf.H_data[2] = -doubleq0;
    quaternion->ekf.H_data[3] = doubleq1;

    quaternion->ekf.H_data[6] = doubleq1;
    quaternion->ekf.H_data[7] = doubleq0;
    quaternion->ekf.H_data[8] = doubleq3;
    quaternion->ekf.H_data[9] = doubleq2;

    quaternion->ekf.H_data[12] = doubleq0;
    quaternion->ekf.H_data[13] = -doubleq1;
    quaternion->ekf.H_data[14] = -doubleq2;
    quaternion->ekf.H_data[15] = doubleq3;
}

/**
 * @brief Using observations and prior estimates to obtain the optimal posterior estimate
 *        Chi-square test is added to judge whether the conditions of fusion acceleration are met
 *        At the same time, the divergence protection is introduced to ensure the necessary
 *        measurement update under bad conditions
 *
 * @param kf
 */
static void QEKF_xhatUpdate(Quaternion_DataTypeDef *quaternion)
{
    static float q0, q1, q2, q3;
    uint8_t sizeof_float = sizeof(float);

    quaternion->ekf.MatStatus = Matrix_Transpose(&quaternion->ekf.H, &quaternion->ekf.HT); // z|x => x|z
    quaternion->ekf.temp_matrix1.numRows = quaternion->ekf.H.numRows;
    quaternion->ekf.temp_matrix1.numCols = quaternion->ekf.Pminus.numCols;
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.H, &quaternion->ekf.Pminus, &quaternion->ekf.temp_matrix1); // temp_matrix1 = H·P'(k)
    quaternion->ekf.temp_matrix2.numRows = quaternion->ekf.temp_matrix1.numRows;
    quaternion->ekf.temp_matrix2.numCols = quaternion->ekf.HT.numCols;
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.temp_matrix1, &quaternion->ekf.HT, &quaternion->ekf.temp_matrix2); // temp_matrix2 = H·P'(k)·HT
    quaternion->ekf.S.numRows = quaternion->ekf.R.numRows;
    quaternion->ekf.S.numCols = quaternion->ekf.R.numCols;
    quaternion->ekf.MatStatus = Matrix_Add(&quaternion->ekf.temp_matrix2, &quaternion->ekf.R, &quaternion->ekf.S); // S = H P'(k) HT + R
    quaternion->ekf.MatStatus = Matrix_Inverse(&quaternion->ekf.S, &quaternion->ekf.temp_matrix2);                 // temp_matrix2 = inv(H·P'(k)·HT + R)

    q0 = quaternion->ekf.xhatminus_data[0];
    q1 = quaternion->ekf.xhatminus_data[1];
    q2 = quaternion->ekf.xhatminus_data[2];
    q3 = quaternion->ekf.xhatminus_data[3];

    quaternion->ekf.temp_vector1.numRows = quaternion->ekf.H.numRows;
    quaternion->ekf.temp_vector1.numCols = 1;

    quaternion->ekf.temp_vector_data1[0] = 2 * (q1 * q3 - q0 * q2);
    quaternion->ekf.temp_vector_data1[1] = 2 * (q0 * q1 + q2 * q3);
    quaternion->ekf.temp_vector_data1[2] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3; // temp_vector1 = h(xhat'(k))

    for (uint8_t i = 0; i < 3; i++)
    {
        quaternion->orientation[i] = acosf(fabsf(quaternion->ekf.temp_vector_data1[i]));
    }

    quaternion->ekf.temp_vector2.numRows = quaternion->ekf.z.numRows;
    quaternion->ekf.temp_vector2.numCols = 1;
    quaternion->ekf.MatStatus = Matrix_Subtract(&quaternion->ekf.z, &quaternion->ekf.temp_vector1, &quaternion->ekf.temp_vector2); // temp_vector2 = z(k) - h(xhat'(k))

    // chi-square test
    quaternion->ekf.temp_matrix1.numRows = quaternion->ekf.temp_vector2.numRows;
    quaternion->ekf.temp_matrix1.numCols = 1;
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.temp_matrix2, &quaternion->ekf.temp_vector2, &quaternion->ekf.temp_matrix1); // temp_matrix1 = inv(H·P'(k)·HT + R)·(z(k) - h(xhat'(k)))
    quaternion->ekf.temp_vector1.numRows = 1;
    quaternion->ekf.temp_vector1.numCols = quaternion->ekf.temp_vector2.numRows;
    quaternion->ekf.MatStatus = Matrix_Transpose(&quaternion->ekf.temp_vector2, &quaternion->ekf.temp_vector1); // temp_vector = z(k) - h(xhat'(k))'
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.temp_vector1, &quaternion->ekf.temp_matrix1, &quaternion->ChiSquare);
    // rk is small,filter converged/converging
    if (quaternion->chisquare_data[0] < 0.5f * quaternion->chisquare_test_threshold)
    {
        quaternion->converge_flag = 1;
    }
    // rk is bigger than thre but once converged
    if (quaternion->chisquare_data[0] > quaternion->chisquare_test_threshold && quaternion->converge_flag)
    {
        if (quaternion->stable_flag)
        {
            quaternion->error_count++;
        }
        else
        {
            quaternion->error_count = 0;
        }

        if (quaternion->error_count > 50)
        {
            quaternion->converge_flag = 0;
            quaternion->ekf.SkipEq5 = 0; // step-5 is cov mat P updating
        }
        else
        {
            //  xhat(k) = xhat'(k)
            //  P(k) = P'(k)
            memcpy(quaternion->ekf.xhat_data, quaternion->ekf.xhatminus_data, sizeof_float * quaternion->ekf.xhatSize);
            memcpy(quaternion->ekf.P_data, quaternion->ekf.Pminus_data, sizeof_float * quaternion->ekf.xhatSize * quaternion->ekf.xhatSize);
            quaternion->ekf.SkipEq5 = 1; // part5 is P updating
            return;
        }
    }
    else
    { // if divergent or rk is not that big/acceptable,use adaptive gain
        // scale adaptive
        if (quaternion->chisquare_data[0] > 0.1f * quaternion->chisquare_test_threshold && quaternion->converge_flag)
        {
            quaternion->gain_scale = (quaternion->chisquare_test_threshold - quaternion->chisquare_data[0]) / (0.9f * quaternion->chisquare_test_threshold);
        }
        else
        {
            quaternion->gain_scale = 1;
        }
        quaternion->error_count = 0;
        quaternion->ekf.SkipEq5 = 0;
    }

    // cal kf-gain K
    quaternion->ekf.temp_matrix1.numRows = quaternion->ekf.Pminus.numRows;
    quaternion->ekf.temp_matrix1.numCols = quaternion->ekf.HT.numCols;
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.Pminus, &quaternion->ekf.HT, &quaternion->ekf.temp_matrix1); // temp_matrix1 = P'(k)·HT
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.temp_matrix1, &quaternion->ekf.temp_matrix2, &quaternion->ekf.K);

    // implement adaptive
    for (uint8_t i = 0; i < quaternion->ekf.K.numRows * quaternion->ekf.K.numCols; i++)
    {
        quaternion->ekf.K_data[i] *= quaternion->gain_scale;
    }
    for (uint8_t i = 4; i < 6; i++)
    {
        for (uint8_t j = 0; j < 3; j++)
        {
            quaternion->ekf.K_data[i * 3 + j] *= quaternion->orientation[i - 4] / 1.5707963f; // 1 rad
        }
    }

    quaternion->ekf.temp_vector1.numRows = quaternion->ekf.K.numRows;
    quaternion->ekf.temp_vector1.numCols = 1;
    quaternion->ekf.MatStatus = Matrix_Multiply(&quaternion->ekf.K, &quaternion->ekf.temp_vector2, &quaternion->ekf.temp_vector1); // temp_vector1 = K(k)·(z(k) - H·xhat'(k))

    if (quaternion->converge_flag)
    {
        for (uint8_t i = 4; i < 6; i++)
        {
            LimitMaxMin(quaternion->ekf.temp_vector1.pData[i], 1e-2f * quaternion->dt, -1e-2f * quaternion->dt);
        }
    }

    quaternion->ekf.temp_vector1.pData[3] = 0;
    quaternion->ekf.MatStatus = Matrix_Add(&quaternion->ekf.xhatminus, &quaternion->ekf.temp_vector1, &quaternion->ekf.xhat);
}

/**
 * @brief Quaternion EKF initialization and some reference value
 * @param[in] process_noise1 quaternion process noise    10
 * @param[in] process_noise2 gyro bias process noise     0.001
 * @param[in] measure_noise  accel measure noise         1000000
 * @param[in] lambda         fading coefficient          0.9996
 * @param[in] lpf            lowpass filter coefficient  0
 */
void QEKF_Init(Quaternion_DataTypeDef *quaternion, float process_noise1, float process_noise2, float measure_noise, float lambda, float lpf)
{
    quaternion->init_flag = 1;
    quaternion->Q1 = process_noise1;
    quaternion->Q2 = process_noise2;
    quaternion->R = measure_noise;
    quaternion->chisquare_test_threshold = 1e-8;
    quaternion->converge_flag = 0;
    quaternion->error_count = 0;
    quaternion->update_count = 0;
    LimitMax(lambda, 1);
    quaternion->lambda = lambda;
    quaternion->accLPFcoef = lpf;

    KF_Init(&quaternion->ekf, 6, 0, 3);
    Matrix_Init(&quaternion->ChiSquare, 1, 1, (float *)quaternion->chisquare_data);

    quaternion->ekf.xhat_data[0] = 1;
    quaternion->ekf.xhat_data[1] = 0;
    quaternion->ekf.xhat_data[2] = 0;
    quaternion->ekf.xhat_data[3] = 0;

    quaternion->ekf.SkipEq3 = 1;
    quaternion->ekf.SkipEq4 = 1;

    memcpy(quaternion->ekf.F_data, QEKF_F, sizeof(QEKF_F));
    memcpy(quaternion->ekf.P_data, QEKF_P, sizeof(QEKF_P));
}
