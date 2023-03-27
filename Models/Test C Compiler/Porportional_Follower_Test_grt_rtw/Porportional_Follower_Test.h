/*
 * Porportional_Follower_Test.h
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

#ifndef RTW_HEADER_Porportional_Follower_Test_h_
#define RTW_HEADER_Porportional_Follower_Test_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "Porportional_Follower_Test_types.h"
#include <cfloat>

extern "C"
{

#include "rt_nonfinite.h"

}

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
struct B_Porportional_Follower_Test_T {
  real_T PreviousSignalValue;          /* '<S1>/Previous Signal Value' */
  real_T Clock;                        /* '<S2>/Clock' */
  real_T Memory2;                      /* '<S2>/Memory2' */
  real_T PreviousVelocity;             /* '<S1>/Previous Velocity' */
  real_T Add;                          /* '<S1>/Add' */
  real_T Add1;                         /* '<S1>/Add1' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_Porportional_Follower_Test_T {
  real_T PreviousSignalValue_PreviousInp;/* '<S1>/Previous Signal Value' */
  real_T Memory2_PreviousInput;        /* '<S2>/Memory2' */
  real_T PreviousVelocity_PreviousInput;/* '<S1>/Previous Velocity' */
};

/* External inputs (root inport signals with default storage) */
struct ExtU_Porportional_Follower_Te_T {
  real_T Signal;                       /* '<Root>/Signal' */
};

/* External outputs (root outports fed by signals with default storage) */
struct ExtY_Porportional_Follower_Te_T {
  real_T Follower;                     /* '<Root>/Follower' */
};

/* Real-time Model Data Structure */
struct tag_RTM_Porportional_Follower_T {
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

/* Class declaration for model Porportional_Follower_Test */
class Porportional_Follower_Test final
{
  /* public data and function members */
 public:
  /* Copy Constructor */
  Porportional_Follower_Test(Porportional_Follower_Test const&) = delete;

  /* Assignment Operator */
  Porportional_Follower_Test& operator= (Porportional_Follower_Test const&) & =
    delete;

  /* Move Constructor */
  Porportional_Follower_Test(Porportional_Follower_Test &&) = delete;

  /* Move Assignment Operator */
  Porportional_Follower_Test& operator= (Porportional_Follower_Test &&) = delete;

  /* Real-Time Model get method */
  RT_MODEL_Porportional_Followe_T * getRTM();

  /* Root inports set method */
  void setExternalInputs(const ExtU_Porportional_Follower_Te_T
    *pExtU_Porportional_Follower_Te_T)
  {
    Porportional_Follower_Test_U = *pExtU_Porportional_Follower_Te_T;
  }

  /* Root outports get method */
  const ExtY_Porportional_Follower_Te_T &getExternalOutputs() const
  {
    return Porportional_Follower_Test_Y;
  }

  /* Initial conditions function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  static void terminate();

  /* Constructor */
  Porportional_Follower_Test();

  /* Destructor */
  ~Porportional_Follower_Test();

  /* private data and function members */
 private:
  /* External inputs */
  ExtU_Porportional_Follower_Te_T Porportional_Follower_Test_U;

  /* External outputs */
  ExtY_Porportional_Follower_Te_T Porportional_Follower_Test_Y;

  /* Block signals */
  B_Porportional_Follower_Test_T Porportional_Follower_Test_B;

  /* Block states */
  DW_Porportional_Follower_Test_T Porportional_Follower_Test_DW;

  /* Real-Time Model */
  RT_MODEL_Porportional_Followe_T Porportional_Follower_Test_M;
};

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
 * hilite_system('TestController/Porportional_Follower_Test')    - opens subsystem TestController/Porportional_Follower_Test
 * hilite_system('TestController/Porportional_Follower_Test/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'TestController'
 * '<S1>'   : 'TestController/Porportional_Follower_Test'
 * '<S2>'   : 'TestController/Porportional_Follower_Test/Delta Time'
 */
#endif                            /* RTW_HEADER_Porportional_Follower_Test_h_ */
