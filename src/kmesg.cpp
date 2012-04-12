// This file controls /dev/kmesg (there is no such file yet (as Apr 2012), but it will be in future)
// /dev/kmesg is for kernel messages, as opposed to /dev/console, which is general console
// kprintf is based on kmesg_write

#include <libfreestanding/string.h>

#include "console.h"

#include "kmesg.h"

#define MARK "[duo] "

/* Always returns nbyte */
ssize_t kmesg_write(const void *buf, size_t nbyte){
	static bool new_line = true;

	const char *cbuf = (const char *)buf;

	size_t begin = 0; /* Begin of current line */
	size_t i = 0; /* Current symbol */

	if(!new_line){
		goto skip;
	}

	for(; i != nbyte; ++i){
		if(i == begin){
			console_write(MARK, strlen(MARK));
		}

skip:
		if(cbuf[i] == '\n'){
			console_write(cbuf + begin, i - begin + 1);
			begin = i + 1;
		}
	}

	if(begin == nbyte){
		new_line = true;
	}else{
		console_write(cbuf + begin, nbyte - begin);
		new_line = false;
	}

	return nbyte;
}
