#include "io.hpp"

uint8_t inb(uint16_t port){
	uint8_t val;
	asm volatile("inb %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));
	return val;
}

void outb(uint16_t port, uint8_t val){
	asm volatile("outb %[val], %[port]" : : [val] "a"(val), [port] "Nd"(port));
}
