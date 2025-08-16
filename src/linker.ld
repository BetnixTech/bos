ENTRY(kmain)

SECTIONS
{
  . = 0x00100000; /* load at 1MB */
  .text : { *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss COMMON) }
}
