#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NLOOP_FOR_ESTIMATION 1000000000UL
#define NANOSECS_PER_MILLISEC 1000000UL
#define NANOSECS_PER_SEC 1000000000UL

static inline long time_nsecs(struct timespec time)
{
    return time.tv_sec * NANOSECS_PER_SEC + time.tv_nsec;
}

static inline long timediff_nsecs(struct timespec before,
    struct timespec after)
{
    return time_nsecs(after) - time_nsecs(before);
}

static unsigned long loops_per_msec()
{
    unsigned long i;
    struct timespec before, after;

    clock_gettime(CLOCK_MONOTONIC, &before);
    for (i = 0; i < NLOOP_FOR_ESTIMATION; i++)
        ;
    clock_gettime(CLOCK_MONOTONIC, &after);

    int ret;
    return NLOOP_FOR_ESTIMATION * NANOSECS_PER_MILLISEC / timediff_nsecs(before, after);
}

static inline void load(unsigned long nloop)
{
    for (unsigned long i = 0; i < nloop; i++)
        ;
}

static void child_fn(int id, struct timespec* buf, int nrecord,
    unsigned long nloop_per_resol, struct timespec start)
{
    int i;
    for (i = 0; i < nrecord; i++) {
        struct timespec t;

        load(nloop_per_resol);
        clock_gettime(CLOCK_MONOTONIC, &t);
        buf[i] = t;
    }
    for (i = 0; i < nrecord; i++) {
        long elapsed = timediff_nsecs(start, buf[i]) / NANOSECS_PER_MILLISEC;
        int progress = (i + 1) * 100 / nrecord;
        printf("%d,%ld,%d\n", id, elapsed, progress);
    }
    exit(EXIT_SUCCESS);
}

static void parent_fn(int nproc)
{
    int i;
    for (i = 0; i < nproc; i++) {
        wait(NULL);
    }
}

static pid_t* pids;

int main(int argc, char* argv[])
{
    printf("id,elapsed,progress\n");

    int ret = EXIT_FAILURE;
    if (argc < 3) {
        fprintf(stderr, "usage: %s <total[ms]> <resolution[ms]>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int nproc = 2;
    int total = atoi(argv[1]);
    int resol = atoi(argv[2]);
    if (total < 1) {
        fprintf(stderr, "<total>(%d) should be >= 1\n", total);
        exit(EXIT_FAILURE);
    }
    if (resol < 1) {
        fprintf(stderr, "<resol>(%d) should be >= 1\n", resol);
        exit(EXIT_FAILURE);
    }
    if (total % resol != 0) {
        fprintf(stderr, "<total>(%d) should be multiple of <resol>(%d)\n", total, resol);
        exit(EXIT_FAILURE);
    }

    int nrecord = total / resol;
    struct timespec* logbuf = malloc(nrecord * sizeof(struct timespec));
    if (!logbuf) {
        err(EXIT_FAILURE, "malloc(logbuf) failed");
    }

    unsigned long nloop_per_resol = loops_per_msec() * resol;
    pids = malloc(nproc * sizeof(pid_t));
    if (pids == NULL) {
        warn("malloc(pids) failed");
        goto free_logbuf;
    }

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int i, ncreated;
    for (i = 0, ncreated = 0; i < nproc; i++, ncreated++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            goto wait_children;
        } else if (pids[i] == 0) {
            if (i == 1) {
                nice(5);
            }
            child_fn(i, logbuf, nrecord, nloop_per_resol, start);
        }
    }
    ret = EXIT_SUCCESS;

wait_children:
    if (ret == EXIT_FAILURE) {
        for (i = 0; i < ncreated; i++) {
            if (kill(pids[i], SIGINT) < 0) {
                warn("kill(%d) failed", pids[i]);
            }
        }
    }
    for (i = 0; i < ncreated; i++) {
        if (wait(NULL) < 0) {
            warn("wait() failed");
        }
    }
free_pids:
    free(pids);
free_logbuf:
    free(logbuf);
    exit(ret);
}
