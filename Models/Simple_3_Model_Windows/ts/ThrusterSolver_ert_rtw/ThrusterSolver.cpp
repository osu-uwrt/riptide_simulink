//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.cpp
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.84
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Fri Oct 27 23:36:26 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "ThrusterSolver.h"
#include "ThrusterSolver_types.h"
#include "rtwtypes.h"
#include <cstring>
#include <cmath>
#include "ThrusterSolver_private.h"
#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include <stddef.h>
#include "zero_crossing_types.h"

extern "C"
{

#include "rt_nonfinite.h"

}

// Function for MATLAB Function: '<S7>/MATLAB Function'
void ThrusterSolver::ThrusterSolver_inv(const real_T x[64], real_T y[64])
{
  int32_T c_j;
  int32_T d_k;
  int32_T jA;
  int32_T jj;
  int32_T kAcol;
  int32_T n;
  for (c_j = 0; c_j < 64; c_j++) {
    ThrusterSolver_B.A[c_j] = x[c_j];
    y[c_j] = 0.0;
  }

  for (c_j = 0; c_j < 8; c_j++) {
    ThrusterSolver_B.ipiv[c_j] = static_cast<int8_T>(c_j + 1);
  }

  for (c_j = 0; c_j < 7; c_j++) {
    jj = c_j * 9;
    n = 8 - c_j;
    jA = 0;
    ThrusterSolver_B.smax_n = std::abs(ThrusterSolver_B.A[jj]);
    for (d_k = 2; d_k <= n; d_k++) {
      ThrusterSolver_B.s_p = std::abs(ThrusterSolver_B.A[(jj + d_k) - 1]);
      if (ThrusterSolver_B.s_p > ThrusterSolver_B.smax_n) {
        jA = d_k - 1;
        ThrusterSolver_B.smax_n = ThrusterSolver_B.s_p;
      }
    }

    if (ThrusterSolver_B.A[jj + jA] != 0.0) {
      if (jA != 0) {
        n = c_j + jA;
        ThrusterSolver_B.ipiv[c_j] = static_cast<int8_T>(n + 1);
        for (jA = 0; jA < 8; jA++) {
          kAcol = jA << 3;
          d_k = kAcol + c_j;
          ThrusterSolver_B.smax_n = ThrusterSolver_B.A[d_k];
          kAcol += n;
          ThrusterSolver_B.A[d_k] = ThrusterSolver_B.A[kAcol];
          ThrusterSolver_B.A[kAcol] = ThrusterSolver_B.smax_n;
        }
      }

      n = (jj - c_j) + 8;
      for (jA = jj + 2; jA <= n; jA++) {
        ThrusterSolver_B.A[jA - 1] /= ThrusterSolver_B.A[jj];
      }
    }

    n = 6 - c_j;
    jA = jj + 10;
    for (d_k = 0; d_k <= n; d_k++) {
      ThrusterSolver_B.smax_n = ThrusterSolver_B.A[((d_k << 3) + jj) + 8];
      if (ThrusterSolver_B.smax_n != 0.0) {
        kAcol = (jA - c_j) + 6;
        for (int32_T ijA{jA}; ijA <= kAcol; ijA++) {
          ThrusterSolver_B.A[ijA - 1] += ThrusterSolver_B.A[((jj + ijA) - jA) +
            1] * -ThrusterSolver_B.smax_n;
        }
      }

      jA += 8;
    }
  }

  for (c_j = 0; c_j < 8; c_j++) {
    ThrusterSolver_B.p[c_j] = static_cast<int8_T>(c_j + 1);
  }

  for (c_j = 0; c_j < 7; c_j++) {
    int8_T ipiv;
    ipiv = ThrusterSolver_B.ipiv[c_j];
    if (ipiv > c_j + 1) {
      jj = ThrusterSolver_B.p[ipiv - 1];
      ThrusterSolver_B.p[ipiv - 1] = ThrusterSolver_B.p[c_j];
      ThrusterSolver_B.p[c_j] = static_cast<int8_T>(jj);
    }
  }

  for (n = 0; n < 8; n++) {
    jj = (ThrusterSolver_B.p[n] - 1) << 3;
    y[n + jj] = 1.0;
    for (jA = n + 1; jA < 9; jA++) {
      c_j = (jj + jA) - 1;
      if (y[c_j] != 0.0) {
        for (d_k = jA + 1; d_k < 9; d_k++) {
          kAcol = (jj + d_k) - 1;
          y[kAcol] -= ThrusterSolver_B.A[(((jA - 1) << 3) + d_k) - 1] * y[c_j];
        }
      }
    }
  }

  for (n = 0; n < 8; n++) {
    jA = n << 3;
    for (d_k = 7; d_k >= 0; d_k--) {
      kAcol = d_k << 3;
      c_j = d_k + jA;
      ThrusterSolver_B.smax_n = y[c_j];
      if (ThrusterSolver_B.smax_n != 0.0) {
        y[c_j] = ThrusterSolver_B.smax_n / ThrusterSolver_B.A[d_k + kAcol];
        for (int32_T ijA{0}; ijA < d_k; ijA++) {
          jj = ijA + jA;
          y[jj] -= ThrusterSolver_B.A[ijA + kAcol] * y[c_j];
        }
      }
    }
  }
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = std::abs(u0);
    tmp_0 = std::abs(u1);
    if (std::isinf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = (rtNaN);
    } else {
      y = std::pow(u0, u1);
    }
  }

  return y;
}

