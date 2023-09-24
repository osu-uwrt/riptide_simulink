//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.cpp
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.11
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Sun Sep 24 14:29:31 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "ThrusterSolver.h"
#include "ThrusterSolver_types.h"
#include "rtwtypes.h"
#include <string.h>
#include <math.h>
#include <emmintrin.h>

extern "C"
{

#include "rt_nonfinite.h"

}

#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include <stddef.h>

// Function for MATLAB Function: '<S5>/MATLAB Function'
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
    ThrusterSolver_B.smax_p = fabs(ThrusterSolver_B.A[jj]);
    for (d_k = 2; d_k <= n; d_k++) {
      ThrusterSolver_B.s_c = fabs(ThrusterSolver_B.A[(jj + d_k) - 1]);
      if (ThrusterSolver_B.s_c > ThrusterSolver_B.smax_p) {
        jA = d_k - 1;
        ThrusterSolver_B.smax_p = ThrusterSolver_B.s_c;
      }
    }

    if (ThrusterSolver_B.A[jj + jA] != 0.0) {
      if (jA != 0) {
        n = c_j + jA;
        ThrusterSolver_B.ipiv[c_j] = static_cast<int8_T>(n + 1);
        for (jA = 0; jA < 8; jA++) {
          kAcol = jA << 3;
          d_k = kAcol + c_j;
          ThrusterSolver_B.smax_p = ThrusterSolver_B.A[d_k];
          kAcol += n;
          ThrusterSolver_B.A[d_k] = ThrusterSolver_B.A[kAcol];
          ThrusterSolver_B.A[kAcol] = ThrusterSolver_B.smax_p;
        }
      }

      n = (jj - c_j) + 8;
      jA = (((((n - jj) - 1) / 2) << 1) + jj) + 2;
      d_k = jA - 2;
      for (kAcol = jj + 2; kAcol <= d_k; kAcol += 2) {
        __m128d tmp;
        tmp = _mm_loadu_pd(&ThrusterSolver_B.A[kAcol - 1]);
        _mm_storeu_pd(&ThrusterSolver_B.A[kAcol - 1], _mm_div_pd(tmp,
          _mm_set1_pd(ThrusterSolver_B.A[jj])));
      }

      for (kAcol = jA; kAcol <= n; kAcol++) {
        ThrusterSolver_B.A[kAcol - 1] /= ThrusterSolver_B.A[jj];
      }
    }

    n = 6 - c_j;
    jA = jj + 10;
    for (d_k = 0; d_k <= n; d_k++) {
      ThrusterSolver_B.smax_p = ThrusterSolver_B.A[((d_k << 3) + jj) + 8];
      if (ThrusterSolver_B.smax_p != 0.0) {
        kAcol = (jA - c_j) + 6;
        for (int32_T ijA = jA; ijA <= kAcol; ijA++) {
          ThrusterSolver_B.A[ijA - 1] += ThrusterSolver_B.A[((jj + ijA) - jA) +
            1] * -ThrusterSolver_B.smax_p;
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
      ThrusterSolver_B.smax_p = y[c_j];
      if (ThrusterSolver_B.smax_p != 0.0) {
        y[c_j] = ThrusterSolver_B.smax_p / ThrusterSolver_B.A[d_k + kAcol];
        for (int32_T ijA = 0; ijA < d_k; ijA++) {
          jj = ijA + jA;
          y[jj] -= ThrusterSolver_B.A[ijA + kAcol] * y[c_j];
        }
      }
    }
  }
}

