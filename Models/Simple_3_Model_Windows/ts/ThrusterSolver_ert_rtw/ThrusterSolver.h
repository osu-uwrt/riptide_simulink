//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.h
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.50
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Thu Oct 19 00:18:05 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_ThrusterSolver_h_
#define RTW_HEADER_ThrusterSolver_h_
#include "rtwtypes.h"
#include "slros2_initialize.h"
#include "ThrusterSolver_types.h"

extern "C"
{

#include "rt_nonfinite.h"

}

#include "zero_crossing_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Block signals (default storage)
struct B_ThrusterSolver_T {
  SL_Bus_std_msgs_Int32MultiArray In1; // '<S14>/In1'
  SL_Bus_std_msgs_Int32MultiArray BusAssignment2;// '<S5>/Bus Assignment2'
  real_T weightsMatrix[64];
  real_T A_tmp[64];
  real_T A[64];
  real_T value[48];
  real_T Divide1[48];                  // '<Root>/Divide1'
  real_T b[36];
  real_T A_m[36];
  real_T DataTypeConversion[8];        // '<Root>/Data Type Conversion'
  real_T RPMs[8];                      // '<Root>/Force to RPM Transform'
  int64_T value_c[8];
  int64_T varargin_1[8];
  SL_Bus_geometry_msgs_Twist In1_n;    // '<S9>/In1'
  SL_Bus_geometry_msgs_Twist In1_g;    // '<S8>/In1'
  real_T AddFeedForward[8];            // '<S7>/Add Feed Forward'
  SL_Bus_geometry_msgs_Twist b_varargout_2;
  real_T dv[6];
  real_T dv1[6];
  char_T b_zeroDelimTopic[36];
  char_T prmName[35];
  char_T b_zeroDelimTopic_k[32];
  char_T prmName_c[30];
  char_T b_zeroDelimTopic_b[26];
  char_T prmName_p[16];
  char_T b_zeroDelimTopic_c[16];
  char_T prmName_f[14];
  int8_T p[8];
  int8_T ipiv[8];
  real_T rtb_CurrentTime_g;
  real_T smax;
  real_T s;
  real_T Subtract;                     // '<S3>/Subtract'
  real_T rtb_VectorConcatenate_tmp;
  real_T smax_g;
  real_T s_m;
  int64_T value_n;
  int64_T value_p;
  int64_T value_l;
  uint64_T prod_unsgn;
  uint64_T yint;
  uint64_T b_y1;
  uint64_T b_y0;
  uint64_T n1;
  uint64_T n0;
  int8_T c_p[6];
  int8_T ipiv_j[6];
  uint32_T len;
};

// Block states (default storage) for system '<Root>'
struct DW_ThrusterSolver_T {
  ros_slros2_internal_block_Cur_T obj; // '<S3>/Current Time'
  ros_slros2_internal_block_Get_T obj_d;// '<Root>/System Limit'
  ros_slros2_internal_block_Get_T obj_j;// '<Root>/Scaling Factor'
  ros_slros2_internal_block_Get_T obj_p;// '<Root>/Individual Limit'
  ros_slros2_internal_block_Get_T obj_e;// '<Root>/Get Thruster Wrench Matrix'
  ros_slros2_internal_block_Get_T obj_a;
                                      // '<Root>/Force To RPM Curve Parameters'
  ros_slros2_internal_block_Sub_T obj_k;// '<S6>/SourceBlock'
  ros_slros2_internal_block_Sub_T obj_h;// '<S2>/SourceBlock'
  ros_slros2_internal_block_Sub_T obj_c;// '<S1>/SourceBlock'
  ros_slros2_internal_block_Pub_T obj_l;// '<S13>/SinkBlock'
  ros_slros2_internal_block_Pub_T obj_db;// '<S12>/SinkBlock'
  real_T PreviousReset_PreviousInput;  // '<S3>/Previous Reset'
  real_T PreviousBMatrix_PreviousInput[48];// '<S7>/Previous B Matrix'
  real_T PreviousWeights_PreviousInput[8];// '<S7>/Previous Weights'
};

// Zero-crossing (trigger) state
struct PrevZCX_ThrusterSolver_T {
  ZCSigState ThrusterSolver_Trig_ZCE;  // '<Root>/Thruster Solver'
  ZCSigState PublishToROS_Trig_ZCE;    // '<Root>/Publish To ROS'
};

// Parameters (default storage)
struct P_ThrusterSolver_T_ {
  SL_Bus_std_msgs_Int32MultiArray Constant_Value;// Computed Parameter: Constant_Value
                                                    //  Referenced by: '<S11>/Constant'

  SL_Bus_std_msgs_Int32MultiArray Constant_Value_k;// Computed Parameter: Constant_Value_k
                                                      //  Referenced by: '<S10>/Constant'

  SL_Bus_std_msgs_Int32MultiArray Out1_Y0;// Computed Parameter: Out1_Y0
                                             //  Referenced by: '<S14>/Out1'

  SL_Bus_std_msgs_Int32MultiArray Constant_Value_m;// Computed Parameter: Constant_Value_m
                                                      //  Referenced by: '<S6>/Constant'

  SL_Bus_geometry_msgs_Twist Out1_Y0_l;// Computed Parameter: Out1_Y0_l
                                          //  Referenced by: '<S8>/Out1'

  SL_Bus_geometry_msgs_Twist Out1_Y0_a;// Computed Parameter: Out1_Y0_a
                                          //  Referenced by: '<S9>/Out1'

