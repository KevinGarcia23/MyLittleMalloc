# MyLittleMalloc
CS 214 – Project 1: My Little Malloc
Spring 2026

Partner 1:
Name: Kerolos Awad
NetID: ka838

Partner 2:
Name: Kevin Garcia
NetID: (Kevin's NetID)

----------------------------------------
Test Plan
----------------------------------------

We tested our allocator using the following approach:

1. Basic Allocation Test:
   Verified that malloc successfully allocates memory blocks of various sizes.

2. Basic Free Test:
   Confirmed that free correctly marks blocks as available and allows them to be reused.

3. Coalescing Test:
   Verified that adjacent free blocks are merged properly to prevent fragmentation.

4. Error Detection Tests:
   - Attempted to free NULL.
   - Attempted to free pointers outside the heap.
   - Attempted double free.
   - Attempted to free non-allocated pointers.
   The allocator correctly detected and reported errors.

5. memtest.c:
   Used to verify that allocated memory maintains correct byte values.

6. memgrind.c:
   Used to measure performance under multiple workloads and confirm stability under repeated allocations and frees.

----------------------------------------
Description of Test Programs
----------------------------------------

memtest.c:
Allocates multiple fixed-size objects, fills them with known values, verifies correctness, and frees them.

memgrind.c:
Runs multiple workloads (A–E) 50 times each and computes average execution time in microseconds.
Workloads include:
- Sequential malloc/free
- Batch allocation then free
- Random allocation/free patterns
- Fragmentation stress test
- Variable-size allocation workload

----------------------------------------
Design Notes
----------------------------------------

- The allocator uses a fixed 4096-byte heap.
- Each block contains an 8-byte header storing size and allocation status.
- Blocks are aligned to 8 bytes.
- Splitting is performed when a block is larger than requested.
- Coalescing is performed with both next and previous blocks.
- Strict pointer validation is implemented in free() to detect invalid frees.
- Heap is reset between memgrind runs to ensure consistent timing.

We tested all programs on the Rutgers iLab machines.
All code compiles with -Wall -Wextra -std=c11.
