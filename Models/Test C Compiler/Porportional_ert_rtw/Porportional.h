//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Porportional.h
//
// Code generated for Simulink model 'Porportional'.
//
// Model version                  : 1.2
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Wed Jan 25 08:14:14 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_Porportional_h_
#define RTW_HEADER_Porportional_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "Porportional_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

// Class declaration for model Porportional
class Porportional final
{
  // public data and function members
 public:
  // Block signals (default storage)
  struct B_Porportional_T {
    real_T Clock;                      // '<S2>/Clock'
    real_T Add;                        // '<S1>/Add'
    real_T Add1;                       // '<S1>/Add1'
  };

  // Block states (default storage) for system '<Root>'
  struct DW_Porportional_T {
    real_T PreviousSignalValue_PreviousInp;// '<S1>/Previous Signal Value'
    real_T Memory2_PreviousInput;      // '<S2>/Memory2'
    real_T PreviousVelocity_PreviousInput;// '<S1>/Previous Velocity'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_Porportional_T {
    real_T Signal;                     // '<Root>/Signal'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_Porportional_T {
    real_T Follower;                   // '<Root>/Follower'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_Porportional_T {
    const char_T *errorStatus;
    RTWSolverInfo solverInfo;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
      time_T stepSize0;
      uint32_T clockTick1;
      SimTimeStep simTimeStep;
      time_T *t;
      time_T tArray[2];
    } Timing;
  };

  // Copy Constructor
  Porportional(Porportional const&) = delete;

  // Assignment Operator
  Porportional& operator= (Porportional const&) & = delete;

  // Move Constructor
  Porportional(Porportional &&) = delete;

  // Move Assignment Operator
  Porportional& operator= (Porportional &&) = delete;

  // Real-Time Model get method
  Porportional::RT_MODEL_Porportional_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_Porportional_T *pExtU_Porportional_T)
  {
    Porportional_U = *pExtU_Porportional_T;
  }

  // Root outports get method
  const ExtY_Porportional_T &getExternalOutputs() const
  {
    return Porportional_Y;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  Porportional();

  // Destructor
  ~Porportional();

  // private data and function members
 private:
  // External inputs
  ExtU_Porportional_T Porportional_U;

  // External outputs
  ExtY_Porportional_T Porportional_Y;

  // Block signals
  B_Porportional_T Porportional_B;

  // Block states
  DW_Porportional_T Porportional_DW;

  // Real-Time Model
  RT_MODEL_Porportional_T Porportional_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('TestController/Porportional Follower')    - opens subsystem TestController/Porportional Follower
//  hilite_system('TestController/Porportional Follower/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'TestController'
//  '<S1>'   : 'TestController/Porportional Follower'
//  '<S2>'   : 'TestController/Porportional Follower/Delta Time'

#endif                                 // RTW_HEADER_Porportional_h_

//
// File trailer for generated code.
//
// [EOF]
//
