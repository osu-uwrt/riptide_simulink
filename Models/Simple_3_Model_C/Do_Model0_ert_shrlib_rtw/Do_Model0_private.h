/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Do_Model0_private.h
 *
 * Code generated for Simulink model 'Do_Model0'.
 *
 * Model version                  : 2.62
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Mon Apr 24 18:45:50 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Do_Model0_private_h_
#define RTW_HEADER_Do_Model0_private_h_
#include "rtwtypes.h"
#include "Do_Model0_types.h"
#include "Do_Model0.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern void Do_Model0_SeperatetoComponents(const real_T rtu_robotFrameVelocity[3],
  real_T *rty_x, real_T *rty_y, real_T *rty_z);
extern void Do_Model0_ConverttoColumnVector(const real_T rtu_row[3], real_T
  rty_column[3]);
extern void Do_Model0_ConverttoRowVector(const real_T rtu_column[3], real_T
  rty_row[3]);
extern void Do_Model0_EulertoUnit(const real_T rtu_euler[3], real_T
  rty_direction[3]);

#endif                                 /* RTW_HEADER_Do_Model0_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
