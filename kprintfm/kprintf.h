#ifndef _KPRINTF_H
#define _KPRINTF_H

#include <frees/stdarg.h>

int vkprintf(const char *__restrict__ format, va_list ap);
int kprintf(const char *__restrict__ format, ...);

#endif // _KPRINTF_H
