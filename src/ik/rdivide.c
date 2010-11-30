/*
 * rdivide.c
 *
 * Embedded MATLAB Coder code generation for M-function 'rdivide'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "rdivide.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void m_rdivide(real_T eml_x, const real_T eml_y[18], real_T eml_z[18])
{
  int32_T eml_i5;
  for(eml_i5 = 0; eml_i5 < 18; eml_i5++) {
    eml_z[eml_i5] = eml_x / eml_y[eml_i5];
  }
}
/* End of Embedded MATLAB Coder code generation (rdivide.c) */
