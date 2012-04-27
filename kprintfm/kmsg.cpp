// This file controls /dev/kmsg (there is no such file yet (as Apr 2012), but it will be in future)
// /dev/kmsg is for kernel messages, as opposed to /dev/console, which is general console
// kprintf is based on kmsg_write

#include <stdint.h>

#include <frees/string.h>

#include "console.h"

#include "kmsg.h"

ssize_t kmsg_write(const void *buf, size_t nbyte){
	static const char kernel_name[] = "[duo] ";

	static bool new_line = true;

	if(nbyte > (SIZE_MAX - 1) / 2){
		nbyte = (SIZE_MAX - 1) / 2;
	}

	const char *cbuf = static_cast<const char *>(buf);

	if(nbyte == 0){
		return 0;
	}

	if(new_line){
		console_write(kernel_name, strlen(kernel_name));
	}

	for(size_t i = 0; i != nbyte; ++i){
		if(cbuf[i] == '\n'){
			console_write(cbuf, i + 1);
			new_line = true;
			return kmsg_write(cbuf + i + 1, nbyte - i - 1);
		}
	}

	console_write(cbuf, nbyte);
	new_line = false;

	return ssize_t(nbyte);
}

int kmsg_fputc(int c){
	kmsg_write(&c, 1);
	return static_cast<unsigned char>(c);
}
