//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Do_Model0.cpp
//
// Code generated for Simulink model 'Do_Model0'.
//
// Model version                  : 2.56
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Sun Mar 26 16:27:27 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Do_Model0.h"
#include "rtwtypes.h"
#include <cmath>
#include <emmintrin.h>
#include "Do_Model0_private.h"

//
// Output and update for atomic system:
//    '<S8>/Seperate to Components'
//    '<S8>/Seperate to World Compents'
//
void Do_Model0::Do_Model0_SeperatetoComponents(const real_T
  rtu_robotFrameVelocity[3], real_T *rty_x, real_T *rty_y, real_T *rty_z)
{
  *rty_x = rtu_robotFrameVelocity[0];
  *rty_y = rtu_robotFrameVelocity[1];
  *rty_z = rtu_robotFrameVelocity[2];
}

//
// Output and update for atomic system:
//    '<S9>/Convert to Column Vector'
//    '<S9>/Convert to Column Vector1'
//
void Do_Model0::Do_Model0_ConverttoColumnVector(const real_T rtu_row[3], real_T
  rty_column[3])
{
  rty_column[0] = rtu_row[0];
  rty_column[1] = rtu_row[1];
  rty_column[2] = rtu_row[2];
}

//
// Output and update for atomic system:
//    '<S9>/Convert to Row Vector'
//    '<S9>/Convert to Row Vector1'
//
void Do_Model0::Do_Model0_ConverttoRowVector(const real_T rtu_column[3], real_T
  rty_row[3])
{
  rty_row[0] = rtu_column[0];
  rty_row[1] = rtu_column[1];
  rty_row[2] = rtu_column[2];
}

//
// Output and update for atomic system:
//    '<S28>/Euler to Unit'
//    '<S29>/Euler to Unit'
//    '<S30>/Euler to Unit'
//    '<S31>/Euler to Unit'
//    '<S33>/Euler to Unit'
//    '<S34>/Euler to Unit'
//    '<S35>/Euler to Unit'
//
void Do_Model0::Do_Model0_EulertoUnit(const real_T rtu_euler[3], real_T
  rty_direction[3])
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
  ct_0 = std::cos(rtu_euler[0]);
  st = std::sin(rtu_euler[0]);
  ct_1 = std::cos(rtu_euler[1]);
  st_0 = std::sin(rtu_euler[1]);
  ct_2 = std::cos(rtu_euler[2]);
  st_1 = std::sin(rtu_euler[2]);
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
  for (int32_T i{0}; i <= 0; i += 2) {
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

  for (int32_T i{2}; i < 3; i++) {
    ct[i] = (a[i + 3] * 0.0 + a[i]) + a[i + 6] * 0.0;
  }

  rty_direction[0] = ct[0];
  rty_direction[1] = ct[1];
  rty_direction[2] = ct[2];
}

