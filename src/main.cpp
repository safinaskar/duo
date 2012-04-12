#include <stdint.h>

#include "multiboot.h"

#include "kmesg.h"

// TODO: Begin of абзац
#include "io.h"
#include "console.h"
#include <stdarg.h>
static void console_putchar(char c){
	console_write(&c, 1);
}

static void console_fputs(const char *s){
	if(s == 0){
		s = "(null)";
	}

	while(*s != 0){
		console_putchar(*s);
		++s;
	}
}

static void console_print_num(uint64_t num, int base){
	const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	char result[64]; // LONG_LONG_MAX in binary system

	int size = 0;

	if(base < 2 || base > 36){
		base = 10;
	}

	do{
		result[size] = digits[num % base];
		num /= base;
		++size;
	}while(num != 0);

	for(int i = size - 1; i >= 0; --i){
		console_putchar(result[i]);
	}
}

static void kprintf(const char *format, ...){
	if(format == 0){
		kprintf("Error: kprintf: format is null");
		return;
	}

	console_fputs("[ kernel ] ");

	va_list ap;
	va_start(ap, format);

	while(*format != 0){
		if(*format == '%'){
			++format;
			switch(*format){
				case '%':
					console_putchar('%');
					break;
				case 'c':
					console_putchar((char)va_arg(ap, int));
					break;
				case 's':
					console_fputs(va_arg(ap, const char *));
					break;
				case 'o':
					console_print_num(va_arg(ap, unsigned), 8);
					break;
				case 'd':
					console_print_num(va_arg(ap, int), 10);
					break;
				case 'x':
					console_print_num(va_arg(ap, unsigned), 16);
					break;
				default:
					console_putchar('%');
					console_putchar(*format);
					break;
			}
		}else{
			console_putchar(*format);
		}
		++format;
	}

	va_end(ap);

	console_putchar('\n');
}

//
// Keyboard driver, depends on low level I/O
//

// Клавиатура выдаёт ESC с запозданием. С этим ничего нельзя поделать (если не установить прерывания)

static uint8_t get_scan(){
	static uint8_t prev = 0;
	uint8_t curr;
	while((curr = inb(0x60)) == prev);
	prev = curr;
	return curr;
}

const char *key_names[128] = { // TODO: проверить, добавить новые клавиши // TODO: показывать пробел как Space?
	/*         0x0          0x1          0x2          0x3          0x4          0x5          0x6          0x7          0x8          0x9          0xa          0xb          0xc          0xd          0xe          0xf */
	/* 0x0  */ 0,           "Esc",       "1",         "2",         "3",         "4",         "5",         "6",         "7",         "8",         "9",         "0",         "-",         "=",         "BackSpace", "Tab",
	/* 0x10 */ "Q",         "W",         "E",         "R",         "T",         "Y",         "U",         "I",         "O",         "P",         "[",         "]",         "Enter",     0,           "A",         "S",
	/* 0x20 */ "D",         "F",         "G",         "H",         "J",         "K",         "L",         ";",         "'",         "`",         0,           "\\",        "Z",         "X",         "C",         "V",
	/* 0x30 */ "B",         "N",         "M",         ",",         ".",         "/",         0,           0,           0,           " ",         0,           "F1",        "F2",        "F3",        "F4",        "F5",
	/* 0x40 */ "F6",        "F7",        "F8",        "F9",        0,           0,           0,           0,           0,           0,           0,           0,           0,           0,           0,           0,
	/* 0x50 */ 0,           0,           0,           0,           0,           0,           0,           "F11",       "F12"
};

static void debug_keyboard(){
	kprintf("Welcome to debug_keyboard");
	kprintf("State  Scan  Name");

	for(;;){
		uint8_t scan = get_scan();
		kprintf("%s  0x%x  %s",
			scan & 0x80 ? "Up   " : "Down ",
			scan & 0x7f,
			key_names[scan & 0x7f] == 0 ? "(No)" : key_names[scan & 0x7f]
		);
	}
}

/*static int get_scan_down(){
	int result;
	while((result = get_scan()) & 0x80);
	return result;
}*/

// TODO: End of абзац

extern "C" void kernel_main(uint32_t magic, multiboot_info_t *){
	/* I init console first, because I want to be able to print to it anytime */
	console_init();

	// TODO: абзац до конца
#define K(x) kmesg_write(x, __builtin_strlen(x))
	K("sTarting\n");

	kprintf("Info: starting");
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC){
		kprintf("Warning: Multiboot magic incorrect");
	}
	debug_keyboard();
	for(;;);
}
