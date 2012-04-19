#ifndef _DUO_MEMORY_H
#define _DUO_MEMORY_H

#include <stddef.h>

extern void *memory;

void *kalloc(size_t size);
void kfree(void *);

#endif // ! _DUO_MEMORY_H
