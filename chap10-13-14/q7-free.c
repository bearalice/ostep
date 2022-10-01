#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *data = (int *)malloc(100);
    free(&data[50]); // why does free(&data[0]) work
    return 0;
}