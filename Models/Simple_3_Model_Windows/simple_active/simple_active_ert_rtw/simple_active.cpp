//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: simple_active.cpp
//
// Code generated for Simulink model 'simple_active'.
//
// Model version                  : 1.20
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Sat Oct 28 01:38:09 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LP64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "simple_active.h"
#include "simple_active_types.h"
#include "rtwtypes.h"
#include <math.h>
#include "simple_active_private.h"

extern "C"
{

#include "rt_nonfinite.h"

}

#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include <stddef.h>
#include "rt_defines.h"

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(static_cast<real_T>(tmp), static_cast<real_T>(tmp_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

void simple_active::simple_active_binary_expand_op(real_T in1[3], const int8_T
  in2_data[], const real_T in3_data[], const int32_T *in3_size, const real_T
  in4[4], const int32_T in5_size[2])
{
  int32_T loop_ub;

  // MATLAB Function: '<Root>/Quat to eul1' incorporates:
  //   MATLAB Function: '<Root>/Quat to eul'

  loop_ub = in5_size[1] == 1 ? *in3_size : in5_size[1];
  for (int32_T i = 0; i < loop_ub; i++) {
    in1[in2_data[0] - 1] = -in3_data[0] * 2.0 * rt_atan2d_snf(in4[1], in4[0]);
  }

  // End of MATLAB Function: '<Root>/Quat to eul1'
}

void simple_active::simple_activ_SystemCore_setup_g
  (ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[24] = { '/', 't', 'a', 'l', 'o', 's', '/', 'o', 'd',
    'o', 'm', 'e', 't', 'r', 'y', '/', 'f', 'i', 'l', 't', 'e', 'r', 'e', 'd' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 24; i++) {
    simple_active_B.b_zeroDelimTopic_b[i] = tmp[i];
  }

  simple_active_B.b_zeroDelimTopic_b[24] = '\x00';
  Sub_simple_active_33.createSubscriber(&simple_active_B.b_zeroDelimTopic_b[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void simple_active::simple_acti_SystemCore_setup_gu
  (ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[36] = { '/', 't', 'a', 'l', 'o', 's', '/', 's', 'i',
    'm', 'p', 'l', 'e', '_', 'a', 'c', 't', 'i', 'v', 'e', '/', 't', 'a', 'r',
    'g', 'e', 't', '_', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 36; i++) {
    simple_active_B.b_zeroDelimTopic[i] = tmp[i];
  }

  simple_active_B.b_zeroDelimTopic[36] = '\x00';
  Sub_simple_active_39.createSubscriber(&simple_active_B.b_zeroDelimTopic[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void simple_active::simple_active_SystemCore_setup
  (ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[35] = { '/', 't', 'a', 'l', 'o', 's', '/', 'c', 'o',
    'n', 't', 'r', 'o', 'l', 'l', 'e', 'r', '/', 'a', 'c', 't', 'i', 'v', 'e',
    '_', 'b', 'o', 'd', 'y', '_', 'f', 'o', 'r', 'c', 'e' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 35; i++) {
    simple_active_B.b_zeroDelimTopic_c[i] = tmp[i];
  }

  simple_active_B.b_zeroDelimTopic_c[35] = '\x00';
  Pub_simple_active_53.createPublisher(&simple_active_B.b_zeroDelimTopic_c[0],
    qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void simple_active::step()
{
  real_T euler_tmp;
  int32_T e_size[2];
  int32_T e_size_0[2];
  int32_T k;
  int32_T trueCount;
  uint32_T len;
  int8_T f_data;
  int8_T f_data_0;
  boolean_T mask1;
  boolean_T mask2;

  // MATLABSystem: '<Root>/Get Parameter4'
  ParamGet_simple_active_48.getParameter(6U, &simple_active_B.value[0], &len);

  // MATLABSystem: '<Root>/Get Parameter1'
  ParamGet_simple_active_44.getParameter(&simple_active_B.value_cv);

  // MATLABSystem: '<S6>/SourceBlock'
  mask1 = Sub_simple_active_33.getLatestMessage(&simple_active_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S6>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S10>/Enable'

  if (mask1) {
    // SignalConversion generated from: '<S10>/In1'
    simple_active_B.In1 = simple_active_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S6>/SourceBlock'
  // End of Outputs for SubSystem: '<S6>/Enabled Subsystem'

  // MATLAB Function: '<Root>/Quat to eul'
  simple_active_B.Divide = 1.0 / sqrt
    (((simple_active_B.In1.pose.pose.orientation.w *
       simple_active_B.In1.pose.pose.orientation.w +
       simple_active_B.In1.pose.pose.orientation.x *
       simple_active_B.In1.pose.pose.orientation.x) +
      simple_active_B.In1.pose.pose.orientation.y *
      simple_active_B.In1.pose.pose.orientation.y) +
     simple_active_B.In1.pose.pose.orientation.z *
     simple_active_B.In1.pose.pose.orientation.z);
  simple_active_B.y[0] = simple_active_B.In1.pose.pose.orientation.w *
    simple_active_B.Divide;
  simple_active_B.y[1] = simple_active_B.In1.pose.pose.orientation.x *
    simple_active_B.Divide;
  simple_active_B.y[2] = simple_active_B.In1.pose.pose.orientation.y *
    simple_active_B.Divide;
  simple_active_B.y[3] = simple_active_B.In1.pose.pose.orientation.z *
    simple_active_B.Divide;
  simple_active_B.aSinInput = (simple_active_B.y[1] * simple_active_B.y[3] -
    simple_active_B.y[0] * simple_active_B.y[2]) * -2.0;
  mask1 = (simple_active_B.aSinInput >= 0.99999999999999778);
  mask2 = (simple_active_B.aSinInput <= -0.99999999999999778);
  if (mask1) {
    simple_active_B.aSinInput = 1.0;
  }

  if (mask2) {
    simple_active_B.aSinInput = -1.0;
  }

  mask1 = (mask1 || mask2);
  simple_active_B.Divide = simple_active_B.y[0] * simple_active_B.y[0];
  simple_active_B.q = simple_active_B.y[1] * simple_active_B.y[1];
  simple_active_B.euler_tmp = simple_active_B.y[2] * simple_active_B.y[2];
  euler_tmp = simple_active_B.y[3] * simple_active_B.y[3];
  simple_active_B.euler[0] = rt_atan2d_snf((simple_active_B.y[1] *
    simple_active_B.y[2] + simple_active_B.y[0] * simple_active_B.y[3]) * 2.0,
    ((simple_active_B.Divide + simple_active_B.q) - simple_active_B.euler_tmp) -
    euler_tmp);
  simple_active_B.euler[1] = asin(simple_active_B.aSinInput);
  simple_active_B.euler[2] = rt_atan2d_snf((simple_active_B.y[2] *
    simple_active_B.y[3] + simple_active_B.y[0] * simple_active_B.y[1]) * 2.0,
    ((simple_active_B.Divide - simple_active_B.q) - simple_active_B.euler_tmp) +
    euler_tmp);
  trueCount = 0;
  if (mask1) {
    trueCount = 1;
  }

  if (trueCount - 1 >= 0) {
    simple_active_B.x_data = simple_active_B.aSinInput;
  }

  for (k = 0; k < trueCount; k++) {
    if (rtIsNaN(simple_active_B.x_data)) {
      simple_active_B.Divide = (rtNaN);
    } else if (simple_active_B.x_data < 0.0) {
      simple_active_B.Divide = -1.0;
    } else {
      simple_active_B.Divide = (simple_active_B.x_data > 0.0);
    }

    simple_active_B.x_data = simple_active_B.Divide;
  }

  k = 0;
  if (mask1) {
    k = 1;
  }

  e_size[0] = 1;
  e_size[1] = k;
  if (mask1) {
    f_data = 1;
  }

  if (trueCount == k) {
    if (trueCount - 1 >= 0) {
      simple_active_B.euler[0] = -simple_active_B.x_data * 2.0 * rt_atan2d_snf
        (simple_active_B.y[1], simple_active_B.y[0]);
    }
  } else {
    simple_active_binary_expand_op(simple_active_B.euler, &f_data,
      &simple_active_B.x_data, &trueCount, simple_active_B.y, e_size);
  }

  trueCount = 0;
  if (mask1) {
    trueCount = 1;
  }

  if (trueCount - 1 >= 0) {
    simple_active_B.euler[2] = 0.0;
  }

  // MATLABSystem: '<S7>/SourceBlock'
  mask1 = Sub_simple_active_39.getLatestMessage(&simple_active_B.b_varargout_2_m);

  // Outputs for Enabled SubSystem: '<S7>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S11>/Enable'

  if (mask1) {
    // SignalConversion generated from: '<S11>/In1'
    simple_active_B.In1_c = simple_active_B.b_varargout_2_m;
  }

  // End of MATLABSystem: '<S7>/SourceBlock'
  // End of Outputs for SubSystem: '<S7>/Enabled Subsystem'

  // MATLAB Function: '<Root>/Quat to eul1'
  simple_active_B.Divide = 1.0 / sqrt(((simple_active_B.In1_c.orientation.w *
    simple_active_B.In1_c.orientation.w + simple_active_B.In1_c.orientation.x *
    simple_active_B.In1_c.orientation.x) + simple_active_B.In1_c.orientation.y *
    simple_active_B.In1_c.orientation.y) + simple_active_B.In1_c.orientation.z *
    simple_active_B.In1_c.orientation.z);
  simple_active_B.y[0] = simple_active_B.In1_c.orientation.w *
    simple_active_B.Divide;
  simple_active_B.y[1] = simple_active_B.In1_c.orientation.x *
    simple_active_B.Divide;
  simple_active_B.y[2] = simple_active_B.In1_c.orientation.y *
    simple_active_B.Divide;
  simple_active_B.y[3] = simple_active_B.In1_c.orientation.z *
    simple_active_B.Divide;
  simple_active_B.aSinInput = (simple_active_B.y[1] * simple_active_B.y[3] -
    simple_active_B.y[0] * simple_active_B.y[2]) * -2.0;
  mask1 = (simple_active_B.aSinInput >= 0.99999999999999778);
  mask2 = (simple_active_B.aSinInput <= -0.99999999999999778);
  if (mask1) {
    simple_active_B.aSinInput = 1.0;
  }

  if (mask2) {
    simple_active_B.aSinInput = -1.0;
  }

  mask1 = (mask1 || mask2);
  simple_active_B.Divide = simple_active_B.y[0] * simple_active_B.y[0];
  simple_active_B.q = simple_active_B.y[1] * simple_active_B.y[1];
  simple_active_B.euler_tmp = simple_active_B.y[2] * simple_active_B.y[2];
  euler_tmp = simple_active_B.y[3] * simple_active_B.y[3];
  simple_active_B.euler_p[0] = rt_atan2d_snf((simple_active_B.y[1] *
    simple_active_B.y[2] + simple_active_B.y[0] * simple_active_B.y[3]) * 2.0,
    ((simple_active_B.Divide + simple_active_B.q) - simple_active_B.euler_tmp) -
    euler_tmp);
  simple_active_B.euler_p[1] = asin(simple_active_B.aSinInput);
  simple_active_B.euler_p[2] = rt_atan2d_snf((simple_active_B.y[2] *
    simple_active_B.y[3] + simple_active_B.y[0] * simple_active_B.y[1]) * 2.0,
    ((simple_active_B.Divide - simple_active_B.q) - simple_active_B.euler_tmp) +
    euler_tmp);
  trueCount = 0;
  if (mask1) {
    trueCount = 1;
  }

  if (trueCount - 1 >= 0) {
    simple_active_B.x_data = simple_active_B.aSinInput;
  }

  for (k = 0; k < trueCount; k++) {
    if (rtIsNaN(simple_active_B.x_data)) {
      simple_active_B.Divide = (rtNaN);
    } else if (simple_active_B.x_data < 0.0) {
      simple_active_B.Divide = -1.0;
    } else {
      simple_active_B.Divide = (simple_active_B.x_data > 0.0);
    }

    simple_active_B.x_data = simple_active_B.Divide;
  }

  k = 0;
  if (mask1) {
    k = 1;
  }

  e_size_0[0] = 1;
  e_size_0[1] = k;
  if (mask1) {
    f_data_0 = 1;
  }

  if (trueCount == k) {
    if (trueCount - 1 >= 0) {
      simple_active_B.euler_p[0] = -simple_active_B.x_data * 2.0 * rt_atan2d_snf
        (simple_active_B.y[1], simple_active_B.y[0]);
    }
  } else {
    simple_active_binary_expand_op(simple_active_B.euler_p, &f_data_0,
      &simple_active_B.x_data, &trueCount, simple_active_B.y, e_size_0);
  }

  trueCount = 0;
  if (mask1) {
    trueCount = 1;
  }

  if (trueCount - 1 >= 0) {
    simple_active_B.euler_p[2] = 0.0;
  }

  // Sum: '<S2>/Current Error' incorporates:
  //   MATLAB Function: '<Root>/Quat to eul1'

  simple_active_B.dv[3] = simple_active_B.euler_p[2];
  simple_active_B.dv[4] = simple_active_B.euler_p[1];
  simple_active_B.dv[5] = simple_active_B.euler_p[0];

  // MATLAB Function: '<Root>/Quat to eul1'
  if (rtIsNaN(simple_active_B.euler_p[2])) {
    // Sum: '<S2>/Current Error'
    simple_active_B.dv[3] = 0.0;
    simple_active_B.dv[4] = 0.0;
    simple_active_B.dv[5] = 0.0;
  }

  // Sum: '<S2>/Current Error' incorporates:
  //   MATLAB Function: '<Root>/Quat to eul'

  simple_active_B.ControlSum[0] = simple_active_B.In1.pose.pose.position.x;
  simple_active_B.ControlSum[1] = simple_active_B.In1.pose.pose.position.y;
  simple_active_B.ControlSum[2] = simple_active_B.In1.pose.pose.position.z;
  simple_active_B.ControlSum[3] = simple_active_B.euler[2];
  simple_active_B.ControlSum[4] = simple_active_B.euler[1];
  simple_active_B.ControlSum[5] = simple_active_B.euler[0];
  simple_active_B.dv[0] = simple_active_B.In1_c.position.x;
  simple_active_B.dv[1] = simple_active_B.In1_c.position.y;
  simple_active_B.dv[2] = simple_active_B.In1_c.position.z;
  for (trueCount = 0; trueCount < 6; trueCount++) {
    simple_active_B.CurrentError[trueCount] =
      simple_active_B.ControlSum[trueCount] - simple_active_B.dv[trueCount];
  }

  // MATLAB Function: '<S2>/Normalize Angular Error'
  simple_active_B.normalizedError[0] = simple_active_B.CurrentError[0];
  simple_active_B.normalizedError[1] = simple_active_B.CurrentError[1];
  simple_active_B.normalizedError[2] = simple_active_B.CurrentError[2];
  if (rtIsNaN(simple_active_B.CurrentError[3] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (rtIsInf(simple_active_B.CurrentError[3] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (simple_active_B.CurrentError[3] + 3.1415926535897931 == 0.0) {
    simple_active_B.Divide = 0.0;
  } else {
    simple_active_B.Divide = fmod(simple_active_B.CurrentError[3] +
      3.1415926535897931, 6.2831853071795862);
    mask1 = (simple_active_B.Divide == 0.0);
    if (!mask1) {
      simple_active_B.q = fabs((simple_active_B.CurrentError[3] +
        3.1415926535897931) / 6.2831853071795862);
      mask1 = !(fabs(simple_active_B.q - floor(simple_active_B.q + 0.5)) >
                2.2204460492503131E-16 * simple_active_B.q);
    }

    if (mask1) {
      simple_active_B.Divide = 0.0;
    } else if (simple_active_B.CurrentError[3] + 3.1415926535897931 < 0.0) {
      simple_active_B.Divide += 6.2831853071795862;
    }
  }

  simple_active_B.normalizedError[3] = simple_active_B.Divide -
    3.1415926535897931;
  if (rtIsNaN(simple_active_B.CurrentError[4] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (rtIsInf(simple_active_B.CurrentError[4] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (simple_active_B.CurrentError[4] + 3.1415926535897931 == 0.0) {
    simple_active_B.Divide = 0.0;
  } else {
    simple_active_B.Divide = fmod(simple_active_B.CurrentError[4] +
      3.1415926535897931, 6.2831853071795862);
    mask1 = (simple_active_B.Divide == 0.0);
    if (!mask1) {
      simple_active_B.q = fabs((simple_active_B.CurrentError[4] +
        3.1415926535897931) / 6.2831853071795862);
      mask1 = !(fabs(simple_active_B.q - floor(simple_active_B.q + 0.5)) >
                2.2204460492503131E-16 * simple_active_B.q);
    }

    if (mask1) {
      simple_active_B.Divide = 0.0;
    } else if (simple_active_B.CurrentError[4] + 3.1415926535897931 < 0.0) {
      simple_active_B.Divide += 6.2831853071795862;
    }
  }

  simple_active_B.normalizedError[4] = simple_active_B.Divide -
    3.1415926535897931;
  if (rtIsNaN(simple_active_B.CurrentError[5] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (rtIsInf(simple_active_B.CurrentError[5] + 3.1415926535897931)) {
    simple_active_B.Divide = (rtNaN);
  } else if (simple_active_B.CurrentError[5] + 3.1415926535897931 == 0.0) {
    simple_active_B.Divide = 0.0;
  } else {
    simple_active_B.Divide = fmod(simple_active_B.CurrentError[5] +
      3.1415926535897931, 6.2831853071795862);
    mask1 = (simple_active_B.Divide == 0.0);
    if (!mask1) {
      simple_active_B.q = fabs((simple_active_B.CurrentError[5] +
        3.1415926535897931) / 6.2831853071795862);
      mask1 = !(fabs(simple_active_B.q - floor(simple_active_B.q + 0.5)) >
                2.2204460492503131E-16 * simple_active_B.q);
    }

    if (mask1) {
      simple_active_B.Divide = 0.0;
    } else if (simple_active_B.CurrentError[5] + 3.1415926535897931 < 0.0) {
      simple_active_B.Divide += 6.2831853071795862;
    }
  }

  simple_active_B.normalizedError[5] = simple_active_B.Divide -
    3.1415926535897931;

  // End of MATLAB Function: '<S2>/Normalize Angular Error'

  // MATLABSystem: '<Root>/Get Parameter3'
  ParamGet_simple_active_47.getParameter(6U, &simple_active_B.value_c[0], &len);

  // MATLABSystem: '<Root>/Get Parameter2'
  ParamGet_simple_active_46.getParameter(6U, &simple_active_B.value_k[0], &len);

  // MATLABSystem: '<Root>/Get Parameter'
  ParamGet_simple_active_43.getParameter(&simple_active_B.value_f);

  // Product: '<Root>/Divide' incorporates:
  //   DataTypeConversion: '<Root>/Force To Double3'
  //   DataTypeConversion: '<Root>/Force To Double4'
  //   MATLABSystem: '<Root>/Get Parameter'
  //   MATLABSystem: '<Root>/Get Parameter1'

  simple_active_B.Divide = static_cast<real_T>(simple_active_B.value_f) /
    static_cast<real_T>(simple_active_B.value_cv);

  // Product: '<S2>/D Control Negative'
  simple_active_B.dv[0] = simple_active_B.In1.twist.twist.linear.x;
  simple_active_B.dv[1] = simple_active_B.In1.twist.twist.linear.y;
  simple_active_B.dv[2] = simple_active_B.In1.twist.twist.linear.z;
  simple_active_B.dv[3] = simple_active_B.In1.twist.twist.angular.x;
  simple_active_B.dv[4] = simple_active_B.In1.twist.twist.angular.y;
  simple_active_B.dv[5] = simple_active_B.In1.twist.twist.angular.z;
  for (trueCount = 0; trueCount < 6; trueCount++) {
    // MATLAB Function: '<S2>/Manage Accumulated Error' incorporates:
    //   Memory: '<S2>/Accumulated Error'

    simple_active_B.q = simple_active_B.normalizedError[trueCount];
    simple_active_B.euler_tmp =
      simple_active_DW.AccumulatedError_PreviousInput[trueCount] +
      simple_active_B.q;
    if (simple_active_B.q > simple_active_B.Divide) {
      simple_active_B.euler_tmp = 0.0;
    }

    if (rtIsNaN(simple_active_B.q)) {
      simple_active_B.euler_tmp = 0.0;
    }

    // Sum: '<S2>/Control Sum' incorporates:
    //   DataTypeConversion: '<Root>/Force To Double'
    //   DataTypeConversion: '<Root>/Force To Double1'
    //   DataTypeConversion: '<Root>/Force To Double2'
    //   DataTypeConversion: '<Root>/Force To Double4'
    //   Gain: '<S2>/D Control'
    //   Gain: '<S2>/I Control'
    //   Gain: '<S2>/P Control'
    //   MATLABSystem: '<Root>/Get Parameter1'
    //   MATLABSystem: '<Root>/Get Parameter2'
    //   MATLABSystem: '<Root>/Get Parameter3'
    //   MATLABSystem: '<Root>/Get Parameter4'
    //   Product: '<Root>/Divide1'
    //   Product: '<Root>/Divide2'
    //   Product: '<Root>/Divide3'
    //   Product: '<S2>/D Control Negative'
    //   Product: '<S2>/I Control Negative'
    //   Product: '<S2>/P Control Negative'

    simple_active_B.ControlSum[trueCount] = (static_cast<real_T>
      (simple_active_B.value[trueCount]) / static_cast<real_T>
      (simple_active_B.value_cv) * simple_active_B.q *
      simple_active_P.PControl_Gain + static_cast<real_T>
      (simple_active_B.value_c[trueCount]) / static_cast<real_T>
      (simple_active_B.value_cv) * simple_active_B.dv[trueCount] *
      simple_active_P.DControl_Gain) + static_cast<real_T>
      (simple_active_B.value_k[trueCount]) / static_cast<real_T>
      (simple_active_B.value_cv) * simple_active_B.euler_tmp *
      simple_active_P.IControl_Gain;

    // MATLAB Function: '<S2>/Manage Accumulated Error'
    simple_active_B.CurrentError[trueCount] = simple_active_B.euler_tmp;
  }

  // BusAssignment: '<Root>/Bus Assignment'
  simple_active_B.BusAssignment.linear.x = simple_active_B.ControlSum[0];
  simple_active_B.BusAssignment.linear.y = simple_active_B.ControlSum[1];
  simple_active_B.BusAssignment.linear.z = simple_active_B.ControlSum[2];
  simple_active_B.BusAssignment.angular.x = simple_active_B.ControlSum[3];
  simple_active_B.BusAssignment.angular.y = simple_active_B.ControlSum[4];
  simple_active_B.BusAssignment.angular.z = simple_active_B.ControlSum[5];

  // MATLABSystem: '<S3>/SinkBlock'
  Pub_simple_active_53.publish(&simple_active_B.BusAssignment);

  // Update for Memory: '<S2>/Accumulated Error'
  for (trueCount = 0; trueCount < 6; trueCount++) {
    simple_active_DW.AccumulatedError_PreviousInput[trueCount] =
      simple_active_B.CurrentError[trueCount];
  }

  // End of Update for Memory: '<S2>/Accumulated Error'
}

// Model initialize function
void simple_active::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    char_T prmName_0[17];
    char_T prmName[7];
    static const char_T tmp[6] = { 'P', 'G', 'a', 'i', 'n', 's' };

    static const char_T tmp_0[18] = { 's', 'c', 'a', 'l', 'i', 'n', 'g', '_',
      'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's' };

    static const char_T tmp_1[6] = { 'D', 'G', 'a', 'i', 'n', 's' };

    static const char_T tmp_2[6] = { 'I', 'G', 'a', 'i', 'n', 's' };

    static const char_T tmp_3[16] = { 'r', 'e', 's', 'e', 't', '_', 't', 'h',
      'r', 'e', 's', 'h', 'h', 'o', 'l', 'd' };

    // SystemInitialize for Enabled SubSystem: '<S6>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S10>/In1' incorporates:
    //   Outport: '<S10>/Out1'

    simple_active_B.In1 = simple_active_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S6>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S7>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S11>/In1' incorporates:
    //   Outport: '<S11>/Out1'

    simple_active_B.In1_c = simple_active_P.Out1_Y0_d;

    // End of SystemInitialize for SubSystem: '<S7>/Enabled Subsystem'

    // Start for MATLABSystem: '<Root>/Get Parameter4'
    simple_active_DW.obj.matlabCodegenIsDeleted = false;
    simple_active_DW.obj.isInitialized = 1;
    prmName[6] = '\x00';
    for (int32_T i = 0; i < 6; i++) {
      // InitializeConditions for Memory: '<S2>/Accumulated Error'
      simple_active_DW.AccumulatedError_PreviousInput[i] =
        simple_active_P.AccumulatedError_InitialConditi;

      // Start for MATLABSystem: '<Root>/Get Parameter4'
      prmName[i] = tmp[i];
      simple_active_B.varargin_1[i] = 0L;
    }

    // Start for MATLABSystem: '<Root>/Get Parameter4'
    ParamGet_simple_active_48.initParam(&prmName[0]);
    ParamGet_simple_active_48.setInitialValue(&simple_active_B.varargin_1[0], 6U);
    simple_active_DW.obj.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/Get Parameter1'
    simple_active_DW.obj_j.matlabCodegenIsDeleted = false;
    simple_active_DW.obj_j.isInitialized = 1;
    for (int32_T i = 0; i < 18; i++) {
      simple_active_B.prmName[i] = tmp_0[i];
    }

    simple_active_B.prmName[18] = '\x00';
    ParamGet_simple_active_44.initParam(&simple_active_B.prmName[0]);
    ParamGet_simple_active_44.setInitialValue(1000000L);
    simple_active_DW.obj_j.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Parameter1'

    // Start for MATLABSystem: '<S6>/SourceBlock'
    simple_active_DW.obj_ex.isInitialized = 0;
    simple_active_DW.obj_ex.matlabCodegenIsDeleted = false;
    simple_activ_SystemCore_setup_g(&simple_active_DW.obj_ex);

    // Start for MATLABSystem: '<S7>/SourceBlock'
    simple_active_DW.obj_k.isInitialized = 0;
    simple_active_DW.obj_k.matlabCodegenIsDeleted = false;
    simple_acti_SystemCore_setup_gu(&simple_active_DW.obj_k);

    // Start for MATLABSystem: '<Root>/Get Parameter3'
    simple_active_DW.obj_e.matlabCodegenIsDeleted = false;
    simple_active_DW.obj_e.isInitialized = 1;
    prmName[6] = '\x00';
    for (int32_T i = 0; i < 6; i++) {
      prmName[i] = tmp_1[i];
      simple_active_B.varargin_1[i] = 0L;
    }

    ParamGet_simple_active_47.initParam(&prmName[0]);
    ParamGet_simple_active_47.setInitialValue(&simple_active_B.varargin_1[0], 6U);
    simple_active_DW.obj_e.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Parameter3'

    // Start for MATLABSystem: '<Root>/Get Parameter2'
    simple_active_DW.obj_n.matlabCodegenIsDeleted = false;
    simple_active_DW.obj_n.isInitialized = 1;
    prmName[6] = '\x00';
    for (int32_T i = 0; i < 6; i++) {
      prmName[i] = tmp_2[i];
      simple_active_B.varargin_1[i] = 0L;
    }

    ParamGet_simple_active_46.initParam(&prmName[0]);
    ParamGet_simple_active_46.setInitialValue(&simple_active_B.varargin_1[0], 6U);
    simple_active_DW.obj_n.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Parameter2'

    // Start for MATLABSystem: '<Root>/Get Parameter'
    simple_active_DW.obj_d.matlabCodegenIsDeleted = false;
    simple_active_DW.obj_d.isInitialized = 1;
    for (int32_T i = 0; i < 16; i++) {
      prmName_0[i] = tmp_3[i];
    }

    prmName_0[16] = '\x00';
    ParamGet_simple_active_43.initParam(&prmName_0[0]);
    ParamGet_simple_active_43.setInitialValue(100000L);
    simple_active_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Parameter'

    // Start for MATLABSystem: '<S3>/SinkBlock'
    simple_active_DW.obj_ng.isInitialized = 0;
    simple_active_DW.obj_ng.matlabCodegenIsDeleted = false;
    simple_active_SystemCore_setup(&simple_active_DW.obj_ng);
  }
}

// Model terminate function
void simple_active::terminate()
{
  // Terminate for MATLABSystem: '<Root>/Get Parameter4'
  if (!simple_active_DW.obj.matlabCodegenIsDeleted) {
    simple_active_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter4'

  // Terminate for MATLABSystem: '<Root>/Get Parameter1'
  if (!simple_active_DW.obj_j.matlabCodegenIsDeleted) {
    simple_active_DW.obj_j.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter1'

  // Terminate for MATLABSystem: '<S6>/SourceBlock'
  if (!simple_active_DW.obj_ex.matlabCodegenIsDeleted) {
    simple_active_DW.obj_ex.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S6>/SourceBlock'

  // Terminate for MATLABSystem: '<S7>/SourceBlock'
  if (!simple_active_DW.obj_k.matlabCodegenIsDeleted) {
    simple_active_DW.obj_k.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S7>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/Get Parameter3'
  if (!simple_active_DW.obj_e.matlabCodegenIsDeleted) {
    simple_active_DW.obj_e.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter3'

  // Terminate for MATLABSystem: '<Root>/Get Parameter2'
  if (!simple_active_DW.obj_n.matlabCodegenIsDeleted) {
    simple_active_DW.obj_n.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter2'

  // Terminate for MATLABSystem: '<Root>/Get Parameter'
  if (!simple_active_DW.obj_d.matlabCodegenIsDeleted) {
    simple_active_DW.obj_d.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter'

  // Terminate for MATLABSystem: '<S3>/SinkBlock'
  if (!simple_active_DW.obj_ng.matlabCodegenIsDeleted) {
    simple_active_DW.obj_ng.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/SinkBlock'
}

// Constructor
simple_active::simple_active() :
  simple_active_B(),
  simple_active_DW(),
  simple_active_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
simple_active::~simple_active()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_simple_active_T * simple_active::getRTM()
{
  return (&simple_active_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
