#ifndef IRQ_H
#define IRQ_H

void irq_install();
void pic_remap(int offset1, int offset2);

#endif
