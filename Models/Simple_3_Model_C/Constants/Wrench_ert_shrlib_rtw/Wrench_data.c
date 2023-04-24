/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Wrench_data.c
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

#include "Wrench.h"

/* Invariant block signals (default storage) */
const ConstB_Wrench_T Wrench_ConstB = {
  -328.32177499999995,                 /* '<S4>/Gravitational Force1' */

  { -0.35150000000000003, 0.386, 0.034 },/* '<S24>/Sum' */

  { 0.4315, 0.386, 0.034 },            /* '<S25>/Sum' */

  { -0.35150000000000003, -0.428, 0.034 },/* '<S26>/Sum' */

  { 0.4315, -0.428, 0.034 },           /* '<S27>/Sum' */

  { -0.26350000000000007, 0.29700000000000004, 0.041400000000000006 },/* '<S29>/Sum' */

  { 0.3725, 0.29700000000000004, 0.041400000000000006 },/* '<S30>/Sum' */

  { -0.26350000000000007, -0.369, 0.041400000000000006 },/* '<S31>/Sum' */

  { 0.3725, -0.369, 0.041400000000000006 }/* '<S32>/Sum' */
};

/* Constant parameters (default storage) */
const ConstP_Wrench_T Wrench_ConstP = {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S24>/HPA Direction'
   *   '<S25>/HPF Direction'
   *   '<S26>/HSA Direction'
   *   '<S27>/HSF Direction'
   */
  { 0.0, 1.57079, 0.0 },

  /* Expression: importedData.robot.thrusters.VPADir
   * Referenced by: '<S29>/VPA Direction'
   */
  { 0.0, 0.0, 0.7854 },

  /* Expression: importedData.robot.thrusters.VPFDir
   * Referenced by: '<S30>/VPF Direction'
   */
  { 0.0, 0.0, 2.35619 },

  /* Expression: importedData.robot.thrusters.VSADir
   * Referenced by: '<S31>/VSA Direction'
   */
  { 0.0, 0.0, -0.7854 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
