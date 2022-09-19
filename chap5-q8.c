#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int rc = fork();
    int status, pid;
    int fd[2];
    char line[1024];

    if (pipe(fd) < 0)
    {
        printf("Pipe err");
        exit(1);
    }

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        if (fork() == 0)
        {
            close(fd[1]);
            printf("Second child: %d\n", getpid());
            int n = read(fd[0], line, 1024);
            write(STDOUT_FILENO, line, n);
            exit(0);
        }
        else
        {
            close(fd[0]);
            write(fd[1], "First child\n", 12);
            exit(0);
        }
    }
    else
    {
        printf("Parent: %d\n", getpid());
    }
    return 0;
}