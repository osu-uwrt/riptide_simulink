//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ThrusterSolver.cpp
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.43
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Thu Oct 12 04:58:06 2023
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

int64_T div_s64_floor(int64_T numerator, int64_T denominator)
{
  int64_T quotient;
  if (denominator == 0L) {
    quotient = numerator >= 0L ? MAX_int64_T : MIN_int64_T;

    // Divide by zero handler
  } else {
    uint64_T absDenominator;
    uint64_T absNumerator;
    uint64_T tempAbsQuotient;
    boolean_T quotientNeedsNegation;
    absNumerator = numerator < 0L ? ~static_cast<uint64_T>(numerator) + 1UL :
      static_cast<uint64_T>(numerator);
    absDenominator = denominator < 0L ? ~static_cast<uint64_T>(denominator) +
      1UL : static_cast<uint64_T>(denominator);
    quotientNeedsNegation = ((numerator < 0L) != (denominator < 0L));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0UL) {
        tempAbsQuotient++;
      }
    }

    quotient = quotientNeedsNegation ? -static_cast<int64_T>(tempAbsQuotient) :
      static_cast<int64_T>(tempAbsQuotient);
  }

  return quotient;
}

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
    ThrusterSolver_B.smax_g = std::abs(ThrusterSolver_B.A[jj]);
    for (d_k = 2; d_k <= n; d_k++) {
      ThrusterSolver_B.s_m = std::abs(ThrusterSolver_B.A[(jj + d_k) - 1]);
      if (ThrusterSolver_B.s_m > ThrusterSolver_B.smax_g) {
        jA = d_k - 1;
        ThrusterSolver_B.smax_g = ThrusterSolver_B.s_m;
      }
    }

    if (ThrusterSolver_B.A[jj + jA] != 0.0) {
      if (jA != 0) {
        n = c_j + jA;
        ThrusterSolver_B.ipiv[c_j] = static_cast<int8_T>(n + 1);
        for (jA = 0; jA < 8; jA++) {
          kAcol = jA << 3;
          d_k = kAcol + c_j;
          ThrusterSolver_B.smax_g = ThrusterSolver_B.A[d_k];
          kAcol += n;
          ThrusterSolver_B.A[d_k] = ThrusterSolver_B.A[kAcol];
          ThrusterSolver_B.A[kAcol] = ThrusterSolver_B.smax_g;
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
      ThrusterSolver_B.smax_g = ThrusterSolver_B.A[((d_k << 3) + jj) + 8];
      if (ThrusterSolver_B.smax_g != 0.0) {
        kAcol = (jA - c_j) + 6;
        for (int32_T ijA{jA}; ijA <= kAcol; ijA++) {
          ThrusterSolver_B.A[ijA - 1] += ThrusterSolver_B.A[((jj + ijA) - jA) +
            1] * -ThrusterSolver_B.smax_g;
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
      ThrusterSolver_B.smax_g = y[c_j];
      if (ThrusterSolver_B.smax_g != 0.0) {
        y[c_j] = ThrusterSolver_B.smax_g / ThrusterSolver_B.A[d_k + kAcol];
        for (int32_T ijA{0}; ijA < d_k; ijA++) {
          jj = ijA + jA;
          y[jj] -= ThrusterSolver_B.A[ijA + kAcol] * y[c_j];
        }
      }
    }
  }
}

// Function for MATLAB Function: '<Root>/Force to RPM Transform'
uint64_T ThrusterSolver::ThrusterSo_eml_mixed_uint64_mul(uint64_T nv, real_T y)
{
  uint64_T z;
  int32_T ex_t;
  if (std::isnan(y)) {
    z = 0UL;
  } else if (y <= 0.0) {
    z = 0UL;
  } else {
    real_T yd;
    yd = std::frexp(y, &ex_t);
    if (ex_t <= -64) {
      z = 0UL;
    } else {
      ThrusterSolver_B.yint = static_cast<uint64_T>(std::round(yd *
        9.007199254740992E+15));
      ThrusterSolver_B.b_y1 = ThrusterSolver_B.yint >> 32;
      ThrusterSolver_B.b_y0 = ThrusterSolver_B.yint & 4294967295UL;
      if (nv == 0UL) {
        z = 0UL;
      } else if (std::isinf(y)) {
        z = MAX_uint64_T;
      } else if (ex_t - 53 > 64) {
        z = MAX_uint64_T;
      } else {
        uint64_T temp;
        ThrusterSolver_B.n1 = nv >> 32;
        ThrusterSolver_B.n0 = nv & 4294967295UL;
        ThrusterSolver_B.yint = ThrusterSolver_B.n0 * ThrusterSolver_B.b_y0;
        ThrusterSolver_B.n0 *= ThrusterSolver_B.b_y1;
        ThrusterSolver_B.b_y0 *= ThrusterSolver_B.n1;
        temp = ((ThrusterSolver_B.n0 & 4294967295UL) + (ThrusterSolver_B.yint >>
                 32)) + (ThrusterSolver_B.b_y0 & 4294967295UL);
        ThrusterSolver_B.yint = (ThrusterSolver_B.yint & 4294967295UL) + (temp <<
          32);
        ThrusterSolver_B.b_y1 = ((ThrusterSolver_B.n1 * ThrusterSolver_B.b_y1 +
          (ThrusterSolver_B.n0 >> 32)) + (ThrusterSolver_B.b_y0 >> 32)) + (temp >>
          32);
        if (ex_t - 53 >= 0) {
          if (ThrusterSolver_B.b_y1 > 0UL) {
            z = MAX_uint64_T;
          } else {
            int16_T tmp;
            tmp = static_cast<int16_T>(117 - ex_t);
            if (static_cast<int16_T>(117 - ex_t) < 0) {
              tmp = 0;
            }

            if ((117 - ex_t < 64) && ((ThrusterSolver_B.yint >>
                  static_cast<uint8_T>(tmp)) > 0UL)) {
              z = MAX_uint64_T;
            } else {
              tmp = static_cast<int16_T>(ex_t - 53);
              if (static_cast<int16_T>(ex_t - 53) < 0) {
                tmp = 0;
              } else if (static_cast<int16_T>(ex_t - 53) > 255) {
                tmp = 255;
              }

              z = ThrusterSolver_B.yint << static_cast<uint8_T>(tmp);
            }
          }
        } else if (ex_t - 53 > -64) {
          int16_T tmp;
          tmp = static_cast<int16_T>(53 - ex_t);
          if (static_cast<int16_T>(53 - ex_t) < 0) {
            tmp = 0;
          }

          if ((ThrusterSolver_B.b_y1 >> static_cast<uint8_T>(tmp)) > 0UL) {
            z = MAX_uint64_T;
          } else {
            int16_T tmp_0;
            int16_T tmp_1;
            tmp = static_cast<int16_T>(53 - ex_t);
            if (static_cast<int16_T>(53 - ex_t) < 0) {
              tmp = 0;
            }

            tmp_0 = static_cast<int16_T>(ex_t + 11);
            if (static_cast<int16_T>(ex_t + 11) < 0) {
              tmp_0 = 0;
            } else if (static_cast<int16_T>(ex_t + 11) > 255) {
              tmp_0 = 255;
            }

            tmp_1 = static_cast<int16_T>(52 - ex_t);
            if (static_cast<int16_T>(52 - ex_t) < 0) {
              tmp_1 = 0;
            }

            z = ((ThrusterSolver_B.yint >> static_cast<uint8_T>(tmp)) +
                 (ThrusterSolver_B.b_y1 << static_cast<uint8_T>(tmp_0))) +
              (ThrusterSolver_B.yint >> static_cast<uint8_T>(tmp_1) & 1UL);
          }
        } else if (ex_t - 53 == -64) {
          z = (ThrusterSolver_B.yint >> 63 & 1UL) + ThrusterSolver_B.b_y1;
        } else {
          int16_T tmp;
          int16_T tmp_0;
          tmp = static_cast<int16_T>(-(ex_t + 11));
          if (static_cast<int16_T>(-(ex_t + 11)) < 0) {
            tmp = 0;
          }

          tmp_0 = static_cast<int16_T>(-(ex_t + 12));
          if (static_cast<int16_T>(-(ex_t + 12)) < 0) {
            tmp_0 = 0;
          }

          z = (ThrusterSolver_B.b_y1 >> static_cast<uint8_T>(tmp_0) & 1UL) +
            (ThrusterSolver_B.b_y1 >> static_cast<uint8_T>(tmp));
        }
      }
    }
  }

  return z;
}

// Function for MATLAB Function: '<Root>/Force to RPM Transform'
int64_T ThrusterSolver::ThrusterSolver_times(int64_T x, real_T y)
{
  int64_T z;
  if (y < 0.0) {
    if (x < 0L) {
      if (x > MIN_int64_T) {
        ThrusterSolver_B.prod_unsgn = static_cast<uint64_T>(-x);
      } else {
        ThrusterSolver_B.prod_unsgn = 9223372036854775808UL;
      }
    } else {
      ThrusterSolver_B.prod_unsgn = static_cast<uint64_T>(x);
    }

    ThrusterSolver_B.prod_unsgn = ThrusterSo_eml_mixed_uint64_mul
      (ThrusterSolver_B.prod_unsgn, -y);
  } else {
    if (x < 0L) {
      if (x > MIN_int64_T) {
        ThrusterSolver_B.prod_unsgn = static_cast<uint64_T>(-x);
      } else {
        ThrusterSolver_B.prod_unsgn = 9223372036854775808UL;
      }
    } else {
      ThrusterSolver_B.prod_unsgn = static_cast<uint64_T>(x);
    }

    ThrusterSolver_B.prod_unsgn = ThrusterSo_eml_mixed_uint64_mul
      (ThrusterSolver_B.prod_unsgn, y);
  }

  if (((x < 0L) && (y > 0.0)) || ((x > 0L) && (y < 0.0))) {
    if (ThrusterSolver_B.prod_unsgn <= 9223372036854775807UL) {
      z = -static_cast<int64_T>(ThrusterSolver_B.prod_unsgn);
    } else {
      z = MIN_int64_T;
    }
  } else if (ThrusterSolver_B.prod_unsgn <= 9223372036854775807UL) {
    z = static_cast<int64_T>(ThrusterSolver_B.prod_unsgn);
  } else {
    z = MAX_int64_T;
  }

  return z;
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
    ThrusterSolver_B.b_zeroDelimTopic_f[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_f[15] = '\x00';
  Pub_ThrusterSolver_226.createPublisher(&ThrusterSolver_B.b_zeroDelimTopic_f[0],
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
    ThrusterSolver_B.b_zeroDelimTopic_c[i] = tmp[i];
  }

  ThrusterSolver_B.b_zeroDelimTopic_c[31] = '\x00';
  Sub_ThrusterSolver_114.createSubscriber(&ThrusterSolver_B.b_zeroDelimTopic_c[0],
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

// Model step function
void ThrusterSolver::step()
{
  int32_T e_k;
  int32_T jj;
  int32_T k;
  boolean_T b_varargout_1;
  boolean_T rtb_Recalculate;

  // MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  ParamGet_ThrusterSolver_126.getParameter(48U, &ThrusterSolver_B.value[0],
    &ThrusterSolver_B.len);

  // MATLABSystem: '<Root>/Scaling Factor'
  ParamGet_ThrusterSolver_197.getParameter(&ThrusterSolver_B.value_n);

  // Product: '<Root>/Divide1' incorporates:
  //   MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
  //   MATLABSystem: '<Root>/Scaling Factor'
  //   Math: '<Root>/Transpose'

  for (k = 0; k < 6; k++) {
    for (jj = 0; jj < 8; jj++) {
      ThrusterSolver_B.Divide1[jj + (k << 3)] = ThrusterSolver_B.value[6 * jj +
        k] / static_cast<real_T>(ThrusterSolver_B.value_n);
    }
  }

  // End of Product: '<Root>/Divide1'

  // MATLABSystem: '<S2>/SourceBlock'
  rtb_Recalculate = Sub_ThrusterSolver_114.getLatestMessage
    (&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S7>/Enable'

  if (rtb_Recalculate) {
    // SignalConversion generated from: '<S7>/In1'
    ThrusterSolver_B.In1 = ThrusterSolver_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

  // MATLABSystem: '<S1>/SourceBlock'
  b_varargout_1 = Sub_ThrusterSolver_115.getLatestMessage
    (&ThrusterSolver_B.b_varargout_2);

  // Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S6>/Enable'

  if (b_varargout_1) {
    // SignalConversion generated from: '<S6>/In1'
    ThrusterSolver_B.In1_g = ThrusterSolver_B.b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S1>/Enabled Subsystem'

  // MATLABSystem: '<Root>/System Limit'
  ParamGet_ThrusterSolver_148.getParameter(&ThrusterSolver_B.value_p);

  // MATLABSystem: '<Root>/Individual Limit'
  ParamGet_ThrusterSolver_147.getParameter(&ThrusterSolver_B.value_l);

  // Logic: '<Root>/Recalculate' incorporates:
  //   MATLABSystem: '<S1>/SourceBlock'
  //   MATLABSystem: '<S2>/SourceBlock'

  rtb_Recalculate = (rtb_Recalculate || b_varargout_1);

  // MATLABSystem: '<Root>/Get Thruster Weights'
  ParamGet_ThrusterSolver_146.getParameter(8U, &ThrusterSolver_B.value_c[0],
    &ThrusterSolver_B.len);

  // Product: '<Root>/Divide' incorporates:
  //   MATLABSystem: '<Root>/Get Thruster Weights'
  //   MATLABSystem: '<Root>/Scaling Factor'

  for (k = 0; k < 8; k++) {
    ThrusterSolver_B.value_c[k] /= static_cast<real_T>(ThrusterSolver_B.value_n);
  }

  // End of Product: '<Root>/Divide'

  // Outputs for Triggered SubSystem: '<Root>/Thruster Solver' incorporates:
  //   TriggerPort: '<S5>/Trigger'

  if (rtb_Recalculate && (ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE !=
                          POS_ZCSIG)) {
    boolean_T b_p;
    boolean_T exitg1;

    // MATLAB Function: '<S5>/MATLAB Function' incorporates:
    //   Product: '<Root>/Divide'
    //   Product: '<Root>/Divide1'

    b_varargout_1 = false;
    b_p = true;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < 8)) {
      if (!(ThrusterSolver_B.value_c[k] ==
            ThrusterSolver_DW.PreviousWeights_PreviousInput[k])) {
        b_p = false;
        exitg1 = true;
      } else {
        k++;
      }
    }

    if (b_p) {
      b_varargout_1 = true;
    }

    if (!b_varargout_1) {
      int32_T jA;
      int32_T kAcol;
      int32_T rtb_Divide1_tmp;
      std::memset(&ThrusterSolver_B.weightsMatrix[0], 0, sizeof(real_T) << 6U);
      for (k = 0; k < 8; k++) {
        ThrusterSolver_B.weightsMatrix[k + (k << 3)] =
          ThrusterSolver_B.value_c[k];
      }

      ThrusterSolver_inv(ThrusterSolver_B.weightsMatrix, ThrusterSolver_B.A_tmp);
      for (k = 0; k < 6; k++) {
        for (jj = 0; jj < 8; jj++) {
          rtb_Divide1_tmp = 6 * jj + k;
          ThrusterSolver_B.value[rtb_Divide1_tmp] = 0.0;
          for (jA = 0; jA < 8; jA++) {
            ThrusterSolver_B.value[rtb_Divide1_tmp] += ThrusterSolver_B.Divide1
              [(k << 3) + jA] * ThrusterSolver_B.A_tmp[(jj << 3) + jA];
          }
        }

        for (jj = 0; jj < 6; jj++) {
          rtb_Divide1_tmp = 6 * jj + k;
          ThrusterSolver_B.A_m[rtb_Divide1_tmp] = 0.0;
          for (jA = 0; jA < 8; jA++) {
            ThrusterSolver_B.A_m[rtb_Divide1_tmp] += ThrusterSolver_B.value[6 *
              jA + k] * ThrusterSolver_B.Divide1[(jj << 3) + jA];
          }
        }
      }

      std::memset(&ThrusterSolver_B.b[0], 0, 36U * sizeof(real_T));
      for (k = 0; k < 6; k++) {
        ThrusterSolver_B.ipiv_j[k] = static_cast<int8_T>(k + 1);
      }

      for (k = 0; k < 5; k++) {
        jj = k * 7;
        rtb_Divide1_tmp = 6 - k;
        jA = 0;
        ThrusterSolver_B.smax = std::abs(ThrusterSolver_B.A_m[jj]);
        for (e_k = 2; e_k <= rtb_Divide1_tmp; e_k++) {
          ThrusterSolver_B.s = std::abs(ThrusterSolver_B.A_m[(jj + e_k) - 1]);
          if (ThrusterSolver_B.s > ThrusterSolver_B.smax) {
            jA = e_k - 1;
            ThrusterSolver_B.smax = ThrusterSolver_B.s;
          }
        }

        if (ThrusterSolver_B.A_m[jj + jA] != 0.0) {
          if (jA != 0) {
            jA += k;
            ThrusterSolver_B.ipiv_j[k] = static_cast<int8_T>(jA + 1);
            for (e_k = 0; e_k < 6; e_k++) {
              kAcol = e_k * 6 + k;
              ThrusterSolver_B.smax = ThrusterSolver_B.A_m[kAcol];
              rtb_Divide1_tmp = e_k * 6 + jA;
              ThrusterSolver_B.A_m[kAcol] = ThrusterSolver_B.A_m[rtb_Divide1_tmp];
              ThrusterSolver_B.A_m[rtb_Divide1_tmp] = ThrusterSolver_B.smax;
            }
          }

          rtb_Divide1_tmp = (jj - k) + 6;
          for (jA = jj + 2; jA <= rtb_Divide1_tmp; jA++) {
            ThrusterSolver_B.A_m[jA - 1] /= ThrusterSolver_B.A_m[jj];
          }
        }

        rtb_Divide1_tmp = 4 - k;
        jA = jj + 8;
        for (e_k = 0; e_k <= rtb_Divide1_tmp; e_k++) {
          ThrusterSolver_B.s = ThrusterSolver_B.A_m[(e_k * 6 + jj) + 6];
          if (ThrusterSolver_B.s != 0.0) {
            kAcol = (jA - k) + 4;
            for (int32_T ijA{jA}; ijA <= kAcol; ijA++) {
              ThrusterSolver_B.A_m[ijA - 1] += ThrusterSolver_B.A_m[((jj + ijA)
                - jA) + 1] * -ThrusterSolver_B.s;
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
        ipiv = ThrusterSolver_B.ipiv_j[k];
        if (ipiv > k + 1) {
          jj = ThrusterSolver_B.c_p[ipiv - 1];
          ThrusterSolver_B.c_p[ipiv - 1] = ThrusterSolver_B.c_p[k];
          ThrusterSolver_B.c_p[k] = static_cast<int8_T>(jj);
        }
      }

      for (rtb_Divide1_tmp = 0; rtb_Divide1_tmp < 6; rtb_Divide1_tmp++) {
        jj = (ThrusterSolver_B.c_p[rtb_Divide1_tmp] - 1) * 6;
        ThrusterSolver_B.b[rtb_Divide1_tmp + jj] = 1.0;
        for (jA = rtb_Divide1_tmp + 1; jA < 7; jA++) {
          k = (jj + jA) - 1;
          if (ThrusterSolver_B.b[k] != 0.0) {
            for (e_k = jA + 1; e_k < 7; e_k++) {
              kAcol = (jj + e_k) - 1;
              ThrusterSolver_B.b[kAcol] -= ThrusterSolver_B.A_m[((jA - 1) * 6 +
                e_k) - 1] * ThrusterSolver_B.b[k];
            }
          }
        }
      }

      for (rtb_Divide1_tmp = 0; rtb_Divide1_tmp < 6; rtb_Divide1_tmp++) {
        jA = 6 * rtb_Divide1_tmp;
        for (e_k = 5; e_k >= 0; e_k--) {
          kAcol = 6 * e_k;
          k = e_k + jA;
          ThrusterSolver_B.s = ThrusterSolver_B.b[k];
          if (ThrusterSolver_B.s != 0.0) {
            ThrusterSolver_B.b[k] = ThrusterSolver_B.s /
              ThrusterSolver_B.A_m[e_k + kAcol];
            for (int32_T ijA{0}; ijA < e_k; ijA++) {
              jj = ijA + jA;
              ThrusterSolver_B.b[jj] -= ThrusterSolver_B.A_m[ijA + kAcol] *
                ThrusterSolver_B.b[k];
            }
          }
        }
      }

      for (k = 0; k < 8; k++) {
        for (jj = 0; jj < 6; jj++) {
          rtb_Divide1_tmp = jj << 3;
          e_k = k + rtb_Divide1_tmp;
          ThrusterSolver_B.value[e_k] = 0.0;
          for (jA = 0; jA < 8; jA++) {
            ThrusterSolver_B.value[e_k] += ThrusterSolver_B.A_tmp[(jA << 3) + k]
              * ThrusterSolver_B.Divide1[rtb_Divide1_tmp + jA];
          }
        }

        for (jj = 0; jj < 6; jj++) {
          rtb_Divide1_tmp = (jj << 3) + k;
          ThrusterSolver_DW.PreviousBMatrix_PreviousInput[rtb_Divide1_tmp] = 0.0;
          for (jA = 0; jA < 6; jA++) {
            ThrusterSolver_DW.PreviousBMatrix_PreviousInput[rtb_Divide1_tmp] +=
              ThrusterSolver_B.value[(jA << 3) + k] * ThrusterSolver_B.b[6 * jj
              + jA];
          }
        }

        ThrusterSolver_DW.PreviousWeights_PreviousInput[k] =
          ThrusterSolver_B.value_c[k];
      }
    }

    // End of MATLAB Function: '<S5>/MATLAB Function'

    // Product: '<S5>/Overall System' incorporates:
    //   SignalConversion generated from: '<S5>/Overall System'

    ThrusterSolver_B.dv[0] = ThrusterSolver_B.In1_g.linear.x;
    ThrusterSolver_B.dv[1] = ThrusterSolver_B.In1_g.linear.y;
    ThrusterSolver_B.dv[2] = ThrusterSolver_B.In1_g.linear.z;
    ThrusterSolver_B.dv[3] = ThrusterSolver_B.In1_g.angular.x;
    ThrusterSolver_B.dv[4] = ThrusterSolver_B.In1_g.angular.y;
    ThrusterSolver_B.dv[5] = ThrusterSolver_B.In1_g.angular.z;

    // Product: '<S5>/Feed Forward ' incorporates:
    //   SignalConversion generated from: '<S5>/Feed Forward '

    ThrusterSolver_B.dv1[0] = ThrusterSolver_B.In1.linear.x;
    ThrusterSolver_B.dv1[1] = ThrusterSolver_B.In1.linear.y;
    ThrusterSolver_B.dv1[2] = ThrusterSolver_B.In1.linear.z;
    ThrusterSolver_B.dv1[3] = ThrusterSolver_B.In1.angular.x;
    ThrusterSolver_B.dv1[4] = ThrusterSolver_B.In1.angular.y;
    ThrusterSolver_B.dv1[5] = ThrusterSolver_B.In1.angular.z;

    // Sum: '<S5>/Feed Forward Sys'
    ThrusterSolver_B.s = -0.0;

    // Sum: '<S5>/Active Sys'
    ThrusterSolver_B.smax = -0.0;
    for (k = 0; k < 8; k++) {
      // Product: '<S5>/Overall System' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate'

      ThrusterSolver_B.RPMs[k] = 0.0;

      // Product: '<S5>/Feed Forward ' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate1'

      ThrusterSolver_B.value_c[k] = 0.0;
      for (jj = 0; jj < 6; jj++) {
        // Product: '<S5>/Overall System' incorporates:
        //   Concatenate: '<S4>/Vector Concatenate'
        //   Product: '<S5>/Feed Forward '

        ThrusterSolver_B.rtb_VectorConcatenate_tmp =
          ThrusterSolver_DW.PreviousBMatrix_PreviousInput[(jj << 3) + k];
        ThrusterSolver_B.RPMs[k] += ThrusterSolver_B.rtb_VectorConcatenate_tmp *
          ThrusterSolver_B.dv[jj];

        // Product: '<S5>/Feed Forward ' incorporates:
        //   Concatenate: '<S4>/Vector Concatenate1'

        ThrusterSolver_B.value_c[k] +=
          ThrusterSolver_B.rtb_VectorConcatenate_tmp * ThrusterSolver_B.dv1[jj];
      }

      // Sum: '<S5>/Individual Allocation' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate'
      //   Concatenate: '<S4>/Vector Concatenate1'
      //   MATLABSystem: '<Root>/Individual Limit'
      //   Product: '<S5>/Individual Overuse Mutliplier'
      //   Sum: '<S5>/Individual Overuse'

      ThrusterSolver_B.rtb_VectorConcatenate_tmp = static_cast<real_T>
        (ThrusterSolver_B.value_l) - ThrusterSolver_B.value_c[k];
      ThrusterSolver_B.value[k] = (ThrusterSolver_B.RPMs[k] -
        ThrusterSolver_B.rtb_VectorConcatenate_tmp) /
        ThrusterSolver_B.rtb_VectorConcatenate_tmp;

      // Sum: '<S5>/Feed Forward Sys' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate1'

      ThrusterSolver_B.s += ThrusterSolver_B.value_c[k];

      // Sum: '<S5>/Active Sys' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate'

      ThrusterSolver_B.smax += ThrusterSolver_B.RPMs[k];
    }

    // Sum: '<S5>/System Allocation' incorporates:
    //   MATLABSystem: '<Root>/System Limit'
    //   Sum: '<S5>/Feed Forward Sys'

    ThrusterSolver_B.s = static_cast<real_T>(ThrusterSolver_B.value_p) -
      ThrusterSolver_B.s;

    // Product: '<S5>/Overuse Multiplier' incorporates:
    //   Sum: '<S5>/Active Sys'
    //   Sum: '<S5>/System Overuse'

    ThrusterSolver_B.s = (ThrusterSolver_B.smax - ThrusterSolver_B.s) /
      ThrusterSolver_B.s;

    // MinMax: '<S5>/Max Individual Overuse'
    ThrusterSolver_B.smax = ThrusterSolver_B.value[0];
    for (k = 0; k < 7; k++) {
      ThrusterSolver_B.smax = std::fmax(ThrusterSolver_B.smax,
        ThrusterSolver_B.value[k + 1]);
    }

    // MinMax: '<S5>/Max Overuse Multiplier' incorporates:
    //   MinMax: '<S5>/Max Individual Overuse'

    ThrusterSolver_B.s = std::fmax(ThrusterSolver_B.s, ThrusterSolver_B.smax);

    // Switch: '<S5>/Apply Overuse' incorporates:
    //   Constant: '<S5>/Constant'

    if (!(ThrusterSolver_B.s > ThrusterSolver_P.ApplyOveruse_Threshold)) {
      ThrusterSolver_B.s = ThrusterSolver_P.Constant_Value_n;
    }

    // Sum: '<S5>/Add' incorporates:
    //   Constant: '<S5>/Constant1'
    //   Switch: '<S5>/Apply Overuse'

    ThrusterSolver_B.smax = ThrusterSolver_B.s +
      ThrusterSolver_P.Constant1_Value_m;
    for (e_k = 0; e_k < 8; e_k++) {
      // Sum: '<S5>/Add Feed Forward' incorporates:
      //   Concatenate: '<S4>/Vector Concatenate'
      //   Concatenate: '<S4>/Vector Concatenate1'
      //   Product: '<S5>/Apply Multiplier'

      ThrusterSolver_B.AddFeedForward[e_k] = ThrusterSolver_B.RPMs[e_k] /
        ThrusterSolver_B.smax + ThrusterSolver_B.value_c[e_k];
    }
  }

  ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE = rtb_Recalculate;

  // End of Outputs for SubSystem: '<Root>/Thruster Solver'

  // Product: '<Root>/Apply Scaling' incorporates:
  //   MATLABSystem: '<Root>/Scaling Factor'
  //   Sum: '<S5>/Add Feed Forward'

  for (k = 0; k < 8; k++) {
    ThrusterSolver_B.value_c[k] = ThrusterSolver_B.AddFeedForward[k] *
      static_cast<real_T>(ThrusterSolver_B.value_n);
  }

  // End of Product: '<Root>/Apply Scaling'

  // MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  ParamGet_ThrusterSolver_205.getParameter(8U, &ThrusterSolver_B.value_k[0],
    &ThrusterSolver_B.len);

  // Product: '<Root>/Apply Scaling Factor' incorporates:
  //   MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  //   MATLABSystem: '<Root>/Scaling Factor'

  for (k = 0; k < 8; k++) {
    ThrusterSolver_B.value_k[k] = div_s64_floor(ThrusterSolver_B.value_k[k],
      ThrusterSolver_B.value_n);
  }

  // End of Product: '<Root>/Apply Scaling Factor'

  // MATLAB Function: '<Root>/Force to RPM Transform' incorporates:
  //   Product: '<Root>/Apply Scaling Factor'
  //   Sum: '<S5>/Add Feed Forward'

  for (e_k = 0; e_k < 8; e_k++) {
    if (ThrusterSolver_B.AddFeedForward[e_k] > 0.0) {
      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[1],
        ThrusterSolver_B.AddFeedForward[e_k]);
      if ((ThrusterSolver_B.value_k[0] < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_k[0])) {
        ThrusterSolver_B.value_p = MIN_int64_T;
      } else if ((ThrusterSolver_B.value_k[0] > 0L) && (ThrusterSolver_B.value_n
                  > MAX_int64_T - ThrusterSolver_B.value_k[0])) {
        ThrusterSolver_B.value_p = MAX_int64_T;
      } else {
        ThrusterSolver_B.value_p = ThrusterSolver_B.value_k[0] +
          ThrusterSolver_B.value_n;
      }

      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[2],
        std::tanh(ThrusterSolver_B.AddFeedForward[e_k]));
      if ((ThrusterSolver_B.value_p < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.value_p = MIN_int64_T;
      } else if ((ThrusterSolver_B.value_p > 0L) && (ThrusterSolver_B.value_n >
                  MAX_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.value_p = MAX_int64_T;
      } else {
        ThrusterSolver_B.value_p += ThrusterSolver_B.value_n;
      }

      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[3],
        rt_powd_snf(ThrusterSolver_B.AddFeedForward[e_k], 0.25));
      if ((ThrusterSolver_B.value_p < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.RPMs[e_k] = -9.2233720368547758E+18;
      } else if ((ThrusterSolver_B.value_p > 0L) && (ThrusterSolver_B.value_n >
                  MAX_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.RPMs[e_k] = 9.2233720368547758E+18;
      } else {
        ThrusterSolver_B.RPMs[e_k] = static_cast<real_T>
          (ThrusterSolver_B.value_p + ThrusterSolver_B.value_n);
      }
    } else {
      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[5],
        ThrusterSolver_B.AddFeedForward[e_k]);
      if ((ThrusterSolver_B.value_k[4] < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_k[4])) {
        ThrusterSolver_B.value_p = MIN_int64_T;
      } else if ((ThrusterSolver_B.value_k[4] > 0L) && (ThrusterSolver_B.value_n
                  > MAX_int64_T - ThrusterSolver_B.value_k[4])) {
        ThrusterSolver_B.value_p = MAX_int64_T;
      } else {
        ThrusterSolver_B.value_p = ThrusterSolver_B.value_k[4] +
          ThrusterSolver_B.value_n;
      }

      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[6],
        std::tanh(ThrusterSolver_B.AddFeedForward[e_k]));
      if ((ThrusterSolver_B.value_p < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.value_p = MIN_int64_T;
      } else if ((ThrusterSolver_B.value_p > 0L) && (ThrusterSolver_B.value_n >
                  MAX_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.value_p = MAX_int64_T;
      } else {
        ThrusterSolver_B.value_p += ThrusterSolver_B.value_n;
      }

      ThrusterSolver_B.value_n = ThrusterSolver_times(ThrusterSolver_B.value_k[7],
        rt_powd_snf(ThrusterSolver_B.AddFeedForward[e_k], 0.25));
      if ((ThrusterSolver_B.value_p < 0L) && (ThrusterSolver_B.value_n <
           MIN_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.RPMs[e_k] = -9.2233720368547758E+18;
      } else if ((ThrusterSolver_B.value_p > 0L) && (ThrusterSolver_B.value_n >
                  MAX_int64_T - ThrusterSolver_B.value_p)) {
        ThrusterSolver_B.RPMs[e_k] = 9.2233720368547758E+18;
      } else {
        ThrusterSolver_B.RPMs[e_k] = static_cast<real_T>
          (ThrusterSolver_B.value_p + ThrusterSolver_B.value_n);
      }
    }
  }

  // End of MATLAB Function: '<Root>/Force to RPM Transform'

  // Outputs for Triggered SubSystem: '<Root>/Publish To ROS' incorporates:
  //   TriggerPort: '<S4>/Trigger'

  if (rtb_Recalculate && (ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE !=
                          POS_ZCSIG)) {
    // DataTypeConversion: '<S4>/Data Type Conversion4' incorporates:
    //   Constant: '<S4>/Constant2'

    ThrusterSolver_B.s = std::floor(ThrusterSolver_P.Constant2_Value);
    if (std::isnan(ThrusterSolver_B.s) || std::isinf(ThrusterSolver_B.s)) {
      ThrusterSolver_B.s = 0.0;
    } else {
      ThrusterSolver_B.s = std::fmod(ThrusterSolver_B.s, 4.294967296E+9);
    }

    ThrusterSolver_B.len = ThrusterSolver_B.s < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.s))) :
      static_cast<uint32_T>(ThrusterSolver_B.s);

    // End of DataTypeConversion: '<S4>/Data Type Conversion4'

    // BusAssignment: '<S4>/Bus Assignment2' incorporates:
    //   Constant: '<S9>/Constant'
    //   DataTypeConversion: '<S4>/Data Type Conversion2'

    ThrusterSolver_B.BusAssignment2 = ThrusterSolver_P.Constant_Value;
    for (e_k = 0; e_k < 8; e_k++) {
      // DataTypeConversion: '<S4>/Data Type Conversion2'
      ThrusterSolver_B.s = std::floor(ThrusterSolver_B.value_c[e_k]);
      if (std::isnan(ThrusterSolver_B.s) || std::isinf(ThrusterSolver_B.s)) {
        ThrusterSolver_B.s = 0.0;
      } else {
        ThrusterSolver_B.s = std::fmod(ThrusterSolver_B.s, 4.294967296E+9);
      }

      ThrusterSolver_B.BusAssignment2.data[e_k] = ThrusterSolver_B.s < 0.0 ? -
        static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.s)) :
        static_cast<int32_T>(static_cast<uint32_T>(ThrusterSolver_B.s));
    }

    ThrusterSolver_B.BusAssignment2.data_SL_Info.CurrentLength =
      ThrusterSolver_B.len;
    ThrusterSolver_B.BusAssignment2.data_SL_Info.ReceivedLength =
      ThrusterSolver_B.len;

    // End of BusAssignment: '<S4>/Bus Assignment2'

    // MATLABSystem: '<S11>/SinkBlock'
    Pub_ThrusterSolver_226.publish(&ThrusterSolver_B.BusAssignment2);

    // DataTypeConversion: '<S4>/Data Type Conversion3' incorporates:
    //   Constant: '<S4>/Constant1'

    ThrusterSolver_B.s = std::floor(ThrusterSolver_P.Constant1_Value);
    if (std::isnan(ThrusterSolver_B.s) || std::isinf(ThrusterSolver_B.s)) {
      ThrusterSolver_B.s = 0.0;
    } else {
      ThrusterSolver_B.s = std::fmod(ThrusterSolver_B.s, 4.294967296E+9);
    }

    ThrusterSolver_B.len = ThrusterSolver_B.s < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.s))) :
      static_cast<uint32_T>(ThrusterSolver_B.s);

    // End of DataTypeConversion: '<S4>/Data Type Conversion3'

    // BusAssignment: '<S4>/Bus Assignment1' incorporates:
    //   Constant: '<S8>/Constant'
    //   DataTypeConversion: '<S4>/Data Type Conversion1'

    ThrusterSolver_B.BusAssignment2 = ThrusterSolver_P.Constant_Value_k;
    for (e_k = 0; e_k < 8; e_k++) {
      // DataTypeConversion: '<S4>/Data Type Conversion1'
      ThrusterSolver_B.s = std::floor(ThrusterSolver_B.RPMs[e_k]);
      if (std::isnan(ThrusterSolver_B.s) || std::isinf(ThrusterSolver_B.s)) {
        ThrusterSolver_B.s = 0.0;
      } else {
        ThrusterSolver_B.s = std::fmod(ThrusterSolver_B.s, 4.294967296E+9);
      }

      ThrusterSolver_B.BusAssignment2.data[e_k] = ThrusterSolver_B.s < 0.0 ? -
        static_cast<int32_T>(static_cast<uint32_T>(-ThrusterSolver_B.s)) :
        static_cast<int32_T>(static_cast<uint32_T>(ThrusterSolver_B.s));
    }

    ThrusterSolver_B.BusAssignment2.data_SL_Info.CurrentLength =
      ThrusterSolver_B.len;
    ThrusterSolver_B.BusAssignment2.data_SL_Info.ReceivedLength =
      ThrusterSolver_B.len;

    // End of BusAssignment: '<S4>/Bus Assignment1'

    // MATLABSystem: '<S10>/SinkBlock'
    Pub_ThrusterSolver_210.publish(&ThrusterSolver_B.BusAssignment2);
  }

  // Outputs for Triggered SubSystem: '<Root>/Thruster Solver' incorporates:
  //   TriggerPort: '<S5>/Trigger'

  ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE = rtb_Recalculate;

  // End of Outputs for SubSystem: '<Root>/Thruster Solver'
  // End of Outputs for SubSystem: '<Root>/Publish To ROS'
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

    static const char_T tmp_4[29]{ 't', 'a', 'l', 'o', 's', '_', 'f', 'o', 'r',
      'c', 'e', '_', 'c', 'u', 'r', 'v', 'e', '_', 'c', 'o', 'e', 'f', 'f', 'i',
      'c', 'e', 'n', 't', 's' };

    static const char_T tmp_3[22]{ 't', 'a', 'l', 'o', 's', '_', 't', 'h', 'r',
      'u', 's', 't', 'e', 'r', '_', 'w', 'e', 'i', 'g', 'h', 't', 's' };

    static const char_T tmp[15]{ 't', 'a', 'l', 'o', 's', '_', 'w', 'r', 'e',
      'n', 'c', 'h', 'm', 'a', 't' };

    static const char_T tmp_2[15]{ 't', 'a', 'l', 'o', 's', '_', 'i', 'n', 'd',
      'i', 'v', '_', 'l', 'i', 'm' };

    static const char_T tmp_1[13]{ 't', 'a', 'l', 'o', 's', '_', 's', 'y', 's',
      '_', 'l', 'i', 'm' };

    real_T varargin_1;
    ThrusterSolver_PrevZCX.PublishToROS_Trig_ZCE = POS_ZCSIG;
    ThrusterSolver_PrevZCX.ThrusterSolver_Trig_ZCE = POS_ZCSIG;

    // SystemInitialize for Enabled SubSystem: '<S2>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S7>/In1' incorporates:
    //   Outport: '<S7>/Out1'

    ThrusterSolver_B.In1 = ThrusterSolver_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S2>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem'
    // SystemInitialize for SignalConversion generated from: '<S6>/In1' incorporates:
    //   Outport: '<S6>/Out1'

    ThrusterSolver_B.In1_g = ThrusterSolver_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem'

    // SystemInitialize for Triggered SubSystem: '<Root>/Thruster Solver'
    // InitializeConditions for Memory: '<S5>/Previous B Matrix'
    std::memcpy(&ThrusterSolver_DW.PreviousBMatrix_PreviousInput[0],
                &ThrusterSolver_P.PreviousBMatrix_InitialConditio[0], 48U *
                sizeof(real_T));
    for (int32_T i{0}; i < 8; i++) {
      // InitializeConditions for Memory: '<S5>/Previous Weights'
      ThrusterSolver_DW.PreviousWeights_PreviousInput[i] =
        ThrusterSolver_P.PreviousWeights_InitialConditio[i];

      // SystemInitialize for Sum: '<S5>/Add Feed Forward' incorporates:
      //   Outport: '<S5>/Thruster Forces'

      ThrusterSolver_B.AddFeedForward[i] = ThrusterSolver_P.ThrusterForces_Y0;
    }

    // End of SystemInitialize for SubSystem: '<Root>/Thruster Solver'

    // SystemInitialize for Triggered SubSystem: '<Root>/Publish To ROS'
    // Start for MATLABSystem: '<S11>/SinkBlock'
    ThrusterSolver_DW.obj_l.isInitialized = 0;
    ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted = false;
    ThrusterSo_SystemCore_setup_fam(&ThrusterSolver_DW.obj_l);

    // Start for MATLABSystem: '<S10>/SinkBlock'
    ThrusterSolver_DW.obj_db.isInitialized = 0;
    ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted = false;
    ThrusterSol_SystemCore_setup_fa(&ThrusterSolver_DW.obj_db);

    // End of SystemInitialize for SubSystem: '<Root>/Publish To ROS'

    // Start for MATLABSystem: '<Root>/Get Thruster Wrench Matrix'
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_e.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_c[i] = tmp[i];
    }

    ThrusterSolver_B.prmName_c[15] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_126.initParam(&ThrusterSolver_B.prmName_c[0]);
    ParamGet_ThrusterSolver_126.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_e.isSetupComplete = true;

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
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj.isInitialized = 1;
    for (int32_T i{0}; i < 13; i++) {
      ThrusterSolver_B.prmName_g[i] = tmp_1[i];
    }

    ThrusterSolver_B.prmName_g[13] = '\x00';
    ParamGet_ThrusterSolver_148.initParam(&ThrusterSolver_B.prmName_g[0]);
    ParamGet_ThrusterSolver_148.setInitialValue(10L);
    ThrusterSolver_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/System Limit'

    // Start for MATLABSystem: '<Root>/Individual Limit'
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_p.isInitialized = 1;
    for (int32_T i{0}; i < 15; i++) {
      ThrusterSolver_B.prmName_c[i] = tmp_2[i];
    }

    ThrusterSolver_B.prmName_c[15] = '\x00';
    ParamGet_ThrusterSolver_147.initParam(&ThrusterSolver_B.prmName_c[0]);
    ParamGet_ThrusterSolver_147.setInitialValue(5L);
    ThrusterSolver_DW.obj_p.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Individual Limit'

    // Start for MATLABSystem: '<Root>/Get Thruster Weights'
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_d.isInitialized = 1;
    for (int32_T i{0}; i < 22; i++) {
      ThrusterSolver_B.prmName_p[i] = tmp_3[i];
    }

    ThrusterSolver_B.prmName_p[22] = '\x00';
    varargin_1 = 0.0;
    ParamGet_ThrusterSolver_146.initParam(&ThrusterSolver_B.prmName_p[0]);
    ParamGet_ThrusterSolver_146.setInitialValue(&varargin_1, 1U);
    ThrusterSolver_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/Get Thruster Weights'

    // Start for MATLABSystem: '<Root>/Force To RPM Curve Parameters'
    ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted = false;
    ThrusterSolver_DW.obj_a.isInitialized = 1;
    for (int32_T i{0}; i < 29; i++) {
      ThrusterSolver_B.prmName_b[i] = tmp_4[i];
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
  if (!ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_e.matlabCodegenIsDeleted = true;
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
  if (!ThrusterSolver_DW.obj.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/System Limit'

  // Terminate for MATLABSystem: '<Root>/Individual Limit'
  if (!ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_p.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Individual Limit'

  // Terminate for MATLABSystem: '<Root>/Get Thruster Weights'
  if (!ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_d.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Get Thruster Weights'

  // Terminate for MATLABSystem: '<Root>/Force To RPM Curve Parameters'
  if (!ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_a.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<Root>/Force To RPM Curve Parameters'

  // Terminate for Triggered SubSystem: '<Root>/Publish To ROS'
  // Terminate for MATLABSystem: '<S11>/SinkBlock'
  if (!ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_l.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S11>/SinkBlock'

  // Terminate for MATLABSystem: '<S10>/SinkBlock'
  if (!ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted) {
    ThrusterSolver_DW.obj_db.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S10>/SinkBlock'
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
