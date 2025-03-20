/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-26 07:53:04
 */

#include "alg_kf.h"

/**
 * @brief      Update data with Kalman filter
 * @param      kf :Structure pointer of Kalman filter
 * @retval     Filtered data pointer
 */
void KF_Update(KF_DataTypeDef *kf)
{
    KF_Watch(kf);

    // 1. xhat'(k)= A·xhat(k-1) + B·u
    KF_xhatminus_Update(kf);

    // 2. P'(k) = A·P(k-1)·AT + Q
    KF_Pminus_Update(kf);

    // 3. K(k) = P'(k)·HT / (H·P'(k)·HT + R)
    KF_K_Update(kf);

    // 4. xhat(k) = xhat'(k) + K(k)·(z(k) - H·xhat'(k))
    KF_xhat_Update(kf);

    // 5. P(k) = (1-K(k)·H)·P'(k) ==> P(k) = P'(k)-K(k)·H·P'(k)
    KF_P_Update(kf);

    memcpy(kf->FilteredValue, kf->xhat_data, sizeof(float) * kf->xhatSize);
}

/**
 * @brief         Kalman Filter xhat Minus Update
 * @param *kf
 * @retval
 */
void KF_xhatminus_Update(KF_DataTypeDef *kf)
{
    if (!kf->SkipEq1)
    {
        if (kf->uSize > 0)
        {
            kf->temp_vector1.numRows = kf->xhatSize;
            kf->temp_vector1.numCols = 1;
            kf->MatStatus = Matrix_Multiply(&kf->F, &kf->xhat, &kf->temp_vector1);
            kf->temp_vector2.numRows = kf->xhatSize;
            kf->temp_vector2.numCols = 1;
            kf->MatStatus = Matrix_Multiply(&kf->B, &kf->u, &kf->temp_vector2);
            kf->MatStatus = Matrix_Add(&kf->temp_vector1, &kf->temp_vector2, &kf->xhatminus);
        }
        else
        {
            kf->MatStatus = Matrix_Multiply(&kf->F, &kf->xhat, &kf->xhatminus);
        }
    }
}

/**
 * @brief         Kalman Filter Pminus Update
 * @param *kf
 * @retval
 */
void KF_Pminus_Update(KF_DataTypeDef *kf)
{
    if (!kf->SkipEq2)
    {
        kf->MatStatus = Matrix_Transpose(&kf->F, &kf->FT);
        kf->MatStatus = Matrix_Multiply(&kf->F, &kf->P, &kf->Pminus);
        kf->temp_matrix1.numRows = kf->Pminus.numRows;
        kf->temp_matrix1.numCols = kf->FT.numCols;
        kf->MatStatus = Matrix_Multiply(&kf->Pminus, &kf->FT, &kf->temp_matrix1); // temp_matrix1 = F P(k-1) FT
        kf->MatStatus = Matrix_Add(&kf->temp_matrix1, &kf->Q, &kf->Pminus);
    }
}

/**
 * @brief         Kalman Filter Set K
 * @param *kf
 * @retval
 */
void KF_K_Update(KF_DataTypeDef *kf)
{
    if (!kf->SkipEq3)
    {
        kf->MatStatus = Matrix_Transpose(&kf->H, &kf->HT); // z|x => x|z
        kf->temp_matrix1.numRows = kf->H.numRows;
        kf->temp_matrix1.numCols = kf->Pminus.numCols;
        kf->MatStatus = Matrix_Multiply(&kf->H, &kf->Pminus, &kf->temp_matrix1); // temp_matrix1 = H·P'(k)
        kf->temp_matrix2.numRows = kf->temp_matrix1.numRows;
        kf->temp_matrix2.numCols = kf->HT.numCols;
        kf->MatStatus = Matrix_Multiply(&kf->temp_matrix1, &kf->HT, &kf->temp_matrix2); // temp_matrix2 = H·P'(k)·HT
        kf->S.numRows = kf->R.numRows;
        kf->S.numCols = kf->R.numCols;
        kf->MatStatus = Matrix_Add(&kf->temp_matrix2, &kf->R, &kf->S); // S = H P'(k) HT + R
        kf->MatStatus = Matrix_Inverse(&kf->S, &kf->temp_matrix2);     // temp_matrix2 = inv(H·P'(k)·HT + R)
        kf->temp_matrix1.numRows = kf->Pminus.numRows;
        kf->temp_matrix1.numCols = kf->HT.numCols;
        kf->MatStatus = Matrix_Multiply(&kf->Pminus, &kf->HT, &kf->temp_matrix1); // temp_matrix1 = P'(k)·HT
        kf->MatStatus = Matrix_Multiply(&kf->temp_matrix1, &kf->temp_matrix2, &kf->K);
    }
}