void ThrusterSolver::ThrusterSo_SystemCore_setup_fam
  (ros_slros2_internal_block_Pub_T *obj)
{
  static const char_T tmp[15]{ '/', 't', 'a', 'l', 'o', 's', '/', 'r', 'e', 'q',
    'f', 'o', 'r', 'c', 'e' };

  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i{0}; i < 15; i++) {
    ThrusterSolver_B.b_zeroDelimTopic_g[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_g[15] = '\x00';
  Pub_ThrusterSolver_226.createPublisher(&ThrusterSolver_B.b_zeroDelimTopic_g[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSol_SystemCore_setup_fa
  (ros_slros2_internal_block_Pub_T *obj)
{
  static const char_T tmp[13]{ '/', 't', 'a', 'l', 'o', 's', '/', 'r', 'e', 'q',
    'R', 'P', 'M' };

  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  char_T b_zeroDelimTopic[14];
  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i{0}; i < 13; i++) {
    b_zeroDelimTopic[i] = tmp[i];
  }

  b_zeroDelimTopic[13] = '\x00';
  Pub_ThrusterSolver_210.createPublisher(&b_zeroDelimTopic[0], qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSolv_SystemCore_setup_f
  (ros_slros2_internal_block_Sub_T *obj)
{
  static const char_T tmp[31]{ '/', 't', 'a', 'l', 'o', 's', '/', 'c', 'o', 'n',
    't', 'r', 'o', 'l', 'l', 'e', 'r', '/', 'F', 'F', '_', 'b', 'o', 'd', 'y',
    '_', 'f', 'o', 'r', 'c', 'e' };

  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i{0}; i < 31; i++) {
    ThrusterSolver_B.b_zeroDelimTopic_k[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_k[31] = '\x00';
  Sub_ThrusterSolver_114.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic_k[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSolver_SystemCore_setup
  (ros_slros2_internal_block_Sub_T *obj)
{
  static const char_T tmp[35]{ '/', 't', 'a', 'l', 'o', 's', '/', 'c', 'o', 'n',
    't', 'r', 'o', 'l', 'l', 'e', 'r', '/', 'a', 'c', 't', 'i', 'v', 'e', '_',
    'b', 'o', 'd', 'y', '_', 'f', 'o', 'r', 'c', 'e' };

  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i{0}; i < 35; i++) {
    ThrusterSolver_B.b_zeroDelimTopic[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic[35] = '\x00';
  Sub_ThrusterSolver_115.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterS_SystemCore_setup_faml
  (ros_slros2_internal_block_Sub_T *obj)
{
  static const char_T tmp[25]{ 'c', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r',
    '/', 's', 'o', 'l', 'v', 'e', 'r', '_', 'w', 'e', 'i', 'g', 'h', 't', 's' };

  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i{0}; i < 25; i++) {
    ThrusterSolver_B.b_zeroDelimTopic_p[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_p[25] = '\x00';
  Sub_ThrusterSolver_272.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic_p[0],
    qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void ThrusterSolver::step()
{
  int32_T enabled_thrusters;
  int32_T i;
  int32_T n;
  boolean_T rtb_OR;
  boolean_T rtb_Recalculate;
  boolean_T value;

  // MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  ParamGet_ThrusterSolver_126.getParameter(48U, &ThrusterSolver_B.value[0],
    &ThrusterSolver_B.len);

  // MATLABSystem: '<Root>/Scaling Factor'
  ParamGet_ThrusterSolver_197.getParameter(&ThrusterSolver_B.value_l);

  // Product: '<Root>/Divide1' incorporates:
  //   DataTypeConversion: '<Root>/Force To Double3'
  //   MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  //   MATLABSystem: '<Root>/Scaling Factor'
  //   Math: '<Root>/Transpose'

  for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++) {
    for (n = 0; n < 8; n++) {
      ThrusterSolver_B.Divide1[n + (enabled_thrusters << 3)] =
        ThrusterSolver_B.value[6 * n + enabled_thrusters] / static_cast<real_T>
        (ThrusterSolver_B.value_l);
    }
  }

  // End of Product: '<Root>/Divide1'

  // MATLABSystem: '<S2>/SourceBlock'
  rtb_Recalculate = Sub_ThrusterSolver_114.getLatestMessage
    (&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S9>/Enable'

  if (rtb_Recalculate) {
    // SignalConversion generated from: '<S9>/In1'
    ThrusterSolver_B.In1_n = ThrusterSolver_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

  // MATLABSystem: '<S1>/SourceBlock'
  rtb_OR = Sub_ThrusterSolver_115.getLatestMessage
    (&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S8>/Enable'

  if (rtb_OR) {
    // SignalConversion generated from: '<S8>/In1'
    ThrusterSolver_B.In1_g = ThrusterSolver_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S1>/Enabled Subsystem'

  // MATLABSystem: '<Root>/System Limit'
  ParamGet_ThrusterSolver_148.getParameter(&ThrusterSolver_B.value_j);

  // MATLABSystem: '<Root>/Individual Limit'
  ParamGet_ThrusterSolver_147.getParameter(&ThrusterSolver_B.value_d);

  // MATLABSystem: '<Root>/Individual Limit1'
  ParamGet_ThrusterSolver_327.getParameter(&ThrusterSolver_B.value_g);

  // MATLABSystem: '<S3>/Current Time'
  currentROS2TimeDouble(&ThrusterSolver_B.rtb_CurrentTime_m);

  // Sum: '<S3>/Subtract' incorporates:
  //   MATLABSystem: '<S3>/Current Time'
  //   Memory: '<S3>/Previous Reset'

  ThrusterSolver_B.Subtract = ThrusterSolver_B.rtb_CurrentTime_m -
    ThrusterSolver_DW.PreviousReset_PreviousInput;

  // MATLABSystem: '<S6>/SourceBlock'
  value = Sub_ThrusterSolver_272.getLatestMessage
    (&ThrusterSolver_B.BusAssignment2);

  // Logic: '<Root>/Recalculate' incorporates:
  //   MATLABSystem: '<S1>/SourceBlock'
  //   MATLABSystem: '<S2>/SourceBlock'
  //   MATLABSystem: '<S6>/SourceBlock'

  rtb_Recalculate = (rtb_Recalculate || rtb_OR || value);

  // Logic: '<S3>/OR' incorporates:
  //   Constant: '<S3>/Constant'
  //   RelationalOperator: '<S3>/GreaterThan'

  rtb_OR = ((ThrusterSolver_B.Subtract >= ThrusterSolver_P.Constant_Value_b) ||
            rtb_Recalculate);

  // Outputs for Enabled SubSystem: '<S6>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S14>/Enable'

  // MATLABSystem: '<S6>/SourceBlock'
  if (value) {
    // SignalConversion generated from: '<S14>/In1'
    ThrusterSolver_B.In1 = ThrusterSolver_B.BusAssignment2;
  }

  // End of Outputs for SubSystem: '<S6>/Enabled Subsystem'

  // Product: '<Root>/Divide' incorporates:
  //   DataTypeConversion: '<Root>/Force To Double3'
  //   MATLABSystem: '<Root>/Scaling Factor'

  for (i = 0; i < 8; i++) {
    ThrusterSolver_B.Divide[i] = static_cast<real_T>(ThrusterSolver_B.In1.data[i])
      / static_cast<real_T>(ThrusterSolver_B.value_l);
  }

  // End of Product: '<Root>/Divide'

  // Outputs for Triggered SubSystem: '<Root>/Thruster Solver' incorporates:
  //   TriggerPort: '<S7>/Trigger'

  if (rtb_OR && (ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE != POS_ZCSIG)) {
    boolean_T b_p;
    boolean_T exitg1;

    // MATLAB Function: '<S7>/MATLAB Function' incorporates:
    //   Product: '<Root>/Divide'
    //   Product: '<Root>/Divide1'

    value = false;
    b_p = true;
    enabled_thrusters = 0;
    exitg1 = false;
    while ((!exitg1) && (enabled_thrusters < 8)) {
      if (!(ThrusterSolver_B.Divide[enabled_thrusters] ==
            ThrusterSolver_DW.PreviousWeights_PreviousInput[enabled_thrusters]))
      {
        b_p = false;
        exitg1 = true;
      } else {
        enabled_thrusters++;
      }
    }

    if (b_p) {
      value = true;
    }

    if (!value) {
      int32_T e_k;
      int32_T jA;
      int32_T kAcol;
      std::memset(&ThrusterSolver_B.weightsMatrix[0], 0, sizeof(real_T) << 6U);
      for (enabled_thrusters = 0; enabled_thrusters < 8; enabled_thrusters++) {
        ThrusterSolver_B.weightsMatrix[enabled_thrusters + (enabled_thrusters <<
          3)] = ThrusterSolver_B.Divide[enabled_thrusters];
      }

      ThrusterSolver_inv(ThrusterSolver_B.weightsMatrix, ThrusterSolver_B.A_tmp);
      for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++) {
        for (n = 0; n < 8; n++) {
          jA = 6 * n + enabled_thrusters;
          ThrusterSolver_B.value[jA] = 0.0;
          for (i = 0; i < 8; i++) {
            ThrusterSolver_B.value[jA] += ThrusterSolver_B.Divide1
              [(enabled_thrusters << 3) + i] * ThrusterSolver_B.A_tmp[(n << 3) +
              i];
          }
        }

        for (n = 0; n < 6; n++) {
          jA = 6 * n + enabled_thrusters;
          ThrusterSolver_B.A_m[jA] = 0.0;
          for (i = 0; i < 8; i++) {
            ThrusterSolver_B.A_m[jA] += ThrusterSolver_B.value[6 * i +
              enabled_thrusters] * ThrusterSolver_B.Divide1[(n << 3) + i];
          }
        }
      }

      std::memset(&ThrusterSolver_B.b[0], 0, 36U * sizeof(real_T));
      for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++) {
        ThrusterSolver_B.ipiv_l[enabled_thrusters] = static_cast<int8_T>
          (enabled_thrusters + 1);
      }

      for (enabled_thrusters = 0; enabled_thrusters < 5; enabled_thrusters++) {
        i = enabled_thrusters * 7;
        n = 6 - enabled_thrusters;
        jA = 0;
        ThrusterSolver_B.smax = std::abs(ThrusterSolver_B.A_m[i]);
        for (e_k = 2; e_k <= n; e_k++) {
          ThrusterSolver_B.s = std::abs(ThrusterSolver_B.A_m[(i + e_k) - 1]);
          if (ThrusterSolver_B.s > ThrusterSolver_B.smax) {
            jA = e_k - 1;
            ThrusterSolver_B.smax = ThrusterSolver_B.s;
          }
        }

        if (ThrusterSolver_B.A_m[i + jA] != 0.0) {
          if (jA != 0) {
            n = enabled_thrusters + jA;
            ThrusterSolver_B.ipiv_l[enabled_thrusters] = static_cast<int8_T>(n +
              1);
            for (e_k = 0; e_k < 6; e_k++) {
              kAcol = e_k * 6 + enabled_thrusters;
              ThrusterSolver_B.smax = ThrusterSolver_B.A_m[kAcol];
              jA = e_k * 6 + n;
              ThrusterSolver_B.A_m[kAcol] = ThrusterSolver_B.A_m[jA];
              ThrusterSolver_B.A_m[jA] = ThrusterSolver_B.smax;
            }
          }

          n = (i - enabled_thrusters) + 6;
          for (jA = i + 2; jA <= n; jA++) {
            ThrusterSolver_B.A_m[jA - 1] /= ThrusterSolver_B.A_m[i];
          }
        }

        n = 4 - enabled_thrusters;
        jA = i + 8;
        for (e_k = 0; e_k <= n; e_k++) {
          ThrusterSolver_B.smax = ThrusterSolver_B.A_m[(e_k * 6 + i) + 6];
          if (ThrusterSolver_B.smax != 0.0) {
            kAcol = (jA - enabled_thrusters) + 4;
            for (int32_T ijA{jA}; ijA <= kAcol; ijA++) {
              ThrusterSolver_B.A_m[ijA - 1] += ThrusterSolver_B.A_m[((i + ijA) -
                jA) + 1] * -ThrusterSolver_B.smax;
            }
          }

          jA += 6;
        }
      }

      for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++) {
        ThrusterSolver_B.c_p[enabled_thrusters] = static_cast<int8_T>
          (enabled_thrusters + 1);
      }

      for (enabled_thrusters = 0; enabled_thrusters < 5; enabled_thrusters++) {
        int8_T ipiv;
        ipiv = ThrusterSolver_B.ipiv_l[enabled_thrusters];
        if (ipiv > enabled_thrusters + 1) {
          i = ThrusterSolver_B.c_p[ipiv - 1];
          ThrusterSolver_B.c_p[ipiv - 1] =
            ThrusterSolver_B.c_p[enabled_thrusters];
          ThrusterSolver_B.c_p[enabled_thrusters] = static_cast<int8_T>(i);
        }
      }

      for (jA = 0; jA < 6; jA++) {
        n = (ThrusterSolver_B.c_p[jA] - 1) * 6;
        ThrusterSolver_B.b[jA + n] = 1.0;
        for (e_k = jA + 1; e_k < 7; e_k++) {
          enabled_thrusters = (n + e_k) - 1;
          if (ThrusterSolver_B.b[enabled_thrusters] != 0.0) {
            for (i = e_k + 1; i < 7; i++) {
              kAcol = (n + i) - 1;
              ThrusterSolver_B.b[kAcol] -= ThrusterSolver_B.A_m[((e_k - 1) * 6 +
                i) - 1] * ThrusterSolver_B.b[enabled_thrusters];
            }
          }
        }
      }

      for (i = 0; i < 6; i++) {
        jA = 6 * i;
        for (e_k = 5; e_k >= 0; e_k--) {
          kAcol = 6 * e_k;
          enabled_thrusters = e_k + jA;
          ThrusterSolver_B.smax = ThrusterSolver_B.b[enabled_thrusters];
          if (ThrusterSolver_B.smax != 0.0) {
            ThrusterSolver_B.b[enabled_thrusters] = ThrusterSolver_B.smax /
              ThrusterSolver_B.A_m[e_k + kAcol];
            for (int32_T ijA{0}; ijA < e_k; ijA++) {
              n = ijA + jA;
              ThrusterSolver_B.b[n] -= ThrusterSolver_B.A_m[ijA + kAcol] *
                ThrusterSolver_B.b[enabled_thrusters];
            }
          }
        }
      }

      for (i = 0; i < 8; i++) {
        for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++)
        {
          jA = enabled_thrusters << 3;
          e_k = i + jA;
          ThrusterSolver_B.value[e_k] = 0.0;
          for (n = 0; n < 8; n++) {
            ThrusterSolver_B.value[e_k] += ThrusterSolver_B.A_tmp[(n << 3) + i] *
              ThrusterSolver_B.Divide1[jA + n];
          }
        }

        for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++)
        {
          jA = (enabled_thrusters << 3) + i;
          ThrusterSolver_DW.PreviousBMatrix_PreviousInput[jA] = 0.0;
          for (n = 0; n < 6; n++) {
            ThrusterSolver_DW.PreviousBMatrix_PreviousInput[jA] +=
              ThrusterSolver_B.value[(n << 3) + i] * ThrusterSolver_B.b[6 *
              enabled_thrusters + n];
          }
        }

        ThrusterSolver_DW.PreviousWeights_PreviousInput[i] =
          ThrusterSolver_B.Divide[i];
      }
    }

    // End of MATLAB Function: '<S7>/MATLAB Function'

    // MATLAB Function: '<S7>/Check Fully Actuated' incorporates:
    //   DataTypeConversion: '<Root>/Force To Double5'
    //   MATLABSystem: '<Root>/Individual Limit1'
    //   Product: '<Root>/Divide1'

    enabled_thrusters = 0;
    for (i = 0; i < 8; i++) {
      if (ThrusterSolver_B.Divide1[i] < ThrusterSolver_B.value_g) {
        enabled_thrusters++;
      }
    }

    // Switch: '<S7>/Switch' incorporates:
    //   MATLAB Function: '<S7>/Check Fully Actuated'

    if (static_cast<real_T>(enabled_thrusters >= 6) >
        ThrusterSolver_P.Switch_Threshold) {
      // Product: '<S7>/Feed Forward ' incorporates:
      //   SignalConversion generated from: '<S7>/Feed Forward '

      ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1_n.linear.x;
      ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1_n.linear.y;
      ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1_n.linear.z;
      ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1_n.angular.x;
      ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1_n.angular.y;
      ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1_n.angular.z;

      // Product: '<S7>/Overall System' incorporates:
      //   SignalConversion generated from: '<S7>/Overall System'

      ThrusterSolver_B.dv1[0] = ThrusterSolver_B.In1_g.linear.x;
      ThrusterSolver_B.dv1[1] = ThrusterSolver_B.In1_g.linear.y;
      ThrusterSolver_B.dv1[2] = ThrusterSolver_B.In1_g.linear.z;
      ThrusterSolver_B.dv1[3] = ThrusterSolver_B.In1_g.angular.x;
      ThrusterSolver_B.dv1[4] = ThrusterSolver_B.In1_g.angular.y;
      ThrusterSolver_B.dv1[5] = ThrusterSolver_B.In1_g.angular.z;
      for (i = 0; i < 8; i++) {
        // Product: '<S7>/Feed Forward '
        ThrusterSolver_B.Switch[i] = 0.0;

        // Product: '<S7>/Overall System'
        ThrusterSolver_B.OverallSystem[i] = 0.0;
        for (enabled_thrusters = 0; enabled_thrusters < 6; enabled_thrusters++)
        {
          // Product: '<S7>/Feed Forward ' incorporates:
          //   Product: '<S7>/Overall System'

          ThrusterSolver_B.smax =
            ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(enabled_thrusters <<
            3) + i];
          ThrusterSolver_B.Switch[i] += ThrusterSolver_B.smax *
            ThrusterSolver_B.dv[enabled_thrusters];
          ThrusterSolver_B.OverallSystem[i] += ThrusterSolver_B.smax *
            ThrusterSolver_B.dv1[enabled_thrusters];
        }

        // Sum: '<S7>/Individual Allocation' incorporates:
        //   DataTypeConversion: '<Root>/Force To Double2'
        //   MATLABSystem: '<Root>/Individual Limit'
        //   Product: '<S7>/Feed Forward '
        //   Product: '<S7>/Overall System'

        ThrusterSolver_B.smax = static_cast<real_T>(ThrusterSolver_B.value_d) -
          ThrusterSolver_B.Switch[i];
        ThrusterSolver_B.Divide[i] = (ThrusterSolver_B.OverallSystem[i] -
          ThrusterSolver_B.smax) / ThrusterSolver_B.smax;
      }

      // MinMax: '<S7>/Max Individual Overuse' incorporates:
      //   Product: '<S7>/Individual Overuse Mutliplier'
      //   Product: '<S7>/Overall System'
      //   Sum: '<S7>/Individual Allocation'
      //   Sum: '<S7>/Individual Overuse'

      ThrusterSolver_B.s = ThrusterSolver_B.Divide[0];
      for (enabled_thrusters = 0; enabled_thrusters < 7; enabled_thrusters++) {
        ThrusterSolver_B.s = std::fmax(ThrusterSolver_B.s,
          ThrusterSolver_B.Divide[enabled_thrusters + 1]);
      }

      // Sum: '<S7>/Active Sys'
      ThrusterSolver_B.smax = -0.0;

      // Sum: '<S7>/Feed Forward Sys'
      ThrusterSolver_B.ApplyOveruse = -0.0;
      for (enabled_thrusters = 0; enabled_thrusters < 8; enabled_thrusters++) {
        // Sum: '<S7>/Active Sys' incorporates:
        //   Product: '<S7>/Overall System'

        ThrusterSolver_B.smax +=
          ThrusterSolver_B.OverallSystem[enabled_thrusters];

        // Sum: '<S7>/Feed Forward Sys' incorporates:
        //   Product: '<S7>/Feed Forward '

        ThrusterSolver_B.ApplyOveruse +=
          ThrusterSolver_B.Switch[enabled_thrusters];
      }

      // Sum: '<S7>/System Allocation' incorporates:
      //   DataTypeConversion: '<Root>/Force To Double1'
      //   MATLABSystem: '<Root>/System Limit'
      //   Sum: '<S7>/Feed Forward Sys'

      ThrusterSolver_B.ApplyOveruse = static_cast<real_T>
        (ThrusterSolver_B.value_j) - ThrusterSolver_B.ApplyOveruse;

      // MinMax: '<S7>/Max Overuse Multiplier' incorporates:
      //   MinMax: '<S7>/Max Individual Overuse'
      //   Product: '<S7>/Overuse Multiplier'
      //   Sum: '<S7>/Active Sys'
      //   Sum: '<S7>/System Overuse'

      ThrusterSolver_B.ApplyOveruse = std::fmax((ThrusterSolver_B.smax -
        ThrusterSolver_B.ApplyOveruse) / ThrusterSolver_B.ApplyOveruse,
        ThrusterSolver_B.s);

      // Switch: '<S7>/Apply Overuse' incorporates:
      //   Constant: '<S7>/Constant'

      if (!(ThrusterSolver_B.ApplyOveruse >
            ThrusterSolver_P.ApplyOveruse_Threshold)) {
        ThrusterSolver_B.ApplyOveruse = ThrusterSolver_P.Constant_Value_n;
      }

      // Sum: '<S7>/Add' incorporates:
      //   Constant: '<S7>/Constant1'
      //   Switch: '<S7>/Apply Overuse'

      ThrusterSolver_B.smax = ThrusterSolver_B.ApplyOveruse +
        ThrusterSolver_P.Constant1_Value_m;

      // Switch: '<S7>/Switch' incorporates:
      //   Product: '<S7>/Apply Multiplier'
      //   Product: '<S7>/Feed Forward '
      //   Product: '<S7>/Overall System'
      //   Sum: '<S7>/Add Feed Forward'

      for (enabled_thrusters = 0; enabled_thrusters < 8; enabled_thrusters++) {
        ThrusterSolver_B.Switch[enabled_thrusters] +=
          ThrusterSolver_B.OverallSystem[enabled_thrusters] /
          ThrusterSolver_B.smax;
      }
    } else {
      // Switch: '<S7>/Switch' incorporates:
      //   Constant: '<S7>/No Force'
      //   Product: '<S7>/Feed Forward '

      std::memcpy(&ThrusterSolver_B.Switch[0], &ThrusterSolver_P.NoForce_Value[0],
                  sizeof(real_T) << 3U);
    }

    // End of Switch: '<S7>/Switch'
  }

  ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE = rtb_OR;

  // End of Outputs for SubSystem: '<Root>/Thruster Solver'

  // MATLABSystem: '<Root>/Individual Limit2'
  ParamGet_ThrusterSolver_335.getParameter(&value);

  // MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  ParamGet_ThrusterSolver_205.getParameter(8U, &ThrusterSolver_B.value_c[0],
    &ThrusterSolver_B.len);
  for (i = 0; i < 8; i++) {
    // Product: '<Root>/Apply Scaling Factor' incorporates:
    //   DataTypeConversion: '<Root>/Force To Double3'
    //   DataTypeConversion: '<Root>/Force To Double4'
    //   MATLABSystem: '<Root>/Force To RPM Curve Parameters'
    //   MATLABSystem: '<Root>/Scaling Factor'

    ThrusterSolver_B.OverallSystem[i] = static_cast<real_T>
      (ThrusterSolver_B.value_c[i]) / static_cast<real_T>
      (ThrusterSolver_B.value_l);

    // MATLAB Function: '<Root>/Force to RPM Transform'
    ThrusterSolver_B.Divide[i] = 0.0;
  }

  // MATLAB Function: '<Root>/Force to RPM Transform' incorporates:
  //   MATLABSystem: '<Root>/Individual Limit2'
  //   Product: '<Root>/Apply Scaling Factor'

  for (int32_T ijA{0}; ijA < 8; ijA++) {
    ThrusterSolver_B.smax = ThrusterSolver_B.Divide[ijA];
    if (value) {
      if (ThrusterSolver_B.Switch[ijA] > 0.01) {
        ThrusterSolver_B.smax = std::fmin(250.0, rt_powd_snf(std::sin
          (ThrusterSolver_B.Switch[ijA] / 60.0 * 3.1415926535897931),
          0.33333333333333331) * 130.0 + 120.0);
      } else if (ThrusterSolver_B.Switch[ijA] < -0.01) {
        ThrusterSolver_B.smax = std::fmax(-250.0, -120.0 - rt_powd_snf(std::abs
          (std::sin(ThrusterSolver_B.Switch[ijA] / 60.0 * 3.1415926535897931)),
          0.33333333333333331) * 130.0);
      }
    } else if (ThrusterSolver_B.Switch[ijA] > 0.0) {
      ThrusterSolver_B.smax = std::round(((ThrusterSolver_B.OverallSystem[1] *
        ThrusterSolver_B.Switch[ijA] + ThrusterSolver_B.OverallSystem[0]) +
        ThrusterSolver_B.OverallSystem[2] * std::tanh
        (ThrusterSolver_B.Switch[ijA])) + rt_powd_snf
        (ThrusterSolver_B.Switch[ijA], 0.25) * ThrusterSolver_B.OverallSystem[3]);
    } else {
      ThrusterSolver_B.smax = std::round(((ThrusterSolver_B.OverallSystem[5] *
        ThrusterSolver_B.Switch[ijA] + ThrusterSolver_B.OverallSystem[4]) +
        ThrusterSolver_B.OverallSystem[6] * std::tanh
        (ThrusterSolver_B.Switch[ijA])) + rt_powd_snf(std::abs
        (ThrusterSolver_B.Switch[ijA]), 0.25) * ThrusterSolver_B.OverallSystem[7]);
    }

    ThrusterSolver_B.Divide[ijA] = ThrusterSolver_B.smax;
  }

  // Product: '<Root>/Apply Scaling' incorporates:
  //   DataTypeConversion: '<Root>/Force To Double3'
  //   MATLABSystem: '<Root>/Scaling Factor'
  //   Switch: '<S7>/Switch'

  for (i = 0; i < 8; i++) {
    ThrusterSolver_B.OverallSystem[i] = ThrusterSolver_B.Switch[i] *
      static_cast<real_T>(ThrusterSolver_B.value_l);
  }

  // End of Product: '<Root>/Apply Scaling'

  // Outputs for Triggered SubSystem: '<Root>/Publish To ROS' incorporates:
  //   TriggerPort: '<S5>/Trigger'

  if (rtb_OR && (ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE != POS_ZCSIG)) {
    // DataTypeConversion: '<S5>/Data Type Conversion4' incorporates:
    //   Constant: '<S5>/Constant2'

    ThrusterSolver_B.smax = std::floor(ThrusterSolver_P.Constant2_Value);
    if (std::isnan(ThrusterSolver_B.smax) || std::isinf(ThrusterSolver_B.smax))
    {
      ThrusterSolver_B.smax = 0.0;
    } else {
      ThrusterSolver_B.smax = std::fmod(ThrusterSolver_B.smax, 4.294967296E+9);
    }

    ThrusterSolver_B.len = ThrusterSolver_B.smax < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.smax))) :
      static_cast<uint32_T>(ThrusterSolver_B.smax);

    // End of DataTypeConversion: '<S5>/Data Type Conversion4'

    // BusAssignment: '<S5>/Bus Assignment2' incorporates:
    //   Constant: '<S11>/Constant'
    //   DataTypeConversion: '<S5>/Data Type Conversion2'

    ThrusterSolver_B.BusAssignment2 = ThrusterSolver_P.Constant_Value;
    for (i = 0; i < 8; i++) {
      // DataTypeConversion: '<S5>/Data Type Conversion2'
      ThrusterSolver_B.smax = std::floor(ThrusterSolver_B.OverallSystem[i]);
      if (std::isnan(ThrusterSolver_B.smax) || std::isinf(ThrusterSolver_B.smax))
      {
        ThrusterSolver_B.smax = 0.0;
      } else {
        ThrusterSolver_B.smax = std::fmod(ThrusterSolver_B.smax, 4.294967296E+9);
      }

      ThrusterSolver_B.BusAssignment2.data[i] = ThrusterSolver_B.smax < 0.0 ? -
        static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.smax)) :
        static_cast<int32_T>(static_cast<uint32_T>(ThrusterSolver_B.smax));
    }

    ThrusterSolver_B.BusAssignment2.data_SL_Info.CurrentLength =
      ThrusterSolver_B.len;
    ThrusterSolver_B.BusAssignment2.data_SL_Info.ReceivedLength =
      ThrusterSolver_B.len;

    // End of BusAssignment: '<S5>/Bus Assignment2'

    // MATLABSystem: '<S13>/SinkBlock'
    Pub_ThrusterSolver_226.publish(&ThrusterSolver_B.BusAssignment2);

    // SignalConversion generated from: '<S5>/Vector Concatenate'
    std::memcpy(&ThrusterSolver_B.OverallSystem[0], &ThrusterSolver_B.Divide[0],
                sizeof(real_T) << 3U);

    // DataTypeConversion: '<S5>/Data Type Conversion3' incorporates:
    //   Constant: '<S5>/Constant1'

    ThrusterSolver_B.smax = std::floor(ThrusterSolver_P.Constant1_Value);
    if (std::isnan(ThrusterSolver_B.smax) || std::isinf(ThrusterSolver_B.smax))
    {
      ThrusterSolver_B.smax = 0.0;
    } else {
      ThrusterSolver_B.smax = std::fmod(ThrusterSolver_B.smax, 4.294967296E+9);
    }

    ThrusterSolver_B.len = ThrusterSolver_B.smax < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.smax))) :
      static_cast<uint32_T>(ThrusterSolver_B.smax);

    // End of DataTypeConversion: '<S5>/Data Type Conversion3'

    // BusAssignment: '<S5>/Bus Assignment1' incorporates:
    //   Constant: '<S10>/Constant'
    //   DataTypeConversion: '<S5>/Data Type Conversion1'

    ThrusterSolver_B.BusAssignment2 = ThrusterSolver_P.Constant_Value_k;
    for (i = 0; i < 8; i++) {
      // DataTypeConversion: '<S5>/Data Type Conversion1'
      ThrusterSolver_B.smax = std::floor(ThrusterSolver_B.OverallSystem[i]);
      if (std::isnan(ThrusterSolver_B.smax) || std::isinf(ThrusterSolver_B.smax))
      {
        ThrusterSolver_B.smax = 0.0;
      } else {
        ThrusterSolver_B.smax = std::fmod(ThrusterSolver_B.smax, 4.294967296E+9);
      }

      ThrusterSolver_B.BusAssignment2.data[i] = ThrusterSolver_B.smax < 0.0 ? -
        static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.smax)) :
        static_cast<int32_T>(static_cast<uint32_T>(ThrusterSolver_B.smax));
    }

    ThrusterSolver_B.BusAssignment2.data_SL_Info.CurrentLength =
      ThrusterSolver_B.len;
    ThrusterSolver_B.BusAssignment2.data_SL_Info.ReceivedLength =
      ThrusterSolver_B.len;

    // End of BusAssignment: '<S5>/Bus Assignment1'

    // MATLABSystem: '<S12>/SinkBlock'
    Pub_ThrusterSolver_210.publish(&ThrusterSolver_B.BusAssignment2);
  }

  // Outputs for Triggered SubSystem: '<Root>/Thruster Solver' incorporates:
  //   TriggerPort: '<S7>/Trigger'

  ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE = rtb_OR;

  // End of Outputs for SubSystem: '<Root>/Thruster Solver'
  // End of Outputs for SubSystem: '<Root>/Publish To ROS'

  // Switch: '<S3>/Time Elaspsed' incorporates:
  //   Constant: '<S3>/Constant'
  //   Logic: '<S3>/OR1'
  //   RelationalOperator: '<S3>/GreaterThan1'

  if ((ThrusterSolver_B.Subtract >= ThrusterSolver_P.Constant_Value_b) ||
      rtb_Recalculate) {
    // Update for Memory: '<S3>/Previous Reset' incorporates:
    //   MATLABSystem: '<S3>/Current Time'

    ThrusterSolver_DW.PreviousReset_PreviousInput =
      ThrusterSolver_B.rtb_CurrentTime_m;
  }

  // End of Switch: '<S3>/Time Elaspsed'
}

