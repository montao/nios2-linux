OUTPUT_FORMAT("elf32-littlenios2")
OUTPUT_ARCH(nios2)
ENTRY(_start)
SECTIONS
{
 . = text_base;
 .text :
 {
   arch/nios2/cpu/start.o (.text)
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t*)
   *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
   *(.gnu.linkonce.r*)
 }
 . = ALIGN (4);
 _etext = .;
 PROVIDE (etext = .);
 __u_boot_cmd_start = .;
 .u_boot_cmd :
 {
   *(.u_boot_cmd)
 }
 . = ALIGN(4);
 __u_boot_cmd_end = .;
 _data = .;
 .data :
 {
   *(.data)
   *(.data.*)
   *(.gnu.linkonce.d*)
 }
 . = ALIGN(16);
 _gp = .;
 PROVIDE (gp = .);
 .sdata :
 {
   *(.sdata)
   *(.sdata.*)
   *(.gnu.linkonce.s.*)
 }
 . = ALIGN(4);
 _edata = .;
 PROVIDE (edata = .);
 __bss_start = .;
 .sbss (NOLOAD) :
 {
   *(.sbss)
   *(.sbss.*)
   *(.gnu.linkonce.sb.*)
   *(.scommon)
 }
 . = ALIGN(4);
 .bss (NOLOAD) :
 {
   *(.bss)
   *(.bss.*)
   *(.dynbss)
   *(COMMON)
   *(.scommon)
 }
 . = ALIGN(4);
 __bss_end__ = .;
 PROVIDE (end = .);
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
}
