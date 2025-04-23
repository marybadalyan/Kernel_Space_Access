#include <iostream>
#include <stdio.h>
#include <stdint.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <signal.h>
    #include <setjmp.h>
    
    static sigjmp_buf jump_buffer;

    void handler(int sig) {
        siglongjmp(jump_buffer, 1);
    }
#endif

bool MemoryAccess(uintptr_t start, uintptr_t end, uintptr_t step) {
#ifdef _WIN32
    for (uintptr_t addr = start; addr < end; addr += step) {
        __try {
            volatile unsigned char *p = (unsigned char *)addr;
            unsigned char val = *p;  // Try to read
            printf("Valid at 0x%08lx: 0x%02x\n", addr, val);
        }
        __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) { // Structured Exception Handling
            printf("Segmentation fault (access violation) resolved\n");
            printf("Invalid at 0x%08lx\n", addr);
        }
    }
    return true;
#else
    struct sigaction sa;  // signal handling method struct 
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    sigaction(SIGSEGV, &sa, NULL);

    for (uintptr_t addr = start; addr < end; addr += step) {
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
    char buffer[4];
    printf("starting address of stack 0x%08lx\n",&buffer);
    MemoryAccess((uintptr_t)buffer, (uintptr_t)(buffer + sizeof(buffer)), 1);


    
    // Kernel memory is NOT accessible from user-space
    // so start somewhere after NULL page but before high addresses
    uintptr_t start = 0x0;
    uintptr_t end = 0x6FFFFFFF;  // upper bound of user space (on 32-bit)
    uintptr_t step = 0x2FFFFFFF;     
    MemoryAccess(start, end, step);


    return 0;
}