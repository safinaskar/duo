#include "io.h"

uint8_t inb(uint16_t port){
	uint8_t val;
	asm volatile("inb %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port)); // TODO: какие ещё ключ. слова нужны кроме volatile?
	return val;
}

void outb(uint16_t port, uint8_t val){
	asm volatile("outb %[val], %[port]" : : [val] "a"(val), [port] "Nd"(port));
}
