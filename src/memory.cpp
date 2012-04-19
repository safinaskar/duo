#include "memory.h"

void *memory;

void *kalloc(size_t size){
	void *result = memory;
	memory = static_cast<char *>(memory) + size;
	return result;
}

void kfree(void *){
}