// Model step function
void Do_Model0::step()
{
  __m128d tmp_0;
  __m128d tmp_1;
  __m128d tmp_3;
  real_T a[9];
  real_T rotationToRobot[9];
  real_T rtb_rotation[9];
  real_T rtb_rotation_a[9];
  real_T rtb_rotation_b[9];
  real_T rtb_ElementProduct[6];
  real_T rtb_ElementProduct_e[6];
  real_T rtb_ElementProduct_g[6];
  real_T rtb_ElementProduct_j5[6];
  real_T rtb_ElementProduct_l[6];
  real_T rtb_ElementProduct_m[6];
  real_T rtb_ElementProduct_mf[6];
  real_T rtb_ElementProduct_n[6];
  real_T st[6];
  real_T tmp[6];
  real_T ct[3];
  real_T rtb_ApplyInverseRoation[3];
  real_T rtb_ApplyRotation[3];
  real_T rtb_Moments[3];
  real_T rtb_Product[3];
  real_T rtb_SumMoments[3];
  real_T rtb_Abs1;
  real_T rtb_Abs1_p;
  real_T rtb_Abs2;
  real_T rtb_Abs3;
  real_T rtb_Product_a_idx_0;
  real_T rtb_Product_a_idx_1;
  real_T rtb_Product_a_idx_2;
  real_T rtb_Product_d_idx_0;
  real_T rtb_Product_d_idx_1;
  real_T rtb_Product_d_idx_2;
  real_T rtb_Product_e_idx_0;
  real_T rtb_Product_e_idx_1;
  real_T rtb_Product_e_idx_2;
  real_T rtb_Product_f0_idx_0;
  real_T rtb_Product_f0_idx_1;
  real_T rtb_Product_f0_idx_2;
  real_T rtb_Product_j_idx_0;
  real_T rtb_Product_j_idx_1;
  real_T rtb_Product_j_idx_2;
  real_T rtb_y_b;
  real_T st_idx_0;
  real_T st_idx_1;
  real_T st_idx_2;
  int32_T k;

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  // MATLAB Function: '<S8>/Create Rotation Matrix' incorporates:
  //   Inport: '<Root>/Angular Position'
  //   MATLAB Function: '<S5>/Create Rotation Matrix'
  //   MATLAB Function: '<S9>/To World Frame'

  rtb_Product[0] = 0.017453292519943295 * Do_Model0_U.AngularPosition[0];
  rtb_Product[1] = 0.017453292519943295 * Do_Model0_U.AngularPosition[1];
  rtb_Product[2] = 0.017453292519943295 * Do_Model0_U.AngularPosition[2];
  rtb_Product_d_idx_0 = std::cos(rtb_Product[0]);
  rtb_Product_d_idx_1 = std::sin(rtb_Product[0]);
  rtb_Product_d_idx_2 = std::cos(rtb_Product[1]);
  rtb_Product_a_idx_0 = std::sin(rtb_Product[1]);
  rtb_Product_a_idx_1 = std::cos(rtb_Product[2]);
  rtb_Product_a_idx_2 = std::sin(rtb_Product[2]);
  rtb_Product_j_idx_0 = rtb_Product_d_idx_0 * rtb_Product_d_idx_2;
  rtb_rotation_b[0] = rtb_Product_j_idx_0;
  rtb_Product_j_idx_1 = rtb_Product_a_idx_0 * rtb_Product_a_idx_2;
  rtb_Product_j_idx_2 = rtb_Product_d_idx_1 * rtb_Product_a_idx_1;
  rtb_rotation_b[3] = rtb_Product_j_idx_1 * rtb_Product_d_idx_0 -
    rtb_Product_j_idx_2;
  st_idx_0 = rtb_Product_a_idx_0 * rtb_Product_a_idx_1;
  st_idx_1 = rtb_Product_d_idx_1 * rtb_Product_a_idx_2;
  rtb_rotation_b[6] = st_idx_0 * rtb_Product_d_idx_0 + st_idx_1;
  st_idx_2 = rtb_Product_d_idx_1 * rtb_Product_d_idx_2;
  rtb_rotation_b[1] = st_idx_2;
  rtb_Product_e_idx_0 = rtb_Product_d_idx_0 * rtb_Product_a_idx_1;
  rtb_rotation_b[4] = rtb_Product_j_idx_1 * rtb_Product_d_idx_1 +
    rtb_Product_e_idx_0;
  rtb_Product_e_idx_1 = rtb_Product_d_idx_0 * rtb_Product_a_idx_2;
  rtb_rotation_b[7] = st_idx_0 * rtb_Product_d_idx_1 - rtb_Product_e_idx_1;
  rtb_rotation_b[2] = -rtb_Product_a_idx_0;
  rtb_Product_a_idx_2 *= rtb_Product_d_idx_2;
  rtb_rotation_b[5] = rtb_Product_a_idx_2;
  rtb_Product_d_idx_2 *= rtb_Product_a_idx_1;
  rtb_rotation_b[8] = rtb_Product_d_idx_2;

  // MATLAB Function: '<S9>/To World Frame' incorporates:
  //   MATLAB Function: '<S5>/Create Rotation Matrix'

  rotationToRobot[0] = rtb_Product_j_idx_0;
  rotationToRobot[3] = rtb_Product_j_idx_1 * rtb_Product_d_idx_0 -
    rtb_Product_j_idx_2;
  rotationToRobot[6] = st_idx_0 * rtb_Product_d_idx_0 + st_idx_1;
  rotationToRobot[1] = st_idx_2;
  rotationToRobot[4] = rtb_Product_j_idx_1 * rtb_Product_d_idx_1 +
    rtb_Product_e_idx_0;
  rotationToRobot[7] = st_idx_0 * rtb_Product_d_idx_1 - rtb_Product_e_idx_1;
  rotationToRobot[2] = -rtb_Product_a_idx_0;
  rotationToRobot[5] = rtb_Product_a_idx_2;
  rotationToRobot[8] = rtb_Product_d_idx_2;
  for (k = 0; k < 3; k++) {
    rtb_Product_d_idx_0 = rtb_Product[k];
    rtb_rotation[3 * k] = rotationToRobot[k];
    rtb_rotation[3 * k + 1] = rotationToRobot[k + 3];
    rtb_rotation[3 * k + 2] = rotationToRobot[k + 6];
    rtb_Product[k] = std::sin(rtb_Product_d_idx_0);
    ct[k] = std::cos(rtb_Product_d_idx_0);
  }

  // MATLAB Function: '<S5>/Create Rotation Matrix'
  rtb_rotation_a[0] = ct[0] * ct[1];
  rtb_Product_d_idx_0 = rtb_Product[1] * rtb_Product[2];
  rtb_rotation_a[3] = rtb_Product_d_idx_0 * ct[0] - rtb_Product[0] * ct[2];
  rtb_Product_d_idx_1 = rtb_Product[1] * ct[2];
  rtb_rotation_a[6] = rtb_Product_d_idx_1 * ct[0] + rtb_Product[0] *
    rtb_Product[2];
  rtb_rotation_a[1] = rtb_Product[0] * ct[1];
  rtb_rotation_a[4] = rtb_Product_d_idx_0 * rtb_Product[0] + ct[0] * ct[2];
  rtb_rotation_a[7] = rtb_Product_d_idx_1 * rtb_Product[0] - ct[0] *
    rtb_Product[2];
  rtb_rotation_a[2] = -rtb_Product[1];
  rtb_rotation_a[5] = ct[1] * rtb_Product[2];
  rtb_rotation_a[8] = ct[1] * ct[2];

  // MATLAB Function: '<S31>/Euler to Unit' incorporates:
  //   Constant: '<S31>/HSF Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, ct);

  // Product: '<S31>/Product' incorporates:
  //   Gain: '<S31>/Thrust To Acting'
  //   Inport: '<Root>/HSF Force'
  //   Sum: '<S3>/Sum'

  rtb_Product[0] = -Do_Model0_U.x1 * ct[0];
  rtb_Product[1] = -Do_Model0_U.x1 * ct[1];
  rtb_Product[2] = -Do_Model0_U.x1 * ct[2];

  // Product: '<S43>/Element Product'
  rtb_ElementProduct[0] = Do_Model0_ConstB.Sum_f[1] * rtb_Product[2];
  rtb_ElementProduct[1] = rtb_Product[0] * Do_Model0_ConstB.Sum_f[2];
  rtb_ElementProduct[2] = Do_Model0_ConstB.Sum_f[0] * rtb_Product[1];
  rtb_ElementProduct[3] = rtb_Product[1] * Do_Model0_ConstB.Sum_f[2];
  rtb_ElementProduct[4] = Do_Model0_ConstB.Sum_f[0] * rtb_Product[2];
  rtb_ElementProduct[5] = rtb_Product[0] * Do_Model0_ConstB.Sum_f[1];

  // MATLAB Function: '<S30>/Euler to Unit' incorporates:
  //   Constant: '<S30>/HSA Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, ct);

  // Product: '<S30>/Product' incorporates:
  //   Gain: '<S30>/Thrust To Acting'
  //   Inport: '<Root>/HSA Force'
  //   Sum: '<S3>/Sum'

  rtb_Product_d_idx_0 = -Do_Model0_U.x2 * ct[0];
  rtb_Product_d_idx_1 = -Do_Model0_U.x2 * ct[1];
  rtb_Product_d_idx_2 = -Do_Model0_U.x2 * ct[2];

  // Product: '<S41>/Element Product'
  rtb_ElementProduct_m[0] = Do_Model0_ConstB.Sum_d[1] * rtb_Product_d_idx_2;
  rtb_ElementProduct_m[1] = rtb_Product_d_idx_0 * Do_Model0_ConstB.Sum_d[2];
  rtb_ElementProduct_m[2] = Do_Model0_ConstB.Sum_d[0] * rtb_Product_d_idx_1;
  rtb_ElementProduct_m[3] = rtb_Product_d_idx_1 * Do_Model0_ConstB.Sum_d[2];
  rtb_ElementProduct_m[4] = Do_Model0_ConstB.Sum_d[0] * rtb_Product_d_idx_2;
  rtb_ElementProduct_m[5] = rtb_Product_d_idx_0 * Do_Model0_ConstB.Sum_d[1];

  // MATLAB Function: '<S28>/Euler to Unit' incorporates:
  //   Constant: '<S28>/HPA Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, ct);

  // Product: '<S28>/Product' incorporates:
  //   Gain: '<S28>/Thrust To Acting'
  //   Inport: '<Root>/HPA Force'
  //   Sum: '<S3>/Sum'

  rtb_Product_a_idx_0 = -Do_Model0_U.x3 * ct[0];
  rtb_Product_a_idx_1 = -Do_Model0_U.x3 * ct[1];
  rtb_Product_a_idx_2 = -Do_Model0_U.x3 * ct[2];

  // Product: '<S37>/Element Product'
  rtb_ElementProduct_n[0] = Do_Model0_ConstB.Sum[1] * rtb_Product_a_idx_2;
  rtb_ElementProduct_n[1] = rtb_Product_a_idx_0 * Do_Model0_ConstB.Sum[2];
  rtb_ElementProduct_n[2] = Do_Model0_ConstB.Sum[0] * rtb_Product_a_idx_1;
  rtb_ElementProduct_n[3] = rtb_Product_a_idx_1 * Do_Model0_ConstB.Sum[2];
  rtb_ElementProduct_n[4] = Do_Model0_ConstB.Sum[0] * rtb_Product_a_idx_2;
  rtb_ElementProduct_n[5] = rtb_Product_a_idx_0 * Do_Model0_ConstB.Sum[1];

  // MATLAB Function: '<S29>/Euler to Unit' incorporates:
  //   Constant: '<S29>/HPF Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.pooled6, ct);

  // Product: '<S29>/Product' incorporates:
  //   Gain: '<S29>/Thrust To Acting'
  //   Inport: '<Root>/HPF Force'
  //   Sum: '<S3>/Sum'

  rtb_Product_j_idx_0 = -Do_Model0_U.x4 * ct[0];
  rtb_Product_j_idx_1 = -Do_Model0_U.x4 * ct[1];
  rtb_Product_j_idx_2 = -Do_Model0_U.x4 * ct[2];

  // Product: '<S39>/Element Product'
  rtb_ElementProduct_j5[0] = Do_Model0_ConstB.Sum_p[1] * rtb_Product_j_idx_2;
  rtb_ElementProduct_j5[1] = rtb_Product_j_idx_0 * Do_Model0_ConstB.Sum_p[2];
  rtb_ElementProduct_j5[2] = Do_Model0_ConstB.Sum_p[0] * rtb_Product_j_idx_1;
  rtb_ElementProduct_j5[3] = rtb_Product_j_idx_1 * Do_Model0_ConstB.Sum_p[2];
  rtb_ElementProduct_j5[4] = Do_Model0_ConstB.Sum_p[0] * rtb_Product_j_idx_2;
  rtb_ElementProduct_j5[5] = rtb_Product_j_idx_0 * Do_Model0_ConstB.Sum_p[1];

  // MATLAB Function: '<S36>/Euler to Unit'
  a[0] = -0.70710360613396317;
  a[3] = 0.70710995622487538;
  a[6] = 0.0;
  a[1] = -0.70710995622487538;
  a[4] = -0.70710360613396317;
  a[7] = -0.0;
  a[2] = 0.0;
  a[5] = -0.0;
  a[8] = 1.0;

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 0; k += 2) {
    __m128d tmp_2;

    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // MATLAB Function: '<S36>/Euler to Unit'
    tmp_0 = _mm_loadu_pd(&a[k]);
    tmp_3 = _mm_set1_pd(0.0);
    tmp_1 = _mm_loadu_pd(&a[k + 3]);
    tmp_2 = _mm_loadu_pd(&a[k + 6]);
    _mm_storeu_pd(&ct[k], _mm_add_pd(_mm_mul_pd(tmp_2, tmp_3), _mm_add_pd
      (_mm_mul_pd(tmp_1, tmp_3), _mm_add_pd(tmp_3, tmp_0))));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  // MATLAB Function: '<S36>/Euler to Unit'
  for (k = 2; k < 3; k++) {
    ct[k] = (a[k + 3] * 0.0 + a[k]) + a[k + 6] * 0.0;
  }

  // Product: '<S36>/Product' incorporates:
  //   Gain: '<S36>/Thrust To Acting'
  //   Inport: '<Root>/VSF Force'
  //   MATLAB Function: '<S36>/Euler to Unit'

  st_idx_0 = -Do_Model0_U.x5 * ct[0];
  st_idx_1 = -Do_Model0_U.x5 * ct[1];
  st_idx_2 = -Do_Model0_U.x5 * ct[2];

  // Product: '<S51>/Element Product'
  tmp[2] = Do_Model0_ConstB.Sum_hq[0];
  tmp[3] = Do_Model0_ConstB.Sum_hq[2];
  st[0] = st_idx_2;
  tmp[0] = Do_Model0_ConstB.Sum_hq[1];
  tmp[4] = Do_Model0_ConstB.Sum_hq[0];
  st[1] = st_idx_0;
  st[3] = st_idx_1;
  tmp[1] = Do_Model0_ConstB.Sum_hq[2];
  tmp[5] = Do_Model0_ConstB.Sum_hq[1];
  st[2] = st_idx_1;
  st[4] = st_idx_2;
  st[5] = st_idx_0;

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 4; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S51>/Element Product'
    tmp_0 = _mm_loadu_pd(&tmp[k]);
    tmp_3 = _mm_loadu_pd(&st[k]);
    _mm_storeu_pd(&rtb_ElementProduct_mf[k], _mm_mul_pd(tmp_0, tmp_3));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  // MATLAB Function: '<S35>/Euler to Unit' incorporates:
  //   Constant: '<S35>/VSA Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.VSADirection_Value, ct);

  // Product: '<S35>/Product' incorporates:
  //   Gain: '<S35>/Thrust To Acting'
  //   Inport: '<Root>/VSA Force'
  //   Sum: '<S3>/Sum'

  rtb_Product_e_idx_0 = -Do_Model0_U.x6 * ct[0];
  rtb_Product_e_idx_1 = -Do_Model0_U.x6 * ct[1];
  rtb_Product_e_idx_2 = -Do_Model0_U.x6 * ct[2];

  // Product: '<S49>/Element Product'
  rtb_ElementProduct_l[0] = Do_Model0_ConstB.Sum_j[1] * rtb_Product_e_idx_2;
  rtb_ElementProduct_l[1] = rtb_Product_e_idx_0 * Do_Model0_ConstB.Sum_j[2];
  rtb_ElementProduct_l[2] = Do_Model0_ConstB.Sum_j[0] * rtb_Product_e_idx_1;
  rtb_ElementProduct_l[3] = rtb_Product_e_idx_1 * Do_Model0_ConstB.Sum_j[2];
  rtb_ElementProduct_l[4] = Do_Model0_ConstB.Sum_j[0] * rtb_Product_e_idx_2;
  rtb_ElementProduct_l[5] = rtb_Product_e_idx_0 * Do_Model0_ConstB.Sum_j[1];

  // MATLAB Function: '<S33>/Euler to Unit' incorporates:
  //   Constant: '<S33>/VPA Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.VPADirection_Value, ct);

  // Product: '<S33>/Product' incorporates:
  //   Gain: '<S33>/Thrust To Acting'
  //   Inport: '<Root>/VPA Force'
  //   Sum: '<S3>/Sum'

  rtb_Product_f0_idx_0 = -Do_Model0_U.x7 * ct[0];
  rtb_Product_f0_idx_1 = -Do_Model0_U.x7 * ct[1];
  rtb_Product_f0_idx_2 = -Do_Model0_U.x7 * ct[2];

  // Product: '<S45>/Element Product'
  rtb_ElementProduct_g[0] = Do_Model0_ConstB.Sum_h[1] * rtb_Product_f0_idx_2;
  rtb_ElementProduct_g[1] = rtb_Product_f0_idx_0 * Do_Model0_ConstB.Sum_h[2];
  rtb_ElementProduct_g[2] = Do_Model0_ConstB.Sum_h[0] * rtb_Product_f0_idx_1;
  rtb_ElementProduct_g[3] = rtb_Product_f0_idx_1 * Do_Model0_ConstB.Sum_h[2];
  rtb_ElementProduct_g[4] = Do_Model0_ConstB.Sum_h[0] * rtb_Product_f0_idx_2;
  rtb_ElementProduct_g[5] = rtb_Product_f0_idx_0 * Do_Model0_ConstB.Sum_h[1];

  // MATLAB Function: '<S34>/Euler to Unit' incorporates:
  //   Constant: '<S34>/VPF Direction'

  Do_Model0_EulertoUnit(Do_Model0_ConstP.VPFDirection_Value, rtb_Moments);

  // Sum: '<S3>/Sum' incorporates:
  //   Gain: '<S34>/Thrust To Acting'
  //   Inport: '<Root>/VPF Force'
  //   Product: '<S34>/Product'
  //   Sum: '<S3>/Moments'

  ct[0] = -Do_Model0_U.x8 * rtb_Moments[0];
  ct[1] = -Do_Model0_U.x8 * rtb_Moments[1];
  ct[2] = -Do_Model0_U.x8 * rtb_Moments[2];

  // Product: '<S47>/Element Product'
  tmp[2] = Do_Model0_ConstB.Sum_pz[0];
  tmp[3] = Do_Model0_ConstB.Sum_pz[2];
  st[0] = ct[2];
  tmp[0] = Do_Model0_ConstB.Sum_pz[1];
  tmp[4] = Do_Model0_ConstB.Sum_pz[0];
  st[1] = ct[0];
  st[3] = ct[1];
  tmp[1] = Do_Model0_ConstB.Sum_pz[2];
  tmp[5] = Do_Model0_ConstB.Sum_pz[1];
  st[2] = ct[1];
  st[4] = ct[2];
  st[5] = ct[0];

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 4; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S47>/Element Product'
    tmp_0 = _mm_loadu_pd(&tmp[k]);
    tmp_3 = _mm_loadu_pd(&st[k]);
    _mm_storeu_pd(&rtb_ElementProduct_e[k], _mm_mul_pd(tmp_0, tmp_3));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  // Sum: '<S17>/Sum' incorporates:
  //   Gain: '<S6>/Gain'
  //   Inport: '<Root>/Angular Position'
  //   Inport: '<Root>/Previous Linear Position'

  rtb_y_b = (0.017453292 * Do_Model0_U.AngularPosition[0] +
             Do_Model0_U.PreviousLinearPosition[2]) + 0.017453292 *
    Do_Model0_U.AngularPosition[1];

  // MATLAB Function: '<S6>/Create Bouancy Vector' incorporates:
  //   Constant: '<S6>/Density of Water'
  //   Constant: '<S6>/Gravitational Acceleration1'
  //   Constant: '<S6>/Point Up'
  //   Constant: '<S6>/Robot Volume'
  //   Product: '<S6>/Bouyant Force'

  rtb_Moments[2] = 323.428310455 * rtb_y_b;

  // MATLAB Function: '<S5>/Concatenate Velocity' incorporates:
  //   Inport: '<Root>/Previous Angular Velcoity'

  rtb_Abs1 = Do_Model0_U.PreviousAngularVelcoity[0];
  rtb_Abs3 = Do_Model0_U.PreviousAngularVelcoity[1];
  rtb_Abs1_p = Do_Model0_U.PreviousAngularVelcoity[2];

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 0; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S5>/Apply Rotation'
    tmp_0 = _mm_loadu_pd(&rtb_rotation_a[k]);
    tmp_3 = _mm_loadu_pd(&rtb_rotation_a[k + 3]);
    tmp_1 = _mm_loadu_pd(&rtb_rotation_a[k + 6]);
    _mm_storeu_pd(&rtb_ApplyRotation[k], _mm_add_pd(_mm_mul_pd(tmp_1,
      _mm_set1_pd(rtb_Abs1_p)), _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd
      (rtb_Abs3)), _mm_add_pd(_mm_mul_pd(tmp_0, _mm_set1_pd(rtb_Abs1)),
      _mm_set1_pd(0.0)))));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  // Product: '<S5>/Apply Rotation'
  for (k = 2; k < 3; k++) {
    rtb_ApplyRotation[k] = (rtb_rotation_a[k + 3] * rtb_Abs3 + rtb_rotation_a[k]
      * rtb_Abs1) + rtb_rotation_a[k + 6] * rtb_Abs1_p;
  }

  // Abs: '<S5>/Abs1' incorporates:
  //   MATLAB Function: '<S5>/Seperate to Components'
  //   Product: '<S5>/Apply Rotation'

  rtb_Abs1 = std::abs(rtb_ApplyRotation[0]);

  // Abs: '<S5>/Abs2' incorporates:
  //   MATLAB Function: '<S5>/Seperate to Components'
  //   Product: '<S5>/Apply Rotation'

  rtb_Abs2 = std::abs(rtb_ApplyRotation[1]);

  // Switch: '<S5>/Ensure Opposite Sign of Velocity Y' incorporates:
  //   Constant: '<S5>/Constant10'
  //   Constant: '<S5>/Constant11'
  //   MATLAB Function: '<S5>/Seperate to Components'
  //   Product: '<S5>/Apply Rotation'

  if (rtb_ApplyRotation[1] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  // Product: '<S5>/Set Y Drag Sign' incorporates:
  //   Polyval: '<S5>/Calculate Y Drag'
  //   Switch: '<S5>/Ensure Opposite Sign of Velocity Y'

  rtb_Abs2 = (-12.0 * rtb_Abs2 - 4.5) * rtb_Abs2 * static_cast<real_T>(k);

  // Abs: '<S5>/Abs3' incorporates:
  //   MATLAB Function: '<S5>/Seperate to Components'
  //   Product: '<S5>/Apply Rotation'

  rtb_Abs3 = std::abs(rtb_ApplyRotation[2]);

  // MATLAB Function: '<S5>/Concatenate Drag' incorporates:
  //   Constant: '<S5>/Constant12'
  //   Constant: '<S5>/Constant13'
  //   Constant: '<S5>/Constant8'
  //   Constant: '<S5>/Constant9'
  //   MATLAB Function: '<S5>/Seperate to Components'
  //   Polyval: '<S5>/Calculate X Drag'
  //   Polyval: '<S5>/Calculate Z Drag'
  //   Product: '<S5>/Apply Rotation'
  //   Product: '<S5>/Set X Drag Sign'
  //   Product: '<S5>/Set Z Drag Sign'
  //   Switch: '<S5>/Ensure Opposite Sign of Velocity X'
  //   Switch: '<S5>/Ensure Opposite Sign of Velocity Z'

  if (rtb_ApplyRotation[0] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs1 = (-6.0 * rtb_Abs1 - 2.0) * rtb_Abs1 * static_cast<real_T>(k);
  if (rtb_ApplyRotation[2] >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  rtb_Abs3 = (-12.0 * rtb_Abs3 - 2.5) * rtb_Abs3 * static_cast<real_T>(k);
  for (k = 0; k < 3; k++) {
    // Sum: '<S47>/Sum'
    rtb_Abs1_p = rtb_ElementProduct_e[k] - rtb_ElementProduct_e[k + 3];

    // Sum: '<S9>/Sum Moments' incorporates:
    //   Sum: '<S37>/Sum'
    //   Sum: '<S39>/Sum'
    //   Sum: '<S41>/Sum'
    //   Sum: '<S43>/Sum'
    //   Sum: '<S45>/Sum'
    //   Sum: '<S49>/Sum'
    //   Sum: '<S51>/Sum'

    rtb_SumMoments[k] = (((((((rtb_ElementProduct[k] - rtb_ElementProduct[k + 3])
      + (rtb_ElementProduct_m[k] - rtb_ElementProduct_m[k + 3])) +
      (rtb_ElementProduct_n[k] - rtb_ElementProduct_n[k + 3])) +
      (rtb_ElementProduct_j5[k] - rtb_ElementProduct_j5[k + 3])) +
      (rtb_ElementProduct_mf[k] - rtb_ElementProduct_mf[k + 3])) +
                          (rtb_ElementProduct_l[k] - rtb_ElementProduct_l[k + 3]))
                         + (rtb_ElementProduct_g[k] - rtb_ElementProduct_g[k + 3]))
      + rtb_Abs1_p;
    rtb_ApplyRotation[k] = rtb_Abs1_p;

    // Product: '<S5>/Apply Inverse Roation' incorporates:
    //   MATLAB Function: '<S5>/Concatenate Drag'
    //   MATLAB Function: '<S5>/Create Rotation Matrix'

    rtb_ApplyInverseRoation[k] = (rtb_rotation_a[3 * k + 1] * rtb_Abs2 +
      rtb_rotation_a[3 * k] * rtb_Abs1) + rtb_rotation_a[3 * k + 2] * rtb_Abs3;
  }

  // MATLAB Function: '<S9>/Convert to Column Vector1'
  Do_Model0_ConverttoColumnVector(rtb_SumMoments, rtb_ApplyRotation);

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 0; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S9>/Convert to World Frame 1'
    tmp_0 = _mm_loadu_pd(&rtb_rotation[k]);
    tmp_3 = _mm_loadu_pd(&rtb_rotation[k + 3]);
    tmp_1 = _mm_loadu_pd(&rtb_rotation[k + 6]);

    // Product: '<S9>/Convert to World Frame 1'
    _mm_storeu_pd(&rtb_SumMoments[k], _mm_add_pd(_mm_mul_pd(tmp_1, _mm_set1_pd
      (rtb_ApplyRotation[2])), _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd
      (rtb_ApplyRotation[1])), _mm_add_pd(_mm_mul_pd(tmp_0, _mm_set1_pd
      (rtb_ApplyRotation[0])), _mm_set1_pd(0.0)))));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  for (k = 2; k < 3; k++) {
    // Product: '<S9>/Convert to World Frame 1'
    rtb_SumMoments[k] = (rtb_rotation[k + 3] * rtb_ApplyRotation[1] +
                         rtb_rotation[k] * rtb_ApplyRotation[0]) +
      rtb_rotation[k + 6] * rtb_ApplyRotation[2];
  }

  // MATLAB Function: '<S9>/Convert to Row Vector1'
  Do_Model0_ConverttoRowVector(rtb_SumMoments, rtb_ApplyRotation);

  // Clock: '<S4>/Clock'
  Do_Model0_B.Clock = (&Do_Model0_M)->Timing.t[0];

  // Sum: '<S4>/Delta Time' incorporates:
  //   Memory: '<S4>/Memory1'

  rtb_Abs2 = Do_Model0_B.Clock - Do_Model0_DW.Memory1_PreviousInput;

  // Product: '<S15>/Element Product'
  rtb_Abs1 = rtb_y_b * rtb_Moments[2];

  // Sum: '<S2>/Current Angular Velocity' incorporates:
  //   Constant: '<S2>/Moments of Inertia'
  //   Inport: '<Root>/Previous Angular Velcoity'
  //   MATLAB Function: '<S5>/Seperate to World Compents'
  //   Product: '<S15>/Element Product'
  //   Product: '<S2>/Angular Acceleration'
  //   Product: '<S2>/Change in Angular Velocity'
  //   Product: '<S5>/Apply Inverse Roation'
  //   Sum: '<S15>/Sum'
  //   Sum: '<S3>/Moments'

  Do_Model0_Y.AngularVelocity[0] = ((rtb_Abs1 - rtb_y_b * 0.0) +
    (rtb_ApplyInverseRoation[0] + rtb_ApplyRotation[0])) / 0.9978 * rtb_Abs2 +
    Do_Model0_U.PreviousAngularVelcoity[0];
  Do_Model0_Y.AngularVelocity[1] = ((rtb_y_b * 0.0 - rtb_Abs1) +
    (rtb_ApplyInverseRoation[1] + rtb_ApplyRotation[1])) / 1.6934 * rtb_Abs2 +
    Do_Model0_U.PreviousAngularVelcoity[1];
  Do_Model0_Y.AngularVelocity[2] = ((rtb_y_b * 0.0 - rtb_y_b * 0.0) +
    (rtb_ApplyInverseRoation[2] + rtb_ApplyRotation[2])) / 2.1284 * rtb_Abs2 +
    Do_Model0_U.PreviousAngularVelcoity[2];

  // MATLAB Function: '<S8>/Row to Column Vector' incorporates:
  //   Inport: '<Root>/Previous Linear Velocity'

  rtb_Abs1 = Do_Model0_U.PreviousLinearVelocity[0];
  rtb_Abs3 = Do_Model0_U.PreviousLinearVelocity[1];
  rtb_Abs1_p = Do_Model0_U.PreviousLinearVelocity[2];

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 0; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S8>/Apply Rotation'
    tmp_0 = _mm_loadu_pd(&rtb_rotation_b[k]);
    tmp_3 = _mm_loadu_pd(&rtb_rotation_b[k + 3]);
    tmp_1 = _mm_loadu_pd(&rtb_rotation_b[k + 6]);

    // Product: '<S8>/Apply Rotation'
    _mm_storeu_pd(&rtb_ApplyInverseRoation[k], _mm_add_pd(_mm_mul_pd(tmp_1,
      _mm_set1_pd(rtb_Abs1_p)), _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd
      (rtb_Abs3)), _mm_add_pd(_mm_mul_pd(tmp_0, _mm_set1_pd(rtb_Abs1)),
      _mm_set1_pd(0.0)))));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  for (k = 2; k < 3; k++) {
    // Product: '<S8>/Apply Rotation'
    rtb_ApplyInverseRoation[k] = (rtb_rotation_b[k + 3] * rtb_Abs3 +
      rtb_rotation_b[k] * rtb_Abs1) + rtb_rotation_b[k + 6] * rtb_Abs1_p;
  }

  // MATLAB Function: '<S8>/Seperate to Components'
  Do_Model0_SeperatetoComponents(rtb_ApplyInverseRoation, &rtb_Abs1, &rtb_Abs3,
    &rtb_y_b);

  // Abs: '<S8>/Abs1'
  rtb_Abs1_p = std::abs(rtb_Abs1);

  // Switch: '<S8>/Ensure Opposite Sign of Velocity X' incorporates:
  //   Constant: '<S8>/Constant8'
  //   Constant: '<S8>/Constant9'

  if (rtb_Abs1 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  // Product: '<S8>/Set X Drag Sign' incorporates:
  //   Polyval: '<S8>/Calculate X Drag'
  //   Switch: '<S8>/Ensure Opposite Sign of Velocity X'

  rtb_Abs1 = (-180.0 * rtb_Abs1_p - 17.0) * rtb_Abs1_p * static_cast<real_T>(k);

  // Abs: '<S8>/Abs2'
  rtb_Abs1_p = std::abs(rtb_Abs3);

  // Switch: '<S8>/Ensure Opposite Sign of Velocity Y' incorporates:
  //   Constant: '<S8>/Constant10'
  //   Constant: '<S8>/Constant11'

  if (rtb_Abs3 >= 0.0) {
    k = 1;
  } else {
    k = -1;
  }

  // Product: '<S8>/Set Y Drag Sign' incorporates:
  //   Polyval: '<S8>/Calculate Y Drag'
  //   Switch: '<S8>/Ensure Opposite Sign of Velocity Y'

  rtb_Abs3 = (-180.0 * rtb_Abs1_p - 17.0) * rtb_Abs1_p * static_cast<real_T>(k);

  // Switch: '<S8>/Ensure Opposite Sign of Velocity Z' incorporates:
  //   Constant: '<S8>/Constant12'
  //   Constant: '<S8>/Constant13'

  if (rtb_y_b >= 0.0) {
    rtb_Abs1_p = 1.0;
  } else {
    rtb_Abs1_p = -1.0;
  }

  // End of Switch: '<S8>/Ensure Opposite Sign of Velocity Z'

  // Abs: '<S8>/Abs3'
  rtb_y_b = std::abs(rtb_y_b);

  // MATLAB Function: '<S8>/Concatenate Drag' incorporates:
  //   Polyval: '<S8>/Calculate Z Drag'
  //   Product: '<S8>/Set Z Drag Sign'

  rtb_y_b = (-460.0 * rtb_y_b - 35.0) * rtb_y_b * rtb_Abs1_p;
  for (k = 0; k < 3; k++) {
    // Product: '<S8>/Apply Inverse Roation' incorporates:
    //   MATLAB Function: '<S8>/Concatenate Drag'
    //   MATLAB Function: '<S8>/Create Rotation Matrix'

    rtb_ApplyInverseRoation[k] = (rtb_rotation_b[3 * k + 1] * rtb_Abs3 +
      rtb_rotation_b[3 * k] * rtb_Abs1) + rtb_rotation_b[3 * k + 2] * rtb_y_b;
  }

  // MATLAB Function: '<S8>/Seperate to World Compents'
  Do_Model0_SeperatetoComponents(rtb_ApplyInverseRoation, &rtb_Abs1_p, &rtb_y_b,
    &rtb_Abs1);

  // Sum: '<S9>/Sum Forces' incorporates:
  //   Product: '<S28>/Product'
  //   Product: '<S29>/Product'
  //   Product: '<S30>/Product'
  //   Product: '<S31>/Product'
  //   Product: '<S33>/Product'
  //   Product: '<S35>/Product'
  //   Product: '<S36>/Product'
  //   Sum: '<S3>/Sum'

  rtb_Product[0] = ((((((rtb_Product[0] + rtb_Product_d_idx_0) +
                        rtb_Product_a_idx_0) + rtb_Product_j_idx_0) + st_idx_0)
                     + rtb_Product_e_idx_0) + rtb_Product_f0_idx_0) + ct[0];
  rtb_Product[1] = ((((((rtb_Product[1] + rtb_Product_d_idx_1) +
                        rtb_Product_a_idx_1) + rtb_Product_j_idx_1) + st_idx_1)
                     + rtb_Product_e_idx_1) + rtb_Product_f0_idx_1) + ct[1];
  rtb_Product[2] = ((((((rtb_Product[2] + rtb_Product_d_idx_2) +
                        rtb_Product_a_idx_2) + rtb_Product_j_idx_2) + st_idx_2)
                     + rtb_Product_e_idx_2) + rtb_Product_f0_idx_2) + ct[2];

  // MATLAB Function: '<S9>/Convert to Column Vector'
  Do_Model0_ConverttoColumnVector(rtb_Product, ct);

  // End of Outputs for SubSystem: '<Root>/Do_Model'
  for (k = 0; k <= 0; k += 2) {
    // Outputs for Atomic SubSystem: '<Root>/Do_Model'
    // Product: '<S9>/Convert to World Frame'
    tmp_0 = _mm_loadu_pd(&rtb_rotation[k]);
    tmp_3 = _mm_loadu_pd(&rtb_rotation[k + 3]);
    tmp_1 = _mm_loadu_pd(&rtb_rotation[k + 6]);

    // Product: '<S9>/Convert to World Frame' incorporates:
    //   Sum: '<S3>/Sum'

    _mm_storeu_pd(&rtb_Product[k], _mm_add_pd(_mm_mul_pd(tmp_1, _mm_set1_pd(ct[2])),
      _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd(ct[1])), _mm_add_pd(_mm_mul_pd
      (tmp_0, _mm_set1_pd(ct[0])), _mm_set1_pd(0.0)))));

    // End of Outputs for SubSystem: '<Root>/Do_Model'
  }

  // Outputs for Atomic SubSystem: '<Root>/Do_Model'
  for (k = 2; k < 3; k++) {
    // Product: '<S9>/Convert to World Frame' incorporates:
    //   Sum: '<S3>/Sum'

    rtb_Product[k] = (rtb_rotation[k + 3] * ct[1] + rtb_rotation[k] * ct[0]) +
      rtb_rotation[k + 6] * ct[2];
  }

  // MATLAB Function: '<S9>/Convert to Row Vector'
  Do_Model0_ConverttoRowVector(rtb_Product, ct);

  // Outport: '<Root>/Linear Acceleration' incorporates:
  //   Constant: '<S2>/Robot Mass'
  //   Product: '<S2>/Acceleration'
  //   Sum: '<S3>/Sum'

  Do_Model0_Y.LinearAcceleration[0] = (rtb_Abs1_p + ct[0]) / 33.5;

  // Sum: '<S2>/Current Velocity' incorporates:
  //   Inport: '<Root>/Previous Linear Velocity'
  //   Outport: '<Root>/Linear Acceleration'
  //   Product: '<S2>/Acceleration'
  //   Product: '<S2>/Change in Velocity'

  Do_Model0_Y.LinearVelocity[0] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[0] +
    Do_Model0_U.PreviousLinearVelocity[0];

  // Outport: '<Root>/Linear Position' incorporates:
  //   Inport: '<Root>/Previous Linear Position'
  //   Product: '<S2>/Change in Position'
  //   Sum: '<S2>/Current Position'
  //   Sum: '<S2>/Current Velocity'

  Do_Model0_Y.LinearPosition[0] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[0] +
    Do_Model0_U.PreviousLinearPosition[0];

  // Outport: '<Root>/Angular Position1' incorporates:
  //   Inport: '<Root>/Angular Position'
  //   Product: '<S2>/Change in Angular Position'
  //   Sum: '<S2>/Current Angular Position'
  //   Sum: '<S2>/Current Angular Velocity'

  Do_Model0_Y.AngularPosition1[0] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[0] +
    Do_Model0_U.AngularPosition[0];

  // Outport: '<Root>/Linear Acceleration' incorporates:
  //   Constant: '<S2>/Robot Mass'
  //   Product: '<S2>/Acceleration'
  //   Sum: '<S3>/Sum'

  Do_Model0_Y.LinearAcceleration[1] = (rtb_y_b + ct[1]) / 33.5;

  // Sum: '<S2>/Current Velocity' incorporates:
  //   Inport: '<Root>/Previous Linear Velocity'
  //   Outport: '<Root>/Linear Acceleration'
  //   Product: '<S2>/Acceleration'
  //   Product: '<S2>/Change in Velocity'

  Do_Model0_Y.LinearVelocity[1] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[1] +
    Do_Model0_U.PreviousLinearVelocity[1];

  // Outport: '<Root>/Linear Position' incorporates:
  //   Inport: '<Root>/Previous Linear Position'
  //   Product: '<S2>/Change in Position'
  //   Sum: '<S2>/Current Position'
  //   Sum: '<S2>/Current Velocity'

  Do_Model0_Y.LinearPosition[1] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[1] +
    Do_Model0_U.PreviousLinearPosition[1];

  // Outport: '<Root>/Angular Position1' incorporates:
  //   Inport: '<Root>/Angular Position'
  //   Product: '<S2>/Change in Angular Position'
  //   Sum: '<S2>/Current Angular Position'
  //   Sum: '<S2>/Current Angular Velocity'

  Do_Model0_Y.AngularPosition1[1] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[1] +
    Do_Model0_U.AngularPosition[1];

  // Outport: '<Root>/Linear Acceleration' incorporates:
  //   Constant: '<S2>/Robot Mass'
  //   MATLAB Function: '<S7>/Create Vector'
  //   Product: '<S2>/Acceleration'
  //   Sum: '<S3>/Sum'

  Do_Model0_Y.LinearAcceleration[2] = (((rtb_Abs1 +
    Do_Model0_ConstB.GravitationalForce1) + ct[2]) + rtb_Moments[2]) / 33.5;

  // Sum: '<S2>/Current Velocity' incorporates:
  //   Inport: '<Root>/Previous Linear Velocity'
  //   Outport: '<Root>/Linear Acceleration'
  //   Product: '<S2>/Acceleration'
  //   Product: '<S2>/Change in Velocity'

  Do_Model0_Y.LinearVelocity[2] = rtb_Abs2 * Do_Model0_Y.LinearAcceleration[2] +
    Do_Model0_U.PreviousLinearVelocity[2];

  // Outport: '<Root>/Linear Position' incorporates:
  //   Inport: '<Root>/Previous Linear Position'
  //   Product: '<S2>/Change in Position'
  //   Sum: '<S2>/Current Position'
  //   Sum: '<S2>/Current Velocity'

  Do_Model0_Y.LinearPosition[2] = rtb_Abs2 * Do_Model0_Y.LinearVelocity[2] +
    Do_Model0_U.PreviousLinearPosition[2];

  // Outport: '<Root>/Angular Position1' incorporates:
  //   Inport: '<Root>/Angular Position'
  //   Product: '<S2>/Change in Angular Position'
  //   Sum: '<S2>/Current Angular Position'
  //   Sum: '<S2>/Current Angular Velocity'

  Do_Model0_Y.AngularPosition1[2] = rtb_Abs2 * Do_Model0_Y.AngularVelocity[2] +
    Do_Model0_U.AngularPosition[2];

  // End of Outputs for SubSystem: '<Root>/Do_Model'

  // Update for Atomic SubSystem: '<Root>/Do_Model'
  // Update for Memory: '<S4>/Memory1'
  Do_Model0_DW.Memory1_PreviousInput = Do_Model0_B.Clock;

  // End of Update for SubSystem: '<Root>/Do_Model'

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  (&Do_Model0_M)->Timing.t[0] =
    ((time_T)(++(&Do_Model0_M)->Timing.clockTick0)) * (&Do_Model0_M)
    ->Timing.stepSize0;

  {
    // Update absolute timer for sample time: [0.05s, 0.0s]
    // The "clockTick1" counts the number of times the code of this task has
    //  been executed. The resolution of this integer timer is 0.05, which is the step size
    //  of the task. Size of "clockTick1" ensures timer will not overflow during the
    //  application lifespan selected.

    (&Do_Model0_M)->Timing.clockTick1++;
  }
}

