/*
 * max.c
 *
 * Embedded MATLAB Coder code generation for M-function 'max'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "max.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
real_T m_max(const real_T eml_varargin_1[5])
{
  real_T eml_maxval;
  int32_T eml_itmp;
  int32_T eml_ix;
  boolean_T eml_guard1 = false;
  boolean_T eml_searchingForNonNaN;
  int32_T eml_k;
  boolean_T eml_exitg1;
  eml_maxval = eml_varargin_1[0];
  eml_itmp = 1;
  eml_ix = 1;
  eml_guard1 = false;
  if(rtIsNaN(eml_maxval)) {
    eml_searchingForNonNaN = true;
    eml_k = 2;
    eml_exitg1 = 0U;
    while((eml_exitg1 == 0U) && (eml_k < 6)) {
      eml_ix++;
      if(!rtIsNaN(eml_varargin_1[eml_ix - 1])) {
        eml_maxval = eml_varargin_1[eml_ix - 1];
        eml_itmp = eml_k;
        eml_searchingForNonNaN = false;
        eml_exitg1 = 1U;
      } else {
        eml_k++;
      }
    }
    if(eml_searchingForNonNaN) {
    } else {
      eml_guard1 = true;
    }
  } else {
    eml_guard1 = true;
  }
  if(eml_guard1 == true) {
    for(eml_itmp++; eml_itmp < 6; eml_itmp++) {
      eml_ix++;
      if(eml_varargin_1[eml_ix - 1] > eml_maxval) {
        eml_maxval = eml_varargin_1[eml_ix - 1];
      }
    }
  }
  return eml_maxval;
}
/* End of Embedded MATLAB Coder code generation (max.c) */
