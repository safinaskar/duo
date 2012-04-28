/* bkerndev - Bran's Kernel Development Tutorial */
/* By:   Brandon F. (friesenb@gmail.com) */
/* Desc: Kernel entry point, stack, and Interrupt Service Routines. */
/* */
/* Notes: No warranty expressed or implied. Use at own risk. */
/* */
/* This is the kernel's entry point. We could either call main here, */
/* or we can use this to setup the stack or other nice stuff, like */
/* perhaps setting up the GDT and segments. Please note that interrupts */
/* are disabled at this point: More on interrupts later! */

/* This will set up our new segment registers. We need to do */
/* something special in order to set CS. We do what is called a */
/* far jump. A jump that includes a segment as well as an offset. */
/* This is declared in C as 'extern void gdt_flush();' */
	.globl gdt_flush
gdt_flush:
	.intel_syntax
	lgdt [gp]
	mov %ax, 0x10
	mov %ds, %ax
	mov %es, %ax
	mov %fs, %ax
	mov %gs, %ax
	mov %ss, %ax
	jmp 0x08:flush2
flush2:
	ret

/* Loads the IDT defined in '_idtp' into the processor. */
/* This is declared in C as 'extern void idt_load();' */
	.globl idt_load
idt_load:
	lidt [idtp]
	ret

/* In just a few pages in this tutorial, we will add our Interrupt */
/* Service Routines (ISRs) right here! */
	.globl isr0
	.globl isr1
	.globl isr2
	.globl isr3
	.globl isr4
	.globl isr5
	.globl isr6
	.globl isr7
	.globl isr8
	.globl isr9
	.globl isr10
	.globl isr11
	.globl isr12
	.globl isr13
	.globl isr14
	.globl isr15
	.globl isr16
	.globl isr17
	.globl isr18
	.globl isr19
	.globl isr20
	.globl isr21
	.globl isr22
	.globl isr23
	.globl isr24
	.globl isr25
	.globl isr26
	.globl isr27
	.globl isr28
	.globl isr29
	.globl isr30
	.globl isr31

/*  0: Divide By Zero Exception */
isr0:
	cli
	push 0
	push 0
	jmp isr_common_stub

/*  1: Debug Exception */
isr1:
	cli
	push 0
	push 1
	jmp isr_common_stub

/*  2: Non Maskable Interrupt Exception */
isr2:
	cli
	push 0
	push 2
	jmp isr_common_stub

/*  3: Int 3 Exception */
isr3:
	cli
	push 0
	push 3
	jmp isr_common_stub

/*  4: INTO Exception */
isr4:
	cli
	push 0
	push 4
	jmp isr_common_stub

/*  5: Out of Bounds Exception */
isr5:
	cli
	push 0
	push 5
	jmp isr_common_stub

/*  6: Invalid Opcode Exception */
isr6:
	cli
	push 0
	push 6
	jmp isr_common_stub

/*  7: Coprocessor Not Available Exception */
isr7:
	cli
	push 0
	push 7
	jmp isr_common_stub

/*  8: Double Fault Exception (With Error Code!) */
isr8:
	cli
	push 8
	jmp isr_common_stub

/*  9: Coprocessor Segment Overrun Exception */
isr9:
	cli
	push 0
	push 9
	jmp isr_common_stub

/* 10: Bad TSS Exception (With Error Code!) */
isr10:
	cli
	push 10
	jmp isr_common_stub

/* 11: Segment Not Present Exception (With Error Code!) */
isr11:
	cli
	push 11
	jmp isr_common_stub

/* 12: Stack Fault Exception (With Error Code!) */
isr12:
	cli
	push 12
	jmp isr_common_stub

/* 13: General Protection Fault Exception (With Error Code!) */
isr13:
	cli
	push 13
	jmp isr_common_stub

/* 14: Page Fault Exception (With Error Code!) */
isr14:
	cli
	push 14
	jmp isr_common_stub

/* 15: Reserved Exception */
isr15:
	cli
	push 0
	push 15
	jmp isr_common_stub

/* 16: Floating Point Exception */
isr16:
	cli
	push 0
	push 16
	jmp isr_common_stub

/* 17: Alignment Check Exception */
isr17:
	cli
	push 0
	push 17
	jmp isr_common_stub

/* 18: Machine Check Exception */
isr18:
	cli
	push 0
	push 18
	jmp isr_common_stub

