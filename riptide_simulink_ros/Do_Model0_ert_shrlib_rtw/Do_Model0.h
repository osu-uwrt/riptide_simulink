/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Do_Model0.h
 *
 * Code generated for Simulink model 'Do_Model0'.
 *
 * Model version                  : 2.63
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Thu May 25 14:18:00 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Do_Model0_h_
#define RTW_HEADER_Do_Model0_h_
#ifndef Do_Model0_COMMON_INCLUDES_
#define Do_Model0_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Do_Model0_COMMON_INCLUDES_ */

#include "Do_Model0_types.h"

/* Macros for accessing real-time model data structure */
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

/* Block signals (default storage) */
typedef struct {
  real_T Clock;                        /* '<S4>/Clock' */
} B_Do_Model0_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Memory1_PreviousInput;        /* '<S4>/Memory1' */
} DW_Do_Model0_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T GravitationalForce1;    /* '<S7>/Gravitational Force1' */
  const real_T Sum[3];                 /* '<S27>/Sum' */
  const real_T Sum_n[3];               /* '<S28>/Sum' */
  const real_T Sum_m[3];               /* '<S29>/Sum' */
  const real_T Sum_h[3];               /* '<S30>/Sum' */
  const real_T Sum_hj[3];              /* '<S32>/Sum' */
  const real_T Sum_he[3];              /* '<S33>/Sum' */
  const real_T Sum_c[3];               /* '<S34>/Sum' */
  const real_T Sum_f[3];               /* '<S35>/Sum' */
} ConstB_Do_Model0_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S27>/HPA Direction'
   *   '<S28>/HPF Direction'
   *   '<S29>/HSA Direction'
   *   '<S30>/HSF Direction'
   */
  real_T pooled6[3];

  /* Expression: importedData.robot.thrusters.VPADir
   * Referenced by: '<S32>/VPA Direction'
   */
  real_T VPADirection_Value[3];

  /* Expression: importedData.robot.thrusters.VPFDir
   * Referenced by: '<S33>/VPF Direction'
   */
  real_T VPFDirection_Value[3];

  /* Expression: importedData.robot.thrusters.VSADir
   * Referenced by: '<S34>/VSA Direction'
   */
  real_T VSADirection_Value[3];
} ConstP_Do_Model0_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T PreviousLinearPosition[3];    /* '<Root>/Previous Linear Position' */
  real_T PreviousAngularPosition[3];   /* '<Root>/Previous Angular Position' */
  real_T PreviousLinearVelocity[3];    /* '<Root>/Previous Linear Velocity' */
  real_T PreviousAngularVelocity[3];   /* '<Root>/Previous Angular Velocity' */
  real32_T x1;                         /* '<Root>/HSF Force' */
  real32_T x2;                         /* '<Root>/HSA Force' */
  real32_T x3;                         /* '<Root>/HPA Force' */
  real32_T x4;                         /* '<Root>/HPF Force' */
  real32_T x5;                         /* '<Root>/VSF Force' */
  real32_T x6;                         /* '<Root>/VSA Force' */
  real32_T x7;                         /* '<Root>/VPA Force' */
  real32_T x8;                         /* '<Root>/VPF Force' */
  real_T GradientBouyancyForce;        /* '<Root>/Gradient Bouyancy Force' */
  real_T GradientBouyancyForcePosition[3];
                                 /* '<Root>/Gradient Bouyancy Force Position' */
} ExtU_Do_Model0_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T LinearPosition[3];            /* '<Root>/Linear Position' */
  real_T AngularPosition[3];           /* '<Root>/Angular Position' */
  real_T LinearVelocity[3];            /* '<Root>/Linear Velocity' */
  real_T AngularVelocity[3];           /* '<Root>/Angular Velocity' */
  real_T LinearAcceleration[3];        /* '<Root>/Linear Acceleration' */
} ExtY_Do_Model0_T;

/* Real-time Model Data Structure */
struct tag_RTM_Do_Model0_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_Do_Model0_T Do_Model0_B;

/* Block states (default storage) */
extern DW_Do_Model0_T Do_Model0_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Do_Model0_T Do_Model0_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Do_Model0_T Do_Model0_Y;
extern const ConstB_Do_Model0_T Do_Model0_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_Do_Model0_T Do_Model0_ConstP;

/* Model entry point functions */
extern void Do_Model0_initialize(void);
extern void Do_Model0_step(void);
extern void Do_Model0_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Do_Model0_T *const Do_Model0_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S6>/Constant' : Unused code path elimination
 * Block '<S6>/Constant2' : Unused code path elimination
 * Block '<S6>/Gain' : Unused code path elimination
 * Block '<S6>/Gain1' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('Simple_3_Model_C/Do_Model')    - opens subsystem Simple_3_Model_C/Do_Model
 * hilite_system('Simple_3_Model_C/Do_Model/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Simple_3_Model_C'
 * '<S1>'   : 'Simple_3_Model_C/Do_Model'
 * '<S2>'   : 'Simple_3_Model_C/Do_Model/Newton's Second Law'
 * '<S3>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation'
 * '<S4>'   : 'Simple_3_Model_C/Do_Model/Newton's Second Law/Delta Time1'
 * '<S5>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag'
 * '<S6>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy'
 * '<S7>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gravity'
 * '<S8>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag'
 * '<S9>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments'
 * '<S10>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Concatenate Drag'
 * '<S11>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Concatenate Velocity'
 * '<S12>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Create Rotation Matrix'
 * '<S13>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Seperate to Components'
 * '<S14>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Seperate to World Compents'
 * '<S15>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy/Bouyant Moments'
 * '<S16>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy/Create Bouancy Vector'
 * '<S17>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gravity/Create Vector'
 * '<S18>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Concatenate Drag'
 * '<S19>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Create Rotation Matrix'
 * '<S20>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Row to Column Vector'
 * '<S21>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Seperate to Components'
 * '<S22>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Seperate to World Compents'
 * '<S23>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector'
 * '<S24>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector1'
 * '<S25>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector'
 * '<S26>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector1'
 * '<S27>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster'
 * '<S28>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster'
 * '<S29>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster'
 * '<S30>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster'
 * '<S31>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/To World Frame'
 * '<S32>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster'
 * '<S33>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster'
 * '<S34>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster'
 * '<S35>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster'
 * '<S36>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Cross Product'
 * '<S37>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Euler to Unit'
 * '<S38>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Cross Product'
 * '<S39>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Euler to Unit'
 * '<S40>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Cross Product'
 * '<S41>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Euler to Unit'
 * '<S42>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Cross Product'
 * '<S43>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Euler to Unit'
 * '<S44>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Cross Product'
 * '<S45>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Euler to Unit'
 * '<S46>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Cross Product'
 * '<S47>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Euler to Unit'
 * '<S48>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Cross Product'
 * '<S49>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Euler to Unit'
 * '<S50>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Cross Product'
 * '<S51>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Euler to Unit'
 */
#endif                                 /* RTW_HEADER_Do_Model0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
