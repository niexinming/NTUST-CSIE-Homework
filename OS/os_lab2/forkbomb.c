__asm__(".global _start; _start:");
#if __x86_64__
__asm__("push 0x39; pop rax; syscall");
#else
__asm__("push 2; pop eax; int 0x80");
#endif
__asm__("jmp _start");
