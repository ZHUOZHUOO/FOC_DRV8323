/*
 * @Date: 2025-02-27 19:16:35
 * @LastEditors: ZHUOZHUOO
 * @LastEditTime: 2025-02-28 00:49:15
 * @FilePath: \undefinedf:\ZHUOZHUOO--Github\FOC_DRV8323\Software\STM32G431 Cube\FOC_DRV8323\MDK-ARM\USER\Main\Foc_Error.h
 * @Description: Do not edit
 */
#ifndef __FOC_COMM_H
#define __FOC_COMM_H

#include "util_fdcan.h"
#include "configure.h"
#include "Foc_Control.h"
#include "Foc_Error.h"
#include "alg_pid.h"

#define CMD_THETA_SET 0x01
#define CMD_CURRENT_SET 0x02
#define CMD_THETA_CURRENT_FEEDBACK 0x03
#define CMD_ERROR_FEEDBACK 0x04

void FDCAN_IntFilterAndStart(void);

#endif
