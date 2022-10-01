#include <stdio.h>
#include <stdlib.h>

typedef struct vector
{
    int *data;
    int size;
    int capacity;
} vector_t;

void vector_insertAtEnd(vector_t *vector, int value)
{
    if (vector == NULL)
    {
        exit(1);
    }
    if (vector->size == vector->capacity)
    {
        vector->capacity = vector->capacity * 2;
        vector->data = (int *)realloc(vector->data, vector->capacity * sizeof(int));
    }
    (vector->data)[vector->size++] = value;
}

void vector_deleteAtEnd(vector_t *vector)
{
    if (vector == NULL)
    {
        exit(1);
    }
    (vector->data)[--(vector->size)] = 0;
    if (vector->size == (int)(vector->capacity / 4))
    {
        vector->capacity = (int)(vector->capacity / 2);
        vector->data = (int *)realloc(vector->data, vector->capacity * sizeof(int));
    }
}

void vector_free(vector_t *vector)
{
    if (vector == NULL)
    {
        exit(1);
    }
    free(vector->data);
    vector->size = 0;
    vector->capacity = 0;
}