/*
 * miniIKSolve.c
 *
 * Embedded MATLAB Coder code generation for M-function 'miniIKSolve'
 *
 * C source code generated on: Tue Nov 09 18:21:04 2010
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "miniIKSolve.h"
#include "norm.h"
#include "miniForwardKinematics.h"
#include "diag.h"
#include "rdivide.h"
#include "svd.h"
#include "miniJacobian.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void m_leastSquares(const real_T eml_A[378], const real_T eml_b[18],
 real_T eml_dq[21]);
static void m_solutionStep(const real_T eml_position[18], const real_T
 eml_q0[21], real_T eml_q[21]);

/* Function Definitions */
static void m_leastSquares(const real_T eml_A[378], const real_T eml_b[18],
 real_T eml_dq[21])
{
  int32_T eml_i2;
  real_T eml_b_A[378];
  real_T eml_Di[378];
  real_T eml_V[441];
  real_T eml_S[378];
  real_T eml_U[324];
  real_T eml_b_S[18];
  real_T eml_hoistedExpr[18];
  real_T eml_dv0[324];
  int32_T eml_i3;
  int32_T eml_i4;
  /*  Least Squares */
  for(eml_i2 = 0; eml_i2 < 378; eml_i2++) {
    eml_b_A[eml_i2] = eml_A[eml_i2];
    eml_Di[eml_i2] = 0.0;
  }
  m_svd(eml_b_A, eml_U, eml_S, eml_V);
  for(eml_i2 = 0; eml_i2 < 18; eml_i2++) {
    eml_b_S[eml_i2] = eml_S[19 * eml_i2];
  }
  m_rdivide(1.0, eml_b_S, eml_hoistedExpr);
  m_diag(eml_hoistedExpr, eml_dv0);
  for(eml_i2 = 0; eml_i2 < 18; eml_i2++) {
    for(eml_i3 = 0; eml_i3 < 18; eml_i3++) {
      eml_Di[eml_i3 + 21 * eml_i2] = eml_dv0[eml_i3 + 18 * eml_i2];
    }
  }
  for(eml_i4 = 0; eml_i4 < 21; eml_i4++) {
    for(eml_i2 = 0; eml_i2 < 18; eml_i2++) {
      eml_b_A[eml_i4 + 21 * eml_i2] = 0.0;
      for(eml_i3 = 0; eml_i3 < 21; eml_i3++) {
        eml_b_A[eml_i4 + 21 * eml_i2] += eml_V[eml_i4 + 21 * eml_i3] *
          eml_Di[eml_i3 + 21 * eml_i2];
      }
    }
    for(eml_i2 = 0; eml_i2 < 18; eml_i2++) {
      eml_S[eml_i4 + 21 * eml_i2] = 0.0;
      for(eml_i3 = 0; eml_i3 < 18; eml_i3++) {
        eml_S[eml_i4 + 21 * eml_i2] += eml_b_A[eml_i4 + 21 * eml_i3] *
          eml_U[eml_i2 + 18 * eml_i3];
      }
    }
    eml_dq[eml_i4] = 0.0;
    for(eml_i2 = 0; eml_i2 < 18; eml_i2++) {
      eml_dq[eml_i4] += eml_S[eml_i4 + 21 * eml_i2] * eml_b[eml_i2];
    }
  }
}
static void m_solutionStep(const real_T eml_position[18], const real_T
 eml_q0[21], real_T eml_q[21])
{
  real_T eml_hoistedExpr[18];
  int32_T eml_i1;
  real_T eml_b_position[18];
  real_T eml_b_hoistedExpr[378];
  /*  Single iteration of least squares solution */
  /*  Change here for different methods (crude) */
  m_miniForwardKinematics(eml_q0, eml_hoistedExpr);
  for(eml_i1 = 0; eml_i1 < 18; eml_i1++) {
    eml_b_position[eml_i1] = eml_position[eml_i1] - eml_hoistedExpr[eml_i1];
  }
  m_miniJacobian(eml_q0, eml_b_hoistedExpr);
  m_leastSquares(eml_b_hoistedExpr, eml_b_position, eml_q);
  for(eml_i1 = 0; eml_i1 < 21; eml_i1++) {
    eml_q[eml_i1] += eml_q0[eml_i1];
  }
}
void miniIKSolve(const real_T eml_goalPosition[12], const real_T
 eml_initialAngles[21], real_T eml_angles[21])
{
  int32_T eml_i0;
  real_T eml_position[18];
  real_T eml_tempangles[21];
  real_T eml_j;
  int32_T eml_completedSolution;
  int32_T eml_exitg1;
  real_T eml_hoistedExpr[18];
  real_T eml_b_position[18];
  real_T eml_b_hoistedExpr[378];
  real_T eml_c_hoistedExpr[21];
  /* % mini IK Solver (single step) */
  /*  This takes a point in a trajectory of goal positions and outputs the corresponding */
  /*  configuration angles.  If the solution fails, the output configuration is */
  /*  identical to the input angle. */
  /*  */
  /*  Inputs: */
  /* 	goalPosition - COLUMN vector of end effector constraints in the following order: */
  /* 		[ LeftFoot X,Y,Z, RightFoot X,Y,Z, Left Wrist X,Y,Z, Right Wrist */
  /* 		X,Y,Z]' */
  /* 	 */
  /* 	initialAngles - vector of joint angles in the following order */
  /* 		[HY	LHY	LHR	LHP	LKP	LAP	LAR	RHY	RHR	RHP	RKP	RAP	RAR	LSP	LSR	LSY	LEB */
  /* 		RSP	RSR	RSY	REB]' */
  /*  */
  /*  Tips if something fails: */
  /*  1)An arm or leg may be trying to extend past a singularity. */
  /*  2) There may be a singularity that coincides with some pose...try playing */
  /*  with the parameters a bit to see if it goes away. */
  /*  3) Your unit scale may be off ( */
  /*  4) The solution tolerance is too tight.  Look at the "tol" variable in */
  /*  the IKSolve function.  This is RARELY an issue if everything is correct */
  for(eml_i0 = 0; eml_i0 < 18; eml_i0++) {
    eml_position[eml_i0] = 0.0;
  }
  /*  Leave the angle constraints zero for now, since we can't do anything with */
  /*  them yet. */
  for(eml_i0 = 0; eml_i0 < 12; eml_i0++) {
    eml_position[eml_i0] = eml_goalPosition[eml_i0];
  }
  m_solutionStep(eml_position, eml_initialAngles, eml_angles);
  for(eml_i0 = 0; eml_i0 < 21; eml_i0++) {
    eml_tempangles[eml_i0] = eml_angles[eml_i0];
  }
  eml_j = 0.0;
  eml_completedSolution = 0;
  do {
    eml_exitg1 = 0U;
    if(!(eml_completedSolution != 0)) {
      /*  Single iteration of least squares solution */
      /*  Change here for different methods (crude) */
      m_miniForwardKinematics(eml_tempangles, eml_hoistedExpr);
      for(eml_i0 = 0; eml_i0 < 18; eml_i0++) {
        eml_b_position[eml_i0] = eml_position[eml_i0] - eml_hoistedExpr[eml_i0];
      }
      m_miniJacobian(eml_tempangles, eml_b_hoistedExpr);
      m_leastSquares(eml_b_hoistedExpr, eml_b_position, eml_c_hoistedExpr);
      for(eml_i0 = 0; eml_i0 < 21; eml_i0++) {
        eml_tempangles[eml_i0] += eml_c_hoistedExpr[eml_i0];
      }
      eml_j++;
      m_miniForwardKinematics(eml_tempangles, eml_hoistedExpr);
      for(eml_i0 = 0; eml_i0 < 18; eml_i0++) {
        eml_b_position[eml_i0] = eml_hoistedExpr[eml_i0] - eml_position[eml_i0];
      }
      if(m_norm(eml_b_position) < 0.0005) {
        for(eml_i0 = 0; eml_i0 < 21; eml_i0++) {
          eml_angles[eml_i0] = eml_tempangles[eml_i0];
        }
        eml_completedSolution = 1;
      } else {
        if(eml_j > 10.0) {
          eml_exitg1 = 1U;
        }
      }
    } else {
      eml_exitg1 = 1U;
    }
  } while(eml_exitg1 == 0U);
  if(!(eml_completedSolution != 0)) {
    /* Assign original position if tolerances are too great */
    for(eml_i0 = 0; eml_i0 < 21; eml_i0++) {
      eml_angles[eml_i0] = eml_initialAngles[eml_i0];
    }
  }
}
/* End of Embedded MATLAB Coder code generation (miniIKSolve.c) */
