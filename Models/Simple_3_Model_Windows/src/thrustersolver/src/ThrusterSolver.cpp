//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.cpp
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.18
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Tue Oct 10 19:23:36 2023
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

#include <cstdio>

extern "C"
{

#include "rt_nonfinite.h"

}

// Function for MATLAB Function: '<S6>/MATLAB Function'
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

void ThrusterSolver::ThrusterSolv_SystemCore_setup_f
  (ros_slros2_internal_block_Sub_T *obj)
{
  static const char_T tmp[25]{ '/', 'c', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e',
    'r', '/', 'F', 'F', '_', 'b', 'o', 'd', 'y', '_', 'f', 'o', 'r', 'c', 'e' };

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
  Sub_ThrusterSolver_114.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic_p[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSolver_SystemCore_setup
  (ros_slros2_internal_block_Sub_T *obj)
{
  static const char_T tmp[29]{ '/', 'c', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e',
    'r', '/', 'a', 'c', 't', 'i', 'v', 'e', '_', 'b', 'o', 'd', 'y', '_', 'f',
    'o', 'r', 'c', 'e' };

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
  for (int32_T i{0}; i < 29; i++) {
    ThrusterSolver_B.b_zeroDelimTopic[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic[29] = '\x00';
  Sub_ThrusterSolver_115.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSol_SystemCore_setup_fa
  (ros_slros2_internal_block_Pub_T *obj)
{
  static const char_T tmp[13]{ '/', 't', 'h', 'r', 'u', 's', 't', 'e', 'r', '_',
    'r', 'p', 'm' };

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
  Pub_ThrusterSolver_113.createPublisher(&b_zeroDelimTopic[0], qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void ThrusterSolver::step()
{
  int32_T jj;
  int32_T k;
  uint32_T len;
  boolean_T b_p;
  boolean_T exitg1;
  boolean_T p;

  // MATLABSystem: '<Root>/RPM Spline Coeffs'
  ParamGet_ThrusterSolver_160.getParameter(80U, &ThrusterSolver_B.value[0], &len);

  // MATLABSystem: '<Root>/Get Thruster Weights'
  ParamGet_ThrusterSolver_146.getParameter(8U, &ThrusterSolver_B.value_b[0],
    &len);

  // MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  ParamGet_ThrusterSolver_126.getParameter(48U, &ThrusterSolver_B.value_m[0],
    &len);

  // MATLAB Function: '<S6>/MATLAB Function' incorporates:
  //   MATLABSystem: '<Root>/Get Thruster Weights'
  //   MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

  p = false;
  b_p = true;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 8)) {
    if (!(ThrusterSolver_B.value_b[k] ==
          ThrusterSolver_DW.PreviousWeights_PreviousInput[k])) {
      b_p = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  if (b_p) {
    p = true;
  }

  if (!p) {
    int32_T e_k;
    int32_T jA;
    int32_T kAcol;
    int32_T value_tmp;
    std::memset(&ThrusterSolver_B.weightsMatrix[0], 0, sizeof(real_T) << 6U);
    for (k = 0; k < 8; k++) {
      ThrusterSolver_B.weightsMatrix[k + (k << 3)] = ThrusterSolver_B.value_b[k];
    }

    ThrusterSolver_inv(ThrusterSolver_B.weightsMatrix, ThrusterSolver_B.A_tmp);
    for (k = 0; k < 6; k++) {
      for (jj = 0; jj < 8; jj++) {
        value_tmp = 6 * jj + k;
        ThrusterSolver_B.value_c[value_tmp] = 0.0;
        for (jA = 0; jA < 8; jA++) {
          ThrusterSolver_B.value_c[value_tmp] += ThrusterSolver_B.value_m[(k <<
            3) + jA] * ThrusterSolver_B.A_tmp[(jj << 3) + jA];
        }
      }

      for (jj = 0; jj < 6; jj++) {
        value_tmp = 6 * jj + k;
        ThrusterSolver_B.A_k[value_tmp] = 0.0;
        for (jA = 0; jA < 8; jA++) {
          ThrusterSolver_B.A_k[value_tmp] += ThrusterSolver_B.value_c[6 * jA + k]
            * ThrusterSolver_B.value_m[(jj << 3) + jA];
        }
      }
    }

    std::memset(&ThrusterSolver_B.b[0], 0, 36U * sizeof(real_T));
    for (k = 0; k < 6; k++) {
      ThrusterSolver_B.ipiv_l[k] = static_cast<int8_T>(k + 1);
    }

    for (k = 0; k < 5; k++) {
      jj = k * 7;
      value_tmp = 6 - k;
      jA = 0;
      ThrusterSolver_B.smax = std::abs(ThrusterSolver_B.A_k[jj]);
      for (e_k = 2; e_k <= value_tmp; e_k++) {
        ThrusterSolver_B.s = std::abs(ThrusterSolver_B.A_k[(jj + e_k) - 1]);
        if (ThrusterSolver_B.s > ThrusterSolver_B.smax) {
          jA = e_k - 1;
          ThrusterSolver_B.smax = ThrusterSolver_B.s;
        }
      }

      if (ThrusterSolver_B.A_k[jj + jA] != 0.0) {
        if (jA != 0) {
          jA += k;
          ThrusterSolver_B.ipiv_l[k] = static_cast<int8_T>(jA + 1);
          for (e_k = 0; e_k < 6; e_k++) {
            kAcol = e_k * 6 + k;
            ThrusterSolver_B.smax = ThrusterSolver_B.A_k[kAcol];
            value_tmp = e_k * 6 + jA;
            ThrusterSolver_B.A_k[kAcol] = ThrusterSolver_B.A_k[value_tmp];
            ThrusterSolver_B.A_k[value_tmp] = ThrusterSolver_B.smax;
          }
        }

        value_tmp = (jj - k) + 6;
        for (jA = jj + 2; jA <= value_tmp; jA++) {
          ThrusterSolver_B.A_k[jA - 1] /= ThrusterSolver_B.A_k[jj];
        }
      }

      value_tmp = 4 - k;
      jA = jj + 8;
      for (e_k = 0; e_k <= value_tmp; e_k++) {
        ThrusterSolver_B.s = ThrusterSolver_B.A_k[(e_k * 6 + jj) + 6];
        if (ThrusterSolver_B.s != 0.0) {
          kAcol = (jA - k) + 4;
          for (int32_T ijA{jA}; ijA <= kAcol; ijA++) {
            ThrusterSolver_B.A_k[ijA - 1] += ThrusterSolver_B.A_k[((jj + ijA) -
              jA) + 1] * -ThrusterSolver_B.s;
          }
        }

        jA += 6;
      }
    }

    for (k = 0; k < 6; k++) {
      ThrusterSolver_B.c_p[k] = static_cast<int8_T>(k + 1);
    }

    for (k = 0; k < 5; k++) {
      int8_T ipiv;
      ipiv = ThrusterSolver_B.ipiv_l[k];
      if (ipiv > k + 1) {
        jj = ThrusterSolver_B.c_p[ipiv - 1];
        ThrusterSolver_B.c_p[ipiv - 1] = ThrusterSolver_B.c_p[k];
        ThrusterSolver_B.c_p[k] = static_cast<int8_T>(jj);
      }
    }

    for (value_tmp = 0; value_tmp < 6; value_tmp++) {
      jj = (ThrusterSolver_B.c_p[value_tmp] - 1) * 6;
      ThrusterSolver_B.b[value_tmp + jj] = 1.0;
      for (jA = value_tmp + 1; jA < 7; jA++) {
        k = (jj + jA) - 1;
        if (ThrusterSolver_B.b[k] != 0.0) {
          for (e_k = jA + 1; e_k < 7; e_k++) {
            kAcol = (jj + e_k) - 1;
            ThrusterSolver_B.b[kAcol] -= ThrusterSolver_B.A_k[((jA - 1) * 6 +
              e_k) - 1] * ThrusterSolver_B.b[k];
          }
        }
      }
    }

    for (value_tmp = 0; value_tmp < 6; value_tmp++) {
      jA = 6 * value_tmp;
      for (e_k = 5; e_k >= 0; e_k--) {
        kAcol = 6 * e_k;
        k = e_k + jA;
        ThrusterSolver_B.s = ThrusterSolver_B.b[k];
        if (ThrusterSolver_B.s != 0.0) {
          ThrusterSolver_B.b[k] = ThrusterSolver_B.s / ThrusterSolver_B.A_k[e_k
            + kAcol];
          for (int32_T ijA{0}; ijA < e_k; ijA++) {
            jj = ijA + jA;
            ThrusterSolver_B.b[jj] -= ThrusterSolver_B.A_k[ijA + kAcol] *
              ThrusterSolver_B.b[k];
          }
        }
      }
    }

    for (k = 0; k < 8; k++) {
      for (jj = 0; jj < 6; jj++) {
        value_tmp = jj << 3;
        e_k = k + value_tmp;
        ThrusterSolver_B.value_c[e_k] = 0.0;
        for (jA = 0; jA < 8; jA++) {
          ThrusterSolver_B.value_c[e_k] += ThrusterSolver_B.A_tmp[(jA << 3) + k]
            * ThrusterSolver_B.value_m[value_tmp + jA];
        }
      }

      for (jj = 0; jj < 6; jj++) {
        value_tmp = (jj << 3) + k;
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[value_tmp] = 0.0;
        for (jA = 0; jA < 6; jA++) {
          ThrusterSolver_DW.PreviousBMatrix_PreviousInput[value_tmp] +=
            ThrusterSolver_B.value_c[(jA << 3) + k] * ThrusterSolver_B.b[6 * jj
            + jA];
        }
      }

      ThrusterSolver_DW.PreviousWeights_PreviousInput[k] =
        ThrusterSolver_B.value_b[k];
    }
  }

  // End of MATLAB Function: '<S6>/MATLAB Function'

  // MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'
  ParamGet_ThrusterSolver_165.getParameter(20U, &ThrusterSolver_B.value_cx[0],
    &len);

  // MATLABSystem: '<S3>/SourceBlock'
  p = Sub_ThrusterSolver_114.getLatestMessage(&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S8>/Enable'

  if (p) {
    // SignalConversion generated from: '<S8>/In1'
    ThrusterSolver_B.In1 = ThrusterSolver_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S3>/SourceBlock'
  // End of Outputs for SubSystem: '<S3>/Enabled Subsystem'

  // Product: '<S6>/Feed Forward ' incorporates:
  //   SignalConversion generated from: '<S6>/Feed Forward '

  ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1.linear.x;
  ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1.linear.y;
  ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1.linear.z;
  ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1.angular.x;
  ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1.angular.y;
  ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1.angular.z;
  for (k = 0; k < 8; k++) {
    ThrusterSolver_B.value_b[k] = 0.0;
    for (jj = 0; jj < 6; jj++) {
      ThrusterSolver_B.value_b[k] +=
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(jj << 3) + k] *
        ThrusterSolver_B.dv[jj];
    }
  }

  // End of Product: '<S6>/Feed Forward '

  // MATLABSystem: '<Root>/System Limit'
  ParamGet_ThrusterSolver_148.getParameter(&ThrusterSolver_B.value_me);

  // Sum: '<S6>/Feed Forward Sys' incorporates:
  //   Product: '<S6>/Feed Forward '

  ThrusterSolver_B.s = -0.0;
  for (k = 0; k < 8; k++) {
    ThrusterSolver_B.s += ThrusterSolver_B.value_b[k];
  }

  // Sum: '<S6>/System Allocation' incorporates:
  //   MATLABSystem: '<Root>/System Limit'
  //   Sum: '<S6>/Feed Forward Sys'

  ThrusterSolver_B.smax = ThrusterSolver_B.value_me - ThrusterSolver_B.s;

  // MATLABSystem: '<S1>/SourceBlock'
  p = Sub_ThrusterSolver_115.getLatestMessage(&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S7>/Enable'

  if (p) {
    // SignalConversion generated from: '<S7>/In1'
    ThrusterSolver_B.In1_g = ThrusterSolver_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S1>/SourceBlock'
  // End of Outputs for SubSystem: '<S1>/Enabled Subsystem'

  // Product: '<S6>/Overall System' incorporates:
  //   SignalConversion generated from: '<S6>/Overall System'

  ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1_g.linear.x;
  ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1_g.linear.y;
  ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1_g.linear.z;
  ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1_g.angular.x;
  ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1_g.angular.y;
  ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1_g.angular.z;

  // Sum: '<S6>/Active Sys'
  ThrusterSolver_B.s = -0.0;
  for (k = 0; k < 8; k++) {
    // Product: '<S6>/Overall System'
    ThrusterSolver_B.OverallSystem[k] = 0.0;
    for (jj = 0; jj < 6; jj++) {
      ThrusterSolver_B.OverallSystem[k] +=
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(jj << 3) + k] *
        ThrusterSolver_B.dv[jj];
    }

    // Sum: '<S6>/Active Sys' incorporates:
    //   Product: '<S6>/Overall System'

    ThrusterSolver_B.s += ThrusterSolver_B.OverallSystem[k];
  }

  // Product: '<S6>/Overuse Multiplier' incorporates:
  //   Sum: '<S6>/Active Sys'
  //   Sum: '<S6>/System Overuse'

  ThrusterSolver_B.smax = (ThrusterSolver_B.s - ThrusterSolver_B.smax) /
    ThrusterSolver_B.smax;

  // MATLABSystem: '<Root>/Individual Limit'
  ParamGet_ThrusterSolver_147.getParameter(&ThrusterSolver_B.value_me);
  for (k = 0; k < 8; k++) {
    // Sum: '<S6>/Individual Allocation' incorporates:
    //   MATLABSystem: '<Root>/Individual Limit'
    //   Product: '<S6>/Feed Forward '
    //   Product: '<S6>/Individual Overuse Mutliplier'
    //   Product: '<S6>/Overall System'
    //   Sum: '<S6>/Individual Overuse'

    ThrusterSolver_B.s = ThrusterSolver_B.value_me - ThrusterSolver_B.value_b[k];
    ThrusterSolver_B.IndividualOveruseMutliplier[k] =
      (ThrusterSolver_B.OverallSystem[k] - ThrusterSolver_B.s) /
      ThrusterSolver_B.s;
  }

  // MinMax: '<S6>/Max Individual Overuse' incorporates:
  //   Product: '<S6>/Individual Overuse Mutliplier'

  ThrusterSolver_B.s = ThrusterSolver_B.IndividualOveruseMutliplier[0];
  for (k = 0; k < 7; k++) {
    ThrusterSolver_B.s = std::fmax(ThrusterSolver_B.s,
      ThrusterSolver_B.IndividualOveruseMutliplier[k + 1]);
  }

  // MinMax: '<S6>/Max Overuse Multiplier' incorporates:
  //   MinMax: '<S6>/Max Individual Overuse'

  ThrusterSolver_B.smax = std::fmax(ThrusterSolver_B.smax, ThrusterSolver_B.s);

  // Switch: '<S6>/Apply Overuse' incorporates:
  //   Constant: '<S6>/Constant'

  if (!(ThrusterSolver_B.smax > ThrusterSolver_P.ApplyOveruse_Threshold)) {
    ThrusterSolver_B.smax = ThrusterSolver_P.Constant_Value_n;
  }

  // Sum: '<S6>/Add' incorporates:
  //   Constant: '<S6>/Constant1'
  //   Switch: '<S6>/Apply Overuse'

  ThrusterSolver_B.smax += ThrusterSolver_P.Constant1_Value;

  // BusAssignment: '<Root>/Bus Assignment' incorporates:
  //   Constant: '<S2>/Constant'

  ThrusterSolver_B.BusAssignment = ThrusterSolver_P.Constant_Value;
  for (k = 0; k < 8; k++) {
    // Sum: '<S6>/Add Feed Forward' incorporates:
    //   Product: '<S6>/Apply Multiplier'
    //   Product: '<S6>/Feed Forward '
    //   Product: '<S6>/Overall System'

    ThrusterSolver_B.value_b[k] += ThrusterSolver_B.OverallSystem[k] /
      ThrusterSolver_B.smax;

    // MATLAB Function: '<Root>/MATLAB Function' incorporates:
    //   MATLABSystem: '<Root>/RPM Spline Coeffs'
    //   MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'

    ThrusterSolver_B.s = 0.0;
    while (ThrusterSolver_B.value_b[k] >= ThrusterSolver_B.value_cx[static_cast<
           int32_T>(ThrusterSolver_B.s + 1.0) - 1]) {
      ThrusterSolver_B.s++;
    }

    ThrusterSolver_B.OverallSystem[k] = ((ThrusterSolver_B.value
      [static_cast<int32_T>(4.0 * ThrusterSolver_B.s + 4.0) - 1] * rt_powd_snf
      (ThrusterSolver_B.value_b[k], 3.0) + ThrusterSolver_B.value
      [static_cast<int32_T>(4.0 * ThrusterSolver_B.s + 3.0) - 1] *
      (ThrusterSolver_B.value_b[k] * ThrusterSolver_B.value_b[k])) +
      ThrusterSolver_B.value[static_cast<int32_T>(4.0 * ThrusterSolver_B.s + 2.0)
      - 1] * ThrusterSolver_B.value_b[k]) + ThrusterSolver_B.value
      [static_cast<int32_T>(4.0 * ThrusterSolver_B.s + 1.0) - 1];

    // End of MATLAB Function: '<Root>/MATLAB Function'

    // DataTypeConversion: '<Root>/Data Type Conversion'
    ThrusterSolver_B.s = std::floor(ThrusterSolver_B.OverallSystem[k]);
    if (std::isnan(ThrusterSolver_B.s) || std::isinf(ThrusterSolver_B.s)) {
      ThrusterSolver_B.s = 0.0;
    } else {
      ThrusterSolver_B.s = std::fmod(ThrusterSolver_B.s, 65536.0);
    }

    // BusAssignment: '<Root>/Bus Assignment' incorporates:
    //   DataTypeConversion: '<Root>/Data Type Conversion'

    ThrusterSolver_B.BusAssignment.data[k] = static_cast<int16_T>
      (ThrusterSolver_B.s < 0.0 ? static_cast<int32_T>(static_cast<int16_T>(-
         static_cast<int16_T>(static_cast<uint16_T>(-ThrusterSolver_B.s)))) :
       static_cast<int32_T>(static_cast<int16_T>(static_cast<uint16_T>
         (ThrusterSolver_B.s))));
  }

  // MATLABSystem: '<S5>/SinkBlock'
  Pub_ThrusterSolver_113.publish(&ThrusterSolver_B.BusAssignment);
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
    static const char_T tmp_2[24]{ 't', 'a', 'l', 'o', 's', '_', 'r', 'p', 'm',
      '_', 's', 'p', 'l', 'i', 'n', 'e', '_', 'c', 'u', 't', 'o', 'f', 'f', 's'
    };

    static const char_T tmp[23]{ 't', 'a', 'l', 'o', 's', '_', 'r', 'p', 'm',
      '_', 's', 'p', 'l', 'i', 'n', 'e', '_', 'c', 'o', 'e', 'f', 'f', 's' };

    static const char_T tmp_0[22]{ 't', 'a', 'l', 'o', 's', '_', 't', 'h', 'r',
      'u', 's', 't', 'e', 'r', '_', 'w', 'e', 'i', 'g', 'h', 't', 's' };

    static const char_T tmp_1[15]{ 't', 'a', 'l', 'o', 's', '_', 'w', 'r', 'e',
      'n', 'c', 'h', 'm', 'a', 't' };

    static const char_T tmp_4[15]{ 't', 'a', 'l', 'o', 's', '_', 'i', 'n', 'd',
      'i', 'v', '_', 'l', 'i', 'm' };

    static const char_T tmp_3[13]{ 't', 'a', 'l', 'o', 's', '_', 's', 'y', 's',
      '_', 'l', 'i', 'm' };

    real_T varargin_1;

    // InitializeConditions for Memory: '<S6>/Previous B Matrix'
    std::memcpy(&ThrusterSolver_DW.PreviousBMatrix_PreviousInput[0],
                &ThrusterSolver_P.PreviousBMatrix_InitialConditio[0], 48U *
                sizeof(real_T));

    // InitializeConditions for Memory: '<S6>/Previous Weights'
    std::memcpy(&ThrusterSolver_DW.PreviousWeights_PreviousInput[0],
                &ThrusterSolver_P.PreviousWeights_InitialConditio[0], sizeof
                (real_T) << 3U);

    // SystemInitialize for Enabled SubSystem: '<S3>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S8>/In1' incorporates:
    //   Outport: '<S8>/Out1'

    ThrusterSolver_B.In1 = ThrusterSolver_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S3>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S7>/In1' incorporates:
    //   Outport: '<S7>/Out1'

    ThrusterSolver_B.In1_g = ThrusterSolver_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem'

    // Start for MATLABSystem: '<Root>/RPM Spline Coeffs'
    ThrusterSolver_DW.obj_f.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_f.isInitialized = 1;
    for (int32_T i{0}; i < 23; i++) {
      ThrusterSolver_B.prmName_c[i] = tmp[i];
    }

    ThrusterSolver_B.prmName_c[23] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_160.initParam(&ThrusterSolver_B.prmName_c[0]);
    ParamGet_ThrusterSolver_160.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_f.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RPM Spline Coeffs'

    // Start for MATLABSystem: '<Root>/Get Thruster Weights'
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_d.isInitialized = 1;
    for (int32_T i{0}; i < 22; i++) {
      ThrusterSolver_B.prmName_f[i] = tmp_0[i];
    }

    ThrusterSolver_B.prmName_f[22] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_146.initParam(&ThrusterSolver_B.prmName_f[0]);
    ParamGet_ThrusterSolver_146.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Thruster Weights'

    // Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
    ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_et.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_g[i] = tmp_1[i];
    }

    ThrusterSolver_B.prmName_g[15] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_126.initParam(&ThrusterSolver_B.prmName_g[0]);
    ParamGet_ThrusterSolver_126.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_et.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

    // Start for MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_e.isInitialized = 1;
    for (int32_T i{0}; i < 24; i++) {
      ThrusterSolver_B.prmName[i] = tmp_2[i];
    }

    ThrusterSolver_B.prmName[24] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_165.initParam(&ThrusterSolver_B.prmName[0]);
    ParamGet_ThrusterSolver_165.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_e.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'

    // Start for MATLABSystem: '<S3>/SourceBlock'
    ThrusterSolver_DW.obj_h.isInitialized = 0;
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = false;
    ThrusterSolv_SystemCore_setup_f(&ThrusterSolver_DW.obj_h);

    // Start for MATLABSystem: '<Root>/System Limit'
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj.isInitialized = 1;
    for (int32_T i{0}; i < 13; i++) {
      ThrusterSolver_B.prmName_g1[i] = tmp_3[i];
    }

    ThrusterSolver_B.prmName_g1[13] = '\x00';
    ParamGet_ThrusterSolver_148.initParam(&ThrusterSolver_B.prmName_g1[0]);
    ParamGet_ThrusterSolver_148.setInitialValue(0.0);
    ThrusterSolver_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/System Limit'

    // Start for MATLABSystem: '<S1>/SourceBlock'
    ThrusterSolver_DW.obj_c.isInitialized = 0;
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = false;
    ThrusterSolver_SystemCore_setup(&ThrusterSolver_DW.obj_c);

    // Start for MATLABSystem: '<Root>/Individual Limit'
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_p.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_g[i] = tmp_4[i];
    }

    ThrusterSolver_B.prmName_g[15] = '\x00';
    ParamGet_ThrusterSolver_147.initParam(&ThrusterSolver_B.prmName_g[0]);
    ParamGet_ThrusterSolver_147.setInitialValue(0.0);
    ThrusterSolver_DW.obj_p.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Individual Limit'

    // Start for MATLABSystem: '<S5>/SinkBlock'
    ThrusterSolver_DW.obj_eh.isInitialized = 0;
    ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted = false;
    ThrusterSol_SystemCore_setup_fa(&ThrusterSolver_DW.obj_eh);
  }
}

// Model terminate function
void ThrusterSolver::terminate()
{
  // Terminate for MATLABSystem: '<Root>/RPM Spline Coeffs'
  if (!ThrusterSolver_DW.obj_f.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_f.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/RPM Spline Coeffs'

  // Terminate for MATLABSystem: '<Root>/Get Thruster Weights'
  if (!ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Weights'

  // Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  if (!ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_et.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

  // Terminate for MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'
  if (!ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/RPM Spline Segment Cutoffs'

  // Terminate for MATLABSystem: '<S3>/SourceBlock'
  if (!ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/System Limit'
  if (!ThrusterSolver_DW.obj.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/System Limit'

  // Terminate for MATLABSystem: '<S1>/SourceBlock'
  if (!ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S1>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/Individual Limit'
  if (!ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit'

  // Terminate for MATLABSystem: '<S5>/SinkBlock'
  if (!ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S5>/SinkBlock'
}

// Constructor
ThrusterSolver::ThrusterSolver() :
  ThrusterSolver_B(),
  ThrusterSolver_DW(),
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
