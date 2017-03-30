#include <asm-generic/vmlinux.lds.h>

OUTPUT_FORMAT("elf32-littlenios2", "elf32-littlenios2", "elf32-littlenios2")

OUTPUT_ARCH(nios)
ENTRY(_start)	/* Defined in head.S */

SECTIONS
{
  . = (CONFIG_MEM_BASE + CONFIG_BOOT_LINK_OFFSET) | CONFIG_KERNEL_REGION_BASE;

  _text = .;
  .text : { *(.text) } = 0
  .rodata : { *(.rodata) *(.rodata.*) }
  _etext = .;

  . = ALIGN(32 / 8);
  .data : { *(.data) }
  . = ALIGN(32 / 8);
  _got = .;
  .got  : { *(.got) _egot = .; *(.got.*) }
  _edata  =  .;

  . = ALIGN(32 / 8);
  __bss_start = .;
  .bss : { *(.bss) *(.sbss) }
  . = ALIGN(32 / 8);
  _ebss = .;
  end = . ;
  _end = . ;

  got_len = (_egot - _got);
}
