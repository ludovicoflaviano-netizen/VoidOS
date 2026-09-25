#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static size_t row, column;
static uint8_t color = 0x07;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
static void clear(void) {
    for (size_t y=0; y<VGA_HEIGHT; y++)
        for (size_t x=0; x<VGA_WIDTH; x++)
            VGA_MEMORY[y*VGA_WIDTH+x] = ((uint16_t)color << 8) | ' ';
    row=0; column=0;
}
static void scroll(void) {
    if (row < VGA_HEIGHT) return;
    for (size_t y=1; y<VGA_HEIGHT; y++)
        for (size_t x=0; x<VGA_WIDTH; x++)
            VGA_MEMORY[(y-1)*VGA_WIDTH+x] = VGA_MEMORY[y*VGA_WIDTH+x];
    for (size_t x=0; x<VGA_WIDTH; x++)
        VGA_MEMORY[(VGA_HEIGHT-1)*VGA_WIDTH+x] = ((uint16_t)color << 8) | ' ';
    row=VGA_HEIGHT-1;
}
static void putc(char c) {
    if (c=='\n') { column=0; row++; scroll(); return; }
    if (c=='\b') {
        if (column) { column--; VGA_MEMORY[row*VGA_WIDTH+column]=((uint16_t)color<<8)|' '; }
        return;
    }
    VGA_MEMORY[row*VGA_WIDTH+column]=((uint16_t)color<<8)|(uint8_t)c;
    if (++column>=VGA_WIDTH) { column=0; row++; scroll(); }
}
static void print(const char* s) { while (*s) putc(*s++); }

static int same(const char* a, const char* b) {
    while (*a && *a==*b) { a++; b++; }
    return *a == *b;
}
static int prefix(const char* s, const char* p) {
    while (*p) { if (*s++ != *p++) return 0; }
    return 1;
}
static char keyboard_getchar(void) {
    static const char map[] =
        "\0\0331234567890-=" "\b"
        "\tqwertyuiop[]\n"
        "\0asdfghjkl;'\0"
        "\\\zxcvbnm,./\0"
        "*\0 ";
    for (;;) {
        if (!(inb(0x64)&1)) continue;
        uint8_t sc=inb(0x60);
        if (sc&0x80 || sc>=sizeof(map)-1) continue;
        if (map[sc]) return map[sc];
    }
}
static void reboot(void) {
    while (inb(0x64)&2) {}
    outb(0x64,0xFE);
    for (;;) __asm__ volatile ("hlt");
}
static void halt(void) {
    print("\nSystem halted.\n");
    for (;;) __asm__ volatile ("cli; hlt");
}
static void shell(void) {
    char input[128];
    size_t n=0;
    print("VoidOS shell. Type 'help' for commands.\n");
    print("void> ");
    for (;;) {
        char c=keyboard_getchar();
        if (c=='\n') {
            input[n]='\0';
            if (!n) { print("void> "); continue; }
            if (same(input,"help")) {
                print("Commands:\n  help       Show this list\n  about      Show system information\n  clear      Clear the screen\n  echo TEXT  Print TEXT\n  reboot     Restart the machine\n  halt       Stop the CPU\n");
            } else if (same(input,"about")) {
                print("VoidOS 0.1\nA small operating system built from scratch.\nKernel: C + x86 assembly\nBoot: GRUB Multiboot\n");
            } else if (same(input,"clear")) {
                clear();
            } else if (prefix(input,"echo ")) {
                print(input+5); putc('\n');
            } else if (same(input,"reboot")) {
                reboot();
            } else if (same(input,"halt")) {
                halt();
            } else {
                print("Unknown command. Type 'help'.\n");
            }
            n=0; print("void> ");
        } else if (c=='\b') {
            if (n) { n--; putc('\b'); }
        } else if (c>=32 && c<=126 && n<sizeof(input)-1) {
            input[n++]=c; putc(c);
        }
    }
}
void kernel_main(uint32_t magic, uint32_t multiboot_info) {
    (void)multiboot_info;
    clear(); color=0x0F;
    print("========================================\n");
    print("              VOIDOS 0.1               \n");
    print("========================================\n");
    if (magic != 0x2BADB002) { print("Boot error: invalid Multiboot magic.\n"); halt(); }
    print("Kernel initialized. Keyboard ready.\n\n");
    shell();
}
