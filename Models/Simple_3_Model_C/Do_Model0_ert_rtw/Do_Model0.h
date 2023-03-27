//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Do_Model0.h
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
#ifndef RTW_HEADER_Do_Model0_h_
#define RTW_HEADER_Do_Model0_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "Do_Model0_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

// Class declaration for model Do_Model0
class Do_Model0 final
{
  // public data and function members
 public:
  // Block signals (default storage)
  struct B_Do_Model0_T {
    real_T Clock;                      // '<S4>/Clock'
  };

  // Block states (default storage) for system '<Root>'
  struct DW_Do_Model0_T {
    real_T Memory1_PreviousInput;      // '<S4>/Memory1'
  };

  // Invariant block signals (default storage)
  struct ConstB_Do_Model0_T {
    real_T GravitationalForce1;        // '<S7>/Gravitational Force1'
    real_T Sum[3];                     // '<S28>/Sum'
    real_T Sum_p[3];                   // '<S29>/Sum'
    real_T Sum_d[3];                   // '<S30>/Sum'
    real_T Sum_f[3];                   // '<S31>/Sum'
    real_T Sum_h[3];                   // '<S33>/Sum'
    real_T Sum_pz[3];                  // '<S34>/Sum'
    real_T Sum_j[3];                   // '<S35>/Sum'
    real_T Sum_hq[3];                  // '<S36>/Sum'
  };

  // Constant parameters (default storage)
  struct ConstP_Do_Model0_T {
    // Pooled Parameter (Mixed Expressions)
    //  Referenced by:
    //    '<S28>/HPA Direction'
    //    '<S29>/HPF Direction'
    //    '<S30>/HSA Direction'
    //    '<S31>/HSF Direction'

    real_T pooled6[3];

    // Expression: importedData.robot.thrusters.VPADir
    //  Referenced by: '<S33>/VPA Direction'

    real_T VPADirection_Value[3];

    // Expression: importedData.robot.thrusters.VPFDir
    //  Referenced by: '<S34>/VPF Direction'

    real_T VPFDirection_Value[3];

    // Expression: importedData.robot.thrusters.VSADir
    //  Referenced by: '<S35>/VSA Direction'

    real_T VSADirection_Value[3];
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_Do_Model0_T {
    real_T PreviousLinearPosition[3];  // '<Root>/Previous Linear Position'
    real_T AngularPosition[3];         // '<Root>/Angular Position'
    real_T PreviousLinearVelocity[3];  // '<Root>/Previous Linear Velocity'
    real_T PreviousAngularVelcoity[3]; // '<Root>/Previous Angular Velcoity'
    real32_T x1;                       // '<Root>/HSF Force'
    real32_T x2;                       // '<Root>/HSA Force'
    real32_T x3;                       // '<Root>/HPA Force'
    real32_T x4;                       // '<Root>/HPF Force'
    real32_T x5;                       // '<Root>/VSF Force'
    real32_T x6;                       // '<Root>/VSA Force'
    real32_T x7;                       // '<Root>/VPA Force'
    real32_T x8;                       // '<Root>/VPF Force'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_Do_Model0_T {
    real_T LinearPosition[3];          // '<Root>/Linear Position'
    real_T AngularPosition1[3];        // '<Root>/Angular Position1'
    real_T LinearVelocity[3];          // '<Root>/Linear Velocity'
    real_T AngularVelocity[3];         // '<Root>/Angular Velocity'
    real_T LinearAcceleration[3];      // '<Root>/Linear Acceleration'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_Do_Model0_T {
    const char_T *errorStatus;
    RTWSolverInfo solverInfo;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
      time_T stepSize0;
      uint32_T clockTick1;
      SimTimeStep simTimeStep;
      time_T *t;
      time_T tArray[2];
    } Timing;
  };

  // Copy Constructor
  Do_Model0(Do_Model0 const&) = delete;

  // Assignment Operator
  Do_Model0& operator= (Do_Model0 const&) & = delete;

  // Move Constructor
  Do_Model0(Do_Model0 &&) = delete;

  // Move Assignment Operator
  Do_Model0& operator= (Do_Model0 &&) = delete;

