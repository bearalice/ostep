#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sched.h>

void measure_cost_syscall(int nloops)
{
    char *filepath = "dummy";
    int fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < nloops; i++)
    {
        read(fd, NULL, 0);
    }
    gettimeofday(&end, NULL);
    printf("system call: %f us\n", (float)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / nloops);
    close(fd);

    remove(filepath);
}

void measure_cost_context_switch(int nloops)
{
    struct timeval start, end;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    int first_pipefd[2], second_pipefd[2];
    if (pipe(first_pipefd) == -1)
    {
        perror("Failed to call pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(second_pipefd) == -1)
    {
        perror("Failed to call pipe");
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1)
        {
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < nloops; i++)
        {
            read(first_pipefd[0], NULL, 0);
            write(second_pipefd[1], NULL, 0);
        }
    }
    else
    {
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1)
        {
            exit(EXIT_FAILURE);
        }

        gettimeofday(&start, NULL);
        for (size_t i = 0; i < nloops; i++)
        {
            write(first_pipefd[1], NULL, 0);
            read(second_pipefd[0], NULL, 0);
        }
        gettimeofday(&end, NULL);
        printf("context switch: %f us\n", (float)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / nloops);
    }
}

int main(int argc, char *argv[])
{
    measure_cost_syscall(1000000);
    measure_cost_context_switch(1000000);
    return 0;
}
