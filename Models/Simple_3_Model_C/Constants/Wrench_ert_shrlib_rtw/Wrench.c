/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Wrench.c
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
#include "rtwtypes.h"
#include "Wrench_private.h"
#include <math.h>
#include <emmintrin.h>

/* External inputs (root inport signals with default storage) */
ExtU_Wrench_T Wrench_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Wrench_T Wrench_Y;

/* Real-time model */
static RT_MODEL_Wrench_T Wrench_M_;
RT_MODEL_Wrench_T *const Wrench_M = &Wrench_M_;

/*
 * Output and update for atomic system:
 *    '<S5>/Seperate to Components'
 *    '<S5>/Seperate to World Compents'
 */
void Wrench_SeperatetoComponents(const real_T rtu_robotFrameVelocity[3], real_T *
  rty_x, real_T *rty_y, real_T *rty_z)
{
  *rty_x = rtu_robotFrameVelocity[0];
  *rty_y = rtu_robotFrameVelocity[1];
  *rty_z = rtu_robotFrameVelocity[2];
}

/*
 * Output and update for atomic system:
 *    '<S6>/Convert to Column Vector'
 *    '<S6>/Convert to Column Vector1'
 */
void Wrench_ConverttoColumnVector(const real_T rtu_row[3], real_T rty_column[3])
{
  rty_column[0] = rtu_row[0];
  rty_column[1] = rtu_row[1];
  rty_column[2] = rtu_row[2];
}

/*
 * Output and update for atomic system:
 *    '<S6>/Convert to Row Vector'
 *    '<S6>/Convert to Row Vector1'
 */
void Wrench_ConverttoRowVector(const real_T rtu_column[3], real_T rty_row[3])
{
  rty_row[0] = rtu_column[0];
  rty_row[1] = rtu_column[1];
  rty_row[2] = rtu_column[2];
}

/*
 * Output and update for atomic system:
 *    '<S24>/Euler to Unit'
 *    '<S25>/Euler to Unit'
 *    '<S26>/Euler to Unit'
 *    '<S27>/Euler to Unit'
 *    '<S29>/Euler to Unit'
 *    '<S30>/Euler to Unit'
 *    '<S31>/Euler to Unit'
 */
void Wrench_EulertoUnit(const real_T rtu_euler[3], real_T rty_direction[3])
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
  for (i = 0; i <= 0; i += 2) {
    __m128d tmp;
    __m128d tmp_0;
    __m128d tmp_1;
    __m128d tmp_2;
    tmp = _mm_loadu_pd(&a[i]);
    tmp_2 = _mm_set1_pd(0.0);
    tmp_0 = _mm_loadu_pd(&a[i + 3]);
    tmp_1 = _mm_loadu_pd(&a[i + 6]);
    _mm_storeu_pd(&ct[i], _mm_add_pd(_mm_mul_pd(tmp_1, tmp_2), _mm_add_pd
      (_mm_mul_pd(tmp_0, tmp_2), _mm_add_pd(tmp_2, tmp))));
  }

  for (i = 2; i < 3; i++) {
    ct[i] = (a[i + 3] * 0.0 + a[i]) + a[i + 6] * 0.0;
  }

  rty_direction[0] = ct[0];
  rty_direction[1] = ct[1];
  rty_direction[2] = ct[2];
}

