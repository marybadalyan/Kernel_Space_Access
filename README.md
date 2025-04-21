### Kernel_Space_Access

This is a simple cross-platform C++ program that probes a range of memory addresses in the kernel vs the stack to explore  accessibllity from user-space. It uses platform-specific techniques to safely handle invalid memory accesses without crashing the program.

**Features**
- Stack inspection: Prints the address of a local stack variable and attempts to read from it byte by byte.

- User-space memory scanning: Iterates over a wide address range to check which memory pages are readable.

- Cross-platform support:

On Windows, uses ```__try / __except``` for structured exception handling.

On Linux/macOS, uses ```sigsetjmp / siglongjmp``` and installs a ```SIGSEGV``` handler.

1. Segmentation Fault (Segfault)
The most common outcome when attempting to directly access kernel space from user space is a segmentation fault. The operating system's memory protection mechanisms will prevent the access and terminate the process to prevent corruption or security vulnerabilities.

This is because the kernel space is protected from direct access by user space processes for security and stability reasons. When you try to access memory regions that belong to the kernel (such as kernel code or kernel data structures), the operating system prevents this by raising a segfault.


To interact with kernel memory from user space, you typically need to use system calls or interfaces provided by the operating system (e.g., through device drivers or specific memory-mapped regions that are mapped into user space).


2. Privilege Escalation or Security Breaches
If a vulnerability exists that allows a user-space process to bypass the kernel's protections, it could potentially escalate its privileges. This could allow the process to access sensitive kernel memory and, in some cases, execute arbitrary kernel code. Such vulnerabilities are often exploited in privilege escalation attacks, potentially granting root access to an attacker.

3. Kernel Panic
A kernel panic could occur if invalid memory is accessed in kernel space and it causes a critical system error that the kernel cannot recover from. This would typically result in a system crash, requiring a reboot. Kernel panics are usually caused by serious bugs in kernel modules or the kernel itself, but improper access to kernel memory could also contribute.

4. Memory Corruption
If a user-space process writes to kernel memory (whether it is intentional or due to a bug), it could corrupt kernel data structures. This could lead to unpredictable behavior, crashes, or even subtle bugs that are hard to detect. Memory corruption in the kernel could affect processes or the entire system's stability.

5. Unintended System Behavior or Crashes
Direct access to kernel space can interfere with the kernel's management of memory, hardware, or other critical resources. This could cause system instability, crashing of user applications, or even the entire system.

6. Resource Leaks or Mismanagement
Improper access to kernel space can result in resource leaks (e.g., memory, file handles) or improper management of system resources. For example, attempting to manipulate kernel structures could lead to inconsistent states, which could degrade system performance over time or cause memory exhaustion.

7. Denial of Service (DoS)
If the kernel is unable to handle improper memory access from user space (e.g., accessing invalid addresses or malformed data), it could lead to a denial of service (DoS) condition. The system might become unresponsive, crash, or fail to allocate resources properly.

8. Security Vulnerabilities (Exploits)
Accessing kernel space may lead to the discovery of vulnerabilities that could be exploited to bypass security mechanisms such as the kernel’s address space layout randomization (ASLR), secure boot, or other protections. Exploiting kernel memory might allow an attacker to run arbitrary code in the kernel, undermining system integrity and security.

9. Unstable Behavior due to Race Conditions
If the memory access from user space is part of an operation that involves synchronization with kernel processes, improper handling could lead to race conditions, where multiple processes try to access the same kernel resource concurrently, resulting in unexpected behavior or crashes.

10. Incorrect Kernel Data Access
Accessing kernel data structures incorrectly (e.g., reading/writing out of bounds or using incorrect offsets) could result in the kernel misinterpreting the data. This might lead to bugs in the system’s operation, such as malfunctioning hardware drivers or incorrect scheduling behavior.

11. Invalid Memory Address Exceptions
If a process tries to access an invalid memory address in kernel space, the operating system may trigger an exception (often a segmentation fault), but if not handled properly, the process may terminate abruptly, leading to unpredictable system behavior.


Since we are only reading invalid memory:

in addresses between ``` 0x0``` and ```0x6FFFFFFF```.

```SIGSEGV``` occurs because the memory you are trying to access is protected by the kernel, and user-space processes are not allowed to access it.

This code handles using sigsetjmp and siglongjmp to recover from the signal and print information about the invalid access.

Other Exceptions (Unlikely in this case):

```SIGBUS``` or other exceptions are less likely in this scenario, but they could happen in specific edge cases (e.g., hardware issues, misaligned memory access) when trying to read invalid addresses. These aren't typically encountered when simply trying to read from kernel-space addresses or protected regions.



## Build Instructions

1. **Clone the repository**:
    ```bash
    git clone https://github.com/marybadalyan/Kernel_Space_Access
    ```

2. **Navigate to the repository**:
    ```bash
    cd Kernel_Space_Access
    ```

3. **Generate build files**:
    ```bash
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
    ```

4. **Build the project**:
    ```bash
    cmake --build build --config Release
    ```

5. **Run the executable** from the build directory:
    ```bash
    ./build/Kernel_Space_Access
    ```

