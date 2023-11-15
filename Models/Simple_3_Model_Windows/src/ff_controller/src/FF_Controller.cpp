//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: FF_Controller.cpp
//
// Code generated for Simulink model 'FF_Controller'.
//
// Model version                  : 1.3
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Sat Oct 21 17:56:54 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "FF_Controller.h"
#include "FF_Controller_types.h"
#include <math.h>
#include <emmintrin.h>
#include "rtwtypes.h"
#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include <stddef.h>
#include "FF_Controller_private.h"
#include "zero_crossing_types.h"

int64_T div_s64_floor(int64_T numerator, int64_T denominator)
{
  int64_T quotient;
  if (denominator == 0L) {
    quotient = numerator >= 0L ? MAX_int64_T : MIN_int64_T;

    // Divide by zero handler
  } else {
    uint64_T absDenominator;
    uint64_T absNumerator;
    uint64_T tempAbsQuotient;
    boolean_T quotientNeedsNegation;
    absNumerator = numerator < 0L ? ~static_cast<uint64_T>(numerator) + 1UL :
      static_cast<uint64_T>(numerator);
    absDenominator = denominator < 0L ? ~static_cast<uint64_T>(denominator) +
      1UL : static_cast<uint64_T>(denominator);
    quotientNeedsNegation = ((numerator < 0L) != (denominator < 0L));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0UL) {
        tempAbsQuotient++;
      }
    }

    quotient = quotientNeedsNegation ? -static_cast<int64_T>(tempAbsQuotient) :
      static_cast<int64_T>(tempAbsQuotient);
  }

  return quotient;
}

