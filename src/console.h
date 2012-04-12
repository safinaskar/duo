#ifndef _DUO_CONSOLE_H
#define _DUO_CONSOLE_H

#include <sys/types.h>

void console_init();
ssize_t console_write(const void *buf, size_t nbyte);

#endif // ! _DUO_CONSOLE_H
