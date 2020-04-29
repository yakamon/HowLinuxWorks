#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE (100 * 1024 * 1024)
#define PAGE_SIZE 4096
#define COMMAND_SIZE 4096

static char* p;
static char command[COMMAND_SIZE];

static void child_func(char* p)
{
    snprintf(command, COMMAND_SIZE, "ps -o pid,comm,vsz,rss,maj_flt,min_flt | egrep \"PID|%d\"", getpid());

    puts("");
    printf("*** child ps info before memory access ***:\n");
    fflush(stdout);
    system(command);

    puts("");
    printf("*** free memory info before memory access ***:\n");
    fflush(stdout);
    system("free");

    int i;
    for (i = 0; i < BUFFER_SIZE; i += PAGE_SIZE) {
        p[i] = 0;
    }

    puts("");
    printf("*** child ps info after memory access ***:\n");
    fflush(stdout);
    system(command);

    puts("");
    printf("*** free memory info after memory access ***:\n");
    fflush(stdout);
    system("free");

    exit(EXIT_SUCCESS);
}

static void parent_func(void)
{
    wait(NULL);
    exit(EXIT_SUCCESS);
}

int main(void)
{
    char* buf;
    p = malloc(BUFFER_SIZE);
    if (p == NULL) {
        err(EXIT_FAILURE, "malloc() failed");
    }

    int i;
    for (i = 0; i < BUFFER_SIZE; i += PAGE_SIZE) {
        p[i] = 0;
    }

    printf("*** free memory info before fork ***:\n");
    fflush(stdout);
    system("free");

    pid_t ret;
    ret = fork();
    if (ret == -1) {
        err(EXIT_FAILURE, "fork() failed");
    }
    if (ret == 0) {
        child_func(p);
    } else {
        parent_func();
    }

    err(EXIT_FAILURE, "shouldn't reach here");
}