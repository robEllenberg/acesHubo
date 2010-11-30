/*
 * diag.c
 *
 * Embedded MATLAB Coder code generation for M-function 'diag'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "diag.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void m_diag(const real_T eml_v[18], real_T eml_d[324])
{
  int32_T eml_j;
  for(eml_j = 0; eml_j < 324; eml_j++) {
    eml_d[eml_j] = 0.0;
  }
  for(eml_j = 0; eml_j < 18; eml_j++) {
    eml_d[eml_j + 18 * eml_j] = eml_v[eml_j];
  }
}
/* End of Embedded MATLAB Coder code generation (diag.c) */
