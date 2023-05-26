/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Do_Model0_data.c
 *
 * Code generated for Simulink model 'Do_Model0'.
 *
 * Model version                  : 2.63
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Thu May 25 14:18:00 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Do_Model0.h"

/* Invariant block signals (default storage) */
const ConstB_Do_Model0_T Do_Model0_ConstB = {
  -328.32177499999995,                 /* '<S7>/Gravitational Force1' */

  { -0.34600000000000003, 0.406, 0.032 },/* '<S27>/Sum' */

  { 0.437, 0.406, 0.032 },             /* '<S28>/Sum' */

  { -0.34600000000000003, -0.408, 0.032 },/* '<S29>/Sum' */

  { 0.437, -0.408, 0.032 },            /* '<S30>/Sum' */

  { -0.25800000000000006, 0.31700000000000006, 0.039400000000000004 },/* '<S32>/Sum' */

  { 0.378, 0.31700000000000006, 0.039400000000000004 },/* '<S33>/Sum' */

  { -0.25800000000000006, -0.349, 0.039400000000000004 },/* '<S34>/Sum' */

  { 0.378, -0.349, 0.039400000000000004 }/* '<S35>/Sum' */
};

/* Constant parameters (default storage) */
const ConstP_Do_Model0_T Do_Model0_ConstP = {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S27>/HPA Direction'
   *   '<S28>/HPF Direction'
   *   '<S29>/HSA Direction'
   *   '<S30>/HSF Direction'
   */
  { 0.0, 1.57079, 0.0 },

  /* Expression: importedData.robot.thrusters.VPADir
   * Referenced by: '<S32>/VPA Direction'
   */
  { 0.0, 0.0, 0.7854 },

  /* Expression: importedData.robot.thrusters.VPFDir
   * Referenced by: '<S33>/VPF Direction'
   */
  { 0.0, 0.0, 2.35619 },

  /* Expression: importedData.robot.thrusters.VSADir
   * Referenced by: '<S34>/VSA Direction'
   */
  { 0.0, 0.0, -0.7854 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
