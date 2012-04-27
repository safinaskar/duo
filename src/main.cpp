#include <stdint.h>

#include "../kprintfm/multiboot2.h"

#include "../kprintfm/console.h"
#include "../kprintfm/kprintf.h"

#include "memory.h"

// TODO: Begin of абзац
#include "../kprintfm/io.h"

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

static void debug_keyboard() __attribute__((noreturn));

static void debug_keyboard(){
	kprintf("Welcome to debug_keyboard\n");
	kprintf("State  Scan  Name\n");

	for(;;){
		uint8_t scan = get_scan();
		kprintf("%s  0x%02x  %s\n",
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

extern "C" void kernel_main(uint32_t magic, const void *mbi /* Multiboot Info Structure */) __attribute__((noreturn));

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


	// TODO: абзац до конца
	debug_keyboard();
	for(;;);
}