void FF_Controller::FF_Controlle_SystemCore_setup_l
  (ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[31] = { '/', 't', 'a', 'l', 'o', 's', '/', 'c', 'o',
    'n', 't', 'r', 'o', 'l', 'l', 'e', 'r', '/', 'F', 'F', '_', 'b', 'o', 'd',
    'y', '_', 'f', 'o', 'r', 'c', 'e' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 31; i++) {
    FF_Controller_B.b_zeroDelimTopic[i] = tmp[i];
  }

  FF_Controller_B.b_zeroDelimTopic[31] = '\x00';
  Pub_FF_Controller_59.createPublisher(&FF_Controller_B.b_zeroDelimTopic[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void FF_Controller::FF_Controller_SystemCore_setup
  (ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  char_T b_zeroDelimTopic[25];
  static const char_T tmp[24] = { '/', 't', 'a', 'l', 'o', 's', '/', 'o', 'd',
    'o', 'm', 'e', 't', 'r', 'y', '_', 'f', 'i', 'l', 't', 'e', 'r', 'e', 'd' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 24; i++) {
    b_zeroDelimTopic[i] = tmp[i];
  }

  b_zeroDelimTopic[24] = '\x00';
  Sub_FF_Controller_60.createSubscriber(&b_zeroDelimTopic[0], qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void FF_Controller::step()
{
  // MATLABSystem: '<S2>/SourceBlock'
  FF_Controller_B.b_varargout_1 = Sub_FF_Controller_60.getLatestMessage
    (&FF_Controller_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S8>/Enable'

  if (FF_Controller_B.b_varargout_1) {
    // SignalConversion generated from: '<S8>/In1'
    FF_Controller_B.In1 = FF_Controller_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

  // MATLABSystem: '<Root>/Scaling Factor'
  ParamGet_FF_Controller_71.getParameter(&FF_Controller_B.value_d);

  // MATLABSystem: '<Root>/COB'
  ParamGet_FF_Controller_67.getParameter(3U, &FF_Controller_B.value_c[0],
    &FF_Controller_B.len);

  // MATLABSystem: '<Root>/COM'
  ParamGet_FF_Controller_68.getParameter(3U, &FF_Controller_B.value_b[0],
    &FF_Controller_B.len);

  // MATLABSystem: '<Root>/Buoyant Force'
  ParamGet_FF_Controller_69.getParameter(&FF_Controller_B.value_g);

  // MATLABSystem: '<Root>/Mass'
  ParamGet_FF_Controller_70.getParameter(&FF_Controller_B.value_l);

  // Outputs for Triggered SubSystem: '<Root>/Calculate FF' incorporates:
  //   TriggerPort: '<S1>/Trigger'

  // MATLABSystem: '<S2>/SourceBlock' incorporates:
  //   Constant: '<S5>/Constant'
  //   Gain: '<S1>/Opposite'
  //   Product: '<S4>/World Frame Force'
  //   Product: '<S4>/World Frame Torque'
  //   SignalConversion generated from: '<S4>/World Frame Force'
  //   Sum: '<S7>/Sum'

  if (FF_Controller_B.b_varargout_1 &&
      (FF_Controller_PrevZCX.CalculateFF_Trig_ZCE != POS_ZCSIG)) {
    __m128d tmp;
    __m128d tmp_0;
    __m128d tmp_1;
    __m128d tmp_2;
    __m128d tmp_4;

    // MATLAB Function: '<S4>/Get Rot Matrix' incorporates:
    //   MATLABSystem: '<Root>/COB'
    //   MATLABSystem: '<Root>/COM'
    //   MATLABSystem: '<Root>/Scaling Factor'
    //   Product: '<Root>/Divide2'
    //   Product: '<Root>/Divide3'
    //   SignalConversion generated from: '<S6>/ SFunction '
    //   Sum: '<S5>/COM To COB'

    FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1 = 1.0 / sqrt
      (((FF_Controller_B.In1.pose.pose.orientation.w *
         FF_Controller_B.In1.pose.pose.orientation.w +
         FF_Controller_B.In1.pose.pose.orientation.x *
         FF_Controller_B.In1.pose.pose.orientation.x) +
        FF_Controller_B.In1.pose.pose.orientation.y *
        FF_Controller_B.In1.pose.pose.orientation.y) +
       FF_Controller_B.In1.pose.pose.orientation.z *
       FF_Controller_B.In1.pose.pose.orientation.z);
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_c =
      FF_Controller_B.In1.pose.pose.orientation.w *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_f =
      FF_Controller_B.In1.pose.pose.orientation.x *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_g =
      FF_Controller_B.In1.pose.pose.orientation.y *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1 *=
      FF_Controller_B.In1.pose.pose.orientation.z;
    FF_Controller_B.tempR_tmp_n =
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1 *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.tempR_tmp_j =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_g *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_g;
    FF_Controller_B.tempR[0] = 1.0 - (FF_Controller_B.tempR_tmp_j +
      FF_Controller_B.tempR_tmp_n) * 2.0;
    FF_Controller_B.tempR_tmp = FF_Controller_B.rtb_TmpSignalConversionAtSFun_f *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_g;
    FF_Controller_B.tempR_tmp_m =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.tempR[1] = (FF_Controller_B.tempR_tmp -
      FF_Controller_B.tempR_tmp_m) * 2.0;
    FF_Controller_B.tempR_tmp_p =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_f *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.tempR_tmp_l =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_g;
    FF_Controller_B.tempR[2] = (FF_Controller_B.tempR_tmp_p +
      FF_Controller_B.tempR_tmp_l) * 2.0;
    FF_Controller_B.tempR[3] = (FF_Controller_B.tempR_tmp +
      FF_Controller_B.tempR_tmp_m) * 2.0;
    FF_Controller_B.tempR_tmp = FF_Controller_B.rtb_TmpSignalConversionAtSFun_f *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_f;
    FF_Controller_B.tempR[4] = 1.0 - (FF_Controller_B.tempR_tmp +
      FF_Controller_B.tempR_tmp_n) * 2.0;
    FF_Controller_B.tempR_tmp_n =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_g *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_g1;
    FF_Controller_B.tempR_tmp_m =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_f;
    FF_Controller_B.tempR[5] = (FF_Controller_B.tempR_tmp_n -
      FF_Controller_B.tempR_tmp_m) * 2.0;
    FF_Controller_B.tempR[6] = (FF_Controller_B.tempR_tmp_p -
      FF_Controller_B.tempR_tmp_l) * 2.0;
    FF_Controller_B.tempR[7] = (FF_Controller_B.tempR_tmp_n +
      FF_Controller_B.tempR_tmp_m) * 2.0;
    FF_Controller_B.tempR[8] = 1.0 - (FF_Controller_B.tempR_tmp +
      FF_Controller_B.tempR_tmp_j) * 2.0;
    for (FF_Controller_B.k = 0; FF_Controller_B.k < 3; FF_Controller_B.k++) {
      FF_Controller_B.world2body_tmp = (static_cast<int8_T>(FF_Controller_B.k +
        1) - 1) * 3;
      FF_Controller_B.world2body[static_cast<int8_T>(FF_Controller_B.k + 1) - 1]
        = FF_Controller_B.tempR[FF_Controller_B.world2body_tmp];
      FF_Controller_B.world2body[static_cast<int8_T>(FF_Controller_B.k + 1) + 2]
        = FF_Controller_B.tempR[FF_Controller_B.world2body_tmp + 1];
      FF_Controller_B.world2body[static_cast<int8_T>(FF_Controller_B.k + 1) + 5]
        = FF_Controller_B.tempR[FF_Controller_B.world2body_tmp + 2];
    }

    for (FF_Controller_B.k = 0; FF_Controller_B.k < 3; FF_Controller_B.k++) {
      FF_Controller_B.tempR[3 * FF_Controller_B.k] =
        FF_Controller_B.world2body[FF_Controller_B.k];
      FF_Controller_B.tempR[3 * FF_Controller_B.k + 1] =
        FF_Controller_B.world2body[FF_Controller_B.k + 3];
      FF_Controller_B.tempR[3 * FF_Controller_B.k + 2] =
        FF_Controller_B.world2body[FF_Controller_B.k + 6];
      FF_Controller_B.value_c[FF_Controller_B.k] = div_s64_floor
        (FF_Controller_B.value_c[FF_Controller_B.k], FF_Controller_B.value_d) -
        div_s64_floor(FF_Controller_B.value_b[FF_Controller_B.k],
                      FF_Controller_B.value_d);
    }

    // End of MATLAB Function: '<S4>/Get Rot Matrix'

    // Sum: '<S5>/Subtract' incorporates:
    //   Constant: '<S5>/Constant3'
    //   MATLABSystem: '<Root>/Buoyant Force'
    //   MATLABSystem: '<Root>/Mass'
    //   MATLABSystem: '<Root>/Scaling Factor'
    //   Product: '<Root>/Divide'
    //   Product: '<Root>/Divide1'
    //   Product: '<S5>/Gravitational Force'

    FF_Controller_B.rtb_TmpSignalConversionAtSFun_c = static_cast<real_T>
      (div_s64_floor(FF_Controller_B.value_g, FF_Controller_B.value_d)) -
      static_cast<real_T>(div_s64_floor(FF_Controller_B.value_l,
      FF_Controller_B.value_d)) * FF_Controller_P.Constant3_Value;

    // Product: '<S7>/Element Product' incorporates:
    //   Constant: '<S5>/Constant'

    FF_Controller_B.value[2] = FF_Controller_B.value_c[0];
    FF_Controller_B.value[3] = FF_Controller_B.value_c[2];
    FF_Controller_B.value[0] = FF_Controller_B.value_c[1];
    FF_Controller_B.value[4] = FF_Controller_B.value_c[0];
    FF_Controller_B.value[1] = FF_Controller_B.value_c[2];
    FF_Controller_B.value[5] = FF_Controller_B.value_c[1];
    FF_Controller_B.rtb_Subtract_m[0] =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    FF_Controller_B.rtb_Subtract_m[1] = FF_Controller_P.Constant_Value_c1;
    FF_Controller_B.rtb_Subtract_m[2] = FF_Controller_P.Constant_Value_c1;
    FF_Controller_B.rtb_Subtract_m[3] = FF_Controller_P.Constant_Value_c1;
    FF_Controller_B.rtb_Subtract_m[4] =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    FF_Controller_B.rtb_Subtract_m[5] = FF_Controller_P.Constant_Value_c1;
    for (FF_Controller_B.k = 0; FF_Controller_B.k < 6; FF_Controller_B.k++) {
      FF_Controller_B.ElementProduct[FF_Controller_B.k] = static_cast<real_T>
        (FF_Controller_B.value[FF_Controller_B.k]) *
        FF_Controller_B.rtb_Subtract_m[FF_Controller_B.k];
    }

    // End of Product: '<S7>/Element Product'
    for (FF_Controller_B.k = 0; FF_Controller_B.k <= 0; FF_Controller_B.k += 2)
    {
      __m128d tmp_3;
      tmp = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k]);
      tmp_4 = _mm_set1_pd(FF_Controller_P.Constant_Value_c1);
      tmp_0 = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k + 3]);
      tmp_1 = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k + 6]);
      tmp_2 = _mm_loadu_pd(&FF_Controller_B.ElementProduct[FF_Controller_B.k]);
      tmp_3 = _mm_loadu_pd(&FF_Controller_B.ElementProduct[FF_Controller_B.k + 3]);
      _mm_storeu_pd(&FF_Controller_B.rtb_ElementProduct_k[FF_Controller_B.k],
                    _mm_sub_pd(tmp_2, tmp_3));
      _mm_storeu_pd(&FF_Controller_B.tempR_c[FF_Controller_B.k], _mm_add_pd
                    (_mm_mul_pd(tmp_1, _mm_set1_pd
        (FF_Controller_B.rtb_TmpSignalConversionAtSFun_c)), _mm_add_pd
                     (_mm_mul_pd(tmp_0, tmp_4), _mm_add_pd(_mm_mul_pd(tmp, tmp_4),
        _mm_set1_pd(0.0)))));
    }

    for (FF_Controller_B.k = 2; FF_Controller_B.k < 3; FF_Controller_B.k++) {
      // Product: '<S4>/World Frame Torque' incorporates:
      //   Sum: '<S7>/Sum'

      FF_Controller_B.rtb_ElementProduct_k[FF_Controller_B.k] =
        FF_Controller_B.ElementProduct[FF_Controller_B.k] -
        FF_Controller_B.ElementProduct[FF_Controller_B.k + 3];
      FF_Controller_B.tempR_c[FF_Controller_B.k] =
        (FF_Controller_B.tempR[FF_Controller_B.k + 3] *
         FF_Controller_P.Constant_Value_c1 +
         FF_Controller_B.tempR[FF_Controller_B.k] *
         FF_Controller_P.Constant_Value_c1) +
        FF_Controller_B.tempR[FF_Controller_B.k + 6] *
        FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    }

    for (FF_Controller_B.k = 0; FF_Controller_B.k <= 0; FF_Controller_B.k += 2)
    {
      tmp = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k]);
      tmp_4 = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k + 3]);
      tmp_0 = _mm_loadu_pd(&FF_Controller_B.tempR[FF_Controller_B.k + 6]);
      tmp_1 = _mm_loadu_pd(&FF_Controller_B.tempR_c[FF_Controller_B.k]);
      tmp_2 = _mm_set1_pd(FF_Controller_P.Opposite_Gain);
      _mm_storeu_pd(&FF_Controller_B.Opposite[FF_Controller_B.k], _mm_mul_pd
                    (tmp_2, tmp_1));
      _mm_storeu_pd(&FF_Controller_B.Opposite[FF_Controller_B.k + 3], _mm_mul_pd
                    (tmp_2, _mm_add_pd(_mm_mul_pd(tmp_0, _mm_set1_pd
        (FF_Controller_B.rtb_ElementProduct_k[2])), _mm_add_pd(_mm_mul_pd(tmp_4,
        _mm_set1_pd(FF_Controller_B.rtb_ElementProduct_k[1])), _mm_add_pd
        (_mm_mul_pd(tmp, _mm_set1_pd(FF_Controller_B.rtb_ElementProduct_k[0])),
         _mm_set1_pd(0.0))))));
    }

    for (FF_Controller_B.k = 2; FF_Controller_B.k < 3; FF_Controller_B.k++) {
      // Gain: '<S1>/Opposite' incorporates:
      //   Product: '<S4>/World Frame Torque'

      FF_Controller_B.Opposite[FF_Controller_B.k] =
        FF_Controller_P.Opposite_Gain *
        FF_Controller_B.tempR_c[FF_Controller_B.k];
      FF_Controller_B.Opposite[FF_Controller_B.k + 3] =
        ((FF_Controller_B.tempR[FF_Controller_B.k + 3] *
          FF_Controller_B.rtb_ElementProduct_k[1] +
          FF_Controller_B.tempR[FF_Controller_B.k] *
          FF_Controller_B.rtb_ElementProduct_k[0]) +
         FF_Controller_B.tempR[FF_Controller_B.k + 6] *
         FF_Controller_B.rtb_ElementProduct_k[2]) *
        FF_Controller_P.Opposite_Gain;
    }
  }

  FF_Controller_PrevZCX.CalculateFF_Trig_ZCE = FF_Controller_B.b_varargout_1;

  // End of Outputs for SubSystem: '<Root>/Calculate FF'

  // Outputs for Triggered SubSystem: '<Root>/Publish' incorporates:
  //   TriggerPort: '<S3>/Trigger'

  if (FF_Controller_B.b_varargout_1 && (FF_Controller_PrevZCX.Publish_Trig_ZCE
       != POS_ZCSIG)) {
    // BusAssignment: '<S3>/Bus Assignment'
    FF_Controller_B.BusAssignment.linear.x = FF_Controller_B.Opposite[0];
    FF_Controller_B.BusAssignment.linear.y = FF_Controller_B.Opposite[1];
    FF_Controller_B.BusAssignment.linear.z = FF_Controller_B.Opposite[2];
    FF_Controller_B.BusAssignment.angular.x = FF_Controller_B.Opposite[3];
    FF_Controller_B.BusAssignment.angular.y = FF_Controller_B.Opposite[4];
    FF_Controller_B.BusAssignment.angular.z = FF_Controller_B.Opposite[5];

    // MATLABSystem: '<S10>/SinkBlock'
    Pub_FF_Controller_59.publish(&FF_Controller_B.BusAssignment);
  }

  FF_Controller_PrevZCX.Publish_Trig_ZCE = FF_Controller_B.b_varargout_1;

  // End of Outputs for SubSystem: '<Root>/Publish'
}

