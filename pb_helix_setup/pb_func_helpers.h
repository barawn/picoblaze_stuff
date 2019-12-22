#ifndef PB_FUNC_HELPERS_H
#define PB_FUNC_HELPERS_H

#include "cpp_helpers.h"

/*
 * These helper macros create the illusion of a callable function
 * with parameters. The convention is that functions use sA/sB/sC
 * as their parameters.
 */

#define PAR0_sAsA EXISTS()
#define PAR1_sBsB EXISTS()
#define PAR2_sCsC EXISTS()

#define FUNC1( par0, func )					       	  \
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR0_sA, par0)))) ( , sA = par0; ) \
    func()

#define FUNC2( par0, par1, func )                                       \
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR0_sA, par0)))) ( , sA = par0; ) \
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR1_sB, par1)))) ( , sB = par1; ) \
  func()

#define FUNC3( par0, par1, par2, func )					\
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR0_sA, par0)))) ( , sA = par0; ) \
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR1_sB, par1)))) ( , sB = par1; ) \
  IF (DOES_VALUE_EXIST(TEST_EXISTS(CAT(PAR2_sC, par2)))) ( , sC = par2; ) \
  func()

#endif