/* Model step function */
void Wrench_step(void)
{
  __m128d tmp_1;
  __m128d tmp_2;
  __m128d tmp_4;
  real_T a[9];
  real_T rotationToRobot[9];
  real_T rtb_rotation[9];
  real_T rtb_rotation_c[9];
  real_T rtb_rotation_k[9];
  real_T ct_0[6];
  real_T rtb_ElementProduct_fu[6];
  real_T tmp[6];
  real_T ct[3];
  real_T rtb_Sum_e[3];
  real_T st[3];
  real_T ct_tmp;
  real_T ct_tmp_0;
  real_T ct_tmp_1;
  real_T rtb_Abs1;
  real_T rtb_Abs1_j;
  real_T rtb_Abs2;
  real_T rtb_Abs3;
  real_T rtb_ApplyRotation_idx_2;
  real_T rtb_ElementProduct_idx_0;
  real_T rtb_ElementProduct_idx_1;
  real_T rtb_ElementProduct_idx_2;
  real_T rtb_ElementProduct_idx_3;
  real_T rtb_ElementProduct_idx_4;
  real_T rtb_ElementProduct_idx_5;
  real_T rtb_Sum_b_idx_0;
  real_T rtb_Sum_b_idx_1;
  real_T rtb_Sum_b_idx_2;
  real_T rtb_Sum_d4_idx_0;
  real_T rtb_Sum_d4_idx_1;
  real_T rtb_Sum_d4_idx_2;
  real_T rtb_Sum_d_idx_0;
  real_T rtb_Sum_d_idx_1;
  real_T rtb_Sum_d_idx_2;
  real_T rtb_Sum_eg_idx_0;
  real_T rtb_Sum_eg_idx_1;
  real_T rtb_Sum_eg_idx_2;
  real_T rtb_Sum_eh_idx_0;
  real_T rtb_Sum_eh_idx_1;
  real_T rtb_Sum_eh_idx_2;
  real_T rtb_Sum_l_idx_0;
  real_T rtb_Sum_l_idx_1;
  real_T rtb_Sum_l_idx_2;
  real_T rtb_bouyantForce_idx_2;
  real_T tmp_0;
  int32_T k;

  /* MATLAB Function: '<S5>/Create Rotation Matrix' incorporates:
   *  Inport: '<Root>/Angular Position'
   *  MATLAB Function: '<S2>/Create Rotation Matrix'
   *  MATLAB Function: '<S6>/To World Frame'
   */
  rtb_Sum_e[0] = 0.017453292519943295 * Wrench_U.AngularPosition[0];
  rtb_Sum_e[1] = 0.017453292519943295 * Wrench_U.AngularPosition[1];
  rtb_Sum_e[2] = 0.017453292519943295 * Wrench_U.AngularPosition[2];
  ct_tmp = cos(rtb_Sum_e[0]);
  rtb_ElementProduct_idx_1 = sin(rtb_Sum_e[0]);
  ct_tmp_0 = cos(rtb_Sum_e[1]);
  rtb_ElementProduct_idx_2 = sin(rtb_Sum_e[1]);
  ct_tmp_1 = cos(rtb_Sum_e[2]);
  rtb_ElementProduct_idx_4 = sin(rtb_Sum_e[2]);
  rtb_ElementProduct_idx_0 = ct_tmp * ct_tmp_0;
  rtb_rotation_k[0] = rtb_ElementProduct_idx_0;
  rtb_ElementProduct_idx_3 = rtb_ElementProduct_idx_2 * rtb_ElementProduct_idx_4;
  rtb_ElementProduct_idx_5 = rtb_ElementProduct_idx_1 * ct_tmp_1;
  rtb_rotation_k[3] = rtb_ElementProduct_idx_3 * ct_tmp -
    rtb_ElementProduct_idx_5;
  rtb_Sum_eg_idx_0 = rtb_ElementProduct_idx_2 * ct_tmp_1;
  rtb_Sum_eg_idx_1 = rtb_ElementProduct_idx_1 * rtb_ElementProduct_idx_4;
  rtb_rotation_k[6] = rtb_Sum_eg_idx_0 * ct_tmp + rtb_Sum_eg_idx_1;
  rtb_Sum_eg_idx_2 = rtb_ElementProduct_idx_1 * ct_tmp_0;
  rtb_rotation_k[1] = rtb_Sum_eg_idx_2;
  rtb_Sum_b_idx_0 = ct_tmp * ct_tmp_1;
  rtb_rotation_k[4] = rtb_ElementProduct_idx_3 * rtb_ElementProduct_idx_1 +
    rtb_Sum_b_idx_0;
  rtb_Sum_b_idx_1 = ct_tmp * rtb_ElementProduct_idx_4;
  rtb_rotation_k[7] = rtb_Sum_eg_idx_0 * rtb_ElementProduct_idx_1 -
    rtb_Sum_b_idx_1;
  rtb_rotation_k[2] = -rtb_ElementProduct_idx_2;
  rtb_ElementProduct_idx_4 *= ct_tmp_0;
  rtb_rotation_k[5] = rtb_ElementProduct_idx_4;
  ct_tmp_0 *= ct_tmp_1;
  rtb_rotation_k[8] = ct_tmp_0;

  /* MATLAB Function: '<S6>/To World Frame' incorporates:
   *  MATLAB Function: '<S2>/Create Rotation Matrix'
   */
  rotationToRobot[0] = rtb_ElementProduct_idx_0;
  rotationToRobot[3] = rtb_ElementProduct_idx_3 * ct_tmp -
    rtb_ElementProduct_idx_5;
  rotationToRobot[6] = rtb_Sum_eg_idx_0 * ct_tmp + rtb_Sum_eg_idx_1;
  rotationToRobot[1] = rtb_Sum_eg_idx_2;
  rotationToRobot[4] = rtb_ElementProduct_idx_3 * rtb_ElementProduct_idx_1 +
    rtb_Sum_b_idx_0;
  rotationToRobot[7] = rtb_Sum_eg_idx_0 * rtb_ElementProduct_idx_1 -
    rtb_Sum_b_idx_1;
  rotationToRobot[2] = -rtb_ElementProduct_idx_2;
  rotationToRobot[5] = rtb_ElementProduct_idx_4;
  rotationToRobot[8] = ct_tmp_0;
  for (k = 0; k < 3; k++) {
    ct_tmp = rtb_Sum_e[k];
    rtb_rotation[3 * k] = rotationToRobot[k];
    rtb_rotation[3 * k + 1] = rotationToRobot[k + 3];
    rtb_rotation[3 * k + 2] = rotationToRobot[k + 6];
    rtb_Sum_e[k] = sin(ct_tmp);
    ct[k] = cos(ct_tmp);
  }

  /* MATLAB Function: '<S2>/Create Rotation Matrix' */
  rtb_rotation_c[0] = ct[0] * ct[1];
  ct_tmp = rtb_Sum_e[1] * rtb_Sum_e[2];
  rtb_rotation_c[3] = ct_tmp * ct[0] - rtb_Sum_e[0] * ct[2];
  ct_tmp_0 = rtb_Sum_e[1] * ct[2];
  rtb_rotation_c[6] = ct_tmp_0 * ct[0] + rtb_Sum_e[0] * rtb_Sum_e[2];
  rtb_rotation_c[1] = rtb_Sum_e[0] * ct[1];
  rtb_rotation_c[4] = ct_tmp * rtb_Sum_e[0] + ct[0] * ct[2];
  rtb_rotation_c[7] = ct_tmp_0 * rtb_Sum_e[0] - ct[0] * rtb_Sum_e[2];
  rtb_rotation_c[2] = -rtb_Sum_e[1];
  rtb_rotation_c[5] = ct[1] * rtb_Sum_e[2];
  rtb_rotation_c[8] = ct[1] * ct[2];

  /* MATLAB Function: '<S27>/Euler to Unit' incorporates:
   *  Constant: '<S27>/HSF Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.pooled5, ct);

  /* Product: '<S27>/Product' incorporates:
   *  Gain: '<S27>/Thrust To Acting'
   *  Inport: '<Root>/HSF Force'
   *  Sum: '<S43>/Sum'
   */
  rtb_Sum_e[0] = -Wrench_U.x1 * ct[0];
  rtb_Sum_e[1] = -Wrench_U.x1 * ct[1];
  rtb_Sum_e[2] = -Wrench_U.x1 * ct[2];

  /* Product: '<S39>/Element Product' */
  rtb_ElementProduct_idx_0 = Wrench_ConstB.Sum_k[1] * rtb_Sum_e[2];
  rtb_ElementProduct_idx_1 = rtb_Sum_e[0] * Wrench_ConstB.Sum_k[2];
  rtb_ElementProduct_idx_2 = Wrench_ConstB.Sum_k[0] * rtb_Sum_e[1];
  rtb_ElementProduct_idx_3 = rtb_Sum_e[1] * Wrench_ConstB.Sum_k[2];
  rtb_ElementProduct_idx_4 = Wrench_ConstB.Sum_k[0] * rtb_Sum_e[2];
  rtb_ElementProduct_idx_5 = rtb_Sum_e[0] * Wrench_ConstB.Sum_k[1];

  /* MATLAB Function: '<S26>/Euler to Unit' incorporates:
   *  Constant: '<S26>/HSA Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.pooled5, ct);

  /* Product: '<S26>/Product' incorporates:
   *  Gain: '<S26>/Thrust To Acting'
   *  Inport: '<Root>/HSA Force'
   *  Sum: '<S41>/Sum'
   */
  rtb_Sum_eg_idx_0 = -Wrench_U.x2 * ct[0];
  rtb_Sum_eg_idx_1 = -Wrench_U.x2 * ct[1];
  rtb_Sum_eg_idx_2 = -Wrench_U.x2 * ct[2];

  /* MATLAB Function: '<S24>/Euler to Unit' incorporates:
   *  Constant: '<S24>/HPA Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.pooled5, ct);

  /* Product: '<S24>/Product' incorporates:
   *  Gain: '<S24>/Thrust To Acting'
   *  Inport: '<Root>/HPA Force'
   *  Sum: '<S45>/Sum'
   */
  rtb_Sum_b_idx_0 = -Wrench_U.x3 * ct[0];
  rtb_Sum_b_idx_1 = -Wrench_U.x3 * ct[1];
  rtb_Sum_b_idx_2 = -Wrench_U.x3 * ct[2];

  /* MATLAB Function: '<S25>/Euler to Unit' incorporates:
   *  Constant: '<S25>/HPF Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.pooled5, ct);

  /* Product: '<S25>/Product' incorporates:
   *  Gain: '<S25>/Thrust To Acting'
   *  Inport: '<Root>/HPF Force'
   *  Sum: '<S47>/Sum'
   */
  rtb_Sum_eh_idx_0 = -Wrench_U.x4 * ct[0];
  rtb_Sum_eh_idx_1 = -Wrench_U.x4 * ct[1];
  rtb_Sum_eh_idx_2 = -Wrench_U.x4 * ct[2];

  /* MATLAB Function: '<S32>/Euler to Unit' */
  a[0] = -0.70710360613396317;
  a[3] = 0.70710995622487538;
  a[6] = 0.0;
  a[1] = -0.70710995622487538;
  a[4] = -0.70710360613396317;
  a[7] = -0.0;
  a[2] = 0.0;
  a[5] = -0.0;
  a[8] = 1.0;
  for (k = 0; k <= 0; k += 2) {
    __m128d tmp_3;

    /* MATLAB Function: '<S32>/Euler to Unit' */
    tmp_1 = _mm_loadu_pd(&a[k]);
    tmp_4 = _mm_set1_pd(0.0);
    tmp_2 = _mm_loadu_pd(&a[k + 3]);
    tmp_3 = _mm_loadu_pd(&a[k + 6]);
    _mm_storeu_pd(&ct[k], _mm_add_pd(_mm_mul_pd(tmp_3, tmp_4), _mm_add_pd
      (_mm_mul_pd(tmp_2, tmp_4), _mm_add_pd(tmp_4, tmp_1))));
  }

  /* MATLAB Function: '<S32>/Euler to Unit' */
  for (k = 2; k < 3; k++) {
    ct[k] = (a[k + 3] * 0.0 + a[k]) + a[k + 6] * 0.0;
  }

  /* Product: '<S32>/Product' incorporates:
   *  Gain: '<S32>/Thrust To Acting'
   *  Inport: '<Root>/VSF Force'
   *  MATLAB Function: '<S32>/Euler to Unit'
   *  Sum: '<S35>/Sum'
   */
  rtb_Sum_d_idx_0 = -Wrench_U.x5 * ct[0];
  rtb_Sum_d_idx_1 = -Wrench_U.x5 * ct[1];
  rtb_Sum_d_idx_2 = -Wrench_U.x5 * ct[2];

  /* MATLAB Function: '<S31>/Euler to Unit' incorporates:
   *  Constant: '<S31>/VSA Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.VSADirection_Value, ct);

  /* Product: '<S31>/Product' incorporates:
   *  Gain: '<S31>/Thrust To Acting'
   *  Inport: '<Root>/VSA Force'
   *  Sum: '<S33>/Sum'
   */
  rtb_Sum_d4_idx_0 = -Wrench_U.x6 * ct[0];
  rtb_Sum_d4_idx_1 = -Wrench_U.x6 * ct[1];
  rtb_Sum_d4_idx_2 = -Wrench_U.x6 * ct[2];

  /* MATLAB Function: '<S29>/Euler to Unit' incorporates:
   *  Constant: '<S29>/VPA Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.VPADirection_Value, ct);

  /* Product: '<S29>/Product' incorporates:
   *  Gain: '<S29>/Thrust To Acting'
   *  Inport: '<Root>/VPA Force'
   *  Sum: '<S37>/Sum'
   */
  rtb_Sum_l_idx_0 = -Wrench_U.x7 * ct[0];
  rtb_Sum_l_idx_1 = -Wrench_U.x7 * ct[1];
  rtb_Sum_l_idx_2 = -Wrench_U.x7 * ct[2];

  /* MATLAB Function: '<S30>/Euler to Unit' incorporates:
   *  Constant: '<S30>/VPF Direction'
   */
  Wrench_EulertoUnit(Wrench_ConstP.VPFDirection_Value, ct);

  /* Product: '<S30>/Product' incorporates:
   *  Gain: '<S30>/Thrust To Acting'
   *  Inport: '<Root>/VPF Force'
   *  Sum: '<S12>/Sum'
   */
  ct_tmp = -Wrench_U.x8 * ct[0];
  ct_tmp_0 = -Wrench_U.x8 * ct[1];
  ct_tmp_1 = -Wrench_U.x8 * ct[2];

  /* Product: '<S43>/Element Product' incorporates:
   *  Product: '<S30>/Product'
   */
  tmp[2] = Wrench_ConstB.Sum_c[0];
  tmp[3] = Wrench_ConstB.Sum_c[2];
  ct_0[0] = ct_tmp_1;
  tmp[0] = Wrench_ConstB.Sum_c[1];
  tmp[4] = Wrench_ConstB.Sum_c[0];
  ct_0[1] = ct_tmp;
  ct_0[3] = ct_tmp_0;
  tmp[1] = Wrench_ConstB.Sum_c[2];
  tmp[5] = Wrench_ConstB.Sum_c[1];
  ct_0[2] = ct_tmp_0;
  ct_0[4] = ct_tmp_1;
  ct_0[5] = ct_tmp;
  for (k = 0; k <= 4; k += 2) {
    /* Product: '<S43>/Element Product' */
    tmp_1 = _mm_loadu_pd(&tmp[k]);
    tmp_4 = _mm_loadu_pd(&ct_0[k]);
    _mm_storeu_pd(&rtb_ElementProduct_fu[k], _mm_mul_pd(tmp_1, tmp_4));
  }

  /* MATLAB Function: '<S3>/Create Bouancy Vector' incorporates:
   *  Constant: '<S3>/Density of Water'
   *  Constant: '<S3>/Gravitational Acceleration1'
   *  Constant: '<S3>/Point Up'
   *  Constant: '<S3>/Robot Volume'
   *  Inport: '<Root>/Gradient Bouyancy Force'
   *  Product: '<S3>/Bouyant Force'
   */
  rtb_bouyantForce_idx_2 = 340.03943213999992 * Wrench_U.GradientBouyancyForce;

  /* MATLAB Function: '<S2>/Concatenate Velocity' incorporates:
   *  Inport: '<Root>/Angular Velocities'
   */
  rtb_Abs3 = Wrench_U.AngularVelocities[0];
  rtb_Abs1_j = Wrench_U.AngularVelocities[1];
  tmp_0 = Wrench_U.AngularVelocities[2];
  for (k = 0; k <= 0; k += 2) {
    /* Product: '<S2>/Apply Rotation' */
    tmp_1 = _mm_loadu_pd(&rtb_rotation_c[k]);
    tmp_4 = _mm_loadu_pd(&rtb_rotation_c[k + 3]);
    tmp_2 = _mm_loadu_pd(&rtb_rotation_c[k + 6]);
    _mm_storeu_pd(&ct[k], _mm_add_pd(_mm_mul_pd(tmp_2, _mm_set1_pd(tmp_0)),
      _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(rtb_Abs1_j)), _mm_add_pd
                 (_mm_mul_pd(tmp_1, _mm_set1_pd(rtb_Abs3)), _mm_set1_pd(0.0)))));
  }

  /* Product: '<S2>/Apply Rotation' */
  for (k = 2; k < 3; k++) {
    ct[k] = (rtb_rotation_c[k + 3] * rtb_Abs1_j + rtb_rotation_c[k] * rtb_Abs3)
      + rtb_rotation_c[k + 6] * tmp_0;
  }

  /* Abs: '<S2>/Abs1' incorporates:
   *  MATLAB Function: '<S2>/Seperate to Components'
   *  Product: '<S2>/Apply Rotation'
   */
  rtb_Abs1 = fabs(ct[0]);

  /* Abs: '<S2>/Abs2' incorporates:
   *  MATLAB Function: '<S2>/Seperate to Components'
   *  Product: '<S2>/Apply Rotation'
   */
  rtb_Abs2 = fabs(ct[1]);

  /* Switch: '<S2>/Ensure Opposite Sign of Velocity Y' incorporates:
   *  Constant: '<S2>/Constant10'
   *  Constant: '<S2>/Constant11'
   *  MATLAB Function: '<S2>/Seperate to Components'
   *  Product: '<S2>/Apply Rotation'
   */
  if (ct[1] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S2>/Set Y Drag Sign' incorporates:
   *  Polyval: '<S2>/Calculate Y Drag'
   *  Switch: '<S2>/Ensure Opposite Sign of Velocity Y'
   */
  rtb_Abs2 = (-12.0 * rtb_Abs2 - 4.5) * rtb_Abs2 * (real_T)k;

  /* Abs: '<S2>/Abs3' incorporates:
   *  MATLAB Function: '<S2>/Seperate to Components'
   *  Product: '<S2>/Apply Rotation'
   */
  rtb_Abs3 = fabs(ct[2]);

  /* MATLAB Function: '<S2>/Concatenate Drag' incorporates:
   *  Constant: '<S2>/Constant12'
   *  Constant: '<S2>/Constant13'
   *  Constant: '<S2>/Constant8'
   *  Constant: '<S2>/Constant9'
   *  MATLAB Function: '<S2>/Seperate to Components'
   *  Polyval: '<S2>/Calculate X Drag'
   *  Polyval: '<S2>/Calculate Z Drag'
   *  Product: '<S2>/Apply Rotation'
   *  Product: '<S2>/Set X Drag Sign'
   *  Product: '<S2>/Set Z Drag Sign'
   *  Switch: '<S2>/Ensure Opposite Sign of Velocity X'
   *  Switch: '<S2>/Ensure Opposite Sign of Velocity Z'
   */
  if (ct[0] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs1 = (-6.0 * rtb_Abs1 - 2.0) * rtb_Abs1 * (real_T)k;
  if (ct[2] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_ApplyRotation_idx_2 = (-12.0 * rtb_Abs3 - 2.5) * rtb_Abs3 * (real_T)k;

  /* MATLAB Function: '<S5>/Row to Column Vector' incorporates:
   *  Inport: '<Root>/Linear Velocities'
   */
  rtb_Abs3 = Wrench_U.LinearVelocities[0];
  rtb_Abs1_j = Wrench_U.LinearVelocities[1];
  tmp_0 = Wrench_U.LinearVelocities[2];
  for (k = 0; k < 3; k++) {
    /* Product: '<S2>/Apply Inverse Roation' incorporates:
     *  MATLAB Function: '<S2>/Concatenate Drag'
     *  MATLAB Function: '<S2>/Create Rotation Matrix'
     */
    st[k] = (rtb_rotation_c[3 * k + 1] * rtb_Abs2 + rtb_rotation_c[3 * k] *
             rtb_Abs1) + rtb_rotation_c[3 * k + 2] * rtb_ApplyRotation_idx_2;

    /* Product: '<S5>/Apply Rotation' */
    ct[k] = (rtb_rotation_k[k + 3] * rtb_Abs1_j + rtb_rotation_k[k] * rtb_Abs3)
      + rtb_rotation_k[k + 6] * tmp_0;
  }

  /* MATLAB Function: '<S5>/Seperate to Components' */
  Wrench_SeperatetoComponents(ct, &rtb_Abs2, &rtb_Abs3, &rtb_Abs1);

  /* Abs: '<S5>/Abs1' */
  rtb_Abs1_j = fabs(rtb_Abs2);

  /* Switch: '<S5>/Ensure Opposite Sign of Velocity X' incorporates:
   *  Constant: '<S5>/Constant8'
   *  Constant: '<S5>/Constant9'
   */
  if (rtb_Abs2 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S5>/Set X Drag Sign' incorporates:
   *  Polyval: '<S5>/Calculate X Drag'
   *  Switch: '<S5>/Ensure Opposite Sign of Velocity X'
   */
  rtb_Abs2 = (-180.0 * rtb_Abs1_j - 17.0) * rtb_Abs1_j * (real_T)k;

  /* Abs: '<S5>/Abs2' */
  rtb_Abs1_j = fabs(rtb_Abs3);

  /* Switch: '<S5>/Ensure Opposite Sign of Velocity Y' incorporates:
   *  Constant: '<S5>/Constant10'
   *  Constant: '<S5>/Constant11'
   */
  if (rtb_Abs3 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  /* Product: '<S5>/Set Y Drag Sign' incorporates:
   *  Polyval: '<S5>/Calculate Y Drag'
   *  Switch: '<S5>/Ensure Opposite Sign of Velocity Y'
   */
  rtb_Abs3 = (-180.0 * rtb_Abs1_j - 17.0) * rtb_Abs1_j * (real_T)k;

  /* Switch: '<S5>/Ensure Opposite Sign of Velocity Z' incorporates:
   *  Constant: '<S5>/Constant12'
   *  Constant: '<S5>/Constant13'
   */
  if (rtb_Abs1 >= 0.0) {
    rtb_Abs1_j = 1.0;
  } else {
    rtb_Abs1_j = -1.0;
  }

  /* End of Switch: '<S5>/Ensure Opposite Sign of Velocity Z' */

  /* Abs: '<S5>/Abs3' */
  rtb_Abs1 = fabs(rtb_Abs1);

  /* MATLAB Function: '<S5>/Concatenate Drag' incorporates:
   *  Polyval: '<S5>/Calculate Z Drag'
   *  Product: '<S5>/Set Z Drag Sign'
   */
  rtb_Abs1 = (-460.0 * rtb_Abs1 - 35.0) * rtb_Abs1 * rtb_Abs1_j;

  /* Product: '<S5>/Apply Inverse Roation' incorporates:
   *  MATLAB Function: '<S5>/Concatenate Drag'
   *  MATLAB Function: '<S5>/Create Rotation Matrix'
   */
  for (k = 0; k < 3; k++) {
    ct[k] = (rtb_rotation_k[3 * k + 1] * rtb_Abs3 + rtb_rotation_k[3 * k] *
             rtb_Abs2) + rtb_rotation_k[3 * k + 2] * rtb_Abs1;
  }

  /* End of Product: '<S5>/Apply Inverse Roation' */

  /* MATLAB Function: '<S5>/Seperate to World Compents' */
  Wrench_SeperatetoComponents(ct, &rtb_Abs1_j, &rtb_Abs1, &rtb_Abs2);

  /* Sum: '<S6>/Sum Forces' incorporates:
   *  Sum: '<S33>/Sum'
   *  Sum: '<S35>/Sum'
   *  Sum: '<S37>/Sum'
   *  Sum: '<S41>/Sum'
   *  Sum: '<S43>/Sum'
   *  Sum: '<S45>/Sum'
   *  Sum: '<S47>/Sum'
   */
  rtb_Sum_e[0] = ((((((rtb_Sum_e[0] + rtb_Sum_eg_idx_0) + rtb_Sum_b_idx_0) +
                     rtb_Sum_eh_idx_0) + rtb_Sum_d_idx_0) + rtb_Sum_d4_idx_0) +
                  rtb_Sum_l_idx_0) + ct_tmp;
  rtb_Sum_e[1] = ((((((rtb_Sum_e[1] + rtb_Sum_eg_idx_1) + rtb_Sum_b_idx_1) +
                     rtb_Sum_eh_idx_1) + rtb_Sum_d_idx_1) + rtb_Sum_d4_idx_1) +
                  rtb_Sum_l_idx_1) + ct_tmp_0;
  rtb_Sum_e[2] = ((((((rtb_Sum_e[2] + rtb_Sum_eg_idx_2) + rtb_Sum_b_idx_2) +
                     rtb_Sum_eh_idx_2) + rtb_Sum_d_idx_2) + rtb_Sum_d4_idx_2) +
                  rtb_Sum_l_idx_2) + ct_tmp_1;

  /* MATLAB Function: '<S6>/Convert to Column Vector' */
  Wrench_ConverttoColumnVector(rtb_Sum_e, ct);
  for (k = 0; k <= 0; k += 2) {
    /* Product: '<S6>/Convert to World Frame' */
    tmp_1 = _mm_loadu_pd(&rtb_rotation[k]);
    tmp_4 = _mm_loadu_pd(&rtb_rotation[k + 3]);
    tmp_2 = _mm_loadu_pd(&rtb_rotation[k + 6]);
    _mm_storeu_pd(&rtb_Sum_e[k], _mm_add_pd(_mm_mul_pd(tmp_2, _mm_set1_pd(ct[2])),
      _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(ct[1])), _mm_add_pd(_mm_mul_pd
      (tmp_1, _mm_set1_pd(ct[0])), _mm_set1_pd(0.0)))));
  }

  /* Product: '<S6>/Convert to World Frame' */
  for (k = 2; k < 3; k++) {
    rtb_Sum_e[k] = (rtb_rotation[k + 3] * ct[1] + rtb_rotation[k] * ct[0]) +
      rtb_rotation[k + 6] * ct[2];
  }

  /* MATLAB Function: '<S6>/Convert to Row Vector' */
  Wrench_ConverttoRowVector(rtb_Sum_e, ct);

  /* Outport: '<Root>/Out1' incorporates:
   *  Sum: '<S1>/Sum'
   */
  Wrench_Y.Out1[0] = rtb_Abs1_j + ct[0];

  /* Sum: '<S39>/Sum' */
  ct_tmp = rtb_ElementProduct_idx_0 - rtb_ElementProduct_idx_3;

  /* Sum: '<S6>/Sum Moments' incorporates:
   *  Product: '<S33>/Element Product'
   *  Product: '<S35>/Element Product'
   *  Product: '<S37>/Element Product'
   *  Product: '<S41>/Element Product'
   *  Product: '<S45>/Element Product'
   *  Product: '<S47>/Element Product'
   *  Sum: '<S33>/Sum'
   *  Sum: '<S35>/Sum'
   *  Sum: '<S37>/Sum'
   *  Sum: '<S41>/Sum'
   *  Sum: '<S43>/Sum'
   *  Sum: '<S45>/Sum'
   *  Sum: '<S47>/Sum'
   */
  rtb_Sum_e[0] = (((((((Wrench_ConstB.Sum_lx[1] * rtb_Sum_eg_idx_2 -
                        rtb_Sum_eg_idx_1 * Wrench_ConstB.Sum_lx[2]) + ct_tmp) +
                      (Wrench_ConstB.Sum[1] * rtb_Sum_b_idx_2 - rtb_Sum_b_idx_1 *
                       Wrench_ConstB.Sum[2])) + (Wrench_ConstB.Sum_l[1] *
    rtb_Sum_eh_idx_2 - rtb_Sum_eh_idx_1 * Wrench_ConstB.Sum_l[2])) +
                    (Wrench_ConstB.Sum_nt[1] * rtb_Sum_d_idx_2 - rtb_Sum_d_idx_1
                     * Wrench_ConstB.Sum_nt[2])) + (Wrench_ConstB.Sum_g[1] *
    rtb_Sum_d4_idx_2 - rtb_Sum_d4_idx_1 * Wrench_ConstB.Sum_g[2])) +
                  (Wrench_ConstB.Sum_n[1] * rtb_Sum_l_idx_2 - rtb_Sum_l_idx_1 *
                   Wrench_ConstB.Sum_n[2])) + (rtb_ElementProduct_fu[0] -
    rtb_ElementProduct_fu[3]);
  ct[0] = ct_tmp;

  /* Outport: '<Root>/Out1' incorporates:
   *  Sum: '<S1>/Sum'
   */
  Wrench_Y.Out1[1] = rtb_Abs1 + ct[1];

  /* Sum: '<S39>/Sum' */
  ct_tmp = rtb_ElementProduct_idx_1 - rtb_ElementProduct_idx_4;

  /* Sum: '<S6>/Sum Moments' incorporates:
   *  Product: '<S33>/Element Product'
   *  Product: '<S35>/Element Product'
   *  Product: '<S37>/Element Product'
   *  Product: '<S41>/Element Product'
   *  Product: '<S45>/Element Product'
   *  Product: '<S47>/Element Product'
   *  Sum: '<S33>/Sum'
   *  Sum: '<S35>/Sum'
   *  Sum: '<S37>/Sum'
   *  Sum: '<S41>/Sum'
   *  Sum: '<S43>/Sum'
   *  Sum: '<S45>/Sum'
   *  Sum: '<S47>/Sum'
   */
  rtb_Sum_e[1] = (((((((rtb_Sum_eg_idx_0 * Wrench_ConstB.Sum_lx[2] -
                        Wrench_ConstB.Sum_lx[0] * rtb_Sum_eg_idx_2) + ct_tmp) +
                      (rtb_Sum_b_idx_0 * Wrench_ConstB.Sum[2] -
                       Wrench_ConstB.Sum[0] * rtb_Sum_b_idx_2)) +
                     (rtb_Sum_eh_idx_0 * Wrench_ConstB.Sum_l[2] -
                      Wrench_ConstB.Sum_l[0] * rtb_Sum_eh_idx_2)) +
                    (rtb_Sum_d_idx_0 * Wrench_ConstB.Sum_nt[2] -
                     Wrench_ConstB.Sum_nt[0] * rtb_Sum_d_idx_2)) +
                   (rtb_Sum_d4_idx_0 * Wrench_ConstB.Sum_g[2] -
                    Wrench_ConstB.Sum_g[0] * rtb_Sum_d4_idx_2)) +
                  (rtb_Sum_l_idx_0 * Wrench_ConstB.Sum_n[2] -
                   Wrench_ConstB.Sum_n[0] * rtb_Sum_l_idx_2)) +
    (rtb_ElementProduct_fu[1] - rtb_ElementProduct_fu[4]);
  ct[1] = ct_tmp;

  /* Outport: '<Root>/Out1' incorporates:
   *  MATLAB Function: '<S4>/Create Vector'
   *  Sum: '<S1>/Sum'
   */
  Wrench_Y.Out1[2] = ((rtb_Abs2 + Wrench_ConstB.GravitationalForce1) + ct[2]) +
    rtb_bouyantForce_idx_2;

  /* Sum: '<S39>/Sum' */
  ct_tmp = rtb_ElementProduct_idx_2 - rtb_ElementProduct_idx_5;

  /* Sum: '<S6>/Sum Moments' incorporates:
   *  Product: '<S33>/Element Product'
   *  Product: '<S35>/Element Product'
   *  Product: '<S37>/Element Product'
   *  Product: '<S41>/Element Product'
   *  Product: '<S45>/Element Product'
   *  Product: '<S47>/Element Product'
   *  Sum: '<S33>/Sum'
   *  Sum: '<S35>/Sum'
   *  Sum: '<S37>/Sum'
   *  Sum: '<S41>/Sum'
   *  Sum: '<S43>/Sum'
   *  Sum: '<S45>/Sum'
   *  Sum: '<S47>/Sum'
   */
  rtb_Sum_e[2] = (((((((Wrench_ConstB.Sum_lx[0] * rtb_Sum_eg_idx_1 -
                        rtb_Sum_eg_idx_0 * Wrench_ConstB.Sum_lx[1]) + ct_tmp) +
                      (Wrench_ConstB.Sum[0] * rtb_Sum_b_idx_1 - rtb_Sum_b_idx_0 *
                       Wrench_ConstB.Sum[1])) + (Wrench_ConstB.Sum_l[0] *
    rtb_Sum_eh_idx_1 - rtb_Sum_eh_idx_0 * Wrench_ConstB.Sum_l[1])) +
                    (Wrench_ConstB.Sum_nt[0] * rtb_Sum_d_idx_1 - rtb_Sum_d_idx_0
                     * Wrench_ConstB.Sum_nt[1])) + (Wrench_ConstB.Sum_g[0] *
    rtb_Sum_d4_idx_1 - rtb_Sum_d4_idx_0 * Wrench_ConstB.Sum_g[1])) +
                  (Wrench_ConstB.Sum_n[0] * rtb_Sum_l_idx_1 - rtb_Sum_l_idx_0 *
                   Wrench_ConstB.Sum_n[1])) + (rtb_ElementProduct_fu[2] -
    rtb_ElementProduct_fu[5]);
  ct[2] = ct_tmp;

  /* MATLAB Function: '<S6>/Convert to Column Vector1' */
  Wrench_ConverttoColumnVector(rtb_Sum_e, ct);
  for (k = 0; k <= 0; k += 2) {
    /* Product: '<S6>/Convert to World Frame 1' */
    tmp_1 = _mm_loadu_pd(&rtb_rotation[k]);
    tmp_4 = _mm_loadu_pd(&rtb_rotation[k + 3]);
    tmp_2 = _mm_loadu_pd(&rtb_rotation[k + 6]);
    _mm_storeu_pd(&rtb_Sum_e[k], _mm_add_pd(_mm_mul_pd(tmp_2, _mm_set1_pd(ct[2])),
      _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(ct[1])), _mm_add_pd(_mm_mul_pd
      (tmp_1, _mm_set1_pd(ct[0])), _mm_set1_pd(0.0)))));
  }

  /* Product: '<S6>/Convert to World Frame 1' */
  for (k = 2; k < 3; k++) {
    rtb_Sum_e[k] = (rtb_rotation[k + 3] * ct[1] + rtb_rotation[k] * ct[0]) +
      rtb_rotation[k + 6] * ct[2];
  }

  /* MATLAB Function: '<S6>/Convert to Row Vector1' */
  Wrench_ConverttoRowVector(rtb_Sum_e, ct);

  /* Outport: '<Root>/Out2' incorporates:
   *  Inport: '<Root>/Gradient Bouyancy Force Position'
   *  MATLAB Function: '<S2>/Seperate to World Compents'
   *  Product: '<S12>/Element Product'
   *  Product: '<S2>/Apply Inverse Roation'
   *  Sum: '<S12>/Sum'
   *  Sum: '<S1>/Moments'
   */
  Wrench_Y.Out2[0] = (Wrench_U.GradientBouyancyForcePosition[1] *
                      rtb_bouyantForce_idx_2 - 0.0 *
                      Wrench_U.GradientBouyancyForcePosition[2]) + (st[0] + ct[0]);
  Wrench_Y.Out2[1] = (0.0 * Wrench_U.GradientBouyancyForcePosition[2] -
                      Wrench_U.GradientBouyancyForcePosition[0] *
                      rtb_bouyantForce_idx_2) + (ct[1] + st[1]);
  Wrench_Y.Out2[2] = (Wrench_U.GradientBouyancyForcePosition[0] * 0.0 - 0.0 *
                      Wrench_U.GradientBouyancyForcePosition[1]) + (ct[2] + st[2]);
}

/* Model initialize function */
void Wrench_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void Wrench_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
