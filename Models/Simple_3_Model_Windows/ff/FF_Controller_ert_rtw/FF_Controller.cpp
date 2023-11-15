//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: FF_Controller.cpp
//
// Code generated for Simulink model 'FF_Controller'.
//
// Model version                  : 1.9
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Fri Oct 27 02:44:30 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LLP64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "FF_Controller.h"
#include "FF_Controller_types.h"
#include <math.h>
#include "rtwtypes.h"
#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include <stddef.h>
#include "zero_crossing_types.h"

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
  static const char_T tmp[24] = { '/', 't', 'a', 'l', 'o', 's', '/', 'o', 'd',
    'o', 'm', 'e', 't', 'r', 'y', '/', 'f', 'i', 'l', 't', 'e', 'r', 'e', 'd' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 24; i++) {
    FF_Controller_B.b_zeroDelimTopic_m[i] = tmp[i];
  }

  FF_Controller_B.b_zeroDelimTopic_m[24] = '\x00';
  Sub_FF_Controller_60.createSubscriber(&FF_Controller_B.b_zeroDelimTopic_m[0],
    qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void FF_Controller::step()
{
  uint32_T len;
  boolean_T b_varargout_1;

  // MATLABSystem: '<S2>/SourceBlock'
  b_varargout_1 = Sub_FF_Controller_60.getLatestMessage
    (&FF_Controller_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S8>/Enable'

  if (b_varargout_1) {
    // SignalConversion generated from: '<S8>/In1'
    FF_Controller_B.In1 = FF_Controller_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

  // MATLABSystem: '<Root>/Scaling Factor'
  ParamGet_FF_Controller_71.getParameter(&FF_Controller_B.value_f);

  // MATLABSystem: '<Root>/COB'
  ParamGet_FF_Controller_67.getParameter(3U, &FF_Controller_B.value[0], &len);

  // MATLABSystem: '<Root>/COM'
  ParamGet_FF_Controller_68.getParameter(3U, &FF_Controller_B.value_k[0], &len);

  // MATLABSystem: '<Root>/Buoyant Force'
  ParamGet_FF_Controller_69.getParameter(&FF_Controller_B.value_g);

  // MATLABSystem: '<Root>/Mass'
  ParamGet_FF_Controller_70.getParameter(&FF_Controller_B.value_g1);

  // Outputs for Triggered SubSystem: '<Root>/Calculate FF' incorporates:
  //   TriggerPort: '<S1>/Trigger'

  // MATLABSystem: '<S2>/SourceBlock' incorporates:
  //   Constant: '<S6>/Constant'
  //   Product: '<S4>/World Frame Force'
  //   SignalConversion generated from: '<S4>/World Frame Force'

  if (b_varargout_1 && (FF_Controller_PrevZCX.CalculateFF_Trig_ZCE != POS_ZCSIG))
  {
    real_T tempR_tmp;
    real_T tempR_tmp_0;
    real_T tempR_tmp_1;
    real_T tempR_tmp_2;
    real_T tempR_tmp_3;
    real_T tempR_tmp_4;

    // MATLAB Function: '<S4>/Body 2 World Rotm' incorporates:
    //   SignalConversion generated from: '<S5>/ SFunction '

    FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv = 1.0 / sqrt
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
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_b =
      FF_Controller_B.In1.pose.pose.orientation.x *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_p =
      FF_Controller_B.In1.pose.pose.orientation.y *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv *=
      FF_Controller_B.In1.pose.pose.orientation.z;
    tempR_tmp_1 = FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    tempR_tmp_4 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_p *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_p;
    FF_Controller_B.tempR[0] = 1.0 - (tempR_tmp_4 + tempR_tmp_1) * 2.0;
    tempR_tmp = FF_Controller_B.rtb_TmpSignalConversionAtSFun_b *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_p;
    tempR_tmp_0 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    FF_Controller_B.tempR[1] = (tempR_tmp - tempR_tmp_0) * 2.0;
    tempR_tmp_2 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_b *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    tempR_tmp_3 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_p;
    FF_Controller_B.tempR[2] = (tempR_tmp_2 + tempR_tmp_3) * 2.0;
    FF_Controller_B.tempR[3] = (tempR_tmp + tempR_tmp_0) * 2.0;
    tempR_tmp = FF_Controller_B.rtb_TmpSignalConversionAtSFun_b *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_b;
    FF_Controller_B.tempR[4] = 1.0 - (tempR_tmp + tempR_tmp_1) * 2.0;
    tempR_tmp_1 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_p *
      FF_Controller_B.rtb_TmpSignalConversionAtSFu_cv;
    tempR_tmp_0 = FF_Controller_B.rtb_TmpSignalConversionAtSFun_c *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_b;
    FF_Controller_B.tempR[5] = (tempR_tmp_1 - tempR_tmp_0) * 2.0;
    FF_Controller_B.tempR[6] = (tempR_tmp_2 - tempR_tmp_3) * 2.0;
    FF_Controller_B.tempR[7] = (tempR_tmp_1 + tempR_tmp_0) * 2.0;
    FF_Controller_B.tempR[8] = 1.0 - (tempR_tmp + tempR_tmp_4) * 2.0;
    for (int32_T k = 0; k < 3; k++) {
      int32_T world2body_tmp;
      world2body_tmp = (static_cast<int8_T>(k + 1) - 1) * 3;
      FF_Controller_B.world2body[static_cast<int8_T>(k + 1) - 1] =
        FF_Controller_B.tempR[world2body_tmp];
      FF_Controller_B.world2body[static_cast<int8_T>(k + 1) + 2] =
        FF_Controller_B.tempR[world2body_tmp + 1];
      FF_Controller_B.world2body[static_cast<int8_T>(k + 1) + 5] =
        FF_Controller_B.tempR[world2body_tmp + 2];
    }

    for (int32_T k = 0; k < 3; k++) {
      FF_Controller_B.tempR[3 * k] = FF_Controller_B.world2body[k];
      FF_Controller_B.tempR[3 * k + 1] = FF_Controller_B.world2body[k + 3];
      FF_Controller_B.tempR[3 * k + 2] = FF_Controller_B.world2body[k + 6];

      // Sum: '<S6>/COM To COB' incorporates:
      //   DataTypeConversion: '<Root>/Force to Double1'
      //   DataTypeConversion: '<Root>/Force to Double4'
      //   DataTypeConversion: '<Root>/Force to Double5'
      //   MATLABSystem: '<Root>/COB'
      //   MATLABSystem: '<Root>/COM'
      //   MATLABSystem: '<Root>/Scaling Factor'
      //   Math: '<S4>/Transpose'
      //   Product: '<Root>/Divide2'
      //   Product: '<Root>/Divide3'

      FF_Controller_B.Transpose[k] = static_cast<real_T>(FF_Controller_B.value[k])
        / static_cast<real_T>(FF_Controller_B.value_f) - static_cast<real_T>
        (FF_Controller_B.value_k[k]) / static_cast<real_T>
        (FF_Controller_B.value_f);
    }

    // End of MATLAB Function: '<S4>/Body 2 World Rotm'

    // Sum: '<S6>/Subtract' incorporates:
    //   Constant: '<S6>/Constant3'
    //   DataTypeConversion: '<Root>/Force to Double'
    //   DataTypeConversion: '<Root>/Force to Double1'
    //   DataTypeConversion: '<Root>/Force to Double3'
    //   MATLABSystem: '<Root>/Buoyant Force'
    //   MATLABSystem: '<Root>/Mass'
    //   MATLABSystem: '<Root>/Scaling Factor'
    //   Product: '<Root>/Divide'
    //   Product: '<Root>/Divide1'
    //   Product: '<S6>/Gravitational Force'

    FF_Controller_B.rtb_TmpSignalConversionAtSFun_c = static_cast<real_T>
      (FF_Controller_B.value_g) / static_cast<real_T>(FF_Controller_B.value_f) -
      static_cast<real_T>(FF_Controller_B.value_g1) / static_cast<real_T>
      (FF_Controller_B.value_f) * FF_Controller_P.Constant3_Value;

    // Product: '<S7>/Element Product' incorporates:
    //   Constant: '<S6>/Constant'

    FF_Controller_B.ElementProduct[0] = FF_Controller_B.Transpose[1] *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    FF_Controller_B.rtb_TmpSignalConversionAtSFun_b = FF_Controller_B.Transpose
      [2] * FF_Controller_P.Constant_Value_b;
    FF_Controller_B.ElementProduct[1] =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_b;
    FF_Controller_B.ElementProduct[2] = FF_Controller_B.Transpose[0] *
      FF_Controller_P.Constant_Value_b;
    FF_Controller_B.ElementProduct[3] =
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_b;
    FF_Controller_B.ElementProduct[4] = FF_Controller_B.Transpose[0] *
      FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    FF_Controller_B.ElementProduct[5] = FF_Controller_B.Transpose[1] *
      FF_Controller_P.Constant_Value_b;
    for (int32_T k = 0; k < 3; k++) {
      // Product: '<S4>/World Frame Torque' incorporates:
      //   Sum: '<S7>/Sum'

      FF_Controller_B.rtb_ElementProduct_c[k] = FF_Controller_B.ElementProduct[k]
        - FF_Controller_B.ElementProduct[k + 3];
      FF_Controller_B.Transpose[k] = (FF_Controller_B.tempR[k + 3] *
        FF_Controller_P.Constant_Value_b + FF_Controller_B.tempR[k] *
        FF_Controller_P.Constant_Value_b) + FF_Controller_B.tempR[k + 6] *
        FF_Controller_B.rtb_TmpSignalConversionAtSFun_c;
    }

    for (int32_T k = 0; k < 3; k++) {
      // Gain: '<S1>/Opposite' incorporates:
      //   Product: '<S4>/World Frame Torque'

      FF_Controller_B.Opposite[k] = FF_Controller_P.Opposite_Gain *
        FF_Controller_B.Transpose[k];
      FF_Controller_B.Opposite[k + 3] = ((FF_Controller_B.tempR[k + 3] *
        FF_Controller_B.rtb_ElementProduct_c[1] + FF_Controller_B.tempR[k] *
        FF_Controller_B.rtb_ElementProduct_c[0]) + FF_Controller_B.tempR[k + 6] *
        FF_Controller_B.rtb_ElementProduct_c[2]) * FF_Controller_P.Opposite_Gain;
    }
  }

  FF_Controller_PrevZCX.CalculateFF_Trig_ZCE = b_varargout_1;

  // End of Outputs for SubSystem: '<Root>/Calculate FF'

  // MATLABSystem: '<Root>/Get Parameter'
  ParamGet_FF_Controller_80.getParameter(&FF_Controller_B.value_g);

  // Outputs for Triggered SubSystem: '<Root>/Publish' incorporates:
  //   TriggerPort: '<S3>/Trigger'

  // MATLABSystem: '<S2>/SourceBlock'
  if (b_varargout_1 && (FF_Controller_PrevZCX.Publish_Trig_ZCE != POS_ZCSIG)) {
    // BusAssignment: '<S3>/Bus Assignment' incorporates:
    //   DataTypeConversion: '<Root>/Force to Double1'
    //   DataTypeConversion: '<Root>/Force to Double2'
    //   MATLABSystem: '<Root>/Get Parameter'
    //   MATLABSystem: '<Root>/Scaling Factor'
    //   Product: '<Root>/Divide4'
    //   Sum: '<S3>/Add'

    FF_Controller_B.BusAssignment.linear.x = FF_Controller_B.Opposite[0];
    FF_Controller_B.BusAssignment.linear.y = FF_Controller_B.Opposite[1];
    FF_Controller_B.BusAssignment.linear.z = FF_Controller_B.Opposite[2];
    FF_Controller_B.BusAssignment.angular.x = FF_Controller_B.Opposite[3];
    FF_Controller_B.BusAssignment.angular.y = FF_Controller_B.Opposite[4];
    FF_Controller_B.BusAssignment.angular.z = static_cast<real_T>
      (FF_Controller_B.value_g) / static_cast<real_T>(FF_Controller_B.value_f) +
      FF_Controller_B.Opposite[5];

    // MATLABSystem: '<S10>/SinkBlock'
    Pub_FF_Controller_59.publish(&FF_Controller_B.BusAssignment);
  }

  FF_Controller_PrevZCX.Publish_Trig_ZCE = b_varargout_1;

  // End of Outputs for SubSystem: '<Root>/Publish'
}

// Model initialize function
void FF_Controller::initialize()
{
  {
    char_T prmName_0[14];
    char_T prmName_1[5];
    char_T prmName[4];
    static const char_T tmp[16] = { 'S', 'c', 'a', 'l', 'i', 'n', 'g', '_', 'P',
      'a', 'r', 'a', 'm', 't', 'e', 'r' };

    static const char_T tmp_0[13] = { 'B', 'u', 'o', 'y', 'a', 'n', 't', '_',
      'F', 'o', 'r', 'c', 'e' };

    static const char_T tmp_1[16] = { 'Y', 'a', 'w', '_', 'C', 'o', 'm', 'p',
      'e', 'n', 's', 'a', 't', 'i', 'o', 'n' };

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
    for (int32_T i = 0; i < 16; i++) {
      FF_Controller_B.prmName[i] = tmp[i];
    }

    FF_Controller_B.prmName[16] = '\x00';
    ParamGet_FF_Controller_71.initParam(&FF_Controller_B.prmName[0]);
    ParamGet_FF_Controller_71.setInitialValue(1LL);
    FF_Controller_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Scaling Factor'

    // Start for MATLABSystem: '<Root>/COB'
    FF_Controller_DW.obj_e.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_e.isInitialized = 1;
    prmName[3] = '\x00';
    prmName[0] = 'C';
    FF_Controller_B.varargin_1[0] = 0LL;
    prmName[1] = 'O';
    FF_Controller_B.varargin_1[1] = 0LL;
    prmName[2] = 'B';
    FF_Controller_B.varargin_1[2] = 0LL;
    ParamGet_FF_Controller_67.initParam(&prmName[0]);
    ParamGet_FF_Controller_67.setInitialValue(&FF_Controller_B.varargin_1[0], 3U);
    FF_Controller_DW.obj_e.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/COM'
    FF_Controller_DW.obj_ny.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_ny.isInitialized = 1;
    prmName[3] = '\x00';
    prmName[0] = 'C';
    FF_Controller_B.varargin_1[0] = 0LL;
    prmName[1] = 'O';
    FF_Controller_B.varargin_1[1] = 0LL;
    prmName[2] = 'M';
    FF_Controller_B.varargin_1[2] = 0LL;
    ParamGet_FF_Controller_68.initParam(&prmName[0]);
    ParamGet_FF_Controller_68.setInitialValue(&FF_Controller_B.varargin_1[0], 3U);
    FF_Controller_DW.obj_ny.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/Buoyant Force'
    FF_Controller_DW.obj_g.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_g.isInitialized = 1;
    for (int32_T i = 0; i < 13; i++) {
      prmName_0[i] = tmp_0[i];
    }

    prmName_0[13] = '\x00';
    ParamGet_FF_Controller_69.initParam(&prmName_0[0]);
    ParamGet_FF_Controller_69.setInitialValue(0LL);
    FF_Controller_DW.obj_g.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Buoyant Force'

    // Start for MATLABSystem: '<Root>/Mass'
    FF_Controller_DW.obj_n.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_n.isInitialized = 1;
    prmName_1[0] = 'M';
    prmName_1[1] = 'a';
    prmName_1[2] = 's';
    prmName_1[3] = 's';
    prmName_1[4] = '\x00';
    ParamGet_FF_Controller_70.initParam(&prmName_1[0]);
    ParamGet_FF_Controller_70.setInitialValue(0LL);
    FF_Controller_DW.obj_n.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/Get Parameter'
    FF_Controller_DW.obj_m.matlabCodegenIsDeleted = false;
    FF_Controller_DW.obj_m.isInitialized = 1;
    for (int32_T i = 0; i < 16; i++) {
      FF_Controller_B.prmName[i] = tmp_1[i];
    }

    FF_Controller_B.prmName[16] = '\x00';
    ParamGet_FF_Controller_80.initParam(&FF_Controller_B.prmName[0]);
    ParamGet_FF_Controller_80.setInitialValue(0LL);
    FF_Controller_DW.obj_m.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Parameter'
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

  // Terminate for MATLABSystem: '<Root>/Get Parameter'
  if (!FF_Controller_DW.obj_m.matlabCodegenIsDeleted) {
    FF_Controller_DW.obj_m.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Parameter'

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
