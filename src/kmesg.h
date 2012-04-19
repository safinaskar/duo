#ifndef _DUO_KMESG_H
#define _DUO_KMESG_H

#include <sys/types.h>
#include <frees/stdarg.h>

ssize_t kmesg_write(const void *buf, size_t nbyte);
int kmesg_fputc(int c);

#endif // ! _DUO_KMESG_H
