/*
 * norm.c
 *
 * Embedded MATLAB Coder code generation for M-function 'norm'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "norm.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
real_T m_norm(const real_T eml_x[18])
{
  real_T eml_y;
  real_T eml_scale;
  boolean_T eml_firstNonZero;
  int32_T eml_k;
  real_T eml_xk;
  real_T eml_t;
  eml_y = 0.0;
  eml_scale = 0.0;
  eml_firstNonZero = true;
  for(eml_k = 0; eml_k < 18; eml_k++) {
    eml_xk = eml_x[eml_k];
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
/* End of Embedded MATLAB Coder code generation (norm.c) */
