# Kernel_Space_Access




Stack (per-thread):
Each thread gets its own stack, automatically created by the OS or runtime when the thread is initialized.

Heap (shared across threads):
All threads in a process share the same heap by default.

The heap is a global memory region managed by the process, and any thread can allocate or deallocate memory from it (e.g., via malloc, new, etc.).



Thread Safety on the Heap
Since the heap is shared:

Allocators (like malloc) use locks or lock-free mechanisms to avoid data races.

This can become a performance bottleneck in multithreaded programs with frequent allocations.


"How much heap does the main thread have?"

The answer is:

The entire heap.
Just like any other thread in the process, the main thread has access to the full shared heap.


you can overload (or exhaust) the heap by using a very large malloc. But what actually happens depends on:

How much memory you request

How much virtual memory is available

Whether your OS does lazy allocation (most modern ones do)

What allocator you're using

🔍 What Happens When You malloc a Huge Block?
Case 1: You malloc a big block, but don’t touch it
Most OSes (like Linux or Windows) use lazy allocation. That means:

malloc() may succeed even for 10 GB.

But the OS doesn’t actually commit that memory until you access it (e.g., write to it).

you can overload (or exhaust) the heap by using a very large malloc. But what actually happens depends on:

How much memory you request

How much virtual memory is available

Whether your OS does lazy allocation (most modern ones do)

What allocator you're using

🔍 What Happens When You malloc a Huge Block?
Case 1: You malloc a big block, but don’t touch it
Most OSes (like Linux or Windows) use lazy allocation. That means:

malloc() may succeed even for 10 GB.

But the OS doesn’t actually commit that memory until you access it (e.g., write to it).
only allocates might not even do it ```malloc(size)```;
```memset(p, 0, size); // Now we force the OS to commit pages```


when is SIGSEGV triggered 

Dereferencing nullptr null is never mapped,
Accessing memory outside allocated heap,Accessing uncommitted memory , Write to memory that malloc reserved but OS didn’t commit ,Stack overflow Local recursion or VLAs too big
Use-after-freeIf memory has been unmapped, yes; otherwise silent corruption
Accessing memory via uninitialized pointer If the value happens to be unmapped, yes — otherwise UB
