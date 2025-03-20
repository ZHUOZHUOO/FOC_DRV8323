/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2023-12-28 01:05:21
 */

#ifndef UTIL_FDCAN_H
#define UTIL_FDCAN_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "fdcan.h"

    void FDCAN_InitTxHeader(FDCAN_TxHeaderTypeDef *pheader, uint32_t id, uint32_t dlc, uint32_t baudrateswitch, uint32_t can_type);
    void FDCAN_IntFilterAndStart(FDCAN_HandleTypeDef *phfdcan);
    void FDCAN_Send(FDCAN_HandleTypeDef *phfdcan, FDCAN_TxHeaderTypeDef *ptxhead, uint8_t *pdata);
    void FDCAN_ErrorHandler(uint32_t ret);

#ifdef __cplusplus
}
#endif
#endif
