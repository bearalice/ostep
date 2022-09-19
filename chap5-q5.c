#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int main(void)
{
    int rc, status, pid;

    rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        int ret = wait(NULL);
        printf("Child: %d, return = %d\n", getpid(), ret);
        exit(0);
    }
    else
    {
        pid = wait(&status);
        if (WIFEXITED(status))
            printf("normal termination of child - %d, exit status = %d\n", pid, WEXITSTATUS(status));
        printf("Parent: %d\n", getpid());
    }

    return 0;
}