  SL_Bus_geometry_msgs_Twist Constant_Value_i;// Computed Parameter: Constant_Value_i
                                                 //  Referenced by: '<S2>/Constant'

  SL_Bus_geometry_msgs_Twist Constant_Value_o;// Computed Parameter: Constant_Value_o
                                                 //  Referenced by: '<S1>/Constant'

  real_T Constant2_Value;              // Expression: 8
                                          //  Referenced by: '<S5>/Constant2'

  real_T Constant1_Value;              // Expression: 8
                                          //  Referenced by: '<S5>/Constant1'

  real_T Constant_Value_n;             // Expression: 0
                                          //  Referenced by: '<S7>/Constant'

  real_T ThrusterForces_Y0;            // Computed Parameter: ThrusterForces_Y0
                                          //  Referenced by: '<S7>/Thruster Forces'

  real_T PreviousBMatrix_InitialConditio[48];
  // Expression: [inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf]
     //  Referenced by: '<S7>/Previous B Matrix'

  real_T PreviousWeights_InitialConditio[8];
                               // Expression: [ inf inf inf inf inf inf inf inf]
                                  //  Referenced by: '<S7>/Previous Weights'

  real_T ApplyOveruse_Threshold;       // Expression: 0
                                          //  Referenced by: '<S7>/Apply Overuse'

  real_T Constant1_Value_m;            // Expression: 1
                                          //  Referenced by: '<S7>/Constant1'

  real_T PreviousReset_InitialCondition;// Expression: 0
                                           //  Referenced by: '<S3>/Previous Reset'

  real_T Constant_Value_b;             // Expression: .02
                                          //  Referenced by: '<S3>/Constant'

};

// Real-time Model Data Structure
struct tag_RTM_ThrusterSolver_T {
  const char_T * volatile errorStatus;
};

// Class declaration for model ThrusterSolver
class ThrusterSolver final
{
  // public data and function members
 public:
  // Copy Constructor
  ThrusterSolver(ThrusterSolver const&) = delete;

  // Assignment Operator
  ThrusterSolver& operator= (ThrusterSolver const&) & = delete;

  // Move Constructor
  ThrusterSolver(ThrusterSolver &&) = delete;

  // Move Assignment Operator
  ThrusterSolver& operator= (ThrusterSolver &&) = delete;

  // Real-Time Model get method
  RT_MODEL_ThrusterSolver_T * getRTM();

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  ThrusterSolver();

  // Destructor
  ~ThrusterSolver();

  // private data and function members
 private:
  // Block signals
  B_ThrusterSolver_T ThrusterSolver_B;

  // Block states
  DW_ThrusterSolver_T ThrusterSolver_DW;

  // Tunable parameters
  static P_ThrusterSolver_T ThrusterSolver_P;

  // Triggered events
  PrevZCX_ThrusterSolver_T ThrusterSolver_PrevZCX;

  // private member function(s) for subsystem '<Root>'
  void ThrusterSolver_inv(const real_T x[64], real_T y[64]);
  uint64_T ThrusterSo_eml_mixed_uint64_mul(uint64_T nv, real_T y);
  int64_T ThrusterSolver_times(int64_T x, real_T y);
  void ThrusterSo_SystemCore_setup_fam(ros_slros2_internal_block_Pub_T *obj);
  void ThrusterSol_SystemCore_setup_fa(ros_slros2_internal_block_Pub_T *obj);
  void ThrusterSolv_SystemCore_setup_f(ros_slros2_internal_block_Sub_T *obj);
  void ThrusterSolver_SystemCore_setup(ros_slros2_internal_block_Sub_T *obj);
  void ThrusterS_SystemCore_setup_faml(ros_slros2_internal_block_Sub_T *obj);

  // Real-Time Model
  RT_MODEL_ThrusterSolver_T ThrusterSolver_M;
};

extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Reshape' : Reshape block reduction
//  Block '<S7>/Reshape' : Reshape block reduction


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
//  '<Root>' : 'ThrusterSolver'
//  '<S1>'   : 'ThrusterSolver/Active Input'
//  '<S2>'   : 'ThrusterSolver/FF Input'
//  '<S3>'   : 'ThrusterSolver/Force Frequency'
//  '<S4>'   : 'ThrusterSolver/Force to RPM Transform'
//  '<S5>'   : 'ThrusterSolver/Publish To ROS'
//  '<S6>'   : 'ThrusterSolver/Subscribe'
//  '<S7>'   : 'ThrusterSolver/Thruster Solver'
//  '<S8>'   : 'ThrusterSolver/Active Input/Enabled Subsystem'
//  '<S9>'   : 'ThrusterSolver/FF Input/Enabled Subsystem'
//  '<S10>'  : 'ThrusterSolver/Publish To ROS/Blank Message1'
//  '<S11>'  : 'ThrusterSolver/Publish To ROS/Blank Message2'
//  '<S12>'  : 'ThrusterSolver/Publish To ROS/Publish Thruster Forces1'
//  '<S13>'  : 'ThrusterSolver/Publish To ROS/Publish Thruster Forces2'
//  '<S14>'  : 'ThrusterSolver/Subscribe/Enabled Subsystem'
//  '<S15>'  : 'ThrusterSolver/Thruster Solver/MATLAB Function'

#endif                                 // RTW_HEADER_ThrusterSolver_h_

//
// File trailer for generated code.
//
// [EOF]
//
