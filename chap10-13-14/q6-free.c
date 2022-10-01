#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *data = (int *)malloc(100);
    free(data);
    printf("after freeing print data[0]: %d\n", data[0]);
    return 0;
}