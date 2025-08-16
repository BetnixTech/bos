#ifndef PORTS_H
#define PORTS_H
#include <stdint.h>

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif
