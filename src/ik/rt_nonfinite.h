/*
 * rt_nonfinite.h
 *
 * Embedded MATLAB Coder code generation for M-function 'miniIKSolve'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

#ifndef __RT_NONFINITE_H__
#define __RT_NONFINITE_H__

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#include <float.h>
#endif
#include <stddef.h>
#include "rtwtypes.h"

extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
extern void rt_InitInfAndNaN(size_t realSize);
extern boolean_T rtIsInf(real_T value);
extern boolean_T rtIsInfF(real32_T value);
extern boolean_T rtIsNaN(real_T value);
extern boolean_T rtIsNaNF(real32_T value);

#endif
/* End of Embedded MATLAB Coder code generation (rt_nonfinite.h) */
