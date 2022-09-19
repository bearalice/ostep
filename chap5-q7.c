#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        close(STDOUT_FILENO);
        printf("child: %d.\n", getpid());
        exit(0);
    }
    else
    {
        printf("Parent: %d\n", getpid());
    }
    return 0;
}