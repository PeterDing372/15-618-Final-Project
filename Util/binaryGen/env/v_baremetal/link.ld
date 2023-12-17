OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  . = 0x80000000;
  .text.init : { *(.text.init) } 
  /* 8000_1000 */
  . = ALIGN(0x1000);
  .tohost : { *(.tohost) }
  /* 8000_2000 */
  . = ALIGN(0x1000);
  .text : { *(.text) }
  /* 8000_3000 */
  . = ALIGN(0x1000);
  .data : { *(.data) }

  /* 8000_5000 */
  /* . = ALIGN(0x1000); 
  .ptl1 : {*(.ptl1) }
  . = ALIGN(0x1000); 
  .ptl2 : {*(.ptl2) }
  . = ALIGN(0x1000); 
  .ptl3 : {*(.ptl3) } */
  .bss : { *(.bss) }
  _end = .;
}

