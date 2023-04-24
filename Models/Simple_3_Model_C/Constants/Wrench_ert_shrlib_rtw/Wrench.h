/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Wrench.h
 *
 * Code generated for Simulink model 'Wrench'.
 *
 * Model version                  : 2.62
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Mon Apr 24 18:44:07 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Wrench_h_
#define RTW_HEADER_Wrench_h_
#ifndef Wrench_COMMON_INCLUDES_
#define Wrench_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Wrench_COMMON_INCLUDES_ */

#include "Wrench_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Invariant block signals (default storage) */
typedef struct {
  const real_T GravitationalForce1;    /* '<S4>/Gravitational Force1' */
  const real_T Sum[3];                 /* '<S24>/Sum' */
  const real_T Sum_l[3];               /* '<S25>/Sum' */
  const real_T Sum_lx[3];              /* '<S26>/Sum' */
  const real_T Sum_k[3];               /* '<S27>/Sum' */
  const real_T Sum_n[3];               /* '<S29>/Sum' */
  const real_T Sum_c[3];               /* '<S30>/Sum' */
  const real_T Sum_g[3];               /* '<S31>/Sum' */
  const real_T Sum_nt[3];              /* '<S32>/Sum' */
} ConstB_Wrench_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S24>/HPA Direction'
   *   '<S25>/HPF Direction'
   *   '<S26>/HSA Direction'
   *   '<S27>/HSF Direction'
   */
  real_T pooled5[3];

  /* Expression: importedData.robot.thrusters.VPADir
   * Referenced by: '<S29>/VPA Direction'
   */
  real_T VPADirection_Value[3];

  /* Expression: importedData.robot.thrusters.VPFDir
   * Referenced by: '<S30>/VPF Direction'
   */
  real_T VPFDirection_Value[3];

  /* Expression: importedData.robot.thrusters.VSADir
   * Referenced by: '<S31>/VSA Direction'
   */
  real_T VSADirection_Value[3];
} ConstP_Wrench_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T LinearVelocities[3];          /* '<Root>/Linear Velocities' */
  real_T AngularPosition[3];           /* '<Root>/Angular Position' */
  real_T AngularVelocities[3];         /* '<Root>/Angular Velocities' */
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
} ExtU_Wrench_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1[3];                      /* '<Root>/Out1' */
  real_T Out2[3];                      /* '<Root>/Out2' */
} ExtY_Wrench_T;

/* Real-time Model Data Structure */
struct tag_RTM_Wrench_T {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_Wrench_T Wrench_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Wrench_T Wrench_Y;
extern const ConstB_Wrench_T Wrench_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_Wrench_T Wrench_ConstP;

/* Model entry point functions */
extern void Wrench_initialize(void);
extern void Wrench_step(void);
extern void Wrench_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Wrench_T *const Wrench_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S3>/Constant' : Unused code path elimination
 * Block '<S3>/Constant2' : Unused code path elimination
 * Block '<S3>/Gain' : Unused code path elimination
 * Block '<S3>/Gain1' : Eliminated nontunable gain of 1
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
 * hilite_system('Simple_3_Model_C/Do_Model/Wrench Calculation')    - opens subsystem Simple_3_Model_C/Do_Model/Wrench Calculation
 * hilite_system('Simple_3_Model_C/Do_Model/Wrench Calculation/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Simple_3_Model_C/Do_Model'
 * '<S1>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation'
 * '<S2>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag'
 * '<S3>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy'
 * '<S4>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gravity'
 * '<S5>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag'
 * '<S6>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments'
 * '<S7>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Concatenate Drag'
 * '<S8>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Concatenate Velocity'
 * '<S9>'   : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Create Rotation Matrix'
 * '<S10>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Seperate to Components'
 * '<S11>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Angular Drag/Seperate to World Compents'
 * '<S12>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy/Bouyant Moments'
 * '<S13>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gradient Bouyancy/Create Bouancy Vector'
 * '<S14>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Gravity/Create Vector'
 * '<S15>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Concatenate Drag'
 * '<S16>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Create Rotation Matrix'
 * '<S17>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Row to Column Vector'
 * '<S18>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Seperate to Components'
 * '<S19>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Linear Drag/Seperate to World Compents'
 * '<S20>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector'
 * '<S21>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Column Vector1'
 * '<S22>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector'
 * '<S23>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/Convert to Row Vector1'
 * '<S24>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster'
 * '<S25>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster'
 * '<S26>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster'
 * '<S27>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster'
 * '<S28>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/To World Frame'
 * '<S29>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster'
 * '<S30>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster'
 * '<S31>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster'
 * '<S32>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster'
 * '<S33>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Cross Product'
 * '<S34>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPA Thruster/Euler to Unit'
 * '<S35>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Cross Product'
 * '<S36>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HPF Thruster/Euler to Unit'
 * '<S37>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Cross Product'
 * '<S38>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSA Thruster/Euler to Unit'
 * '<S39>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Cross Product'
 * '<S40>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/HSF Thruster/Euler to Unit'
 * '<S41>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Cross Product'
 * '<S42>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPA Thruster/Euler to Unit'
 * '<S43>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Cross Product'
 * '<S44>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VPF Thruster/Euler to Unit'
 * '<S45>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Cross Product'
 * '<S46>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSA Thruster/Euler to Unit'
 * '<S47>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Cross Product'
 * '<S48>'  : 'Simple_3_Model_C/Do_Model/Wrench Calculation/Thruster Forces and Moments/VSF Thruster/Euler to Unit'
 */
#endif                                 /* RTW_HEADER_Wrench_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
