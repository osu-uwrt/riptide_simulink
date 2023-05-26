/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Do_Model0.c
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
#include "rtwtypes.h"
#include "Do_Model0_private.h"
#include <math.h>

/* Block signals (default storage) */
B_Do_Model0_T Do_Model0_B;

/* Block states (default storage) */
DW_Do_Model0_T Do_Model0_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Do_Model0_T Do_Model0_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Do_Model0_T Do_Model0_Y;

/* Real-time model */
static RT_MODEL_Do_Model0_T Do_Model0_M_;
RT_MODEL_Do_Model0_T *const Do_Model0_M = &Do_Model0_M_;

/*
 * Output and update for atomic system:
 *    '<S8>/Seperate to Components'
 *    '<S8>/Seperate to World Compents'
 */
void Do_Model0_SeperatetoComponents(const real_T rtu_robotFrameVelocity[3],
  real_T *rty_x, real_T *rty_y, real_T *rty_z)
{
  *rty_x = rtu_robotFrameVelocity[0];
  *rty_y = rtu_robotFrameVelocity[1];
  *rty_z = rtu_robotFrameVelocity[2];
}

/*
 * Output and update for atomic system:
 *    '<S9>/Convert to Column Vector'
 *    '<S9>/Convert to Column Vector1'
 */
void Do_Model0_ConverttoColumnVector(const real_T rtu_row[3], real_T rty_column
  [3])
{
  rty_column[0] = rtu_row[0];
  rty_column[1] = rtu_row[1];
  rty_column[2] = rtu_row[2];
}

/*
 * Output and update for atomic system:
 *    '<S9>/Convert to Row Vector'
 *    '<S9>/Convert to Row Vector1'
 */
void Do_Model0_ConverttoRowVector(const real_T rtu_column[3], real_T rty_row[3])
{
  rty_row[0] = rtu_column[0];
  rty_row[1] = rtu_column[1];
  rty_row[2] = rtu_column[2];
}

/*
 * Output and update for atomic system:
 *    '<S27>/Euler to Unit'
 *    '<S28>/Euler to Unit'
 *    '<S29>/Euler to Unit'
 *    '<S30>/Euler to Unit'
 *    '<S32>/Euler to Unit'
 *    '<S33>/Euler to Unit'
 *    '<S34>/Euler to Unit'
 */
void Do_Model0_EulertoUnit(const real_T rtu_euler[3], real_T rty_direction[3])
{
  real_T a[9];
  real_T ct[3];
  real_T a_tmp;
  real_T ct_0;
  real_T ct_1;
  real_T ct_2;
  real_T st;
  real_T st_0;
  real_T st_1;
  int32_T i;
  ct_0 = cos(rtu_euler[0]);
  st = sin(rtu_euler[0]);
  ct_1 = cos(rtu_euler[1]);
  st_0 = sin(rtu_euler[1]);
  ct_2 = cos(rtu_euler[2]);
  st_1 = sin(rtu_euler[2]);
  a[0] = ct_1 * ct_2;
  a[3] = -ct_1 * st_1;
  a[6] = st_0;
  a_tmp = st * ct_2;
  a[1] = a_tmp * st_0 + ct_0 * st_1;
  ct_2 *= ct_0;
  a[4] = ct_2 - st * st_0 * st_1;
  a[7] = -ct_1 * st;
  a[2] = st * st_1 - ct_2 * st_0;
  a[5] = ct_0 * st_0 * st_1 + a_tmp;
  a[8] = ct_0 * ct_1;
  for (i = 0; i < 3; i++) {
    ct[i] = (a[i + 3] * 0.0 + a[i]) + a[i + 6] * 0.0;
  }

  rty_direction[0] = ct[0];
  rty_direction[1] = ct[1];
  rty_direction[2] = ct[2];
}

