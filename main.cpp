#include <iostream>
#include <cstdlib>   // for malloc, free
#include <cstring>   // for memset
#ifdef _WIN32
    #include <windows.h>
#else
    #include <signal.h>
    #include <sys/resource.h>
    #include <unistd.h>
#endif

bool testArraySize(size_t sizeMBytes) {
    size_t count = sizeMBytes * 1024 * 1024; // Bytes to number of ints
    // Will not be freeing  allocated memory
    #ifdef _WIN32
        __try {
            int* arr = (int*)malloc(count); 
            memset(arr, 0, count); // Now we force the OS to commit pages
            std::cout << "Allocated " << sizeMBytes << " MB" << std::endl;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER){ // Structured Exception Handling
            std::cout << "Heap overflow at " << sizeMBytes << " MB!" << std::endl;
            return false;
        }
    #else
        int* arr  =(int*)malloc(count);
        memset(arr, 0, count);
        std::cout << "Allocated " << sizeMBytes  << " MB" << std::endl;
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
        std::cerr << "Caught SIGSEGV. Exiting cleanly.\n";
        _exit(EXIT_FAILURE);
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr); // a POSIX signal	Sent by the OS when your program violates memory access rules
}
#endif


int main() {
    #ifndef _WIN32
        setup_segfault_handler();
    #endif
    
        size_t sizeMBytes = 1 ; //since heap is usualy 
    
        while (true) {
            std::cout << "Trying " << sizeMBytes << " MB..." << std::endl;
            if (!testArraySize(sizeMBytes)) {
                std::cout << "Program terminated due to heap overflow." << std::endl;
                break;
            }
            sizeMBytes = static_cast<size_t>(sizeMBytes * 2);
        }
    
        return 0;
    }