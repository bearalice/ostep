#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int rc;

    rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("hello\n");
        exit(0);
    }
    else
    {
        sleep(3);
        printf("goodbye\n");
    }

    return 0;
}