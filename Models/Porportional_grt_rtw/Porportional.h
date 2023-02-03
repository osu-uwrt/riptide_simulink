/*
 * Porportional.h
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

#ifndef RTW_HEADER_Porportional_h_
#define RTW_HEADER_Porportional_h_
#ifndef Porportional_COMMON_INCLUDES_
#define Porportional_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                                 /* Porportional_COMMON_INCLUDES_ */

#include "Porportional_types.h"
#include <float.h>
#include <string.h>
#include <stddef.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
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

/* Block signals (default storage) */
typedef struct {
  real_T PreviousSignalValue;          /* '<S1>/Previous Signal Value' */
  real_T Clock;                        /* '<S2>/Clock' */
  real_T Memory2;                      /* '<S2>/Memory2' */
  real_T PreviousVelocity;             /* '<S1>/Previous Velocity' */
  real_T Add;                          /* '<S1>/Add' */
  real_T Add1;                         /* '<S1>/Add1' */
} B_Porportional_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T PreviousSignalValue_PreviousInp;/* '<S1>/Previous Signal Value' */
  real_T Memory2_PreviousInput;        /* '<S2>/Memory2' */
  real_T PreviousVelocity_PreviousInput;/* '<S1>/Previous Velocity' */
} DW_Porportional_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T In1;                          /* '<Root>/In1' */
} ExtU_Porportional_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1;                         /* '<Root>/Out1' */
} ExtY_Porportional_T;

/* Parameters (default storage) */
struct P_Porportional_T_ {
  real_T P_Value;                      /* Expression: 9
                                        * Referenced by: '<S1>/P'
                                        */
  real_T PreviousSignalValue_InitialCond;/* Expression: 0
                                          * Referenced by: '<S1>/Previous Signal Value'
                                          */
  real_T Memory2_InitialCondition;     /* Expression: 0
                                        * Referenced by: '<S2>/Memory2'
                                        */
  real_T PreviousVelocity_InitialConditi;/* Expression: 0
                                          * Referenced by: '<S1>/Previous Velocity'
                                          */
};

/* Real-time Model Data Structure */
struct tag_RTM_Porportional_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;

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
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_Porportional_T Porportional_P;

/* Block signals (default storage) */
extern B_Porportional_T Porportional_B;

/* Block states (default storage) */
extern DW_Porportional_T Porportional_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Porportional_T Porportional_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Porportional_T Porportional_Y;

/* Model entry point functions */
extern void Porportional_initialize(void);
extern void Porportional_step(void);
extern void Porportional_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Porportional_T *const Porportional_M;

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
 * hilite_system('untitled/Porportional Follower')    - opens subsystem untitled/Porportional Follower
 * hilite_system('untitled/Porportional Follower/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'untitled'
 * '<S1>'   : 'untitled/Porportional Follower'
 * '<S2>'   : 'untitled/Porportional Follower/Delta Time'
 */
#endif                                 /* RTW_HEADER_Porportional_h_ */
