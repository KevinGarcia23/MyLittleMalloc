CS 214 – Project 1: My Little Malloc
Spring 2026

Partner 1:
Name: Kerolos Awad
NetID: ka838

Partner 2:
Name: Kevin Garcia
NetID: Kag408

This is our test Plan

We tested our allocator using the following approach:

1. Basic Allocation Test:
   We Verified that malloc successfully allocates memory blocks of various sizes.

2. Basic Free Test:
   We Confirmed that free correctly marks blocks as available and allows them to be reused.

3. Combine Test:
   We finalized and Verified that adjacent free blocks are merged properly to prevent any fragmentation.

4. Error Detection Tests:
   - We Attempted to free NULL.
   - We Attempted to free pointers outside the heap.
   - We Attempted double free.
   - We also Attempted to free non-allocated pointers. The allocator correctly detected and reported errors.

5. memtest.c:
   We used memtest.c to verify that allocated memory maintains correct byte values.

6. memgrind.c:
   And we used memgrind.c to measure performance under multiple workloads and confirm stability under repeated allocations and frees.

This our Description of the Test Programs

memtest.c:
This Allocates multiple fixed size objects, fills them with known values, verifies correctness, and frees them.

memgrind.c:
Runs multiple workloads from A to E 50 times each and computes average execution time in microseconds.
Workloads that includes:
Sequential malloc/free
Batch allocation then free
Random allocation/free patterns
Fragmentation stress test
Variable size allocation workload

This is our Design Notes and thought process 

-The allocator uses a fixed 4096-byte heap.
- Each block contains an 8-byte header storing size and allocation status.
- Blocks are aligned to 8 bytes.
- Splitting is performed when a block is larger than requested.
- Combine is performed with both next and previous blocks.
- Strict pointer validation is implemented in free() to detect invalid frees.
- Heap is reset between memgrind runs to ensure consistent timing.

We tested all programs on the Rutgers iLab machines.
All our codes compiles with -Wall -Wextra -std=c11.
