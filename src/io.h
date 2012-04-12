#ifndef _DUO_IO_H
#define _DUO_IO_H

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif // ! _DUO_IO_H
