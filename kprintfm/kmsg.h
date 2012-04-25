#ifndef _KMSG_H
#define _KMSG_H

#include <sys/types.h>
#include <frees/stdarg.h>

ssize_t kmsg_write(const void *buf, size_t nbyte);
int kmsg_fputc(int c);

#endif // ! _KMSG_H
