#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *val = malloc(sizeof(int));
    val = NULL;
    printf("%d\n", *val);
    free(val);
    return 0;
}