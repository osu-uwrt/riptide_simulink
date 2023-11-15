//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: FF_Controller.h
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
#ifndef RTW_HEADER_FF_Controller_h_
#define RTW_HEADER_FF_Controller_h_
#include "rtwtypes.h"
#include "slros2_initialize.h"
#include "FF_Controller_types.h"
#include <stddef.h>
#include "zero_crossing_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Block signals (default storage)
struct B_FF_Controller_T {
  SL_Bus_nav_msgs_Odometry In1;        // '<S8>/In1'
  SL_Bus_nav_msgs_Odometry b_varargout_2;
  real_T world2body[9];
  real_T tempR[9];
  int64_T value[6];
  SL_Bus_geometry_msgs_Twist BusAssignment;// '<S3>/Bus Assignment'
  real_T Opposite[6];                  // '<S1>/Opposite'
  real_T ElementProduct[6];            // '<S7>/Element Product'
  real_T rtb_Subtract_m[6];
  char_T b_zeroDelimTopic[32];
  char_T prmName[31];
  real_T tempR_c[3];
  real_T rtb_ElementProduct_k[3];
  int64_T value_c[3];
  int64_T value_b[3];
  int64_T varargin_1[3];
  char_T prmName_p[20];
  real_T rtb_TmpSignalConversionAtSFun_c;
  real_T rtb_TmpSignalConversionAtSFun_f;
  real_T rtb_TmpSignalConversionAtSFun_g;
  real_T rtb_TmpSignalConversionAtSFu_g1;
  real_T tempR_tmp;
  real_T tempR_tmp_m;
  real_T tempR_tmp_n;
  real_T tempR_tmp_p;
  real_T tempR_tmp_l;
  real_T tempR_tmp_j;
  int64_T value_d;
  int64_T value_g;
  int64_T value_l;
  int32_T k;
  int32_T world2body_tmp;
  uint32_T len;
  boolean_T b_varargout_1;
};

// Block states (default storage) for system '<Root>'
struct DW_FF_Controller_T {
  ros_slros2_internal_block_Get_T obj; // '<Root>/Scaling Factor'
  ros_slros2_internal_block_Get_T obj_n;// '<Root>/Mass'
  ros_slros2_internal_block_Get_T obj_ny;// '<Root>/COM'
  ros_slros2_internal_block_Get_T obj_e;// '<Root>/COB'
  ros_slros2_internal_block_Get_T obj_g;// '<Root>/Buoyant Force'
  ros_slros2_internal_block_Sub_T obj_c;// '<S2>/SourceBlock'
  ros_slros2_internal_block_Pub_T obj_n3;// '<S10>/SinkBlock'
};

// Zero-crossing (trigger) state
struct PrevZCX_FF_Controller_T {
  ZCSigState Publish_Trig_ZCE;         // '<Root>/Publish'
  ZCSigState CalculateFF_Trig_ZCE;     // '<Root>/Calculate FF'
};

// Parameters (default storage)
struct P_FF_Controller_T_ {
  SL_Bus_nav_msgs_Odometry Out1_Y0;    // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S8>/Out1'

  SL_Bus_nav_msgs_Odometry Constant_Value;// Computed Parameter: Constant_Value
                                             //  Referenced by: '<S2>/Constant'

  SL_Bus_geometry_msgs_Twist Constant_Value_c;// Computed Parameter: Constant_Value_c
                                                 //  Referenced by: '<S9>/Constant'

  real_T FFTwist_Y0;                   // Computed Parameter: FFTwist_Y0
                                          //  Referenced by: '<S1>/FF Twist'

  real_T Constant_Value_c1;            // Expression: 0
                                          //  Referenced by: '<S5>/Constant'

  real_T Constant3_Value;              // Expression: 9.80665
                                          //  Referenced by: '<S5>/Constant3'

  real_T Opposite_Gain;                // Expression: -1
                                          //  Referenced by: '<S1>/Opposite'

};

// Real-time Model Data Structure
struct tag_RTM_FF_Controller_T {
  const char_T * volatile errorStatus;
};

// Class declaration for model FF_Controller
class FF_Controller
{
  // public data and function members
 public:
  // Real-Time Model get method
  RT_MODEL_FF_Controller_T * getRTM();

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  FF_Controller();

  // Destructor
  ~FF_Controller();

  // private data and function members
 private:
  // Block signals
  B_FF_Controller_T FF_Controller_B;

  // Block states
  DW_FF_Controller_T FF_Controller_DW;

  // Tunable parameters
  static P_FF_Controller_T FF_Controller_P;

  // Triggered events
  PrevZCX_FF_Controller_T FF_Controller_PrevZCX;

  // private member function(s) for subsystem '<Root>'
  void FF_Controlle_SystemCore_setup_l(ros_slros2_internal_block_Pub_T *obj);
  void FF_Controller_SystemCore_setup(ros_slros2_internal_block_Sub_T *obj);

  // Real-Time Model
  RT_MODEL_FF_Controller_T FF_Controller_M;
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
//  '<Root>' : 'FF_Controller'
//  '<S1>'   : 'FF_Controller/Calculate FF'
//  '<S2>'   : 'FF_Controller/Odom'
//  '<S3>'   : 'FF_Controller/Publish'
//  '<S4>'   : 'FF_Controller/Calculate FF/Calculate World Frame Force and Torque'
//  '<S5>'   : 'FF_Controller/Calculate FF/Calculate World Frame Force and Torque/Body Frame Force and Torque'
//  '<S6>'   : 'FF_Controller/Calculate FF/Calculate World Frame Force and Torque/Get Rot Matrix'
//  '<S7>'   : 'FF_Controller/Calculate FF/Calculate World Frame Force and Torque/Body Frame Force and Torque/FF Torque'
//  '<S8>'   : 'FF_Controller/Odom/Enabled Subsystem'
//  '<S9>'   : 'FF_Controller/Publish/FF Twist'
//  '<S10>'  : 'FF_Controller/Publish/Publish'

#endif                                 // RTW_HEADER_FF_Controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
