#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#else
    #include <signal.h>
    #include <sys/resource.h>
    #include <unistd.h>
#endif

bool testArraySize(size_t sizeBytes) {
    const size_t count = sizeBytes; // Bytes to number of ints

    #ifdef _WIN32
        __try {
            int* arr = (int*)malloc(count);
            memset(arr, 0, count); // Now we force the OS to commit pages
            std::cout << "Allocated " << sizeBytes << " bytes" << std::endl;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER){ // Structured Exception Handling
            std::cout << "Heap overflow at " << sizeBytes << " bytes!" << std::endl;
            return false;
        }
    #else
        int* arr  =(int*)malloc(count);
        memset(arr, 0, count);
        std::cout << "Allocated " << sizeBytes  << " bytes" << std::endl;
        return true;
    #endif
}

#ifndef _WIN32
void setup_segfault_handler() {
    struct rlimit core_limit = {0, 0};
    setrlimit(RLIMIT_CORE, &core_limit);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = [](int, siginfo_t*, void*) {
        _exit(EXIT_FAILURE); // Just exit silently
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr); // a POSIX signal	Sent by the OS when your program violates memory access rules
}
#endif


int main() {
    #ifndef _WIN32
        setup_segfault_handler();
    #endif
    
        size_t sizeBytes = 100;
    
        while (true) {
            std::cout << "Trying " << sizeBytes << " bytes..." << std::endl;
            if (!testArraySize(sizeBytes)) {
                std::cout << "Program terminated due to heap overflow." << std::endl;
                break;
            }
            sizeBytes = static_cast<size_t>(sizeBytes * 1.5);
        }
    
        return 0;
    }