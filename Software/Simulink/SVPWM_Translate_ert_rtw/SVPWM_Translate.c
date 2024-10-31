/*
 * File: SVPWM_Translate.c
 *
 * Code generated for Simulink model 'SVPWM_Translate'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Wed Oct 30 21:33:38 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "SVPWM_Translate.h"
#include <math.h>
#include "rtwtypes.h"

/* External inputs (root inport signals with default storage) */
ExtU_SVPWM_Translate_T SVPWM_Translate_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_SVPWM_Translate_T SVPWM_Translate_Y;

/* Real-time model */
static RT_MODEL_SVPWM_Translate_T SVPWM_Translate_M_;
RT_MODEL_SVPWM_Translate_T *const SVPWM_Translate_M = &SVPWM_Translate_M_;

/* Model step function */
void SVPWM_Translate_step(void)
{
  real_T rtb_Add2;

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   *  Inport: '<Root>/In3'
   *  MinMax: '<S1>/MinMax'
   *  MinMax: '<S1>/MinMax3'
   *  Sum: '<S1>/Add'
   */
  rtb_Add2 = (fmax(fmax(SVPWM_Translate_U.Va, SVPWM_Translate_U.Vb),
                   SVPWM_Translate_U.Vc) + fmin(fmin(SVPWM_Translate_U.Va,
    SVPWM_Translate_U.Vb), SVPWM_Translate_U.Vc)) * -0.5;

  /* Outport: '<Root>/Out1' incorporates:
   *  Gain: '<Root>/Gain'
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   *  Inport: '<Root>/In3'
   *  Sum: '<Root>/Add'
   *  Sum: '<Root>/Add1'
   *  Sum: '<Root>/Add2'
   */
  SVPWM_Translate_Y.Out1[0] = (SVPWM_Translate_U.Va + rtb_Add2) *
    1.1547005383792517;
  SVPWM_Translate_Y.Out1[1] = (SVPWM_Translate_U.Vb + rtb_Add2) *
    1.1547005383792517;
  SVPWM_Translate_Y.Out1[2] = (SVPWM_Translate_U.Vc + rtb_Add2) *
    1.1547005383792517;
}

/* Model initialize function */
void SVPWM_Translate_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void SVPWM_Translate_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
