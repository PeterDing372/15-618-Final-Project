# See LICENSE for license details.

#*****************************************************************************
# lw.S
#-----------------------------------------------------------------------------
#
# Test lw instruction.
#

#include "riscv_test.h"
#include "test_macros.h"

RVTEST_RV64U
RVTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Basic tests
  #-------------------------------------------------------------
# test_1:
    li  TESTNUM, 2;
    # Store a value to the address 0x82000000
    # li t0, tdat # Load the address 0x82000000 into register t0
    la t0, tdat # Load the address 0x82000000 into register t0
    li t1, 123         # Load the value 123 into register t1
    sw t1, 0(t0)       # Store the value from t1 to the address in t0

    # Read the value back from the address 0x82000000
    lw t2, 0(t0)       # Load the value from the address in t0 into t2


  TEST_PASSFAIL

RVTEST_CODE_END

  .data
RVTEST_DATA_BEGIN

  TEST_DATA
tdat:
tdat1:  .word 0x00ff00ff
tdat2:  .word 0xff00ff00
tdat3:  .word 0x0ff00ff0
tdat4:  .word 0xf00ff00f



RVTEST_DATA_END
