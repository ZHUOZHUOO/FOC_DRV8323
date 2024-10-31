/*
 * File: SVPWM_Translate.h
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

#ifndef RTW_HEADER_SVPWM_Translate_h_
#define RTW_HEADER_SVPWM_Translate_h_
#ifndef SVPWM_Translate_COMMON_INCLUDES_
#define SVPWM_Translate_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* SVPWM_Translate_COMMON_INCLUDES_ */

#include "SVPWM_Translate_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Va;                           /* '<Root>/In1' */
  real_T Vb;                           /* '<Root>/In2' */
  real_T Vc;                           /* '<Root>/In3' */
} ExtU_SVPWM_Translate_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1[3];                      /* '<Root>/Out1' */
} ExtY_SVPWM_Translate_T;

/* Real-time Model Data Structure */
struct tag_RTM_SVPWM_Translate_T {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_SVPWM_Translate_T SVPWM_Translate_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SVPWM_Translate_T SVPWM_Translate_Y;

/* Model entry point functions */
extern void SVPWM_Translate_initialize(void);
extern void SVPWM_Translate_step(void);
extern void SVPWM_Translate_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SVPWM_Translate_T *const SVPWM_Translate_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SVPWM_Translate'
 * '<S1>'   : 'SVPWM_Translate/-0.5(MAX+MIN)'
 */
#endif                                 /* RTW_HEADER_SVPWM_Translate_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
