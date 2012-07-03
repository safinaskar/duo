#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>

extern void *memory;

void *malloc(size_t size);
void free(void *);

#endif // ! _MEMORY_H
