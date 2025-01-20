# Practice Problem 9.10 (solution page 885)
Describe a reference pattern that results in severe external fragmentation in an allocator based on simple segregated storage.

## Solution:
- ~~Repeatedly allocate $2^i$ ($i = 0, 1, 2, \dots$) bytes~~
- The application makes numerous allocation and free requests of $2^i$ ($i = 0, 1, 2, \dots$) bytes
- We allocate $2^j$ size **multiple times until** the free list for the size class is empty, then the allocator requests additional memory from the operating system. 
- Then we allocate $2^{j + 1}$, although the total block size of the free blocks is enough, but each one is not enough, we have to ask OS for additional memory, and so on.