#ifndef _KPRINTF_H
#define _KPRINTF_H

#include <frees/stdarg.h>

int vkprintf(const char *__restrict__ format, va_list ap) __attribute__((format(printf, 1, 0)));
int kprintf(const char *__restrict__ format, ...) __attribute__((format(printf, 1, 2)));

#endif // ! _KPRINTF_H
