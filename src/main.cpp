#include <stdint.h>

#include "../kprintfm/multiboot2.h"

#include "../kprintfm/console.hpp"
#include "../kprintfm/kprintf.hpp"

#include "../bkerndev/system.hpp"

#include "memory.hpp"

// Begin of bkerndev
static volatile char console_buffer[20000]; // TODO: ready for read(...)ing // TODO: exploit me
static volatile int console_buffer_begin = 0; // TODO: HACK
static volatile int console_buffer_size = 0; // TODO: exploit me
static volatile bool console_force_reading = 0;

#include <frees/string.h>
#include "../kprintfm/io.hpp"
/* TODO: Вырубается ли irq на время его обработки? */
void irq_keyboard(struct regs *){
	/* REAL: other keys (если читать up'ы, то там много интересного, что может помочь отличить клавиши), support shifts, right alt and left alt are different keys etc, support numpad */
	/* REAL: нормальный Ctrl-D, а не Ctrl */
	static const char layout[0x80] = {
		0,    0,    '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\b', 0,
		'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\n', 4,    'a',  's',
		'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '`',  0,    0,    'z',  'x',  'c',  'v',
		'b',  'n',  'm',  ',',  '.',  '/',  0,    '*',  0,    ' ',  0,    0,    0,    0,     0,   0,
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    '-',  0,    '5',  0,     '+'
	};

	static char editing[2000]; // TODO: exploit me
	static int editing_size = 0; // TODO: exploit me

	uint8_t scan = inb(0x60);
	if(!(scan & 0x80)){
		char key = layout[scan & 0x7f];

		switch(key){
			case 0:
				break;
			case 4:
				memcpy(console_buffer + console_buffer_size, editing, editing_size);
				console_buffer_size += editing_size;

				editing_size = 0;

				console_force_reading = true;
				break;
			case '\n':
				memcpy(console_buffer + console_buffer_size, editing, editing_size);
				console_buffer_size += editing_size;

				console_buffer[console_buffer_size] = '\n';
				++console_buffer_size;

				editing_size = 0;

				console_force_reading = true;

				console_write(&key, 1);
				break;
			case '\b':
				if(editing_size != 0){
					--editing_size;

					console_write(&key, 1);
				}
				break;
			default:
				editing[editing_size] = key;
				++editing_size;

				console_write(&key, 1);
				break;
		}
	}
}

// TODO: нормальный read, который бы мог возвращать курсор назад
ssize_t console_read(void *buf, size_t nbyte){
	if(nbyte > console_buffer_size - console_buffer_begin){
		while(!console_force_reading);
		console_force_reading = false;
		if(nbyte > console_buffer_size - console_buffer_begin){
			nbyte = console_buffer_size - console_buffer_begin;
		}
	}

	memcpy(buf, console_buffer + console_buffer_begin, nbyte);
	console_buffer_begin += nbyte;
	return nbyte; // TODO: что если не помещается в nbyte?
}

/// End of bkerndev
/// Begin of fus (fake user space)

void *operator new[](size_t size){
	return malloc(size);
}

void operator delete[](void *ptr){
	return free(ptr);
}

#include <stdio.h>

FILE *stdin;
FILE *stdout;
FILE *stderr;

int putchar(int c){
	char ch = c;
	console_write(&ch, 1);
	return ch;
}

double fabs(double x){
	return x >= 0 ? x : -x;
}

int vfprintf(FILE *, const char *format, va_list ap){
	return vprintf(format, ap);
}

int fprintf(FILE *fp, const char *__restrict__ format, ...){
	va_list ap;
	va_start(ap, format);
	int result = vfprintf(fp, format, ap);
	va_end(ap);

	return result;
}

int fputs(const char *s, FILE *){
	console_write(s, strlen(s));
	return 0;
}

/* TODO: Oh, shi... */
char *fgets(char *s, int, FILE *){
	for(;;){
		char c;
		if(console_read(&c, 1) == 0){
			break;
		}
		if(c == '\n'){
			*s = '\n';
			++s;
			break;
		}
		*s = c;
		++s;
	}
	*s = 0;
	return 0;
}

int main();

void main_kernel_loop() __attribute__((noreturn));

void main_kernel_loop(){
	for(;;){
		kprintf("The program exited, press `Enter' to restart\n");

		char s[100]; // TODO: why?
		fgets(s, 100, stdin);

		main();
	}
}

void exit(int) __attribute__((noreturn));

void exit(int){
	main_kernel_loop();
}

extern "C" void errx(int status, const char *format, ...){
	va_list ap;
	va_start(ap, format);
	fprintf(stderr, format, ap);
	va_end(ap);

	putchar('\n');
	exit(1);
}

/// End of fus

extern "C" void kernel_main(uint32_t magic, const void *mbi /* Multiboot Info Structure */) __attribute__((noreturn));

// TODO: panic должен быть нормальным, в частности, он должен отключать прервания или что-то типа того. чтобы буквы переставали появляться при нажимании клавиш
extern "C" void kernel_main(uint32_t magic, const void *mbi){

	/** Starting **/

	console_init();
	kprintf("Info: starting, welcome to Duo!\n");


	/** Multiboot stuff **/

	if(magic != MULTIBOOT2_BOOTLOADER_MAGIC){
		kprintf("Kernel panic: wrong Multiboot magic\n");
		for(;;);
	}

	{ /* LATER: сделать нормальное управление памятью, и для этого дописать этот блок. Если загрузчик не дал инфы про память, то паниковать */
		const multiboot_tag *tag = (const multiboot_tag *)((const char *)mbi + 8 /* Fixed part of mbi */);

		while(tag->type != MULTIBOOT_TAG_TYPE_END){
			int size = ((int(tag->size) - 1) / MULTIBOOT_TAG_ALIGN + 1) * MULTIBOOT_TAG_ALIGN;

			if(tag->type == MULTIBOOT_TAG_TYPE_MMAP){
				const multiboot_tag_mmap *mmap = (const multiboot_tag_mmap *)tag;
				int es = int(mmap->entry_size);

				for(const char *cent = (const char *)&(mmap->entries); cent != (const char *)mmap + size; cent += es){
					//const multiboot_mmap_entry *ent = (const multiboot_mmap_entry *)cent;
				}
			}

			tag = (const multiboot_tag *)((const char *)tag + size);
		}
	}


	/** Memory (will depend on Multiboot stuff) **/

	memory = (void *)0x100000;


	/** bkerndev **/

	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	irq_install_handler(1, &irq_keyboard);

	__asm__ __volatile__ ("sti"); // bkerndev


	/** FUS **/

	main();
	main_kernel_loop();
}
