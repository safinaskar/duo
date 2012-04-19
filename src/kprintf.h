#ifndef _DUO_KPRINTF_H
#define _DUO_KPRINTF_H

#include <frees/stdarg.h>

int vkprintf(const char *__restrict__ format, va_list ap);
int kprintf(const char *__restrict__ format, ...);

#endif // _DUO_KPRINTF_H
