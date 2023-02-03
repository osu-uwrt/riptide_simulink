/*
 * Porportional.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Porportional".
 *
 * Model version              : 1.0
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Tue Jan 24 13:39:21 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Porportional.h"
#include "rtwtypes.h"
#include "Porportional_private.h"
#include <string.h>
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_Porportional_T Porportional_B;

/* Block states (default storage) */
DW_Porportional_T Porportional_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Porportional_T Porportional_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Porportional_T Porportional_Y;

/* Real-time model */
static RT_MODEL_Porportional_T Porportional_M_;
RT_MODEL_Porportional_T *const Porportional_M = &Porportional_M_;

/* Model step function */
void Porportional_step(void)
{
  real_T rtb_Subtract_f;

  /* Outputs for Atomic SubSystem: '<Root>/Porportional Follower' */
  /* Memory: '<S1>/Previous Signal Value' */
  Porportional_B.PreviousSignalValue =
    Porportional_DW.PreviousSignalValue_PreviousInp;

  /* Clock: '<S2>/Clock' */
  Porportional_B.Clock = Porportional_M->Timing.t[0];

  /* Memory: '<S2>/Memory2' */
  Porportional_B.Memory2 = Porportional_DW.Memory2_PreviousInput;

  /* Sum: '<S2>/Subtract' */
  rtb_Subtract_f = Porportional_B.Clock - Porportional_B.Memory2;

  /* Memory: '<S1>/Previous Velocity' */
  Porportional_B.PreviousVelocity =
    Porportional_DW.PreviousVelocity_PreviousInput;

  /* Sum: '<S1>/Add' incorporates:
   *  Constant: '<S1>/P'
   *  Inport: '<Root>/In1'
   *  Product: '<S1>/Acceleration'
   *  Product: '<S1>/Delta Velocity'
   *  Sum: '<S1>/Subtract'
   */
  Porportional_B.Add = (Porportional_U.In1 - Porportional_B.PreviousSignalValue)
    * Porportional_P.P_Value * rtb_Subtract_f + Porportional_B.PreviousVelocity;

  /* Sum: '<S1>/Add1' incorporates:
   *  Product: '<S1>/Delta Position'
   */
  Porportional_B.Add1 = rtb_Subtract_f * Porportional_B.Add +
    Porportional_B.PreviousSignalValue;

  /* End of Outputs for SubSystem: '<Root>/Porportional Follower' */

  /* Outport: '<Root>/Out1' */
  Porportional_Y.Out1 = Porportional_B.Add1;

  /* Matfile logging */
  rt_UpdateTXYLogVars(Porportional_M->rtwLogInfo, (Porportional_M->Timing.t));

  /* Update for Atomic SubSystem: '<Root>/Porportional Follower' */
  /* Update for Memory: '<S1>/Previous Signal Value' */
  Porportional_DW.PreviousSignalValue_PreviousInp = Porportional_B.Add1;

  /* Update for Memory: '<S2>/Memory2' */
  Porportional_DW.Memory2_PreviousInput = Porportional_B.Clock;

  /* Update for Memory: '<S1>/Previous Velocity' */
  Porportional_DW.PreviousVelocity_PreviousInput = Porportional_B.Add;

  /* End of Update for SubSystem: '<Root>/Porportional Follower' */

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.0s, 0.0s] */
    if ((rtmGetTFinal(Porportional_M)!=-1) &&
        !((rtmGetTFinal(Porportional_M)-Porportional_M->Timing.t[0]) >
          Porportional_M->Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus(Porportional_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++Porportional_M->Timing.clockTick0)) {
    ++Porportional_M->Timing.clockTickH0;
  }

  Porportional_M->Timing.t[0] = Porportional_M->Timing.clockTick0 *
    Porportional_M->Timing.stepSize0 + Porportional_M->Timing.clockTickH0 *
    Porportional_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.2s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.2, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    Porportional_M->Timing.clockTick1++;
    if (!Porportional_M->Timing.clockTick1) {
      Porportional_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void Porportional_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)Porportional_M, 0,
                sizeof(RT_MODEL_Porportional_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Porportional_M->solverInfo,
                          &Porportional_M->Timing.simTimeStep);
    rtsiSetTPtr(&Porportional_M->solverInfo, &rtmGetTPtr(Porportional_M));
    rtsiSetStepSizePtr(&Porportional_M->solverInfo,
                       &Porportional_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Porportional_M->solverInfo, (&rtmGetErrorStatus
      (Porportional_M)));
    rtsiSetRTModelPtr(&Porportional_M->solverInfo, Porportional_M);
  }

  rtsiSetSimTimeStep(&Porportional_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Porportional_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Porportional_M, &Porportional_M->Timing.tArray[0]);
  rtmSetTFinal(Porportional_M, 10.0);
  Porportional_M->Timing.stepSize0 = 0.2;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    Porportional_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(Porportional_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Porportional_M->rtwLogInfo, (NULL));
    rtliSetLogT(Porportional_M->rtwLogInfo, "tout");
    rtliSetLogX(Porportional_M->rtwLogInfo, "");
    rtliSetLogXFinal(Porportional_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Porportional_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Porportional_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Porportional_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Porportional_M->rtwLogInfo, 1);
    rtliSetLogY(Porportional_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Porportional_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Porportional_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &Porportional_B), 0,
                sizeof(B_Porportional_T));

  /* states (dwork) */
  (void) memset((void *)&Porportional_DW, 0,
                sizeof(DW_Porportional_T));

  /* external inputs */
  Porportional_U.In1 = 0.0;

  /* external outputs */
  Porportional_Y.Out1 = 0.0;

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(Porportional_M->rtwLogInfo, 0.0, rtmGetTFinal
    (Porportional_M), Porportional_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Porportional_M)));

  /* SystemInitialize for Atomic SubSystem: '<Root>/Porportional Follower' */
  /* InitializeConditions for Memory: '<S1>/Previous Signal Value' */
  Porportional_DW.PreviousSignalValue_PreviousInp =
    Porportional_P.PreviousSignalValue_InitialCond;

  /* InitializeConditions for Memory: '<S2>/Memory2' */
  Porportional_DW.Memory2_PreviousInput =
    Porportional_P.Memory2_InitialCondition;

  /* InitializeConditions for Memory: '<S1>/Previous Velocity' */
  Porportional_DW.PreviousVelocity_PreviousInput =
    Porportional_P.PreviousVelocity_InitialConditi;

  /* End of SystemInitialize for SubSystem: '<Root>/Porportional Follower' */
}

/* Model terminate function */
void Porportional_terminate(void)
{
  /* (no terminate code required) */
}
