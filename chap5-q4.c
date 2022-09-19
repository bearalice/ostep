#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int rc;

    rc = fork();
    char *args[] = {"ls", "-l", NULL};
    char *env[] = {NULL};

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        execl("/bin/ls", "ls", "-l", (char *)0);
        execv("/bin/ls", args);
        execle("/bin/ls", "ls", "-l", (char *)0, env);
        execve("/bin/ls", args, env);
        execlp("ls", "ls", "-l", (char *)0);
        execvp("ls", args);
    }
    else
    {
        printf("parent process\n");
    }

    return 0;
}