/**
 * @brief         Kalman Filter xhat Update
 * @param *kf
 * @retval
 */
void KF_xhat_Update(KF_DataTypeDef *kf)
{
    if (!kf->SkipEq4)
    {
        kf->temp_vector1.numRows = kf->H.numRows;
        kf->temp_vector1.numCols = 1;
        kf->MatStatus = Matrix_Multiply(&kf->H, &kf->xhatminus, &kf->temp_vector1); // temp_vector1 = H xhat'(k)
        kf->temp_vector2.numRows = kf->z.numRows;
        kf->temp_vector2.numCols = 1;
        kf->MatStatus = Matrix_Subtract(&kf->z, &kf->temp_vector1, &kf->temp_vector2); // temp_vector2 = z(k) - H·xhat'(k)
        kf->temp_vector1.numRows = kf->K.numRows;
        kf->temp_vector1.numCols = 1;
        kf->MatStatus = Matrix_Multiply(&kf->K, &kf->temp_vector2, &kf->temp_vector1); // temp_vector1 = K(k)·(z(k) - H·xhat'(k))
        kf->MatStatus = Matrix_Add(&kf->xhatminus, &kf->temp_vector1, &kf->xhat);
    }
}

/**
 * @brief         Kalman Filter P Update
 * @param *kf
 * @retval
 */
void KF_P_Update(KF_DataTypeDef *kf)
{
    if (!kf->SkipEq5)
    {
        kf->temp_matrix1.numRows = kf->K.numRows;
        kf->temp_matrix1.numCols = kf->H.numCols;
        kf->temp_matrix2.numRows = kf->temp_matrix1.numRows;
        kf->temp_matrix2.numCols = kf->Pminus.numCols;
        kf->MatStatus = Matrix_Multiply(&kf->K, &kf->H, &kf->temp_matrix1);                 // temp_matrix1 = K(k)·H
        kf->MatStatus = Matrix_Multiply(&kf->temp_matrix1, &kf->Pminus, &kf->temp_matrix2); // temp_matrix2 = K(k)·H·P'(k)
        kf->MatStatus = Matrix_Subtract(&kf->Pminus, &kf->temp_matrix2, &kf->P);
    }
}

/**
 * @brief      Initialization of Kalman filter
 * @param      kf :Structure pointer of Kalman filter
 * @param      xhatSize :State variable matrix size
 * @param      uSize :Control matrix size
 * @param      zSize :Observation matrix size
 * @retval     NULL
 */