// Model initialize function
void Do_Model0::initialize()
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&Do_Model0_M)->solverInfo, &(&Do_Model0_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&Do_Model0_M)->solverInfo, &rtmGetTPtr((&Do_Model0_M)));
    rtsiSetStepSizePtr(&(&Do_Model0_M)->solverInfo, &(&Do_Model0_M)
                       ->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&(&Do_Model0_M)->solverInfo, (&rtmGetErrorStatus
      ((&Do_Model0_M))));
    rtsiSetRTModelPtr(&(&Do_Model0_M)->solverInfo, (&Do_Model0_M));
  }

  rtsiSetSimTimeStep(&(&Do_Model0_M)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&(&Do_Model0_M)->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr((&Do_Model0_M), &(&Do_Model0_M)->Timing.tArray[0]);
  (&Do_Model0_M)->Timing.stepSize0 = 0.05;
}

// Model terminate function
void Do_Model0::terminate()
{
  // (no terminate code required)
}

// Constructor
Do_Model0::Do_Model0() :
  Do_Model0_U(),
  Do_Model0_Y(),
  Do_Model0_B(),
  Do_Model0_DW(),
  Do_Model0_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
Do_Model0::~Do_Model0()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
Do_Model0::RT_MODEL_Do_Model0_T * Do_Model0::getRTM()
{
  return (&Do_Model0_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
