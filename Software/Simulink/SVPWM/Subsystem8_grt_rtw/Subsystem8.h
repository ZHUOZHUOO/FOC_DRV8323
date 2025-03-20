/*
 * Subsystem8.h
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

#ifndef RTW_HEADER_Subsystem8_h_
#define RTW_HEADER_Subsystem8_h_
#ifndef Subsystem8_COMMON_INCLUDES_
#define Subsystem8_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                                 /* Subsystem8_COMMON_INCLUDES_ */

#include "Subsystem8_types.h"
#include <float.h>
#include <string.h>
#include <stddef.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Continuous states (default storage) */
typedef struct {
  real_T LPS3_CSTATE[6];               /* '<S1>/LPS3' */
} X_Subsystem8_T;

/* State derivatives (default storage) */
typedef struct {
  real_T LPS3_CSTATE[6];               /* '<S1>/LPS3' */
} XDot_Subsystem8_T;

/* State disabled  */
typedef struct {
  boolean_T LPS3_CSTATE[6];            /* '<S1>/LPS3' */
} XDis_Subsystem8_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T iqiqh;                        /* '<Root>/iq+iqh' */
} ExtU_Subsystem8_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1;                         /* '<Root>/Out1' */
} ExtY_Subsystem8_T;

/* Parameters (default storage) */
struct P_Subsystem8_T_ {
  real_T LPS3_A_pr[11];                /* Computed Parameter: LPS3_A_pr
                                        * Referenced by: '<S1>/LPS3'
                                        */
  real_T LPS3_B_pr;                    /* Computed Parameter: LPS3_B_pr
                                        * Referenced by: '<S1>/LPS3'
                                        */
  real_T LPS3_C_pr;                    /* Computed Parameter: LPS3_C_pr
                                        * Referenced by: '<S1>/LPS3'
                                        */
  real_T LPS3_InitialCondition;        /* Expression: 0
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_A_ir[11];              /* Computed Parameter: LPS3_A_ir
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_A_jc[7];               /* Computed Parameter: LPS3_A_jc
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_B_ir;                  /* Computed Parameter: LPS3_B_ir
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_B_jc[2];               /* Computed Parameter: LPS3_B_jc
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_C_ir;                  /* Computed Parameter: LPS3_C_ir
                                        * Referenced by: '<S1>/LPS3'
                                        */
  uint32_T LPS3_C_jc[7];               /* Computed Parameter: LPS3_C_jc
                                        * Referenced by: '<S1>/LPS3'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Subsystem8_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_Subsystem8_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_Subsystem8_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[6];
  real_T odeF[3][6];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T tStart;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_Subsystem8_T Subsystem8_P;

/* Continuous states (default storage) */
extern X_Subsystem8_T Subsystem8_X;

/* Disabled states (default storage) */
extern XDis_Subsystem8_T Subsystem8_XDis;

/* External inputs (root inport signals with default storage) */
extern ExtU_Subsystem8_T Subsystem8_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Subsystem8_T Subsystem8_Y;

/* Model entry point functions */
extern void Subsystem8_initialize(void);
extern void Subsystem8_step(void);
extern void Subsystem8_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Subsystem8_T *const Subsystem8_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('SVPWM_nosense/Subsystem8')    - opens subsystem SVPWM_nosense/Subsystem8
 * hilite_system('SVPWM_nosense/Subsystem8/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SVPWM_nosense'
 * '<S1>'   : 'SVPWM_nosense/Subsystem8'
 */
#endif                                 /* RTW_HEADER_Subsystem8_h_ */
