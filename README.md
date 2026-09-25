# VoidOS

VoidOS is a small operating system built from scratch in C and x86 assembly.

## Desktop build

- GRUB Multiboot boot
- 32-bit protected-mode kernel
- GRUB-provided 32-bit linear framebuffer
- Custom graphical desktop
- Windows-like taskbar and Start menu
- Desktop icons
- Draggable windows
- Terminal window
- File Explorer foundation
- About window
- PS/2 mouse and keyboard input
- Reboot and halt commands
- Reproducible ISO build
- Windows launcher executable build

The project is intentionally lightweight. It is a custom OS, not a Linux distribution.

## Build

Install the Linux dependencies from the GitHub Actions workflow, then run:

    make
    make run

The ISO is written to build/voidos.iso.

## Windows launcher

The CI build also produces voidos-launcher.exe. It launches the ISO with qemu-system-i386.exe. QEMU must be installed separately and available in PATH.

The OS itself boots from the ISO. The EXE is a Windows host launcher.

## Windows EXE compatibility

Arbitrary Windows .exe programs do not run inside VoidOS yet. That requires a PE loader and a Win32 compatibility layer. This desktop build leaves that as a separate future subsystem.

## Roadmap

1. Interrupts and IDT
2. Memory manager
3. Processes and scheduler
4. User mode and system calls
5. Filesystem and storage drivers
6. Networking
7. Native application framework
8. PE/Win32 compatibility layer
9. x86_64 long mode
10. Hardware drivers and installer

License: MIT
