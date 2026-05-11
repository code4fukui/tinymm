# tinymm

> 日本語のREADMEはこちらです: [README.ja.md](README.ja.md)

A tiny, dependency-free memory management library.

`tinymm` provides a simple memory allocator that operates on a pre-supplied memory buffer. It uses a bump-pointer for new allocations and can reuse freed blocks for subsequent requests of the exact same size.

## Features

*   **Simple Bump Allocation:** New memory requests are served by advancing a pointer through the main buffer.
*   **Block Reuse:** `tinymm_free` marks a block as available. `tinymm_malloc` can reclaim this block if a new request matches its size exactly.
*   **Fixed Metadata:** A static array tracks all allocated and freed blocks, limiting the total number of distinct allocations.
*   **No Defragmentation:** The library does not coalesce adjacent free blocks.

This makes it suitable for embedded systems or applications with predictable, repetitive memory allocation patterns.

## API

The public API is defined in `[tinymm.h](tinymm.h)`.

```c
void tinymm_init(void* memtop, int memlen, int entrylen);
void* tinymm_malloc(unsigned long len);
void tinymm_free(void* p);
```

*   `tinymm_init(void* memtop, int memlen, int entrylen)`
    Initializes the memory manager with a user-provided buffer (`memtop`), its total size (`memlen`), and the maximum number of allocation entries to track (`entrylen`).

*   `void* tinymm_malloc(unsigned long len)`
    Allocates a block of `len` bytes. It first attempts to reuse a previously freed block of the exact same size. If none is available, it allocates a new block from the main buffer. Returns `NULL` if the entry limit is reached or if there is not enough memory.

*   `void tinymm_free(void* p)`
    Marks a memory block as free, making it available for a future `tinymm_malloc` request of the same size. Memory is not returned to the main buffer pool or the OS.

## Usage

### Add as a Git Submodule

```sh
git submodule add https://github.com/code4fukui/tinymm
git commit -m "Add tinymm submodule"
```

### Example

```c
#include "tinymm.h"
#include <stdio.h>

// Define the memory pool and entry limit
#define MEM_SIZE (1024 * 1024)
#define MAX_ENTRIES 100
char memory_pool[MEM_SIZE];

int main() {
  // Initialize the allocator
  tinymm_init(memory_pool, MEM_SIZE, MAX_ENTRIES);

  // Allocate memory
  char* block1 = tinymm_malloc(1024);
  if (block1) {
    printf("Allocated 1024 bytes at %p\n", block1);
    // Use the memory...
    block1[0] = 'a';
  }

  // Free the block
  tinymm_free(block1);
  printf("Freed block.\n");

  // This allocation will reuse the memory from block1
  char* block2 = tinymm_malloc(1024);
  if (block2) {
    printf("Re-allocated 1024 bytes at %p\n", block2);
  }

  return 0;
}
```

## Building and Testing

To compile and run the included test suite, use Make:

```sh
make
```

This command compiles `tinymm_test.c`, runs the resulting executable, and prints the exit code. The test performs 100 cycles of allocating and freeing 100 memory blocks, validating data integrity throughout the process.

→ An exit code of `0` indicates that all tests passed.

## License

This project is licensed under the MIT License.