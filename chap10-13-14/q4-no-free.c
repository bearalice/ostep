#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *val = malloc(sizeof(int));
    *val = 1;
    printf("the value is: %d\n", *val);
    return 0;
}