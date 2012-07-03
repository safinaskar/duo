#include "memory.hpp"

void *memory;

void *malloc(size_t size){
	void *result = memory;
	memory = static_cast<char *>(memory) + size;
	return result;
}

void free(void *){
}
