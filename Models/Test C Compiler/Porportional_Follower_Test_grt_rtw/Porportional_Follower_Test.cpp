/*
 * Porportional_Follower_Test.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Porportional_Follower_Test".
 *
 * Model version              : 1.4
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C++ source code generated on : Sun Mar 26 17:46:18 2023
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Porportional_Follower_Test.h"
#include "rtwtypes.h"
#include "Porportional_Follower_Test_private.h"

extern "C"
{

#include "rt_nonfinite.h"

}

/* Model step function */
void Porportional_Follower_Test::step()
{
  real_T rtb_Subtract_f;

  /* Outputs for Atomic SubSystem: '<Root>/Porportional_Follower_Test' */
  /* Memory: '<S1>/Previous Signal Value' */
  Porportional_Follower_Test_B.PreviousSignalValue =
    Porportional_Follower_Test_DW.PreviousSignalValue_PreviousInp;

  /* Clock: '<S2>/Clock' */
  Porportional_Follower_Test_B.Clock = (&Porportional_Follower_Test_M)->
    Timing.t[0];

  /* Memory: '<S2>/Memory2' */
  Porportional_Follower_Test_B.Memory2 =
    Porportional_Follower_Test_DW.Memory2_PreviousInput;

  /* Sum: '<S2>/Subtract' */
  rtb_Subtract_f = Porportional_Follower_Test_B.Clock -
    Porportional_Follower_Test_B.Memory2;

  /* Memory: '<S1>/Previous Velocity' */
  Porportional_Follower_Test_B.PreviousVelocity =
    Porportional_Follower_Test_DW.PreviousVelocity_PreviousInput;

  /* Sum: '<S1>/Add' incorporates:
   *  Constant: '<S1>/P'
   *  Inport: '<Root>/Signal'
   *  Product: '<S1>/Acceleration'
   *  Product: '<S1>/Delta Velocity'
   *  Sum: '<S1>/Subtract'
   */
  Porportional_Follower_Test_B.Add = (Porportional_Follower_Test_U.Signal -
    Porportional_Follower_Test_B.PreviousSignalValue) * 3.0 * rtb_Subtract_f +
    Porportional_Follower_Test_B.PreviousVelocity;

  /* Sum: '<S1>/Add1' incorporates:
   *  Product: '<S1>/Delta Position'
   */
  Porportional_Follower_Test_B.Add1 = rtb_Subtract_f *
    Porportional_Follower_Test_B.Add +
    Porportional_Follower_Test_B.PreviousSignalValue;

  /* End of Outputs for SubSystem: '<Root>/Porportional_Follower_Test' */

  /* Outport: '<Root>/Follower' */
  Porportional_Follower_Test_Y.Follower = Porportional_Follower_Test_B.Add1;

  /* Matfile logging */
  rt_UpdateTXYLogVars((&Porportional_Follower_Test_M)->rtwLogInfo,
                      ((&Porportional_Follower_Test_M)->Timing.t));

  /* Update for Atomic SubSystem: '<Root>/Porportional_Follower_Test' */
  /* Update for Memory: '<S1>/Previous Signal Value' */
  Porportional_Follower_Test_DW.PreviousSignalValue_PreviousInp =
    Porportional_Follower_Test_B.Add1;

  /* Update for Memory: '<S2>/Memory2' */
  Porportional_Follower_Test_DW.Memory2_PreviousInput =
    Porportional_Follower_Test_B.Clock;

  /* Update for Memory: '<S1>/Previous Velocity' */
  Porportional_Follower_Test_DW.PreviousVelocity_PreviousInput =
    Porportional_Follower_Test_B.Add;

  /* End of Update for SubSystem: '<Root>/Porportional_Follower_Test' */

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.0s, 0.0s] */
    if ((rtmGetTFinal((&Porportional_Follower_Test_M))!=-1) &&
        !((rtmGetTFinal((&Porportional_Follower_Test_M))-
           (&Porportional_Follower_Test_M)->Timing.t[0]) >
          (&Porportional_Follower_Test_M)->Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus((&Porportional_Follower_Test_M), "Simulation finished");
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
  if (!(++(&Porportional_Follower_Test_M)->Timing.clockTick0)) {
    ++(&Porportional_Follower_Test_M)->Timing.clockTickH0;
  }

  (&Porportional_Follower_Test_M)->Timing.t[0] = (&Porportional_Follower_Test_M
    )->Timing.clockTick0 * (&Porportional_Follower_Test_M)->Timing.stepSize0 + (
    &Porportional_Follower_Test_M)->Timing.clockTickH0 *
    (&Porportional_Follower_Test_M)->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.1s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.1, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    (&Porportional_Follower_Test_M)->Timing.clockTick1++;
    if (!(&Porportional_Follower_Test_M)->Timing.clockTick1) {
      (&Porportional_Follower_Test_M)->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void Porportional_Follower_Test::initialize()
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&Porportional_Follower_Test_M)->solverInfo,
                          &(&Porportional_Follower_Test_M)->Timing.simTimeStep);
    rtsiSetTPtr(&(&Porportional_Follower_Test_M)->solverInfo, &rtmGetTPtr
                ((&Porportional_Follower_Test_M)));
    rtsiSetStepSizePtr(&(&Porportional_Follower_Test_M)->solverInfo,
                       &(&Porportional_Follower_Test_M)->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&(&Porportional_Follower_Test_M)->solverInfo,
                          (&rtmGetErrorStatus((&Porportional_Follower_Test_M))));
    rtsiSetRTModelPtr(&(&Porportional_Follower_Test_M)->solverInfo,
                      (&Porportional_Follower_Test_M));
  }

  rtsiSetSimTimeStep(&(&Porportional_Follower_Test_M)->solverInfo,
                     MAJOR_TIME_STEP);
  rtsiSetSolverName(&(&Porportional_Follower_Test_M)->solverInfo,
                    "FixedStepDiscrete");
  rtmSetTPtr((&Porportional_Follower_Test_M), &(&Porportional_Follower_Test_M)
             ->Timing.tArray[0]);
  rtmSetTFinal((&Porportional_Follower_Test_M), 10.0);
  (&Porportional_Follower_Test_M)->Timing.stepSize0 = 0.1;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (nullptr);
    (&Porportional_Follower_Test_M)->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo((&Porportional_Follower_Test_M)->rtwLogInfo, (nullptr));
    rtliSetLogXSignalPtrs((&Porportional_Follower_Test_M)->rtwLogInfo, (nullptr));
    rtliSetLogT((&Porportional_Follower_Test_M)->rtwLogInfo, "tout");
    rtliSetLogX((&Porportional_Follower_Test_M)->rtwLogInfo, "");
    rtliSetLogXFinal((&Porportional_Follower_Test_M)->rtwLogInfo, "");
    rtliSetLogVarNameModifier((&Porportional_Follower_Test_M)->rtwLogInfo, "rt_");
    rtliSetLogFormat((&Porportional_Follower_Test_M)->rtwLogInfo, 4);
    rtliSetLogMaxRows((&Porportional_Follower_Test_M)->rtwLogInfo, 0);
    rtliSetLogDecimation((&Porportional_Follower_Test_M)->rtwLogInfo, 1);
    rtliSetLogY((&Porportional_Follower_Test_M)->rtwLogInfo, "");
    rtliSetLogYSignalInfo((&Porportional_Follower_Test_M)->rtwLogInfo, (nullptr));
    rtliSetLogYSignalPtrs((&Porportional_Follower_Test_M)->rtwLogInfo, (nullptr));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime((&Porportional_Follower_Test_M)->rtwLogInfo,
    0.0, rtmGetTFinal((&Porportional_Follower_Test_M)),
    (&Porportional_Follower_Test_M)->Timing.stepSize0, (&rtmGetErrorStatus
    ((&Porportional_Follower_Test_M))));
}

/* Model terminate function */
void Porportional_Follower_Test::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
Porportional_Follower_Test::Porportional_Follower_Test() :
  Porportional_Follower_Test_U(),
  Porportional_Follower_Test_Y(),
  Porportional_Follower_Test_B(),
  Porportional_Follower_Test_DW(),
  Porportional_Follower_Test_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
Porportional_Follower_Test::~Porportional_Follower_Test()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_Porportional_Followe_T * Porportional_Follower_Test::getRTM()
{
  return (&Porportional_Follower_Test_M);
}
