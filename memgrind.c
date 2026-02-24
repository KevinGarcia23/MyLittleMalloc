#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define NUM_RUNS 50
#define NUM_OPS 200

/* Function to get time difference in microseconds */
long time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000000L +
           (end.tv_usec - start.tv_usec);
}

/* ===========================
   Workload A
   malloc 1 byte, free immediately
   =========================== */
void workloadA() {
    for (int i = 0; i < NUM_OPS; i++) {
        char *ptr = malloc(1);
        free(ptr);
    }
}

/* ===========================
   Workload B
   malloc 1000 times, then free 1000 times
   =========================== */
void workloadB() {
    char *ptrs[NUM_OPS];

    for (int i = 0; i < NUM_OPS; i++) {
        ptrs[i] = malloc(1);
    }

    for (int i = 0; i < NUM_OPS; i++) {
        free(ptrs[i]);
    }
}

/* ===========================
   Workload C
   Random malloc/free
   =========================== */
void workloadC() {
    char *ptrs[NUM_OPS] = {0};
    int allocated = 0;

    for (int i = 0; i < NUM_OPS; i++) {
        int action = rand() % 2;

        if (action == 0 && allocated < NUM_OPS) {
            ptrs[allocated++] = malloc(1);
        } else if (allocated > 0) {
            free(ptrs[--allocated]);
        }
    }

    while (allocated > 0) {
        free(ptrs[--allocated]);
    }
}


/* ===========================
   Workload D
   Heavy fragmentation test
   =========================== */
void workloadD() {
    char *ptrs[NUM_OPS] = {0};

    for (int i = 0; i < NUM_OPS; i++) {
        ptrs[i] = malloc(1);
    }

    for (int i = 0; i < NUM_OPS; i += 2) {
        free(ptrs[i]);
    }

    for (int i = 0; i < NUM_OPS; i += 2) {
        ptrs[i] = malloc(1);
    }

    for (int i = 0; i < NUM_OPS; i++) {
        free(ptrs[i]);
    }
}





/* ===========================
   Workload E
   Variable size allocations
   =========================== */
void workloadE() {
    char *ptrs[NUM_OPS];

    for (int i = 0; i < NUM_OPS; i++) {
        int size = (rand() % 12) + 1;
        ptrs[i] = malloc(size);
    }

    for (int i = NUM_OPS - 1; i >= 0; i--) {
        free(ptrs[i]);
    }
}


int main() {
    struct timeval start, end;
    long total;
    srand(42);

    printf("Running memgrind...\n\n");

    /* Workload A */
    total = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        reset_heap();
        gettimeofday(&start, NULL);
        workloadA();
        gettimeofday(&end, NULL);
        total += time_diff(start, end);
    }
    printf("Workload A average: %ld microseconds\n", total / NUM_RUNS);

    /* Workload B */
    total = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        reset_heap();
        gettimeofday(&start, NULL);
        workloadB();
        gettimeofday(&end, NULL);
        total += time_diff(start, end);
    }
    printf("Workload B average: %ld microseconds\n", total / NUM_RUNS);

    /* Workload C */
    total = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        reset_heap();
        gettimeofday(&start, NULL);
        workloadC();
        gettimeofday(&end, NULL);
        total += time_diff(start, end);
    }
    printf("Workload C average: %ld microseconds\n", total / NUM_RUNS);

/* Workload D */
total = 0;
for (int i = 0; i < NUM_RUNS; i++) {
    reset_heap();
    gettimeofday(&start, NULL);
    workloadD();
    gettimeofday(&end, NULL);
    total += time_diff(start, end);
}
printf("Workload D average: %ld microseconds\n", total / NUM_RUNS);

/* Workload E */
total = 0;
for (int i = 0; i < NUM_RUNS; i++) {
    reset_heap();
    gettimeofday(&start, NULL);
    workloadE();
    gettimeofday(&end, NULL);
    total += time_diff(start, end);
}
printf("Workload E average: %ld microseconds\n", total / NUM_RUNS);



    return 0;
}
