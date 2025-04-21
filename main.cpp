#include <iostream>
#include <cstdlib>   // for malloc, free
#include <cstring>   // for memset
#ifdef _WIN32
    #include <windows.h>
#else
    #include <signal.h>
    #include <sys/resource.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif

bool testArraySize(size_t sizeMBytes) {
    size_t count = sizeMBytes * 1024 * 1024;

#ifdef _WIN32
    __try {
        int* arr = (int*)malloc(count); 
        memset(arr, 0, count);
        std::cout << "Allocated " << sizeMBytes << " MB" << std::endl;
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        std::cout << "Heap overflow at " << sizeMBytes << " MB!" << std::endl;
        return false;
    }
#else
    int* arr = (int*)malloc(count);
    if (arr == nullptr) { //catch the error before memset so git hub actions wont OOM-kill 
        std::cerr << "malloc failed at " << sizeMBytes << " MB\n";
        return false;
    }
    memset(arr, 0, count);
    std::cout << "Allocated " << sizeMBytes << " MB" << std::endl;
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

void set_memory_limit(size_t limit_MB) { //if exceeded the memory limit used  memset will get SIGSEGV
    struct rlimit rlim;
    rlim.rlim_cur = rlim.rlim_max = limit_MB * 1024 * 1024; // Limit in bytes
    if (setrlimit(RLIMIT_AS, &rlim) != 0) {
        perror("setrlimit failed");
    } else {
        std::cout << "Set virtual memory limit to " << limit_MB << " MB\n";
    }
}
#endif

int main() {
    #ifndef _WIN32
        setup_segfault_handler();
        set_memory_limit(4096); // 4 GB cap for testing
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