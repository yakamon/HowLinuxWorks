#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#define CACHE_LINE_SIZE 64
#define NLOOP (4 * 1024UL * 1024 * 1024)
#define NSECS_PER_SEC 1000000000UL

static inline long time_nsec(struct timespec t)
{
    return t.tv_sec * NSECS_PER_SEC + t.tv_nsec;
}

static inline long diff_nsec(struct timespec before, struct timespec after)
{
    return (time_nsec(after) - time_nsec(before));
}

int main(int argc, char* argv[])
{
    char* program_name;
    program_name = argv[0];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <size[KB]\n", program_name);
        exit(EXIT_FAILURE);
    }

    register int size;
    size = atoi(argv[1]) * 1024;
    if (!size) {
        fprintf(stderr, "size should be >= 1: %d\n", size);
        exit(EXIT_FAILURE);
    }

    char* buffer;
    buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buffer == (void*)-1) {
        err(EXIT_FAILURE, "mmap() failed");
    }

    struct timespec before, after;
    clock_gettime(CLOCK_MONOTONIC, &before);

    int i;
    for (i = 0; i < NLOOP / (size / CACHE_LINE_SIZE); i++) {
        long j;
        for (j = 0; j < size; j += CACHE_LINE_SIZE) {
            buffer[j] = 0;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &after);

    printf("%d,%f\n", (double)diff_nsec(before, after) / NLOOP);

    if (munmap(buffer, size) == -1) {
        err(EXIT_FAILURE, "munmap() failed");
    }
    exit(EXIT_SUCCESS);
}