void ThrusterSolver::ThrusterSolv_SystemCore_setup_f
  (ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[25] = { '/', 'c', 'o', 'n', 't', 'r', 'o', 'l', 'l',
    'e', 'r', '/', 'F', 'F', '_', 'b', 'o', 'd', 'y', '_', 'f', 'o', 'r', 'c',
    'e' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 25; i++) {
    ThrusterSolver_B.b_zeroDelimTopic_c[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_c[25] = '\x00';
  Sub_ThrusterSolver_114.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic_c[0],
    qos_profile);
  obj->isSetupComplete = true;
}

void ThrusterSolver::ThrusterSolver_SystemCore_setup
  (ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[29] = { '/', 'c', 'o', 'n', 't', 'r', 'o', 'l', 'l',
    'e', 'r', '/', 'a', 'c', 't', 'i', 'v', 'e', '_', 'b', 'o', 'd', 'y', '_',
    'f', 'o', 'r', 'c', 'e' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 29; i++) {
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
  rmw_qos_durability_policy_t durability;
  rmw_qos_history_policy_t history;
  rmw_qos_profile_t qos_profile;
  rmw_qos_reliability_policy_t reliability;
  static const char_T tmp[16] = { '/', 't', 'h', 'r', 'u', 's', 't', 'e', 'r',
    '_', 'f', 'o', 'r', 'c', 'e', 's' };

  obj->isInitialized = 1;
  qos_profile = rmw_qos_profile_default;
  history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  SET_QOS_VALUES(qos_profile, history, (size_t)1.0, durability, reliability);
  for (int32_T i = 0; i < 16; i++) {
    ThrusterSolver_B.b_zeroDelimTopic_b[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_b[16] = '\x00';
  Pub_ThrusterSolver_113.createPublisher(&ThrusterSolver_B.b_zeroDelimTopic_b[0],
    qos_profile);
  obj->isSetupComplete = true;
}

// Model step function
void ThrusterSolver::step()
{
  __m128d tmp;
  boolean_T b_p;
  boolean_T exitg1;
  boolean_T p;

  // MATLABSystem: '<Root>/Get Thruster Weights'
  ParamGet_ThrusterSolver_146.getParameter(8U, &ThrusterSolver_B.value_k[0],
    &ThrusterSolver_B.len);

  // MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  ParamGet_ThrusterSolver_126.getParameter(48U, &ThrusterSolver_B.value[0],
    &ThrusterSolver_B.len);

  // MATLAB Function: '<S5>/MATLAB Function' incorporates:
  //   MATLABSystem: '<Root>/Get Thruster Weights'
  //   MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

  p = false;
  b_p = true;
  ThrusterSolver_B.k = 0;
  exitg1 = false;
  while ((!exitg1) && (ThrusterSolver_B.k < 8)) {
    if (!(ThrusterSolver_B.value_k[ThrusterSolver_B.k] ==
          ThrusterSolver_DW.PreviousWeights_PreviousInput[ThrusterSolver_B.k]))
    {
      b_p = false;
      exitg1 = true;
    } else {
      ThrusterSolver_B.k++;
    }
  }

  if (b_p) {
    p = true;
  }

  if (!p) {
    memset(&ThrusterSolver_B.weightsMatrix[0], 0, sizeof(real_T) << 6U);
    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
      ThrusterSolver_B.weightsMatrix[ThrusterSolver_B.k + (ThrusterSolver_B.k <<
        3)] = ThrusterSolver_B.value_k[ThrusterSolver_B.k];
    }

    ThrusterSolver_inv(ThrusterSolver_B.weightsMatrix, ThrusterSolver_B.A_tmp);
    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 6; ThrusterSolver_B.k++) {
      for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 8; ThrusterSolver_B.jj
           ++) {
        ThrusterSolver_B.value_tmp = 6 * ThrusterSolver_B.jj +
          ThrusterSolver_B.k;
        ThrusterSolver_B.value_m[ThrusterSolver_B.value_tmp] = 0.0;
        for (ThrusterSolver_B.jA = 0; ThrusterSolver_B.jA < 8;
             ThrusterSolver_B.jA++) {
          ThrusterSolver_B.value_m[ThrusterSolver_B.value_tmp] +=
            ThrusterSolver_B.value[(ThrusterSolver_B.k << 3) +
            ThrusterSolver_B.jA] * ThrusterSolver_B.A_tmp[(ThrusterSolver_B.jj <<
            3) + ThrusterSolver_B.jA];
        }
      }

      for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 6; ThrusterSolver_B.jj
           ++) {
        ThrusterSolver_B.value_tmp = 6 * ThrusterSolver_B.jj +
          ThrusterSolver_B.k;
        ThrusterSolver_B.A_c[ThrusterSolver_B.value_tmp] = 0.0;
        for (ThrusterSolver_B.jA = 0; ThrusterSolver_B.jA < 8;
             ThrusterSolver_B.jA++) {
          ThrusterSolver_B.A_c[ThrusterSolver_B.value_tmp] +=
            ThrusterSolver_B.value_m[6 * ThrusterSolver_B.jA +
            ThrusterSolver_B.k] * ThrusterSolver_B.value[(ThrusterSolver_B.jj <<
            3) + ThrusterSolver_B.jA];
        }
      }
    }

    memset(&ThrusterSolver_B.b[0], 0, 36U * sizeof(real_T));
    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 6; ThrusterSolver_B.k++) {
      ThrusterSolver_B.ipiv_g[ThrusterSolver_B.k] = static_cast<int8_T>
        (ThrusterSolver_B.k + 1);
    }

    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 5; ThrusterSolver_B.k++) {
      ThrusterSolver_B.jj = ThrusterSolver_B.k * 7;
      ThrusterSolver_B.value_tmp = 6 - ThrusterSolver_B.k;
      ThrusterSolver_B.jA = 0;
      ThrusterSolver_B.smax = fabs(ThrusterSolver_B.A_c[ThrusterSolver_B.jj]);
      for (ThrusterSolver_B.e_k = 2; ThrusterSolver_B.e_k <=
           ThrusterSolver_B.value_tmp; ThrusterSolver_B.e_k++) {
        ThrusterSolver_B.s = fabs(ThrusterSolver_B.A_c[(ThrusterSolver_B.jj +
          ThrusterSolver_B.e_k) - 1]);
        if (ThrusterSolver_B.s > ThrusterSolver_B.smax) {
          ThrusterSolver_B.jA = ThrusterSolver_B.e_k - 1;
          ThrusterSolver_B.smax = ThrusterSolver_B.s;
        }
      }

      if (ThrusterSolver_B.A_c[ThrusterSolver_B.jj + ThrusterSolver_B.jA] != 0.0)
      {
        if (ThrusterSolver_B.jA != 0) {
          ThrusterSolver_B.jA += ThrusterSolver_B.k;
          ThrusterSolver_B.ipiv_g[ThrusterSolver_B.k] = static_cast<int8_T>
            (ThrusterSolver_B.jA + 1);
          for (ThrusterSolver_B.e_k = 0; ThrusterSolver_B.e_k < 6;
               ThrusterSolver_B.e_k++) {
            ThrusterSolver_B.kAcol = ThrusterSolver_B.e_k * 6 +
              ThrusterSolver_B.k;
            ThrusterSolver_B.smax = ThrusterSolver_B.A_c[ThrusterSolver_B.kAcol];
            ThrusterSolver_B.value_tmp = ThrusterSolver_B.e_k * 6 +
              ThrusterSolver_B.jA;
            ThrusterSolver_B.A_c[ThrusterSolver_B.kAcol] =
              ThrusterSolver_B.A_c[ThrusterSolver_B.value_tmp];
            ThrusterSolver_B.A_c[ThrusterSolver_B.value_tmp] =
              ThrusterSolver_B.smax;
          }
        }

        ThrusterSolver_B.value_tmp = (ThrusterSolver_B.jj - ThrusterSolver_B.k)
          + 6;
        ThrusterSolver_B.jA = (((((ThrusterSolver_B.value_tmp -
          ThrusterSolver_B.jj) - 1) / 2) << 1) + ThrusterSolver_B.jj) + 2;
        ThrusterSolver_B.e_k = ThrusterSolver_B.jA - 2;
        for (ThrusterSolver_B.kAcol = ThrusterSolver_B.jj + 2;
             ThrusterSolver_B.kAcol <= ThrusterSolver_B.e_k;
             ThrusterSolver_B.kAcol += 2) {
          tmp = _mm_loadu_pd(&ThrusterSolver_B.A_c[ThrusterSolver_B.kAcol - 1]);
          _mm_storeu_pd(&ThrusterSolver_B.A_c[ThrusterSolver_B.kAcol - 1],
                        _mm_div_pd(tmp, _mm_set1_pd
            (ThrusterSolver_B.A_c[ThrusterSolver_B.jj])));
        }

        for (ThrusterSolver_B.kAcol = ThrusterSolver_B.jA;
             ThrusterSolver_B.kAcol <= ThrusterSolver_B.value_tmp;
             ThrusterSolver_B.kAcol++) {
          ThrusterSolver_B.A_c[ThrusterSolver_B.kAcol - 1] /=
            ThrusterSolver_B.A_c[ThrusterSolver_B.jj];
        }
      }

      ThrusterSolver_B.value_tmp = 4 - ThrusterSolver_B.k;
      ThrusterSolver_B.jA = ThrusterSolver_B.jj + 8;
      for (ThrusterSolver_B.e_k = 0; ThrusterSolver_B.e_k <=
           ThrusterSolver_B.value_tmp; ThrusterSolver_B.e_k++) {
        ThrusterSolver_B.smax = ThrusterSolver_B.A_c[(ThrusterSolver_B.e_k * 6 +
          ThrusterSolver_B.jj) + 6];
        if (ThrusterSolver_B.smax != 0.0) {
          ThrusterSolver_B.kAcol = (ThrusterSolver_B.jA - ThrusterSolver_B.k) +
            4;
          for (ThrusterSolver_B.ijA = ThrusterSolver_B.jA; ThrusterSolver_B.ijA <=
               ThrusterSolver_B.kAcol; ThrusterSolver_B.ijA++) {
            ThrusterSolver_B.A_c[ThrusterSolver_B.ijA - 1] +=
              ThrusterSolver_B.A_c[((ThrusterSolver_B.jj + ThrusterSolver_B.ijA)
              - ThrusterSolver_B.jA) + 1] * -ThrusterSolver_B.smax;
          }
        }

        ThrusterSolver_B.jA += 6;
      }
    }

    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 6; ThrusterSolver_B.k++) {
      ThrusterSolver_B.c_p[ThrusterSolver_B.k] = static_cast<int8_T>
        (ThrusterSolver_B.k + 1);
    }

    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 5; ThrusterSolver_B.k++) {
      int8_T ipiv;
      ipiv = ThrusterSolver_B.ipiv_g[ThrusterSolver_B.k];
      if (ipiv > ThrusterSolver_B.k + 1) {
        ThrusterSolver_B.jj = ThrusterSolver_B.c_p[ipiv - 1];
        ThrusterSolver_B.c_p[ipiv - 1] = ThrusterSolver_B.c_p[ThrusterSolver_B.k];
        ThrusterSolver_B.c_p[ThrusterSolver_B.k] = static_cast<int8_T>
          (ThrusterSolver_B.jj);
      }
    }

    for (ThrusterSolver_B.value_tmp = 0; ThrusterSolver_B.value_tmp < 6;
         ThrusterSolver_B.value_tmp++) {
      ThrusterSolver_B.jj = (ThrusterSolver_B.c_p[ThrusterSolver_B.value_tmp] -
        1) * 6;
      ThrusterSolver_B.b[ThrusterSolver_B.value_tmp + ThrusterSolver_B.jj] = 1.0;
      for (ThrusterSolver_B.jA = ThrusterSolver_B.value_tmp + 1;
           ThrusterSolver_B.jA < 7; ThrusterSolver_B.jA++) {
        ThrusterSolver_B.k = (ThrusterSolver_B.jj + ThrusterSolver_B.jA) - 1;
        if (ThrusterSolver_B.b[ThrusterSolver_B.k] != 0.0) {
          for (ThrusterSolver_B.e_k = ThrusterSolver_B.jA + 1;
               ThrusterSolver_B.e_k < 7; ThrusterSolver_B.e_k++) {
            ThrusterSolver_B.kAcol = (ThrusterSolver_B.jj + ThrusterSolver_B.e_k)
              - 1;
            ThrusterSolver_B.b[ThrusterSolver_B.kAcol] -= ThrusterSolver_B.A_c
              [((ThrusterSolver_B.jA - 1) * 6 + ThrusterSolver_B.e_k) - 1] *
              ThrusterSolver_B.b[ThrusterSolver_B.k];
          }
        }
      }
    }

    for (ThrusterSolver_B.value_tmp = 0; ThrusterSolver_B.value_tmp < 6;
         ThrusterSolver_B.value_tmp++) {
      ThrusterSolver_B.jA = 6 * ThrusterSolver_B.value_tmp;
      for (ThrusterSolver_B.e_k = 5; ThrusterSolver_B.e_k >= 0;
           ThrusterSolver_B.e_k--) {
        ThrusterSolver_B.kAcol = 6 * ThrusterSolver_B.e_k;
        ThrusterSolver_B.k = ThrusterSolver_B.e_k + ThrusterSolver_B.jA;
        ThrusterSolver_B.smax = ThrusterSolver_B.b[ThrusterSolver_B.k];
        if (ThrusterSolver_B.smax != 0.0) {
          ThrusterSolver_B.b[ThrusterSolver_B.k] = ThrusterSolver_B.smax /
            ThrusterSolver_B.A_c[ThrusterSolver_B.e_k + ThrusterSolver_B.kAcol];
          for (ThrusterSolver_B.ijA = 0; ThrusterSolver_B.ijA <
               ThrusterSolver_B.e_k; ThrusterSolver_B.ijA++) {
            ThrusterSolver_B.jj = ThrusterSolver_B.ijA + ThrusterSolver_B.jA;
            ThrusterSolver_B.b[ThrusterSolver_B.jj] -=
              ThrusterSolver_B.A_c[ThrusterSolver_B.ijA + ThrusterSolver_B.kAcol]
              * ThrusterSolver_B.b[ThrusterSolver_B.k];
          }
        }
      }
    }

    for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
      for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 6; ThrusterSolver_B.jj
           ++) {
        ThrusterSolver_B.value_tmp = ThrusterSolver_B.jj << 3;
        ThrusterSolver_B.e_k = ThrusterSolver_B.k + ThrusterSolver_B.value_tmp;
        ThrusterSolver_B.value_m[ThrusterSolver_B.e_k] = 0.0;
        for (ThrusterSolver_B.jA = 0; ThrusterSolver_B.jA < 8;
             ThrusterSolver_B.jA++) {
          ThrusterSolver_B.value_m[ThrusterSolver_B.e_k] +=
            ThrusterSolver_B.A_tmp[(ThrusterSolver_B.jA << 3) +
            ThrusterSolver_B.k] *
            ThrusterSolver_B.value[ThrusterSolver_B.value_tmp +
            ThrusterSolver_B.jA];
        }
      }

      for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 6; ThrusterSolver_B.jj
           ++) {
        ThrusterSolver_B.value_tmp = (ThrusterSolver_B.jj << 3) +
          ThrusterSolver_B.k;
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[ThrusterSolver_B.value_tmp]
          = 0.0;
        for (ThrusterSolver_B.jA = 0; ThrusterSolver_B.jA < 6;
             ThrusterSolver_B.jA++) {
          ThrusterSolver_DW.PreviousBMatrix_PreviousInput[ThrusterSolver_B.value_tmp]
            += ThrusterSolver_B.value_m[(ThrusterSolver_B.jA << 3) +
            ThrusterSolver_B.k] * ThrusterSolver_B.b[6 * ThrusterSolver_B.jj +
            ThrusterSolver_B.jA];
        }
      }

      ThrusterSolver_DW.PreviousWeights_PreviousInput[ThrusterSolver_B.k] =
        ThrusterSolver_B.value_k[ThrusterSolver_B.k];
    }
  }

  // End of MATLAB Function: '<S5>/MATLAB Function'

  // MATLABSystem: '<S3>/SourceBlock'
  p = Sub_ThrusterSolver_114.getLatestMessage(&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S7>/Enable'

  if (p) {
    // SignalConversion generated from: '<S7>/In1'
    ThrusterSolver_B.In1 = ThrusterSolver_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S3>/SourceBlock'
  // End of Outputs for SubSystem: '<S3>/Enabled Subsystem'

  // Product: '<S5>/Feed Forward ' incorporates:
  //   Reshape: '<Root>/Reshape2'

  ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1.linear.x;
  ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1.linear.y;
  ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1.linear.z;
  ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1.angular.x;
  ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1.angular.y;
  ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1.angular.z;
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
    ThrusterSolver_B.value_k[ThrusterSolver_B.k] = 0.0;
    for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 6; ThrusterSolver_B.jj++)
    {
      ThrusterSolver_B.value_k[ThrusterSolver_B.k] +=
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(ThrusterSolver_B.jj <<
        3) + ThrusterSolver_B.k] * ThrusterSolver_B.dv[ThrusterSolver_B.jj];
    }
  }

  // End of Product: '<S5>/Feed Forward '

  // MATLABSystem: '<Root>/Individual Limit'
  ParamGet_ThrusterSolver_147.getParameter(&ThrusterSolver_B.value_f);
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k <= 6; ThrusterSolver_B.k += 2)
  {
    // Sum: '<S5>/Individual Allocation' incorporates:
    //   MATLABSystem: '<Root>/Individual Limit'
    //   Product: '<S5>/Individual Overuse Mutliplier'
    //   Sum: '<S5>/Add Feed Forward'

    tmp = _mm_loadu_pd(&ThrusterSolver_B.value_k[ThrusterSolver_B.k]);
    _mm_storeu_pd
      (&ThrusterSolver_B.IndividualOveruseMutliplier[ThrusterSolver_B.k],
       _mm_sub_pd(_mm_set1_pd(static_cast<real_T>(ThrusterSolver_B.value_f)),
                  tmp));
  }

  // MATLABSystem: '<S1>/SourceBlock'
  p = Sub_ThrusterSolver_115.getLatestMessage(&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S6>/Enable'

  if (p) {
    // SignalConversion generated from: '<S6>/In1'
    ThrusterSolver_B.In1_g = ThrusterSolver_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S1>/SourceBlock'
  // End of Outputs for SubSystem: '<S1>/Enabled Subsystem'

  // Product: '<S5>/Overall System' incorporates:
  //   Product: '<S5>/Individual Overuse Mutliplier'
  //   Reshape: '<Root>/Reshape3'
  //   Sum: '<S5>/Individual Overuse'

  ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1_g.linear.x;
  ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1_g.linear.y;
  ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1_g.linear.z;
  ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1_g.angular.x;
  ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1_g.angular.y;
  ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1_g.angular.z;
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
    ThrusterSolver_B.smax =
      ThrusterSolver_B.IndividualOveruseMutliplier[ThrusterSolver_B.k];
    ThrusterSolver_B.OverallSystem[ThrusterSolver_B.k] = 0.0;
    for (ThrusterSolver_B.jj = 0; ThrusterSolver_B.jj < 6; ThrusterSolver_B.jj++)
    {
      ThrusterSolver_B.OverallSystem[ThrusterSolver_B.k] +=
        ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(ThrusterSolver_B.jj <<
        3) + ThrusterSolver_B.k] * ThrusterSolver_B.dv[ThrusterSolver_B.jj];
    }

    ThrusterSolver_B.IndividualOveruseMutliplier[ThrusterSolver_B.k] =
      (ThrusterSolver_B.OverallSystem[ThrusterSolver_B.k] -
       ThrusterSolver_B.smax) / ThrusterSolver_B.smax;
  }

  // End of Product: '<S5>/Overall System'

  // MATLABSystem: '<Root>/System Limit'
  ParamGet_ThrusterSolver_148.getParameter(&ThrusterSolver_B.value_f);

  // Sum: '<S5>/Feed Forward Sys'
  ThrusterSolver_B.smax = -0.0;

  // Sum: '<S5>/Active Sys'
  ThrusterSolver_B.s = -0.0;
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
    // Sum: '<S5>/Feed Forward Sys' incorporates:
    //   Sum: '<S5>/Add Feed Forward'

    ThrusterSolver_B.smax += ThrusterSolver_B.value_k[ThrusterSolver_B.k];

    // Sum: '<S5>/Active Sys' incorporates:
    //   Product: '<S5>/Overall System'

    ThrusterSolver_B.s += ThrusterSolver_B.OverallSystem[ThrusterSolver_B.k];
  }

  // Sum: '<S5>/System Allocation' incorporates:
  //   MATLABSystem: '<Root>/System Limit'
  //   Sum: '<S5>/Feed Forward Sys'

  ThrusterSolver_B.smax = static_cast<real_T>(ThrusterSolver_B.value_f) -
    ThrusterSolver_B.smax;

  // Product: '<S5>/Overuse Multiplier' incorporates:
  //   Sum: '<S5>/Active Sys'
  //   Sum: '<S5>/System Overuse'

  ThrusterSolver_B.smax = (ThrusterSolver_B.s - ThrusterSolver_B.smax) /
    ThrusterSolver_B.smax;

  // MinMax: '<S5>/Max Individual Overuse' incorporates:
  //   Product: '<S5>/Individual Overuse Mutliplier'

  ThrusterSolver_B.s = ThrusterSolver_B.IndividualOveruseMutliplier[0];
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 7; ThrusterSolver_B.k++) {
    ThrusterSolver_B.u1 =
      ThrusterSolver_B.IndividualOveruseMutliplier[ThrusterSolver_B.k + 1];
    if ((!(ThrusterSolver_B.s >= ThrusterSolver_B.u1)) && (!rtIsNaN
         (ThrusterSolver_B.u1))) {
      ThrusterSolver_B.s = ThrusterSolver_B.u1;
    }
  }

  // MinMax: '<S5>/Max Overuse Multiplier' incorporates:
  //   MinMax: '<S5>/Max Individual Overuse'

  if ((!(ThrusterSolver_B.smax >= ThrusterSolver_B.s)) && (!rtIsNaN
       (ThrusterSolver_B.s))) {
    ThrusterSolver_B.smax = ThrusterSolver_B.s;
  }

  // End of MinMax: '<S5>/Max Overuse Multiplier'

  // Switch: '<S5>/Apply Overuse' incorporates:
  //   Constant: '<S5>/Constant'

  if (!(ThrusterSolver_B.smax > ThrusterSolver_P.ApplyOveruse_Threshold)) {
    ThrusterSolver_B.smax = ThrusterSolver_P.Constant_Value_n;
  }

  // Sum: '<S5>/Add' incorporates:
  //   Constant: '<S5>/Constant1'
  //   Switch: '<S5>/Apply Overuse'

  ThrusterSolver_B.smax += ThrusterSolver_P.Constant1_Value;

  // BusAssignment: '<Root>/Bus Assignment' incorporates:
  //   Constant: '<S2>/Constant'
  //   DataTypeConversion: '<Root>/Data Type Conversion'
  //   Product: '<S5>/Apply Multiplier'
  //   Product: '<S5>/Overall System'
  //   Sum: '<S5>/Add Feed Forward'

  ThrusterSolver_B.BusAssignment = ThrusterSolver_P.Constant_Value;
  for (ThrusterSolver_B.k = 0; ThrusterSolver_B.k < 8; ThrusterSolver_B.k++) {
    ThrusterSolver_B.BusAssignment.data[ThrusterSolver_B.k] =
      static_cast<real32_T>(ThrusterSolver_B.OverallSystem[ThrusterSolver_B.k] /
      ThrusterSolver_B.smax + ThrusterSolver_B.value_k[ThrusterSolver_B.k]);
  }

  // End of BusAssignment: '<Root>/Bus Assignment'

  // MATLABSystem: '<S4>/SinkBlock'
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

  {
    static const char_T tmp[17] = { 'm', 'y', 'n', 'a', 'm', 'e', 's', 'p', 'a',
      'c', 'e', '.', 'p', 'a', 'r', 'a', 'm' };

    // InitializeConditions for Memory: '<S5>/Previous B Matrix'
    memcpy(&ThrusterSolver_DW.PreviousBMatrix_PreviousInput[0],
           &ThrusterSolver_P.PreviousBMatrix_InitialConditio[0], 48U * sizeof
           (real_T));

    // InitializeConditions for Memory: '<S5>/Previous Weights'
    memcpy(&ThrusterSolver_DW.PreviousWeights_PreviousInput[0],
           &ThrusterSolver_P.PreviousWeights_InitialConditio[0], sizeof(real_T) <<
           3U);

    // SystemInitialize for Enabled SubSystem: '<S3>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S7>/In1' incorporates:
    //   Outport: '<S7>/Out1'

    ThrusterSolver_B.In1 = ThrusterSolver_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S3>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S6>/In1' incorporates:
    //   Outport: '<S6>/Out1'

    ThrusterSolver_B.In1_g = ThrusterSolver_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem'

    // Start for MATLABSystem: '<Root>/Get Thruster Weights' incorporates:
    //   MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
    //   MATLABSystem: '<Root>/Individual Limit'
    //   MATLABSystem: '<Root>/System Limit'

    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_d.isInitialized = 1;
    for (int32_T i = 0; i < 17; i++) {
      ThrusterSolver_B.prmName_tmp[i] = tmp[i];
    }

    real_T varargin_1;
    ThrusterSolver_B.prmName_tmp[17] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_146.initParam(&ThrusterSolver_B.prmName_tmp[0]);
    ParamGet_ThrusterSolver_146.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Thruster Weights'

    // Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_e.isInitialized = 1;
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_126.initParam(&ThrusterSolver_B.prmName_tmp[0]);
    ParamGet_ThrusterSolver_126.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_e.isSetupComplete = true;

    // Start for MATLABSystem: '<S3>/SourceBlock'
    ThrusterSolver_DW.obj_h.isInitialized = 0;
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = false;
    ThrusterSolv_SystemCore_setup_f(&ThrusterSolver_DW.obj_h);

    // Start for MATLABSystem: '<Root>/Individual Limit'
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_p.isInitialized = 1;
    ParamGet_ThrusterSolver_147.initParam(&ThrusterSolver_B.prmName_tmp[0]);
    ParamGet_ThrusterSolver_147.setInitialValue(0L);
    ThrusterSolver_DW.obj_p.isSetupComplete = true;

    // Start for MATLABSystem: '<S1>/SourceBlock'
    ThrusterSolver_DW.obj_c.isInitialized = 0;
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = false;
    ThrusterSolver_SystemCore_setup(&ThrusterSolver_DW.obj_c);

    // Start for MATLABSystem: '<Root>/System Limit'
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj.isInitialized = 1;
    ParamGet_ThrusterSolver_148.initParam(&ThrusterSolver_B.prmName_tmp[0]);
    ParamGet_ThrusterSolver_148.setInitialValue(0L);
    ThrusterSolver_DW.obj.isSetupComplete = true;

    // Start for MATLABSystem: '<S4>/SinkBlock'
    ThrusterSolver_DW.obj_eh.isInitialized = 0;
    ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted = false;
    ThrusterSol_SystemCore_setup_fa(&ThrusterSolver_DW.obj_eh);
  }
}

// Model terminate function
void ThrusterSolver::terminate()
{
  // Terminate for MATLABSystem: '<Root>/Get Thruster Weights'
  if (!ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Weights'

  // Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  if (!ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'

  // Terminate for MATLABSystem: '<S3>/SourceBlock'
  if (!ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_h.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/Individual Limit'
  if (!ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit'

  // Terminate for MATLABSystem: '<S1>/SourceBlock'
  if (!ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_c.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S1>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/System Limit'
  if (!ThrusterSolver_DW.obj.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/System Limit'

  // Terminate for MATLABSystem: '<S4>/SinkBlock'
  if (!ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_eh.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S4>/SinkBlock'
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