// Model initialize function
void ThrusterSolver::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  // non-finite (run-time) assignments
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[0] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[1] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[2] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[3] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[4] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[5] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[6] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[7] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[8] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[9] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[10] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[11] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[12] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[13] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[14] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[15] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[16] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[17] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[18] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[19] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[20] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[21] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[22] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[23] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[24] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[25] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[26] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[27] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[28] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[29] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[30] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[31] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[32] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[33] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[34] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[35] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[36] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[37] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[38] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[39] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[40] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[41] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[42] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[43] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[44] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[45] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[46] = rtInf;
  ThrusterSolver_P.PreviousBMatrix_InitialConditio[47] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[0] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[1] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[2] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[3] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[4] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[5] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[6] = rtInf;
  ThrusterSolver_P.PreviousWeights_InitialConditio[7] = rtInf;

  {
    static const char_T tmp_0[34]{ 't', 'h', 'r', 'u', 's', 't', 'e', 'r', '_',
      's', 'o', 'l', 'v', 'e', 'r', '_', 's', 'c', 'a', 'l', 'i', 'n', 'g', '_',
      'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's' };

    static const char_T tmp_3[30]{ 't', 'h', 'r', 'u', 's', 't', 'e', 'r', '_',
      's', 'o', 'l', 'v', 'e', 'r', '_', 'd', 'i', 's', 'a', 'b', 'l', 'e', '_',
      'w', 'e', 'i', 'g', 'h', 't' };

    static const char_T tmp_5[29]{ 't', 'a', 'l', 'o', 's', '_', 'f', 'o', 'r',
      'c', 'e', '_', 'c', 'u', 'r', 'v', 'e', '_', 'c', 'o', 'e', 'f', 'f', 'i',
      'c', 'e', 'n', 't', 's' };

    static const char_T tmp_4[19]{ 't', 'h', 'r', 'u', 's', 't', 'e', 'r', '_',
      't', 'a', 'b', 'l', 'e', '_', 'm', 'o', 'd', 'e' };

    static const char_T tmp[15]{ 't', 'a', 'l', 'o', 's', '_', 'w', 'r', 'e',
      'n', 'c', 'h', 'm', 'a', 't' };

    static const char_T tmp_2[15]{ 't', 'a', 'l', 'o', 's', '_', 'i', 'n', 'd',
      'i', 'v', '_', 'l', 'i', 'm' };

    static const char_T tmp_1[13]{ 't', 'a', 'l', 'o', 's', '_', 's', 'y', 's',
      '_', 'l', 'i', 'm' };

    ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE = POS_ZCSIG;
    ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE = POS_ZCSIG;

    // InitializeConditions for Memory: '<S3>/Previous Reset'
    ThrusterSolver_DW.PreviousReset_PreviousInput =
      ThrusterSolver_P.PreviousReset_InitialCondition;

    // SystemInitialize for Enabled SubSystem: '<S2>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S9>/In1' incorporates:
    //   Outport: '<S9>/Out1'

    ThrusterSolver_B.In1_n = ThrusterSolver_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S2>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S8>/In1' incorporates:
    //   Outport: '<S8>/Out1'

    ThrusterSolver_B.In1_g = ThrusterSolver_P.Out1_Y0_l;

    // End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S6>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S14>/In1' incorporates:
    //   Outport: '<S14>/Out1'

    ThrusterSolver_B.In1 = ThrusterSolver_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S6>/Enabled Subsystem'

    // SystemInitialize for Triggered SubSystem: '<Root>/Thruster Solver'
    // InitializeConditions for Memory: '<S7>/Previous B Matrix'
    std::memcpy(&ThrusterSolver_DW.PreviousBMatrix_PreviousInput[0],
                &ThrusterSolver_P.PreviousBMatrix_InitialConditio[0], 48U *
                sizeof(real_T));
    for (int32_T i{0}; i < 8; i++) {
      // InitializeConditions for Memory: '<S7>/Previous Weights'
      ThrusterSolver_DW.PreviousWeights_PreviousInput[i] =
        ThrusterSolver_P.PreviousWeights_InitialConditio[i];

      // SystemInitialize for Switch: '<S7>/Switch' incorporates:
      //   Outport: '<S7>/Thruster Forces'
      //   Product: '<S7>/Feed Forward '

      ThrusterSolver_B.Switch[i] = ThrusterSolver_P.ThrusterForces_Y0;
    }

    // End of SystemInitialize for SubSystem: '<Root>/Thruster Solver'

    // SystemInitialize for Triggered SubSystem: '<Root>/Publish To ROS'
    // Start for MATLABSystem: '<S13>/SinkBlock'
    ThrusterSolver_DW.obj_l.isInitialized = 0;
    ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted = false;
    ThrusterSo_SystemCore_setup_fam(&ThrusterSolver_DW.obj_l);

    // Start for MATLABSystem: '<S12>/SinkBlock'
    ThrusterSolver_DW.obj_db.isInitialized = 0;
    ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted = false;
    ThrusterSol_SystemCore_setup_fa(&ThrusterSolver_DW.obj_db);

    // End of SystemInitialize for SubSystem: '<Root>/Publish To ROS'

    // Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
    ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_et.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_f[i] = tmp[i];
    }

    real_T varargin_1;
    ThrusterSolver_B.prmName_f[15] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_126.initParam(&ThrusterSolver_B.prmName_f[0]);
    ParamGet_ThrusterSolver_126.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_et.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

    // Start for MATLABSystem: '<Root>/Scaling Factor'
    ThrusterSolver_DW.obj_j.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_j.isInitialized = 1;
    for (int32_T i{0}; i < 34; i++) {
      ThrusterSolver_B.prmName[i] = tmp_0[i];
    }

    ThrusterSolver_B.prmName[34] = '\x00';
    ParamGet_ThrusterSolver_197.initParam(&ThrusterSolver_B.prmName[0]);
    ParamGet_ThrusterSolver_197.setInitialValue(0L);
    ThrusterSolver_DW.obj_j.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Scaling Factor'

    // Start for MATLABSystem: '<S2>/SourceBlock'
    ThrusterSolver_DW.obj_h.isInitialized = 0;
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = false;
    ThrusterSolv_SystemCore_setup_f(&ThrusterSolver_DW.obj_h);

    // Start for MATLABSystem: '<S1>/SourceBlock'
    ThrusterSolver_DW.obj_c.isInitialized = 0;
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = false;
    ThrusterSolver_SystemCore_setup(&ThrusterSolver_DW.obj_c);

    // Start for MATLABSystem: '<Root>/System Limit'
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_d.isInitialized = 1;
    for (int32_T i{0}; i < 13; i++) {
      ThrusterSolver_B.prmName_g[i] = tmp_1[i];
    }

    ThrusterSolver_B.prmName_g[13] = '\x00';
    ParamGet_ThrusterSolver_148.initParam(&ThrusterSolver_B.prmName_g[0]);
    ParamGet_ThrusterSolver_148.setInitialValue(10L);
    ThrusterSolver_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/System Limit'

    // Start for MATLABSystem: '<Root>/Individual Limit'
    ThrusterSolver_DW.obj_pl.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_pl.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_f[i] = tmp_2[i];
    }

    ThrusterSolver_B.prmName_f[15] = '\x00';
    ParamGet_ThrusterSolver_147.initParam(&ThrusterSolver_B.prmName_f[0]);
    ParamGet_ThrusterSolver_147.setInitialValue(5L);
    ThrusterSolver_DW.obj_pl.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Individual Limit'

    // Start for MATLABSystem: '<Root>/Individual Limit1'
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_e.isInitialized = 1;
    for (int32_T i{0}; i < 30; i++) {
      ThrusterSolver_B.prmName_c[i] = tmp_3[i];
    }

    ThrusterSolver_B.prmName_c[30] = '\x00';
    ParamGet_ThrusterSolver_327.initParam(&ThrusterSolver_B.prmName_c[0]);
    ParamGet_ThrusterSolver_327.setInitialValue(10000000L);
    ThrusterSolver_DW.obj_e.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Individual Limit1'

    // Start for MATLABSystem: '<S3>/Current Time'
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj.isInitialized = 1;
    ThrusterSolver_DW.obj.isSetupComplete = true;

    // Start for MATLABSystem: '<S6>/SourceBlock'
    ThrusterSolver_DW.obj_k.isInitialized = 0;
    ThrusterSolver_DW.obj_k.matlabCodegenIsDeleted = false;
    ThrusterS_SystemCore_setup_faml(&ThrusterSolver_DW.obj_k);

    // Start for MATLABSystem: '<Root>/Individual Limit2'
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_p.isInitialized = 1;
    for (int32_T i{0}; i < 19; i++) {
      ThrusterSolver_B.prmName_cv[i] = tmp_4[i];
    }

    ThrusterSolver_B.prmName_cv[19] = '\x00';
    ParamGet_ThrusterSolver_335.initParam(&ThrusterSolver_B.prmName_cv[0]);
    ParamGet_ThrusterSolver_335.setInitialValue(false);
    ThrusterSolver_DW.obj_p.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Individual Limit2'

    // Start for MATLABSystem: '<Root>/Force To RPM Curve Parameters'
    ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_a.isInitialized = 1;
    for (int32_T i{0}; i < 29; i++) {
      ThrusterSolver_B.prmName_b[i] = tmp_5[i];
    }

    ThrusterSolver_B.prmName_b[29] = '\x00';
    std::memset(&ThrusterSolver_B.varargin_1[0], 0, sizeof(int64_T) << 3U);
    ParamGet_ThrusterSolver_205.initParam(&ThrusterSolver_B.prmName_b[0]);
    ParamGet_ThrusterSolver_205.setInitialValue(&ThrusterSolver_B.varargin_1[0],
      8U);
    ThrusterSolver_DW.obj_a.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  }
}

