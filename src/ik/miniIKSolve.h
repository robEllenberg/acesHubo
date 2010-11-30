/*
 * miniIKSolve.h
 *
 * Embedded MATLAB Coder code generation for M-function 'miniIKSolve'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

#ifndef __MINIIKSOLVE_H__
#define __MINIIKSOLVE_H__
/* Include files */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "rt_MAXd_snf.h"
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "miniIKSolve_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern void miniIKSolve(const real_T eml_goalPosition[12], const real_T eml_initialAngles[21], real_T eml_angles[21]);
#endif
/* End of Embedded MATLAB Coder code generation (miniIKSolve.h) */
