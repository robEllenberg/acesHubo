/*
 * rtGetInf.c
 *
 * Embedded MATLAB Coder code generation for M-function 'miniIKSolve'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/*
 * Abstract:
 *       Real-Time Workshop function to initialize non-finite, Inf and MinusInf
 */
#include "rtGetInf.h"
#define NumBitsPerChar	8

/* Function: rtGetInf ==================================================
 * Abstract:
 * Initialize rtInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T inf = 0.0;
  if (bitsPerReal == 32) {
    inf = rtGetInfF();
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

        *(uint32_T *)&tmpVal.bitVal.words.wordH = 0x7FF00000;
        *(uint32_T *)&tmpVal.bitVal.words.wordL = 0x00000000;
        inf = tmpVal.fltVal;
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

        *(uint32_T *)&tmpVal.bitVal.words.wordH = 0x7FF00000;
        *(uint32_T *)&tmpVal.bitVal.words.wordL = 0x00000000;
        inf = tmpVal.fltVal;
        break;
      }
    }
  }

  return inf;
}

/* Function: rtGetInfF ==================================================
 * Abstract:
 * Initialize rtInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetInfF(void)
{
  real32_T infF = 0.0;
  *(uint32_T *)&infF = 0x7F800000;
  return infF;
}

/* Function: rtGetMinusInf ==================================================
 * Abstract:
 * Initialize rtMinusInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetMinusInf(void)
{
  size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
  real_T minf = 0.0;
  if (bitsPerReal == 32) {
    minf = rtGetMinusInfF();
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

        *(uint32_T *)&tmpVal.bitVal.words.wordH = 0xFFF00000;
        *(uint32_T *)&tmpVal.bitVal.words.wordL = 0x00000000;
        minf = tmpVal.fltVal;
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

        *(uint32_T *)&tmpVal.bitVal.words.wordH = 0xFFF00000;
        *(uint32_T *)&tmpVal.bitVal.words.wordL = 0x00000000;
        minf = tmpVal.fltVal;
        break;
      }
    }
  }

  return minf;
}

/* Function: rtGetMinusInfF ==================================================
 * Abstract:
 * Initialize rtMinusInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetMinusInfF(void)
{
  real32_T minfF = 0.0;
  *(uint32_T *)&minfF = 0xFF800000;
  return minfF;
}

#undef NumBitsPerChar
/* End of Embedded MATLAB Coder code generation (rtGetInf.c) */
