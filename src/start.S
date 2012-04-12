#define ASM_FILE
#include "multiboot.h"

#define FLAGS MULTIBOOT_MEMORY_INFO /* TODO: use memory info! */ /* My kernel is ELF, you should link it as ELF */
#define STACK_SIZE 0x4000 /* 16 KiB */

	.align MULTIBOOT_HEADER_ALIGN
	.long MULTIBOOT_HEADER_MAGIC
	.long FLAGS
	.long -(MULTIBOOT_HEADER_MAGIC + FLAGS)

	.globl _start
_start: /* I assume we need not to use underscores (`_') to access C functions, also I assume default entry point is `_start' */
	movl $(stack + STACK_SIZE), %esp
	pushl %ebx /* Multiboot information structure */
	pushl %eax /* Multiboot magic */
	call kernel_main
loop:
	jmp loop /* If kernel_main suddenly returns (this should never happen) */

	.comm stack, STACK_SIZE