  // Real-Time Model get method
  Do_Model0::RT_MODEL_Do_Model0_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_Do_Model0_T *pExtU_Do_Model0_T)
  {
    Do_Model0_U = *pExtU_Do_Model0_T;
  }

  // Root outports get method
  const ExtY_Do_Model0_T &getExternalOutputs() const
  {
    return Do_Model0_Y;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  Do_Model0();

  // Destructor
  ~Do_Model0();

  // private data and function members
 private:
  // External inputs
  ExtU_Do_Model0_T Do_Model0_U;

  // External outputs
  ExtY_Do_Model0_T Do_Model0_Y;

  // Block signals
  B_Do_Model0_T Do_Model0_B;

  // Block states
  DW_Do_Model0_T Do_Model0_DW;

  // private member function(s) for subsystem '<S8>/Seperate to Components'
  static void Do_Model0_SeperatetoComponents(const real_T
    rtu_robotFrameVelocity[3], real_T *rty_x, real_T *rty_y, real_T *rty_z);

  // private member function(s) for subsystem '<S9>/Convert to Column Vector'
  static void Do_Model0_ConverttoColumnVector(const real_T rtu_row[3], real_T
    rty_column[3]);

  // private member function(s) for subsystem '<S9>/Convert to Row Vector'
  static void Do_Model0_ConverttoRowVector(const real_T rtu_column[3], real_T
    rty_row[3]);

  // private member function(s) for subsystem '<S28>/Euler to Unit'
  static void Do_Model0_EulertoUnit(const real_T rtu_euler[3], real_T
    rty_direction[3]);

  // Real-Time Model
  RT_MODEL_Do_Model0_T Do_Model0_M;
};

extern const Do_Model0::ConstB_Do_Model0_T Do_Model0_ConstB;// constant block i/o 

// Constant parameters (default storage)
extern const Do_Model0::ConstP_Do_Model0_T Do_Model0_ConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S6>/Gain1' : Eliminated nontunable gain of 1


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('Simple_3_Model/Do_Model')    - opens subsystem Simple_3_Model/Do_Model
//  hilite_system('Simple_3_Model/Do_Model/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Simple_3_Model'
//  '<S1>'   : 'Simple_3_Model/Do_Model'
//  '<S2>'   : 'Simple_3_Model/Do_Model/Newton's Second Law'
//  '<S3>'   : 'Simple_3_Model/Do_Model/Wrench Calculation'
//  '<S4>'   : 'Simple_3_Model/Do_Model/Newton's Second Law/Delta Time1'
//  '<S5>'   : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag'
//  '<S6>'   : 'Simple_3_Model/Do_Model/Wrench Calculation/Gradient Bouyancy'
//  '<S7>'   : 'Simple_3_Model/Do_Model/Wrench Calculation/Gravity'
//  '<S8>'   : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag'
//  '<S9>'   : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments'
//  '<S10>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag/Concatenate Drag'
//  '<S11>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag/Concatenate Velocity'
//  '<S12>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag/Create Rotation Matrix'
//  '<S13>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag/Seperate to Components'
//  '<S14>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Angular Drag/Seperate to World Compents'
//  '<S15>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Gradient Bouyancy/Bouyant Moments'
//  '<S16>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Gradient Bouyancy/Create Bouancy Vector'
//  '<S17>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Gradient Bouyancy/Insert Gradient Bouyacy Here'
//  '<S18>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Gravity/Create Vector'
//  '<S19>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag/Concatenate Drag'
//  '<S20>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag/Create Rotation Matrix'
//  '<S21>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag/Row to Column Vector'
//  '<S22>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag/Seperate to Components'
//  '<S23>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Linear Drag/Seperate to World Compents'
//  '<S24>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector'
//  '<S25>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector1'
//  '<S26>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector'
//  '<S27>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector1'
//  '<S28>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster'
//  '<S29>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster'
//  '<S30>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster'
//  '<S31>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster'
//  '<S32>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/To World Frame'
//  '<S33>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster'
//  '<S34>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster'
//  '<S35>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster'
//  '<S36>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster'
//  '<S37>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Cross Product'
//  '<S38>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Euler to Unit'
//  '<S39>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Cross Product'
//  '<S40>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Euler to Unit'
//  '<S41>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Cross Product'
//  '<S42>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Euler to Unit'
//  '<S43>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Cross Product'
//  '<S44>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Euler to Unit'
//  '<S45>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Cross Product'
//  '<S46>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Euler to Unit'
//  '<S47>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Cross Product'
//  '<S48>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Euler to Unit'
//  '<S49>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Cross Product'
//  '<S50>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Euler to Unit'
//  '<S51>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Cross Product'
//  '<S52>'  : 'Simple_3_Model/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Euler to Unit'

#endif                                 // RTW_HEADER_Do_Model0_h_

//
// File trailer for generated code.
//
// [EOF]
//
