#include <stdio.h>
#include <string.h>
#include "../../ex20/dbg.h"
#include <time.h>

#define MEM_SIZE  100000
int main(int argc, char *argv[])
{
    char from[MEM_SIZE] = {'a'};
    char to[MEM_SIZE] = {'c'};
    struct timespec start, end;
    long seconds, nanoseconds;
    double elapsed_ms, elapsed_us;
    int repeat_cnt = 100000;

    // setup the from to have some stuff
    memset(from, 'x', MEM_SIZE);
    // set it to a failure mode
    memset(to, 'y', MEM_SIZE);

    // use normal copy to
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < repeat_cnt; i++)
    {
        memset(from, 'x', MEM_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;

    elapsed_ms = seconds * 1000.0 + nanoseconds / 1e6;
    elapsed_us = seconds * 1e6 + nanoseconds / 1e3;

    printf("memset: Elapsed time: %.3f ms, %.3f us\n", elapsed_ms, elapsed_us);


    // memmove
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < repeat_cnt; i++)
    {
        memmove(to, from, MEM_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;

    elapsed_ms = seconds * 1000.0 + nanoseconds / 1e6;
    elapsed_us = seconds * 1e6 + nanoseconds / 1e3;

    printf("memmove: Elapsed time: %.3f ms, %.3f us\n", elapsed_ms, elapsed_us);

    //
    memset(to, 'y', 1000);

    // memcpy
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < repeat_cnt; i++)
    {
        memcpy(to, from, MEM_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;

    elapsed_ms = seconds * 1000.0 + nanoseconds / 1e6;
    elapsed_us = seconds * 1e6 + nanoseconds / 1e3;

    printf("memcpy: Elapsed time: %.3f ms, %.3f us\n", elapsed_ms, elapsed_us);

    return 0;
}