#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
#define ALIGNMENT 8

/* --------------------------
   Heap Declaration (Required)
   -------------------------- */

static union {
    char bytes[MEMLENGTH];
    double not_used;   // ensures 8-byte alignment
} heap;

/* --------------------------
   Header Encoding
   We store:
     - size (multiple of 8)
     - allocation bit in LSB
   -------------------------- */

#define ALLOCATED 1
#define FREE 0

/* Mask out lowest 3 bits (alignment bits) */
#define SIZE_MASK (~0x7)

/* --------------------------
   Global Initialization Flag
   -------------------------- */

static int initialized = 0;

/* --------------------------
   Utility Functions
   -------------------------- */

static size_t align(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

static size_t get_size(size_t header) {
    return header & SIZE_MASK;
}

static int is_allocated(size_t header) {
    return header & ALLOCATED;
}

static void set_header(size_t *header_ptr, size_t size, int allocated) {
    *header_ptr = size | allocated;
}

static void *get_payload(size_t *header_ptr) {
    return (void *)((char *)header_ptr + sizeof(size_t));
}

static size_t *get_header(void *payload_ptr) {
    return (size_t *)((char *)payload_ptr - sizeof(size_t));
}

static size_t *next_chunk(size_t *header_ptr) {
    return (size_t *)((char *)header_ptr + get_size(*header_ptr));
}

/* --------------------------
   Leak Detector
   -------------------------- */

static void check_leaks(void) {
    char *ptr = heap.bytes;
    size_t leaked_bytes = 0;
    int leaked_objects = 0;

    while (ptr < heap.bytes + MEMLENGTH) {
        size_t *header = (size_t *)ptr;
        size_t size = get_size(*header);

        if (is_allocated(*header)) {
            leaked_objects++;
            leaked_bytes += size - sizeof(size_t);
        }

        ptr += size;
    }

    if (leaked_objects > 0) {
        fprintf(stderr,
            "mymalloc: %zu bytes leaked in %d objects.\n",
            leaked_bytes,
            leaked_objects);
    }
}

/* --------------------------
   Initialization
   -------------------------- */

static void initialize_heap(void) {
    size_t *first = (size_t *)heap.bytes;

    set_header(first, MEMLENGTH, FREE);

    initialized = 1;

    atexit(check_leaks);
}

/* --------------------------
   malloc Implementation
   -------------------------- */

void *mymalloc(size_t size, char *file, int line) {

    if (!initialized) {
        initialize_heap();
    }

    if (size == 0) {
        return NULL;
    }

    size = align(size);

    size_t total_size = size + sizeof(size_t);

    char *ptr = heap.bytes;

    while (ptr < heap.bytes + MEMLENGTH) {

        size_t *header = (size_t *)ptr;
        size_t chunk_size = get_size(*header);

        if (!is_allocated(*header) && chunk_size >= total_size) {

            size_t remaining = chunk_size - total_size;

            if (remaining >= (sizeof(size_t) + ALIGNMENT)) {
                /* Split chunk */
                set_header(header, total_size, ALLOCATED);

                size_t *new_header =
                    (size_t *)(ptr + total_size);

                set_header(new_header, remaining, FREE);
            } else {
                /* Allocate entire chunk */
                set_header(header, chunk_size, ALLOCATED);
            }

            return get_payload(header);
        }

        ptr += chunk_size;
    }

    fprintf(stderr,
        "malloc: Unable to allocate %zu bytes (%s:%d)\n",
        size,
        file,
        line);

    return NULL;
}

/* --------------------------
   free Implementation
   -------------------------- */

void myfree(void *ptr, char *file, int line) {

    if (!initialized) {
        initialize_heap();
    }

    if (ptr == NULL) {
        fprintf(stderr,
            "free: Inappropriate pointer (%s:%d)\n",
            file,
            line);
        exit(2);
    }

    if (ptr < (void *)heap.bytes ||
        ptr >= (void *)(heap.bytes + MEMLENGTH)) {

        fprintf(stderr,
            "free: Inappropriate pointer (%s:%d)\n",
            file,
            line);
        exit(2);
    }

    size_t *header = get_header(ptr);

    if (!is_allocated(*header)) {
        fprintf(stderr,
            "free: Inappropriate pointer (%s:%d)\n",
            file,
            line);
        exit(2);
    }

    /* Mark free */
    set_header(header, get_size(*header), FREE);

    /* Coalesce with next */
    size_t *next = next_chunk(header);

    if ((char *)next < heap.bytes + MEMLENGTH &&
        !is_allocated(*next)) {

        size_t new_size =
            get_size(*header) + get_size(*next);

        set_header(header, new_size, FREE);
    }

    /* Coalesce with previous */
    char *scan = heap.bytes;
    size_t *prev = NULL;

    while (scan < (char *)header) {
        size_t *curr = (size_t *)scan;
        if (next_chunk(curr) == header) {
            prev = curr;
            break;
        }
        scan += get_size(*curr);
    }

    if (prev && !is_allocated(*prev)) {
        size_t new_size =
            get_size(*prev) + get_size(*header);

        set_header(prev, new_size, FREE);
    }
}