void KF_Init(KF_DataTypeDef *kf, uint8_t xhatSize, uint8_t uSize, uint8_t zSize)
{
    kf->xhatSize = xhatSize;
    kf->uSize = uSize;
    kf->zSize = zSize;

    // filter data
    kf->FilteredValue = (float *)malloc(sizeof(float) * xhatSize);
    memset(kf->FilteredValue, 0, sizeof(float) * xhatSize);
    kf->MeasureVector = (float *)malloc(sizeof(float) * zSize);
    memset(kf->MeasureVector, 0, sizeof(float) * zSize);
    kf->ControlVector = (float *)malloc(sizeof(float) * uSize);
    memset(kf->ControlVector, 0, sizeof(float) * uSize);

    // xhat x(k|k)
    kf->xhat_data = (float *)malloc(sizeof(float) * xhatSize);
    memset(kf->xhat_data, 0, sizeof(float) * xhatSize);
    Matrix_Init(&kf->xhat, kf->xhatSize, 1, (float *)kf->xhat_data);

    // xhatminus x(k|k-1)
    kf->xhatminus_data = (float *)malloc(sizeof(float) * xhatSize);
    memset(kf->xhatminus_data, 0, sizeof(float) * xhatSize);
    Matrix_Init(&kf->xhatminus, kf->xhatSize, 1, (float *)kf->xhatminus_data);

    if (uSize != 0)
    {
        // control vector u
        kf->u_data = (float *)malloc(sizeof(float) * uSize);
        memset(kf->u_data, 0, sizeof(float) * uSize);
        Matrix_Init(&kf->u, kf->uSize, 1, (float *)kf->u_data);
    }

    // measurement vector z
    kf->z_data = (float *)malloc(sizeof(float) * zSize);
    memset(kf->z_data, 0, sizeof(float) * zSize);
    Matrix_Init(&kf->z, kf->zSize, 1, (float *)kf->z_data);

    // covariance matrix P(k|k)
    kf->P_data = (float *)malloc(sizeof(float) * xhatSize * xhatSize);
    memset(kf->P_data, 0, sizeof(float) * xhatSize * xhatSize);
    Matrix_Init(&kf->P, kf->xhatSize, kf->xhatSize, (float *)kf->P_data);

    // create covariance matrix P(k|k-1)
    kf->Pminus_data = (float *)malloc(sizeof(float) * xhatSize * xhatSize);
    memset(kf->Pminus_data, 0, sizeof(float) * xhatSize * xhatSize);
    Matrix_Init(&kf->Pminus, kf->xhatSize, kf->xhatSize, (float *)kf->Pminus_data);

    // state transition matrix F FT
    kf->F_data = (float *)malloc(sizeof(float) * xhatSize * xhatSize);
    kf->FT_data = (float *)malloc(sizeof(float) * xhatSize * xhatSize);
    memset(kf->F_data, 0, sizeof(float) * xhatSize * xhatSize);
    memset(kf->FT_data, 0, sizeof(float) * xhatSize * xhatSize);
    Matrix_Init(&kf->F, kf->xhatSize, kf->xhatSize, (float *)kf->F_data);
    Matrix_Init(&kf->FT, kf->xhatSize, kf->xhatSize, (float *)kf->FT_data);

    if (uSize != 0)
    {
        // control matrix B
        kf->B_data = (float *)malloc(sizeof(float) * xhatSize * uSize);
        memset(kf->B_data, 0, sizeof(float) * xhatSize * uSize);
        Matrix_Init(&kf->B, kf->xhatSize, kf->uSize, (float *)kf->B_data);
    }

    // measurement matrix H
    kf->H_data = (float *)malloc(sizeof(float) * zSize * xhatSize);
    kf->HT_data = (float *)malloc(sizeof(float) * xhatSize * zSize);
    memset(kf->H_data, 0, sizeof(float) * zSize * xhatSize);
    memset(kf->HT_data, 0, sizeof(float) * xhatSize * zSize);
    Matrix_Init(&kf->H, kf->zSize, kf->xhatSize, (float *)kf->H_data);
    Matrix_Init(&kf->HT, kf->xhatSize, kf->zSize, (float *)kf->HT_data);

    // process noise covariance matrix Q
    kf->Q_data = (float *)malloc(sizeof(float) * xhatSize * xhatSize);
    memset(kf->Q_data, 0, sizeof(float) * xhatSize * xhatSize);
    Matrix_Init(&kf->Q, kf->xhatSize, kf->xhatSize, (float *)kf->Q_data);

    // measurement noise covariance matrix R
    kf->R_data = (float *)malloc(sizeof(float) * zSize * zSize);
    memset(kf->R_data, 0, sizeof(float) * zSize * zSize);
    Matrix_Init(&kf->R, kf->zSize, kf->zSize, (float *)kf->R_data);

    // kalman gain K
    kf->K_data = (float *)malloc(sizeof(float) * xhatSize * zSize);
    memset(kf->K_data, 0, sizeof(float) * xhatSize * zSize);
    Matrix_Init(&kf->K, kf->xhatSize, kf->zSize, (float *)kf->K_data);

    kf->S_data = (float *)malloc(sizeof(float) * kf->xhatSize * kf->xhatSize);
    kf->temp_matrix_data1 = (float *)malloc(sizeof(float) * kf->xhatSize * kf->xhatSize);
    kf->temp_matrix_data2 = (float *)malloc(sizeof(float) * kf->xhatSize * kf->xhatSize);
    kf->temp_vector_data1 = (float *)malloc(sizeof(float) * kf->xhatSize);
    kf->temp_vector_data2 = (float *)malloc(sizeof(float) * kf->xhatSize);
    Matrix_Init(&kf->S, kf->xhatSize, kf->xhatSize, (float *)kf->S_data);
    Matrix_Init(&kf->temp_matrix1, kf->xhatSize, kf->xhatSize, (float *)kf->temp_matrix_data1);
    Matrix_Init(&kf->temp_matrix2, kf->xhatSize, kf->xhatSize, (float *)kf->temp_matrix_data2);
    Matrix_Init(&kf->temp_vector1, kf->xhatSize, 1, (float *)kf->temp_vector_data1);
    Matrix_Init(&kf->temp_vector2, kf->xhatSize, 1, (float *)kf->temp_vector_data2);

    kf->SkipEq1 = 0;
    kf->SkipEq2 = 0;
    kf->SkipEq3 = 0;
    kf->SkipEq4 = 0;
    kf->SkipEq5 = 0;
}

/**
 * @brief      Matrix H K R auto adjustment
 * @param      *kf ：
 * @retval
 */
void KF_Watch(KF_DataTypeDef *kf)
{
    memcpy(kf->z_data, kf->MeasureVector, sizeof(float) * kf->zSize);
    memset(kf->MeasureVector, 0, sizeof(float) * kf->zSize);
    memcpy(kf->u_data, kf->ControlVector, sizeof(float) * kf->uSize);
}