// Model initialize function
void FF_Controller::initialize()
{
  {
    char_T prmName_0[11];
    char_T prmName[10];
    static const char_T tmp[30] = { 'F', 'F', '_', 'C', 'o', 'n', 't', 'r', 'o',
      'l', 'l', 'e', 'r', '_', 'S', 'c', 'a', 'l', 'i', 'n', 'g', '_', 'P', 'a',
      'r', 'a', 'm', 't', 'e', 'r' };

    static const char_T tmp_0[9] = { 'T', 'a', 'l', 'o', 's', '_', 'C', 'O', 'B'
    };

    static const char_T tmp_1[9] = { 'T', 'a', 'l', 'o', 's', '_', 'C', 'O', 'M'
    };

    static const char_T tmp_2[19] = { 'T', 'a', 'l', 'o', 's', '_', 'B', 'u',
      'o', 'y', 'a', 'n', 't', '_', 'F', 'o', 'r', 'c', 'e' };

    static const char_T tmp_3[10] = { 'T', 'a', 'l', 'o', 's', '_', 'M', 'a',
      's', 's' };

    FF_Controller_PrevZCX.CalculateFF_Trig_ZCE = POS_ZCSIG;
    FF_Controller_PrevZCX.Publish_Trig_ZCE = POS_ZCSIG;

    // SystemInitialize for Enabled SubSystem: '<S2>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S8>/In1' incorporates:
    //   Outport: '<S8>/Out1'

    FF_Controller_B.In1 = FF_Controller_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S2>/Enabled Subsystem'

    // SystemInitialize for Triggered SubSystem: '<Root>/Calculate FF'
    for (int32_T i = 0; i < 6; i++) {
      // SystemInitialize for Gain: '<S1>/Opposite' incorporates:
      //   Outport: '<S1>/FF Twist'

      FF_Controller_B.Opposite[i] = FF_Controller_P.FFTwist_Y0;
    }

    // End of SystemInitialize for SubSystem: '<Root>/Calculate FF'

    // SystemInitialize for Triggered SubSystem: '<Root>/Publish'
    // Start for MATLABSystem: '<S10>/SinkBlock'
    FF_Controller_DW.obj_n3.isInitialized = 0;
    FF_Controller_DW.obj_n3.matlabCodegenIsDeleted = false;
    FF_Controlle_SystemCore_setup_l(&FF_Controller_DW.obj_n3);

    // End of SystemInitialize for SubSystem: '<Root>/Publish'

    // Start for MATLABSystem: '<S2>/SourceBlock'
    FF_Controller_DW.obj_c.isInitialized = 0;
    FF_Controller_DW.obj_c.matlabCodegenIsDeleted = false;
    FF_Controller_SystemCore_setup(&FF_Controller_DW.obj_c);

    // Start for MATLABSystem: '<Root>/Scaling Factor'
    FF_Controller_DW.obj.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj.isInitialized = 1;
    for (int32_T i = 0; i < 30; i++) {
      FF_Controller_B.prmName[i] = tmp[i];
    }

    FF_Controller_B.prmName[30] = '\x00';
    ParamGet_FF_Controller_71.initParam(&FF_Controller_B.prmName[0]);
    ParamGet_FF_Controller_71.setInitialValue(1L);
    FF_Controller_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Scaling Factor'

    // Start for MATLABSystem: '<Root>/COB'
    FF_Controller_DW.obj_e.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_e.isInitialized = 1;
    for (int32_T i = 0; i < 9; i++) {
      prmName[i] = tmp_0[i];
    }

    prmName[9] = '\x00';
    FF_Controller_B.varargin_1[0] = 0L;
    FF_Controller_B.varargin_1[1] = 0L;
    FF_Controller_B.varargin_1[2] = 0L;
    ParamGet_FF_Controller_67.initParam(&prmName[0]);
    ParamGet_FF_Controller_67.setInitialValue(&FF_Controller_B.varargin_1[0], 3U);
    FF_Controller_DW.obj_e.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/COB'

    // Start for MATLABSystem: '<Root>/COM'
    FF_Controller_DW.obj_ny.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_ny.isInitialized = 1;
    for (int32_T i = 0; i < 9; i++) {
      prmName[i] = tmp_1[i];
    }

    prmName[9] = '\x00';
    FF_Controller_B.varargin_1[0] = 0L;
    FF_Controller_B.varargin_1[1] = 0L;
    FF_Controller_B.varargin_1[2] = 0L;
    ParamGet_FF_Controller_68.initParam(&prmName[0]);
    ParamGet_FF_Controller_68.setInitialValue(&FF_Controller_B.varargin_1[0], 3U);
    FF_Controller_DW.obj_ny.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/COM'

    // Start for MATLABSystem: '<Root>/Buoyant Force'
    FF_Controller_DW.obj_g.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_g.isInitialized = 1;
    for (int32_T i = 0; i < 19; i++) {
      FF_Controller_B.prmName_p[i] = tmp_2[i];
    }

    FF_Controller_B.prmName_p[19] = '\x00';
    ParamGet_FF_Controller_69.initParam(&FF_Controller_B.prmName_p[0]);
    ParamGet_FF_Controller_69.setInitialValue(0L);
    FF_Controller_DW.obj_g.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Buoyant Force'

    // Start for MATLABSystem: '<Root>/Mass'
    FF_Controller_DW.obj_n.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_n.isInitialized = 1;
    for (int32_T i = 0; i < 10; i++) {
      prmName_0[i] = tmp_3[i];
    }

    prmName_0[10] = '\x00';
    ParamGet_FF_Controller_70.initParam(&prmName_0[0]);
    ParamGet_FF_Controller_70.setInitialValue(0L);
    FF_Controller_DW.obj_n.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Mass'
  }
}

