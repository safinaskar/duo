// This file controls /dev/kmesg (there is no such file yet (as Apr 2012), but it will be in future)
// /dev/kmesg is for kernel messages, as opposed to /dev/console, which is general console
// kprintf is based on kmesg_write

#include <frees/string.h>

#include "console.h"

#include "kmesg.h"

#define MARK "[duo] "

/* Always returns nbyte */
ssize_t kmesg_write(const void *buf, size_t nbyte){
	static bool new_line = true;

	const char *cbuf = static_cast<const char *>(buf);

	if(nbyte == 0){
		return 0;
	}

	if(new_line){
		console_write(MARK, strlen(MARK));
	}

	for(size_t i = 0; i != nbyte; ++i){
		if(cbuf[i] == '\n'){
			console_write(cbuf, i + 1);
			new_line = true;
			return kmesg_write(cbuf + i + 1, nbyte - i - 1);
		}
	}

	console_write(cbuf, nbyte);
	new_line = false;

	return ssize_t(nbyte);
}

int kmesg_fputc(int c){
	kmesg_write(&c, 1);
	return static_cast<unsigned char>(c);
}
