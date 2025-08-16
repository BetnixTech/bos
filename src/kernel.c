// Simple kernel: writes "Betnix OS" to VGA text mode
void kmain(void) {
    volatile unsigned short *video = (unsigned short*)0xB8000;
    const char *msg = "Betnix OS - bootable demo";
    for (int i = 0; msg[i] != 0; ++i) {
        video[i] = (unsigned short)msg[i] | (0x0F << 8);
    }
    while (1) { } // hang
}
