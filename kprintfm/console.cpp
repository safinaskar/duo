// This file controls /dev/console (as opposed to /dev/kmsg, which is located in kmsg.c). There is no such files yet (as Apr 2012), but they will be in the future

#include <stdint.h>

#include "io.h"

#include "console.h"

static volatile uint8_t *const screen = reinterpret_cast<volatile uint8_t *>(0xb8000);

static const int rows = 25;
static const int cols = 80;

static int row;
static int col;

static void set_cursor(){
	int pos = row * 80 + col;

	outb(0x3d4, 0x0f);
	outb(0x3d5, uint8_t(pos));
	outb(0x3d4, 0x0e);
	outb(0x3d5, uint8_t(pos >> 8));
}

static void clear(){
	// We cannot use memset, because `screen' is `volatile'
	for(int i = 0; i != rows * cols; ++i){
		screen[2 * i] = ' '; // Qemu has bug: if we write NUL charasters here, this will not actually clear screen in curses mode. So, we need to write spaces
	}

	row = 0;
	col = 0;

	set_cursor();
}

void console_init(){
	// Setting color
	for(int i = 0; i != rows * cols; ++i){
		screen[2 * i + 1] = 0x7; // Light gray on black
	}

	// We need to clear screen at boot time, because Qemu doesn't do this
	clear();
}

static void new_line(){
	++row;
	col = 0;

	if(row == rows){
		for(int i = 0; i != (rows - 1) * cols; ++i){
			screen[2 * i] = screen[2 * (i + cols)];
		}

		for(int i = 0; i != cols; ++i){
			screen[2 * ((rows - 1) * cols + i)] = ' ';
		}

		row = rows - 1;
	}
}

/* Always returns nbyte */
/* REAL: real console, which ignores non-printing charasters */
ssize_t console_write(const void *buf, size_t nbyte){
	if(nbyte > (SIZE_MAX - 1) / 2){
		nbyte = (SIZE_MAX - 1) / 2;
	}

	const char *cbuf = static_cast<const char *>(buf);

	for(size_t i = 0; i != nbyte; ++i){
		switch(cbuf[i]){
			case '\n':
				new_line();
				break;
			case '\b':
				if(col != 0){
					--col;
					screen[2 * (row * cols + col)] = ' ';
				}
				break;
			default:
				screen[2 * (row * cols + col)] = uint8_t(cbuf[i]);
				++col;

				if(col == cols){
					new_line();
				}

				break;
		}
	}

	set_cursor();

	return ssize_t(nbyte);
}
