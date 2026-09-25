# VoidOS

VoidOS is a small operating system written from scratch in C and x86 assembly.

Current build:
- i386 protected-mode kernel
- GRUB Multiboot boot
- VGA text console
- PS/2 keyboard input
- Built-in command shell
- Reboot and halt commands
- Reproducible ISO build through GitHub Actions

Build locally:
sudo apt install build-essential gcc-multilib binutils grub-pc-bin grub-common xorriso qemu-system-x86
make
make run

The ISO is written to build/voidos.iso.

Shell commands: help, about, clear, echo TEXT, reboot, halt.

Roadmap: interrupts, GDT/IDT, memory management, filesystem, processes, system calls, user programs, framebuffer graphics, networking, and x86_64 support.

License: MIT
