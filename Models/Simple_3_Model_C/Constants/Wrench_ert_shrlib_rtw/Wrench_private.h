/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Wrench_private.h
 *
 * Code generated for Simulink model 'Wrench'.
 *
 * Model version                  : 2.62
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Mon Apr 24 18:44:07 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Wrench_private_h_
#define RTW_HEADER_Wrench_private_h_
#include "rtwtypes.h"
#include "Wrench_types.h"
#include "Wrench.h"

extern void Wrench_SeperatetoComponents(const real_T rtu_robotFrameVelocity[3],
  real_T *rty_x, real_T *rty_y, real_T *rty_z);
extern void Wrench_ConverttoColumnVector(const real_T rtu_row[3], real_T
  rty_column[3]);
extern void Wrench_ConverttoRowVector(const real_T rtu_column[3], real_T
  rty_row[3]);
extern void Wrench_EulertoUnit(const real_T rtu_euler[3], real_T rty_direction[3]);

#endif                                 /* RTW_HEADER_Wrench_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
