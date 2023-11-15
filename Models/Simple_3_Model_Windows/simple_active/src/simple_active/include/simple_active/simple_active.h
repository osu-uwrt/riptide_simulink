//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: simple_active.h
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
#ifndef RTW_HEADER_simple_active_h_
#define RTW_HEADER_simple_active_h_
#include "rtwtypes.h"
#include "slros2_initialize.h"
#include "simple_active_types.h"

extern "C"
{

#include "rt_nonfinite.h"

}

extern "C"
{

#include "rtGetInf.h"

}

#include <stddef.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Block signals (default storage)
struct B_simple_active_T {
  SL_Bus_nav_msgs_Odometry In1;        // '<S10>/In1'
  SL_Bus_nav_msgs_Odometry b_varargout_2;
  SL_Bus_geometry_msgs_Pose In1_c;     // '<S11>/In1'
  SL_Bus_geometry_msgs_Pose b_varargout_2_m;
  SL_Bus_geometry_msgs_Twist BusAssignment;// '<Root>/Bus Assignment'
  real_T ControlSum[6];                // '<S2>/Control Sum'
  real_T CurrentError[6];              // '<S2>/Current Error'
  real_T normalizedError[6];           // '<S2>/Normalize Angular Error'
  real_T dv[6];
  int64_T value[6];
  int64_T value_c[6];
  int64_T value_k[6];
  int64_T varargin_1[6];
  char_T b_zeroDelimTopic[37];
  char_T b_zeroDelimTopic_c[36];
  real_T y[4];
  char_T b_zeroDelimTopic_b[25];
  real_T euler[3];
  real_T euler_p[3];
  char_T prmName[19];
  real_T x_data;
  real_T aSinInput;
  real_T q;
  real_T Divide;                       // '<Root>/Divide'
  real_T euler_tmp;
  int64_T value_cv;
  int64_T value_f;
};

// Block states (default storage) for system '<Root>'
struct DW_simple_active_T {
  ros_slros2_internal_block_Get_T obj; // '<Root>/Get Parameter4'
  ros_slros2_internal_block_Get_T obj_e;// '<Root>/Get Parameter3'
  ros_slros2_internal_block_Get_T obj_n;// '<Root>/Get Parameter2'
  ros_slros2_internal_block_Get_T obj_j;// '<Root>/Get Parameter1'
  ros_slros2_internal_block_Get_T obj_d;// '<Root>/Get Parameter'
  ros_slros2_internal_block_Pub_T obj_ng;// '<S3>/SinkBlock'
  ros_slros2_internal_block_Sub_T obj_k;// '<S7>/SourceBlock'
  ros_slros2_internal_block_Sub_T obj_ex;// '<S6>/SourceBlock'
  real_T AccumulatedError_PreviousInput[6];// '<S2>/Accumulated Error'
};

// Parameters (default storage)
struct P_simple_active_T_ {
  SL_Bus_nav_msgs_Odometry Out1_Y0;    // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S10>/Out1'

  SL_Bus_nav_msgs_Odometry Constant_Value;// Computed Parameter: Constant_Value
                                             //  Referenced by: '<S6>/Constant'

  SL_Bus_geometry_msgs_Pose Out1_Y0_d; // Computed Parameter: Out1_Y0_d
                                          //  Referenced by: '<S11>/Out1'

  SL_Bus_geometry_msgs_Pose Constant_Value_p;// Computed Parameter: Constant_Value_p
                                                //  Referenced by: '<S7>/Constant'

  SL_Bus_geometry_msgs_Twist Constant_Value_m;// Computed Parameter: Constant_Value_m
                                                 //  Referenced by: '<S1>/Constant'

  real_T PControl_Gain;                // Expression: -1
                                          //  Referenced by: '<S2>/P Control'

  real_T DControl_Gain;                // Expression: -1
                                          //  Referenced by: '<S2>/D Control'

  real_T AccumulatedError_InitialConditi;// Expression: 0
                                            //  Referenced by: '<S2>/Accumulated Error'

  real_T IControl_Gain;                // Expression: -1
                                          //  Referenced by: '<S2>/I Control'

};

// Real-time Model Data Structure
struct tag_RTM_simple_active_T {
  const char_T * volatile errorStatus;
};

// Class declaration for model simple_active
class simple_active
{
  // public data and function members
 public:
  // Real-Time Model get method
  RT_MODEL_simple_active_T * getRTM();

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  simple_active();

  // Destructor
  ~simple_active();

  // private data and function members
 private:
  // Block signals
  B_simple_active_T simple_active_B;

  // Block states
  DW_simple_active_T simple_active_DW;

  // Tunable parameters
  static P_simple_active_T simple_active_P;

  // private member function(s) for subsystem '<Root>'
  void simple_active_binary_expand_op(real_T in1[3], const int8_T in2_data[],
    const real_T in3_data[], const int32_T *in3_size, const real_T in4[4], const
    int32_T in5_size[2]);
  void simple_activ_SystemCore_setup_g(ros_slros2_internal_block_Sub_T *obj);
  void simple_acti_SystemCore_setup_gu(ros_slros2_internal_block_Sub_T *obj);
  void simple_active_SystemCore_setup(ros_slros2_internal_block_Pub_T *obj);

  // Real-Time Model
  RT_MODEL_simple_active_T simple_active_M;
};

extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'simple_active'
//  '<S1>'   : 'simple_active/Blank Message'
//  '<S2>'   : 'simple_active/Control System'
//  '<S3>'   : 'simple_active/Publish Active Control'
//  '<S4>'   : 'simple_active/Quat to eul'
//  '<S5>'   : 'simple_active/Quat to eul1'
//  '<S6>'   : 'simple_active/Subscribe'
//  '<S7>'   : 'simple_active/Subscribe1'
//  '<S8>'   : 'simple_active/Control System/Manage Accumulated Error'
//  '<S9>'   : 'simple_active/Control System/Normalize Angular Error'
//  '<S10>'  : 'simple_active/Subscribe/Enabled Subsystem'
//  '<S11>'  : 'simple_active/Subscribe1/Enabled Subsystem'

#endif                                 // RTW_HEADER_simple_active_h_

//
// File trailer for generated code.
//
// [EOF]
//
