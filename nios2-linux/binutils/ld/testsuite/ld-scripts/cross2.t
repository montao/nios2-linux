NOCROSSREFS ( .text .data )
SECTIONS
{
  .text : { *(.text) *(.text.*) *(.pr) }
  .data : 
  { *(.data) *(.data.*) 
  	_gp = ABSOLUTE(. + 0x7ff0);
    *(.sdata) *(.rw) *(.tc0) *(.tc) *(.toc) 
   }
}
