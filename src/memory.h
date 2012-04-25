#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>

extern void *memory;

void *kalloc(size_t size);
void kfree(void *);

#endif // ! _MEMORY_H