// Model terminate function
void FF_Controller::terminate()
{
  // Terminate for MATLABSystem: '<S2>/SourceBlock'
  if (!FF_Controller_DW.obj_c.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_c.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S2>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/Scaling Factor'
  if (!FF_Controller_DW.obj.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Scaling Factor'

  // Terminate for MATLABSystem: '<Root>/COB'
  if (!FF_Controller_DW.obj_e.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_e.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/COB'

  // Terminate for MATLABSystem: '<Root>/COM'
  if (!FF_Controller_DW.obj_ny.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_ny.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/COM'

  // Terminate for MATLABSystem: '<Root>/Buoyant Force'
  if (!FF_Controller_DW.obj_g.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_g.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Buoyant Force'

  // Terminate for MATLABSystem: '<Root>/Mass'
  if (!FF_Controller_DW.obj_n.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_n.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Mass'

  // Terminate for Triggered SubSystem: '<Root>/Publish'
  // Terminate for MATLABSystem: '<S10>/SinkBlock'
  if (!FF_Controller_DW.obj_n3.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_n3.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S10>/SinkBlock'
  // End of Terminate for SubSystem: '<Root>/Publish'
}

// Constructor
FF_Controller::FF_Controller() :
  FF_Controller_B(),
  FF_Controller_DW(),
  FF_Controller_PrevZCX(),
  FF_Controller_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
FF_Controller::~FF_Controller()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_FF_Controller_T * FF_Controller::getRTM()
{
  return (&FF_Controller_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
