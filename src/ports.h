#ifndef PORTS_H
#define PORTS_H
#include <stdint.h>
#define KEYBOARD_CTRL 0x64


unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif
