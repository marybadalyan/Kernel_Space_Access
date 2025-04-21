#include <iostream>
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <signal.h>
    #include <setjmp.h>
    #include <unistd.h>

    static sigjmp_buf jump_buffer;

    void handler(int sig) {
        siglongjmp(jump_buffer, 1);
    }
#endif

bool MemoryAccess(unsigned long start, unsigned long end, unsigned long step) {
#ifdef _WIN32
    for (unsigned long addr = start; addr < end; addr += step) {
        __try {
            volatile unsigned char *p = (unsigned char *)addr;
            unsigned char val = *p;  // Try to read
            printf("Valid at 0x%08lx: 0x%02x\n", addr, val);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            printf("Seg fault resolved\n");
            printf("Invalid at 0x%08lx\n", addr);
        }
    }
    return true;
#else
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    sigaction(SIGSEGV, &sa, NULL);

    for (unsigned long addr = start; addr < end; addr += step) {
        if (sigsetjmp(jump_buffer, 1) == 0) {
            volatile unsigned char *p = (unsigned char *)addr;
            unsigned char val = *p;  // Try to read
            printf("Valid at 0x%08lx: 0x%02x\n", addr, val);
        } else {
            printf("Seg fault resolved\n");
            printf("Invalid at 0x%08lx\n", addr);
        }
    }
    return true;
#endif
}


int main() {
    // Kernel memory is NOT accessible from user-space
    // so start somewhere after NULL page but before high addresses
    unsigned long start = 0x500;
    unsigned long end = 0x7FFFFFFF;  // upper bound of user space (on 32-bit)
    unsigned long step = 0x2FFFFFF;     
    MemoryAccess(start, end, step);

    return 0;
}