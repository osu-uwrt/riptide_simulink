//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Porportional.cpp
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
#include "Porportional.h"
#include "rtwtypes.h"
#include "Porportional_private.h"

// Model step function
void Porportional::step()
{
  real_T rtb_Subtract_f;

  // Outputs for Atomic SubSystem: '<Root>/Porportional Follower'
  // Clock: '<S2>/Clock'
  Porportional_B.Clock = (&Porportional_M)->Timing.t[0];

  // Sum: '<S2>/Subtract' incorporates:
  //   Memory: '<S2>/Memory2'

  rtb_Subtract_f = Porportional_B.Clock - Porportional_DW.Memory2_PreviousInput;

  // Sum: '<S1>/Add' incorporates:
  //   Constant: '<S1>/P'
  //   Inport: '<Root>/Signal'
  //   Memory: '<S1>/Previous Velocity'
  //   Product: '<S1>/Acceleration'
  //   Product: '<S1>/Delta Velocity'
  //   Sum: '<S1>/Subtract'

  Porportional_B.Add = (Porportional_U.Signal -
                        Porportional_DW.PreviousSignalValue_PreviousInp) * 3.0 *
    rtb_Subtract_f + Porportional_DW.PreviousVelocity_PreviousInput;

  // Sum: '<S1>/Add1' incorporates:
  //   Product: '<S1>/Delta Position'

  Porportional_B.Add1 = rtb_Subtract_f * Porportional_B.Add +
    Porportional_DW.PreviousSignalValue_PreviousInp;

  // End of Outputs for SubSystem: '<Root>/Porportional Follower'

  // Outport: '<Root>/Follower'
  Porportional_Y.Follower = Porportional_B.Add1;

  // Update for Atomic SubSystem: '<Root>/Porportional Follower'
  // Update for Memory: '<S1>/Previous Signal Value'
  Porportional_DW.PreviousSignalValue_PreviousInp = Porportional_B.Add1;

  // Update for Memory: '<S2>/Memory2'
  Porportional_DW.Memory2_PreviousInput = Porportional_B.Clock;

  // Update for Memory: '<S1>/Previous Velocity'
  Porportional_DW.PreviousVelocity_PreviousInput = Porportional_B.Add;

  // End of Update for SubSystem: '<Root>/Porportional Follower'

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  (&Porportional_M)->Timing.t[0] =
    ((time_T)(++(&Porportional_M)->Timing.clockTick0)) * (&Porportional_M)
    ->Timing.stepSize0;

  {
    // Update absolute timer for sample time: [0.1s, 0.0s]
    // The "clockTick1" counts the number of times the code of this task has
    //  been executed. The resolution of this integer timer is 0.1, which is the step size
    //  of the task. Size of "clockTick1" ensures timer will not overflow during the
    //  application lifespan selected.

    (&Porportional_M)->Timing.clockTick1++;
  }
}

// Model initialize function
void Porportional::initialize()
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&Porportional_M)->solverInfo, &(&Porportional_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&Porportional_M)->solverInfo, &rtmGetTPtr((&Porportional_M)));
    rtsiSetStepSizePtr(&(&Porportional_M)->solverInfo, &(&Porportional_M)
                       ->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&(&Porportional_M)->solverInfo, (&rtmGetErrorStatus
      ((&Porportional_M))));
    rtsiSetRTModelPtr(&(&Porportional_M)->solverInfo, (&Porportional_M));
  }

  rtsiSetSimTimeStep(&(&Porportional_M)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&(&Porportional_M)->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr((&Porportional_M), &(&Porportional_M)->Timing.tArray[0]);
  (&Porportional_M)->Timing.stepSize0 = 0.1;
}

// Model terminate function
void Porportional::terminate()
{
  // (no terminate code required)
}

// Constructor
Porportional::Porportional() :
  Porportional_U(),
  Porportional_Y(),
  Porportional_B(),
  Porportional_DW(),
  Porportional_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
Porportional::~Porportional()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
Porportional::RT_MODEL_Porportional_T * Porportional::getRTM()
{
  return (&Porportional_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
