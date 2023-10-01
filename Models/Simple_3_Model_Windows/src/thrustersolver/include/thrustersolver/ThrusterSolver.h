//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.h
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.17
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Sun Oct  1 14:02:19 2023
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

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Block signals (default storage)
struct B_ThrusterSolver_T {
  SL_Bus_std_msgs_Int16MultiArray BusAssignment;// '<Root>/Bus Assignment'
  real_T value[80];
  real_T weightsMatrix[64];
  real_T A_tmp[64];
  real_T A[64];
  real_T value_m[48];
  real_T value_c[48];
  real_T b[36];
  real_T A_k[36];
  real_T value_cx[20];
  real_T value_b[8];
  real_T OverallSystem[8];             // '<S6>/Overall System'
  real_T IndividualOveruseMutliplier[8];// '<S6>/Individual Overuse Mutliplier'
  SL_Bus_geometry_msgs_Twist In1;      // '<S8>/In1'
  SL_Bus_geometry_msgs_Twist In1_g;    // '<S7>/In1'
  SL_Bus_geometry_msgs_Twist b_varargout_2;
  real_T dv[6];
  char_T b_zeroDelimTopic[30];
  char_T b_zeroDelimTopic_p[26];
  char_T prmName[25];
  char_T prmName_c[24];
  char_T prmName_f[23];
  char_T prmName_g[16];
  char_T prmName_g1[14];
  int8_T p[8];
  int8_T ipiv[8];
  real_T smax;
  real_T s;
  real_T smax_m;
  real_T s_n;
  int64_T value_p;
  int8_T c_p[6];
  int8_T ipiv_l[6];
  int32_T k;
  int32_T kAcol;
  int32_T jj;
  int32_T e_k;
  int32_T jA;
  int32_T ijA;
  int32_T value_tmp;
  int32_T kAcol_j;
  int32_T jj_d;
  int32_T c_j;
  int32_T n;
  uint32_T len;
};

// Block states (default storage) for system '<Root>'
struct DW_ThrusterSolver_T {
  ros_slros2_internal_block_Get_T obj; // '<Root>/System Limit'
  ros_slros2_internal_block_Get_T obj_e;// '<Root>/RPM Spline Segment Cutoffs'
  ros_slros2_internal_block_Get_T obj_f;// '<Root>/RPM Spline Coeffs'
  ros_slros2_internal_block_Get_T obj_p;// '<Root>/Individual Limit'
  ros_slros2_internal_block_Get_T obj_et;// '<Root>/Get Thruster Wrench Matrix'
  ros_slros2_internal_block_Get_T obj_d;// '<Root>/Get Thruster Weights'
  ros_slros2_internal_block_Sub_T obj_h;// '<S3>/SourceBlock'
  ros_slros2_internal_block_Sub_T obj_c;// '<S1>/SourceBlock'
  ros_slros2_internal_block_Pub_T obj_eh;// '<S5>/SinkBlock'
  real_T PreviousBMatrix_PreviousInput[48];// '<S6>/Previous B Matrix'
  real_T PreviousWeights_PreviousInput[8];// '<S6>/Previous Weights'
};

// Parameters (default storage)
struct P_ThrusterSolver_T_ {
  SL_Bus_std_msgs_Int16MultiArray Constant_Value;// Computed Parameter: Constant_Value
                                                    //  Referenced by: '<S2>/Constant'

  SL_Bus_geometry_msgs_Twist Out1_Y0;  // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S7>/Out1'

  SL_Bus_geometry_msgs_Twist Out1_Y0_a;// Computed Parameter: Out1_Y0_a
                                          //  Referenced by: '<S8>/Out1'

  SL_Bus_geometry_msgs_Twist Constant_Value_i;// Computed Parameter: Constant_Value_i
                                                 //  Referenced by: '<S3>/Constant'

  SL_Bus_geometry_msgs_Twist Constant_Value_o;// Computed Parameter: Constant_Value_o
                                                 //  Referenced by: '<S1>/Constant'

  real_T Constant_Value_n;             // Expression: 0
                                          //  Referenced by: '<S6>/Constant'

  real_T PreviousBMatrix_InitialConditio[48];
  // Expression: [inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf; inf inf inf inf inf inf]
     //  Referenced by: '<S6>/Previous B Matrix'

  real_T PreviousWeights_InitialConditio[8];
                               // Expression: [ inf inf inf inf inf inf inf inf]
                                  //  Referenced by: '<S6>/Previous Weights'

  real_T ApplyOveruse_Threshold;       // Expression: 0
                                          //  Referenced by: '<S6>/Apply Overuse'

  real_T Constant1_Value;              // Expression: 1
                                          //  Referenced by: '<S6>/Constant1'

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

  // private member function(s) for subsystem '<Root>'
  void ThrusterSolver_inv(const real_T x[64], real_T y[64]);
  void ThrusterSolv_SystemCore_setup_f(ros_slros2_internal_block_Sub_T *obj);
  void ThrusterSolver_SystemCore_setup(ros_slros2_internal_block_Sub_T *obj);
  void ThrusterSol_SystemCore_setup_fa(ros_slros2_internal_block_Pub_T *obj);

  // Real-Time Model
  RT_MODEL_ThrusterSolver_T ThrusterSolver_M;
};

extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Reshape' : Reshape block reduction
//  Block '<Root>/Reshape1' : Reshape block reduction
//  Block '<Root>/Reshape2' : Reshape block reduction
//  Block '<Root>/Reshape3' : Reshape block reduction
//  Block '<Root>/Reshape4' : Reshape block reduction
//  Block '<Root>/Reshape5' : Reshape block reduction
//  Block '<S6>/Reshape' : Reshape block reduction


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
//  '<S2>'   : 'ThrusterSolver/Blank Message'
//  '<S3>'   : 'ThrusterSolver/FF Input'
//  '<S4>'   : 'ThrusterSolver/MATLAB Function'
//  '<S5>'   : 'ThrusterSolver/Publish Thruster Forces'
//  '<S6>'   : 'ThrusterSolver/Thruster Solver'
//  '<S7>'   : 'ThrusterSolver/Active Input/Enabled Subsystem'
//  '<S8>'   : 'ThrusterSolver/FF Input/Enabled Subsystem'
//  '<S9>'   : 'ThrusterSolver/Thruster Solver/MATLAB Function'

#endif                                 // RTW_HEADER_ThrusterSolver_h_

//
// File trailer for generated code.
//
// [EOF]
//