/* 19: Reserved */
isr19:
	cli
	push 0
	push 19
	jmp isr_common_stub

/* 20: Reserved */
isr20:
	cli
	push 0
	push 20
	jmp isr_common_stub

/* 21: Reserved */
isr21:
	cli
	push 0
	push 21
	jmp isr_common_stub

/* 22: Reserved */
isr22:
	cli
	push 0
	push 22
	jmp isr_common_stub

/* 23: Reserved */
isr23:
	cli
	push 0
	push 23
	jmp isr_common_stub

/* 24: Reserved */
isr24:
	cli
	push 0
	push 24
	jmp isr_common_stub

/* 25: Reserved */
isr25:
	cli
	push 0
	push 25
	jmp isr_common_stub

/* 26: Reserved */
isr26:
	cli
	push 0
	push 26
	jmp isr_common_stub

/* 27: Reserved */
isr27:
	cli
	push 0
	push 27
	jmp isr_common_stub

/* 28: Reserved */
isr28:
	cli
	push 0
	push 28
	jmp isr_common_stub

/* 29: Reserved */
isr29:
	cli
	push 0
	push 29
	jmp isr_common_stub

/* 30: Reserved */
isr30:
	cli
	push 0
	push 30
	jmp isr_common_stub

/* 31: Reserved */
isr31:
	cli
	push 0
	push 31
	jmp isr_common_stub


/* We call a C function in here. We need to let the assembler know */
/* that '_fault_handler' exists in another file */

/* This is our common ISR stub. It saves the processor state, sets */
/* up for kernel mode segments, calls the C-level fault handler, */
/* and finally restores the stack frame. */
isr_common_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov %ax, 0x10
	mov %ds, %ax
	mov %es, %ax
	mov %fs, %ax
	mov %gs, %ax
	mov %eax, %esp
	push %eax
	.att_syntax
	mov $fault_handler, %eax
	.intel_syntax
	call %eax
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add %esp, 8
	iret

	.globl irq0
	.globl irq1
	.globl irq2
	.globl irq3
	.globl irq4
	.globl irq5
	.globl irq6
	.globl irq7
	.globl irq8
	.globl irq9
	.globl irq10
	.globl irq11
	.globl irq12
	.globl irq13
	.globl irq14
	.globl irq15

/* 32: IRQ0 */
irq0:
	cli
	push 0
	push 32
	jmp irq_common_stub

/* 33: IRQ1 */
irq1:
	cli
	push 0
	push 33
	jmp irq_common_stub

/* 34: IRQ2 */
irq2:
	cli
	push 0
	push 34
	jmp irq_common_stub

/* 35: IRQ3 */
irq3:
	cli
	push 0
	push 35
	jmp irq_common_stub

/* 36: IRQ4 */
irq4:
	cli
	push 0
	push 36
	jmp irq_common_stub

/* 37: IRQ5 */
irq5:
	cli
	push 0
	push 37
	jmp irq_common_stub

/* 38: IRQ6 */
irq6:
	cli
	push 0
	push 38
	jmp irq_common_stub

/* 39: IRQ7 */
irq7:
	cli
	push 0
	push 39
	jmp irq_common_stub

/* 40: IRQ8 */
irq8:
	cli
	push 0
	push 40
	jmp irq_common_stub

/* 41: IRQ9 */
irq9:
	cli
	push 0
	push 41
	jmp irq_common_stub

/* 42: IRQ10 */
irq10:
	cli
	push 0
	push 42
	jmp irq_common_stub

/* 43: IRQ11 */
irq11:
	cli
	push 0
	push 43
	jmp irq_common_stub

/* 44: IRQ12 */
irq12:
	cli
	push 0
	push 44
	jmp irq_common_stub

/* 45: IRQ13 */
irq13:
	cli
	push 0
	push 45
	jmp irq_common_stub

/* 46: IRQ14 */
irq14:
	cli
	push 0
	push 46
	jmp irq_common_stub

/* 47: IRQ15 */
irq15:
	cli
	push 0
	push 47
	jmp irq_common_stub


irq_common_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs

	mov %ax, 0x10
	mov %ds, %ax
	mov %es, %ax
	mov %fs, %ax
	mov %gs, %ax
	mov %eax, %esp

	push %eax
	.att_syntax
	mov $irq_handler, %eax
	.intel_syntax
	call %eax
	pop %eax

	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add %esp, 8
	iret
	.att_syntax
