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
# .text
    li  TESTNUM, 2;
    # make the into t0 address 0x82000000
    li	t0,65
    slli	t0,t0,0x19

    lw t2, 0(t0)       # Load the value from the address in t0 into t2
    lui	t1, 0x0eadb
    beq t2, t1, continue
    ecall

continue:
    li t3, 1           # Load 1 into t3
    add t3, t3, t3     # Add t3 to itself (1+1)


    

    # sw t1, 0(t0)       # Store the value from t1 to the address in t0

    # Read the value back from the address 0x82000000
    # lw t2, 0(t0)       # Load the value from the address in t0 into t2


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

# .space 4096 # Reserve 4096 bytes of space
# # create a second page here
# page2:
# page2_1:  .word 0x00ff00ff
# page2_2:  .word 0xff00ff00
# page2_3:  .word 0x0ff00ff0
# page2_4:  .word 0xf00ff00f

RVTEST_DATA_END

.section .data2
# create a second page here
page2:
page2_1:  .word 0x00ff00ff
page2_2:  .word 0xff00ff00
page2_3:  .word 0x0ff00ff0
page2_4:  .word 0xf00ff00f

