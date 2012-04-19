/* My kernel is ELF, you should link it as ELF */

#define ASM_FILE
#include "multiboot2.h"

#define STACK_SIZE 0x4000 /* 16 KiB */
#define U_VIRT 8 /* Multiboot 1.6 spec says that "u_virt is unsigned data of the same size as target architecture virtual address size", i. e. 4 bytes for i386. But GRUB 2.0~beta3 assumes u_virt is 8 bytes for i386, so I use this value */

/*
 * Multiboot header
 */

	.align MULTIBOOT_HEADER_ALIGN
header:
	.long MULTIBOOT2_HEADER_MAGIC
	.long MULTIBOOT_ARCHITECTURE_I386
	.long header_end - header
	.long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (header_end - header))


/*
 * Information request tag
 */

	.align U_VIRT
rq:
	.short MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
	.short 0
	.long rq_end - rq /* GRUB 2.0~beta3 has bug: this .long should be multiply of U_VIRT, so we need to repeat MULTIBOOT_TAG_TYPE_MMAP twice */
	.long MULTIBOOT_TAG_TYPE_MMAP
	.long MULTIBOOT_TAG_TYPE_MMAP
rq_end:


/*
 * Final tag
 */

	.align U_VIRT
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8


/*
 * End of header
 */

header_end:


/*
 * Code
 */

	.globl _start

/* I assume we need not to use underscores (`_') to access C functions, also I assume default entry point is `_start' */
_start:
	movl $(stack + STACK_SIZE), %esp
	pushl %ebx /* Multiboot information structure */
	pushl %eax /* Multiboot magic */
	call kernel_main

loop:
	jmp loop /* If kernel_main suddenly returns (this should never happen) */


/*
 * Stack
 */

	.comm stack, STACK_SIZE
