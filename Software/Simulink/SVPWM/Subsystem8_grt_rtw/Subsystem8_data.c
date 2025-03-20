/*
 * Subsystem8_data.c
 *
 * Code generation for model "Subsystem8".
 *
 * Model version              : 1.11
 * Simulink Coder version : 23.2 (R2023b) 01-Aug-2023
 * C source code generated on : Wed Mar 19 22:39:14 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Subsystem8.h"

/* Block parameters (default storage) */
P_Subsystem8_T Subsystem8_P = {
  /* Computed Parameter: LPS3_A_pr
   * Referenced by: '<S1>/LPS3'
   */
  { -1256.6370614359173, 1256.6370614359173, -4987.123729239649,
    -1256.6370614359168, 1256.6370614359171, -4987.123729239649,
    -1256.6370614359173, -4987.123729239649, 4987.123729239649,
    4987.123729239649, 4987.123729239649 },

  /* Computed Parameter: LPS3_B_pr
   * Referenced by: '<S1>/LPS3'
   */
  1256.6370614359173,

  /* Computed Parameter: LPS3_C_pr
   * Referenced by: '<S1>/LPS3'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S1>/LPS3'
   */
  0.0,

  /* Computed Parameter: LPS3_A_ir
   * Referenced by: '<S1>/LPS3'
   */
  { 0U, 1U, 3U, 1U, 2U, 4U, 1U, 5U, 0U, 1U, 2U },

  /* Computed Parameter: LPS3_A_jc
   * Referenced by: '<S1>/LPS3'
   */
  { 0U, 3U, 6U, 8U, 9U, 10U, 11U },

  /* Computed Parameter: LPS3_B_ir
   * Referenced by: '<S1>/LPS3'
   */
  0U,

  /* Computed Parameter: LPS3_B_jc
   * Referenced by: '<S1>/LPS3'
   */
  { 0U, 1U },

  /* Computed Parameter: LPS3_C_ir
   * Referenced by: '<S1>/LPS3'
   */
  0U,

  /* Computed Parameter: LPS3_C_jc
   * Referenced by: '<S1>/LPS3'
   */
  { 0U, 0U, 0U, 1U, 1U, 1U, 1U }
};
