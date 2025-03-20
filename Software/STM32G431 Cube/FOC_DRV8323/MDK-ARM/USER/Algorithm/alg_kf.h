/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix 3113624526@qq.com
 * @LastEditTime: 2024-02-12 14:39:38
 */

#ifndef ALG_KF_H
#define ALG_KF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib_math.h"
#include "stdlib.h"

    typedef struct kf_t
    {
        float *FilteredValue;
        float *MeasureVector;
        float *ControlVector;

        uint8_t xhatSize;
        uint8_t uSize;
        uint8_t zSize;

        int8_t MatStatus;
        uint8_t SkipEq1, SkipEq2, SkipEq3, SkipEq4, SkipEq5;

        mat xhat;      // x(k|k)
        mat xhatminus; // x(k|k-1)
        mat u;         // control vector u
        mat z;         // measurement vector z
        mat P;         // covariance matrix P(k|k)
        mat Pminus;    // covariance matrix P(k|k-1)
        mat F, FT;     // state transition matrix F FT
        mat B;         // control matrix B
        mat H, HT;     // measurement matrix H
        mat Q;         // process noise covariance matrix Q
        mat R;         // measurement noise covariance matrix R
        mat K;         // kalman gain  K
        mat S, temp_matrix1, temp_matrix2, temp_vector1, temp_vector2;

        float *xhat_data, *xhatminus_data;
        float *u_data;
        float *z_data;
        float *P_data, *Pminus_data;
        float *F_data, *FT_data;
        float *B_data;
        float *H_data, *HT_data;
        float *Q_data;
        float *R_data;
        float *K_data;
        float *S_data;
        float *temp_matrix_data1, *temp_matrix_data2, *temp_vector_data1, *temp_vector_data2;
    } KF_DataTypeDef;

    void KF_Update(KF_DataTypeDef *kf);
    void KF_xhatminus_Update(KF_DataTypeDef *kf);
    void KF_Pminus_Update(KF_DataTypeDef *kf);
    void KF_K_Update(KF_DataTypeDef *kf);
    void KF_xhat_Update(KF_DataTypeDef *kf);
    void KF_P_Update(KF_DataTypeDef *kf);

    void KF_Init(KF_DataTypeDef *kf, uint8_t xhatSize, uint8_t uSize, uint8_t zSize);
    void KF_Watch(KF_DataTypeDef *kf);

#ifdef __cplusplus
}
#endif

#endif
