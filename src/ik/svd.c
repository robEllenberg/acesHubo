/*
 * svd.c
 *
 * Embedded MATLAB Coder code generation for M-function 'svd'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "svd.h"
#include "max.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static real_T m_b_eml_xdotc(int32_T eml_n, const real_T eml_x[324], int32_T
 eml_ix0, const real_T eml_y[324], int32_T eml_iy0);
static real_T m_b_eml_xnrm2(int32_T eml_n, const real_T eml_x[21], int32_T
 eml_ix0);
static void m_b_refp1_eml_blas_xrot(real_T eml_x[324], int32_T eml_ix0, int32_T
 eml_iy0, real_T eml_c, real_T eml_s);
static void m_b_refp2_eml_blas_xswap(int32_T eml_n, real_T eml_x[324], int32_T
 eml_ix0, int32_T eml_incx, int32_T eml_iy0, int32_T eml_incy);
static void m_b_refp2_eml_xscal(real_T eml_a, real_T eml_x[441], int32_T
 eml_ix0);
static void m_b_refp3_eml_xscal(int32_T eml_n, real_T eml_a, real_T eml_x[21],
 int32_T eml_ix0);
static void m_b_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[324], int32_T eml_iy0);
static void m_b_refp5_eml_xaxpy(int32_T eml_n, real_T eml_a, const real_T
 eml_x[18], int32_T eml_ix0, real_T eml_y[378], int32_T eml_iy0);
static real_T m_c_eml_xdotc(int32_T eml_n, const real_T eml_x[441], int32_T
 eml_ix0, const real_T eml_y[441], int32_T eml_iy0);
static void m_c_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[441], int32_T eml_iy0);
static real_T m_eml_xdotc(int32_T eml_n, const real_T eml_x[378], int32_T
 eml_ix0, const real_T eml_y[378], int32_T eml_iy0);
static real_T m_eml_xnrm2(int32_T eml_n, const real_T eml_x[378], int32_T
 eml_ix0);
static void m_refp1_eml_blas_xrot(real_T eml_x[441], int32_T eml_ix0, int32_T
 eml_iy0, real_T eml_c, real_T eml_s);
static void m_refp2_eml_blas_xswap(int32_T eml_n, real_T eml_x[441], int32_T
 eml_ix0, int32_T eml_incx, int32_T eml_iy0, int32_T eml_incy);
static void m_refp2_eml_xscal(real_T eml_a, real_T eml_x[324], int32_T eml_ix0);
static void m_refp3_eml_xscal(int32_T eml_n, real_T eml_a, real_T eml_x[378],
 int32_T eml_ix0);
static void m_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[378], int32_T eml_iy0);
static void m_refp5_eml_xaxpy(int32_T eml_n, real_T eml_a, const real_T
 eml_x[378], int32_T eml_ix0, real_T eml_y[18], int32_T eml_iy0);
static void m_refp7_eml_xrotg(real_T *eml_a, real_T *eml_b, real_T *eml_c,
 real_T *eml_s);

/* Function Definitions */
static real_T m_b_eml_xdotc(int32_T eml_n, const real_T eml_x[324], int32_T
 eml_ix0, const real_T eml_y[324], int32_T eml_iy0)
{
  real_T eml_d;
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  eml_d = 0.0;
  if(eml_n < 1) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_d += eml_x[eml_ix - 1] * eml_y[eml_iy - 1];
      eml_ix++;
      eml_iy++;
    }
  }
  return eml_d;
}
static real_T m_b_eml_xnrm2(int32_T eml_n, const real_T eml_x[21], int32_T
 eml_ix0)
{
  real_T eml_y;
  real_T eml_scale;
  boolean_T eml_firstNonZero;
  int32_T eml_kend;
  int32_T eml_k;
  real_T eml_xk;
  real_T eml_t;
  eml_y = 0.0;
  if(eml_n < 1) {
  } else if(eml_n == 1) {
    return fabs(eml_x[eml_ix0 - 1]);
  } else {
    eml_scale = 0.0;
    eml_firstNonZero = true;
    eml_kend = eml_ix0 + (eml_n - 1);
    for(eml_k = eml_ix0; eml_k <= eml_kend; eml_k++) {
      eml_xk = eml_x[eml_k - 1];
      if(eml_xk != 0.0) {
        eml_xk = fabs(eml_xk);
        if(eml_firstNonZero) {
          eml_scale = eml_xk;
          eml_y = 1.0;
          eml_firstNonZero = false;
        } else if(eml_scale < eml_xk) {
          eml_t = eml_scale / eml_xk;
          eml_y = 1.0 + eml_y * eml_t * eml_t;
          eml_scale = eml_xk;
        } else {
          eml_t = eml_xk / eml_scale;
          eml_y += eml_t * eml_t;
        }
      }
    }
    return eml_scale * sqrt(eml_y);
  }
  return eml_y;
}
static void m_b_refp1_eml_blas_xrot(real_T eml_x[324], int32_T eml_ix0, int32_T
 eml_iy0, real_T eml_c, real_T eml_s)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  real_T eml_y;
  real_T eml_b_y;
  eml_ix = eml_ix0;
  eml_iy = eml_iy0;
  for(eml_k = 0; eml_k < 18; eml_k++) {
    eml_y = eml_s * eml_x[eml_iy - 1];
    eml_b_y = eml_c * eml_x[eml_ix - 1];
    eml_x[eml_iy - 1] = eml_c * eml_x[eml_iy - 1] - eml_s * eml_x[eml_ix - 1];
    eml_x[eml_ix - 1] = eml_b_y + eml_y;
    eml_iy++;
    eml_ix++;
  }
  /* Empty Loop. */
}
static void m_b_refp2_eml_blas_xswap(int32_T eml_n, real_T eml_x[324], int32_T
 eml_ix0, int32_T eml_incx, int32_T eml_iy0, int32_T
 eml_incy)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_saturatedUnaryMinus;
  int32_T eml_b_saturatedUnaryMinus;
  int32_T eml_k;
  real_T eml_temp;
  eml_ix = eml_ix0;
  eml_iy = eml_iy0;
  if(eml_incx < 0) {
    if(eml_incx <= MIN_int32_T) {
      eml_saturatedUnaryMinus = MAX_int32_T;
    } else {
      eml_saturatedUnaryMinus = -eml_incx;
    }
  } else {
    eml_saturatedUnaryMinus = eml_incx;
  }
  if(eml_incy < 0) {
    if(eml_incy <= MIN_int32_T) {
      eml_b_saturatedUnaryMinus = MAX_int32_T;
    } else {
      eml_b_saturatedUnaryMinus = -eml_incy;
    }
  } else {
    eml_b_saturatedUnaryMinus = eml_incy;
  }
  for(eml_k = 1; eml_k <= eml_n; eml_k++) {
    eml_temp = eml_x[eml_ix - 1];
    eml_x[eml_ix - 1] = eml_x[eml_iy - 1];
    eml_x[eml_iy - 1] = eml_temp;
    if(eml_incx < 0) {
      eml_ix -= eml_saturatedUnaryMinus;
    } else {
      eml_ix += eml_saturatedUnaryMinus;
    }
    if(eml_incy < 0) {
      eml_iy -= eml_b_saturatedUnaryMinus;
    } else {
      eml_iy += eml_b_saturatedUnaryMinus;
    }
  }
  /* Empty Loop. */
}
static void m_b_refp2_eml_xscal(real_T eml_a, real_T eml_x[441], int32_T eml_ix0)
{
  int32_T eml_loop_ub;
  int32_T eml_k;
  eml_loop_ub = eml_ix0 + 20;
  for(eml_k = eml_ix0; eml_k <= eml_loop_ub; eml_k++) {
    eml_x[eml_k - 1] *= eml_a;
  }
}
static void m_b_refp3_eml_xscal(int32_T eml_n, real_T eml_a, real_T eml_x[21],
 int32_T eml_ix0)
{
  int32_T eml_loop_ub;
  int32_T eml_k;
  eml_loop_ub = eml_ix0 + (eml_n - 1);
  for(eml_k = eml_ix0; eml_k <= eml_loop_ub; eml_k++) {
    eml_x[eml_k - 1] *= eml_a;
  }
}
static void m_b_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[324], int32_T eml_iy0)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  if((eml_n < 1) || (eml_a == 0.0)) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_y[eml_iy - 1] += eml_a * eml_y[eml_ix - 1];
      eml_iy++;
      eml_ix++;
    }
  }
}
static void m_b_refp5_eml_xaxpy(int32_T eml_n, real_T eml_a, const real_T
 eml_x[18], int32_T eml_ix0, real_T eml_y[378], int32_T
 eml_iy0)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  if((eml_n < 1) || (eml_a == 0.0)) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_y[eml_iy - 1] += eml_a * eml_x[eml_ix - 1];
      eml_iy++;
      eml_ix++;
    }
  }
}
static real_T m_c_eml_xdotc(int32_T eml_n, const real_T eml_x[441], int32_T
 eml_ix0, const real_T eml_y[441], int32_T eml_iy0)
{
  real_T eml_d;
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  eml_d = 0.0;
  if(eml_n < 1) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_d += eml_x[eml_ix - 1] * eml_y[eml_iy - 1];
      eml_ix++;
      eml_iy++;
    }
  }
  return eml_d;
}
static void m_c_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[441], int32_T eml_iy0)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  if((eml_n < 1) || (eml_a == 0.0)) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_y[eml_iy - 1] += eml_a * eml_y[eml_ix - 1];
      eml_iy++;
      eml_ix++;
    }
  }
}
static real_T m_eml_xdotc(int32_T eml_n, const real_T eml_x[378], int32_T
 eml_ix0, const real_T eml_y[378], int32_T eml_iy0)
{
  real_T eml_d;
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  eml_d = 0.0;
  if(eml_n < 1) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_d += eml_x[eml_ix - 1] * eml_y[eml_iy - 1];
      eml_ix++;
      eml_iy++;
    }
  }
  return eml_d;
}
static real_T m_eml_xnrm2(int32_T eml_n, const real_T eml_x[378], int32_T
 eml_ix0)
{
  real_T eml_y;
  real_T eml_scale;
  boolean_T eml_firstNonZero;
  int32_T eml_kend;
  int32_T eml_k;
  real_T eml_xk;
  real_T eml_t;
  eml_y = 0.0;
  if(eml_n < 1) {
  } else if(eml_n == 1) {
    return fabs(eml_x[eml_ix0 - 1]);
  } else {
    eml_scale = 0.0;
    eml_firstNonZero = true;
    eml_kend = eml_ix0 + (eml_n - 1);
    for(eml_k = eml_ix0; eml_k <= eml_kend; eml_k++) {
      eml_xk = eml_x[eml_k - 1];
      if(eml_xk != 0.0) {
        eml_xk = fabs(eml_xk);
        if(eml_firstNonZero) {
          eml_scale = eml_xk;
          eml_y = 1.0;
          eml_firstNonZero = false;
        } else if(eml_scale < eml_xk) {
          eml_t = eml_scale / eml_xk;
          eml_y = 1.0 + eml_y * eml_t * eml_t;
          eml_scale = eml_xk;
        } else {
          eml_t = eml_xk / eml_scale;
          eml_y += eml_t * eml_t;
        }
      }
    }
    return eml_scale * sqrt(eml_y);
  }
  return eml_y;
}
static void m_refp1_eml_blas_xrot(real_T eml_x[441], int32_T eml_ix0, int32_T
 eml_iy0, real_T eml_c, real_T eml_s)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  real_T eml_y;
  real_T eml_b_y;
  eml_ix = eml_ix0;
  eml_iy = eml_iy0;
  for(eml_k = 0; eml_k < 21; eml_k++) {
    eml_y = eml_s * eml_x[eml_iy - 1];
    eml_b_y = eml_c * eml_x[eml_ix - 1];
    eml_x[eml_iy - 1] = eml_c * eml_x[eml_iy - 1] - eml_s * eml_x[eml_ix - 1];
    eml_x[eml_ix - 1] = eml_b_y + eml_y;
    eml_iy++;
    eml_ix++;
  }
  /* Empty Loop. */
}
static void m_refp2_eml_blas_xswap(int32_T eml_n, real_T eml_x[441], int32_T
 eml_ix0, int32_T eml_incx, int32_T eml_iy0, int32_T
 eml_incy)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_saturatedUnaryMinus;
  int32_T eml_b_saturatedUnaryMinus;
  int32_T eml_k;
  real_T eml_temp;
  eml_ix = eml_ix0;
  eml_iy = eml_iy0;
  if(eml_incx < 0) {
    if(eml_incx <= MIN_int32_T) {
      eml_saturatedUnaryMinus = MAX_int32_T;
    } else {
      eml_saturatedUnaryMinus = -eml_incx;
    }
  } else {
    eml_saturatedUnaryMinus = eml_incx;
  }
  if(eml_incy < 0) {
    if(eml_incy <= MIN_int32_T) {
      eml_b_saturatedUnaryMinus = MAX_int32_T;
    } else {
      eml_b_saturatedUnaryMinus = -eml_incy;
    }
  } else {
    eml_b_saturatedUnaryMinus = eml_incy;
  }
  for(eml_k = 1; eml_k <= eml_n; eml_k++) {
    eml_temp = eml_x[eml_ix - 1];
    eml_x[eml_ix - 1] = eml_x[eml_iy - 1];
    eml_x[eml_iy - 1] = eml_temp;
    if(eml_incx < 0) {
      eml_ix -= eml_saturatedUnaryMinus;
    } else {
      eml_ix += eml_saturatedUnaryMinus;
    }
    if(eml_incy < 0) {
      eml_iy -= eml_b_saturatedUnaryMinus;
    } else {
      eml_iy += eml_b_saturatedUnaryMinus;
    }
  }
  /* Empty Loop. */
}
static void m_refp2_eml_xscal(real_T eml_a, real_T eml_x[324], int32_T eml_ix0)
{
  int32_T eml_loop_ub;
  int32_T eml_k;
  eml_loop_ub = eml_ix0 + 17;
  for(eml_k = eml_ix0; eml_k <= eml_loop_ub; eml_k++) {
    eml_x[eml_k - 1] *= eml_a;
  }
}
static void m_refp3_eml_xscal(int32_T eml_n, real_T eml_a, real_T eml_x[378],
 int32_T eml_ix0)
{
  int32_T eml_loop_ub;
  int32_T eml_k;
  eml_loop_ub = eml_ix0 + (eml_n - 1);
  for(eml_k = eml_ix0; eml_k <= eml_loop_ub; eml_k++) {
    eml_x[eml_k - 1] *= eml_a;
  }
}
static void m_refp4_eml_xaxpy(int32_T eml_n, real_T eml_a, int32_T eml_ix0,
 real_T eml_y[378], int32_T eml_iy0)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  if((eml_n < 1) || (eml_a == 0.0)) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_y[eml_iy - 1] += eml_a * eml_y[eml_ix - 1];
      eml_iy++;
      eml_ix++;
    }
  }
}
static void m_refp5_eml_xaxpy(int32_T eml_n, real_T eml_a, const real_T
 eml_x[378], int32_T eml_ix0, real_T eml_y[18], int32_T
 eml_iy0)
{
  int32_T eml_ix;
  int32_T eml_iy;
  int32_T eml_k;
  if((eml_n < 1) || (eml_a == 0.0)) {
  } else {
    eml_ix = eml_ix0;
    eml_iy = eml_iy0;
    for(eml_k = 1; eml_k <= eml_n; eml_k++) {
      eml_y[eml_iy - 1] += eml_a * eml_x[eml_ix - 1];
      eml_iy++;
      eml_ix++;
    }
  }
}
static void m_refp7_eml_xrotg(real_T *eml_a, real_T *eml_b, real_T *eml_c,
 real_T *eml_s)
{
  real_T eml_roe;
  real_T eml_absa;
  real_T eml_absb;
  real_T eml_scale;
  real_T eml_ads;
  real_T eml_bds;
  eml_roe = *eml_b;
  eml_absa = fabs(*eml_a);
  eml_absb = fabs(*eml_b);
  if(eml_absa > eml_absb) {
    eml_roe = *eml_a;
  }
  eml_scale = eml_absa + eml_absb;
  if(eml_scale == 0.0) {
    *eml_s = 0.0;
    *eml_c = 1.0;
    eml_scale = 0.0;
    *eml_b = 0.0;
  } else {
    eml_ads = eml_absa / eml_scale;
    eml_bds = eml_absb / eml_scale;
    eml_scale *= sqrt(eml_ads * eml_ads + eml_bds * eml_bds);
    if(eml_roe < 0.0) {
      eml_scale = -eml_scale;
    }
    *eml_c = *eml_a / eml_scale;
    *eml_s = *eml_b / eml_scale;
    if(eml_absa > eml_absb) {
      *eml_b = *eml_s;
    } else if(*eml_c != 0.0) {
      *eml_b = 1.0 / *eml_c;
    } else {
      *eml_b = 1.0;
    }
  }
  *eml_a = eml_scale;
}
void m_svd(real_T eml_A[378], real_T eml_Uout[324], real_T eml_Sout[378], real_T
 eml_Vout[441])
{
  int32_T eml_q;
  real_T eml_s[19];
  real_T eml_e[21];
  real_T eml_work[18];
  int32_T eml_qp1;
  int32_T eml_ii;
  int32_T eml_m;
  int32_T eml_qs;
  real_T eml_ztest0;
  int32_T eml_b_ii;
  int32_T eml_b_q;
  real_T eml_ztest;
  real_T eml_snorm;
  boolean_T eml_exitg2;
  boolean_T eml_exitg1;
  real_T eml_f;
  real_T eml_sn;
  real_T eml_dv1[5];
  real_T eml_scale;
  real_T eml_sm;
  real_T eml_sqds;
  for(eml_q = 0; eml_q < 19; eml_q++) {
    eml_s[eml_q] = 0.0;
  }
  for(eml_q = 0; eml_q < 21; eml_q++) {
    eml_e[eml_q] = 0.0;
  }
  for(eml_q = 0; eml_q < 18; eml_q++) {
    eml_work[eml_q] = 0.0;
  }
  for(eml_q = 0; eml_q < 324; eml_q++) {
    eml_Uout[eml_q] = 0.0;
  }
  for(eml_q = 0; eml_q < 441; eml_q++) {
    eml_Vout[eml_q] = 0.0;
  }
  for(eml_q = 0; eml_q < 18; eml_q++) {
    eml_qp1 = eml_q + 2;
    eml_ii = (eml_q + 1) + 18 * eml_q;
    eml_m = 17 - eml_q;
    eml_qs = eml_m + 1;
    if(eml_q + 1 <= 17) {
      eml_ztest0 = m_eml_xnrm2(eml_qs, eml_A, eml_ii);
      if(eml_ztest0 == 0.0) {
        eml_s[eml_q] = 0.0;
      } else {
        if(eml_A[eml_ii - 1] < 0.0) {
          eml_ztest0 = -eml_ztest0;
        }
        eml_s[eml_q] = eml_ztest0;
        m_refp3_eml_xscal(eml_qs, 1.0 / eml_s[eml_q], eml_A, eml_ii);
        eml_A[eml_ii - 1]++;
        eml_s[eml_q] = -eml_s[eml_q];
      }
    }
    for(eml_b_ii = eml_qp1; eml_b_ii < 22; eml_b_ii++) {
      eml_b_q = (eml_q + 1) + 18 * (eml_b_ii - 1);
      if((eml_q + 1 <= 17) && (eml_s[eml_q] != 0.0)) {
        m_refp4_eml_xaxpy(eml_qs, -(m_eml_xdotc(eml_qs, eml_A, eml_ii, eml_A,
           eml_b_q) / eml_A[eml_q + 18 * eml_q]), eml_ii
         , eml_A, eml_b_q);
      }
      eml_e[eml_b_ii - 1] = eml_A[eml_b_q - 1];
    }
    if(eml_q + 1 <= 17) {
      for(eml_b_ii = eml_q + 1; eml_b_ii < 19; eml_b_ii++) {
        eml_Uout[(eml_b_ii - 1) + 18 * eml_q] = eml_A[(eml_b_ii - 1) + 18 *
          eml_q];
      }
    }
    eml_ii = 20 - eml_q;
    eml_ztest0 = m_b_eml_xnrm2(eml_ii, eml_e, eml_qp1);
    if(eml_ztest0 == 0.0) {
      eml_e[eml_q] = 0.0;
    } else {
      if(eml_e[eml_qp1 - 1] < 0.0) {
        eml_ztest0 = -eml_ztest0;
      }
      eml_e[eml_q] = eml_ztest0;
      m_b_refp3_eml_xscal(eml_ii, 1.0 / eml_e[eml_q], eml_e, eml_qp1);
      eml_e[eml_qp1 - 1]++;
    }
    eml_e[eml_q] = -eml_e[eml_q];
    if((eml_qp1 <= 18) && (eml_e[eml_q] != 0.0)) {
      for(eml_b_ii = eml_qp1; eml_b_ii < 19; eml_b_ii++) {
        eml_work[eml_b_ii - 1] = 0.0;
      }
      for(eml_b_ii = eml_qp1; eml_b_ii < 22; eml_b_ii++) {
        m_refp5_eml_xaxpy(eml_m, eml_e[eml_b_ii - 1], eml_A, eml_qp1 + 18 *
         (eml_b_ii - 1), eml_work, eml_qp1);
      }
      for(eml_b_ii = eml_qp1; eml_b_ii < 22; eml_b_ii++) {
        m_b_refp5_eml_xaxpy(eml_m, (-eml_e[eml_b_ii - 1]) / eml_e[eml_qp1 - 1],
         eml_work, eml_qp1, eml_A, eml_qp1 + 18 * (
          eml_b_ii - 1));
      }
    }
    while(eml_qp1 < 22) {
      eml_Vout[(eml_qp1 - 1) + 21 * eml_q] = eml_e[eml_qp1 - 1];
      eml_qp1++;
    }
  }
  eml_m = 19;
  eml_s[17] = eml_A[323];
  eml_s[18] = 0.0;
  eml_e[18] = 0.0;
  for(eml_b_ii = 0; eml_b_ii < 18; eml_b_ii++) {
    eml_Uout[306 + eml_b_ii] = 0.0;
  }
  eml_Uout[323] = 1.0;
  for(eml_q = 17; eml_q > 0; eml_q += -1) {
    eml_qs = 19 - eml_q;
    eml_ii = eml_q + 18 * (eml_q - 1);
    if(eml_s[eml_q - 1] != 0.0) {
      for(eml_b_ii = eml_q + 1; eml_b_ii < 19; eml_b_ii++) {
        eml_b_q = eml_q + 18 * (eml_b_ii - 1);
        m_b_refp4_eml_xaxpy(eml_qs, -(m_b_eml_xdotc(eml_qs, eml_Uout, eml_ii,
           eml_Uout, eml_b_q) / eml_Uout[eml_ii - 1]),
         eml_ii, eml_Uout, eml_b_q);
      }
      for(eml_b_ii = eml_q; eml_b_ii < 19; eml_b_ii++) {
        eml_Uout[(eml_b_ii - 1) + 18 * (eml_q - 1)] = -eml_Uout[(eml_b_ii - 1) +
          18 * (eml_q - 1)];
      }
      eml_Uout[eml_ii - 1]++;
      eml_b_ii = eml_q - 1;
      for(eml_qs = 1; eml_qs <= eml_b_ii; eml_qs++) {
        eml_Uout[(eml_qs - 1) + 18 * (eml_q - 1)] = 0.0;
      }
    } else {
      for(eml_b_ii = 0; eml_b_ii < 18; eml_b_ii++) {
        eml_Uout[eml_b_ii + 18 * (eml_q - 1)] = 0.0;
      }
      eml_Uout[eml_ii - 1] = 1.0;
    }
  }
  for(eml_b_q = 21; eml_b_q > 0; eml_b_q += -1) {
    if((eml_b_q <= 18) && (eml_e[eml_b_q - 1] != 0.0)) {
      eml_qp1 = eml_b_q + 1;
      eml_ii = 21 - eml_b_q;
      eml_b_ii = eml_qp1 + 21 * (eml_b_q - 1);
      for(eml_qs = eml_qp1; eml_qs < 22; eml_qs++) {
        eml_q = eml_qp1 + 21 * (eml_qs - 1);
        m_c_refp4_eml_xaxpy(eml_ii, -(m_c_eml_xdotc(eml_ii, eml_Vout, eml_b_ii,
           eml_Vout, eml_q) / eml_Vout[eml_b_ii - 1]),
         eml_b_ii, eml_Vout, eml_q);
      }
    }
    for(eml_b_ii = 0; eml_b_ii < 21; eml_b_ii++) {
      eml_Vout[eml_b_ii + 21 * (eml_b_q - 1)] = 0.0;
    }
    eml_Vout[(eml_b_q - 1) + 21 * (eml_b_q - 1)] = 1.0;
  }
  for(eml_b_ii = 0; eml_b_ii < 19; eml_b_ii++) {
    if(eml_s[eml_b_ii] != 0.0) {
      eml_ztest = fabs(eml_s[eml_b_ii]);
      eml_ztest0 = eml_s[eml_b_ii] / eml_ztest;
      eml_s[eml_b_ii] = eml_ztest;
      if(eml_b_ii + 1 < 19) {
        eml_e[eml_b_ii] /= eml_ztest0;
      }
      if(eml_b_ii + 1 <= 18) {
        m_refp2_eml_xscal(eml_ztest0, eml_Uout, 1 + 18 * eml_b_ii);
      }
    }
    if((eml_b_ii + 1 < 19) && (eml_e[eml_b_ii] != 0.0)) {
      eml_ztest = fabs(eml_e[eml_b_ii]);
      eml_ztest0 = eml_ztest / eml_e[eml_b_ii];
      eml_e[eml_b_ii] = eml_ztest;
      eml_s[eml_b_ii + 1] *= eml_ztest0;
      m_b_refp2_eml_xscal(eml_ztest0, eml_Vout, 1 + 21 * (eml_b_ii + 1));
    }
  }
  eml_b_q = 0;
  eml_snorm = 0.0;
  for(eml_b_ii = 0; eml_b_ii < 19; eml_b_ii++) {
    eml_ztest0 = fabs(eml_s[eml_b_ii]);
    eml_ztest = fabs(eml_e[eml_b_ii]);
    eml_ztest0 = rt_MAXd_snf(eml_ztest0, eml_ztest);
    eml_snorm = rt_MAXd_snf(eml_snorm, eml_ztest0);
  }
  while((eml_m > 0) && (!(eml_b_q >= 75))) {
    eml_ii = eml_m - 1;
    eml_exitg2 = 0U;
    while(!((eml_exitg2 == 1U) || (eml_ii == 0))) {
      eml_ztest0 = fabs(eml_e[eml_ii - 1]);
      if((eml_ztest0 <= 2.2204460492503131E-016 * (fabs(eml_s[eml_ii - 1]) +
         fabs(eml_s[eml_ii]))) || (eml_ztest0 <=
        1.0020841800044864E-292) || ((eml_b_q > 20) && (eml_ztest0 <=
         2.2204460492503131E-016 * eml_snorm))) {
        eml_e[eml_ii - 1] = 0.0;
        eml_exitg2 = 1U;
      } else {
        eml_ii += -1;
      }
    }
    if(eml_ii == eml_m - 1) {
      eml_b_ii = 4;
    } else {
      eml_qs = eml_m;
      eml_b_ii = eml_m;
      eml_exitg1 = 0U;
      while((eml_exitg1 == 0U) && (eml_b_ii >= eml_ii)) {
        eml_qs = eml_b_ii;
        if(eml_b_ii == eml_ii) {
          eml_exitg1 = 1U;
        } else {
          eml_ztest0 = 0.0;
          if(eml_b_ii < eml_m) {
            eml_ztest0 = fabs(eml_e[eml_b_ii - 1]);
          }
          if(eml_b_ii > eml_ii + 1) {
            eml_ztest0 += fabs(eml_e[eml_b_ii - 2]);
          }
          eml_ztest = fabs(eml_s[eml_b_ii - 1]);
          if((eml_ztest <= 2.2204460492503131E-016 * eml_ztest0) || (eml_ztest
            <= 1.0020841800044864E-292)) {
            eml_s[eml_b_ii - 1] = 0.0;
            eml_exitg1 = 1U;
          } else {
            eml_b_ii += -1;
          }
        }
      }
      if(eml_qs == eml_ii) {
        eml_b_ii = 3;
      } else if(eml_qs == eml_m) {
        eml_b_ii = 1;
      } else {
        eml_b_ii = 2;
        eml_ii = eml_qs;
      }
    }
    eml_ii++;
    switch(eml_b_ii) {
     case 1:
      eml_f = eml_e[eml_m - 2];
      eml_e[eml_m - 2] = 0.0;
      for(eml_b_ii = eml_m - 1; eml_b_ii >= eml_ii; eml_b_ii += -1) {
        eml_ztest0 = eml_s[eml_b_ii - 1];
        m_refp7_eml_xrotg(&eml_ztest0, &eml_f, &eml_ztest, &eml_sn);
        eml_s[eml_b_ii - 1] = eml_ztest0;
        if(eml_b_ii > eml_ii) {
          eml_qs = eml_b_ii - 1;
          eml_f = (-eml_sn) * eml_e[eml_qs - 1];
          eml_e[eml_qs - 1] *= eml_ztest;
        }
        m_refp1_eml_blas_xrot(eml_Vout, 1 + 21 * (eml_b_ii - 1), 1 + 21 * (eml_m
          - 1), eml_ztest, eml_sn);
      }
      break;
     case 2:
      eml_b_ii = eml_ii - 1;
      eml_f = eml_e[eml_b_ii - 1];
      eml_e[eml_b_ii - 1] = 0.0;
      while(eml_ii <= eml_m) {
        eml_ztest0 = eml_s[eml_ii - 1];
        m_refp7_eml_xrotg(&eml_ztest0, &eml_f, &eml_ztest, &eml_sn);
        eml_s[eml_ii - 1] = eml_ztest0;
        eml_f = (-eml_sn) * eml_e[eml_ii - 1];
        eml_e[eml_ii - 1] *= eml_ztest;
        m_b_refp1_eml_blas_xrot(eml_Uout, 1 + 18 * (eml_ii - 1), 1 + 18 *
         (eml_b_ii - 1), eml_ztest, eml_sn);
        eml_ii++;
      }
      break;
     case 3:
      eml_b_ii = eml_m - 1;
      eml_dv1[0] = fabs(eml_s[eml_m - 1]);
      eml_dv1[1] = fabs(eml_s[eml_b_ii - 1]);
      eml_dv1[2] = fabs(eml_e[eml_b_ii - 1]);
      eml_dv1[3] = fabs(eml_s[eml_ii - 1]);
      eml_dv1[4] = fabs(eml_e[eml_ii - 1]);
      eml_scale = m_max(eml_dv1);
      eml_sm = eml_s[eml_m - 1] / eml_scale;
      eml_ztest0 = eml_s[eml_b_ii - 1] / eml_scale;
      eml_ztest = eml_e[eml_b_ii - 1] / eml_scale;
      eml_sqds = eml_s[eml_ii - 1] / eml_scale;
      eml_sn = ((eml_ztest0 + eml_sm) * (eml_ztest0 - eml_sm) + eml_ztest *
        eml_ztest) / 2.0;
      eml_ztest0 = eml_sm * eml_ztest;
      eml_ztest0 *= eml_ztest0;
      eml_ztest = 0.0;
      if((eml_sn != 0.0) || (eml_ztest0 != 0.0)) {
        eml_ztest = sqrt(eml_sn * eml_sn + eml_ztest0);
        if(eml_sn < 0.0) {
          eml_ztest = -eml_ztest;
        }
        eml_ztest = eml_ztest0 / (eml_sn + eml_ztest);
      }
      eml_f = (eml_sqds + eml_sm) * (eml_sqds - eml_sm) + eml_ztest;
      eml_ztest0 = eml_sqds * (eml_e[eml_ii - 1] / eml_scale);
      for(eml_qs = eml_ii; eml_qs <= eml_b_ii; eml_qs++) {
        eml_q = eml_qs + 1;
        m_refp7_eml_xrotg(&eml_f, &eml_ztest0, &eml_sm, &eml_scale);
        if(eml_qs > eml_ii) {
          eml_e[eml_qs - 2] = eml_f;
        }
        eml_ztest0 = eml_scale * eml_e[eml_qs - 1];
        eml_ztest = eml_sm * eml_s[eml_qs - 1];
        eml_e[eml_qs - 1] = eml_sm * eml_e[eml_qs - 1] - eml_scale *
          eml_s[eml_qs - 1];
        eml_sn = eml_s[eml_q - 1];
        eml_s[eml_q - 1] *= eml_sm;
        m_refp1_eml_blas_xrot(eml_Vout, 1 + 21 * (eml_qs - 1), 1 + 21 * eml_qs,
         eml_sm, eml_scale);
        eml_ztest += eml_ztest0;
        eml_ztest0 = eml_scale * eml_sn;
        m_refp7_eml_xrotg(&eml_ztest, &eml_ztest0, &eml_sn, &eml_sm);
        eml_s[eml_qs - 1] = eml_ztest;
        eml_f = eml_sn * eml_e[eml_qs - 1] + eml_sm * eml_s[eml_q - 1];
        eml_s[eml_q - 1] = (-eml_sm) * eml_e[eml_qs - 1] + eml_sn * eml_s[eml_q
          - 1];
        eml_ztest0 = eml_sm * eml_e[eml_q - 1];
        eml_e[eml_q - 1] *= eml_sn;
        if(eml_qs < 18) {
          m_b_refp1_eml_blas_xrot(eml_Uout, 1 + 18 * (eml_qs - 1), 1 + 18 *
           eml_qs, eml_sn, eml_sm);
        }
      }
      eml_e[eml_m - 2] = eml_f;
      eml_b_q++;
      break;
     default:
      if(eml_s[eml_ii - 1] < 0.0) {
        eml_s[eml_ii - 1] = -eml_s[eml_ii - 1];
        m_b_refp2_eml_xscal(-1.0, eml_Vout, 1 + 21 * (eml_ii - 1));
      }
      for(eml_qp1 = eml_ii + 1; (eml_ii < 19) && (eml_s[eml_ii - 1] <
        eml_s[eml_qp1 - 1]); eml_qp1++) {
        eml_ztest = eml_s[eml_ii - 1];
        eml_s[eml_ii - 1] = eml_s[eml_qp1 - 1];
        eml_s[eml_qp1 - 1] = eml_ztest;
        m_refp2_eml_blas_xswap(21, eml_Vout, 1 + 21 * (eml_ii - 1), 1, 1 + 21 *
         eml_ii, 1);
        if(eml_ii < 18) {
          m_b_refp2_eml_blas_xswap(18, eml_Uout, 1 + 18 * (eml_ii - 1), 1, 1 +
           18 * eml_ii, 1);
        }
        eml_ii = eml_qp1;
      }
      eml_b_q = 0;
      eml_m--;
      break;
    }
  }
  for(eml_q = 0; eml_q < 378; eml_q++) {
    eml_Sout[eml_q] = 0.0;
  }
  for(eml_b_ii = 0; eml_b_ii < 18; eml_b_ii++) {
    eml_Sout[eml_b_ii + 18 * eml_b_ii] = eml_s[eml_b_ii];
  }
}
/* End of Embedded MATLAB Coder code generation (svd.c) */
