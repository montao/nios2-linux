	.file	"asm-offsets.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.text
.Ltext0:
	.section	.text.main,"ax",@progbits
	.align	2
	.global	main
	.type	main, @function
main:
.LFB81:
.LSM0:
.LSM1:
#APP
	
->GENERATED_GBL_DATA_SIZE 80 (sizeof(struct global_data) + 15) & ~15
.LSM2:
	
->GENERATED_BD_INFO_SIZE 32 (sizeof(struct bd_info) + 15) & ~15
.LSM3:
#NO_APP
	mov	r2, zero
	ret	
.LFE81:
	.size	main, .-main
	.section	.debug_frame,"",@progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -4
	.byte	0x1f
	.byte	0xc
	.uleb128 0x1b
	.uleb128 0x0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB81
	.4byte	.LFE81-.LFB81
	.align	2
.LEFDE0:
	.section	.text
.Letext0:
	.section	.debug_line
	.4byte	.LELT0-.LSLT0
.LSLT0:
	.2byte	0x2
	.4byte	.LELTP0-.LASLTP0
.LASLTP0:
	.byte	0x1
	.byte	0x1
	.byte	0xf6
	.byte	0xf5
	.byte	0xa
	.byte	0x0
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x1
	.ascii	"/data/dwesterg/nios2-linux/nios2-linux/toolchain-mmu/x86-lin"
	.ascii	"ux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include"
	.byte	0
	.ascii	"/data/dwesterg/nios2-linux/nios2-linux/u-boot/include"
	.byte	0
	.ascii	"lib"
	.byte	0
	.byte	0x0
	.string	"common.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/types.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"linux/posix_types.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/posix_types.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"linux/types.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/ptrace.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"stdarg.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.string	"ide.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"part.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"flash.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"stddef.h"
	.uleb128 0x1
	.uleb128 0x0
	.uleb128 0x0
	.string	"compiler.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"lmb.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/u-boot.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"command.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"image.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/global_data.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"net.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm/status_led.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"bootstage.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"environment.h"
	.uleb128 0x2
	.uleb128 0x0
	.uleb128 0x0
	.string	"asm-offsets.c"
	.uleb128 0x3
	.uleb128 0x0
	.uleb128 0x0
	.byte	0x0
.LELTP0:
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.Letext0
	.byte	0x0
	.uleb128 0x1
	.byte	0x1
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LSM0
	.byte	0x4
	.uleb128 0x16
	.byte	0x2a
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LSM1
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LSM2
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LSM3
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LFE81
	.byte	0x0
	.uleb128 0x1
	.byte	0x1
.LELT0:
	.section	.debug_info
	.4byte	0x85
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.Ldebug_line0
	.4byte	.LASF10
	.byte	0x1
	.4byte	.LASF11
	.4byte	.LASF12
	.uleb128 0x2
	.4byte	.LASF0
	.byte	0x1
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF1
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF2
	.byte	0x2
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF3
	.byte	0x1
	.byte	0x6
	.uleb128 0x2
	.4byte	.LASF4
	.byte	0x2
	.byte	0x5
	.uleb128 0x3
	.string	"int"
	.byte	0x4
	.byte	0x5
	.uleb128 0x2
	.4byte	.LASF5
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF6
	.byte	0x8
	.byte	0x5
	.uleb128 0x2
	.4byte	.LASF7
	.byte	0x8
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF1
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF8
	.byte	0x4
	.byte	0x5
	.uleb128 0x2
	.4byte	.LASF9
	.byte	0x1
	.byte	0x6
	.uleb128 0x4
	.byte	0x1
	.4byte	.LASF13
	.byte	0x16
	.byte	0x17
	.byte	0x1
	.4byte	0x40
	.4byte	.LFB81
	.4byte	.LFE81
	.byte	0x1
	.byte	0x6b
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x10
	.uleb128 0x6
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.4byte	0x17
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x89
	.4byte	0x71
	.string	"main"
	.4byte	0x0
	.section	.debug_aranges,"",@progbits
	.4byte	0x24
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.2byte	0x0
	.2byte	0x0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.LFB81
	.4byte	.LFE81-.LFB81
	.4byte	0x0
	.4byte	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF11:
	.string	"lib/asm-offsets.c"
.LASF7:
	.string	"long long unsigned int"
.LASF9:
	.string	"char"
.LASF6:
	.string	"long long int"
.LASF12:
	.string	"/data/dwesterg/nios2-linux/nios2-linux/u-boot"
.LASF0:
	.string	"unsigned char"
.LASF4:
	.string	"short int"
.LASF8:
	.string	"long int"
.LASF5:
	.string	"unsigned int"
.LASF1:
	.string	"long unsigned int"
.LASF10:
	.string	"GNU C 4.1.2"
.LASF3:
	.string	"signed char"
.LASF2:
	.string	"short unsigned int"
.LASF13:
	.string	"main"
	.ident	"GCC: (GNU) 4.1.2"