// Model terminate function
void ThrusterSolver::terminate()
{
  // Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  if (!ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

  // Terminate for MATLABSystem: '<Root>/Scaling Factor'
  if (!ThrusterSolver_DW.obj_j.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_j.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Scaling Factor'

  // Terminate for MATLABSystem: '<S2>/SourceBlock'
  if (!ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S2>/SourceBlock'

  // Terminate for MATLABSystem: '<S1>/SourceBlock'
  if (!ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S1>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/System Limit'
  if (!ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/System Limit'

  // Terminate for MATLABSystem: '<Root>/Individual Limit'
  if (!ThrusterSolver_DW.obj_pl.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_pl.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit'

  // Terminate for MATLABSystem: '<Root>/Individual Limit1'
  if (!ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit1'

  // Terminate for MATLABSystem: '<S3>/Current Time'
  if (!ThrusterSolver_DW.obj.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/Current Time'

  // Terminate for MATLABSystem: '<S6>/SourceBlock'
  if (!ThrusterSolver_DW.obj_k.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_k.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S6>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/Individual Limit2'
  if (!ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit2'

  // Terminate for MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  if (!ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Force To RPM Curve Parameters'

  // Terminate for Triggered SubSystem: '<Root>/Publish To ROS'
  // Terminate for MATLABSystem: '<S13>/SinkBlock'
  if (!ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S13>/SinkBlock'

  // Terminate for MATLABSystem: '<S12>/SinkBlock'
  if (!ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S12>/SinkBlock'
  // End of Terminate for SubSystem: '<Root>/Publish To ROS'
}

// Constructor
ThrusterSolver::ThrusterSolver() :
  ThrusterSolver_B(),
  ThrusterSolver_DW(),
  ThrusterSolver_PrevZCX(),
  ThrusterSolver_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
ThrusterSolver::~ThrusterSolver()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_ThrusterSolver_T * ThrusterSolver::getRTM()
{
  return (&ThrusterSolver_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
