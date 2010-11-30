/*
 * rtGetNaN.c
 *
 * Embedded MATLAB Coder code generation for M-function 'miniIKSolve'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/*
 * Abstract:
 *       Real-Time Workshop function to initialize non-finite, NaN
 */
#include "rtGetNaN.h"
#define NumBitsPerChar	8

/* Function: rtGetNaN ==================================================
 * Abstract:
 * Initialize rtNaN needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetNaN(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T nan = 0.0;
  if (bitsPerReal == 32) {
    nan = rtGetNaNF();
  } else {
  uint16_T one = 1;
  enum {
    LittleEndian,
    BigEndian
  } machByteOrder = (*((uint8_T *) &one) == 1) ? LittleEndian : BigEndian;
  switch (machByteOrder) {
   case LittleEndian:
    {
      typedef struct {
        struct {
          real32_T wordL;
          real32_T wordH;
        } words;
      } LittleEndianIEEEDouble;

      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      *(uint32_T *)&tmpVal.bitVal.words.wordH = 0xFFF80000;
      *(uint32_T *)&tmpVal.bitVal.words.wordL = 0x00000000;
      nan = tmpVal.fltVal;
      break;
    }

   case BigEndian:
    {
      typedef struct {
        struct {
            real32_T wordH;
            real32_T wordL;
          } words;
        } BigEndianIEEEDouble;

        union {
          BigEndianIEEEDouble bitVal;
          real_T fltVal;
        } tmpVal;

        *(uint32_T *)&tmpVal.bitVal.words.wordH = 0x7FFFFFFF;
        *(uint32_T *)&tmpVal.bitVal.words.wordL = 0xFFFFFFFF;
        nan = tmpVal.fltVal;
        break;
      }
    }
  }

  return nan;
}

/* Function: rtGetNaNF ==================================================
 * Abstract:
 * Initialize rtNaNF needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetNaNF(void)
{
  real32_T nanF = 0.0;
  uint16_T one = 1;
  enum {
    LittleEndian,
    BigEndian
  } machByteOrder = (*((uint8_T *) &one) == 1) ? LittleEndian : BigEndian;
  switch (machByteOrder) {
   case LittleEndian:
    {
      *(uint32_T *)&nanF = 0xFFC00000;
      break;
    }

   case BigEndian:
    {
      *(uint32_T *)&nanF = 0x7FFFFFFF;
      break;
    }
  }

  return nanF;
}

#undef NumBitsPerChar

/* End of Embedded MATLAB Coder code generation (rtGetNaN.c) */