/* Model step function */
void Do_Model0_step(void)
{
  real_T a[9];
  real_T rotationToRobot[9];
  real_T rtb_rotation[9];
  real_T rtb_rotation_b[9];
  real_T rtb_rotation_g[9];
  real_T ct_0[6];
  real_T rtb_ElementProduct[6];
  real_T rtb_ElementProduct_bv[6];
  real_T rtb_ElementProduct_dd[6];
  real_T rtb_ElementProduct_e[6];
  real_T rtb_ElementProduct_f[6];
  real_T rtb_ElementProduct_h[6];
  real_T rtb_ElementProduct_lz[6];
  real_T rtb_ElementProduct_m[6];
  real_T rtb_ElementProduct_n[6];
  real_T st_0[6];
  real_T tmp[6];
  real_T tmp_0[6];
  real_T ct[3];
  real_T rtb_ApplyInverseRoation[3];
  real_T rtb_Product[3];
  real_T rtb_Product_m[3];
  real_T rtb_SumMoments[3];
  real_T rtb_column_m[3];
  real_T st[3];
  real_T rtb_Abs1;
  real_T rtb_Abs1_e;
  real_T rtb_Abs2;
  real_T rtb_Abs3;
  real_T rtb_Product_a_idx_0;
  real_T rtb_Product_a_idx_1;
  real_T rtb_Product_a_idx_2;
  real_T rtb_Product_e_idx_0;
  real_T rtb_Product_e_idx_1;
  real_T rtb_Product_e_idx_2;
  real_T rtb_Product_fi_idx_0;
  real_T rtb_Product_fi_idx_1;
  real_T rtb_Product_fi_idx_2;
  real_T rtb_Product_i_idx_0;
  real_T rtb_Product_i_idx_1;
  real_T rtb_Product_i_idx_2;
  real_T rtb_Product_o_idx_0;
  real_T rtb_Product_o_idx_1;
  real_T rtb_Product_o_idx_2;
  real_T rtb_y;
  int32_T k;

  /* Outputs for Atomic SubSystem: '<Root>/Do_Model' */
  /* MATLAB Function: '<S8>/Create Rotation Matrix' incorporates:
   *  Inport: '<Root>/Previous Angular Position'
   *  MATLAB Function: '<S5>/Create Rotation Matrix'
   *  MATLAB Function: '<S9>/To World Frame'
   */
  rtb_Product[0] = 0.017453292519943295 * Do_Model0_U.PreviousAngularPosition[0];
  rtb_Product[1] = 0.017453292519943295 * Do_Model0_U.PreviousAngularPosition[1];
  rtb_Product[2] = 0.017453292519943295 * Do_Model0_U.PreviousAngularPosition[2];
  rtb_Product_o_idx_0 = cos(rtb_Product[0]);
  rtb_Product_o_idx_1 = sin(rtb_Product[0]);
  rtb_Product_o_idx_2 = cos(rtb_Product[1]);
  rtb_Product_a_idx_0 = sin(rtb_Product[1]);
  rtb_Product_a_idx_1 = cos(rtb_Product[2]);
  rtb_Product_a_idx_2 = sin(rtb_Product[2]);
  rtb_Product_i_idx_0 = rtb_Product_o_idx_0 * rtb_Product_o_idx_2;
  rtb_rotation_g[0] = rtb_Product_i_idx_0;
  rtb_Product_i_idx_1 = rtb_Product_a_idx_0 * rtb_Product_a_idx_2;
  rtb_Product_i_idx_2 = rtb_Product_o_idx_1 * rtb_Product_a_idx_1;
  rtb_rotation_g[3] = rtb_Product_i_idx_1 * rtb_Product_o_idx_0 -
    rtb_Product_i_idx_2;
  rtb_Product_fi_idx_0 = rtb_Product_a_idx_0 * rtb_Product_a_idx_1;
  rtb_Product_fi_idx_1 = rtb_Product_o_idx_1 * rtb_Product_a_idx_2;
  rtb_rotation_g[6] = rtb_Product_fi_idx_0 * rtb_Product_o_idx_0 +
    rtb_Product_fi_idx_1;
  rtb_Product_fi_idx_2 = rtb_Product_o_idx_1 * rtb_Product_o_idx_2;
  rtb_rotation_g[1] = rtb_Product_fi_idx_2;
  rtb_Product_e_idx_0 = rtb_Product_o_idx_0 * rtb_Product_a_idx_1;
  rtb_rotation_g[4] = rtb_Product_i_idx_1 * rtb_Product_o_idx_1 +
    rtb_Product_e_idx_0;
  rtb_Product_e_idx_1 = rtb_Product_o_idx_0 * rtb_Product_a_idx_2;
  rtb_rotation_g[7] = rtb_Product_fi_idx_0 * rtb_Product_o_idx_1 -
    rtb_Product_e_idx_1;
  rtb_rotation_g[2] = -rtb_Product_a_idx_0;
  rtb_Product_e_idx_2 = rtb_Product_o_idx_2 * rtb_Product_a_idx_2;
  rtb_rotation_g[5] = rtb_Product_e_idx_2;
  rtb_Product_o_idx_2 *= rtb_Product_a_idx_1;
  rtb_rotation_g[8] = rtb_Product_o_idx_2;

  /* MATLAB Function: '<S9>/To World Frame' incorporates:
   *  MATLAB Function: '<S5>/Create Rotation Matrix'
   */
  st[0] = rtb_Product_o_idx_1;
  st[1] = rtb_Product_a_idx_0;
  st[2] = rtb_Product_a_idx_2;
  rotationToRobot[0] = rtb_Product_i_idx_0;
  rotationToRobot[3] = rtb_Product_i_idx_1 * rtb_Product_o_idx_0 -
    rtb_Product_i_idx_2;
  rotationToRobot[6] = rtb_Product_fi_idx_0 * rtb_Product_o_idx_0 +
    rtb_Product_fi_idx_1;
  rotationToRobot[1] = rtb_Product_fi_idx_2;
  rotationToRobot[4] = rtb_Product_i_idx_1 * rtb_Product_o_idx_1 +
    rtb_Product_e_idx_0;
  rotationToRobot[7] = rtb_Product_fi_idx_0 * rtb_Product_o_idx_1 -
    rtb_Product_e_idx_1;
  rotationToRobot[2] = -rtb_Product_a_idx_0;
  rotationToRobot[5] = rtb_Product_e_idx_2;
  rotationToRobot[8] = rtb_Product_o_idx_2;
  for (k = 0; k < 3; k++) {
    rtb_Product_o_idx_0 = rtb_Product[k];
    rtb_rotation[3 * k] = rotationToRobot[k];
    rtb_rotation[3 * k + 1] = rotationToRobot[k + 3];
    rtb_rotation[3 * k + 2] = rotationToRobot[k + 6];
    rtb_Product[k] = sin(rtb_Product_o_idx_0);
    ct[k] = cos(rtb_Product_o_idx_0);
  }

  /* MATLAB Function: '<S5>/Create Rotation Matrix' */
  rtb_rotation_b[0] = ct[0] * ct[1];
  rtb_Product_o_idx_0 = rtb_Product[1] * rtb_Product[2];
  rtb_rotation_b[3] = rtb_Product_o_idx_0 * ct[0] - rtb_Product[0] * ct[2];
  rtb_Product_o_idx_1 = rtb_Product[1] * ct[2];
  rtb_rotation_b[6] = rtb_Product_o_idx_1 * ct[0] + rtb_Product[0] *
    rtb_Product[2];
  rtb_rotation_b[1] = rtb_Product[0] * ct[1];
  rtb_rotation_b[4] = rtb_Product_o_idx_0 * rtb_Product[0] + ct[0] * ct[2];
  rtb_rotation_b[7] = rtb_Product_o_idx_1 * rtb_Product[0] - ct[0] *
    rtb_Product[2];
  rtb_rotation_b[2] = -rtb_Product[1];
  rtb_rotation_b[5] = ct[1] * rtb_Product[2];
  rtb_rotation_b[8] = ct[1] * ct[2];

  /* MATLAB Function: '<S30>/Euler to Unit' incorporates:
   *  Constant: '<S30>/HSF Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, st);

  /* Product: '<S30>/Product' incorporates:
   *  Gain: '<S30>/Thrust To Acting'
   *  Inport: '<Root>/HSF Force'
   */
  rtb_Product[0] = -Do_Model0_U.x1 * st[0];
  rtb_Product[1] = -Do_Model0_U.x1 * st[1];
  rtb_Product[2] = -Do_Model0_U.x1 * st[2];

  /* Product: '<S42>/Element Product' */
  rtb_ElementProduct[0] = Do_Model0_ConstB.Sum_h[1] * rtb_Product[2];
  rtb_ElementProduct[1] = rtb_Product[0] * Do_Model0_ConstB.Sum_h[2];
  rtb_ElementProduct[2] = Do_Model0_ConstB.Sum_h[0] * rtb_Product[1];
  rtb_ElementProduct[3] = rtb_Product[1] * Do_Model0_ConstB.Sum_h[2];
  rtb_ElementProduct[4] = Do_Model0_ConstB.Sum_h[0] * rtb_Product[2];
  rtb_ElementProduct[5] = rtb_Product[0] * Do_Model0_ConstB.Sum_h[1];

  /* MATLAB Function: '<S29>/Euler to Unit' incorporates:
   *  Constant: '<S29>/HSA Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, st);

  /* Product: '<S29>/Product' incorporates:
   *  Gain: '<S29>/Thrust To Acting'
   *  Inport: '<Root>/HSA Force'
   */
  rtb_Product_m[0] = -Do_Model0_U.x2 * st[0];
  rtb_Product_m[1] = -Do_Model0_U.x2 * st[1];
  rtb_Product_m[2] = -Do_Model0_U.x2 * st[2];

  /* Product: '<S40>/Element Product' */
  rtb_ElementProduct_h[0] = Do_Model0_ConstB.Sum_m[1] * rtb_Product_m[2];
  rtb_ElementProduct_h[1] = rtb_Product_m[0] * Do_Model0_ConstB.Sum_m[2];
  rtb_ElementProduct_h[2] = Do_Model0_ConstB.Sum_m[0] * rtb_Product_m[1];
  rtb_ElementProduct_h[3] = rtb_Product_m[1] * Do_Model0_ConstB.Sum_m[2];
  rtb_ElementProduct_h[4] = Do_Model0_ConstB.Sum_m[0] * rtb_Product_m[2];
  rtb_ElementProduct_h[5] = rtb_Product_m[0] * Do_Model0_ConstB.Sum_m[1];

  /* MATLAB Function: '<S27>/Euler to Unit' incorporates:
   *  Constant: '<S27>/HPA Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, st);

  /* Product: '<S27>/Product' incorporates:
   *  Gain: '<S27>/Thrust To Acting'
   *  Inport: '<Root>/HPA Force'
   */
  rtb_Product_o_idx_0 = -Do_Model0_U.x3 * st[0];
  rtb_Product_o_idx_1 = -Do_Model0_U.x3 * st[1];
  rtb_Product_o_idx_2 = -Do_Model0_U.x3 * st[2];

  /* Product: '<S36>/Element Product' */
  rtb_ElementProduct_m[0] = Do_Model0_ConstB.Sum[1] * rtb_Product_o_idx_2;
  rtb_ElementProduct_m[1] = rtb_Product_o_idx_0 * Do_Model0_ConstB.Sum[2];
  rtb_ElementProduct_m[2] = Do_Model0_ConstB.Sum[0] * rtb_Product_o_idx_1;
  rtb_ElementProduct_m[3] = rtb_Product_o_idx_1 * Do_Model0_ConstB.Sum[2];
  rtb_ElementProduct_m[4] = Do_Model0_ConstB.Sum[0] * rtb_Product_o_idx_2;
  rtb_ElementProduct_m[5] = rtb_Product_o_idx_0 * Do_Model0_ConstB.Sum[1];

  /* MATLAB Function: '<S28>/Euler to Unit' incorporates:
   *  Constant: '<S28>/HPF Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, st);

  /* Product: '<S28>/Product' incorporates:
   *  Gain: '<S28>/Thrust To Acting'
   *  Inport: '<Root>/HPF Force'
   */
  rtb_Product_a_idx_0 = -Do_Model0_U.x4 * st[0];
  rtb_Product_a_idx_1 = -Do_Model0_U.x4 * st[1];
  rtb_Product_a_idx_2 = -Do_Model0_U.x4 * st[2];

  /* Product: '<S38>/Element Product' */
  rtb_ElementProduct_n[0] = Do_Model0_ConstB.Sum_n[1] * rtb_Product_a_idx_2;
  rtb_ElementProduct_n[1] = rtb_Product_a_idx_0 * Do_Model0_ConstB.Sum_n[2];
  rtb_ElementProduct_n[2] = Do_Model0_ConstB.Sum_n[0] * rtb_Product_a_idx_1;
  rtb_ElementProduct_n[3] = rtb_Product_a_idx_1 * Do_Model0_ConstB.Sum_n[2];
  rtb_ElementProduct_n[4] = Do_Model0_ConstB.Sum_n[0] * rtb_Product_a_idx_2;
  rtb_ElementProduct_n[5] = rtb_Product_a_idx_0 * Do_Model0_ConstB.Sum_n[1];

  /* MATLAB Function: '<S35>/Euler to Unit' incorporates:
   *  Constant: '<S35>/VSF Direction'
   */
  st[0] = 0.0;
  st[1] = 0.0;
  st[2] = -0.70710995622487538;
  a[0] = -0.70710360613396317;
  a[3] = 0.70710995622487538;
  a[6] = 0.0;
  a[1] = -0.70710995622487538;
  a[4] = -0.70710360613396317;
  a[7] = -0.0;
  a[2] = 0.0;
  a[5] = -0.0;
  a[8] = 1.0;
  for (k = 0; k < 3; k++) {
    ct[k] = (a[k + 3] * 0.0 + a[k]) + a[k + 6] * 0.0;
  }

  /* Product: '<S35>/Product' incorporates:
   *  Gain: '<S35>/Thrust To Acting'
   *  Inport: '<Root>/VSF Force'
   *  MATLAB Function: '<S35>/Euler to Unit'
   */
  rtb_Product_i_idx_0 = -Do_Model0_U.x5 * ct[0];
  rtb_Product_i_idx_1 = -Do_Model0_U.x5 * ct[1];
  rtb_Product_i_idx_2 = -Do_Model0_U.x5 * ct[2];

  /* Product: '<S50>/Element Product' */
  rtb_ElementProduct_e[0] = Do_Model0_ConstB.Sum_f[1] * rtb_Product_i_idx_2;
  rtb_ElementProduct_e[1] = rtb_Product_i_idx_0 * Do_Model0_ConstB.Sum_f[2];
  rtb_ElementProduct_e[2] = Do_Model0_ConstB.Sum_f[0] * rtb_Product_i_idx_1;
  rtb_ElementProduct_e[3] = rtb_Product_i_idx_1 * Do_Model0_ConstB.Sum_f[2];
  rtb_ElementProduct_e[4] = Do_Model0_ConstB.Sum_f[0] * rtb_Product_i_idx_2;
  rtb_ElementProduct_e[5] = rtb_Product_i_idx_0 * Do_Model0_ConstB.Sum_f[1];

  /* MATLAB Function: '<S34>/Euler to Unit' incorporates:
   *  Constant: '<S34>/VSA Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.VSADirection_Value, st);

  /* Product: '<S34>/Product' incorporates:
   *  Gain: '<S34>/Thrust To Acting'
   *  Inport: '<Root>/VSA Force'
   */
  rtb_Product_fi_idx_0 = -Do_Model0_U.x6 * st[0];
  rtb_Product_fi_idx_1 = -Do_Model0_U.x6 * st[1];
  rtb_Product_fi_idx_2 = -Do_Model0_U.x6 * st[2];

  /* Product: '<S48>/Element Product' */
  rtb_ElementProduct_dd[0] = Do_Model0_ConstB.Sum_c[1] * rtb_Product_fi_idx_2;
  rtb_ElementProduct_dd[1] = rtb_Product_fi_idx_0 * Do_Model0_ConstB.Sum_c[2];
  rtb_ElementProduct_dd[2] = Do_Model0_ConstB.Sum_c[0] * rtb_Product_fi_idx_1;
  rtb_ElementProduct_dd[3] = rtb_Product_fi_idx_1 * Do_Model0_ConstB.Sum_c[2];
  rtb_ElementProduct_dd[4] = Do_Model0_ConstB.Sum_c[0] * rtb_Product_fi_idx_2;
  rtb_ElementProduct_dd[5] = rtb_Product_fi_idx_0 * Do_Model0_ConstB.Sum_c[1];

  /* MATLAB Function: '<S32>/Euler to Unit' incorporates:
   *  Constant: '<S32>/VPA Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.VPADirection_Value, st);

  /* Product: '<S32>/Product' incorporates:
   *  Gain: '<S32>/Thrust To Acting'
   *  Inport: '<Root>/VPA Force'
   */
  rtb_Product_e_idx_0 = -Do_Model0_U.x7 * st[0];
  rtb_Product_e_idx_1 = -Do_Model0_U.x7 * st[1];
  rtb_Product_e_idx_2 = -Do_Model0_U.x7 * st[2];

  /* Product: '<S44>/Element Product' */
  rtb_ElementProduct_lz[0] = Do_Model0_ConstB.Sum_hj[1] * rtb_Product_e_idx_2;
  rtb_ElementProduct_lz[1] = rtb_Product_e_idx_0 * Do_Model0_ConstB.Sum_hj[2];
  rtb_ElementProduct_lz[2] = Do_Model0_ConstB.Sum_hj[0] * rtb_Product_e_idx_1;
  rtb_ElementProduct_lz[3] = rtb_Product_e_idx_1 * Do_Model0_ConstB.Sum_hj[2];
  rtb_ElementProduct_lz[4] = Do_Model0_ConstB.Sum_hj[0] * rtb_Product_e_idx_2;
  rtb_ElementProduct_lz[5] = rtb_Product_e_idx_0 * Do_Model0_ConstB.Sum_hj[1];

  /* MATLAB Function: '<S33>/Euler to Unit' incorporates:
   *  Constant: '<S33>/VPF Direction'
   */
  Do_Model0_EulertoUnit(Do_Model0_ConstP.VPFDirection_Value, rtb_column_m);

  /* Product: '<S33>/Product' incorporates:
   *  Gain: '<S33>/Thrust To Acting'
   *  Inport: '<Root>/VPF Force'
   */
  st[0] = -Do_Model0_U.x8 * rtb_column_m[0];
  st[1] = -Do_Model0_U.x8 * rtb_column_m[1];
  st[2] = -Do_Model0_U.x8 * rtb_column_m[2];

  /* Product: '<S46>/Element Product' */
  tmp[2] = Do_Model0_ConstB.Sum_he[0];
  tmp[3] = Do_Model0_ConstB.Sum_he[2];
  st_0[0] = st[2];
  tmp[0] = Do_Model0_ConstB.Sum_he[1];
  tmp[4] = Do_Model0_ConstB.Sum_he[0];
  st_0[1] = st[0];
  st_0[3] = st[1];
  tmp[1] = Do_Model0_ConstB.Sum_he[2];
  tmp[5] = Do_Model0_ConstB.Sum_he[1];
  st_0[2] = st[1];
  st_0[4] = st[2];
  st_0[5] = st[0];

  /* MATLAB Function: '<S6>/Create Bouancy Vector' incorporates:
   *  Constant: '<S6>/Density of Water'
   *  Constant: '<S6>/Gravitational Acceleration1'
   *  Constant: '<S6>/Point Up'
   *  Constant: '<S6>/Robot Volume'
   *  Inport: '<Root>/Gradient Bouyancy Force'
   *  Product: '<S6>/Bouyant Force'
   */
  ct[2] = 323.428310455 * Do_Model0_U.GradientBouyancyForce;

  /* Product: '<S15>/Element Product' incorporates:
   *  Inport: '<Root>/Gradient Bouyancy Force Position'
   */
  tmp_0[2] = Do_Model0_U.GradientBouyancyForcePosition[0];
  tmp_0[3] = Do_Model0_U.GradientBouyancyForcePosition[2];
  ct_0[0] = ct[2];
  tmp_0[0] = Do_Model0_U.GradientBouyancyForcePosition[1];
  tmp_0[4] = Do_Model0_U.GradientBouyancyForcePosition[0];
  ct_0[1] = 0.0;
  ct_0[3] = 0.0;
  tmp_0[1] = Do_Model0_U.GradientBouyancyForcePosition[2];
  tmp_0[5] = Do_Model0_U.GradientBouyancyForcePosition[1];
  ct_0[2] = 0.0;
  ct_0[4] = ct[2];
  ct_0[5] = 0.0;
  for (k = 0; k < 6; k++) {
    /* Product: '<S46>/Element Product' */
    rtb_ElementProduct_f[k] = tmp[k] * st_0[k];

    /* Product: '<S15>/Element Product' */
    rtb_ElementProduct_bv[k] = tmp_0[k] * ct_0[k];
  }

  /* MATLAB Function: '<S5>/Concatenate Velocity' incorporates:
   *  Inport: '<Root>/Previous Angular Velocity'
   */
  rtb_Abs1 = Do_Model0_U.PreviousAngularVelocity[0];
  rtb_Abs3 = Do_Model0_U.PreviousAngularVelocity[1];
  rtb_y = Do_Model0_U.PreviousAngularVelocity[2];

  /* Product: '<S5>/Apply Rotation' */
  for (k = 0; k < 3; k++) {
    rtb_column_m[k] = (rtb_rotation_b[k + 3] * rtb_Abs3 + rtb_rotation_b[k] *
                       rtb_Abs1) + rtb_rotation_b[k + 6] * rtb_y;
  }

  /* End of Product: '<S5>/Apply Rotation' */

  /* Abs: '<S5>/Abs1' incorporates:
   *  MATLAB Function: '<S5>/Seperate to Components'
   *  Product: '<S5>/Apply Rotation'
   */
  rtb_Abs1 = fabs(rtb_column_m[0]);

  /* Abs: '<S5>/Abs2' incorporates:
   *  MATLAB Function: '<S5>/Seperate to Components'
   *  Product: '<S5>/Apply Rotation'
   */
  rtb_Abs2 = fabs(rtb_column_m[1]);

  /* Switch: '<S5>/Ensure Opposite Sign of Velocity Y' incorporates:
   *  Constant: '<S5>/Constant10'
   *  Constant: '<S5>/Constant11'
   *  MATLAB Function: '<S5>/Seperate to Components'
   *  Product: '<S5>/Apply Rotation'
   */
  if (rtb_column_m[1] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S5>/Set Y Drag Sign' incorporates:
   *  Polyval: '<S5>/Calculate Y Drag'
   *  Switch: '<S5>/Ensure Opposite Sign of Velocity Y'
   */
  rtb_Abs2 = (-12.0 * rtb_Abs2 - 4.5) * rtb_Abs2 * (real_T)k;

  /* Abs: '<S5>/Abs3' incorporates:
   *  MATLAB Function: '<S5>/Seperate to Components'
   *  Product: '<S5>/Apply Rotation'
   */
  rtb_Abs3 = fabs(rtb_column_m[2]);

  /* MATLAB Function: '<S5>/Concatenate Drag' incorporates:
   *  Constant: '<S5>/Constant12'
   *  Constant: '<S5>/Constant13'
   *  Constant: '<S5>/Constant8'
   *  Constant: '<S5>/Constant9'
   *  MATLAB Function: '<S5>/Seperate to Components'
   *  Polyval: '<S5>/Calculate X Drag'
   *  Polyval: '<S5>/Calculate Z Drag'
   *  Product: '<S5>/Apply Rotation'
   *  Product: '<S5>/Set X Drag Sign'
   *  Product: '<S5>/Set Z Drag Sign'
   *  Switch: '<S5>/Ensure Opposite Sign of Velocity X'
   *  Switch: '<S5>/Ensure Opposite Sign of Velocity Z'
   */
  if (rtb_column_m[0] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs1 = (-6.0 * rtb_Abs1 - 2.0) * rtb_Abs1 * (real_T)k;
  if (rtb_column_m[2] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs3 = (-12.0 * rtb_Abs3 - 2.5) * rtb_Abs3 * (real_T)k;
  for (k = 0; k < 3; k++) {
    /* Sum: '<S46>/Sum' */
    rtb_y = rtb_ElementProduct_f[k] - rtb_ElementProduct_f[k + 3];

    /* Sum: '<S9>/Sum Moments' incorporates:
     *  Sum: '<S36>/Sum'
     *  Sum: '<S38>/Sum'
     *  Sum: '<S40>/Sum'
     *  Sum: '<S42>/Sum'
     *  Sum: '<S44>/Sum'
     *  Sum: '<S48>/Sum'
     *  Sum: '<S50>/Sum'
     */
    rtb_SumMoments[k] = (((((((rtb_ElementProduct[k] - rtb_ElementProduct[k + 3])
      + (rtb_ElementProduct_h[k] - rtb_ElementProduct_h[k + 3])) +
      (rtb_ElementProduct_m[k] - rtb_ElementProduct_m[k + 3])) +
      (rtb_ElementProduct_n[k] - rtb_ElementProduct_n[k + 3])) +
      (rtb_ElementProduct_e[k] - rtb_ElementProduct_e[k + 3])) +
                          (rtb_ElementProduct_dd[k] - rtb_ElementProduct_dd[k +
      3])) + (rtb_ElementProduct_lz[k] - rtb_ElementProduct_lz[k + 3])) + rtb_y;
    rtb_column_m[k] = rtb_y;

    /* Product: '<S5>/Apply Inverse Roation' incorporates:
     *  MATLAB Function: '<S5>/Concatenate Drag'
     *  MATLAB Function: '<S5>/Create Rotation Matrix'
     */
    rtb_ApplyInverseRoation[k] = (rtb_rotation_b[3 * k + 1] * rtb_Abs2 +
      rtb_rotation_b[3 * k] * rtb_Abs1) + rtb_rotation_b[3 * k + 2] * rtb_Abs3;
  }

  /* MATLAB Function: '<S9>/Convert to Column Vector1' */
  Do_Model0_ConverttoColumnVector(rtb_SumMoments, rtb_column_m);
  for (k = 0; k < 3; k++) {
    /* Product: '<S9>/Convert to World Frame 1' */
    rtb_SumMoments[k] = (rtb_rotation[k + 3] * rtb_column_m[1] + rtb_rotation[k]
                         * rtb_column_m[0]) + rtb_rotation[k + 6] *
      rtb_column_m[2];
  }

  /* MATLAB Function: '<S9>/Convert to Row Vector1' */
  Do_Model0_ConverttoRowVector(rtb_SumMoments, rtb_column_m);

  /* Clock: '<S4>/Clock' */
  Do_Model0_B.Clock = Do_Model0_M->Timing.t[0];

  /* Sum: '<S4>/Delta Time' incorporates:
   *  Memory: '<S4>/Memory1'
   */
  rtb_Abs2 = Do_Model0_B.Clock - Do_Model0_DW.Memory1_PreviousInput;

  /* Sum: '<S2>/Current Angular Velocity' incorporates:
   *  Constant: '<S2>/Moments of Inertia'
   *  Inport: '<Root>/Previous Angular Velocity'
   *  MATLAB Function: '<S5>/Seperate to World Compents'
   *  Product: '<S2>/Angular Acceleration'
   *  Product: '<S2>/Change in Angular Velocity'
   *  Product: '<S5>/Apply Inverse Roation'
   *  Sum: '<S15>/Sum'
   *  Sum: '<S3>/Moments'
   */
  Do_Model0_Y.AngularVelocity[0] = ((rtb_ApplyInverseRoation[0] + rtb_column_m[0])
    + (rtb_ElementProduct_bv[0] - rtb_ElementProduct_bv[3])) / 0.9978 * rtb_Abs2
    + Do_Model0_U.PreviousAngularVelocity[0];
  Do_Model0_Y.AngularVelocity[1] = ((rtb_ApplyInverseRoation[1] + rtb_column_m[1])
    + (rtb_ElementProduct_bv[1] - rtb_ElementProduct_bv[4])) / 1.6934 * rtb_Abs2
    + Do_Model0_U.PreviousAngularVelocity[1];
  Do_Model0_Y.AngularVelocity[2] = ((rtb_ApplyInverseRoation[2] + rtb_column_m[2])
    + (rtb_ElementProduct_bv[2] - rtb_ElementProduct_bv[5])) / 2.1284 * rtb_Abs2
    + Do_Model0_U.PreviousAngularVelocity[2];

  /* MATLAB Function: '<S8>/Row to Column Vector' incorporates:
   *  Inport: '<Root>/Previous Linear Velocity'
   */
  rtb_Abs1 = Do_Model0_U.PreviousLinearVelocity[0];
  rtb_Abs3 = Do_Model0_U.PreviousLinearVelocity[1];
  rtb_y = Do_Model0_U.PreviousLinearVelocity[2];
  for (k = 0; k < 3; k++) {
    /* Product: '<S8>/Apply Rotation' */
    rtb_ApplyInverseRoation[k] = (rtb_rotation_g[k + 3] * rtb_Abs3 +
      rtb_rotation_g[k] * rtb_Abs1) + rtb_rotation_g[k + 6] * rtb_y;
  }

  /* MATLAB Function: '<S8>/Seperate to Components' */
  Do_Model0_SeperatetoComponents(rtb_ApplyInverseRoation, &rtb_Abs3, &rtb_y,
    &rtb_Abs1);

  /* Abs: '<S8>/Abs1' */
  rtb_Abs1_e = fabs(rtb_Abs3);

  /* Switch: '<S8>/Ensure Opposite Sign of Velocity X' incorporates:
   *  Constant: '<S8>/Constant8'
   *  Constant: '<S8>/Constant9'
   */
  if (rtb_Abs3 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S8>/Set X Drag Sign' incorporates:
   *  Polyval: '<S8>/Calculate X Drag'
   *  Switch: '<S8>/Ensure Opposite Sign of Velocity X'
   */
  rtb_Abs3 = (-180.0 * rtb_Abs1_e - 17.0) * rtb_Abs1_e * (real_T)k;

  /* Abs: '<S8>/Abs2' */
  rtb_Abs1_e = fabs(rtb_y);

  /* Switch: '<S8>/Ensure Opposite Sign of Velocity Y' incorporates:
   *  Constant: '<S8>/Constant10'
   *  Constant: '<S8>/Constant11'
   */
  if (rtb_y >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S8>/Set Y Drag Sign' incorporates:
   *  Polyval: '<S8>/Calculate Y Drag'
   *  Switch: '<S8>/Ensure Opposite Sign of Velocity Y'
   */
  rtb_y = (-180.0 * rtb_Abs1_e - 17.0) * rtb_Abs1_e * (real_T)k;

  /* Abs: '<S8>/Abs3' */
  rtb_Abs1_e = fabs(rtb_Abs1);

  /* MATLAB Function: '<S8>/Concatenate Drag' incorporates:
   *  Constant: '<S8>/Constant12'
   *  Constant: '<S8>/Constant13'
   *  Polyval: '<S8>/Calculate Z Drag'
   *  Product: '<S8>/Set Z Drag Sign'
   *  Switch: '<S8>/Ensure Opposite Sign of Velocity Z'
   */
  if (rtb_Abs1 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs1 = (-460.0 * rtb_Abs1_e - 35.0) * rtb_Abs1_e * (real_T)k;
  for (k = 0; k < 3; k++) {
    /* Product: '<S8>/Apply Inverse Roation' incorporates:
     *  MATLAB Function: '<S8>/Concatenate Drag'
     *  MATLAB Function: '<S8>/Create Rotation Matrix'
     */
    rtb_ApplyInverseRoation[k] = (rtb_rotation_g[3 * k + 1] * rtb_y +
      rtb_rotation_g[3 * k] * rtb_Abs3) + rtb_rotation_g[3 * k + 2] * rtb_Abs1;
  }

  /* MATLAB Function: '<S8>/Seperate to World Compents' */
  Do_Model0_SeperatetoComponents(rtb_ApplyInverseRoation, &rtb_Abs1, &rtb_Abs3,
    &rtb_y);

  /* Sum: '<S9>/Sum Forces' incorporates:
   *  Product: '<S27>/Product'
   *  Product: '<S28>/Product'
   *  Product: '<S29>/Product'
   *  Product: '<S30>/Product'
   *  Product: '<S32>/Product'
   *  Product: '<S34>/Product'
   *  Product: '<S35>/Product'
   */
  rtb_Product[0] = ((((((rtb_Product[0] + rtb_Product_m[0]) +
                        rtb_Product_o_idx_0) + rtb_Product_a_idx_0) +
                      rtb_Product_i_idx_0) + rtb_Product_fi_idx_0) +
                    rtb_Product_e_idx_0) + st[0];
  rtb_Product[1] = ((((((rtb_Product[1] + rtb_Product_m[1]) +
                        rtb_Product_o_idx_1) + rtb_Product_a_idx_1) +
                      rtb_Product_i_idx_1) + rtb_Product_fi_idx_1) +
                    rtb_Product_e_idx_1) + st[1];
  rtb_Product[2] = ((((((rtb_Product[2] + rtb_Product_m[2]) +
                        rtb_Product_o_idx_2) + rtb_Product_a_idx_2) +
                      rtb_Product_i_idx_2) + rtb_Product_fi_idx_2) +
                    rtb_Product_e_idx_2) + st[2];

  /* MATLAB Function: '<S9>/Convert to Column Vector' */
  Do_Model0_ConverttoColumnVector(rtb_Product, st);
  for (k = 0; k < 3; k++) {
    /* Product: '<S9>/Convert to World Frame' */
    rtb_Product[k] = (rtb_rotation[k + 3] * st[1] + rtb_rotation[k] * st[0]) +
      rtb_rotation[k + 6] * st[2];
  }

  /* MATLAB Function: '<S9>/Convert to Row Vector' */
  Do_Model0_ConverttoRowVector(rtb_Product, rtb_Product_m);

  /* Product: '<S2>/Acceleration' incorporates:
   *  Constant: '<S2>/Robot Mass'
   *  Sum: '<S3>/Sum'
   */
  Do_Model0_Y.LinearAcceleration[0] = (rtb_Abs1 + rtb_Product_m[0]) / 33.5;

  /* Sum: '<S2>/Current Velocity' incorporates:
   *  Inport: '<Root>/Previous Linear Velocity'
   *  Product: '<S2>/Acceleration'
   *  Product: '<S2>/Change in Velocity'
   */
  Do_Model0_Y.LinearVelocity[0] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[0] +
    Do_Model0_U.PreviousLinearVelocity[0];

  /* Outport: '<Root>/Linear Position' incorporates:
   *  Inport: '<Root>/Previous Linear Position'
   *  Product: '<S2>/Change in Position'
   *  Sum: '<S2>/Current Position'
   *  Sum: '<S2>/Current Velocity'
   */
  Do_Model0_Y.LinearPosition[0] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[0] +
    Do_Model0_U.PreviousLinearPosition[0];

  /* Outport: '<Root>/Angular Position' incorporates:
   *  Inport: '<Root>/Previous Angular Position'
   *  Product: '<S2>/Change in Angular Position'
   *  Sum: '<S2>/Current Angular Position'
   *  Sum: '<S2>/Current Angular Velocity'
   */
  Do_Model0_Y.AngularPosition[0] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[0] +
    Do_Model0_U.PreviousAngularPosition[0];

  /* Product: '<S2>/Acceleration' incorporates:
   *  Constant: '<S2>/Robot Mass'
   *  Sum: '<S3>/Sum'
   */
  Do_Model0_Y.LinearAcceleration[1] = (rtb_Abs3 + rtb_Product_m[1]) / 33.5;

  /* Sum: '<S2>/Current Velocity' incorporates:
   *  Inport: '<Root>/Previous Linear Velocity'
   *  Product: '<S2>/Acceleration'
   *  Product: '<S2>/Change in Velocity'
   */
  Do_Model0_Y.LinearVelocity[1] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[1] +
    Do_Model0_U.PreviousLinearVelocity[1];

  /* Outport: '<Root>/Linear Position' incorporates:
   *  Inport: '<Root>/Previous Linear Position'
   *  Product: '<S2>/Change in Position'
   *  Sum: '<S2>/Current Position'
   *  Sum: '<S2>/Current Velocity'
   */
  Do_Model0_Y.LinearPosition[1] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[1] +
    Do_Model0_U.PreviousLinearPosition[1];

  /* Outport: '<Root>/Angular Position' incorporates:
   *  Inport: '<Root>/Previous Angular Position'
   *  Product: '<S2>/Change in Angular Position'
   *  Sum: '<S2>/Current Angular Position'
   *  Sum: '<S2>/Current Angular Velocity'
   */
  Do_Model0_Y.AngularPosition[1] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[1] +
    Do_Model0_U.PreviousAngularPosition[1];

  /* Product: '<S2>/Acceleration' incorporates:
   *  Constant: '<S2>/Robot Mass'
   *  MATLAB Function: '<S7>/Create Vector'
   *  Sum: '<S3>/Sum'
   */
  Do_Model0_Y.LinearAcceleration[2] = (((rtb_y +
    Do_Model0_ConstB.GravitationalForce1) + rtb_Product_m[2]) + ct[2]) / 33.5;

  /* Sum: '<S2>/Current Velocity' incorporates:
   *  Inport: '<Root>/Previous Linear Velocity'
   *  Product: '<S2>/Acceleration'
   *  Product: '<S2>/Change in Velocity'
   */
  Do_Model0_Y.LinearVelocity[2] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[2] +
    Do_Model0_U.PreviousLinearVelocity[2];

  /* Outport: '<Root>/Linear Position' incorporates:
   *  Inport: '<Root>/Previous Linear Position'
   *  Product: '<S2>/Change in Position'
   *  Sum: '<S2>/Current Position'
   *  Sum: '<S2>/Current Velocity'
   */
  Do_Model0_Y.LinearPosition[2] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[2] +
    Do_Model0_U.PreviousLinearPosition[2];

  /* Outport: '<Root>/Angular Position' incorporates:
   *  Inport: '<Root>/Previous Angular Position'
   *  Product: '<S2>/Change in Angular Position'
   *  Sum: '<S2>/Current Angular Position'
   *  Sum: '<S2>/Current Angular Velocity'
   */
  Do_Model0_Y.AngularPosition[2] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[2] +
    Do_Model0_U.PreviousAngularPosition[2];

  /* End of Outputs for SubSystem: '<Root>/Do_Model' */

  /* Update for Atomic SubSystem: '<Root>/Do_Model' */
  /* Update for Memory: '<S4>/Memory1' */
  Do_Model0_DW.Memory1_PreviousInput = Do_Model0_B.Clock;

  /* End of Update for SubSystem: '<Root>/Do_Model' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  Do_Model0_M->Timing.t[0] =
    ((time_T)(++Do_Model0_M->Timing.clockTick0)) * Do_Model0_M->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.05s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.05, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    Do_Model0_M->Timing.clockTick1++;
  }
}

/* Model initialize function */
void Do_Model0_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Do_Model0_M->solverInfo,
                          &Do_Model0_M->Timing.simTimeStep);
    rtsiSetTPtr(&Do_Model0_M->solverInfo, &rtmGetTPtr(Do_Model0_M));
    rtsiSetStepSizePtr(&Do_Model0_M->solverInfo, &Do_Model0_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Do_Model0_M->solverInfo, (&rtmGetErrorStatus
      (Do_Model0_M)));
    rtsiSetRTModelPtr(&Do_Model0_M->solverInfo, Do_Model0_M);
  }

  rtsiSetSimTimeStep(&Do_Model0_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Do_Model0_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Do_Model0_M, &Do_Model0_M->Timing.tArray[0]);
  Do_Model0_M->Timing.stepSize0 = 0.05;
}

/* Model terminate function */
void Do_Model0_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
