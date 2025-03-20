/*
 * Subsystem8.c
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
#include "rtwtypes.h"
#include "Subsystem8_private.h"
#include "rt_nonfinite.h"

/* Continuous states */
X_Subsystem8_T Subsystem8_X;

/* Disabled State Vector */
XDis_Subsystem8_T Subsystem8_XDis;

/* External inputs (root inport signals with default storage) */
ExtU_Subsystem8_T Subsystem8_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Subsystem8_T Subsystem8_Y;

/* Real-time model */
static RT_MODEL_Subsystem8_T Subsystem8_M_;
RT_MODEL_Subsystem8_T *const Subsystem8_M = &Subsystem8_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 6;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Subsystem8_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Subsystem8_step();
  Subsystem8_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Subsystem8_step();
  Subsystem8_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void Subsystem8_step(void)
{
  real_T rtb_LPS3;
  int32_T ci;
  uint32_T ri;
  if (rtmIsMajorTimeStep(Subsystem8_M)) {
    /* set solver stop time */
    if (!(Subsystem8_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Subsystem8_M->solverInfo,
                            ((Subsystem8_M->Timing.clockTickH0 + 1) *
        Subsystem8_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Subsystem8_M->solverInfo,
                            ((Subsystem8_M->Timing.clockTick0 + 1) *
        Subsystem8_M->Timing.stepSize0 + Subsystem8_M->Timing.clockTickH0 *
        Subsystem8_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Subsystem8_M)) {
    Subsystem8_M->Timing.t[0] = rtsiGetT(&Subsystem8_M->solverInfo);
  }

  /* StateSpace: '<S1>/LPS3' */
  rtb_LPS3 = 0.0;
  for (ci = 0; ci < 6; ci++) {
    for (ri = Subsystem8_P.LPS3_C_jc[(uint32_T)ci]; ri < Subsystem8_P.LPS3_C_jc
         [(uint32_T)ci + 1U]; ri++) {
      rtb_LPS3 += Subsystem8_P.LPS3_C_pr * Subsystem8_X.LPS3_CSTATE[(uint32_T)ci];
    }
  }

  /* End of StateSpace: '<S1>/LPS3' */

  /* Outport: '<Root>/Out1' */
  Subsystem8_Y.Out1 = rtb_LPS3;
  if (rtmIsMajorTimeStep(Subsystem8_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(Subsystem8_M->rtwLogInfo, (Subsystem8_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(Subsystem8_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(Subsystem8_M)!=-1) &&
          !((rtmGetTFinal(Subsystem8_M)-(((Subsystem8_M->Timing.clockTick1+
               Subsystem8_M->Timing.clockTickH1* 4294967296.0)) * 5.0E-7)) >
            (((Subsystem8_M->Timing.clockTick1+Subsystem8_M->Timing.clockTickH1*
               4294967296.0)) * 5.0E-7) * (DBL_EPSILON))) {
        rtmSetErrorStatus(Subsystem8_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&Subsystem8_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++Subsystem8_M->Timing.clockTick0)) {
      ++Subsystem8_M->Timing.clockTickH0;
    }

    Subsystem8_M->Timing.t[0] = rtsiGetSolverStopTime(&Subsystem8_M->solverInfo);

    {
      /* Update absolute timer for sample time: [5.0E-7s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 5.0E-7, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      Subsystem8_M->Timing.clockTick1++;
      if (!Subsystem8_M->Timing.clockTick1) {
        Subsystem8_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Subsystem8_derivatives(void)
{
  XDot_Subsystem8_T *_rtXdot;
  int_T is;
  uint32_T ri;
  _rtXdot = ((XDot_Subsystem8_T *) Subsystem8_M->derivs);

  /* Derivatives for StateSpace: '<S1>/LPS3' incorporates:
   *  Inport: '<Root>/iq+iqh'
   */
  for (is = 0; is < 6; is++) {
    _rtXdot->LPS3_CSTATE[is] = 0.0;
  }

  for (is = 0; is < 6; is++) {
    for (ri = Subsystem8_P.LPS3_A_jc[(uint32_T)is]; ri < Subsystem8_P.LPS3_A_jc
         [(uint32_T)is + 1U]; ri++) {
      _rtXdot->LPS3_CSTATE[Subsystem8_P.LPS3_A_ir[ri]] +=
        Subsystem8_P.LPS3_A_pr[ri] * Subsystem8_X.LPS3_CSTATE[(uint32_T)is];
    }
  }

  for (ri = Subsystem8_P.LPS3_B_jc[0U]; ri < Subsystem8_P.LPS3_B_jc[1U]; ri++) {
    _rtXdot->LPS3_CSTATE[Subsystem8_P.LPS3_B_ir] += Subsystem8_P.LPS3_B_pr *
      Subsystem8_U.iqiqh;
  }

  /* End of Derivatives for StateSpace: '<S1>/LPS3' */
}

/* Model initialize function */
void Subsystem8_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)Subsystem8_M, 0,
                sizeof(RT_MODEL_Subsystem8_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Subsystem8_M->solverInfo,
                          &Subsystem8_M->Timing.simTimeStep);
    rtsiSetTPtr(&Subsystem8_M->solverInfo, &rtmGetTPtr(Subsystem8_M));
    rtsiSetStepSizePtr(&Subsystem8_M->solverInfo,
                       &Subsystem8_M->Timing.stepSize0);
    rtsiSetdXPtr(&Subsystem8_M->solverInfo, &Subsystem8_M->derivs);
    rtsiSetContStatesPtr(&Subsystem8_M->solverInfo, (real_T **)
                         &Subsystem8_M->contStates);
    rtsiSetNumContStatesPtr(&Subsystem8_M->solverInfo,
      &Subsystem8_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Subsystem8_M->solverInfo,
      &Subsystem8_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Subsystem8_M->solverInfo,
      &Subsystem8_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Subsystem8_M->solverInfo,
      &Subsystem8_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&Subsystem8_M->solverInfo, (boolean_T**)
      &Subsystem8_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&Subsystem8_M->solverInfo, (&rtmGetErrorStatus
      (Subsystem8_M)));
    rtsiSetRTModelPtr(&Subsystem8_M->solverInfo, Subsystem8_M);
  }

  rtsiSetSimTimeStep(&Subsystem8_M->solverInfo, MAJOR_TIME_STEP);
  Subsystem8_M->intgData.y = Subsystem8_M->odeY;
  Subsystem8_M->intgData.f[0] = Subsystem8_M->odeF[0];
  Subsystem8_M->intgData.f[1] = Subsystem8_M->odeF[1];
  Subsystem8_M->intgData.f[2] = Subsystem8_M->odeF[2];
  Subsystem8_M->contStates = ((X_Subsystem8_T *) &Subsystem8_X);
  Subsystem8_M->contStateDisabled = ((XDis_Subsystem8_T *) &Subsystem8_XDis);
  Subsystem8_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&Subsystem8_M->solverInfo, (void *)&Subsystem8_M->intgData);
  rtsiSetIsMinorTimeStepWithModeChange(&Subsystem8_M->solverInfo, false);
  rtsiSetSolverName(&Subsystem8_M->solverInfo,"ode3");
  rtmSetTPtr(Subsystem8_M, &Subsystem8_M->Timing.tArray[0]);
  rtmSetTFinal(Subsystem8_M, 50.0);
  Subsystem8_M->Timing.stepSize0 = 5.0E-7;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    Subsystem8_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(Subsystem8_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Subsystem8_M->rtwLogInfo, (NULL));
    rtliSetLogT(Subsystem8_M->rtwLogInfo, "tout");
    rtliSetLogX(Subsystem8_M->rtwLogInfo, "");
    rtliSetLogXFinal(Subsystem8_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Subsystem8_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Subsystem8_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Subsystem8_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Subsystem8_M->rtwLogInfo, 1);
    rtliSetLogY(Subsystem8_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Subsystem8_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Subsystem8_M->rtwLogInfo, (NULL));
  }

  /* states (continuous) */
  {
    (void) memset((void *)&Subsystem8_X, 0,
                  sizeof(X_Subsystem8_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&Subsystem8_XDis, 0,
                  sizeof(XDis_Subsystem8_T));
  }

  /* external inputs */
  Subsystem8_U.iqiqh = 0.0;

  /* external outputs */
  Subsystem8_Y.Out1 = 0.0;

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(Subsystem8_M->rtwLogInfo, 0.0, rtmGetTFinal
    (Subsystem8_M), Subsystem8_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Subsystem8_M)));

  {
    int_T is;

    /* InitializeConditions for StateSpace: '<S1>/LPS3' */
    for (is = 0; is < 6; is++) {
      Subsystem8_X.LPS3_CSTATE[is] = Subsystem8_P.LPS3_InitialCondition;
    }

    /* End of InitializeConditions for StateSpace: '<S1>/LPS3' */
  }
}

/* Model terminate function */
void Subsystem8_terminate(void)
{
  /* (no terminate code required) */
}
